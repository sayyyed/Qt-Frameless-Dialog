#ifndef FRAMELESSDIALOG_H
#define FRAMELESSDIALOG_H

#include <QDialog>
#include <QGridLayout>

class FramelessDialog : public QDialog
{
public:
    FramelessDialog(QWidget *parent = Q_NULLPTR);

    void addWidget(QWidget *widget, int row, int column,
                   int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::Alignment());
    void setIsMovable(bool isMovable);

private:
    QGridLayout *m_frameGridLayout = Q_NULLPTR;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool m_isMouseDown = false;
    QPoint m_originalPosition;
    QPoint m_mouseDownPoint;

    bool m_isMovable = true;
};

#endif // FRAMELESSDIALOG_H
