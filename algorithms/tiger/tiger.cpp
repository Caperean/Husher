#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <cryptopp/tiger.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <QDebug>

QString hashTiger(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    CryptoPP::Tiger tiger;
    std::vector<char> buffer(4096);

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        tiger.Update(reinterpret_cast<const CryptoPP::byte*>(buffer.data()), file.gcount());
    }

    CryptoPP::byte digest[CryptoPP::Tiger::DIGESTSIZE];
    tiger.Final(digest);

    
    std::string hexOutput;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(hexOutput), false); // false = brak spacji
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return QString::fromStdString(hexOutput).toUpper();
}
