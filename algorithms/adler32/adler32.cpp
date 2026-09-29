#include "algorithms.hpp"
#include <QFile>
#include <QByteArray>
#include <QDebug>


QString hashAdler32(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return QString();
    }

    const quint32 MOD_ADLER = 65521;
    quint32 a = 1;
    quint32 b = 0;

    
    while (!file.atEnd()) {
        QByteArray buffer = file.read(4096);
        for (char byte : buffer) {
            a = (a + static_cast<quint8>(byte)) % MOD_ADLER;
            b = (b + a) % MOD_ADLER;
        }
    }

    quint32 adler = (b << 16) | a;

    
    return QString("%1").arg(adler, 8, 16, QChar('0')).toUpper();
}
