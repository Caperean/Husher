#pragma once


#include <QComboBox>
#include <QVariant>

// Enum globalny, żeby był dostępny w całym projekcie
enum class HashType {
    AutoDetect,
    SHA256,
    SHA224,
    SHA384,
    SHA512,
    SHA512_256,
    SHA512_224,
    SHA1,
    SHA3_224,
    SHA3_256,
    SHA3_384,
    SHA3_512,
    MD5,
    BLAKE2b,
    BLAKE2s,
    BLAKE3,
    CRC32,
    CRC64,
    RIPEMD160,
    Whirlpool,
    Keccak256,
    Keccak512,
    xxHash32,
    xxHash64,
    xxHash3,
    Adler32,
    Tiger,
    GOST,
    Streebog256,
    Streebog512
};

// Funkcja do wypełniania QComboBox
void setHashingAlgorithm(QComboBox *comboBox);
void on_compareBtn_clicked();


