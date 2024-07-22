#include <QWidgetAction>
#include <QMessageBox>
#include "eventeditform.h"
#include "ui_eventeditform.h"

EventEditForm::EventEditForm(Database *db, QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::EventEditForm)
{
    ui->setupUi(this);
    m_pdb = db;
    m_pColorDlg = new QColorDialog(this);
    QString query = "SELECT DISTINCT p_color FROM alarms_tbl";
    m_pdb->execQuery(query);
    int index = 0;
    while (m_pdb->nextRecord())
    {
        QStringList colors = m_pdb->fetchValue(0).toString().split(",");
        int red = colors[0].toInt();
        int green = colors[1].toInt();
        int blue = colors[2].toInt();
        m_pColorDlg->setCustomColor(index, QColor(red, green, blue));
        index++;
    }

    query = "SELECT DISTINCT s_type FROM tags_tbl ORDER BY s_type";
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return;
    }
    while (m_pdb->nextRecord())
    {
        ui->deviceBox->addItem(m_pdb->fetchValue(0).toString());
    }
    ui->deviceBox->addItem("RADTP");

    ui->typeBox->addItem("Норма", 1);
    ui->typeBox->addItem("Отказ", 10);
    ui->typeBox->addItem("Отказ связи", 7);
    ui->typeBox->addItem("ПУ", 2);
    ui->typeBox->addItem("АУ", 3);
    ui->typeBox->addItem("Клапан открыт", 11);
    ui->typeBox->addItem("Клапан закрыт", 12);
}

EventEditForm::~EventEditForm()
{
    delete ui;
    m_pColorDlg->deleteLater();
}

bool EventEditForm::editRecord(QString key)
{
    setWindowTitle("Редактировать запись");
    QString query = QString("SELECT i_code, i_numbit, i_bit, p_color, s_description, s_type, i_priorit, i_type_event "
                    "FROM alarms_tbl WHERE i_code = '%1'").arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->codeEdit->setText(m_pdb->fetchValue(0).toString());
        ui->bitEdit->setText(m_pdb->fetchValue(1).toString());
        ui->valueEdit->setText(m_pdb->fetchValue(2).toString());
        ui->colorEdit->setText(m_pdb->fetchValue(3).toString());
        QStringList colors = m_pdb->fetchValue(3).toString().split(",");
        int red = colors[0].toInt();
        int green = colors[1].toInt();
        int blue = colors[2].toInt();
        m_pColorDlg->setCurrentColor(QColor(red, green, blue));
        ui->colorEdit->setStyleSheet(
                    QString("color: rgb(0, 0, 0); background-color: rgb(%1, %2, %3);")
                    .arg(red).arg(green).arg(blue));
        ui->descEdit->setText(m_pdb->fetchValue(4).toString());
        ui->deviceBox->setCurrentIndex(ui->deviceBox->findText(m_pdb->fetchValue(5).toString()));
        ui->priorityEdit->setText(m_pdb->fetchValue(6).toString());
        ui->typeBox->setCurrentIndex(ui->typeBox->findData(m_pdb->fetchValue(7).toString()));
    }
    m_key = key;
    m_isEdit = true;
    return true;
}

bool EventEditForm::copyRecord(QString key)
{
    setWindowTitle("Копировать запись");
    QString query = QString("SELECT i_code, i_numbit, i_bit, p_color, s_description, s_type, i_priorit, i_type_event "
                    "FROM alarms_tbl WHERE i_code = '%1'").arg(key);
    if (!m_pdb->execQuery(query)) {
        m_pdb->showError(this);
        return false;
    }
    if (m_pdb->nextRecord()) {
        ui->codeEdit->setText(m_pdb->fetchValue(0).toString());
        ui->bitEdit->setText(m_pdb->fetchValue(1).toString());
        ui->valueEdit->setText(m_pdb->fetchValue(2).toString());
        ui->colorEdit->setText(m_pdb->fetchValue(3).toString());
        QStringList colors = m_pdb->fetchValue(3).toString().split(",");
        int red = colors[0].toInt();
        int green = colors[1].toInt();
        int blue = colors[2].toInt();
        m_pColorDlg->setCurrentColor(QColor(red, green, blue));
        ui->colorEdit->setStyleSheet(
                    QString("color: rgb(0, 0, 0); background-color: rgb(%1, %2, %3);")
                    .arg(red).arg(green).arg(blue));
        ui->descEdit->setText(m_pdb->fetchValue(4).toString());
        ui->deviceBox->setCurrentIndex(ui->deviceBox->findText(m_pdb->fetchValue(5).toString()));
        ui->priorityEdit->setText(m_pdb->fetchValue(6).toString());
        ui->typeBox->setCurrentIndex(ui->typeBox->findData(m_pdb->fetchValue(7).toString()));
    }
    m_key = key;
    ui->codeEdit->setFocus();
    ui->codeEdit->selectAll();
    return true;
}

void EventEditForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit closed();
}



void EventEditForm::on_colorButton_clicked()
{
    m_pColorDlg->exec();
    QColor color = m_pColorDlg->selectedColor();
    ui->colorEdit->setStyleSheet(
                QString("color: rgb(0, 0, 0); background-color: rgb(%1, %2, %3);")
                .arg(color.red()).arg(color.green()).arg(color.blue()));
    ui->colorEdit->setText(QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue()));
}


void EventEditForm::on_cancelButton_clicked()
{
    close();
}


void EventEditForm::on_okButton_clicked()
{
    QString query;
    if (ui->codeEdit->text().simplified().isEmpty()) {
        QMessageBox::critical(this, "Ошибка", "Поле 'Код' не может быть пустым");
        return;
    }
    if (!m_key.isEmpty()) {
        if (m_isEdit) {
            query = "UPDATE alarms_tbl SET i_code = '%1', i_numbit = '%2', i_bit = '%3', p_color='%4', s_description = '%5', "
                    "s_type = '%6', i_priorit = '%7', i_type_event = '%8' "
                    "WHERE s_name = '%9'";
        }
        else
        {
            query = "INSERT INTO alarms_tbl (i_code, i_numbit, i_bit, p_color, s_description, s_type, i_priorit, i_type_event) "
                    "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')";
        }
    } else {
        query = "INSERT INTO networksettings_tbl (i_code, i_numbit, i_bit, p_color, s_description, s_type, i_priorit, i_type_event) "
                "VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8')";
    }
    query = query.arg(ui->codeEdit->text().simplified()).arg(ui->bitEdit->text().simplified()).arg(ui->valueEdit->text().simplified())
            .arg(ui->colorEdit->text().simplified()).arg(ui->descEdit->text().simplified()).arg(ui->deviceBox->currentText().simplified())
            .arg(ui->priorityEdit->text().simplified()).arg(ui->typeBox->currentData().toString()).arg(m_key);
    if (!m_pdb->execQuery(query))  {
        m_pdb->showError(this);
        return;
    }
    close();
    emit saved();
}

