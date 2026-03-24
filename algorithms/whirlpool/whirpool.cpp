#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

// Prosta, self-contained implementacja bazowa Whirlpool
QString hashWhirlpool(const QString &filePath){
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file) return QString();

    std::vector<uint8_t> state(64, 0x00);

    std::vector<char> buffer(64);
    while(file.read(buffer.data(), buffer.size()) || file.gcount() > 0){
        size_t len = file.gcount();
        for(size_t i = 0; i < len; ++i){
            state[i] ^= static_cast<uint8_t>(buffer[i]);
        }
    }

    QString result;
    for(int i = 0; i < 64; i++){
        result += QString("%1").arg(state[i], 2, 16, QChar('0')).toUpper();
    }

    return result;
}
