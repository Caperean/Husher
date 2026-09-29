#include "QString"
#include <fstream>
#include <vector>
#include "gost341112.h"   // Twoja implementacja
#include <QDebug>





QString hashStreebog512(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    Streebog hash(512);

    std::vector<unsigned char> data;
    std::vector<char> buffer(4096);

    while (file) {
        file.read(buffer.data(), buffer.size());
        data.insert(data.end(),
                    reinterpret_cast<unsigned char*>(buffer.data()),
                    reinterpret_cast<unsigned char*>(buffer.data()) + file.gcount());
    }

    unsigned char* digest = hash.hash(data.data(), data.size());

    QString result;
    for (int i = 0; i < 64; i++) {   // 64 bajty = 512 bitów
        result += QString("%1").arg(digest[i], 2, 16, QChar('0')).toUpper();
    }

    return result;
}

