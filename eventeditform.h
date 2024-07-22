#ifndef EVENTEDITFORM_H
#define EVENTEDITFORM_H

#include <QWidget>
#include <QColorDialog>
#include "database.h"

namespace Ui {
class EventEditForm;
}

class EventEditForm : public QWidget
{
    Q_OBJECT
public:
    explicit EventEditForm(Database *db, QWidget *parent = nullptr);
    ~EventEditForm();
    bool editRecord(QString key);
    bool copyRecord(QString key);
signals:
    void closed();
    void saved();
protected:
    void closeEvent(QCloseEvent *e);
private:
    Ui::EventEditForm *ui;
    Database *m_pdb;
    QColorDialog *m_pColorDlg;
    QString m_key;
    bool m_isEdit = false;

private slots:

    void on_colorButton_clicked();
    void on_cancelButton_clicked();
    void on_okButton_clicked();
};

#endif // EVENTEDITFORM_H
