#pragma once
#include <ui_ActivationManager.h>
#include <QWidget>
#include <QTimer>
#include <QHotkey>
#include <notification.h>
/*
* 快捷键设置类
* 设置激活方式
* 设置切换方式
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
	//激活信号
	void  isActivated(QPoint pos,bool isActivated);
	//切换信号
	void changed();
private:
	Ui::ActivationManagerClass ui;
	//鼠标
	GlobalMouseHook* m_mouseHook;
	Qt::MouseButton m_buttonChange = Qt::MiddleButton;
	Qt::MouseButton m_buttonActivate = Qt::MiddleButton;
	bool m_isMove = false;
	bool m_isPressed = false;
	bool m_isSpotShow = false;
	QPoint pressPos;
	QTimer* longPressTimer;
	// 保存快捷键对象
	QHotkey* m_pActivateWindowHotkey;
	QHotkey* m_SwitchFunctionHotkey;
	Notification* notification;

private slots:
	//鼠标事件管理
	void mouseControlManger(bool isMouseControl);
	//键盘事件管理
	void keyboardControlManger(bool isKeyboardControl);

	void onGlobalHotkeyChanged(bool isRegister);
	// 槽函数：用户完成快捷键输入时触发
	void onKeySequenceEditFinished(const QKeySequence& keySequence);

	// 槽函数：Hotkey 1 被激活时触发
	void onActivateWindowHotkeyActivated();

	// 槽函数：Hotkey 1 被释放时触发
	void onActivateWindowHotkeyReleased();

	// 槽函数：Hotkey 2 被激活时触发
	void onSwitchFunctionHotkeyActivated();
	//键盘

};
