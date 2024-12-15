#include "DonationWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

DonationWindow::DonationWindow(QWidget* parent)
    : QWidget(parent), imageStack(new QStackedWidget), wechatLabel(new QLabel), alipayLabel(new QLabel) {
    setupUI();
}

void DonationWindow::setupUI() {
    setWindowIcon(QIcon(":/Spotlight/res/donation.png"));
    setWindowTitle(QString::fromLocal8Bit("֧������"));
    setFixedSize(450, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ��л��
    QLabel* thankYouLabel = new QLabel(QString::fromLocal8Bit("��л��֧�����ǵĿ�Դ���ߣ�����֧�������Ǽ���ǰ���Ķ�����"));
    thankYouLabel->setWordWrap(true);
    thankYouLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(thankYouLabel);

    // ��ť����
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* wechatButton = new QPushButton(QString::fromLocal8Bit("΢��"));
    QPushButton* alipayButton = new QPushButton(QString::fromLocal8Bit("֧����"));
    buttonLayout->addWidget(wechatButton);
    buttonLayout->addWidget(alipayButton);
    mainLayout->addLayout(buttonLayout);

    // ��ά��չʾ
    wechatLabel->setPixmap(QPixmap(":/Spotlight/res/wechat_qr.jpg").scaled(300, 300, Qt::KeepAspectRatio));
    alipayLabel->setPixmap(QPixmap(":/Spotlight/res/alipay_qr.jpg").scaled(300, 300, Qt::KeepAspectRatio));

    wechatLabel->setAlignment(Qt::AlignCenter);
    alipayLabel->setAlignment(Qt::AlignCenter);

    imageStack->addWidget(wechatLabel);
    imageStack->addWidget(alipayLabel);
    mainLayout->addWidget(imageStack);

    // �رհ�ť
    QPushButton* closeButton = new QPushButton(QString::fromLocal8Bit("�ر�"));
    mainLayout->addWidget(closeButton);
    mainLayout->setAlignment(closeButton, Qt::AlignCenter);
    connect(wechatButton, &QPushButton::clicked, [this]() { imageStack->setCurrentIndex(0); });
    connect(alipayButton, &QPushButton::clicked, [this]() { imageStack->setCurrentIndex(1); });
    // ���رհ�ť���ź����ӵ����ڹرչ���
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}
