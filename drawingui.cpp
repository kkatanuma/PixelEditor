/**
 * @brief By Bryce Radle
 * Defines the behavior of mouse events for the
 * label that holds the image being modified
 * Reviewed By:Keita Katanuma
 */

#include "drawingui.h"
#include <QMouseEvent>
#include <QEvent>

/**
 * @brief DrawingUi::DrawingUi
 * Default constructor for the drawing ui.
 * @param parent
 */
DrawingUi::DrawingUi(QWidget *parent)
    : QLabel{parent}
{

}

/**
 * @brief DrawingUi::mousePressEvent
 * If the user has not selected a tool they cannot draw
 * When they do have a tool selected get the point they clicked
 * for the model to change pixels and tell the label the user is
 * drawing. This is so we can track movement when the mouse is held
 * @param event
 * Holds the mouse data from the computer
 */
void DrawingUi::mousePressEvent(QMouseEvent *event)
{
    if(!toolSelected){
        return;
    }

    if (event->button() == Qt::LeftButton){
        pointClicked = event->position().toPoint();
        drawing = true;
        emit clicked(pointClicked);
    }
}

/**
 * @brief DrawingUi::mouseMoveEvent
 * When the user is drawing tell the model the points they pass over
 * buttons is used here and not button to return the state
 * @param event
 * Holds data about the mouse changes
 */
void DrawingUi::mouseMoveEvent(QMouseEvent *event)
{
    if(drawing && (event->buttons() & Qt::LeftButton) && toolSelected){
        pointClicked = event->position().toPoint();
        emit clicked(pointClicked);
    }
}

/**
 * @brief DrawingUi::mouseReleaseEvent
 * When the user releases the mouse they are don drawing.
 * This works outside the label, so clicking draging and
 * releasing outside the label doesn't cause undefined behavior
 * @param event
 * Holds the mouse data from the computer
 */
void DrawingUi::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && drawing){
        drawing = false;
    }
}

/**
 * @brief DrawingUi::toolChosen
 * Allows the model to tell the drawing ui if
 * a tool is selected.
 * @param selected
 * The condition of the tools as sent by the model
 */
void DrawingUi::toolChosen(bool selected)
{
    toolSelected = selected;
}
