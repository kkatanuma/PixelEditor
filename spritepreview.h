#ifndef SPRITEPREVIEW_H
#define SPRITEPREVIEW_H

#include <QWidget>
#include "model.h"

namespace Ui {
    class spritePreview;
}

class spritePreview : public QWidget
{
    Q_OBJECT

public:
    explicit spritePreview(QWidget *parent = nullptr);
    ~spritePreview();

public slots:
    void setSpriteWidth(Model*, int);

private:
    Model* modelPtr;
    Ui::spritePreview *ui;
    int previewFrame, frameDelay;
    bool previewActive;
    void closeEvent(QCloseEvent *event);
    void previewAnimation();
};

#endif // SPRITEPREVIEW_H
