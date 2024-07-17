#ifndef DEVICEEDITFORM_H
#define DEVICEEDITFORM_H

#include <QWidget>
#include "database.h"

namespace Ui {
class DeviceEditForm;
}

class DeviceEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceEditForm(QWidget *parent = nullptr);
    ~DeviceEditForm();
    void setDatabase(Database *db) {m_pdb = db;}
    bool editRecord(QString key);
    bool copyRecord(QString key);

signals:
    void saved();
    void closed();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    Ui::DeviceEditForm *ui;
    Database *m_pdb;
    QString m_key;
    bool m_isEdit;
};

#endif // DEVICEEDITFORM_H
