#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "connectionform.h"
#include "networkeditform.h"
#include "deviceeditform.h"
#include "eventeditform.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pdb = new Database();
    //ui->eventTable->hideColumn(0);
    connect(ui->connectAction, &QAction::triggered, this, &MainWindow::showConnectForm);
    connect(ui->netRefreshButton, &QToolButton::clicked, this, &MainWindow::updateNetworkTable);
    connect(ui->networkTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::networkTableCellDblClicked);
    connect(ui->devRefreshButton, &QToolButton::clicked, this, &MainWindow::updateDeviceTable);
    connect(ui->deviceTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::deviceTableCellDblClicked);
    connect(ui->eventTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::eventTableCellDblClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_pdb;
}

void MainWindow::showEvent(QShowEvent *e)
{
    QMainWindow::showEvent(e);
    ui->networkTable->sortByColumn(0, Qt::AscendingOrder);
    ui->deviceTable->sortByColumn(0, Qt::AscendingOrder);
    ui->eventTable->sortByColumn(1, Qt::AscendingOrder);
    openDb();
}

void MainWindow::showConnectForm()
{
    ConnectionForm *cf = new ConnectionForm(this);
    connect(cf, &ConnectionForm::connectClicked, this, &MainWindow::settingsSaved);
    cf->show();
}

void MainWindow::openDb()
{
    Settings::ConnSettings cs = Settings::instance()->getConnSettings();
    if (cs.host.isEmpty()) {
        showConnectForm();
    }
    else {
        if (!m_pdb->open(cs.host, cs.database, cs.user, cs.password)) {
            m_pdb->showError(this);
            return;
        }
        updateNetworkTable();
        updateDeviceTable();
        updateEventTable();
    }
}

void MainWindow::settingsSaved()
{
    if (sender()) sender()->deleteLater();
    openDb();
}

void MainWindow::formClosed()
{
    if (sender()) sender()->deleteLater();
}

void MainWindow::updateNetworkTable()
{
    while (ui->networkTable->rowCount()) ui->networkTable->removeRow(0);
    QString query = "SELECT s_name, s_ip, s_ip_reserve, i_port, s_driver FROM networksettings_tbl";
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    ui->networkTable->setSortingEnabled(false);
    for (int i=0; m_pdb->nextRecord(); i++)
    {
        ui->networkTable->insertRow(i);
        for (int j=0; j<5; j++)
        {
            ui->networkTable->setItem(i, j, new QTableWidgetItem(m_pdb->fetchValue(j).toString()));
        }
    }
    ui->networkTable->setSortingEnabled(true);
    ui->networkTable->resizeColumnsToContents();
}

void MainWindow::networkTableCellDblClicked(int row, int column)
{
    QString key = ui->networkTable->item(row, 0)->text();
    NetworkEditForm *nef = new NetworkEditForm(this);
    nef->setDatabase(m_pdb);
    if (!nef->editRecord(key)) {
        nef->deleteLater();
        return;
    }
    connect(nef, &NetworkEditForm::saved, this, &MainWindow::updateNetworkTable);
    connect(nef, &NetworkEditForm::closed, this, &MainWindow::formClosed);
    nef->show();
}


void MainWindow::on_netAddButton_clicked()
{
    NetworkEditForm *nef = new NetworkEditForm(this);
    nef->setDatabase(m_pdb);
    connect(nef, &NetworkEditForm::saved, this, &MainWindow::updateNetworkTable);
    connect(nef, &NetworkEditForm::closed, this, &MainWindow::formClosed);
    nef->show();
}


void MainWindow::on_netEditButton_clicked()
{
    int curRow = ui->networkTable->currentRow();
    if (curRow < 0) return;
    networkTableCellDblClicked(curRow, 0);
}


void MainWindow::on_netDeleteButton_clicked()
{
    int curRow = ui->networkTable->currentRow();
    if (curRow < 0) return;
    if (QMessageBox::question(this, "Внимание!!!", "Вы действительно хотите удалить запись?") == QMessageBox::Yes) {
        QString query = "DELETE FROM networksettings_tbl WHERE s_name = '%1'";
        query = query.arg(ui->networkTable->item(curRow, 0)->text());
        if (!m_pdb->execQuery(query)) {
            m_pdb->showError(this);
            return;
        }
        updateNetworkTable();
    }
}


void MainWindow::on_netCopyButton_clicked()
{
    int curRow = ui->networkTable->currentRow();
    if (curRow < 0) return;
    QString key = ui->networkTable->item(curRow, 0)->text();
    NetworkEditForm *nef = new NetworkEditForm(this);
    nef->setDatabase(m_pdb);
    if (!nef->copyRecord(key)) {
        nef->deleteLater();
        return;
    }
    connect(nef, &NetworkEditForm::saved, this, &MainWindow::updateNetworkTable);
    connect(nef, &NetworkEditForm::closed, this, &MainWindow::formClosed);
    nef->show();
}

void MainWindow::updateDeviceTable()
{
    while (ui->deviceTable->rowCount()) ui->deviceTable->removeRow(0);
    QString query = "SELECT s_name, s_type, s_unit, s_room, s_name_plc1, s_name_plc2, s_driver, s_descr FROM tags_tbl";
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    ui->deviceTable->setSortingEnabled(false);
    for (int i=0; m_pdb->nextRecord(); i++)
    {
        ui->deviceTable->insertRow(i);
        for (int j=0; j<8; j++)
        {
            ui->deviceTable->setItem(i, j, new QTableWidgetItem(m_pdb->fetchValue(j).toString()));
        }
    }
    ui->deviceTable->setSortingEnabled(true);
    ui->deviceTable->resizeColumnsToContents();
}

void MainWindow::deviceTableCellDblClicked(int row, int column)
{
    QString key = ui->deviceTable->item(row, 0)->text();

    DeviceEditForm *def = new DeviceEditForm(this);
    def->setDatabase(m_pdb);
    def->editRecord(key);
    connect(def, &DeviceEditForm::saved, this, &MainWindow::updateDeviceTable);
    connect(def, &DeviceEditForm::closed, this, &MainWindow::formClosed);
    def->show();
}

void MainWindow::on_devAddButton_clicked()
{
    DeviceEditForm *def = new DeviceEditForm(this);
    def->setDatabase(m_pdb);
    connect(def, &DeviceEditForm::saved, this, &MainWindow::updateDeviceTable);
    connect(def, &DeviceEditForm::closed, this, &MainWindow::formClosed);
    def->show();
}

void MainWindow::on_devEditButton_clicked()
{
    int curRow = ui->deviceTable->currentRow();
    if (curRow < 0) return;

    QString key = ui->deviceTable->item(curRow, 0)->text();

    DeviceEditForm *def = new DeviceEditForm(this);
    def->setDatabase(m_pdb);
    def->editRecord(key);
    connect(def, &DeviceEditForm::saved, this, &MainWindow::updateDeviceTable);
    connect(def, &DeviceEditForm::closed, this, &MainWindow::formClosed);
    def->show();
}

void MainWindow::on_devDeleteButton_clicked()
{
    int curRow = ui->deviceTable->currentRow();
    if (curRow < 0) return;
    if (QMessageBox::question(this, "Внимание!!!", "Вы действительно хотите удалить запись?") == QMessageBox::Yes) {
        QString query = "DELETE FROM tags_tbl WHERE s_name = '%1'";
        query = query.arg(ui->deviceTable->item(curRow, 0)->text());
        if (!m_pdb->execQuery(query)) {
            m_pdb->showError(this);
            return;
        }
        updateDeviceTable();
    }
}


void MainWindow::on_devCopyButton_clicked()
{
    int curRow = ui->deviceTable->currentRow();
    if (curRow < 0) return;

    QString key = ui->deviceTable->item(curRow, 0)->text();

    DeviceEditForm *def = new DeviceEditForm(this);
    def->setDatabase(m_pdb);
    def->copyRecord(key);
    connect(def, &DeviceEditForm::saved, this, &MainWindow::updateDeviceTable);
    connect(def, &DeviceEditForm::closed, this, &MainWindow::formClosed);
    def->show();
}

void MainWindow::updateEventTable()
{
    while (ui->eventTable->rowCount()) ui->eventTable->removeRow(0);
    QString query = "SELECT i_code, s_type, i_numbit, i_bit, p_color, i_priorit, i_type_event, s_description FROM alarms_tbl";
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    ui->eventTable->setSortingEnabled(false);
    for (int i=0; m_pdb->nextRecord(); i++)
    {
        ui->eventTable->insertRow(i);
        int r, g, b;
        QStringList color = m_pdb->fetchValue(4).toString().split(",");
        r = color[0].toInt();
        g = color[1].toInt();
        b = color[2].toInt();
        for (int j=0; j<8; j++)
        {
            ui->eventTable->setItem(i, j, new QTableWidgetItem(m_pdb->fetchValue(j).toString()));
            ui->eventTable->item(i, j)->setForeground(QBrush(QColor(0,0,0)));
            ui->eventTable->item(i, j)->setBackground(QBrush(QColor(r,g,b)));
        }
    }
    ui->eventTable->setSortingEnabled(true);
    ui->eventTable->resizeColumnsToContents();
}

void MainWindow::eventTableCellDblClicked(int row, int column)
{
    QString key = ui->eventTable->item(row, 0)->text();
    EventEditForm *eef = new EventEditForm(m_pdb, this);
    if (!eef->editRecord(key)) {
        eef->deleteLater();
        return;
    }
    connect(eef, &EventEditForm::saved, this, &MainWindow::updateNetworkTable);
    connect(eef, &EventEditForm::closed, this, &MainWindow::formClosed);
    eef->show();
}
