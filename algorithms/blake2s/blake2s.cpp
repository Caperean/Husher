#include "algorithms.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QString>
#include <cstdint>
#include <cstring>
#include <vector>

// ==================== Implementacja Blake2s ====================
static const uint32_t blake2s_IV[8] =
{
    0x6A09E667UL, 0xBB67AE85UL,
    0x3C6EF372UL, 0xA54FF53AUL,
    0x510E527FUL, 0x9B05688CUL,
    0x1F83D9ABUL, 0x5BE0CD19UL
};

static const uint8_t blake2s_sigma[10][16] =
{
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
    { 14,10,4, 8, 9,15,13,6, 1,12,0, 2, 11,7, 5, 3  },
    { 11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4 },
    { 7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8 },
    { 9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13 },
    { 2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9 },
    { 12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11 },
    { 13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10 },
    { 6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5 },
    { 10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0 }
};

inline uint32_t rotr32(uint32_t w, unsigned c)
{
    return (w >> c) | (w << (32 - c));
}

inline void blake2s_G(uint32_t m[16], uint32_t v[16], int a, int b, int c, int d, int x, int y)
{
    v[a] = v[a] + v[b] + m[x];
    v[d] = rotr32(v[d] ^ v[a], 16);
    v[c] = v[c] + v[d];
    v[b] = rotr32(v[b] ^ v[c], 12);
    v[a] = v[a] + v[b] + m[y];
    v[d] = rotr32(v[d] ^ v[a], 8);
    v[c] = v[c] + v[d];
    v[b] = rotr32(v[b] ^ v[c], 7);
}

void blake2s_compress(uint32_t h[8], const uint8_t block[64], uint64_t t, bool last)
{
    uint32_t v[16], m[16];
    memcpy(v, h, 8 * sizeof(uint32_t));
    memcpy(v + 8, blake2s_IV, 8 * sizeof(uint32_t));

    v[12] ^= (uint32_t)t;
    v[13] ^= (uint32_t)(t >> 32);

    if (last)
        v[14] = ~v[14];

    for (int i = 0; i < 16; i++)
    {
        m[i] = ((uint32_t)block[i * 4 + 0] << 0) |
               ((uint32_t)block[i * 4 + 1] << 8) |
               ((uint32_t)block[i * 4 + 2] << 16) |
               ((uint32_t)block[i * 4 + 3] << 24);
    }

    for (int i = 0; i < 10; i++)
    {
        blake2s_G(m, v, 0, 4, 8, 12, blake2s_sigma[i][0], blake2s_sigma[i][1]);
        blake2s_G(m, v, 1, 5, 9, 13, blake2s_sigma[i][2], blake2s_sigma[i][3]);
        blake2s_G(m, v, 2, 6, 10, 14, blake2s_sigma[i][4], blake2s_sigma[i][5]);
        blake2s_G(m, v, 3, 7, 11, 15, blake2s_sigma[i][6], blake2s_sigma[i][7]);
        blake2s_G(m, v, 0, 5, 10, 15, blake2s_sigma[i][8], blake2s_sigma[i][9]);
        blake2s_G(m, v, 1, 6, 11, 12, blake2s_sigma[i][10], blake2s_sigma[i][11]);
        blake2s_G(m, v, 2, 7, 8, 13, blake2s_sigma[i][12], blake2s_sigma[i][13]);
        blake2s_G(m, v, 3, 4, 9, 14, blake2s_sigma[i][14], blake2s_sigma[i][15]);
    }

    for (int i = 0; i < 8; i++)
        h[i] ^= v[i] ^ v[i + 8];
}

// ==================== Klasa Blake2s ====================
class Blake2sHasher
{
public:
    Blake2sHasher()
    {
        reset();
    }

    void reset()
    {
        for (int i = 0; i < 8; i++)
            h[i] = blake2s_IV[i];
        h[0] ^= 0x01010020;
        t = 0;
        bufferSize = 0;
    }

    void update(const uint8_t *data, size_t length)
    {
        size_t offset = 0;
        while (offset < length)
        {
            size_t left = length - offset;
            size_t fill = 64 - bufferSize;
            if (left > fill)
            {
                memcpy(buffer + bufferSize, data + offset, fill);
                t += 64;
                blake2s_compress(h, buffer, t, false);
                bufferSize = 0;
                offset += fill;
            }
            else
            {
                memcpy(buffer + bufferSize, data + offset, left);
                bufferSize += left;
                offset += left;
            }
        }
    }

    void finalize(uint8_t *out)
    {
        t += bufferSize;
        memset(buffer + bufferSize, 0, 64 - bufferSize);
        blake2s_compress(h, buffer, t, true);
        for (int i = 0; i < 8; i++)
        {
            out[i * 4 + 0] = (h[i] >> 0) & 0xFF;
            out[i * 4 + 1] = (h[i] >> 8) & 0xFF;
            out[i * 4 + 2] = (h[i] >> 16) & 0xFF;
            out[i * 4 + 3] = (h[i] >> 24) & 0xFF;
        }
    }

private:
    uint32_t h[8];
    uint64_t t;
    uint8_t buffer[64];
    size_t bufferSize;
};

// ==================== Funkcja z sygnaturą jak w Twoim kodzie ====================
QString hashBLAKE2s(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return QString();
    }

    Blake2sHasher hasher;

    while (!file.atEnd()) {
        QByteArray chunk = file.read(4096);
        hasher.update(reinterpret_cast<const uint8_t*>(chunk.constData()), chunk.size());
    }

    uint8_t out[32];
    hasher.finalize(out);

    QString result;
    for (int i = 0; i < 32; i++) {
        result.append(QString("%1").arg(out[i], 2, 16, QChar('0')).toUpper());
    }

    return result;
}
