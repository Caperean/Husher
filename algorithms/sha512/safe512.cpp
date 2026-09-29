#include "algorithms.hpp"
#include "QString"
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <string>
#include <QDebug>

QString hashSHA512(const QString &filePath) {
    std::string digest;
    try {
        CryptoPP::SHA512 hash;
        CryptoPP::FileSource fs(filePath.toStdString().c_str(), true,
            new CryptoPP::HashFilter(hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest), true // duże litery
                )
            )
        );
    } catch(...) {
        return QString();
    }
    return QString::fromStdString(digest);
}
