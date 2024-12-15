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
    setWindowTitle(QString::fromLocal8Bit("支持我们"));
    setFixedSize(450, 450);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 感谢语
    QLabel* thankYouLabel = new QLabel(QString::fromLocal8Bit("感谢您支持我们的开源工具！您的支持是我们继续前进的动力！"));
    thankYouLabel->setWordWrap(true);
    thankYouLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(thankYouLabel);

    // 按钮布局
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* wechatButton = new QPushButton(QString::fromLocal8Bit("微信"));
    QPushButton* alipayButton = new QPushButton(QString::fromLocal8Bit("支付宝"));
    buttonLayout->addWidget(wechatButton);
    buttonLayout->addWidget(alipayButton);
    mainLayout->addLayout(buttonLayout);

    // 二维码展示
    wechatLabel->setPixmap(QPixmap(":/Spotlight/res/wechat_qr.jpg").scaled(300, 300, Qt::KeepAspectRatio));
    alipayLabel->setPixmap(QPixmap(":/Spotlight/res/alipay_qr.jpg").scaled(300, 300, Qt::KeepAspectRatio));

    wechatLabel->setAlignment(Qt::AlignCenter);
    alipayLabel->setAlignment(Qt::AlignCenter);

    imageStack->addWidget(wechatLabel);
    imageStack->addWidget(alipayLabel);
    mainLayout->addWidget(imageStack);

    // 关闭按钮
    QPushButton* closeButton = new QPushButton(QString::fromLocal8Bit("关闭"));
    mainLayout->addWidget(closeButton);
    mainLayout->setAlignment(closeButton, Qt::AlignCenter);
    connect(wechatButton, &QPushButton::clicked, [this]() { imageStack->setCurrentIndex(0); });
    connect(alipayButton, &QPushButton::clicked, [this]() { imageStack->setCurrentIndex(1); });
    // 将关闭按钮的信号连接到窗口关闭功能
    connect(closeButton, &QPushButton::clicked, this, &QWidget::close);
}
