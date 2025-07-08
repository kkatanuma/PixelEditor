#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "colorselection.h"
#include "spritepreview.h"
#include "model.h"
#include "stampselection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Model model;
    StampSelection stamps;

signals:
    void sendCurrentColor(QColor);
    void toolActive(bool);
    void toolColor(QColor);
    void eraserChecked(bool);
    void stampChecked(bool);
    void openSpritePreview(Model*, int);

public slots:
    void setCurrentColor(QColor);
    void addFrame();
    void duplicateFrame();
    void deleteFrame();
    void updateFrame(int);
    void pencilToggled(bool);
    void eraserToggled(bool);
    void stampToggled(bool);
    void stampPlaced();

private:
    Ui::MainWindow *ui;
    colorSelection colorSelection;
    spritePreview spritePreview;
    QColor currentColor;
    int previewFrame;

    QString getColorString();
    void showColorSelection();
    void showSpritePreview();
    void previewAnimation();
    void setActiveTool(bool, bool);

    void showStampSelection();

    void updateView();
    void updateSpinBox(int);

    void createFileMenu();
    QMenu *fileMenu;
    QAction *newFile;
    QAction *save;
    QAction *open;
};
#endif // MAINWINDOW_H
