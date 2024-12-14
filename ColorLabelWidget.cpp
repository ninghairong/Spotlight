#include "ColorLabelWidget.h"
#include <qbuttongroup.h>
#include <QColorDialog>
#include <QDebug>
ColorLabelWidget::ColorLabelWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	// ����QButtonGroup��������QRadioButton
	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->setExclusive(true);  // ȷ��ÿ��ֻ��һ����ť��ѡ��
	QList<QRadioButton*> radioButtons = findChildren<QRadioButton*>();
	for (auto radioButton : radioButtons)
	{
		m_buttonGroup->addButton(radioButton);
	}
	// ����QButtonGroup���źţ���ѡ��ı�ʱ������ɫ
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
	// ��ȡѡ�е�QRadioButton
	QRadioButton* selectedButton = qobject_cast<QRadioButton*>(m_buttonGroup->checkedButton());
	if (selectedButton) {
		// ��ѡ�е�QRadioButton��ȡ��ʽ���еı�����ɫ
		QColor selectedColor = extractColorFromStyleSheet(selectedButton);
		setColor(selectedColor);
	}
}
QColor ColorLabelWidget::extractColorFromStyleSheet(QRadioButton* radioButton) {
	// ��QRadioButton����ʽ������ȡ������ɫ
	QString styleSheet = radioButton->styleSheet();
	QRegularExpression regex("background-color:\\s*(#[0-9a-fA-F]{6}|#[0-9a-fA-F]{3})");
	QRegularExpressionMatch match = regex.match(styleSheet);

	if (match.hasMatch()) {
		QString colorValue = match.captured(1);
		return QColor(colorValue);  // ֱ�ӷ���QColor����
	}
	return QColor();  // ���û���ҵ���ɫ���򷵻�Ĭ����ɫ
}

void ColorLabelWidget::setColor(QColor selectedColor)
{
	QString backgroundColor = selectedColor.name();
	// ���ѡ����Ǻ�ɫ�������ñ߿�Ϊ��ɫ
	QString borderColor = (backgroundColor == "#000000") ? "#aaff7f" : "black";

	// �����µ���ʽ��
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

	// ������ʽӦ�õ���ť
	ui.borderColorButton->setStyleSheet(styleSheet);
	emit colorChanged(selectedColor);
}

void ColorLabelWidget::on_borderColorButton_clicked()
{
	// ����ɫѡ��Ի��򣬻�ȡ�û�ѡ�����ɫ
	QColor selectedColor = QColorDialog::getColor(Qt::gray, this, QString::fromLocal8Bit("ѡ����ɫ"));
	// ����û��Ƿ�ѡ������Ч��ɫ
	if (selectedColor.isValid()) {
		// ��ȡѡ�����ɫ��ʮ�����Ʊ�ʾ
		setColor(selectedColor);
	}
}