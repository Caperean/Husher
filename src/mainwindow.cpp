#include "mainwindow.h"
#include "ui_mainwindow.h"   
#include "utils.hpp"
#include <QString>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)   
{
    ui->setupUi(this);         

    ui->hash->setPlaceholderText("Hash to compare");
    ui->path->setPlaceholderText("Path");

    connect(ui->pickBtn, &QPushButton::clicked,
        this, &MainWindow::on_pickBtn_clicked);
    connect(ui->compareBtn, &QPushButton::clicked,
        this, &MainWindow::on_compareBtn_clicked);

    ui->info->setReadOnly(true);

    ui->statusView->setFixedSize(128, 128);
    ui->statusView->setScaledContents(false);
    setStatus(Status::Waiting);

    
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
        ui->info->setText("App or file not found.");
        break;

    case Status::Confirm:
        ui->statusView->setPixmap(QPixmap(":/images/confirm.png"));
        ui->info->setText("Hash matches.");
        break;

    case Status::Rejected:
        ui->statusView->setPixmap(QPixmap(":/images/rejected.png"));
        ui->info->setText("Hash does not match!");
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
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Select Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    if (!dir.isEmpty()) {
        ui->path->setText(dir);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
