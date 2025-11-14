#include "SpriteAnimation.h"

SpriteAnimation::SpriteAnimation(QWidget *parent)
    : QWidget(parent)
    , m_currentFrame(0)
    , m_frameCount(0)
    , m_frameWidth(0)
    , m_frameHeight(0)
    , m_scale(1.0f)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &SpriteAnimation::nextFrame);
}

void SpriteAnimation::loadSpriteSheet(const QString &filePath, int frameWidth, int frameHeight, int frameCount, float scale)
{
    QPixmap originalSpriteSheet;
    if (!originalSpriteSheet.load(filePath)) {
        qDebug() << "Failed to load sprite sheet:" << filePath;
        return;
    }

    m_scale = scale;
    m_frameWidth = frameWidth;
    m_frameHeight = frameHeight;
    m_frameCount = frameCount;
    m_currentFrame = 0;

    m_frames.clear();

    int framesPerRow = originalSpriteSheet.width() / frameWidth;

    for (int i = 0; i < frameCount; ++i) {
        int frameX = (i % framesPerRow) * frameWidth;
        int frameY = (i / framesPerRow) * frameHeight;

        QPixmap originalFrame = originalSpriteSheet.copy(frameX, frameY, frameWidth, frameHeight);

        QPixmap scaledFrame = originalFrame.scaled(
            frameWidth * scale,
            frameHeight * scale,
            Qt::KeepAspectRatio,
            Qt::FastTransformation
            );

        m_frames.append(scaledFrame);
    }

    setFixedSize(m_frameWidth * m_scale, m_frameHeight * m_scale);
    update();
}

void SpriteAnimation::startAnimation(int fps)
{
    m_timer->start(1000 / fps);
}

void SpriteAnimation::stopAnimation()
{
    m_timer->stop();
}

void SpriteAnimation::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (m_frames.isEmpty() || m_currentFrame >= m_frames.size()) {
        qDebug() << "Paint event: no frames to draw. Frames count:" << m_frames.size() << "Current frame:" << m_currentFrame;
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter.drawPixmap(0, 0, m_frames[m_currentFrame]);
}

void SpriteAnimation::nextFrame()
{
    m_currentFrame = (m_currentFrame + 1) % m_frameCount;
    update();
}

void SpriteAnimation::setScale(float scale)
{
    m_scale = scale;
    if (m_frameWidth > 0 && m_frameHeight > 0) {
        setFixedSize(m_frameWidth * m_scale, m_frameHeight * m_scale);
    }
    update();
}

void SpriteAnimation::setPosition(int x, int y)
{
    move(x, y);
}
