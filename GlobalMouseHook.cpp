#include "GlobalMouseHook.h"
#include <QDebug>
// 静态变量初始化
HHOOK GlobalMouseHook::s_mouseHook = nullptr;
GlobalMouseHook* GlobalMouseHook::s_instance = nullptr;

GlobalMouseHook* GlobalMouseHook::GetGlobalMouseHook()
{
    if (s_instance == nullptr)
    {
        s_instance = new GlobalMouseHook;
    }
    return s_instance;
}
GlobalMouseHook::GlobalMouseHook(QObject* parent) : QObject(parent) {
}

GlobalMouseHook::~GlobalMouseHook() {
    uninstallHook(); // 确保钩子卸载
}

bool GlobalMouseHook::installHook() {
    if (s_mouseHook) {
        return false; // 如果已安装钩子，则不重复安装
    }
    s_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0); // 全局低级鼠标钩子
    return s_mouseHook != nullptr;
}

void GlobalMouseHook::uninstallHook() {
    if (s_mouseHook) {
        UnhookWindowsHookEx(s_mouseHook);
        s_mouseHook = nullptr;
    }
}

// 默认情况下，应用程序定义的窗口不会接收双击消息。
// 由于生成双击消息所涉及的系统开销，因此这些消息仅针对属于具有 CS_DBLCLKS 类样式的类的窗口生成。
// 应用程序在注册窗口类时必须设置此样式。 有关详细信息，请参阅窗口类。
// from https://learn.microsoft.com/zh-cn/windows/win32/inputdev/about-mouse-input
LRESULT CALLBACK GlobalMouseHook::MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && s_instance) {
        auto* mouseStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        DWORD currentTime = GetTickCount64();
        QPoint currentPos(mouseStruct->pt.x, mouseStruct->pt.y);

        switch (wParam) {
        case WM_MOUSEMOVE:
            s_instance->mouseMoved(currentPos);
            break;

        case WM_MBUTTONDOWN:
            if ((currentTime - s_instance->lastClickTime <= GetDoubleClickTime()) &&
                (currentPos == s_instance->lastClickPos)) {
                // 双击检测
                s_instance->mouseButtonDCliecked(currentPos, Qt::MiddleButton);
            }
            else {
                // 更新单击状态
                s_instance->lastClickTime = currentTime;
                s_instance->lastClickPos = currentPos;

                // 发送单击信号
                s_instance->mouseButtonPressed(currentPos, Qt::MiddleButton);
            }
            break;

        case WM_RBUTTONDOWN:
            if ((currentTime - s_instance->lastClickTime <= GetDoubleClickTime()) &&
                (currentPos == s_instance->lastClickPos)) {
                // 双击检测
                s_instance->mouseButtonDCliecked(currentPos, Qt::RightButton);
            }
            else {
                // 更新单击状态
                s_instance->lastClickTime = currentTime;
                s_instance->lastClickPos = currentPos;
                // 发送单击信号
                s_instance->mouseButtonPressed(currentPos, Qt::RightButton);
            }
            break;

        case WM_LBUTTONUP:
            s_instance->mouseButtonReleased(currentPos, Qt::LeftButton);
            break;

        case WM_RBUTTONUP:
            s_instance->mouseButtonReleased(currentPos, Qt::RightButton);
            break;

        case WM_LBUTTONDOWN:
            s_instance->mouseButtonPressed(currentPos, Qt::LeftButton);
            break;

        case WM_MBUTTONUP:
            s_instance->mouseButtonReleased(currentPos, Qt::MiddleButton);
            break;
        }
    }
    return CallNextHookEx(s_mouseHook, nCode, wParam, lParam); // 传递事件
}