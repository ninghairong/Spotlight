#pragma once

#include <QWidget>

class PreviewWidget  : public QWidget
{
	Q_OBJECT

public:
	PreviewWidget(QWidget *parent);
	~PreviewWidget();
    void setSpotlightRadius(int radius);
    void setMaskOpacity(qreal opacity);
    void setZoomFactor(qreal zoom);
    void setIsZoomIn(bool isZoomIn);
    void setBorderColor(int color);
    void setBorderColor(QColor color);
    void setBorderWidth(qreal borderWidth);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPoint m_spotlightPos; // �۹��λ��
    int m_radius = 50;    // �۹�ư뾶
    qreal m_opacity = 0.8; // ����͸����
    qreal m_zoomFactor = 1.5; // �Ŵ���
    bool m_isZoomIn = true; // �Ƿ�Ŵ�
    QColor m_borderColor = Qt::white; //�Ŵ󾵱߿���ɫ
    qreal m_borderWidth = 1;
    QPixmap m_pixMap;
};
