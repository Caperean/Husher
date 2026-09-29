#include "mainwindow.h"
#include "ui_mainwindow.h"   
#include <QString>
#include <QFileDialog>
#include <QAbstractItemView>
#include <QFutureWatcher>
#include "algorithms.hpp" 
#include <QClipboard>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)   
{
    ui->setupUi(this);         

    ui->hash->setPlaceholderText("Hash to compare");
    ui->path->setPlaceholderText("Path");

   

    ui->info->setReadOnly(true);

    ui->statusView->setFixedSize(256, 192);
    ui->statusView->setAlignment(Qt::AlignCenter);
    ui->statusView->setScaledContents(true);
    setStatus(Status::Waiting);

    setHashingAlgorithm(ui->hashingSystem);
    ui->hashingSystem->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->hashingSystem->view()->setMinimumWidth(190);

    hashWatcher = new QFutureWatcher<QString>(this);

connect(hashWatcher, &QFutureWatcher<QString>::finished, this, [this]() {
    QString computedHash = hashWatcher->result();
    QString expectedHash = ui->hash->text().trimmed();

    ui->compareBtn->setEnabled(true);

    if (computedHash.isEmpty()) {
        ui->info->setText("Failed to compute hash.");
        setStatus(Status::NotFound);
        return;
    }
    lastComputedHash = computedHash;
    if (computedHash.compare(expectedHash, Qt::CaseInsensitive) == 0)
        setStatus(Status::Confirm);
    else
        setStatus(Status::Rejected);
    
});

}

void MainWindow::setStatus(Status status)
{
    switch (status) {
    case Status::Waiting:
        ui->statusView->setPixmap(QPixmap(":/images/waiting.png"));
        ui->info->setText("Waiting for hash and path...");
        break;

    case Status::NotFound:
        ui->statusView->setPixmap(QPixmap(":/images/notFound.png"));
        ui->info->setText("File not found.");
        break;

    case Status::Confirm:
        ui->statusView->setPixmap(QPixmap(":/images/confirm.png"));
        ui->info->setText("Hash matches.");
        break;

    case Status::Rejected:
        ui->statusView->setPixmap(QPixmap(":/images/rejected.png"));
        ui->info->setText("Hash does not match!");
        break;

    case Status::InProgress:
        ui->statusView->setPixmap(QPixmap(":/images/in_progress.png"));
        ui->info->setText("Calculating hash in progress...");
        break;
    }
}


bool MainWindow::validatePath(const QString &path)
{
    if (path.isEmpty()) {
        ui->info->setText(" Path is empty");
        return false;
    }

    QFileInfo info(path);

    if (!info.exists()) {
        setStatus(Status::NotFound);
        return false;
    }

    if (!info.isDir() && !info.isFile()) {
        setStatus(Status::NotFound);
        return false;
    }

    return true;
}
bool MainWindow::validateHash(const QString &hash)
{
    if (hash.isEmpty()) {
        ui->info->setText("Hash is empty");
        return false;
    }
    return true;
}


void MainWindow::on_pickBtn_clicked()
{
    QString file = QFileDialog::getOpenFileName(
        this,
        tr("Wybierz plik"),
        QDir::homePath(),
        tr("Wszystkie pliki (*.*)")
    );

    if (!file.isEmpty()) {
        ui->path->setText(file);
    }
}
void MainWindow::on_copyBtn_clicked()
{
    if (lastComputedHash.isEmpty()) {
        ui->info->setText("No computed hash to copy.");
        return;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(lastComputedHash);

    ui->info->setText("File hash copied to clipboard.");
}


MainWindow::~MainWindow()
{
    delete ui;
}//cmake --build . --parallel
//
