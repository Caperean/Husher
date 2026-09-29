#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cryptopp/whirlpool.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <QDebug>

QString hashWhirlpool(const QString &filePath) {
    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if (!file) return QString();

    CryptoPP::Whirlpool whirlpool;
    std::vector<char> buffer(4096);

    while (file.read(buffer.data(), buffer.size()) || file.gcount() > 0) {
        whirlpool.Update(reinterpret_cast<const CryptoPP::byte*>(buffer.data()), file.gcount());
    }

    CryptoPP::byte digest[CryptoPP::Whirlpool::DIGESTSIZE];
    whirlpool.Final(digest);

   
    std::string hexOutput;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(hexOutput), false);
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return QString::fromStdString(hexOutput).toUpper();
}
