#include "BackgroundWindow.h"

BackgroundWindow::BackgroundWindow(const QString& imagePath, QWidget *parent)
    : QMainWindow(parent)
    , m_imagePath(imagePath)
    , m_spriteAnimation(nullptr)
{
    setBackgroundImage(imagePath);
}

BackgroundWindow::BackgroundWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_spriteAnimation(nullptr)
{
    setBackgroundImage(":/res/background2000.png");
}

void BackgroundWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_scaledPixmap.isNull()) {
        painter.drawPixmap(0, 0, m_scaledPixmap);
    }

    QMainWindow::paintEvent(event);
}

void BackgroundWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!m_originalPixmap.isNull()) {
        m_scaledPixmap = m_originalPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
}

void BackgroundWindow::setBackgroundImage(const QString& imagePath)
{
    m_imagePath = imagePath;
    if (m_originalPixmap.load(imagePath)) {
        m_scaledPixmap = m_originalPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
}

void BackgroundWindow::setSpriteAnimation(SpriteAnimation* animation)
{
    if (m_spriteAnimation) {
        m_spriteAnimation->deleteLater();
    }
    m_spriteAnimation = animation;
    if (m_spriteAnimation) {
        m_spriteAnimation->setParent(this);
        m_spriteAnimation->show();
    }
}

void BackgroundWindow::createDefaultAnimation(const QString& spritePath, int frameWidth,
                                              int frameHeight, int frameCount, float scale)
{
    if (m_spriteAnimation) {
        m_spriteAnimation->deleteLater();
    }

    m_spriteAnimation = new SpriteAnimation(this);
    m_spriteAnimation->loadSpriteSheet(spritePath, frameWidth, frameHeight, frameCount, scale);
    m_spriteAnimation->startAnimation(12);
}
