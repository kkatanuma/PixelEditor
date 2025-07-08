
#ifndef STAMPSELECTION_H
#define STAMPSELECTION_H

#include <QWidget>

namespace Ui {
    class StampSelection;
}

class StampSelection : public QWidget
{
    Q_OBJECT
public:
    explicit StampSelection(QWidget *parent = nullptr);
    ~StampSelection();

private:
    Ui::StampSelection *ui;

    void yellowSpriteChosen();
    void blueSpriteChosen();
    void warriorSpriteChosen();
    void crabSpriteChosen();

signals:
    void setStamp(QImage);
    void stampSelected(bool);
};

#endif // STAMPSELECTION_H

