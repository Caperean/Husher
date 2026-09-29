#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <QDebug>
#include <vector>
#include "xxhash.h" // oficjalna biblioteka

QString hashXxHash64(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    XXH64_state_t* state = XXH64_createState();
    XXH64_reset(state, 0); // seed 0

    std::vector<char> buffer(4096);
    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        XXH64_update(state, buffer.data(), file.gcount());
    }

    uint64_t digest = XXH64_digest(state);
    XXH64_freeState(state);

    QString result;
    for (int i = 0; i < 8; ++i)
        result += QString("%1").arg((digest >> (56 - 8*i)) & 0xFF, 2, 16, QChar('0')).toUpper();

    return result;
}
