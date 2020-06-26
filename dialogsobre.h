#ifndef DIALOGSOBRE_H
#define DIALOGSOBRE_H

#include <QDialog>

namespace Ui {
    class DialogSobre;
}

class DialogSobre : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSobre(QWidget *parent = 0);
    ~DialogSobre();

private:
    Ui::DialogSobre *ui;
};

#endif // DIALOGSOBRE_H
