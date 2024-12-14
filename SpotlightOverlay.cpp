#include "SpotlightOverlay.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
SpotlightOverlay* SpotlightOverlay::m_pSpotlight = nullptr;

SpotlightOverlay* SpotlightOverlay::GetSpotlightOverlay()
{
	if (m_pSpotlight == nullptr)
	{
		m_pSpotlight = new SpotlightOverlay;
	}
	return m_pSpotlight;
}

SpotlightOverlay::SpotlightOverlay(QWidget* parent) : QWidget(parent) {
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_ShowWithoutActivating);
	resize(QApplication::primaryScreen()->size());

}

void SpotlightOverlay::setSpotlightRadius(int radius) {
	m_radius = height() / 2 * 0.8 * radius / 100;
	update();
}

void SpotlightOverlay::setMaskOpacity(qreal opacity) {

	m_opacity = opacity / 100.0;
	update();
}

void SpotlightOverlay::setZoomFactor(qreal zoom) {
	m_zoomFactor = zoom / 10;
	update();
}

void SpotlightOverlay::setIsZoomIn(bool isZoomIn)
{
	m_isZoomIn = isZoomIn;
	update();
}

void SpotlightOverlay::setBorderColor(int val)
{
	QColor color = QColor::fromHsv(val, 255, 255);
	m_borderColor = color;
	update();
}
void SpotlightOverlay::setBorderColor(QColor color)
{
	m_borderColor = color;
	update();
}

void SpotlightOverlay::setBorderWidth(qreal borderWidth)
{
	m_borderWidth = borderWidth;
	update();
}

void SpotlightOverlay::updateSpotlightPosition(const QPoint& pos) {
	m_spotlightPos = pos;
	update();
}

void SpotlightOverlay::showSpotlight() {
	m_isActive = true;
	if (m_isZoomIn)
	{
		QScreen* pScreen = QGuiApplication::primaryScreen();
		m_pixMap = pScreen->grabWindow(0);
	}
	showFullScreen();
	update();
}

void SpotlightOverlay::hideSpotlight() {
	m_isActive = false;
	hide();
}

void SpotlightOverlay::paintEvent(QPaintEvent* event) {
	if (!m_isActive) return;

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (!m_isZoomIn) {
		// 聚光灯模式
		QRegion spotlightRegion(QRect(m_spotlightPos.x() - m_radius, m_spotlightPos.y() - m_radius, m_radius * 2, m_radius * 2), QRegion::Ellipse);
		QRegion maskRegion = QRegion(rect()).subtracted(spotlightRegion);
		painter.setClipRegion(maskRegion);
		painter.fillRect(rect(), QColor(0, 0, 0, static_cast<int>(m_opacity * 255)));
	}
	else {
		// 放大镜模式
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
		painter.drawPixmap(zoomRect, m_pixMap, sourceRect);

		if (m_borderWidth > 0)
		{
			//绘制边框
			painter.setClipRegion(QRegion(zoomRect));
			QPen pen(m_borderColor);
			pen.setWidth(m_borderWidth);
			painter.setPen(pen);
			painter.setBrush(Qt::NoBrush);
			// 绘制边框，确保与放大镜区域一致
			painter.drawEllipse(zoomRect.adjusted(m_borderWidth / 2, m_borderWidth / 2, -m_borderWidth / 2, -m_borderWidth / 2));
		}
	}
}
