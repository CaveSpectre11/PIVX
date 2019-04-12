#ifndef DEFAULTDIALOG_H
#define DEFAULTDIALOG_H

#include <QDialog>

namespace Ui {
class DefaultDialog;
}

class DefaultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DefaultDialog(QWidget *parent = nullptr);
    ~DefaultDialog();

    void setText(QString title = "", QString message = "", QString okBtnText = "", QString cancelBtnText = "");

private:
    Ui::DefaultDialog *ui;
};

#endif // DEFAULTDIALOG_H
