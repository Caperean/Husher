#pragma once

#include <QString>

using HashFunction = QString(*)(const QString &inputPath);

QString hashAdler32(const QString &inputPath);
QString hashBLAKE2b(const QString &inputPath);
QString hashBLAKE2s(const QString &inputPath);
QString hashBLAKE3(const QString &inputPath);
QString hashCRC32(const QString &filePath);
QString hashCRC64(const QString &filePath);
QString hashGOST(const QString &filePath);
QString hashKeccak256(const QString &filePath);
QString hashKeccak512(const QString &filePath);
QString hashMD5(const QString &filePath);
QString hashRIPEMD160(const QString &filePath);
QString hashSHA1(const QString &filePath);
QString hashSHA3_224(const QString &filePath);
QString hashSHA3_256(const QString &filePath);
QString hashSHA3_384(const QString &filePath);
QString hashSHA3_512(const QString &filePath);
QString hashSHA224(const QString &filePath);
QString hashSHA256(const QString &filePath);
QString hashSHA384(const QString &filePath);
QString hashSHA512(const QString &filePath);
QString hashSHA512_224(const QString &filePath);
QString hashSHA512_256(const QString &filePath);
QString hashStreebog256(const QString &filePath);
QString hashStreebog512(const QString &filePath);
QString hashTiger(const QString &filePath);
QString hashWhirlpool(const QString &filePath);
QString hashXxHash3(const QString &filePath);
QString hashXxHash32(const QString &filePath);
QString hashXxHash64(const QString &filePath);
