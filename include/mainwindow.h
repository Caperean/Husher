#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QComboBox>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>


namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum class Status {
        Waiting,
        NotFound,
        Confirm,
        Rejected,
        InProgress
    };

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
QFutureWatcher<QString> *hashWatcher;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setHashingAlgorithm(QComboBox *comboBox);  
private:
    QString Path;
    Ui::MainWindow *ui;
    QString lastComputedHash;


    QString computeHash(const QString &filePath, MainWindow::HashType algo);
    bool validatePath(const QString &path);
    bool validateHash(const QString &hash);
    void setStatus(Status status);

private slots:
    void on_pickBtn_clicked();
    void on_compareBtn_clicked();
    void on_copyBtn_clicked();
};

#endif // MAINWINDOW_H
