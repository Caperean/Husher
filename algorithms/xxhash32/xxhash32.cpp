#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <QDebug>
#include "xxhash/xxhash.h" 

QString hashXxHash32(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    XXH32_state_t* state = XXH32_createState();
    XXH32_reset(state, 0); // seed 0

    std::vector<char> buffer(4096);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        XXH32_update(state, buffer.data(), file.gcount());
    }

    uint32_t digest = XXH32_digest(state);
    XXH32_freeState(state);

    QString result;
    for (int i = 0; i < 4; ++i)
        result += QString("%1").arg((digest >> (24 - 8*i)) & 0xFF, 2, 16, QChar('0')).toUpper();

    return result;
}
