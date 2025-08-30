#ifndef NOTEWINDOW_H
#define NOTEWINDOW_H

#include <QWidget>

namespace Ui {
class NoteWindow;
}

class NoteWindow : public QWidget
{
    Q_OBJECT

public:
    explicit NoteWindow(QWidget *parent = nullptr);
    ~NoteWindow();

private:
    Ui::NoteWindow *ui;
};

#endif // NOTEWINDOW_H
