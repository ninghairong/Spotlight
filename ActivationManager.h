#pragma once
#include <ui_ActivationManager.h>
#include <QWidget>
#include <QTimer>
#include <QHotkey>
#include <notification.h>
/*
* ��ݼ�������
* ���ü��ʽ
* �����л���ʽ
*/
class GlobalMouseHook;
class ActivationManager  : public QWidget
{
	Q_OBJECT

public:
	ActivationManager(QWidget* parent);
	~ActivationManager();
	void saveSetParameter();
signals:
	//�����ź�
	void  isActivated(QPoint pos,bool isActivated);
	//�л��ź�
	void changed();
private:
	Ui::ActivationManagerClass ui;
	//���
	GlobalMouseHook* m_mouseHook;
	Qt::MouseButton m_buttonChange = Qt::MiddleButton;
	Qt::MouseButton m_buttonActivate = Qt::MiddleButton;
	bool m_isMove = false;
	bool m_isPressed = false;
	bool m_isSpotShow = false;
	QPoint pressPos;
	QTimer* longPressTimer;
	// �����ݼ�����
	QHotkey* m_pActivateWindowHotkey;
	QHotkey* m_SwitchFunctionHotkey;
	Notification* notification;

private slots:
	//����¼�����
	void mouseControlManger(bool isMouseControl);
	//�����¼�����
	void keyboardControlManger(bool isKeyboardControl);

	void onGlobalHotkeyChanged(bool isRegister);
	// �ۺ������û���ɿ�ݼ�����ʱ����
	void onKeySequenceEditFinished(const QKeySequence& keySequence);

	// �ۺ�����Hotkey 1 ������ʱ����
	void onActivateWindowHotkeyActivated();

	// �ۺ�����Hotkey 1 ���ͷ�ʱ����
	void onActivateWindowHotkeyReleased();

	// �ۺ�����Hotkey 2 ������ʱ����
	void onSwitchFunctionHotkeyActivated();
	//����

};
