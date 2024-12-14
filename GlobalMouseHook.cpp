#include "GlobalMouseHook.h"
#include <QDebug>
// ��̬������ʼ��
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
    uninstallHook(); // ȷ������ж��
}

bool GlobalMouseHook::installHook() {
    if (s_mouseHook) {
        return false; // ����Ѱ�װ���ӣ����ظ���װ
    }
    s_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, nullptr, 0); // ȫ�ֵͼ���깳��
    return s_mouseHook != nullptr;
}

void GlobalMouseHook::uninstallHook() {
    if (s_mouseHook) {
        UnhookWindowsHookEx(s_mouseHook);
        s_mouseHook = nullptr;
    }
}

// Ĭ������£�Ӧ�ó�����Ĵ��ڲ������˫����Ϣ��
// ��������˫����Ϣ���漰��ϵͳ�����������Щ��Ϣ��������ھ��� CS_DBLCLKS ����ʽ����Ĵ������ɡ�
// Ӧ�ó�����ע�ᴰ����ʱ�������ô���ʽ�� �й���ϸ��Ϣ������Ĵ����ࡣ
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
                // ˫�����
                s_instance->mouseButtonDCliecked(currentPos, Qt::MiddleButton);
            }
            else {
                // ���µ���״̬
                s_instance->lastClickTime = currentTime;
                s_instance->lastClickPos = currentPos;

                // ���͵����ź�
                s_instance->mouseButtonPressed(currentPos, Qt::MiddleButton);
            }
            break;

        case WM_RBUTTONDOWN:
            if ((currentTime - s_instance->lastClickTime <= GetDoubleClickTime()) &&
                (currentPos == s_instance->lastClickPos)) {
                // ˫�����
                s_instance->mouseButtonDCliecked(currentPos, Qt::RightButton);
            }
            else {
                // ���µ���״̬
                s_instance->lastClickTime = currentTime;
                s_instance->lastClickPos = currentPos;
                // ���͵����ź�
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
    return CallNextHookEx(s_mouseHook, nCode, wParam, lParam); // �����¼�
}