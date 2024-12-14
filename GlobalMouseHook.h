#ifndef GLOBALMOUSEHOOK_H
#define GLOBALMOUSEHOOK_H

#include <QObject>
#include <QPoint>

// ���� Windows API
#include <windows.h>

class GlobalMouseHook : public QObject {
    Q_OBJECT

public:
    static GlobalMouseHook* GetGlobalMouseHook();
    
    ~GlobalMouseHook();

    bool installHook();   // ��װȫ����깳��
    void uninstallHook(); // ж�ع���

signals:
    void mouseMoved(const QPoint& pos); // ����ƶ��ź�
    void mouseButtonPressed(QPoint pos,Qt::MouseButton button); // ��갴��
    void mouseButtonReleased(QPoint pos,Qt::MouseButton button); // ����ɿ�
    void mouseButtonDCliecked(QPoint pos,Qt::MouseButton button); // ���˫��
private:
    static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK s_mouseHook;  // ��깳�Ӿ��
    static GlobalMouseHook* s_instance; // ���ڵ��� Qt �źŵ�ʵ��
    GlobalMouseHook(QObject* parent = nullptr);
    DWORD lastClickTime = 0; // ��һ����갴��ʱ��
    QPoint lastClickPos;     // ��һ����갴��λ��

};

#endif // GLOBALMOUSEHOOK_H
