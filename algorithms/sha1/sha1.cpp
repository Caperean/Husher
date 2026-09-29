#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <QDebug>


struct SHA1_CTX {
    uint32_t state[5];
    uint64_t count;
    uint8_t buffer[64];
};

static void SHA1_Init(SHA1_CTX *ctx) {
    ctx->state[0] = 0x67452301;
    ctx->state[1] = 0xefcdab89;
    ctx->state[2] = 0x98badcfe;
    ctx->state[3] = 0x10325476;
    ctx->state[4] = 0xc3d2e1f0;
    ctx->count = 0;
}

static void SHA1_Transform(uint32_t state[5], const uint8_t buffer[64]);

static void SHA1_Update(SHA1_CTX *ctx, const uint8_t *data, size_t len) {
    size_t i, index, partLen;
    index = (ctx->count / 8) % 64;
    ctx->count += len * 8;
    partLen = 64 - index;

    if(len >= partLen) {
        memcpy(&ctx->buffer[index], data, partLen);
        SHA1_Transform(ctx->state, ctx->buffer);
        for(i = partLen; i + 63 < len; i += 64)
            SHA1_Transform(ctx->state, &data[i]);
        index = 0;
    } else i = 0;
    memcpy(&ctx->buffer[index], &data[i], len - i);
}

static void SHA1_Final(uint8_t digest[20], SHA1_CTX *ctx) {
    uint8_t bits[8];
    for(int i=0;i<8;i++) bits[i] = (ctx->count >> (56 - 8*i)) & 0xFF;

    size_t index = (ctx->count / 8) % 64;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);
    static uint8_t PADDING[64] = {0x80};
    SHA1_Update(ctx, PADDING, padLen);
    SHA1_Update(ctx, bits, 8);

    for(int i=0;i<5;i++) {
        digest[i*4]     = (ctx->state[i] >> 24) & 0xFF;
        digest[i*4+1]   = (ctx->state[i] >> 16) & 0xFF;
        digest[i*4+2]   = (ctx->state[i] >> 8) & 0xFF;
        digest[i*4+3]   = ctx->state[i] & 0xFF;
    }
}

#define ROL(x,n) (((x) << (n)) | ((x) >> (32-(n))))

static void SHA1_Transform(uint32_t state[5], const uint8_t buffer[64]) {
    uint32_t w[80], a,b,c,d,e,t;
    for(int i=0;i<16;i++)
        w[i] = (buffer[i*4]<<24) | (buffer[i*4+1]<<16) | (buffer[i*4+2]<<8) | (buffer[i*4+3]);
    for(int i=16;i<80;i++)
        w[i] = ROL(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);

    a=state[0]; b=state[1]; c=state[2]; d=state[3]; e=state[4];

    for(int i=0;i<80;i++) {
        if(i<20) t=ROL(a,5)+((b&c)|(~b&d))+e+w[i]+0x5a827999;
        else if(i<40) t=ROL(a,5)+(b^c^d)+e+w[i]+0x6ed9eba1;
        else if(i<60) t=ROL(a,5)+((b&c)|(b&d)|(c&d))+e+w[i]+0x8f1bbcdc;
        else t=ROL(a,5)+(b^c^d)+e+w[i]+0xca62c1d6;
        e=d; d=c; c=ROL(b,30); b=a; a=t;
    }

    state[0]+=a; state[1]+=b; state[2]+=c; state[3]+=d; state[4]+=e;
}

QString hashSHA1(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    SHA1_CTX ctx;
    SHA1_Init(&ctx);

    std::vector<char> buffer(4096);
    while(file) {
        file.read(buffer.data(), buffer.size());
        SHA1_Update(&ctx, reinterpret_cast<uint8_t*>(buffer.data()), file.gcount());
    }

    uint8_t digest[20];
    SHA1_Final(digest, &ctx);

    QString result;
    for(int i=0;i<20;i++) result += QString("%1").arg(digest[i],2,16,QChar('0')).toUpper();
    return result;
}
