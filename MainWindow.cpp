#include "MainWindow.h"
#include <QSettings>

#include <QDebug>
//初始化鼠标和托盘事件
void MainWindow::InitMouseAndTray()
{
	// 创建聚光灯窗口
	m_spotlight = SpotlightOverlay::GetSpotlightOverlay();

	// 创建全局鼠标钩子
	m_mouseHook = GlobalMouseHook::GetGlobalMouseHook();

	m_pActivationManager = ui.activationManager;

	connect(m_pActivationManager, &ActivationManager::changed, [=] {
		bool isChecked = ui.zoomInRadio->isChecked(); ui.zoomInRadio->setChecked(!isChecked); ui.lightRadio->setChecked(isChecked); update();
		});
	
	connect(m_pActivationManager, &ActivationManager::isActivated, [=](const QPoint& postion,bool activated) {
		if (activated)
		{
			m_spotlight->updateSpotlightPosition(postion);
			m_spotlight->showSpotlight();
			connect(m_mouseHook, &GlobalMouseHook::mouseMoved, [=](const QPoint& pos) {
				m_spotlight->updateSpotlightPosition(pos);
				});
		}
		else
		{
			m_spotlight->hideSpotlight();
			disconnect(m_mouseHook, &GlobalMouseHook::mouseMoved,0,0);
		}
		});
	if (!m_mouseHook->installHook()) {
		qFatal("Failed to install global mouse hook!");
	}
	//程序托盘

	// 初始化托盘图标
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setIcon(QIcon(":/Spotlight/res/icon.png")); // 设置托盘图标（需提供图标路径）
	trayIcon->setToolTip(QString::fromLocal8Bit("Spotlight"));

	// 创建托盘菜单
	QMenu* trayMenu = new QMenu(this);
	QAction* restoreAction = new QAction(QString::fromLocal8Bit("打开主窗口"), this);
	QAction* changeMode = new QAction(QString::fromLocal8Bit("切换"), this);
	QAction* restartAction = new QAction(QString::fromLocal8Bit("重新启动"), this);
	QAction* quitAction = new QAction(QString::fromLocal8Bit("退出"), this);

	trayMenu->addAction(restoreAction);
	trayMenu->addAction(changeMode);
	trayMenu->addAction(restartAction);
	trayMenu->addSeparator();
	trayMenu->addAction(quitAction);

	trayIcon->setContextMenu(trayMenu);

	// 信号与槽连接
	connect(restoreAction, &QAction::triggered, this, &MainWindow::showMainWindow);
	connect(changeMode, &QAction::triggered, this, [&]() {bool isChecked = ui.zoomInRadio->isChecked(); ui.zoomInRadio->setChecked(!isChecked); ui.lightRadio->setChecked(isChecked); update(); });
	connect(restartAction, &QAction::triggered, this, &MainWindow::restartApplication);
	connect(quitAction, &QAction::triggered, this, &MainWindow::exitApplication);
	connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);

	// 显示托盘图标
	trayIcon->show();
}
//初始化配置信息
void MainWindow::InitConfigInfo()
{
	QString path = QCoreApplication::applicationDirPath();          //获取源文件路径
	QSettings* m_iniFile = new QSettings(path + "/settings.ini", QSettings::IniFormat);  //保存.ini路径
	m_iniFile->beginGroup("showInfo");
	bool zoomIn = m_iniFile->value("zoomIn", false).toBool();
	int lightSize = m_iniFile->value("lightSize", 40).toInt();
	int contrast = m_iniFile->value("contrast", 60).toInt();
	int zoomInSize = m_iniFile->value("zoomInSize", 40).toInt();
	int zoomFactor = m_iniFile->value("zoomFactor", 15).toInt();
	QString borderColor = m_iniFile->value("borderColor", "#ffffff").toString();
	int borderWidth = m_iniFile->value("borderWidth", 2).toInt();
	m_iniFile->endGroup();
	ui.zoomInRadio->setChecked(zoomIn);
	m_spotlight->setIsZoomIn(zoomIn);
	ui.previewWidget->setIsZoomIn(zoomIn);
	ui.zoomInSizeSlider->setValue(zoomInSize);
	ui.zoomInSizeLabel->setText(QString::number(zoomInSize) + "%");
	ui.lightSizeSlider->setValue(lightSize);
	ui.lightSizeLabel->setText(QString::number(lightSize) + "%");
	if (zoomIn)
	{
		m_spotlight->setSpotlightRadius(zoomInSize);
		ui.previewWidget->setSpotlightRadius(zoomInSize);
	}
	else
	{
		m_spotlight->setSpotlightRadius(lightSize);
		ui.previewWidget->setSpotlightRadius(lightSize);
	}
	ui.zoomFactorSlider->setValue(zoomFactor);
	ui.zoomFactorLabel->setText(QString::number(zoomFactor / 10.0, 'f', 1) + QString::fromLocal8Bit("倍"));
	m_spotlight->setZoomFactor(zoomFactor);
	ui.previewWidget->setZoomFactor(zoomFactor);

	ui.contrastSlider->setValue(contrast);
	ui.contrastLabel->setText(QString::number(contrast) + "%");
	m_spotlight->setMaskOpacity(contrast);
	ui.previewWidget->setMaskOpacity(contrast);

	//ui.borderColorSlider->setValue(borderColor);
	//QColor color = QColor::fromHsv(borderColor, 255, 255);
	//QColor color(borderColor);
	//ui.borderColorLabel->setStyleSheet(QString("background-color: %1").arg(color.name()));
	ui.colorSelectWidget->setCurrentColor(borderColor);
	m_spotlight->setBorderColor(borderColor);
	ui.previewWidget->setBorderColor(borderColor);

	ui.borderWidthSlider->setValue(borderWidth);
	ui.borderWidthLabel->setText(QString::number(borderWidth) + "px");
	m_spotlight->setBorderWidth(borderWidth);
	ui.previewWidget->setBorderWidth(borderWidth);
}
//初始化UI事件
void MainWindow::InitUIEvent()
{
	connect(ui.zoomInRadio, &QRadioButton::toggled, [=](bool isChecked)
		{
			m_spotlight->setIsZoomIn(isChecked);
			ui.previewWidget->setIsZoomIn(isChecked);

			if (isChecked)
			{
				m_spotlight->setSpotlightRadius(ui.zoomInSizeSlider->value());
				ui.previewWidget->setSpotlightRadius(ui.zoomInSizeSlider->value());
				ui.stackedWidget->setCurrentIndex(0);
			}
			else
			{
				m_spotlight->setSpotlightRadius(ui.lightSizeSlider->value());
				ui.previewWidget->setSpotlightRadius(ui.lightSizeSlider->value());
				ui.stackedWidget->setCurrentIndex(1);
			}
		});
	connect(ui.lightSizeSlider, &QSlider::valueChanged, [=](int val)
		{
			m_spotlight->setSpotlightRadius(val);
			ui.previewWidget->setSpotlightRadius(val);
			ui.lightSizeLabel->setText(QString::number(val) + "%");
		});
	connect(ui.zoomInSizeSlider, &QSlider::valueChanged, [=](int val)
		{
			m_spotlight->setSpotlightRadius(val);
			ui.previewWidget->setSpotlightRadius(val);
			ui.zoomInSizeLabel->setText(QString::number(val) + "%");
		});
	connect(ui.zoomFactorSlider, &QSlider::valueChanged, [=](int val)
		{
			m_spotlight->setZoomFactor(val);
			ui.previewWidget->setZoomFactor(val);
			ui.zoomFactorLabel->setText(QString::number(val / 10.0, 'f', 1) + QString::fromLocal8Bit("倍"));
		});
	connect(ui.contrastSlider, &QSlider::valueChanged, [=](int val)
		{
			m_spotlight->setMaskOpacity(val);
			ui.previewWidget->setMaskOpacity(val);
			ui.contrastLabel->setText(QString::number(val) + "%");
		});
	//connect(ui.borderColorSlider, &QSlider::valueChanged, this, [=](int val) {
	//	m_spotlight->setBorderColor(val);
	//	ui.previewWidget->setBorderColor(val);
	//	//QColor color = QColor::fromHsv(val, 255, 255);
	//	QColor color(val);
	//	ui.borderColorLabel->setStyleSheet(QString("background-color: %1").arg(color.name()));
	//	});
	connect(ui.borderWidthSlider, &QSlider::valueChanged, this, [=](int val) {
		m_spotlight->setBorderWidth(val);
		ui.previewWidget->setBorderWidth(val);
		ui.borderWidthLabel->setText(QString::number(val) + "px");
		});

	connect(ui.colorSelectWidget, &ColorLabelWidget::colorChanged, [=](QColor color)
		{
			m_spotlight->setBorderColor(color);
			ui.previewWidget->setBorderColor(color);
			m_borderColor = color;
		});
}


MainWindow::MainWindow(QWidget* parent)
	: QWidget(parent)
{
	//绑定UI
	ui.setupUi(this);
	//初始化鼠标和托盘事件
	InitMouseAndTray();
	//初始化ui事件
	InitUIEvent();
	//初始化配置信息
	InitConfigInfo();
	DonationWindow* donation = new DonationWindow;

	connect(ui.pushButton, &QPushButton::clicked, this, [=] {	
		donation->show();
		});
}

MainWindow::~MainWindow()
{
	//释放资源
	m_mouseHook->uninstallHook();
	m_mouseHook->deleteLater();
	m_spotlight->deleteLater();
}


void MainWindow::showMainWindow() {
	showNormal(); // 恢复主窗口
	activateWindow(); // 激活窗口
}

void  MainWindow::restartApplication() {
	//QMessageBox::information(this, QString::fromLocal8Bit("重新启动"), QString::fromLocal8Bit("程序将重新启动..."));
	qApp->exit(931);
}

void  MainWindow::exitApplication() {
	trayIcon->hide(); // 隐藏托盘图标
	qApp->quit(); // 退出应用程序
}

void  MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		showMainWindow(); // 左键单击显示主窗口
	}
}
//保存配置参数
void MainWindow::saveSetParameter()
{
	QString path = QCoreApplication::applicationDirPath();          //获取源文件路径
	QSettings* m_iniFile = new QSettings(path + "/settings.ini", QSettings::IniFormat);  //保存.ini路径
	m_iniFile->beginGroup("showInfo");
	m_iniFile->setValue("zoomIn", ui.zoomInRadio->isChecked());
	m_iniFile->setValue("lightSize", ui.lightSizeSlider->value());
	m_iniFile->setValue("contrast", ui.contrastSlider->value());
	m_iniFile->setValue("zoomInSize", ui.zoomInSizeSlider->value());
	m_iniFile->setValue("zoomFactor", ui.zoomFactorSlider->value());
	m_iniFile->setValue("borderColor", m_borderColor.name());
	m_iniFile->setValue("borderWidth", ui.borderWidthSlider->value());
	m_iniFile->endGroup();
}

void MainWindow::closeEvent(QCloseEvent* event) {
	saveSetParameter();
	m_pActivationManager->saveSetParameter();
	if (trayIcon->isVisible()) {
		hide(); // 隐藏主窗口
		event->ignore(); // 忽略关闭事件
	}
	else {
		QWidget::closeEvent(event); // 调用基类处理
	}
}

void MainWindow::onTabChanged(int index)
{
	saveSetParameter();
	m_pActivationManager->saveSetParameter();
}
