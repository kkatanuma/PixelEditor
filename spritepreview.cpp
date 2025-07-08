/**
 * @brief By Cole Phillips
 * This class handles the set up of the sprite preview window
 * and its display.
 * Reviewed By:Maxwell Gesteland
 */
#include "spritepreview.h"
#include "ui_spritepreview.h"
#include <QTimer>

/**
 * @brief spritePreview::spritePreview
 * Sets up the ui and initializes instance variables
 * needed by the code.
 * @param parent
 */
spritePreview::spritePreview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spritePreview)
{
    ui->setupUi(this);
    previewFrame = 0;
    frameDelay = 0;
    previewActive = false;
}

/**
 * @brief spritePreview::~spritePreview
 * Destructor for the sprite preview ui.
 */
spritePreview::~spritePreview()
{
    delete ui;
}

/**
 * @brief spritePreview::setSpriteWidth
 * setup and show sprite window, initiate animation
 * @param model
 * pointer of the model
 * @param delay
 * delay to use for animation
 */
void spritePreview::setSpriteWidth(Model* model, int delay)
{
    frameDelay = delay;
    modelPtr = model;
    int width = DEFAULT_WIDTH / model->pixelWidth;

    ui->previewSprite->setGeometry( (158 - width)/2, (128 - width)/2 ,width ,width);
    this->show();
    previewActive = true;
    previewAnimation();
}

/**
 * @brief spritePreview::previewAnimation
 * manage the animation of the sprite preview
 */
void spritePreview::previewAnimation()
{
    if(previewFrame >= (int)modelPtr->frames.size()){
        previewFrame = 0;
    }

    ui->previewSprite->setPixmap(QPixmap::fromImage(modelPtr->frames[previewFrame++]));
    if(previewActive){
        QTimer::singleShot(frameDelay, this, &spritePreview::previewAnimation);
    }
}

/**
 * @brief spritePreview::closeEvent
 * stop the preview animation before closing
 * @param event
 * close event
 */
void spritePreview::closeEvent(QCloseEvent *event)
{
    previewActive = false;
    QWidget::closeEvent(event);
}
