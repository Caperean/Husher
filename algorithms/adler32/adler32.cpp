#include "algorithms.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <cryptopp/adler32.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

QString hashAdler32(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return QString();
    }

    CryptoPP::Adler32 adler;

    while (!file.atEnd()) {
        QByteArray buffer = file.read(4096);
        adler.Update(reinterpret_cast<const CryptoPP::byte*>(buffer.constData()), buffer.size());
    }

    CryptoPP::byte digest[CryptoPP::Adler32::DIGESTSIZE];
    adler.Final(digest);

    // Konwersja wyniku do hex string (duże litery)
    CryptoPP::HexEncoder encoder(nullptr, false); // false = bez spacji
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return QString::fromStdString(output).toUpper();
}

