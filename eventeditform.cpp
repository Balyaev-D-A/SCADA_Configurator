#include <QWidgetAction>
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
    ui->typeBox->addItem("RADTP");

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
    return true;
}

void EventEditForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit closed();
}



void EventEditForm::on_toolButton_clicked()
{
    m_pColorDlg->show();
}

