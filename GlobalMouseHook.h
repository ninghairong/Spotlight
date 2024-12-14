#ifndef GLOBALMOUSEHOOK_H
#define GLOBALMOUSEHOOK_H

#include <QObject>
#include <QPoint>

// 引入 Windows API
#include <windows.h>

class GlobalMouseHook : public QObject {
    Q_OBJECT

public:
    static GlobalMouseHook* GetGlobalMouseHook();
    
    ~GlobalMouseHook();

    bool installHook();   // 安装全局鼠标钩子
    void uninstallHook(); // 卸载钩子

signals:
    void mouseMoved(const QPoint& pos); // 鼠标移动信号
    void mouseButtonPressed(QPoint pos,Qt::MouseButton button); // 鼠标按下
    void mouseButtonReleased(QPoint pos,Qt::MouseButton button); // 鼠标松开
    void mouseButtonDCliecked(QPoint pos,Qt::MouseButton button); // 鼠标双击
private:
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK s_mouseHook;  // 鼠标钩子句柄
    static GlobalMouseHook* s_instance; // 用于调用 Qt 信号的实例
    GlobalMouseHook(QObject* parent = nullptr);
    DWORD lastClickTime = 0; // 上一次鼠标按下时间
    QPoint lastClickPos;     // 上一次鼠标按下位置

};

#endif // GLOBALMOUSEHOOK_H
