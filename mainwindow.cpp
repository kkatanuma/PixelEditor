/**
 * @brief By all team members
 * View for our SpriteEditor handles UserInputs
 * and connect calls for model and view to communicate
 * Reviewed By:Keita Katanuma
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QActionGroup>

/**
 * @brief MainWindow::MainWindow
 * Constructs the main window ui and sets up all needed connections.
 * Sets up the start state of all features.
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Sprite Editor");

    ui->deleteFrame->setEnabled(false);
    ui->currentFrame->setPixmap(QPixmap::fromImage(model.frames[0]));
    ui->currentFrame->setScaledContents(true);
    //Initial Size
    ui->sizeBox->setCurrentIndex(1);

    createFileMenu();

    //Manage Frames
    connect(ui->addFrame, &QPushButton::clicked,
            this, &MainWindow::addFrame);
    connect(ui->duplicateFrame, &QPushButton::clicked,
            this, &MainWindow::duplicateFrame);
    connect(ui->deleteFrame, &QPushButton::clicked,
            this, &MainWindow::deleteFrame);

    //Clear/Fill frames
    connect(ui->clearFrameButton, &QPushButton::clicked,
            &model, &Model::clearCurrentFrame);
    connect(ui->fillFrameButton, &QPushButton::clicked,
            &model, &Model::fillFrame);

    //General Setup
    connect(ui->sizeBox, &QComboBox::currentIndexChanged,
            &model, &Model::setFrameSize);
    connect(ui->frameSpinBox, &QSpinBox::valueChanged,
            this, &MainWindow::updateFrame);
    connect(ui->frameSpinBox, &QSpinBox::valueChanged,
            &model, &Model::setCurrentFrame);
    connect(ui->currentFrame, &DrawingUi::clicked,
            &model, &Model::pointClicked);
    connect(&model, &Model::redraw,
           this, &MainWindow::updateView);
    connect(&model, &Model::updateComboBox,
            ui->sizeBox, &QComboBox::setCurrentIndex);
    connect(&model, &Model::updateSpinBox,
            this, &MainWindow::updateSpinBox);

    //only allows the user to draw when the pen is selected
    connect(this, &MainWindow::toolActive,
            ui->currentFrame, &DrawingUi::toolChosen);
    connect(ui->pencil, &QPushButton::toggled,
            this, &MainWindow::pencilToggled);
    connect(ui->eraser, &QPushButton::toggled,
            this, &MainWindow::eraserToggled);
    connect(this, &MainWindow::toolColor,
            &model, &Model::setColor);
    connect(this, &MainWindow::eraserChecked,
            &model, &Model::setEraserActive);

    //Set up the connections for the stamps menu button
    //As well as the signals and slots that handle stamp
    //operations
    connect(ui->stamps, &QPushButton::clicked,
            this, &MainWindow::showStampSelection);
    connect(&stamps, &StampSelection::setStamp,
            &model, &Model::setStamp);
    connect(&stamps, &StampSelection::stampSelected,
            this, &MainWindow::stampToggled);
    connect(this, &MainWindow::stampChecked,
            &model, &Model::setStampActive);
    connect(&model, &Model::stampPlaced,
            this, &MainWindow::stampPlaced);

    //Color Selection Setup
    currentColor.setRgb(0,0,0,255);
    ui->currentColorPreview->setStyleSheet("background-color:" + getColorString());

    connect(ui->changeColor, &QPushButton::clicked,
            this, &MainWindow::showColorSelection);
    connect(this, &MainWindow::sendCurrentColor,
            &colorSelection, &colorSelection::setCurrentColor);
    connect(&colorSelection, &colorSelection::setColor,
            this, &MainWindow::setCurrentColor);
    connect(&colorSelection, &colorSelection::setColor,
            &model, &Model::setColor);

    //Sprite Preview
    previewFrame = 0;
    previewAnimation();
    connect(ui->previewSprite, &QPushButton::clicked,
            this, &MainWindow::showSpritePreview);
    connect(this, &MainWindow::openSpritePreview,
            &spritePreview, &spritePreview::setSpriteWidth);
}

/**
 * @brief MainWindow::~MainWindow
 * Destructor or the main window ui.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::createMenus
 * Creates file menu for more
 * familiar button locations
 */
void MainWindow::createFileMenu()
{
    newFile = new QAction(tr("New File"), this);
    newFile->setShortcut(QKeySequence::New);
    open = new QAction(tr("Open"), this);
    open->setShortcut(QKeySequence::Open);
    save = new QAction(tr("Save"), this);
    save->setShortcut(QKeySequence::Save);

    connect(newFile, &QAction::triggered,
            &model, &Model::newFile);
    connect(open, &QAction::triggered,
            &model, &Model::openFile);
    connect(save, &QAction::triggered,
            &model, &Model::saveFile);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(newFile);
    fileMenu->addAction(open);
    fileMenu->addAction(save);

    menuBar()->addMenu(fileMenu);
}

/**
 * @brief MainWindow::showColorSelection
 * Shows the ColorSelection Window
 */
void MainWindow::showColorSelection(){
    colorSelection.show();
    emit sendCurrentColor(currentColor);
}

/**
 * @brief MainWindow::showStampSelection
 * Shows StampSelection window
 */
void MainWindow::showStampSelection()
{
    stamps.show();
    ui->pencil->setChecked(false);
    ui->eraser->setChecked(false);
}

/**
 * @brief MainWindow::addFrame
 * adds a new blank frame next in the sprite's order
 */
void MainWindow::addFrame()
{
    model.addFrame(ui->frameSpinBox->value(), DEFAULT_WIDTH, DEFAULT_WIDTH);
    ui->frameSpinBox->setMaximum(model.frames.size());
    ui->frameSpinBox->setValue(ui->frameSpinBox->value() + 1);

    updateView();

    if (model.frames.size() > 1){
        ui->deleteFrame->setEnabled(true);
    }
}

/**
 * @brief MainWindow::duplicateFrame
 * inserts a copy of the current frame next in the sprite's order
 */
void MainWindow::duplicateFrame()
{
    model.duplicateFrame(ui->frameSpinBox->value());
    ui->frameSpinBox->setMaximum(model.frames.size());
    ui->frameSpinBox->setValue(ui->frameSpinBox->value() + 1);

    updateView();

    if (model.frames.size() > 1){
        ui->deleteFrame->setEnabled(true);
    }
}

/**
 * @brief MainWindow::deleteFrame
 * deletes the current frame from the model, and updates UI to
 * show the frame previous to this
 */
void MainWindow::deleteFrame()
{
    model.deleteFrame(ui->frameSpinBox->value());
    ui->frameSpinBox->setValue(ui->frameSpinBox->value() - 1);
    ui->frameSpinBox->setMaximum(model.frames.size());

    updateView();

    if(model.frames.size() == 1){
        ui->deleteFrame->setEnabled(false);
    }
}

/**
 * @brief MainWindow::updateFrame
 * Update the displayed image to QImage of frameNum index
 * in frames vector
 * @param frameNum
 * index of QImage in frames vector
 */
void MainWindow::updateFrame(int frameNum)
{
    ui->currentFrame->setPixmap(QPixmap::fromImage(model.frames[frameNum - 1]));
}

/**
 * @brief MainWindow::pencilToggled
 * When checked tells drawing ui a tool is active
 * Tell the model to use the current color
 * Uncheck the other tools
 * Tell the model the eraser is off
 * When unchecked look for other active tools
 * If all tools are off then tell drawing ui
 * @param selected
 * The checked state of the button
 */
void MainWindow::pencilToggled(bool selected)
{
    if(selected){
        emit toolActive(true);
        emit eraserChecked(false);
        emit stampChecked(false);
        emit toolColor(currentColor);
        ui->eraser->setChecked(false);
    }

    //both tools are off
    if(!(selected || ui->eraser->isChecked())){
        emit toolActive(false);
    }
}

/**
 * @brief MainWindow::eraserToggled
 * When checked tells drawing ui a tool is active
 * Uncheck the other tools
 * Tell the model the eraser is on
 * When unchecked look for other active tools
 * If all tools are off then tell drawing ui
 * @param selected
 * The checked state of the button
 */
void MainWindow::eraserToggled(bool selected)
{
    if(selected){
        emit eraserChecked(true);
        emit stampChecked(false);
        emit toolActive(true);
        ui->pencil->setChecked(false);

    }
    if(!(selected || ui->pencil->isChecked())){
        emit toolActive(false);
    }
}

/**
 * @brief MainWindow::stampToggled
 * Called when the stamp is chosen by the user to update the ui
 * and inform the model that a stamp is going to be placed.
 * @param selected
 * Bool that represents the action of selecting a stamp.
 */
void MainWindow::stampToggled(bool selected)
{
    if(selected){
        emit toolActive(true);
        emit stampChecked(true);

        ui->pencil->setChecked(false);
        ui->eraser->setChecked(false);
    }
}

/**
 * @brief MainWindow::stampPlaced
 * Slot that updates the ui when the user has placed the stamp.
 */
void MainWindow::stampPlaced()
{
    emit toolActive(false);
    emit stampChecked(false);
}

/**
 * @brief MainWindow::setCurrentColor
 * sets the currentColor to given color and updates UI
 * @param color
 * color to set currentColor to
 */
void MainWindow::setCurrentColor(QColor color)
{
    currentColor = color;
    ui->currentColorPreview->setStyleSheet("background-color:" + getColorString());
}

/**
 * @brief MainWindow::getColorString
 * converts the currentColor into a string for use in stylesheets
 * @return QString Color in stylesheet format
 */
QString MainWindow::getColorString()
{
    QString colorString("rgba(");
    colorString += QString::number(currentColor.red()) +=",";
    colorString += QString::number(currentColor.green()) +=",";
    colorString += QString::number(currentColor.blue()) +=",";
    colorString += QString::number(currentColor.alpha()) +=")";
    return colorString;
}

/**
 * @brief MainWindow::updateView
 * Shows the selected frame in the UI
 */
void MainWindow::updateView()
{
    ui->currentFrame->setPixmap(QPixmap::fromImage(model.frames[ui->frameSpinBox->value() - 1]));
    model.isSaved = false;
}

/**
 * @brief MainWindow::updateSpinBox
 * slots when user open project of .gif file
 * that contains multiple frames to update framesCount Spinbox
 * @param max
 * Max number of frame count
 */
void MainWindow::updateSpinBox(int max)
{
    ui->frameSpinBox->setValue(1);
    ui->frameSpinBox->setMaximum(max);

    if (model.frames.size() <= 1){
        ui->deleteFrame->setEnabled(false);
    }else{
        ui->deleteFrame->setEnabled(true);
    }
}

/**
 * @brief MainWindow::previewAnimation
 * Shows the next frame in order in the preview UI
 */
void MainWindow::previewAnimation()
{
    if(previewFrame >= (int)model.frames.size()){
        previewFrame = 0;
    }

    ui->spritePreview->setPixmap(QPixmap::fromImage(model.frames[previewFrame++]));
    QTimer::singleShot(1000/ui->fpsSlider->value(),this, &MainWindow::previewAnimation);
}

void MainWindow::showSpritePreview(){
    emit openSpritePreview(&model, 1000/ui->fpsSlider->value() );
}
