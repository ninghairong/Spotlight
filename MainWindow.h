#pragma once

#include <QWidget>
#include "ui_MainWindow.h"
#include "GlobalMouseHook.h"
#include "SpotlightOverlay.h"
#include <QApplication>
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QGraphicsScene>
#include <ActivationManager.h>
class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();
protected:
	void closeEvent(QCloseEvent* event) override;
private:
	Ui::MainWindowClass ui;
	SpotlightOverlay* m_spotlight;
	GlobalMouseHook* m_mouseHook;
	QSystemTrayIcon* trayIcon;
	QColor m_borderColor;
	ActivationManager* m_pActivationManager;
	//functional
	void InitMouseAndTray();
	void InitUIEvent();
	void InitConfigInfo();

private slots:
	void showMainWindow();

	void restartApplication();

	void exitApplication();

	void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

	void saveSetParameter();

	void onTabChanged(int index);
};
