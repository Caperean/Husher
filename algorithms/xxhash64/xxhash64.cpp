#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

// Prosta, self-contained wersja bazowa xxHash64
QString hashXxHash64(const QString &filePath){
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    uint64_t seed = 0xDEADBEEFDEADBEEFULL;
    uint64_t h64 = seed;

    std::vector<char> buffer(64);
    while(file.read(buffer.data(), buffer.size()) || file.gcount() > 0){
        size_t len = file.gcount();
        for(size_t i=0;i<len;i++){
            h64 += static_cast<uint64_t>(buffer[i]);
            h64 ^= (h64 >> 33);
            h64 *= 0xff51afd7ed558ccdULL;
            h64 ^= (h64 >> 33);
            h64 *= 0xc4ceb9fe1a85ec53ULL;
            h64 ^= (h64 >> 33);
        }
    }

    QString result;
    for(int i=0;i<8;i++){
        result += QString("%1").arg((h64 >> (56-8*i)) & 0xFF, 2, 16, QChar('0')).toUpper();
    }

    return result;
}
