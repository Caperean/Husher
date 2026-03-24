#include "utils.hpp"
#include <QComboBox>

void setHashingAlgorithm(QComboBox *comboBox)
{
    comboBox->clear();

    // Auto-detect
    comboBox->addItem("Auto-detect", QVariant::fromValue(HashType::AutoDetect));
    comboBox->addSeparator();

    // SHA-2
    comboBox->addItem("SHA-256", QVariant::fromValue(HashType::SHA256));
    comboBox->addItem("SHA-224", QVariant::fromValue(HashType::SHA224));
    comboBox->addItem("SHA-384", QVariant::fromValue(HashType::SHA384));
    comboBox->addItem("SHA-512", QVariant::fromValue(HashType::SHA512));
    comboBox->addSeparator();

    // SHA-1
    comboBox->addItem("SHA-1", QVariant::fromValue(HashType::SHA1));
    comboBox->addSeparator();

    // SHA-3
    comboBox->addItem("SHA3-224", QVariant::fromValue(HashType::SHA3_224));
    comboBox->addItem("SHA3-256", QVariant::fromValue(HashType::SHA3_256));
    comboBox->addItem("SHA3-384", QVariant::fromValue(HashType::SHA3_384));
    comboBox->addItem("SHA3-512", QVariant::fromValue(HashType::SHA3_512));
    comboBox->addSeparator();

    // SHA-512 variants
    comboBox->addItem("SHA-512/256", QVariant::fromValue(HashType::SHA512_256));
    comboBox->addItem("SHA-512/224", QVariant::fromValue(HashType::SHA512_224));
    comboBox->addSeparator();

    // MD5
    comboBox->addItem("MD5", QVariant::fromValue(HashType::MD5));
    comboBox->addSeparator();

    // BLAKE
    comboBox->addItem("BLAKE2b", QVariant::fromValue(HashType::BLAKE2b));
    comboBox->addItem("BLAKE2s", QVariant::fromValue(HashType::BLAKE2s));
    comboBox->addItem("BLAKE3", QVariant::fromValue(HashType::BLAKE3));
    comboBox->addSeparator();

    // CRC
    comboBox->addItem("CRC32 (ISO)", QVariant::fromValue(HashType::CRC32));
    comboBox->addItem("CRC64 (ECMA)", QVariant::fromValue(HashType::CRC64));
    comboBox->addSeparator();

    // Rare
    comboBox->addItem("RIPEMD-160", QVariant::fromValue(HashType::RIPEMD160));
    comboBox->addItem("Whirlpool", QVariant::fromValue(HashType::Whirlpool));
    comboBox->addSeparator();

    // Keccak
    comboBox->addItem("Keccak-256", QVariant::fromValue(HashType::Keccak256));
    comboBox->addItem("Keccak-512", QVariant::fromValue(HashType::Keccak512));
    comboBox->addSeparator();

    // Fast
    comboBox->addItem("xxHash32", QVariant::fromValue(HashType::xxHash32));
    comboBox->addItem("xxHash64", QVariant::fromValue(HashType::xxHash64));
    comboBox->addItem("xxHash3 (64-bit)", QVariant::fromValue(HashType::xxHash3));
    comboBox->addSeparator();

    // Checksum + legacy
    comboBox->addItem("Adler-32", QVariant::fromValue(HashType::Adler32));
    comboBox->addItem("Tiger", QVariant::fromValue(HashType::Tiger));
    comboBox->addItem("GOST R 34.11-94", QVariant::fromValue(HashType::GOST));
    comboBox->addSeparator();

    // Streebog
    comboBox->addItem("Streebog-256", QVariant::fromValue(HashType::Streebog256));
    comboBox->addItem("Streebog-512", QVariant::fromValue(HashType::Streebog512));

    // Ustawienie domyślnego
    comboBox->setCurrentText("SHA-256");
}

void MainWindow::on_compareBtn_clicked()
{
    // Implementacja porównywania hashów
}
