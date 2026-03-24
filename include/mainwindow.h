#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum class Status {
    Waiting,
    NotFound,
    Confirm,
    Rejected
};

public:
    QString Path;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    setHashingAlgorithm(QComboBox *comboBox);

private:
    bool validatePath(const QString &path);
    bool validateHash(const QString &hash);
    void setStatus(Status status);

private slots:
    void on_pickBtn_clicked();
    void on_compareBtn_clicked();

};

#endif // MAINWINDOW_H
