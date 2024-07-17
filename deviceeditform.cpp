#include <QMessageBox>
#include "deviceeditform.h"
#include "ui_deviceeditform.h"

DeviceEditForm::DeviceEditForm(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::DeviceEditForm)
{
    ui->setupUi(this);
    QStringList protocols = {"SM1820", "RADTP"};
    ui->protocolBox->addItems(protocols);
}

DeviceEditForm::~DeviceEditForm()
{
    delete ui;
}

void DeviceEditForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit closed();
}

bool DeviceEditForm::editRecord(QString key)
{
    QString query = "SELECT s_name, s_name_plc1, s_name_plc2, s_name_dev, s_driver, s_addr_driver, s_descr, "
                    "s_unit, s_room, s_type, i_num_form, i_num_comma, d_low_thr, d_hight_thr, d_alarm_thr, "
                    "d_mix, d_max, i_type_sig, i_arch_time_ms FROM tags_tbl WHERE s_name = '%1'";
    query = query.arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->nameEdit->setText(m_pdb->fetchValue(0).toString());
        ui->plc1Edit->setText(m_pdb->fetchValue(1).toString());
        ui->plc2Edit->setText(m_pdb->fetchValue(2).toString());
        ui->deviceEdit->setText(m_pdb->fetchValue(3).toString());
        ui->protocolBox->setCurrentIndex(ui->protocolBox->findText(m_pdb->fetchValue(4).toString()));
        ui->s_addr_driverEdit->setText(m_pdb->fetchValue(5).toString());
        ui->descrTextEdit->document()->setPlainText(m_pdb->fetchValue(6).toString());
        ui->unitEdit->setText(m_pdb->fetchValue(7).toString());
        ui->roomEdit->setText(m_pdb->fetchValue(8).toString());
        ui->typeEdit->setText(m_pdb->fetchValue(9).toString());
        ui->numEdit->setText(m_pdb->fetchValue(10).toString());
        ui->numcommaEdit->setText(m_pdb->fetchValue(11).toString());
        ui->tecnEdit->setText(m_pdb->fetchValue(12).toString());
        ui->warnEdit->setText(m_pdb->fetchValue(13).toString());
        ui->emergEdit->setText(m_pdb->fetchValue(14).toString());
        ui->minEdit->setText(m_pdb->fetchValue(15).toString());
        ui->maxEdit->setText(m_pdb->fetchValue(16).toString());
        ui->i_type_sigEdit->setText(m_pdb->fetchValue(17).toString());
        ui->i_arch_time_msEdit->setText(m_pdb->fetchValue(18).toString());
        m_key = key;
        m_isEdit = true;
        return true;
    }
    return false;
}

bool DeviceEditForm::copyRecord(QString key)
{
    QString query = "SELECT s_name, s_name_plc1, s_name_plc2, s_name_dev, s_driver, s_addr_driver, s_descr, "
                    "s_unit, s_room, s_type, i_num_form, i_num_comma, d_low_thr, d_hight_thr, d_alarm_thr, "
                    "d_mix, d_max, i_type_sig, i_arch_time_ms FROM tags_tbl WHERE s_name = '%1'";
    query = query.arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->nameEdit->setText(m_pdb->fetchValue(0).toString());
        ui->plc1Edit->setText(m_pdb->fetchValue(1).toString());
        ui->plc2Edit->setText(m_pdb->fetchValue(2).toString());
        ui->deviceEdit->setText(m_pdb->fetchValue(3).toString());
        ui->protocolBox->setCurrentIndex(ui->protocolBox->findText(m_pdb->fetchValue(4).toString()));
        ui->s_addr_driverEdit->setText(m_pdb->fetchValue(5).toString());
        ui->descrTextEdit->document()->setPlainText(m_pdb->fetchValue(6).toString());
        ui->unitEdit->setText(m_pdb->fetchValue(7).toString());
        ui->roomEdit->setText(m_pdb->fetchValue(8).toString());
        ui->typeEdit->setText(m_pdb->fetchValue(9).toString());
        ui->numEdit->setText(m_pdb->fetchValue(10).toString());
        ui->numcommaEdit->setText(m_pdb->fetchValue(11).toString());
        ui->tecnEdit->setText(m_pdb->fetchValue(12).toString());
        ui->warnEdit->setText(m_pdb->fetchValue(13).toString());
        ui->emergEdit->setText(m_pdb->fetchValue(14).toString());
        ui->minEdit->setText(m_pdb->fetchValue(15).toString());
        ui->maxEdit->setText(m_pdb->fetchValue(16).toString());
        ui->i_type_sigEdit->setText(m_pdb->fetchValue(17).toString());
        ui->i_arch_time_msEdit->setText(m_pdb->fetchValue(18).toString());
        ui->nameEdit->setFocus();
        ui->nameEdit->selectAll();
        return true;
    }
    return false;
}



void DeviceEditForm::on_cancelButton_clicked()
{
    close();
}


void DeviceEditForm::on_okButton_clicked()
{
    QString query;
    if (ui->nameEdit->text().simplified().isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Поле 'Метка' не может быть пустым");
        return;
    }
    if (!m_key.isEmpty()) {
        if (m_isEdit) {
            query = "UPDATE tags_tbl SET s_name = '%1', s_name_plc1 = '%2', s_name_plc2 = '%3', s_name_dev = '%4', s_driver = '%5', "
                    "s_addr_driver = '%6', s_descr = '%7', s_unit = '%8', s_room = '%9', s_type = '%10', i_num_form = '%11', "
                    "i_num_comma = '%12', d_low_thr = '%13', d_hight_thr = '%14', d_alarm_thr = '%15', "
                    "d_mix = '%16', d_max = '%17', i_type_sig = '%18', i_arch_time_ms = '%19'"
                    "WHERE s_name = '%20'";
        }
        else
        {
            query = "INSERT INTO tags_tbl (s_name, s_name_plc1, s_name_plc2, s_name_dev, s_driver, s_addr_driver, s_descr, "
                    "s_unit, s_room, s_type, i_num_form, i_num_comma, d_low_thr, d_hight_thr, d_alarm_thr, "
                    "d_mix, d_max, i_type_sig, i_arch_time_ms) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', "
                    "'%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19')";
        }
    } else {
        query = "INSERT INTO tags_tbl (s_name, s_name_plc1, s_name_plc2, s_name_dev, s_driver, s_addr_driver, s_descr, "
                "s_unit, s_room, s_type, i_num_form, i_num_comma, d_low_thr, d_hight_thr, d_alarm_thr, "
                "d_mix, d_max, i_type_sig, i_arch_time_ms) VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9', "
                "'%10', '%11', '%12', '%13', '%14', '%15', '%16', '%17', '%18', '%19')";
    }
    query = query.arg(ui->nameEdit->text().simplified()).arg(ui->plc1Edit->text().simplified()).arg(ui->plc2Edit->text().simplified())
            .arg(ui->deviceEdit->text().simplified()).arg(ui->protocolBox->currentText().simplified()).arg(ui->s_addr_driverEdit->text().simplified())
            .arg(ui->descrTextEdit->document()->toPlainText()).arg(ui->unitEdit->text().simplified()).arg(ui->roomEdit->text().simplified())
            .arg(ui->typeEdit->text().simplified()).arg(ui->numEdit->text().simplified()).arg(ui->numcommaEdit->text().simplified())
            .arg(ui->tecnEdit->text().simplified()).arg(ui->warnEdit->text().simplified()).arg(ui->emergEdit->text().simplified())
            .arg(ui->minEdit->text().simplified()).arg(ui->maxEdit->text().simplified()).arg(ui->i_type_sigEdit->text().simplified())
            .arg(ui->i_arch_time_msEdit->text().simplified()).arg(m_key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    close();
    emit saved();
}

