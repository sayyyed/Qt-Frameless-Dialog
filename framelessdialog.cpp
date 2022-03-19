#include "framelessdialog.h"
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QToolButton>
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QSizeGrip>

FramelessDialog::FramelessDialog(QWidget *parent) : QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(400, 400);

    setMouseTracking(true);

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->setSpacing(0);
    setLayout(mainGridLayout);

    QFrame *mainFrame = new QFrame;
    mainGridLayout->addWidget(mainFrame);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    mainFrame->setLayout(vLayout);

    mainFrame->setStyleSheet(R"(
                             .QFrame{
                             background-color: white;
                             border: 2px solid transparent;
                             border-radius: 10px;
                             }
                             )");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setOffset(3);
    shadow->setColor(QColor(63, 63, 63, 100));
    shadow->setBlurRadius(2);
    mainFrame->setGraphicsEffect(shadow);

    QToolButton *closeBtn = new QToolButton;
    closeBtn->setMinimumSize(24, 24);
    closeBtn->setIcon(QIcon("://FramelessDialogResources/close-window-icon.png"));
    closeBtn->setIconSize(QSize(24, 24));
    closeBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    closeBtn->setAutoRaise(true);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setToolTip(tr("Close"));

    connect(closeBtn, &QToolButton::clicked, this, &QDialog::close);

    QToolButton *maximizeBtn = new QToolButton;
    maximizeBtn->setMinimumSize(24, 24);
    maximizeBtn->setIcon(QIcon("://FramelessDialogResources/restore-icon.png"));
    maximizeBtn->setIconSize(QSize(24, 24));
    maximizeBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    maximizeBtn->setAutoRaise(true);
    maximizeBtn->setCursor(Qt::PointingHandCursor);
    maximizeBtn->setToolTip(tr("Maximize"));

    QLabel *titleLabel = new QLabel;
    connect(this, &FramelessDialog::windowTitleChanged, titleLabel, &QLabel::setText);

    titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->addWidget(closeBtn, 0, Qt::AlignTop);
    headerLayout->addWidget(maximizeBtn, 0, Qt::AlignTop);
    headerLayout->addWidget(titleLabel, 1, Qt::AlignTop | Qt::AlignCenter);
    vLayout->addLayout(headerLayout, 1);

    m_frameGridLayout = new QGridLayout;
    vLayout->addLayout(m_frameGridLayout, 8);

    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setStyleSheet(R"(
                            image: url(:/FramelessDialogResources/resize-icon.png);
                            width: 20px;
                            height: 20px;
                            )");
    vLayout->addWidget(sizeGrip, 1, Qt::AlignBottom | Qt::AlignLeft);

    connect(maximizeBtn, &QToolButton::clicked, [=]()
    {
        int margin = 9;
        int borderRadius = 10;

        if(this->windowState() == Qt::WindowMaximized )
        {
            this->showNormal();
            maximizeBtn->setToolTip(tr("Restore"));
            maximizeBtn->setIcon(QIcon("://FramelessDialogResources/restore-icon.png"));
            sizeGrip->show();
            m_isMovable = true;
        }
        else
        {
            this->showMaximized();
            maximizeBtn->setToolTip(tr("Maximize"));
            maximizeBtn->setIcon(QIcon("://FramelessDialogResources/minimize-icon.png"));
            sizeGrip->hide();
            m_isMovable = false;

            margin = 0;
            borderRadius = 0;
        }

        mainGridLayout->setMargin(margin);
        mainFrame->setStyleSheet(QString(R"(
                                 .QFrame{
                                 background-color: white;
                                 border: 2px solid transparent;
                                 border-radius: %0px;
                                 }
                                 )").arg(borderRadius));
    });
}

void FramelessDialog::addWidget(QWidget *widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_frameGridLayout->addWidget(widget, row, column, rowSpan, columnSpan, alignment);
}

void FramelessDialog::mousePressEvent(QMouseEvent *event)
{
    if(!m_isMovable)
        return;

    if(event->button() == Qt::LeftButton)
    {
        m_originalPosition = this->mapToGlobal(QPoint(0, 0));
//        QRect topBarRect(m_originalPosition.x(), m_originalPosition.y(), this->width(), 50);
//        if(topBarRect.contains(event->globalPos()))
//        {
            setCursor(Qt::SizeAllCursor);
            m_mouseDownPoint = event->globalPos();
            m_isMouseDown = true;
//        }
    }
}

void FramelessDialog::mouseReleaseEvent(QMouseEvent *)
{
    if(!m_isMovable)
        return;

    m_isMouseDown = false;
    setCursor(Qt::ArrowCursor);
}

void FramelessDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isMovable && m_isMouseDown)
        move(m_originalPosition + (event->globalPos() - m_mouseDownPoint));
}

void FramelessDialog::setIsMovable(bool isMovable)
{
    m_isMovable = isMovable;
}
