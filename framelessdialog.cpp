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

    QGridLayout *mainGridLayout = new QGridLayout;
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(3);
    setLayout(mainGridLayout);

    QFrame *mainFrame = new QFrame;
    mainFrame->setObjectName("mainFrame");
    mainGridLayout->addWidget(mainFrame);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    mainFrame->setLayout(vLayout);

    mainFrame->setStyleSheet(R"(
                             #mainFrame{
                             background-color: white;
                             border: 2px solid rgb(230, 230, 230);
                             border-radius: 8px;
                             }
                             )");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setOffset(3);
    shadow->setColor(QColor(63, 63, 63, 100));
    shadow->setBlurRadius(2);
    mainFrame->setGraphicsEffect(shadow);

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
                                 border-top-left-radius: 8px;
                                 border-top-right-radius: 8px;
                               }
                              #topBarFrame:hover{
                                background-color: rgb(240, 240, 240);
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
    m_maximizeBtn->setIcon(QIcon("://FramelessDialogResources/restore-icon.png"));
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
    m_sizeGrip->setStyleSheet(R"(
                            image: url(:/FramelessDialogResources/resize-icon.png);
                            width: 24px;
                            height: 24px;
                            )");
    vLayout->addWidget(m_sizeGrip, 0, Qt::AlignBottom | Qt::AlignLeft);
    m_sizeGrip->setVisible(m_isResizable);

    connect(m_maximizeBtn, &QToolButton::clicked, [=]()
    {
        int margin = 3;
        int borderRadius = 8;

        if(this->windowState() == Qt::WindowMaximized)
        {
            this->showNormal();
            m_maximizeBtn->setToolTip(tr("Maximize"));
            m_maximizeBtn->setIcon(QIcon("://FramelessDialogResources/restore-icon.png"));
            m_sizeGrip->show();

            if(m_isMovable)
                m_topBarFrame->setCursor(Qt::SizeAllCursor);
        }
        else
        {
            this->showMaximized();
            m_maximizeBtn->setToolTip(tr("Restore"));
            m_maximizeBtn->setIcon(QIcon("://FramelessDialogResources/minimize-icon.png"));
            m_sizeGrip->hide();

            if(m_isMovable)
                m_topBarFrame->setCursor(Qt::ArrowCursor);

            margin = 0;
            borderRadius = 0;
        }

        mainGridLayout->setMargin(margin);
        mainFrame->setStyleSheet(QString(R"(
                                 #mainFrame{
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
    m_sizeGrip->setVisible(isResizable);
    m_maximizeBtn->setVisible(isResizable);
}

void FramelessDialog::mousePressEvent(QMouseEvent *event)
{
    if(!m_isMovable)
        return;

    if(this->windowState() == Qt::WindowMaximized)
        return;

    if(event->button() == Qt::LeftButton)
    {
        m_originalPosition = this->mapToGlobal(QPoint(0, 0));
//        QRect topBarRect(m_originalPosition.x(), m_originalPosition.y(), this->width(), m_topBarFrame->height());
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
