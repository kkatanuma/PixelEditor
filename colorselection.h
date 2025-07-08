#ifndef COLORSELECTION_H
#define COLORSELECTION_H

#include <QWidget>

namespace Ui {
    class colorSelection;
}

class colorSelection : public QWidget
{
    Q_OBJECT

public:
    explicit colorSelection(QWidget *parent = nullptr);
    ~colorSelection();

signals:
    void setColor(QColor);

public slots:
    void applyColor();
    void updateNewR(int);
    void updateNewG(int);
    void updateNewB(int);
    void updateNewA(int);
    void updateNewColor(int);
    void setCurrentColor(QColor);

private:
    Ui::colorSelection *ui;
    QColor newColor, currentColor;

    void setNewColor();
    void sendColor();
    void closeEvent(QCloseEvent *event);
    QString getColorString(QColor);

};

#endif // COLORSELECTION_H
