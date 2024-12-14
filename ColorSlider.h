#ifndef COLORSLIDER_H
#define COLORSLIDER_H

#include <QSlider>
#include <QWidget>

class ColorSlider : public QSlider {
    Q_OBJECT
public:
    explicit ColorSlider(QWidget* parent = nullptr);

protected:
   //// void paintEvent(QPaintEvent* event) override;
};

#endif // COLORSLIDER_H
