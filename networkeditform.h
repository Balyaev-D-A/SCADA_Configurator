#ifndef NETWORKEDITFORM_H
#define NETWORKEDITFORM_H

#include <QWidget>
#include "database.h"

namespace Ui {
class networkEditForm;
}

class NetworkEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkEditForm(QWidget *parent = nullptr);
    ~NetworkEditForm();
    void setDatabase(Database *db) {m_pdb = db;}
    bool editRecord(QString key);
    bool copyRecord(QString key);

signals:
    void saved();
    void closed();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::networkEditForm *ui;
    Database *m_pdb;
    QString m_key;
    bool m_isEdit = false;
};

#endif // NETWORKEDITFORM_H
