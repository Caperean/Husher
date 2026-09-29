#include "algorithms.hpp"
#include "QString"
#include <cryptopp/sha3.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <string>
#include <stdexcept>
#include <QDebug>

QString hashSHA3_512(const QString &filePath) {
    try {
        std::string digest;
        CryptoPP::SHA3_512 hash;

        // FileSource w Crypto++ obsłuży wczytanie pliku i hashowanie w locie
        CryptoPP::FileSource fs(filePath.toStdString().c_str(), true,
            new CryptoPP::HashFilter(hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest), true // true = uppercase
                )
            )
        );

        return QString::fromStdString(digest);
    } catch (const std::exception& e) {
        // jeśli plik nie istnieje lub błąd od Crypto++
        return QString();
    }
}

