#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <mutex>
#include <vector>

//��Ϣ����
enum NotifyType {
    Notify_Type_None = 0, // ��ͨ
    Notify_Type_Success, // �ɹ�
    Notify_Type_Error, // ����
    Notify_Type_Warning, // ����
    Notify_Type_Information // ��Ϣ
};

//��Ϣλ��
enum NotifyPosition {
    Pos_Top_Right = 0, // ���Ͻǿ�ʼ���ѣ���������
    Pos_Top_Left, // ���Ͻǿ�ʼ���ѣ���������
    Pos_Bottom_Left, // ���½ǿ�ʼ���ѣ���������
    Pos_Bottom_Right // ���½ǿ�ʼ���ѣ���������
};

class NotificationLabel;
class NotificationItem;
class Notification : public QObject
{
    Q_OBJECT
public:
    explicit Notification(QObject* parent = nullptr);
    ~Notification() override;

    /**
     * @brief Push ��ʾ��ʾ��
     * @param type ��ʾ����
     * @param pos ��ʾ�ķ���(���ϡ����¡����ϡ�����)
     * @param title ��ʾ�ı���
     * @param content ��ʾ������
     * @param nLive ��ʾ��Ĵ��ʱ�䣬Ĭ��3000ms������ֵ����0����ʾ����ʧ��
     */
    void Push(NotifyType type, NotifyPosition pos, QString title, QString content, int nLive = 3000);
    void Push(QString content, NotifyType type = Notify_Type_None, NotifyPosition pos = NotifyPosition::Pos_Bottom_Right,int nLive = 1000);

private:
    const int nMargin = 15;
    QSize m_size;
    std::mutex m_vecMtx;
    std::vector<NotificationItem*> m_vecItem;

private slots:
    void itemRemoved(NotificationItem* pItem);
};

class NotificationItem : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationItem(QWidget* parent,
        NotifyType type,
        NotifyPosition pos,
        QString title ,
        QString content,
        int nLife = 3000);
    explicit NotificationItem(QWidget* parent,
        QString content = QString("this is a message"),
        NotifyType type = Notify_Type_None,
        NotifyPosition pos = Pos_Top_Right,
        int nLife = 3000);
    ~NotificationItem() override;
    void Show();
    NotifyPosition GetPosType() const;
    bool IsAppearEnd() const;

private:
    void Appear();
    void Disappear();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const int nFixedWidth = 300;
    const int nMinHeight = 30;
    const int nTopPadding = 5;
    const int nLeftPadding = 20;
    const int nRightPadding = 20;
    const int nMargin = 20;
    NotifyPosition m_enPos;
    QTimer m_liftTimer;
    int m_nLifeTime;
    bool m_bAppearEnd;

signals:
    void itemRemoved(NotificationItem*);
};

class NotificationLabel : public QWidget
{
    Q_OBJECT
public:
    explicit NotificationLabel(QWidget* parent = nullptr, int nFixedWidth = 300, QString content = "");
    ~NotificationLabel() override;
    void Adjust();

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QStringList m_strList;
    QString m_strText;
    int m_nHeight;
    int m_nMargin = 5; // ���¼��
    QColor m_conetentColor = QColor(0x606266); // ���ݵ�������ɫ
};

#endif // NOTIFICATION_H