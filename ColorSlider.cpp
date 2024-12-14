#include "ColorSlider.h"

ColorSlider::ColorSlider(QWidget* parent) : QSlider(Qt::Horizontal, parent) {
	setMinimum(0);
	setMaximum(16777215);

	// 设置 QSS 样式
	setStyleSheet(R"(
        QSlider::groove:horizontal {
    border: 0px solid #999999;
    height: 12px;
    border-radius: 6px;
    background: qlineargradient(
        spread:pad, x1:0, y1:0, x2:1, y2:0,
        stop:0 #000000, stop:0.2 #555555, stop:0.4 #AAAAAA,
        stop:0.6 #DDDDDD, stop:0.8 #BBBBBB, stop:1 #FFFFFF
    );
}
        QSlider::handle:horizontal {
            background: white;
            border: 1px solid #666666;
            width: 16px;
            height: 16px;
            border-radius: 8px;
            margin: -2px 0; /* 将滑块嵌入滑动条中间 */
        }

        QSlider::handle:horizontal:hover {
            background: #DDDDDD;
            border: 1px solid #AAAAAA;
        }
    )");
}
