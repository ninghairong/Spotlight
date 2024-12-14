#include "ColorLabelWidget.h"
#include <qbuttongroup.h>
#include <QColorDialog>
#include <QDebug>
ColorLabelWidget::ColorLabelWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// 创建QButtonGroup来管理多个QRadioButton
	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->setExclusive(true);  // 确保每次只有一个按钮被选中
	QList<QRadioButton*> radioButtons = findChildren<QRadioButton*>();
	for (auto radioButton : radioButtons)
	{
		m_buttonGroup->addButton(radioButton);
	}
	// 连接QButtonGroup的信号，当选择改变时更新颜色
	connect(m_buttonGroup, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this, &ColorLabelWidget::updateColor);
}

ColorLabelWidget::~ColorLabelWidget()
{

}
void ColorLabelWidget::setCurrentColor(QColor color)
{
	setColor(color);
}
void ColorLabelWidget::updateColor(QAbstractButton* button) {
	// 获取选中的QRadioButton
	QRadioButton* selectedButton = qobject_cast<QRadioButton*>(m_buttonGroup->checkedButton());
	if (selectedButton) {
		// 从选中的QRadioButton获取样式表中的背景颜色
		QColor selectedColor = extractColorFromStyleSheet(selectedButton);
		setColor(selectedColor);
	}
}
QColor ColorLabelWidget::extractColorFromStyleSheet(QRadioButton* radioButton) {
	// 从QRadioButton的样式表中提取背景颜色
	QString styleSheet = radioButton->styleSheet();
	QRegularExpression regex("background-color:\\s*(#[0-9a-fA-F]{6}|#[0-9a-fA-F]{3})");
	QRegularExpressionMatch match = regex.match(styleSheet);

	if (match.hasMatch()) {
		QString colorValue = match.captured(1);
		return QColor(colorValue);  // 直接返回QColor对象
	}
	return QColor();  // 如果没有找到颜色，则返回默认颜色
}

void ColorLabelWidget::setColor(QColor selectedColor)
{
	QString backgroundColor = selectedColor.name();
	// 如果选择的是黑色，则设置边框为白色
	QString borderColor = (backgroundColor == "#000000") ? "#aaff7f" : "black";

	// 创建新的样式表
	QString styleSheet = QString("QToolButton { "
		"background-color: %1; "
		"border: 1px solid %2; "
		"width: 50px; "
		"height: 50px; "
		"border-radius: 0px; "
		"} "
		"QToolButton:hover { "
		"border: 2px solid black; "
		"} "
		"QToolButton:pressed { "
		"border: 3px solid black; "
		"}").arg(backgroundColor).arg(borderColor);

	// 将新样式应用到按钮
	ui.borderColorButton->setStyleSheet(styleSheet);
	emit colorChanged(selectedColor);
}

void ColorLabelWidget::on_borderColorButton_clicked()
{
	// 打开颜色选择对话框，获取用户选择的颜色
	QColor selectedColor = QColorDialog::getColor(Qt::gray, this, QString::fromLocal8Bit("选择颜色"));
	// 检查用户是否选择了有效颜色
	if (selectedColor.isValid()) {
		// 获取选择的颜色的十六进制表示
		setColor(selectedColor);
	}
}