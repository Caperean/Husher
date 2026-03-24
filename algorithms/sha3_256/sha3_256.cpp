#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

static const uint64_t keccakf_rndc[24] = {
    0x0000000000000001ULL,0x0000000000008082ULL,0x800000000000808aULL,0x8000000080008000ULL,
    0x000000000000808bULL,0x0000000080000001ULL,0x8000000080008081ULL,0x8000000000008009ULL,
    0x000000000000008aULL,0x0000000000000088ULL,0x0000000080008009ULL,0x000000008000000aULL,
    0x000000008000808bULL,0x800000000000008bULL,0x8000000000008089ULL,0x8000000000008003ULL,
    0x8000000000008002ULL,0x8000000000000080ULL,0x000000000000800aULL,0x800000008000000aULL,
    0x8000000080008081ULL,0x8000000000008080ULL,0x0000000080000001ULL,0x8000000080008008ULL
};

static const int keccakf_rotc[24] = {
 1,3,6,10,15,21,28,36,45,55,2,14,
 27,41,56,8,25,43,62,18,39,61,20,44
};

static const int keccakf_piln[24] = {
10,7,11,17,18,3,5,16,8,21,24,4,
15,23,19,13,12,2,20,14,22,9,6,1
};

static inline uint64_t rol(uint64_t x,int y){ return (x<<y)|(x>>(64-y)); }

static void keccakf(uint64_t st[25]){
    int i,j,round; uint64_t t,bc[5];
    for(round=0;round<24;round++){
        for(i=0;i<5;i++) bc[i]=st[i]^st[i+5]^st[i+10]^st[i+15]^st[i+20];
        for(i=0;i<5;i++){ t=bc[(i+4)%5]^rol(bc[(i+1)%5],1); for(j=0;j<25;j+=5) st[j+i]^=t; }
        t=st[1]; j=0;
        for(i=0;i<24;i++){ bc[0]=st[keccakf_piln[i]]; st[keccakf_piln[i]]=rol(t,keccakf_rotc[i]); t=bc[0]; }
        for(j=0;j<25;j+=5) for(i=0;i<5;i++) st[j+i]^=~st[j+(i+1)%5]&st[j+(i+2)%5];
        st[0]^=keccakf_rndc[round];
    }
}

QString hashSHA3_256(const QString &filePath){
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    uint64_t st[25]; memset(st,0,sizeof(st));
    std::vector<char> buffer(136); // rate = 1088 bits = 136 bytes

    while(file){
        file.read(buffer.data(), buffer.size());
        std::streamsize readBytes=file.gcount();
        for(std::streamsize i=0;i<readBytes;i++) st[i/8]^=((uint64_t)(uint8_t)buffer[i])<<(8*(i%8));
        keccakf(st);
    }

    st[(0x06)/8]^=0x06ULL<<(0x06%8);
    st[16]^=0x80ULL<<((64-8*0)%64);
    keccakf(st);

    QString result;
    for(int i=0;i<8;i++){ // 256 bit = 32 bytes
        for(int j=0;j<8;j++){
            if(i*8+j>=32) break;
            result+=QString("%1").arg(uint8_t(st[i]>>(8*j)),2,16,QChar('0')).toUpper();
        }
    }
    return result;
}
