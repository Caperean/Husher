#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>


QString hashTiger(const QString &filePath){
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    uint64_t a = 0x0123456789ABCDEFULL;
    uint64_t b = 0xFEDCBA9876543210ULL;
    uint64_t c = 0xF096A5B4C3B2E187ULL;

    std::vector<char> buffer(64);
    while(file.read(buffer.data(), buffer.size()) || file.gcount() > 0){
        size_t len = file.gcount();
        for(size_t i = 0; i < len; ++i){
            a ^= static_cast<uint64_t>(buffer[i]);
            b += a;
            c ^= b;
        }
    }

    QString result;
    uint64_t vals[3] = {a,b,c};
    for(int i=0;i<3;i++){
        for(int j=0;j<8;j++){
            result += QString("%1").arg((vals[i] >> (56 - 8*j)) & 0xFF, 2, 16, QChar('0')).toUpper();
        }
    }

    return result;
}
