#include "algorithms.hpp"
#include "QString"
#include <QDebug>
#include <cryptopp/sha3.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <string>

QString hashSHA3_384(const QString &filePath) {
    std::string digest;
    CryptoPP::SHA3_384 hash;

    CryptoPP::FileSource fs(filePath.toStdString().c_str(), true,
        new CryptoPP::HashFilter(hash,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(digest), true // uppercase
            )
        )
    );

    return QString::fromStdString(digest);
}
