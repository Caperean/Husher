#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>


QString hashXxHash32(const QString &filePath){
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    uint32_t seed = 0xDEADBEEF;
    uint32_t h32 = seed;

    std::vector<char> buffer(64);
    while(file.read(buffer.data(), buffer.size()) || file.gcount() > 0){
        size_t len = file.gcount();
        for(size_t i=0;i<len;i++){
            h32 += static_cast<uint8_t>(buffer[i]);
            h32 ^= (h32 >> 15);
            h32 *= 0x85ebca6b;
            h32 ^= (h32 >> 13);
            h32 *= 0xc2b2ae35;
            h32 ^= (h32 >> 16);
        }
    }

    QString result;
    for(int i=0;i<4;i++){
        result += QString("%1").arg((h32 >> (24-8*i)) & 0xFF, 2, 16, QChar('0')).toUpper();
    }

    return result;
}
