#include "algorithms.hpp"
#include "QString"
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
    size_t i=0;
    if (len >= partLen) {
        memcpy(&ctx->buffer[index], input, partLen);
        RIPEMD160_Transform(ctx->state, ctx->buffer);
        for(i = partLen; i + 63 < len; i += 64)
            RIPEMD160_Transform(ctx->state, &input[i]);
        index = 0;
    }
    memcpy(&ctx->buffer[index], &input[i], len - i);
}

static void RIPEMD160_Final(uint8_t digest[20], RIPEMD160_CTX *ctx) {
    uint8_t bits[8];
    for(int i=0;i<8;i++) bits[i] = (ctx->count >> (8*i)) & 0xFF;

    size_t index = (ctx->count / 8) % 64;
    size_t padLen = (index < 56) ? (56 - index) : (120 - index);
    static uint8_t PADDING[64] = {0x80};
    RIPEMD160_Update(ctx, PADDING, padLen);
    RIPEMD160_Update(ctx, bits, 8);

    for(int i=0;i<5;i++) {
        digest[i*4]     = ctx->state[i] & 0xFF;
        digest[i*4+1]   = (ctx->state[i] >> 8) & 0xFF;
        digest[i*4+2]   = (ctx->state[i] >> 16) & 0xFF;
        digest[i*4+3]   = (ctx->state[i] >> 24) & 0xFF;
    }
}

// podstawowe operacje
#define F(x,y,z) ((x) ^ (y) ^ (z))
#define G(x,y,z) (((x) & (y)) | (~(x) & (z)))
#define H(x,y,z) (((x) | ~(y)) ^ (z))
#define I(x,y,z) (((x) & (z)) | ((y) & ~(z)))
#define J(x,y,z) ((x) ^ ((y) | ~(z)))
#define ROTL(x,n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a,b,c,d,e,x,s) { a += F(b,c,d) + x; a = ROTL(a,s) + e; }
#define GG(a,b,c,d,e,x,s) { a += G(b,c,d) + x + 0x5a827999; a = ROTL(a,s) + e; }
#define HH(a,b,c,d,e,x,s) { a += H(b,c,d) + x + 0x6ed9eba1; a = ROTL(a,s) + e; }
#define II(a,b,c,d,e,x,s) { a += I(b,c,d) + x + 0x8f1bbcdc; a = ROTL(a,s) + e; }
#define JJ(a,b,c,d,e,x,s) { a += J(b,c,d) + x + 0xa953fd4e; a = ROTL(a,s) + e; }

static const int r[80] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
7,4,13,1,10,6,15,3,12,0,9,5,2,14,11,8,
3,10,14,4,9,15,8,1,2,7,0,6,13,11,5,12,
1,9,11,10,0,8,12,4,13,3,7,15,14,5,6,2,
4,0,5,9,7,12,2,10,14,1,3,8,11,6,15,13
};

static const int rr[80] = {
5,14,7,0,9,2,11,4,13,6,15,8,1,10,3,12,
6,11,3,7,0,13,5,10,14,15,8,12,4,9,1,2,
15,5,1,3,7,14,6,9,11,8,12,2,10,0,13,4,
8,6,4,1,3,11,15,0,5,12,2,13,9,7,10,14,
12,15,10,4,1,5,8,7,6,2,13,14,0,3,9,11
};

static const int s[80] = {
11,14,15,12,5,8,7,9,11,13,14,15,6,7,9,8,
7,6,8,13,11,9,7,15,7,12,15,9,11,7,13,12,
11,13,6,7,14,9,13,15,14,8,13,6,5,12,7,5,
11,12,14,15,14,15,9,8,9,14,5,6,8,6,5,12,
9,15,5,11,6,8,13,12,5,12,13,14,11,8,5,6
};

static const int ss[80] = {
8,9,9,11,13,15,15,5,7,7,8,11,14,14,12,6,
9,13,15,7,12,8,9,11,7,7,12,7,6,15,13,11,
9,7,15,11,8,6,6,14,12,13,5,14,13,13,7,5,
15,5,8,11,14,14,6,14,6,9,12,9,12,5,15,8,
8,5,12,9,12,5,14,6,8,13,6,5,15,13,11,11
};

static void RIPEMD160_Transform(uint32_t state[5], const uint8_t block[64]) {
    uint32_t a1,b1,c1,d1,e1,a2,b2,c2,d2,e2,t,x[16];
    for(int i=0;i<16;i++) x[i]=block[i*4]|(block[i*4+1]<<8)|(block[i*4+2]<<16)|(block[i*4+3]<<24);
    a1=state[0]; b1=state[1]; c1=state[2]; d1=state[3]; e1=state[4];
    a2=a1; b2=b1; c2=c1; d2=d1; e2=e1;
    for(int i=0;i<80;i++) {
        t=a1; a1=e1; e1=d1; d1=ROTL(c1,10); c1=b1;
        if(i<16) FF(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if(i<32) GG(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if(i<48) HH(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else if(i<64) II(a1,b1,c1,d1,e1,x[r[i]],s[i]);
        else JJ(a1,b1,c1,d1,e1,x[r[i]],s[i]);

        t=a2; a2=e2; e2=d2; d2=ROTL(c2,10); c2=b2;
        if(i<16) JJ(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if(i<32) II(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if(i<48) HH(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else if(i<64) GG(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
        else FF(a2,b2,c2,d2,e2,x[rr[i]],ss[i]);
    }
    uint32_t tstate=state[1]+c1+d2; state[1]=state[2]+d1+e2; state[2]=state[3]+e1+a2; state[3]=state[4]+a1+b2;
    state[4]=state[0]+b1+c2; state[0]=tstate;
}

QString hashRIPEMD160(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    RIPEMD160_CTX ctx;
    RIPEMD160_Init(&ctx);

    std::vector<char> buffer(4096);
    while(file) {
        file.read(buffer.data(), buffer.size());
        RIPEMD160_Update(&ctx, reinterpret_cast<uint8_t*>(buffer.data()), file.gcount());
    }

    uint8_t digest[20];
    RIPEMD160_Final(digest, &ctx);

    QString result;
    for(int i=0;i<20;i++) result += QString("%1").arg(digest[i],2,16,QChar('0')).toUpper();
    return result;
}
