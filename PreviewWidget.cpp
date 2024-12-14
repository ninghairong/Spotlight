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
	// 加载图片
	QPixmap pixmap(":/Spotlight/res/img-b7f5935e94f6deda4fda5e2820c48bef.jpg");
	// 绘制图片，填充整个窗口
	painter.drawPixmap(rect(), pixmap);
	if (!m_isZoomIn) {
		// 聚光灯模式
		QRegion spotlightRegion(QRect(m_spotlightPos.x() - m_radius, m_spotlightPos.y() - m_radius, m_radius * 2, m_radius * 2), QRegion::Ellipse);
		QRegion maskRegion = QRegion(rect()).subtracted(spotlightRegion);
		painter.setClipRegion(maskRegion);
		painter.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(m_opacity * 255)));
	}
	else {
		// 放大镜模式
		// 创建目标大小的 QPixmap
		QPixmap resizedPixmap(width(), height());
		resizedPixmap.fill(Qt::transparent); // 填充透明背景

		// 使用 QPainter 绘制原始图片到目标大小

		QPainter painter1(&resizedPixmap);
		painter1.setRenderHint(QPainter::Antialiasing);
		painter1.setRenderHint(QPainter::SmoothPixmapTransform,true); // 平滑绘制
		painter1.drawPixmap(resizedPixmap.rect(), m_pixMap);
		// 放大镜区域
		QRect zoomRect(m_spotlightPos.x() - m_radius, m_spotlightPos.y() - m_radius, m_radius * 2, m_radius * 2);

		// 计算放大源区域
		QRectF sourceRect(
			m_spotlightPos.x() - m_radius / m_zoomFactor,
			m_spotlightPos.y() - m_radius / m_zoomFactor,
			(m_radius * 2) / m_zoomFactor,
			(m_radius * 2) / m_zoomFactor
		);

		// 启用平滑像素变换
		painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

		// 绘制放大内容到圆形区域
		painter.setClipRegion(QRegion(zoomRect, QRegion::Ellipse));
		painter.drawPixmap(zoomRect, resizedPixmap, sourceRect);
		//painter.drawPixmap(rect(), resizedPixmap, sourceRect);

		// 恢复剪辑区域并绘制边框
		if (m_borderWidth > 0)
		{
			painter.setClipRegion(QRegion(zoomRect));
			QPen pen(m_borderColor);
			pen.setWidth(m_borderWidth);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);

			// 绘制边框，确保与放大镜区域一致
			painter.drawEllipse(zoomRect.adjusted(m_borderWidth / 2, m_borderWidth / 2, -m_borderWidth / 2, -m_borderWidth / 2));
		}
		
	}
	QWidget::paintEvent(event);
}
