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
    resize(800, 600);

    m_mainGridLayout = new QGridLayout;
    m_mainGridLayout->setObjectName("mainGridLayout");
    m_mainGridLayout->setSpacing(0);
    setLayout(m_mainGridLayout);

    int mainFrameColumn = 0;

    //Horizontal Size Grips
    if(m_isResizable)
    {
        mainFrameColumn = 1;

        m_leftHorizontalSizeGrip = new QFrame(this);
        m_mainGridLayout->addWidget(m_leftHorizontalSizeGrip, 0, 0);
        m_leftHorizontalSizeGrip->setMinimumWidth(3);
        m_leftHorizontalSizeGrip->setMaximumWidth(3);
        m_leftHorizontalSizeGrip->setCursor(Qt::SizeHorCursor);

        m_rightHorizontalSizeGrip = new QFrame(this);
        m_mainGridLayout->addWidget(m_rightHorizontalSizeGrip, 0, 2);
        m_rightHorizontalSizeGrip->setMinimumWidth(3);
        m_rightHorizontalSizeGrip->setMaximumWidth(3);
        m_rightHorizontalSizeGrip->setCursor(Qt::SizeHorCursor);
    }

    m_mainFrame = new QFrame;
    m_mainFrame->setObjectName("mainFrame");
    m_mainGridLayout->addWidget(m_mainFrame, 0, mainFrameColumn);

    m_mainGridLayout->setMargin(9);
    m_mainGridLayout->setSpacing(0);
    m_mainFrame->setStyleSheet(QString(R"(
                                       #mainFrame{
                                       background-color: white;
                                       border: 1px solid gray;
                                       border-radius: 8px;
                                       }
                                       )"));

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    m_mainFrame->setLayout(vLayout);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setOffset(0, 0.5);
    shadow->setColor(Qt::black);
    shadow->setBlurRadius(9);
    m_mainFrame->setGraphicsEffect(shadow);

    //Top bar
    m_topBarFrame = new QFrame;
    m_topBarFrame->setObjectName("topBarFrame");
    m_topBarFrame->setMaximumHeight(40);
    m_topBarFrame->setLayoutDirection(Qt::LeftToRight);

    if(m_isMovable && this->windowState() != Qt::WindowMaximized)
        m_topBarFrame->setCursor(Qt::SizeAllCursor);

    QHBoxLayout *topBarLayout = new QHBoxLayout;
    topBarLayout->setMargin(2);
    m_topBarFrame->setLayout(topBarLayout);

    m_topBarFrame->setStyleSheet(R"(
                                 #topBarFrame{
                                 background-color: rgb(240, 240, 240);
                                 border-top-left-radius: 8px;
                                 border-top-right-radius: 8px;
                                 }
                                 )");

    QToolButton *closeBtn = new QToolButton;
    closeBtn->setMinimumSize(24, 24);
    closeBtn->setIcon(QIcon("://FramelessDialogResources/close-window-icon.png"));
    closeBtn->setIconSize(QSize(24, 24));
    closeBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    closeBtn->setAutoRaise(true);
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setToolTip(tr("Close"));

    connect(closeBtn, &QToolButton::clicked, this, &QDialog::close);

    m_maximizeBtn = new QToolButton;
    m_maximizeBtn->setMinimumSize(24, 24);
    m_maximizeBtn->setIcon(QIcon("://FramelessDialogResources/window-icon.png"));
    m_maximizeBtn->setIconSize(QSize(24, 24));
    m_maximizeBtn->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_maximizeBtn->setAutoRaise(true);
    m_maximizeBtn->setCursor(Qt::PointingHandCursor);
    m_maximizeBtn->setToolTip(tr("Maximize"));
    m_maximizeBtn->setVisible(m_isResizable);

    QLabel *titleLabel = new QLabel;
    titleLabel->setAlignment(Qt::AlignCenter);

    connect(this, &FramelessDialog::windowTitleChanged, titleLabel, &QLabel::setText);

    QLabel *topBarIconLabel = new QLabel;
    topBarIconLabel->setMinimumSize(24, 24);
    topBarIconLabel->setMaximumSize(24, 24);
    topBarIconLabel->setScaledContents(true);
    connect(this, &FramelessDialog::windowIconChanged, [=]()
    {
        topBarIconLabel->setPixmap(this->windowIcon().pixmap(24, 24));
    });

    topBarLayout->addWidget(topBarIconLabel, 0, Qt::AlignTop | Qt::AlignLeft);
    topBarLayout->addWidget(titleLabel, 10, Qt::AlignTop | Qt::AlignCenter);
    topBarLayout->addWidget(m_maximizeBtn, 0, Qt::AlignTop);
    topBarLayout->addWidget(closeBtn, 0, Qt::AlignTop);
    vLayout->addWidget(m_topBarFrame);

//    QFrame *horizontalLine = new QFrame;
//    horizontalLine->setFrameShape(QFrame::HLine);
//    horizontalLine->setFrameShadow(QFrame::Sunken);
//    vLayout->addWidget(horizontalLine);

    m_frameGridLayout = new QGridLayout;
    vLayout->addLayout(m_frameGridLayout, 1);

    m_sizeGrip = new QSizeGrip(this);
    m_sizeGrip->setToolTip(tr("Resize"));
    m_sizeGrip->setStyleSheet(R"(
                            image: url(:/FramelessDialogResources/Resize-black-icon.png);
                            width: 16px;
                            height: 16px;
                            )");
    vLayout->addWidget(m_sizeGrip, 0, Qt::AlignBottom | Qt::AlignLeft);
    m_sizeGrip->setVisible(m_isResizable);

    connect(m_maximizeBtn, &QToolButton::clicked, [=]()
    {
        if(this->isMaximized())
            this->showNormal();
        else
            this->showMaximized();
    });
}

void FramelessDialog::addWidget(QWidget *widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_frameGridLayout->addWidget(widget, row, column, rowSpan, columnSpan, alignment);
}

void FramelessDialog::addLayout(QLayout *layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_frameGridLayout->addLayout(layout, row, column, rowSpan, columnSpan, alignment);
}

void FramelessDialog::setIsMovable(bool isMovable)
{
    m_isMovable = isMovable;
    m_topBarFrame->setCursor(m_isMovable ? Qt::SizeAllCursor : Qt::ArrowCursor);
}

void FramelessDialog::setIsResizable(bool isResizable)
{
    m_isResizable = isResizable;

    m_sizeGrip->setVisible(m_isResizable);
    m_maximizeBtn->setVisible(m_isResizable);
    m_leftHorizontalSizeGrip->setVisible(m_isResizable);
    m_rightHorizontalSizeGrip->setVisible(m_isResizable);
}

void FramelessDialog::mousePressEvent(QMouseEvent *event)
{
    if(m_isMovable)
    {
        if(this->isMaximized())
            return;

        if(event->button() == Qt::LeftButton)
        {
            m_originalPosition = this->mapToGlobal(QPoint(0, 0));
            QRect topBarRect(m_originalPosition.x(), m_originalPosition.y(), this->width(), m_topBarFrame->height());
            if(topBarRect.contains(event->globalPos()))
            {
                setCursor(Qt::SizeAllCursor);
                m_mouseDownPoint = event->globalPos();
                m_isMouseDown = true;
                setWindowOpacity(0.85);
                return;
            }
        }
    }

    if(m_isResizable)
    {
        if(event->button() == Qt::LeftButton)
        {
            m_originalWidth = this->width();

            QPoint p = m_leftHorizontalSizeGrip->mapToGlobal(QPoint(0, 0));
            QRect leftHorizontalSizeGripRect(p.x(), p.y(), m_leftHorizontalSizeGrip->width(), this->height());
            if(leftHorizontalSizeGripRect.contains(event->globalPos()))
            {
                m_originalPosition = this->mapToGlobal(QPoint(0, 0));
                m_mouseDownPoint = event->globalPos();
                m_isHorizontalResizing = true;
                m_isLeftHorizontalSizeGripClicked = true;
                setWindowOpacity(0.85);
                return;
            }

            p = m_rightHorizontalSizeGrip->mapToGlobal(QPoint(0, 0));
            QRect rightHorizontalSizeGripRect(p.x(), p.y(), m_rightHorizontalSizeGrip->width(), this->height());
            if(rightHorizontalSizeGripRect.contains(event->globalPos()))
            {
                m_originalPosition = this->mapToGlobal(QPoint(0, 0));
                m_mouseDownPoint = event->globalPos();
                m_isHorizontalResizing = true;
                m_isRightHorizontalSizeGripClicked = true;
                setWindowOpacity(0.85);
            }
        }
    }
}

void FramelessDialog::mouseReleaseEvent(QMouseEvent *)
{
    if(m_isMovable)
    {
        m_isMouseDown = false;
        setCursor(Qt::ArrowCursor);
    }

    if(m_isResizable)
    {
        if(m_isHorizontalResizing == true)
        {
            m_isHorizontalResizing = false;
            m_isLeftHorizontalSizeGripClicked = false;
            m_isRightHorizontalSizeGripClicked = false;
        }
    }

    if(this->windowOpacity() < 1)
        setWindowOpacity(1);
}

void FramelessDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isMovable && m_isMouseDown)
    {
        move(m_originalPosition + (event->globalPos() - m_mouseDownPoint));
        return;
    }

    if(m_isResizable)
    {
        if(m_isHorizontalResizing)
        {
            if(m_isLeftHorizontalSizeGripClicked)
            {
                resize(m_originalWidth+(event->globalPos().x() - m_mouseDownPoint.x()), this->height());
            }
            else if(m_isRightHorizontalSizeGripClicked)
            {
                int distance = event->globalPos().x() - m_mouseDownPoint.x();
                resize(m_originalWidth+(-distance), this->height());
                if(this->width() <= this->minimumWidth())
                    return;

                move(m_originalPosition.x() + distance, m_originalPosition.y());
            }
        }
    }
}

void FramelessDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!m_isResizable)
        return;

    if(event->button() == Qt::LeftButton)
    {
        QPoint originalPosition = this->mapToGlobal(QPoint(0, 0));
        QRect topBarRect(originalPosition.x(), originalPosition.y(), this->width(), 50);
        if(topBarRect.contains(event->globalPos()))
        {
            m_maximizeBtn->click();
        }
    }
}

bool FramelessDialog::event(QEvent *event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
//        QWindowStateChangeEvent *windowStateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);
//        if(windowStateChangeEvent)
//        {
            m_maximizeBtn->setToolTip(tr(isMaximized() ? "Restore" : "Maximize"));
            m_maximizeBtn->setIcon(QIcon(isMaximized() ? "://FramelessDialogResources/restore-icon.png" : "://FramelessDialogResources/window-icon.png"));
            m_sizeGrip->setVisible(!isMaximized());

            if(m_isMovable)
                m_topBarFrame->setCursor(isMaximized() ? Qt::ArrowCursor : Qt::SizeAllCursor);

            m_mainGridLayout->setMargin(isMaximized() ? 0 : 3);
            m_mainFrame->setStyleSheet(QString(R"(
                                               #mainFrame{
                                               background-color: white;
                                               border: 1px solid gray;
                                               border-radius: %0px;
                                               }
                                               )").arg(isMaximized() ? 0 : 8));

            if(m_isResizable)
            {
                m_leftHorizontalSizeGrip->setVisible(!isMaximized());
                m_rightHorizontalSizeGrip->setVisible(!isMaximized());
            }

            return true;
//        }
    }

    return QDialog::event(event);
}
