#include <cryptopp/sha3.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include "algorithms.hpp"
#include <QString>
#include <QDebug>
QString hashSHA3_224(const QString &filePath) {
    std::string digest;
    CryptoPP::SHA3_224 hash;
    CryptoPP::FileSource fs(filePath.toStdString().c_str(), true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest), true // uppercase
            )
        )
    );
    return QString::fromStdString(digest);
}
