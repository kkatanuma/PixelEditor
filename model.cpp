/**
 * @brief Written by all team members
 * Manages the centeral logic of the program
 * Connects different parts of the UI and
 * executes the desired effects
 * Reviewed by: Bryce Radle
 */

#include "model.h"
#include <QObject>
#include <QImage>
#include <QMovie>
#include<QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPainter>

/**
 * @brief Model::Model
 * Set the starting state of the program
 * Create an image for the user to draw on
 * Setting the pixel width changes the size of
 * the sprite
 * @param parent
 */
Model::Model(QObject *parent)
    : QObject{parent}
{
    QImage firstFrame(DEFAULT_WIDTH, DEFAULT_WIDTH, QImage::Format_ARGB32);
    firstFrame.fill(transparentColor);

    frames.push_back(firstFrame.copy());

    currentFrameIndex = 0;
    pixelWidth = 16;

    color.setRgb(0,0,0);
}

/**
 * @brief Model::addFrame
 * Method to add frame to the vector holding a QImage
 * image added is a filled with a trasparent background
 * @param index
 * index where Image shoudld be added
 * @param width
 * @param height
 */
void Model::addFrame(int index, int width, int height)
{
    QImage newFrame(width, height, QImage::Format_ARGB32);
    newFrame.fill(transparentColor);

    frames.insert(frames.begin() + index, newFrame.copy());
}

/**
 * @brief Model::duplicateFrame
 * copy this frame and insert the copy into the next position
 * of the sprite
 * @param index
 * index to add duplicated frame after
 * retrieved from the frame spin box so
 * the value must have 1 subtracted to get the index
 */
void Model::duplicateFrame(int index)
{
    QImage newFrame(frames[index-1].copy());
    frames.insert(frames.begin() + index, newFrame.copy());
}

/**
 * @brief Model::deleteFrame
 * delete the current frame and update ui to show frame
 * previous to this one
 * @param index
 * index of frame to delete
 * retrieved from the frame spin box so
 * the value must have 1 subtracted to get the index
 */
void Model::deleteFrame(int index)
{
    frames.erase(frames.begin() + (index - 1));
}

/**
 * @brief Model::setCurrentFrame
 * set the current frame index to the given indexs
 * @param currentFrame
 * index of the current frame
 * Value retrieved from the spin box
 * so subtract 1 to get the index
 */
void Model::setCurrentFrame(int currentFrame)
{
    currentFrameIndex = currentFrame - 1;
}

/**
 * @brief Model::setEraserActive
 * This is so the model knows when the eraser tool is active
 * This way the current color of the pencil can be changed in
 * the MainWindow without changing the color the model is using
 * @param active
 * This is the checked state of the eraser button
 */
void Model::setEraserActive(bool active)
{
    eraserActive = active;
    if(active){
        color = transparentColor;
    }
}

/**
 * @brief Model::setStampActive
 * Tells the model that the user wants to place
 * a stamp so that the model doesn't draw on a pixel
 * @param active
 */
void Model::setStampActive(bool active)
{
    stampActive = active;
}

/**
 * @brief Model::clearCurrentFrame
 * Fills the current frame with a transparent background
 */
void Model::clearCurrentFrame()
{
    frames[currentFrameIndex].fill(transparentColor);
    emit redraw();
}

/**
 * @brief Model::fillFrame
 * Fills the current frame with the pencil color
 * If the eraser is active the screen is cleared
 */
void Model::fillFrame()
{
    frames[currentFrameIndex].fill(color);
    emit redraw();
}

/**
 * @brief Model::setFrameSize
 * This recieves a signal from the sizeBox
 * 0 is 64 pixels
 * 1 is 32 pixels
 * 2 is 16 pixels
 * 3 is 8 pixels
 * default should never be reached
 * @param selectedSize
 * The changed index of the sizeBox
 */
void Model::setFrameSize(int selectedSize)
{
    switch(selectedSize){
    case 0:
        resizeFrames(8);
        break;
    case 1:
        resizeFrames(16);
        break;
    case 2:
        resizeFrames(32);
        break;
    case 3:
        resizeFrames(64);
        break;
    default:
        resizeFrames(16);
        break;
    }
}

/**
 * @brief Model::resizeFrames
 * Takes what is on the screen and changes the size to fit
 * the new pixel width. This copies the old frame and then
 * redraws all the pixels in the new position oriented to the
 * top left corner.
 * @param newPixelWidth
 * The pixel width changes the size of the image
 */
void Model::resizeFrames(int newPixelWidth)
{
    int oldFrameSize = DEFAULT_WIDTH / pixelWidth;
    int newFrameSize = DEFAULT_WIDTH / newPixelWidth;

    int smallerFrameSize = oldFrameSize;
    if(newFrameSize < oldFrameSize){
        smallerFrameSize = newFrameSize;
    }

    QImage tempImage(smallerFrameSize, smallerFrameSize, QImage::Format_ARGB32);
    QColor originalColor = color;

    //convert all frames
    for(int i = 0; i < (int)frames.size(); i++){

        //put this frames contents into temp image
        for(int x = 0; x < tempImage.width(); x++){
            for(int y = 0; y < tempImage.width(); y++){
                tempImage.setPixelColor(x, y, frames[i].pixelColor(x * pixelWidth, y * pixelWidth));
            }
        }

        frames[i].fill(transparentColor);

        //fill in pixels from temp to new size
        for(int x = 0; x < tempImage.width(); x++){
            for(int y = 0; y < tempImage.width(); y++){
                color = tempImage.pixelColor(x, y);
                fillPixel(i, x * newPixelWidth, y * newPixelWidth, newPixelWidth);
            }
        }
    }

    color = originalColor;
    pixelWidth = newPixelWidth;
    emit redraw();
}

/**
 * @brief Model::pointClicked
 * Recieves the point on the label
 * that was clicked by the user
 * If a stamp had previously been chosen
 * the stamp will be added at that point
 * otherwise the pixel at that point is changed
 * @param point
 */
void Model::pointClicked(QPoint point)
{
    //If the user clicked a stamp to place, the first click will place a stamp.
    if(stampActive){
        addStamp(stampSelected, point);
    }
    else{
        int pixelX = point.x() / pixelWidth * pixelWidth;
        int pixelY = point.y() / pixelWidth * pixelWidth;
        fillPixel(currentFrameIndex, pixelX, pixelY, pixelWidth);
        emit redraw();
    }
}

/**
 * @brief Model::setColor
 * Changes the color to the user selected color unless
 * the eraser is currently active
 * @param newColor
 */
void Model::setColor(QColor newColor)
{
    if(!eraserActive){
        color = newColor;
    }
}

/**
 * @brief Model::setStamp
 * stores the chosen stamp from the stamp menu
 * in the instance variable to be used when drawing the stamp.
 * @param stamp
 * The stamp that was selected.
 */
void Model::setStamp(QImage stamp)
{
    stampSelected = stamp;
}

/**
 * @brief Model::fillPixel
 * Becuase the pixel width is how we change the size
 * of the sprite we need to fill extra pixels on the image
 * We add extra pixels with the for loops and make sure
 * they are in the range of the label size
 * @param frameIndex
 * The frame to draw on
 * @param x
 * The x position of the pixel
 * @param y
 * The y position of the pixel
 * @param widthOfPixel
 * The current width of the pixel
 */
void Model::fillPixel(int frameIndex, int x, int y, int widthOfPixel)
{
    for(int xExtraPixels = 0; xExtraPixels < widthOfPixel; xExtraPixels++){
        for(int yExtraPixels = 0; yExtraPixels < widthOfPixel; yExtraPixels++){
            int xPosition = x + xExtraPixels;
            int yPosition = y + yExtraPixels;
            bool xPositionInRange = xPosition >= 0 && xPosition < DEFAULT_WIDTH;
            bool yPositionInRange = yPosition >= 0 && yPosition < DEFAULT_WIDTH;
            if(xPositionInRange && yPositionInRange)
                frames[frameIndex].setPixelColor(xPosition, yPosition, color);
        }
    }
}

/**
 * @brief Model::openFile
 * Method called when user choose open file from menue
 * method will handle different file types, and display error message
 * if unable to read the project
 */
void Model::openFile()
{
    if(!isSaved){
        QMessageBox msgBox;
        msgBox.setText("The project has been modified.");
        msgBox.setInformativeText("Do you want to save your changes before opening new file?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch(ret){
        case QMessageBox::Save:
              saveFile();
              break;
        case QMessageBox::Discard:
              break;
        case QMessageBox::Cancel:
              return;
        default:
              break;
        }
    }
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Project"), ".",
                       ("Project files (*.ssp);;Image files (*.png *.jpg *.gif)"));
    if(!fileName.isNull()){
        QFileInfo fileInfo(fileName);
        QString ext = fileInfo.suffix();
        if(ext == "png" || ext == "jpg"){
            loadImageFile(fileName);
        }else if (ext == "ssp"){
            loadProjectFile(fileName);
        }else if(ext == "gif"){
            loadGifFile(fileName);
        }else{
            QMessageBox msgBox;
            msgBox.setText("Selected file type is not supported.");
            msgBox.exec();
            return;
        }
    }
}

/**
 * @brief Model::loadImageFile
 * Method to load the image file to the project
 * and send signal to view to update project size
 * @param fileName
 */
void Model::loadImageFile(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QImage tempImage(DEFAULT_WIDTH, DEFAULT_WIDTH, QImage::Format_ARGB32);
    tempImage.load(&file, nullptr);
    emit updateComboBox(0);
    QImage scaledImage = tempImage.scaled(DEFAULT_WIDTH, DEFAULT_WIDTH);
    frames.at(0).swap(scaledImage);
    emit redraw();
}

/**
 * @brief Model::loadGifFile
 * Method to load the gif file to project.
 * uses QMovie to determin how many frames in the .gif file
 * and load each frame as QImage
 * @param fileName
 */
void Model::loadGifFile(const QString &fileName)
{
    QMovie gif(fileName);
    if(!gif.isValid()){
        QMessageBox msgBox;
        msgBox.setText("Invalid or unsupported GIF file.");
        msgBox.exec();
        return;
    }
    emit updateComboBox(0);
    for(int frameIndex = 0; frameIndex < gif.frameCount(); frameIndex++){
        gif.jumpToFrame(frameIndex);
        if(frameIndex == 0){
            frames.at(0) = gif.currentImage().scaled(DEFAULT_WIDTH, DEFAULT_WIDTH);
        }
        else{
            addFrame(frameIndex, DEFAULT_WIDTH, DEFAULT_WIDTH);
            frames.at(frameIndex) = gif.currentImage().scaled(DEFAULT_WIDTH, DEFAULT_WIDTH);
        }
        gif.jumpToNextFrame();
    }
    emit updateSpinBox(frames.size());
    emit redraw();
}

/**
 * @brief Model::loadProjectFile
 * Method to read Jsonstring from the file selected by user.
 * if able to parse the Json, it will load the project and send signal to
 * view to update
 * @param fileName
 * QString contains the filepath
 */
void Model::loadProjectFile(const QString &fileName)
{
    QFile file(fileName);
    QColor currentColor = color;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QString jsonString = in.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8(), &error);
        if(error.error != QJsonParseError::NoError){
            QMessageBox msgBox;
            msgBox.setText("Error Occued while parsing Json" + error.errorString());
            msgBox.exec();
            return;
        }
        else{
            QJsonObject jsonData = jsonDoc.object();
            loadFramesFromJson(jsonData);
            color = currentColor;
            emit redraw();
        }
    }
}

/**
 * @brief Model::loadFramesFromJson
 * method to load the image from the JsonObject
 * converts the jsondocument to object and add number of frames needed in the project
 * Compatible size are : 4x4, 8x8, 16x16, 32x32, 64x64
 * @param jsonData
 * JsonData contains the project info
 */
void Model::loadFramesFromJson(const QJsonObject &jsonData)
{
    int height = jsonData["height"].toInt();
    int width = jsonData["width"].toInt();
    int numberOfFrames = jsonData["numberOfFrames"].toInt();
    QJsonObject framesOjbect = jsonData["frames"].toObject();

    if(height != width){
        QMessageBox msgBox;
        msgBox.setText("Image must be square");
        msgBox.exec();
        return;
    }
    int comboBoxIndex = 0;
    switch(width){
    case 4:
        comboBoxIndex =3;
        break;
    case 8:
        comboBoxIndex = 3;
        break;
    case 16:
        comboBoxIndex = 2;
        break;
    case 32:
        comboBoxIndex = 1;
        break;
    case 64:
        comboBoxIndex = 0;
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText("Unable to load image: Compatible sizes are: "
                       "4x4, 8x8, 16x16, 32x32, 64x64");
        msgBox.exec();
        return;
    }
    isSaved = true;
    newFile();
    pixelWidth = DEFAULT_WIDTH / width;

    for(int frameIndex = 0; frameIndex < numberOfFrames; frameIndex++){
        if(frameIndex == static_cast<int>(frames.size())){
            addFrame(frameIndex, DEFAULT_WIDTH, DEFAULT_WIDTH);
        }
        createImageFromJson(framesOjbect, frameIndex);
    }
    emit updateComboBox(comboBoxIndex);
    emit updateSpinBox(frames.size());
}

/**
 * @brief Model::createImageFromJson
 * This method will convert the JsonObject that contains a frame information and
 * draw pixel based on the informaiton passed in by using fillPixel method
 * @param framesObject
 * JsonObject that contains frame infomraiton
 * @param frameIndex
 * Index of frame trying to convert to pixels
 */
void Model::createImageFromJson(const QJsonObject &framesObject, int frameIndex)
{
    QString key = QString("frame%1").arg(frameIndex);
    QJsonArray frameArray = framesObject[key].toArray();
    for(int rowIndex = 0; rowIndex < frameArray.size(); rowIndex++){
        QJsonArray row = frameArray[rowIndex].toArray();
        for(int colIndex = 0; colIndex < row.size(); colIndex++){
            QJsonArray rgba = row[colIndex].toArray();
            QColor value(rgba[0].toInt(), rgba[1].toInt(), rgba[2].toInt(), rgba[3].toInt());
            color = value;
            fillPixel(frameIndex, colIndex * pixelWidth, rowIndex * pixelWidth, pixelWidth);
        }
    }
}

/**
 * @brief Model::saveFile
 * This method is called when save menu is selected from the menu.
 * Write the .ssp file of current project informaiton.
 */
void Model::saveFile()
{
    QJsonObject jsonData = createJsonData();
    QJsonDocument jsonDoc(jsonData);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Indented);
    saveJsonToFile(jsonString);
}

/**
 * @brief Model::createJsonData
 * Create JsonObject from frames vector contains QImage
 * @return
 * JsonObject contains height, width, number of frames, and each frame
 * infomration
 */
QJsonObject Model::createJsonData()
{
    QJsonObject jsonData;
    int height = frames.at(0).height() / pixelWidth;
    int width = frames.at(0).width() / pixelWidth;
    int numberOfFrames = frames.size();
    jsonData["height"] = height;
    jsonData["width"] = width;
    jsonData["numberOfFrames"] = numberOfFrames;
    QJsonObject framesJson;
    for(size_t index = 0; index < frames.size(); index++){
        QJsonArray frame = createFrameArray(frames.at(index));
        QString key = QString("frame%1").arg(index);
        framesJson[key] = frame;
    }
    jsonData["frames"] = framesJson;
    return jsonData;
}

/**
 * @brief Model::createFrameArray
 * Create JsonArray frame that contains 2D array of JsonArray
 * 2D array contains the RGBA color information of each pixel in rows
 * @param frameImage
 * @return
 * QJsonArray that contains a frame information
 */
QJsonArray Model::createFrameArray(QImage &frameImage)
{
    QJsonArray frame;
    for(int y = 0; y < frameImage.height() / pixelWidth; y++){
        QJsonArray row;
        for(int x = 0; x < frameImage.width() / pixelWidth; x++){
            QRgb rgb = frameImage.pixel(x * pixelWidth, y * pixelWidth);
            QJsonArray rgba;
            rgba.append(qRed(rgb));
            rgba.append(qGreen(rgb));
            rgba.append(qBlue(rgb));
            rgba.append(qAlpha(rgb));
            row.append(rgba);
        }
        frame.append(row);
    }
    return frame;
}

/**
 * @brief Model::saveJsonToFile
 * Function to write the Json string to text file.
 * Once text file is created it will display message to inform user
 * @param jsonString
 * JsonString converted from JsonDocument
 */
void Model::saveJsonToFile(const QString &jsonString)
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Project", "", "Sprite sheet (*.ssp)");
    if(!filePath.isNull()){
        QFile file(filePath);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QTextStream out(&file);
            out << jsonString;
            file.close();
            isSaved = true;
            QMessageBox msgBox;
            msgBox.setText("Project has been saved.");
            msgBox.exec();
        }
    }
}

/**
 * @brief Model::newFile
 * Reset the project to the initial state, if the file is not saved
 * it will promt user to save the file before resetting project
 */
void Model::newFile(){
    if(!isSaved){
        QMessageBox msgBox;
        msgBox.setText("Would like to save your progress?");
        msgBox.setInformativeText("unsaved data will be lost");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch(ret){
        case QMessageBox::Save:
              saveFile();
              break;
        case QMessageBox::Discard:
              break;
        case QMessageBox::Cancel:
              return;
        default:
              break;
        }
    }
    while(frames.size()!=1){
        deleteFrame(frames.size()-1);
    }
    emit updateSpinBox(1);
    clearCurrentFrame();
}

/**
 * @brief Model::addStamp
 * Draws the stamp onto the current frame and redraws the frame.
 * Also, emits a signal to inform ui that the stamp has been placed.
 * @param stamp
 * The stamp being drawn.
 * @param point
 * The location where the stamp will be drawn.
 */
void Model::addStamp(QImage stamp, QPoint point)
{
    QImage baseImg = frames.at(currentFrameIndex).copy();
    QPainter painter(&baseImg);

    painter.drawImage(point.x(), point.y(), stamp);
    painter.end();
    frames.at(currentFrameIndex).swap(baseImg);
    emit redraw();
    emit stampPlaced();
}
