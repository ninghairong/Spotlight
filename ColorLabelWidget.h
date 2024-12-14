#pragma once

#include <QWidget>
#include <ui_ColorLabelWidget.h>
#include <QButtonGroup>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
class ColorLabelWidget : public QWidget
{
	Q_OBJECT
public:
	ColorLabelWidget(QWidget* parent = nullptr);
	~ColorLabelWidget();
	void setCurrentColor(QColor color);
private:
	Ui::ColorLabelWidgetClass ui;
	QButtonGroup* m_buttonGroup;
	QColor extractColorFromStyleSheet(QRadioButton* radioButton);
	void setColor(QColor color);
private slots:
	void updateColor(QAbstractButton* button);
	void on_borderColorButton_clicked();
signals:
	// 定义信号，传递颜色
	void colorChanged(const QColor& newColor);
};
