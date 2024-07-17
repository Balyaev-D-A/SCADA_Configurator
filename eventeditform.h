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
//    bool copyRecord(QString key);
signals:
    void closed();
    void saved();
protected:
    void closeEvent(QCloseEvent *e);
private:
    Ui::EventEditForm *ui;
    Database *m_pdb;
    QColorDialog *m_pColorDlg;

private slots:
    void on_toolButton_clicked();
};

#endif // EVENTEDITFORM_H
