#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShowEvent>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void showEvent(QShowEvent *e);

private:
    Ui::MainWindow *ui;
    Database *m_pdb;
private slots:
    void openDb();
    void updateNetworkTable();
    void updateDeviceTable();
    void updateEventTable();
    void settingsSaved();
    void formClosed();
    void networkTableCellDblClicked(int row, int column);
    void deviceTableCellDblClicked(int row, int column);
    void eventTableCellDblClicked(int row, int column);
    void showConnectForm();
    void on_netAddButton_clicked();
    void on_netEditButton_clicked();
    void on_netDeleteButton_clicked();
    void on_netCopyButton_clicked();
    void on_devAddButton_clicked();
    void on_devEditButton_clicked();
    void on_devDeleteButton_clicked();
    void on_devCopyButton_clicked();
};
#endif // MAINWINDOW_H
