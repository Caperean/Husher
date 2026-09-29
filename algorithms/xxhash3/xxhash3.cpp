#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <QDebug>
#include "xxhash/xxhash.h" 

QString hashXxHash3(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    XXH3_state_t* state = XXH3_createState();
    XXH3_64bits_reset(state);

    std::vector<char> buffer(4096);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        XXH3_64bits_update(state, buffer.data(), file.gcount());
    }

    uint64_t digest = XXH3_64bits_digest(state);
    XXH3_freeState(state);

    QString result;
    for (int i = 0; i < 8; ++i)
        result += QString("%1").arg((digest >> (56 - 8*i)) & 0xFF, 2, 16, QChar('0')).toUpper();

    return result;
}
