//#include "blake3.hpp"
#include "blake3.h"
#include "QString"
#include <fstream>
#include <vector>
#include <iostream>

QString hashBLAKE3(const QString &filePath)
{
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);

    std::vector<char> buffer(4096);
    while (file) {
        file.read(buffer.data(), buffer.size());
        std::streamsize readBytes = file.gcount();
        if (readBytes > 0) {
            blake3_hasher_update(&hasher, buffer.data(), static_cast<size_t>(readBytes));
        }
    }

    uint8_t hash[BLAKE3_OUT_LEN];
    blake3_hasher_finalize(&hasher, hash, BLAKE3_OUT_LEN);

    QString result;
    char buf[3];
    for (size_t i = 0; i < BLAKE3_OUT_LEN; ++i) {
        snprintf(buf, sizeof(buf), "%02X", hash[i]);
        result += QString(buf);
    }

    return result;
}


