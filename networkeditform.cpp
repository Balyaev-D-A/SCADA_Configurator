#include <QMessageBox>
#include "networkeditform.h"
#include "ui_networkeditform.h"

NetworkEditForm::NetworkEditForm(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::networkEditForm)
{
    ui->setupUi(this);
    QStringList protocols = {"SM1820", "RADTP"};
    ui->protocolBox->addItems(protocols);
}

NetworkEditForm::~NetworkEditForm()
{
    delete ui;
}

void NetworkEditForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit closed();
}

void NetworkEditForm::on_okButton_clicked()
{
    QString query;
    if (ui->nameEdit->text().simplified().isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Поле 'Название' не может быть пустым");
        return;
    }
    if (!m_key.isEmpty()) {
        if (m_isEdit) {
            query = "UPDATE networksettings_tbl SET s_name = '%1', s_ip = '%2', s_ip_reserve = '%3', i_port='%4', s_driver = '%5' "
                    "WHERE s_name = '%6'";
        }
        else
        {
            query = "INSERT INTO networksettings_tbl (s_name, s_ip, s_ip_reserve, i_port, s_driver) VALUES ('%1', '%2', '%3', '%4', '%5')";
        }
    } else {
        query = "INSERT INTO networksettings_tbl (s_name, s_ip, s_ip_reserve, i_port, s_driver) VALUES ('%1', '%2', '%3', '%4', '%5')";
    }
    query = query.arg(ui->nameEdit->text().simplified()).arg(ui->addressEdit->text().simplified()).arg(ui->raddressEdit->text().simplified())
            .arg(ui->portEdit->text().simplified()).arg(ui->protocolBox->currentText().simplified()).arg(m_key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    close();
    emit saved();
}


void NetworkEditForm::on_cancelButton_clicked()
{
    close();
}

bool NetworkEditForm::editRecord(QString key)
{
    QString query = "SELECT s_name, s_ip, s_ip_reserve, i_port, s_driver FROM networksettings_tbl WHERE s_name = '%1'";
    query = query.arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->nameEdit->setText(m_pdb->fetchValue(0).toString());
        ui->addressEdit->setText(m_pdb->fetchValue(1).toString());
        ui->raddressEdit->setText(m_pdb->fetchValue(2).toString());
        ui->portEdit->setText(m_pdb->fetchValue(3).toString());
        ui->protocolBox->setCurrentIndex(ui->protocolBox->findText(m_pdb->fetchValue(4).toString()));
        m_key = key;
        m_isEdit = true;
        return true;
    }
    return false;
}

bool NetworkEditForm::copyRecord(QString key)
{
    QString query = "SELECT s_name, s_ip, s_ip_reserve, i_port, s_driver FROM networksettings_tbl WHERE s_name = '%1'";
    query = query.arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->nameEdit->setText(m_pdb->fetchValue(0).toString());
        ui->addressEdit->setText(m_pdb->fetchValue(1).toString());
        ui->raddressEdit->setText(m_pdb->fetchValue(2).toString());
        ui->portEdit->setText(m_pdb->fetchValue(3).toString());
        ui->protocolBox->setCurrentIndex(ui->protocolBox->findText(m_pdb->fetchValue(4).toString()));
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return true;
    }
    return false;
}
