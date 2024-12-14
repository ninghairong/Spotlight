#include <QApplication>
#include <MainWindow.h>
#include <QProcess>
#include <QSettings>
int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	MainWindow w;
	QString path = QCoreApplication::applicationDirPath();
	QSettings* iniFile = new QSettings(path + "/settings.ini", QSettings::IniFormat);  //保存.ini路径
	iniFile->beginGroup("isShowWindowInfo");
	bool isShow = iniFile->value("isShowWindow", true).toBool();
	iniFile->endGroup();
	if(isShow) w.show();
	int e = app.exec();
	if (e == 931)
	{
		// 传入 qApp->applicationFilePath()，启动自己
		QProcess::startDetached(qApp->applicationFilePath(), QStringList());
		return 0;
	}
	return 0;
}
