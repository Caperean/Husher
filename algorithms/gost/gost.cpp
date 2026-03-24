#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>

// Tabela podstawowa S-box dla GOST R 34.11-94
static const uint8_t gost_sbox[8][16] = {
    {4, 10, 9, 2, 13, 8, 0, 14, 6, 11, 1, 12, 7, 15, 5, 3},
    {14, 11, 4, 12, 6, 13, 15, 10, 2, 3, 8, 1, 0, 7, 5, 9},
    {5, 8, 1, 13, 10, 3, 4, 2, 14, 15, 12, 7, 6, 0, 9, 11},
    {7, 13, 10, 1, 0, 8, 9, 15, 14, 4, 6, 12, 11, 2, 5, 3},
    {6, 12, 7, 1, 5, 15, 13, 8, 4, 10, 9, 14, 0, 3, 11, 2},
    {4, 11, 10, 0, 7, 2, 1, 13, 3, 6, 8, 5, 9, 12, 15, 14},
    {13, 11, 4, 1, 3, 15, 5, 9, 0, 10, 14, 7, 6, 8, 2, 12},
    {1, 15, 13, 0, 5, 7, 10, 4, 9, 2, 3, 14, 6, 11, 8, 12}
};

// Prosta implementacja GOST R 34.11-94
QString hashGOST(const QString &filePath)
{
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    std::vector<uint8_t> buffer(32, 0); // stan wewnętrzny 256-bit
    std::vector<uint8_t> block(32, 0);

    while (file) {
        file.read(reinterpret_cast<char*>(block.data()), block.size());
        std::streamsize readBytes = file.gcount();
        if (readBytes == 0) break;

        for (std::streamsize i = 0; i < readBytes; ++i) {
            buffer[i] ^= block[i];
        }

        for (int round = 0; round < 12; ++round) {
            for (int i = 0; i < 32; ++i) {
                buffer[i] = gost_sbox[i % 8][buffer[i] >> 4] << 4 | gost_sbox[i % 8][buffer[i] & 0x0F];
            }
        }
    }

    QString result;
    for (size_t i = 0; i < buffer.size(); ++i) {
        result += QString("%1").arg(buffer[i], 2, 16, QChar('0')).toUpper();
    }

    return result;
}
