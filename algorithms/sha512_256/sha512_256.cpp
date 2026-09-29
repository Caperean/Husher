#include <QString>
#include <fstream>
#include <vector>
#include <cryptopp/sha.h>
#include <QDebug>

QString hashSHA512_256(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    CryptoPP::SHA512 sha512;
    std::vector<char> buffer(4096);

    while (file) {
        file.read(buffer.data(), buffer.size());
        sha512.Update(reinterpret_cast<const CryptoPP::byte*>(buffer.data()), file.gcount());
    }

    CryptoPP::byte fullDigest[CryptoPP::SHA512::DIGESTSIZE];
    sha512.Final(fullDigest);

    // SHA-512/256 = pierwsze 32 bajty (256 bitów)
    QString result;
    for (size_t i = 0; i < 32; i++)
        result += QString("%1").arg(fullDigest[i], 2, 16, QChar('0')).toUpper();

    return result;
}

