#ifndef DRAWINGUI_H
#define DRAWINGUI_H

#include <QLabel>
#include <QWidget>

class DrawingUi : public QLabel
{
    Q_OBJECT
public:
    DrawingUi(QWidget *parent = nullptr);

private:
    QPoint pointClicked;
    bool drawing = false;
    bool toolSelected = true;
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

public slots:
    void toolChosen(bool);

signals:
    void clicked(QPoint);
};

#endif // DRAWINGUI_H
