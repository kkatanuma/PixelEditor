#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <vector>
#include <QImage>
#include<QJsonObject>
#include <QJsonArray>

const int DEFAULT_WIDTH = 512;

using std::vector;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    vector<QImage> frames;
    QColor color;
    int currentFrameIndex;
    int pixelWidth;
    bool isSaved = true;

    void newFile();
    void openFile();
    void saveFile();
    void deleteFrame(int);
    void duplicateFrame(int);
    void addFrame(int, int, int);

public slots:
    void fillFrame();
    void clearCurrentFrame();
    void setFrameSize(int);
    void setCurrentFrame(int);
    void setStampActive(bool);
    void setEraserActive(bool);
    void pointClicked(QPoint);
    void setColor(QColor);
    void setStamp(QImage);

signals:
    void redraw();
    void stampPlaced();
    void updateComboBox(int);
    void updateSpinBox(int);

private:
    bool eraserActive = false;
    bool stampActive = false;
    QImage stampSelected;
    const QColor transparentColor = QColor(255, 255, 255, 0);

    void fillPixel(int, int, int, int);
    void addStamp(QImage, QPoint);
    void loadImageFile(const QString&);
    void loadGifFile(const QString&);
    void loadProjectFile(const QString&);
    void loadFramesFromJson(const QJsonObject&);
    void createImageFromJson(const QJsonObject&,int);
    void saveJsonToFile(const QString&);
    QJsonObject createJsonData();
    QJsonArray createFrameArray(QImage&);
    void resizeFrames(int);
};

#endif // MODEL_H
