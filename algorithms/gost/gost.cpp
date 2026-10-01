#include "algorithms.hpp"
#include <QString>
#include <fstream>
#include <vector>
#include <cstring>

#include <cryptopp/gost.h>   // GOST 28147-89 block cipher (used as the core of the hash)
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

// ---------------------------------------------------------------------------
// GOST R 34.11-94 hash (RFC 5831) built on top of Crypto++'s CryptoPP::GOST
// block cipher.
//
// Crypto++ has no GOSTR3411_94 class, only the cipher. The hash is a
// Miyaguchi-Preneel-like construction around that cipher, so only the glue
// (key generation, mixing, padding, checksum) is implemented here.
//
// NOTE: Crypto++'s GOST cipher has a fixed S-box (the RFC 5831 "test"
// parameter set), so this computes the *test-parameters* variant, e.g.
//   GOST("abc") = f3134348c44fb1b2a277729e2285ebb5cb5e0f29c975bc753b70497c06a4d51d
// The "CryptoPro" variant (used by OpenSSL gost engine) gives different digests.
// ---------------------------------------------------------------------------
namespace {

class GOSTR3411_94
{
public:
    GOSTR3411_94() { std::memset(m_h, 0, 32); std::memset(m_sum, 0, 32); }

    void Update(const CryptoPP::byte *data, size_t len)
    {
        m_total += len;

        if(m_bufLen)
        {
            size_t take = 32 - m_bufLen;
            if(take > len) take = len;
            std::memcpy(m_buf + m_bufLen, data, take);
            m_bufLen += take; data += take; len -= take;
            if(m_bufLen == 32) { Block(m_buf); m_bufLen = 0; }
        }
        while(len >= 32) { Block(data); data += 32; len -= 32; }
        if(len) { std::memcpy(m_buf, data, len); m_bufLen = len; }
    }

    // writes 32 bytes; object must not be reused afterwards
    void Final(CryptoPP::byte *out)
    {
        if(m_bufLen)
        {
            std::memset(m_buf + m_bufLen, 0, 32 - m_bufLen);
            Block(m_buf);
        }

        CryptoPP::byte lenBlock[32] = {0};
        const unsigned long long bits = m_total * 8ULL;
        for(int i = 0; i < 8; ++i)
            lenBlock[i] = static_cast<CryptoPP::byte>(bits >> (8 * i));

        Compress(m_h, lenBlock);
        Compress(m_h, m_sum);
        std::memcpy(out, m_h, 32);
    }

private:
    typedef CryptoPP::byte byte_t;

    CryptoPP::byte m_h[32], m_sum[32], m_buf[32];
    size_t m_bufLen = 0;
    unsigned long long m_total = 0;

    void Block(const byte_t *m)
    {
        unsigned c = 0;                         // sum += m  (mod 2^256, little endian)
        for(int i = 0; i < 32; ++i) { c += m_sum[i] + m[i]; m_sum[i] = byte_t(c); c >>= 8; }
        Compress(m_h, m);
    }

    static void A(byte_t *y)
    {
        byte_t t[32];
        std::memcpy(t, y + 8, 24);
        for(int i = 0; i < 8; ++i) t[24 + i] = y[i] ^ y[8 + i];
        std::memcpy(y, t, 32);
    }

    static void P(const byte_t *y, byte_t *r)
    {
        for(int i = 0; i < 4; ++i)
            for(int k = 1; k <= 8; ++k)
                r[i + 4 * (k - 1)] = y[8 * i + k - 1];
    }

    static void Psi(byte_t *y)
    {
        unsigned short w[16];
        for(int i = 0; i < 16; ++i) w[i] = static_cast<unsigned short>(y[2*i] | (y[2*i+1] << 8));
        unsigned short nw = w[0] ^ w[1] ^ w[2] ^ w[3] ^ w[12] ^ w[15];
        for(int i = 0; i < 15; ++i) w[i] = w[i + 1];
        w[15] = nw;
        for(int i = 0; i < 16; ++i) { y[2*i] = byte_t(w[i]); y[2*i+1] = byte_t(w[i] >> 8); }
    }

    static void Compress(byte_t *H, const byte_t *M)
    {
        static const byte_t C3[32] = {
            0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
            0x00,0xff,0xff,0x00,0xff,0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xff,0xff,0x00,0xff};

        byte_t U[32], V[32], W[32], K[4][32], S[32];
        std::memcpy(U, H, 32);
        std::memcpy(V, M, 32);

        // key generation
        for(int j = 0; j < 4; ++j)
        {
            if(j)
            {
                A(U);
                if(j == 2) for(int i = 0; i < 32; ++i) U[i] ^= C3[i];
                A(V); A(V);
            }
            for(int i = 0; i < 32; ++i) W[i] = U[i] ^ V[i];
            P(W, K[j]);
        }

        // encryption of the four 64-bit parts of H with Crypto++'s GOST cipher
        for(int j = 0; j < 4; ++j)
        {
            CryptoPP::GOST::Encryption enc;
            enc.SetKey(K[j], 32);
            enc.ProcessBlock(H + 8 * j, S + 8 * j);
        }

        // mixing: H = psi^61( H ^ psi( M ^ psi^12(S) ) )
        for(int i = 0; i < 12; ++i) Psi(S);
        for(int i = 0; i < 32; ++i) S[i] ^= M[i];
        Psi(S);
        for(int i = 0; i < 32; ++i) S[i] ^= H[i];
        for(int i = 0; i < 61; ++i) Psi(S);
        std::memcpy(H, S, 32);
    }
};

} // namespace

QString hashGOST(const QString &filePath)
{
    using namespace CryptoPP;

    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file)
        return QString();

    GOSTR3411_94 hash;
    std::string digest;

    std::vector<char> buffer(4096);

    while(file)
    {
        file.read(buffer.data(), buffer.size());
        std::streamsize readBytes = file.gcount();

        if(readBytes > 0)
            hash.Update(reinterpret_cast<const byte*>(buffer.data()), readBytes);
    }

    byte out[32];
    hash.Final(out);

    // HEX UPPERCASE — tak jak w Twoim SHA1
    QString result;
    for(int i=0;i<32;i++)
        result += QString("%1").arg(out[i], 2, 16, QChar('0')).toUpper();

    return result;
}