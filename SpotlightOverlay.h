#ifndef SPOTLIGHTOVERLAY_H
#define SPOTLIGHTOVERLAY_H

#include <QWidget>
#include <QPoint>
#include <QPainter>
#include <QTimer>

class SpotlightOverlay : public QWidget {
    Q_OBJECT

public:
   static SpotlightOverlay* GetSpotlightOverlay();
    void setSpotlightRadius(int radius);
    void setMaskOpacity(qreal opacity);
    void setZoomFactor(qreal zoom);
    void setIsZoomIn(bool isZoomIn);
    void setBorderColor(int color);
    void setBorderColor(QColor color);
    void setBorderWidth(qreal borderWidth);
    void updateSpotlightPosition(const QPoint& pos);
    void showSpotlight();
    void hideSpotlight();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPoint m_spotlightPos; // 聚光灯位置
    int m_radius = 200;    // 聚光灯半径
    qreal m_opacity = 0.8; // 遮罩透明度
    qreal m_zoomFactor = 1.5; // 放大倍数
    bool m_isActive = false; // 是否激活
    bool m_isZoomIn = true; // 是否放大
    QColor m_borderColor = Qt::white; //放大镜边框颜色
    qreal m_borderWidth = 1;
    QPixmap m_pixMap;

    //单例模式
    SpotlightOverlay(QWidget* parent = nullptr);
    static SpotlightOverlay* m_pSpotlight;
};

#endif // SPOTLIGHTOVERLAY_H
