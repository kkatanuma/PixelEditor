/**
 * @brief By Cole Phillips
 * This class handles creating and interacting with the color selection
 * widget that allows for the user to customize which color they
 * would like to draw with.
 * Reviewed By:Maxwell Gesteland
 */

#include "colorselection.h"
#include "ui_colorselection.h"

/**
 * @brief colorSelection::colorSelection
 * Constructor for the colorSelection widget.
 * Connects all signals and slots for items in the
 * menu as well as any interactions with the other
 * classes.
 * @param parent
 */
colorSelection::colorSelection(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::colorSelection)
{
    //Set up window and base color to black
    ui->setupUi(this);
    this->setWindowTitle("Color Selection");
    newColor.setRgb(0,0,0,255);
    setNewColor();

    //Button Setup
    connect(ui->closeButton, &QPushButton::clicked,
            this, &colorSelection::sendColor);
    connect(ui->applyButton, &QPushButton::clicked,
            this, &colorSelection::applyColor);

    //Slider Setup
    connect(ui->rSlider, &QSlider::valueChanged,
            this, &colorSelection::updateNewR);
    connect(ui->gSlider, &QSlider::valueChanged,
            this, &colorSelection::updateNewG);
    connect(ui->bSlider, &QSlider::valueChanged,
            this, &colorSelection::updateNewB);
    connect(ui->aSlider, &QSlider::valueChanged,
            this, &colorSelection::updateNewA);

    //Combo Box Setup
    connect(ui->colorSelect, &QComboBox::activated,
            this, &colorSelection::updateNewColor);
}

/**
 * @brief colorSelection::~colorSelection
 * Destructor for color selection ui.
 */
colorSelection::~colorSelection()
{
    delete ui;
}

/**
 * @brief colorSelection::setCurrentColor
 * sets currentColor to given color and updates UI
 * @param color
 * color to set currentColor to
 */
void colorSelection::setCurrentColor(QColor color)
{
    currentColor = color;
    ui->CurrentColorPreview->setStyleSheet("background-color:" + getColorString(currentColor));
}

/**
 * @brief colorSelection::sendColor
 * sends currentColor to MainWindow
 */
void colorSelection::sendColor()
{
    emit setColor(currentColor);
}

/**
 * @brief colorSelection::closeEvent
 * intercepts close event to update MainWindow color before closing this window
 * @param event
 */
void colorSelection::closeEvent(QCloseEvent *event)
{
    sendColor();
    QWidget::closeEvent(event);
}

/**
 * @brief colorSelection::getColorString
 * convert given color into a string in style sheet format
 * @param color
 * color to turn into string
 * @return QString
 * Style Sheet format of given color
 */
QString colorSelection::getColorString(QColor color)
{
    QString colorString("rgba(");
    colorString += QString::number(color.red()) +=",";
    colorString += QString::number(color.green()) +=",";
    colorString += QString::number(color.blue()) +=",";
    colorString += QString::number(color.alpha()) +=")";

    return colorString;
}

/**
 * @brief colorSelection::applyColor
 * sets newColor to currentColor
 */
void colorSelection::applyColor()
{
    setCurrentColor(newColor);
}

/**
 * @brief colorSelection::updateNewR
 * updates r component of newColor to given value
 * @param val
 * value to set red component of the new color to
 */
void colorSelection::updateNewR(int val)
{
    newColor.setRed(val);
    setNewColor();
}

/**
 * @brief colorSelection::updateNewG
 * updates g component of newColor to given value
 * @param val
 * value to set green component of the new color to
 */
void colorSelection::updateNewG(int val)
{
    newColor.setGreen(val);
    setNewColor();
}

/**
 * @brief colorSelection::updateNewB
 * updates b component of newColor to given value
 * @param val
 * value to set blue component of the new color to
 */
void colorSelection::updateNewB(int val)
{
    newColor.setBlue(val);
    setNewColor();
}

/**
 * @brief colorSelection::updateNewA
 * updates a component of newColor to given value
 * @param val
 * value to set alpha component of the new color to
 */
void colorSelection::updateNewA(int val)
{
    newColor.setAlpha(val);
    setNewColor();
}

/**
 * @brief colorSelection::updateNewColor
 * updates newColor to color selected in comboBox
 * and then updates UI to reflect the chosen color
 * @param choice
 * set new color to chosen color in combo box
 */
void colorSelection::updateNewColor(int choice)
{
    switch(choice){
    case 0: //Black
        newColor.setRgb(0,0,0);
        break;
    case 1: //White
        newColor.setRgb(255,255,255);
        break;
    case 2: //Red
        newColor.setRgb(255,0,0);
        break;
    case 3: //Orange
        newColor.setRgb(255,127,0);
        break;
    case 4: //Yellow
        newColor.setRgb(255,255,0);
        break;
    case 5: //Green
        newColor.setRgb(0,255,0);
        break;
    case 6: //Blue
        newColor.setRgb(0,0,255);
        break;
    case 7: //Indigo
        newColor.setRgb(75,0,130);
        break;
    case 8: //Violet
        newColor.setRgb(148,0,211);
        break;
    }

    setNewColor();
    ui->rSlider->setValue(newColor.red());
    ui->gSlider->setValue(newColor.green());
    ui->bSlider->setValue(newColor.blue());
    ui->aSlider->setValue(newColor.alpha());
}

/**
 * @brief colorSelection::setNewColor
 * updates UI to reflect newColor
 */
void colorSelection::setNewColor()
{
    ui->NewColorPreview->setStyleSheet("background-color:" + getColorString(newColor));
}

