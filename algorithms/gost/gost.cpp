#include "algorithms.hpp"
#include <QString>
#include <fstream>
#include <vector>

#include <cryptopp/gost.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

QString hashGOST(const QString &filePath)
{
    using namespace CryptoPP;

    std::ifstream file(filePath.toStdString(), std::ios::binary);
    if(!file)
        return QString();

    GOSTR3411_94 hash;
    std::string digest;

    std::vector<char> buffer(4096);

    while(file)
    {
        file.read(buffer.data(), buffer.size());
        std::streamsize readBytes = file.gcount();

        if(readBytes > 0)
            hash.Update(reinterpret_cast<const byte*>(buffer.data()), readBytes);
    }

    byte out[32];
    hash.Final(out);

    // HEX UPPERCASE — tak jak w Twoim SHA1
    QString result;
    for(int i=0;i<32;i++)
        result += QString("%1").arg(out[i], 2, 16, QChar('0')).toUpper();

    return result;
}
