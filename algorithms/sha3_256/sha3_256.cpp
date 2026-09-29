
#include "algorithms.hpp"
#include "QString"
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <QDebug>
// Crypto++ główne nagłówki
#include <cryptopp/sha3.h>      // SHA3-256, SHA3-224, SHA3-512
#include <cryptopp/files.h>      // FileSource
#include <cryptopp/hex.h>        // HexEncoder
#include <cryptopp/filters.h>    // HashFilter, StringSink

QString hashSHA3_256(const QString &filePath) {
    std::string digest;
    CryptoPP::SHA3_256 hash;

    try {
        CryptoPP::FileSource fs(filePath.toStdString().c_str(), true,
            new CryptoPP::HashFilter(hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest), true // uppercase
                )
            )
        );
    } catch (const CryptoPP::Exception& e) {
        throw std::runtime_error("Crypto++ error: " + std::string(e.what()));
    }

    return QString::fromStdString(digest);
}

