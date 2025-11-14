#ifndef BACKGROUNDWINDOW_H
#define BACKGROUNDWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPainter>
#include "SpriteAnimation.h"

class BackgroundWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BackgroundWindow(const QString& imagePath, QWidget *parent = nullptr);
    explicit BackgroundWindow(QWidget *parent = nullptr);

    SpriteAnimation* getSpriteAnimation() { return m_spriteAnimation; }
    void setSpriteAnimation(SpriteAnimation* animation);
    void createDefaultAnimation(const QString& spritePath = ":/res/StandSpriteSheet.png",
                                int frameWidth = 64, int frameHeight = 64,
                                int frameCount = 4, float scale = 1.0f);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void setBackgroundImage(const QString& imagePath);

private:
    QPixmap m_originalPixmap;
    QPixmap m_scaledPixmap;
    QString m_imagePath;

    SpriteAnimation* m_spriteAnimation;
};

#endif // BACKGROUNDWINDOW_H
