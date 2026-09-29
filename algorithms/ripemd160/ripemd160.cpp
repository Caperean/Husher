#include "algorithms.hpp"
#include <QString>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

struct RIPEMD160_CTX {
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[64];
};

static void RIPEMD160_Init(RIPEMD160_CTX *ctx) {
    ctx->count = 0;
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
}

static void RIPEMD160_Transform(uint32_t state[5], const uint8_t block[64]);

static void RIPEMD160_Update(RIPEMD160_CTX *ctx, const uint8_t *input, size_t len) {
    size_t index = (ctx->count / 8) % 64;
    ctx->count += len * 8;
    size_t partLen = 64 - index;
    size_t i = 0;
    if (len >= partLen) {
        memcpy(&ctx->buffer[index], input, partLen);
        RIPEMD160_Transform(ctx->state, ctx->buffer);
        for (i = partLen; i + 63 < len; i += 64)
            RIPEMD160_Transform(ctx->state, &input[i]);
        index = 0;
    }
    memcpy(&ctx->buffer[index], &input[i], len - i);
}

static void RIPEMD160_Final(uint8_t digest[20], RIPEMD160_CTX *ctx) {
    uint8_t bits[8];
    for (int i = 0; i < 8; i++) bits[i] = (ctx->count >> (8 * i)) & 0xFF;

    size_t index = (ctx->count / 8) % 64;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);
    static uint8_t PADDING[64] = { 0x80 };
    RIPEMD160_Update(ctx, PADDING, padLen);
    RIPEMD160_Update(ctx, bits, 8);

    for (int i = 0; i < 5; i++) {
        digest[i * 4] = ctx->state[i] & 0xFF;
        digest[i * 4 + 1] = (ctx->state[i] >> 8) & 0xFF;
        digest[i * 4 + 2] = (ctx->state[i] >> 16) & 0xFF;
        digest[i * 4 + 3] = (ctx->state[i] >> 24) & 0xFF;
    }
}

// podstawowe operacje
#define F(x,y,z) ((x) ^ (y) ^ (z))
#define G(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define H(x,y,z) (((x) | ~(y)) ^ (z))
#define I(x,y,z) (((x) & (z)) | ((y) & ~(z)))
#define J(x,y,z) ((x) ^ ((y) | ~(z)))
#define ROTL(x,n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a,b,c,d,e,x,s) do { a += F(b,c,d) + x; a = ROTL(a,s) + e; } while(0)
#define GG(a,b,c,d,e,x,s) do { a += G(b,c,d) + x + 0x5a827999; a = ROTL(a,s) + e; } while(0)
#define HH(a,b,c,d,e,x,s) do { a += H(b,c,d) + x + 0x6ed9eba1; a = ROTL(a,s) + e; } while(0)
#define II(a,b,c,d,e,x,s) do { a += I(b,c,d) + x + 0x8f1bbcdc; a = ROTL(a,s) + e; } while(0)
#define JJ(a,b,c,d,e,x,s) do { a += J(b,c,d) + x + 0xa953fd4e; a = ROTL(a,s) + e; } while(0)

// tablice
static const int r[80] = { /* ... wszystkie elementy jak wcześniej ... */ };
static const int rr[80] = { /* ... wszystkie elementy jak wcześniej ... */ };
static const int s[80] = { /* ... wszystkie elementy jak wcześniej ... */ };
static const int ss[80] = { /* ... wszystkie elementy jak wcześniej ... */ };

static void RIPEMD160_Transform(uint32_t state[5], const uint8_t block[64]) {
    uint32_t a1, b1, c1, d1, e1, a2, b2, c2, d2, e2, t, x[16];
    for (int i = 0; i < 16; i++) 
        x[i] = block[i * 4] | (block[i * 4 + 1] << 8) | (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24);

    a1 = state[0]; b1 = state[1]; c1 = state[2]; d1 = state[3]; e1 = state[4];
    a2 = a1; b2 = b1; c2 = c1; d2 = d1; e2 = e1;

    for (int i = 0; i < 80; i++) {
        t = a1; a1 = e1; e1 = d1; d1 = ROTL(c1, 10); c1 = b1;
        if (i < 16) FF(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if (i < 32) GG(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if (i < 48) HH(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if (i < 64) II(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else JJ(a1,b1,c1,d1,e1,x[r[i]],s[i]);

        t = a2; a2 = e2; e2 = d2; d2 = ROTL(c2, 10); c2 = b2;
        if (i < 16) JJ(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if (i < 32) II(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if (i < 48) HH(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if (i < 64) GG(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else FF(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
    }

    uint32_t tstate = state[1] + c1 + d2;
    state[1] = state[2] + d1 + e2;
    state[2] = state[3] + e1 + a2;
    state[3] = state[4] + a1 + b2;
    state[4] = state[0] + b1 + c2;
    state[0] = tstate;
}

QString hashRIPEMD160(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);

    std::vector<char> buffer(4096);
    while (file) {
        file.read(buffer.data(), buffer.size());
        std::streamsize readBytes = file.gcount();
        if(readBytes > 0)
            RIPEMD160_Update(&ctx, reinterpret_cast<uint8_t*>(buffer.data()), readBytes);
    }

    uint8_t digest[20];
    RIPEMD160_Final(digest, &ctx);

    QString result;
    for (int i = 0; i < 20; i++)
        result += QString("%1").arg(digest[i], 2, 16, QChar('0')).toUpper();

    return result;
}

