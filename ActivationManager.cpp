#include "ActivationManager.h"
#include <GlobalMouseHook.h>
#include <QCursor>
#include <QSettings>
#include <qdebug.h>

ActivationManager::ActivationManager(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_pActivateWindowHotkey = new QHotkey(ui.activatekeySequenceEdit->keySequence(), true, this);  // �Զ�ע��
	connect(m_pActivateWindowHotkey, &QHotkey::activated, this, &ActivationManager::onActivateWindowHotkeyActivated);
	connect(m_pActivateWindowHotkey, &QHotkey::released, this, &ActivationManager::onActivateWindowHotkeyReleased);
	connect(m_pActivateWindowHotkey, &QHotkey::registeredChanged, this, &ActivationManager::onGlobalHotkeyChanged);
	m_SwitchFunctionHotkey = new QHotkey(ui.changeKeySequenceEdit->keySequence(), true, this);  // �Զ�ע��
	connect(m_SwitchFunctionHotkey, &QHotkey::activated, this, &ActivationManager::onSwitchFunctionHotkeyActivated);
	connect(m_SwitchFunctionHotkey, &QHotkey::registeredChanged, this, &ActivationManager::onGlobalHotkeyChanged);
	notification = new Notification(nullptr);
	m_mouseHook = GlobalMouseHook::GetGlobalMouseHook();
	//��ʼ��������Ϣ
	QString path = QCoreApplication::applicationDirPath();          //��ȡԴ�ļ�·��
	QSettings* m_iniFile = new QSettings(path + "/settings.ini", QSettings::IniFormat);  //����.ini·��
	m_iniFile->beginGroup("controlInfo");
	bool openMouseControl = m_iniFile->value("openMouseControl", true).toBool();
	int activateMouseButton = m_iniFile->value("activateMouseButton", 0).toInt();
	int changedMouseButton = m_iniFile->value("changeMouseButton", 0).toInt();
	int openKeyboardControl = m_iniFile->value("openKeyboardControl", true).toBool();
	QString activateKeySequence = m_iniFile->value("activateKeySequence", "Ctrl+Shift+CapsLock").toString();
	QString changeKeySequence = m_iniFile->value("changeKeySequence", "Ctrl+Space").toString();
	m_iniFile->endGroup();
	//���������ļ���ʼ��UI
	//���
	connect(ui.isMouseControlButton, &QRadioButton::toggled, this, &ActivationManager::mouseControlManger);
	connect(ui.mouseActivateComBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
		{
			if (index == 0)
			{
				m_buttonActivate = Qt::MiddleButton;
			}
			else if (index == 1)
			{
				m_buttonActivate = Qt::RightButton;
			}
		});
	connect(ui.mouseChangeComBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index)
		{
			if (index == 0)
			{
				m_buttonChange = Qt::MiddleButton;
			}
			else if (index == 1)
			{
				m_buttonChange = Qt::RightButton;
			}
		});
	ui.isMouseControlButton->setChecked(openMouseControl);
	ui.mouseActivateComBox->setCurrentIndex(activateMouseButton);
	ui.mouseChangeComBox->setCurrentIndex(changedMouseButton);
	if (activateMouseButton == 0) m_buttonActivate = Qt::MiddleButton;
	else if (activateMouseButton == 1)m_buttonActivate = Qt::RightButton;
	if (changedMouseButton == 0)m_buttonChange = Qt::MiddleButton;
	else if (changedMouseButton == 1)
		m_buttonChange = Qt::RightButton;
	/*ui.mouseActivateComBox->setEnabled(openMouseControl);
	ui.mouseChangeComBox->setEnabled(openMouseControl);*/

	//����
	connect(ui.isKeyboardControlButton, &QRadioButton::toggled, this, &ActivationManager::keyboardControlManger);
	// ������ݼ��༭����ź�
	connect(ui.activatekeySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, &ActivationManager::onKeySequenceEditFinished);
	connect(ui.changeKeySequenceEdit, &QKeySequenceEdit::keySequenceChanged, this, &ActivationManager::onKeySequenceEditFinished);
	/*ui.activatekeySequenceEdit->setEnabled(openKeyboardControl);
	ui.changeKeySequenceEdit->setEnabled(openKeyboardControl);*/
	//��ʼ������
	ui.activatekeySequenceEdit->setKeySequence(activateKeySequence);
	ui.changeKeySequenceEdit->setKeySequence(changeKeySequence);
	ui.isKeyboardControlButton->setChecked(openKeyboardControl);
	mouseControlManger(openMouseControl);
	keyboardControlManger(openKeyboardControl);
}

ActivationManager::~ActivationManager()
{
	// ����ʱȡ��ע���ݼ�
	if (m_pActivateWindowHotkey) {
		m_pActivateWindowHotkey->setRegistered(false);
		delete m_pActivateWindowHotkey;
	}
	if (m_SwitchFunctionHotkey) {
		m_SwitchFunctionHotkey->setRegistered(false);
		delete m_SwitchFunctionHotkey;
	}
}
void ActivationManager::keyboardControlManger(bool isKeyboardControl)
{
	qDebug() << "keyboardControlManger";
	ui.activatekeySequenceEdit->setEnabled(isKeyboardControl);
	ui.changeKeySequenceEdit->setEnabled(isKeyboardControl);
	m_pActivateWindowHotkey->setRegistered(isKeyboardControl);
	m_SwitchFunctionHotkey->setRegistered(isKeyboardControl);
}

void ActivationManager::mouseControlManger(bool isMouseControl)
{
	//��������������ź�
	qDebug() << "mouseControlManger toggle";
	ui.mouseActivateComBox->setEnabled(isMouseControl);
	ui.mouseChangeComBox->setEnabled(isMouseControl);
	if (isMouseControl)
	{
		connect(m_mouseHook, &GlobalMouseHook::mouseButtonDCliecked, [=](QPoint pos, Qt::MouseButton button) {
			if (button == m_buttonChange) {
				notification->Push(QString::fromLocal8Bit("�л��ɹ���"), Notify_Type_Success, Pos_Bottom_Right, 1000);
				emit changed();
			}
			});
		connect(m_mouseHook, &GlobalMouseHook::mouseButtonPressed, [=](QPoint pos, Qt::MouseButton button) {
			if (button == m_buttonActivate) {
				//m_isPressed = true;
				//longPressTimer->start(1000); // 1 ��󴥷�����
				//pressPos = pos;    // ��¼����λ��
				emit isActivated(pos, true);
			}
			});

		connect(m_mouseHook, &GlobalMouseHook::mouseButtonReleased, [=](QPoint pos, Qt::MouseButton button) {
			if (button == m_buttonActivate) {
				qDebug() << "Released";
				emit isActivated(pos, false);
			}
			});

	}
	else
	{
		disconnect(m_mouseHook, 0, 0, 0);
	}
	//�ͷ�����������ź�
}

// �ۺ��� - ���û���ɿ�ݼ�����ʱ����
void ActivationManager::onKeySequenceEditFinished(const QKeySequence& keySequence)
{
	QKeySequenceEdit* p_sender = dynamic_cast<QKeySequenceEdit*>(sender());
	// ��ȡ�û�����Ŀ�ݼ�
	QKeySequence sequence = keySequence;
	// ����ݼ��Ƿ���Ч
	if (sequence.isEmpty()) {
		notification->Push(QString::fromLocal8Bit("��������Ч�Ŀ�ݼ�"), Notify_Type_Error);
		return;
	}
	if (p_sender == ui.activatekeySequenceEdit)
	{
		m_pActivateWindowHotkey->setShortcut(sequence, true);  // ���¿�ݼ�������ע��
	}
	else
	{
		m_SwitchFunctionHotkey->setShortcut(sequence, true);  // ���¿�ݼ�������ע��
	}
}

// �ۺ��� - Hotkey 1 ������ʱ����
void ActivationManager::onActivateWindowHotkeyActivated()
{
	QPoint globalMousePos = QCursor::pos();
	emit isActivated(globalMousePos, true);
}

// �ۺ��� - Hotkey 2 ������ʱ����
void ActivationManager::onSwitchFunctionHotkeyActivated()
{
	notification->Push(QString::fromLocal8Bit("�л��ɹ���"), Notify_Type_Success, Pos_Bottom_Right, 1000);
	emit changed();
}
// �ۺ��� - Hotkey 1 ���ͷ�ʱ����
void ActivationManager::onActivateWindowHotkeyReleased()
{
	QPoint globalMousePos = QCursor::pos();
	emit isActivated(globalMousePos, false);
}

void ActivationManager::onGlobalHotkeyChanged(bool isRegister)
{
	qDebug() << "onGlobalHotkeyChanged";
	QHotkey* p_sender = dynamic_cast<QHotkey*>(sender());
	if (p_sender == m_SwitchFunctionHotkey)
	{
		if (isRegister) {
			ui.keyChangeValid->setPixmap(QPixmap(":/Spotlight/res/type_success.png"));
			ui.keyChangeValid->adjustSize();
			//notification->Push(QString::fromLocal8Bit("��ݼ�ע��ɹ�"), Notify_Type_Success);
		}
		else {
			ui.keyChangeValid->setPixmap(QPixmap(":/Spotlight/res/type_error.png"));
			ui.keyChangeValid->adjustSize();
			//notification->Push(QString::fromLocal8Bit("��ݼ��ѱ�ռ��,���������룡"), Notify_Type_Error);
		}
	}
	else if (p_sender == m_pActivateWindowHotkey)
	{
		if (isRegister) {
			ui.keyActivateValid->setPixmap(QPixmap(":/Spotlight/res/type_success.png"));
			ui.keyActivateValid->adjustSize();
			//notification->Push(QString::fromLocal8Bit("��ݼ�ע��ɹ�"), Notify_Type_Success);
		}
		else {
			ui.keyActivateValid->setPixmap(QPixmap(":/Spotlight/res/type_error.png"));
			ui.keyActivateValid->adjustSize();
			//notification->Push(QString::fromLocal8Bit("��ݼ��ѱ�ռ��,���������룡"), Notify_Type_Error);
		}
	}
}

void ActivationManager::saveSetParameter()
{
	//����������Ϣ
	QString path = QCoreApplication::applicationDirPath();          //��ȡԴ�ļ�·��
	QSettings* m_iniFile = new QSettings(path + "/settings.ini", QSettings::IniFormat);  //����.ini·��
	m_iniFile->beginGroup("controlInfo");
	m_iniFile->setValue("openMouseControl", ui.isMouseControlButton->isChecked());
	m_iniFile->setValue("activateMouseButton", ui.mouseActivateComBox->currentIndex());
	m_iniFile->setValue("changeMouseButton", ui.mouseChangeComBox->currentIndex());
	m_iniFile->setValue("openKeyboardControl", ui.isKeyboardControlButton->isChecked());
	m_iniFile->setValue("activateKeySequence", ui.activatekeySequenceEdit->keySequence().toString());
	m_iniFile->setValue("changeKeySequence", ui.changeKeySequenceEdit->keySequence().toString());
	m_iniFile->endGroup();
}