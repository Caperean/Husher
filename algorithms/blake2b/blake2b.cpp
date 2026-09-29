#include "algorithms.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <cstdint>
#include <cstring>


// ==================== FIX dla MinGW ====================
#if defined(__GNUC__)
typedef unsigned __int128 uint128_t;
#else
#error "Compiler does not support 128-bit integers"
#endif

// ==================== Stałe ====================
static const uint64_t blake2b_IV[8] =
{
    0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL,
    0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
    0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
    0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
};

static const uint8_t blake2b_sigma[12][16] =
{
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},
    {11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4},
    {7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},
    {9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13},
    {2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},
    {12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11},
    {13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},
    {6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5},
    {10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0},
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3}
};

// ==================== Helpery ====================
inline uint64_t rotr64(uint64_t w, unsigned c)
{
    return (w >> c) | (w << (64 - c));
}

inline void G(uint64_t v[16], uint64_t m[16],
              int a,int b,int c,int d,int x,int y)
{
    v[a] += v[b] + m[x];
    v[d] = rotr64(v[d] ^ v[a], 32);
    v[c] += v[d];
    v[b] = rotr64(v[b] ^ v[c], 24);
    v[a] += v[b] + m[y];
    v[d] = rotr64(v[d] ^ v[a], 16);
    v[c] += v[d];
    v[b] = rotr64(v[b] ^ v[c], 63);
}

// ==================== Compress ====================
static void blake2b_compress(uint64_t h[8],
                             const uint8_t block[128],
                             uint128_t t,
                             bool last)
{
    uint64_t v[16];
    uint64_t m[16];

    for(int i=0;i<8;i++) v[i]=h[i];
    for(int i=0;i<8;i++) v[i+8]=blake2b_IV[i];

    v[12] ^= (uint64_t)t;
    v[13] ^= (uint64_t)(t >> 64);

    if(last) v[14] = ~v[14];

    for(int i=0;i<16;i++)
    {
        m[i] =
            ((uint64_t)block[i*8+0]) |
            ((uint64_t)block[i*8+1] << 8) |
            ((uint64_t)block[i*8+2] << 16) |
            ((uint64_t)block[i*8+3] << 24) |
            ((uint64_t)block[i*8+4] << 32) |
            ((uint64_t)block[i*8+5] << 40) |
            ((uint64_t)block[i*8+6] << 48) |
            ((uint64_t)block[i*8+7] << 56);
    }

    for(int i=0;i<12;i++)
    {
        G(v,m,0,4,8,12,blake2b_sigma[i][0],blake2b_sigma[i][1]);
        G(v,m,1,5,9,13,blake2b_sigma[i][2],blake2b_sigma[i][3]);
        G(v,m,2,6,10,14,blake2b_sigma[i][4],blake2b_sigma[i][5]);
        G(v,m,3,7,11,15,blake2b_sigma[i][6],blake2b_sigma[i][7]);
        G(v,m,0,5,10,15,blake2b_sigma[i][8],blake2b_sigma[i][9]);
        G(v,m,1,6,11,12,blake2b_sigma[i][10],blake2b_sigma[i][11]);
        G(v,m,2,7,8,13,blake2b_sigma[i][12],blake2b_sigma[i][13]);
        G(v,m,3,4,9,14,blake2b_sigma[i][14],blake2b_sigma[i][15]);
    }

    for(int i=0;i<8;i++)
        h[i] ^= v[i] ^ v[i+8];
}

// ==================== Hasher ====================
class Blake2b
{
public:
    Blake2b()
    {
        for(int i=0;i<8;i++) h[i]=blake2b_IV[i];
        h[0] ^= 0x01010040;
        t = 0;
        buflen = 0;
    }

    void update(const uint8_t* data,size_t len)
    {
        while(len > 0)
        {
            size_t take = std::min(len, 128 - buflen);
            memcpy(buf + buflen, data, take);

            buflen += take;
            data += take;
            len -= take;

            if(buflen == 128)
            {
                t += 128;
                blake2b_compress(h, buf, t, false);
                buflen = 0;
            }
        }
    }

    void final(uint8_t out[64])
    {
        t += buflen;
        memset(buf + buflen, 0, 128 - buflen);
        blake2b_compress(h, buf, t, true);

        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                out[i*8+j] = (h[i] >> (8*j)) & 0xFF;
    }

private:
    uint64_t h[8];
    uint128_t t;
    uint8_t buf[128];
    size_t buflen;
};

// ==================== API ====================
QString hashBLAKE2b(const QString &filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot open file:" << filePath;
        return QString();
    }

    Blake2b ctx;

    while(!file.atEnd())
    {
        QByteArray chunk = file.read(4096);
        ctx.update(reinterpret_cast<const uint8_t*>(chunk.constData()), chunk.size());
    }

    uint8_t hash[64];
    ctx.final(hash);

    QString result;
    for(int i=0;i<64;i++)
        result += QString("%1").arg(hash[i],2,16,QChar('0')).toUpper();

    return result;
}
