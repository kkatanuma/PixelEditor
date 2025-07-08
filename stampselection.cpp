/**
 * @brief By Maxwell Gesteland
 * This class handles the setting up all necessary
 * signals/slots needed for the stamp selection menu
 * as well as the ui itself.
 * Reviewed By: Cole Phillips
 */
#include "stampselection.h"
#include "ui_stampselection.h"

/**
 * @brief StampSelection::StampSelection
 * This constructor sets up the window and titles it.
 * It then sets up the connections for all the buttons
 * in the window.
 * @param parent
 * Provides all the widget functionality
 */
StampSelection::StampSelection(QWidget *parent) :
    QWidget{parent},
    ui(new Ui::StampSelection)
{
    ui->setupUi(this);
    this->setWindowTitle("Stamps");

    connect(ui->yellowSpritePushButton, &QPushButton::clicked,
            this, &StampSelection::yellowSpriteChosen);
    connect(ui->blueSpritePushButton, &QPushButton::clicked,
            this, &StampSelection::blueSpriteChosen);
    connect(ui->warriorSpritePushButton, &QPushButton::clicked,
            this, &StampSelection::warriorSpriteChosen);
    connect(ui->crabSpritePushButton, &QPushButton::clicked,
            this, &StampSelection::crabSpriteChosen);
}

/**
 * @brief StampSelection::~StampSelection
 * Destructor for the stamp selection window.
 */
StampSelection::~StampSelection()
{
    delete ui;
}

/**
 * @brief StampSelection::yellowSpriteChosen
 * When the yellow sprite stamp is chosen, this
 * loads the image informs the ui and model that a stamp has been selected
 */
void StampSelection::yellowSpriteChosen()
{
    QImage yellowSprite(":/Resources/stamp/Resources/stamp/yellowSprite.png");

    emit stampSelected(true);
    emit setStamp(yellowSprite);
}

/**
 * @brief StampSelection::blueSpriteChosen
 * When the blue sprite stamp is chosen, this
 * loads the image and informs the ui and model that a stamp has been selected
 */
void StampSelection::blueSpriteChosen()
{
    QImage blueSprite(":/Resources/stamp/Resources/stamp/blueSprite.png");

    emit stampSelected(true);
    emit setStamp(blueSprite);
}

/**
 * @brief StampSelection::warriorSpriteChosen
 * When the warrior sprite stamp is chosen, this
 * loads the image and informs the ui and model that a stamp has been selected
 */
void StampSelection::warriorSpriteChosen()
{
    QImage warriorSprite(":/Resources/stamp/Resources/stamp/warriorSprite.png");

    emit stampSelected(true);
    emit setStamp(warriorSprite);
}

/**
 * @brief StampSelection::crabSpriteChosen
 * When the crab sprite stamp is chosen, this
 * loads the image and informs the ui and model that a stamp has been selected
 */
void StampSelection::crabSpriteChosen()
{
    QImage crabSprite(":/Resources/stamp/Resources/stamp/crabSprite.png");

    emit stampSelected(true);
    emit setStamp(crabSprite);
}

