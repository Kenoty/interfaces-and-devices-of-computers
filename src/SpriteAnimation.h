#ifndef SPRITEANIMATION_H
#define SPRITEANIMATION_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPainter>

class SpriteAnimation : public QWidget
{
    Q_OBJECT

public:
    SpriteAnimation(QWidget *parent = nullptr);
    void loadSpriteSheet(const QString &filePath, int frameWidth, int frameHeight, int frameCount, float scale = 1.0f);
    void startAnimation(int fps = 24);
    void stopAnimation();
    void setScale(float scale);
    void setPosition(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void nextFrame();

private:
    QVector<QPixmap> m_frames;
    QPixmap m_spriteSheet;
    QTimer *m_timer;
    int m_currentFrame;
    int m_frameCount;
    int m_frameWidth;
    int m_frameHeight;
    float m_scale;
};

#endif // SPRITEANIMATION_H
