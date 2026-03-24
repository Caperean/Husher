#include "algorithms.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <openssl/evp.h>

QString hashBLAKE2b(const QString &inputPath)
{
    QFile file(inputPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << inputPath;
        return QString();
    }

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) return QString();

    const EVP_MD *md = EVP_blake2b512();
    if (EVP_DigestInit_ex(mdctx, md, nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        return QString();
    }

    while (!file.atEnd()) {
        QByteArray buffer = file.read(4096);
        if (EVP_DigestUpdate(mdctx, buffer.constData(), buffer.size()) != 1) {
            EVP_MD_CTX_free(mdctx);
            return QString();
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int length = 0;
    if (EVP_DigestFinal_ex(mdctx, hash, &length) != 1) {
        EVP_MD_CTX_free(mdctx);
        return QString();
    }

    EVP_MD_CTX_free(mdctx);

    QString result;
    for (unsigned int i = 0; i < length; ++i) {
        result.append(QString("%1").arg(hash[i], 2, 16, QChar('0')).toUpper());
    }

    return result;
}
