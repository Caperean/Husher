#include <QComboBox> //mainwindow_utils.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h" 
#include "algorithms.hpp"
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>


void MainWindow::setHashingAlgorithm(QComboBox *comboBox)
{
    comboBox->clear();

    // Auto-detect
    comboBox->addItem("Auto-detect", QVariant::fromValue(HashType::AutoDetect));
    comboBox->insertSeparator(comboBox->count());


    // SHA-2
    comboBox->addItem("SHA-256", QVariant::fromValue(HashType::SHA256));
    comboBox->addItem("SHA-224", QVariant::fromValue(HashType::SHA224));
    comboBox->addItem("SHA-384", QVariant::fromValue(HashType::SHA384));
    comboBox->addItem("SHA-512", QVariant::fromValue(HashType::SHA512));
    comboBox->insertSeparator(comboBox->count());

    // SHA-1
    comboBox->addItem("SHA-1", QVariant::fromValue(HashType::SHA1));
    comboBox->insertSeparator(comboBox->count());


    // SHA-3
    comboBox->addItem("SHA3-224", QVariant::fromValue(HashType::SHA3_224));
    comboBox->addItem("SHA3-256", QVariant::fromValue(HashType::SHA3_256));
    comboBox->addItem("SHA3-384", QVariant::fromValue(HashType::SHA3_384));
    comboBox->addItem("SHA3-512", QVariant::fromValue(HashType::SHA3_512));
    comboBox->insertSeparator(comboBox->count());

    // SHA-512 variants
    comboBox->addItem("SHA-512/256", QVariant::fromValue(HashType::SHA512_256));
    comboBox->addItem("SHA-512/224", QVariant::fromValue(HashType::SHA512_224));
    comboBox->insertSeparator(comboBox->count());

    // MD5
    comboBox->addItem("MD5", QVariant::fromValue(HashType::MD5));
    comboBox->insertSeparator(comboBox->count());

    // BLAKE
    comboBox->addItem("BLAKE2b", QVariant::fromValue(HashType::BLAKE2b));
    comboBox->addItem("BLAKE2s", QVariant::fromValue(HashType::BLAKE2s));
    comboBox->addItem("BLAKE3", QVariant::fromValue(HashType::BLAKE3));
    comboBox->insertSeparator(comboBox->count());

    // CRC
    comboBox->addItem("CRC32 (ISO)", QVariant::fromValue(HashType::CRC32));
    comboBox->addItem("CRC64 (ECMA)", QVariant::fromValue(HashType::CRC64));
    comboBox->insertSeparator(comboBox->count());

    // Rare
    comboBox->addItem("RIPEMD-160", QVariant::fromValue(HashType::RIPEMD160));
    comboBox->addItem("Whirlpool", QVariant::fromValue(HashType::Whirlpool));
    comboBox->insertSeparator(comboBox->count());

    // Keccak
    comboBox->addItem("Keccak-256", QVariant::fromValue(HashType::Keccak256));
    comboBox->addItem("Keccak-512", QVariant::fromValue(HashType::Keccak512));
    comboBox->insertSeparator(comboBox->count());

    // Fast
    comboBox->addItem("xxHash32", QVariant::fromValue(HashType::xxHash32));
    comboBox->addItem("xxHash64", QVariant::fromValue(HashType::xxHash64));
    comboBox->addItem("xxHash3 (64-bit)", QVariant::fromValue(HashType::xxHash3));
    comboBox->insertSeparator(comboBox->count());

    // Checksum + legacy
    comboBox->addItem("Adler-32", QVariant::fromValue(HashType::Adler32));
    comboBox->addItem("Tiger", QVariant::fromValue(HashType::Tiger));
    comboBox->addItem("GOST R 34.11-94", QVariant::fromValue(HashType::GOST));
    comboBox->insertSeparator(comboBox->count());

    // Streebog
    comboBox->addItem("Streebog-256", QVariant::fromValue(HashType::Streebog256));
    comboBox->addItem("Streebog-512", QVariant::fromValue(HashType::Streebog512));

    // Ustawienie domyślnego
    comboBox->setCurrentText("SHA-256");
}

void MainWindow::on_compareBtn_clicked()
{
    const QString path = ui->path->text().trimmed();
    const QString expectedHash = ui->hash->text().trimmed();

    if (!validatePath(path)) return;

    if (!validateHash(expectedHash)) {
        ui->info->setText("Hash is empty");
        setStatus(Status::Waiting);
        return;
    }

    QVariant data = ui->hashingSystem->currentData();
    if (!data.isValid()) {
        ui->info->setText("Invalid hash algorithm selected.");
        setStatus(Status::Waiting);
        return;
    }
    ui->compareBtn->setEnabled(false);
    HashType algo = data.value<HashType>();

    setStatus(Status::InProgress);

    auto future = QtConcurrent::run([this, path, algo]() {
        return computeHash(path, algo);
    });

    hashWatcher->setFuture(future);
}


QString MainWindow::computeHash(const QString &filePath, MainWindow::HashType algo)
{
    switch (algo) {

    case MainWindow::HashType::SHA256:
        return hashSHA256(filePath);

    case MainWindow::HashType::SHA224:
        return hashSHA224(filePath);

    case MainWindow::HashType::SHA384:
        return hashSHA384(filePath);

    case MainWindow::HashType::SHA512:
        return hashSHA512(filePath);

    case MainWindow::HashType::SHA512_256:
        return hashSHA512_256(filePath);

    case MainWindow::HashType::SHA512_224:
        return hashSHA512_224(filePath);

    case MainWindow::HashType::SHA1:
        return hashSHA1(filePath);

    case MainWindow::HashType::SHA3_224:
        return hashSHA3_224(filePath);

    case MainWindow::HashType::SHA3_256:
        return hashSHA3_256(filePath);

    case MainWindow::HashType::SHA3_384:
        return hashSHA3_384(filePath);

    case MainWindow::HashType::SHA3_512:
        return hashSHA3_512(filePath);

    case MainWindow::HashType::MD5:
        return hashMD5(filePath);

    case MainWindow::HashType::BLAKE2b:
        return hashBLAKE2b(filePath);

    case MainWindow::HashType::BLAKE2s:
        return hashBLAKE2s(filePath);

    case MainWindow::HashType::BLAKE3:
        return hashBLAKE3(filePath);

    case MainWindow::HashType::CRC32:
        return hashCRC32(filePath);

    case MainWindow::HashType::CRC64:
        return hashCRC64(filePath);

    case MainWindow::HashType::RIPEMD160:
        return hashRIPEMD160(filePath);

    case MainWindow::HashType::Whirlpool:
        return hashWhirlpool(filePath);

    case MainWindow::HashType::Keccak256:
        return hashKeccak256(filePath);

    case MainWindow::HashType::Keccak512:
        return hashKeccak512(filePath);

    case MainWindow::HashType::xxHash32:
        return hashXxHash32(filePath);

    case MainWindow::HashType::xxHash64:
        return hashXxHash64(filePath);

    case MainWindow::HashType::xxHash3:
        return hashXxHash3(filePath);

    case MainWindow::HashType::Adler32:
        return hashAdler32(filePath);

    case MainWindow::HashType::Tiger:
        return hashTiger(filePath);

    case MainWindow::HashType::GOST:
        return hashGOST(filePath);

    case MainWindow::HashType::Streebog256:
        return hashStreebog256(filePath);

    case MainWindow::HashType::Streebog512:
        return hashStreebog512(filePath);

    case MainWindow::HashType::AutoDetect:
        // TODO: automatyczne wykrywanie algorytmu
        return QString();
    }

    return QString();
}
