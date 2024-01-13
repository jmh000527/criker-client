#pragma once

#include <QMouseEvent>
#include <QWidget>

class ImageView : public QWidget {
    Q_OBJECT

public:
    ImageView(QWidget* parent = nullptr);
    ~ImageView();

    void loadImage(const QString& filePath);
    void loadImage(const QImage& image); //重载

    QImage getCurrentScaledImage() const;
    int getXBlank() const;
    int getYBlank() const;

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QImage m_Image; //当前显示的图像
    qreal m_ZoomValue = 1.0;
    int m_XPtInterval = 0;
    int m_YPtInterval = 0;
    QPoint m_OldPos;
    bool m_Pressed = false;
    QRect m_picRect{};

private slots:
    void onLoadImage(void);
    void onZoomInImage(void);
    void onZoomOutImage(void);
    void onPresetImage(void);
};
