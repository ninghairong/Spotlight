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
    QPoint m_spotlightPos; // �۹��λ��
    int m_radius = 200;    // �۹�ư뾶
    qreal m_opacity = 0.8; // ����͸����
    qreal m_zoomFactor = 1.5; // �Ŵ���
    bool m_isActive = false; // �Ƿ񼤻�
    bool m_isZoomIn = true; // �Ƿ�Ŵ�
    QColor m_borderColor = Qt::white; //�Ŵ󾵱߿���ɫ
    qreal m_borderWidth = 1;
    QPixmap m_pixMap;

    //����ģʽ
    SpotlightOverlay(QWidget* parent = nullptr);
    static SpotlightOverlay* m_pSpotlight;
};

#endif // SPOTLIGHTOVERLAY_H
