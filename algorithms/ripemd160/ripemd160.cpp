#include "algorithms.hpp"
#include <QString>
#include <cryptopp/ripemd.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <QDebug>
QString hashRIPEMD160(const QString &filePath)
{
    std::string digest;
    try {
        CryptoPP::RIPEMD160 hash;

        CryptoPP::FileSource fs(
            filePath.toStdString().c_str(), 
            true,  // true – czyta cały plik
            new CryptoPP::HashFilter(
                hash,
                new CryptoPP::HexEncoder(
                    new CryptoPP::StringSink(digest),
                    true // duże litery w HEX
                )
            )
        );
    }
    catch (const std::exception &e) {
        qWarning() << "hashRIPEMD160 error:" << e.what();
        return QString();
    }

    return QString::fromStdString(digest);
}
