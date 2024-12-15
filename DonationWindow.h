#ifndef DONATIONWINDOW_H
#define DONATIONWINDOW_H

#include <QWidget>
#include <QStackedWidget>

class QLabel;

class DonationWindow : public QWidget {
    Q_OBJECT

public:
    explicit DonationWindow(QWidget* parent = nullptr);

private:
    QStackedWidget* imageStack; // �л�չʾ��ά������
    QLabel* wechatLabel;
    QLabel* alipayLabel;

    void setupUI();          
};

#endif // DONATIONWINDOW_H
