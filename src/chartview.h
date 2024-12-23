#ifndef CHARTVIEW_E_H
#define CHARTVIEW_E_H

#include <QChartView>
#include <QMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QDateTime>
#include <QLocale>
QT_CHARTS_USE_NAMESPACE

enum ChartType
{
    LossPacketType = 0,
    RttType
};

class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QWidget *parent = nullptr);
    ~ChartView();
    void        setChartType(ChartType type);
    static void SetLocale(const QLocale &locale);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    QPoint    m_lastPoint;
    bool      m_isPress   = false;
    bool      m_ctrlPress = false;
    ChartType m_type;

    QGraphicsLineItem *x_line   = nullptr;
    QGraphicsLineItem *y_line   = nullptr;
    QGraphicsTextItem *m_txtPos = nullptr;

protected:
    static QLocale locale_;

signals:
};

#endif  // CHARTVIEW_H
