#ifndef FRAMELESSDIALOG_H
#define FRAMELESSDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QSizeGrip>
#include <QToolButton>
#include <QFrame>

class FramelessDialog : public QDialog
{
public:
    FramelessDialog(QWidget *parent = Q_NULLPTR);

    void addWidget(QWidget *widget, int row, int column,
                   int rowSpan = 1, int columnSpan = 1, Qt::Alignment alignment = Qt::Alignment());
    void addLayout(QLayout *layout, int row, int column,
                   int rowSpan = 1, int columnSpan = 1, Qt::Alignment alignment = Qt::Alignment());

    void setIsMovable(bool isMovable);

    void setIsResizable(bool isResizable);

private:
    bool m_isMovable = true;
    bool m_isMouseDown = false;
    QPoint m_originalPosition;
    QPoint m_mouseDownPoint;

    QGridLayout *m_mainGridLayout = Q_NULLPTR;
    QFrame *m_mainFrame = Q_NULLPTR;
    QGridLayout *m_frameGridLayout = Q_NULLPTR;
    QFrame *m_topBarFrame = Q_NULLPTR;

    //
    bool m_isResizable = true;
    QToolButton *m_maximizeBtn = Q_NULLPTR;
    QSizeGrip *m_sizeGrip = Q_NULLPTR;
    QFrame *m_leftHorizontalSizeGrip = Q_NULLPTR;
    bool m_isLeftHorizontalSizeGripClicked = false;
    QFrame *m_rightHorizontalSizeGrip = Q_NULLPTR;
    bool m_isRightHorizontalSizeGripClicked = false;
    bool m_isHorizontalResizing = false;
    int m_originalWidth = 0;

protected:
    bool event(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // FRAMELESSDIALOG_H
