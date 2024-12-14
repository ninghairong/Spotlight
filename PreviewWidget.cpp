#include "PreviewWidget.h"
#include <QPainter>
PreviewWidget::PreviewWidget(QWidget *parent)
	: QWidget(parent)
{
	m_pixMap = QPixmap(":/Spotlight/res/img-b7f5935e94f6deda4fda5e2820c48bef.jpg");
	m_pixMap.scaled(width(), height());
}

PreviewWidget::~PreviewWidget()
{

}

void PreviewWidget::setSpotlightRadius(int radius) {
	m_radius = height() / 2 * 0.8 * radius / 100;
	update();
}

void PreviewWidget::setMaskOpacity(qreal opacity) {

	m_opacity = opacity / 100.0;
	update();
}

void PreviewWidget::setZoomFactor(qreal zoom) {
	m_zoomFactor = zoom / 10;
	update();
}

void PreviewWidget::setIsZoomIn(bool isZoomIn)
{
	m_isZoomIn = isZoomIn;
	update();
}

void PreviewWidget::setBorderColor(int val)
{
	QColor color = QColor::fromHsv(val, 255, 255);
	m_borderColor = color;
	update();
}
void PreviewWidget::setBorderColor(QColor color)
{
	m_borderColor = color;
	update();
}


void PreviewWidget::setBorderWidth(qreal borderWidth)
{
	m_borderWidth = borderWidth;
	update();
}
void PreviewWidget::paintEvent(QPaintEvent* event) {
	m_spotlightPos = QPoint(width() / 2, height() / 2);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	// ����ͼƬ
	QPixmap pixmap(":/Spotlight/res/img-b7f5935e94f6deda4fda5e2820c48bef.jpg");
	// ����ͼƬ�������������
	painter.drawPixmap(rect(), pixmap);
	if (!m_isZoomIn) {
		// �۹��ģʽ
		QRegion spotlightRegion(QRect(m_spotlightPos.x() - m_radius, m_spotlightPos.y() - m_radius, m_radius * 2, m_radius * 2), QRegion::Ellipse);
		QRegion maskRegion = QRegion(rect()).subtracted(spotlightRegion);
		painter.setClipRegion(maskRegion);
		painter.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(m_opacity * 255)));
	}
	else {
		// �Ŵ�ģʽ
		// ����Ŀ���С�� QPixmap
		QPixmap resizedPixmap(width(), height());
		resizedPixmap.fill(Qt::transparent); // ���͸������

		// ʹ�� QPainter ����ԭʼͼƬ��Ŀ���С

		QPainter painter1(&resizedPixmap);
		painter1.setRenderHint(QPainter::Antialiasing);
		painter1.setRenderHint(QPainter::SmoothPixmapTransform,true); // ƽ������
		painter1.drawPixmap(resizedPixmap.rect(), m_pixMap);
		// �Ŵ�����
		QRect zoomRect(m_spotlightPos.x() - m_radius, m_spotlightPos.y() - m_radius, m_radius * 2, m_radius * 2);

		// ����Ŵ�Դ����
		QRectF sourceRect(
			m_spotlightPos.x() - m_radius / m_zoomFactor,
			m_spotlightPos.y() - m_radius / m_zoomFactor,
			(m_radius * 2) / m_zoomFactor,
			(m_radius * 2) / m_zoomFactor
		);

		// ����ƽ�����ر任
		painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

		// ���ƷŴ����ݵ�Բ������
		painter.setClipRegion(QRegion(zoomRect, QRegion::Ellipse));
		painter.drawPixmap(zoomRect, resizedPixmap, sourceRect);
		//painter.drawPixmap(rect(), resizedPixmap, sourceRect);

		// �ָ��������򲢻��Ʊ߿�
		if (m_borderWidth > 0)
		{
			painter.setClipRegion(QRegion(zoomRect));
			QPen pen(m_borderColor);
			pen.setWidth(m_borderWidth);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);

			// ���Ʊ߿�ȷ����Ŵ�����һ��
			painter.drawEllipse(zoomRect.adjusted(m_borderWidth / 2, m_borderWidth / 2, -m_borderWidth / 2, -m_borderWidth / 2));
		}
		
	}
	QWidget::paintEvent(event);
}
