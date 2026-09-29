#include "QString"
#include <fstream>
#include <vector>
#include "gost341112.h"   // Twoja implementacja
#include <QDebug>

QString hashStreebog256(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    // 256-bitowy Streebog
    Streebog hash(256);

    std::vector<unsigned char> data;
    std::vector<char> buffer(4096);

    while (file) {
        file.read(buffer.data(), buffer.size());
        data.insert(data.end(),
                    reinterpret_cast<unsigned char*>(buffer.data()),
                    reinterpret_cast<unsigned char*>(buffer.data()) + file.gcount());
    }

    // Obliczenie skrótu
    unsigned char* digest = hash.hash(data.data(), data.size());

    // Konwersja na hex
    QString result;
    for (int i = 0; i < 32; i++) {   // 32 bajty = 256 bitów
        result += QString("%1").arg(digest[i], 2, 16, QChar('0')).toUpper();
    }

    return result;
}

