#include "chartview.h"
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QSplineSeries>
#include <QLocale>

QLocale ChartView::locale_ = QLocale();
ChartView::ChartView(QWidget *parent)
    : QChartView{parent}
{
    // 创建线
    x_line = new QGraphicsLineItem();
    // 设置颜色
    x_line->setPen(QPen(QColor(100, 100, 100)));
    x_line->setZValue(2);
    y_line = new QGraphicsLineItem();
    y_line->setPen(QPen(QColor(100, 100, 100)));
    y_line->setZValue(2);
    // 添加到scene中。
    this->scene()->addItem(x_line);
    this->scene()->addItem(y_line);

    m_txtPos = new QGraphicsTextItem();
    QFont font;
    font.setPointSize(8);
    m_txtPos->setFont(font);
    m_txtPos->setZValue(2);
    this->scene()->addItem(m_txtPos);
}

ChartView::~ChartView() {}

void ChartView::setChartType(ChartType type)
{
    m_type = type;
}

void ChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_lastPoint = event->pos();
        m_isPress   = true;
    }
}

void ChartView::SetLocale(const QLocale &locale)
{
    locale_ = locale;
}
void ChartView::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint curPos = event->pos();
    // 获取chart范围
    QRectF chartRange = this->chart()->plotArea();
    if ((curPos.x() > chartRange.left() && curPos.x() < chartRange.right()) && (curPos.y() > chartRange.top() && curPos.y() < chartRange.bottom()))
    {
        this->setCursor(Qt::CrossCursor);
        QPointF curVal   = this->chart()->mapToValue(QPointF(curPos));
        QLocale locale   = locale_;
        QString strX     = locale.toString(QDateTime::fromMSecsSinceEpoch(curVal.x()), "h:mm:ss");
        QString coordStr = "";
        if (m_type == LossPacketType)
        {
            // coordStr = QString(tr("time:%1, LossPacket:%2")).arg(strX).arg(curVal.y());
            coordStr = QString(tr("time:%1")).arg(strX);
        }
        else if (m_type == RttType)
        {
            // coordStr = QString(tr("time:%1, Rtt:%2")).arg(strX).arg(curVal.y());
            coordStr = QString(tr("time:%1")).arg(strX);
        }

        m_txtPos->setPlainText(coordStr);
        m_txtPos->setPos(curPos.x(), curPos.y());

        x_line->setLine(event->x(), chartRange.top(), event->x(), chartRange.bottom());
        y_line->setLine(chartRange.left(), event->y(), chartRange.right(), event->y());

        m_txtPos->show();
        x_line->show();
        y_line->show();

        if (m_isPress)
        {
            QPoint offset = curPos - m_lastPoint;
            m_lastPoint   = curPos;
            this->chart()->scroll(-offset.x(), 0);
        }
    }
    else
    {
        this->setCursor(Qt::ArrowCursor);
        m_txtPos->hide();
        x_line->hide();
        y_line->hide();
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPress = false;
    if (event->button() == Qt::RightButton)
    {
        // if (m_alreadySaveRange)
        //{
        //    this->chart()->axisX()->setRange(m_xMin, m_xMax);
        //    this->chart()->axisY()->setRange(m_yMin, m_yMax);
        //}
    }
}

void ChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QList<QAbstractSeries *> listSeries = this->chart()->series();

    for (QAbstractSeries *value : listSeries)
    {
        QSplineSeries *series = dynamic_cast<QSplineSeries *>(value);
        if (series != nullptr && series->count() > 1)
        {
            QPointF latestPoint = series->at(series->count() - 1);
            QPointF firstPoint  = series->at(0);
            this->chart()->axisX(series)->setRange(QDateTime::fromMSecsSinceEpoch(firstPoint.x()), QDateTime::fromMSecsSinceEpoch(latestPoint.x()));

            break;
        }
    }
}

void ChartView::wheelEvent(QWheelEvent *event)
{
    const QPoint curPos = event->pos();
    QPointF      curVal = this->chart()->mapToValue(QPointF(curPos));
    const double factor = 1.5;  //缩放比例
    if (m_ctrlPress)
    {  // Y轴
        QValueAxis * axisY    = dynamic_cast<QValueAxis *>(this->chart()->axisY());
        const double yMin     = axisY->min();
        const double yMax     = axisY->max();
        const double yCentral = curVal.y();

        double bottomOffset;
        double topOffset;
        if (event->delta() > 0)
        {  //放大
            bottomOffset = 1.0 / factor * (yCentral - yMin);
            topOffset    = 1.0 / factor * (yMax - yCentral);
        }
        else
        {  //缩小
            bottomOffset = 1.0 * factor * (yCentral - yMin);
            topOffset    = 1.0 * factor * (yMax - yCentral);
        }
        this->chart()->axisY()->setRange(yCentral - bottomOffset, yCentral + topOffset);
    }
    else
    {  // X轴
        QDateTimeAxis *axisX = dynamic_cast<QDateTimeAxis *>(this->chart()->axisX());
        if (axisX)
        {
            const double xMin     = axisX->min().toMSecsSinceEpoch();
            const double xMax     = axisX->max().toMSecsSinceEpoch();
            const double xCentral = curVal.x();

            double leftOffset;
            double rightOffset;
            if (event->delta() > 0)
            {  //放大
                leftOffset  = 1.0 / factor * (xCentral - xMin);
                rightOffset = 1.0 / factor * (xMax - xCentral);
            }
            else
            {  //缩小
                leftOffset  = 1.0 * factor * (xCentral - xMin);
                rightOffset = 1.0 * factor * (xMax - xCentral);
            }
            this->chart()->axisX()->setRange(QDateTime::fromMSecsSinceEpoch(xCentral - leftOffset), QDateTime::fromMSecsSinceEpoch(xCentral + rightOffset));
        }
    }
}

void ChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Control:
        // m_ctrlPress = true;
        break;
    }
}

void ChartView::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Control:
        // m_ctrlPress = false;
        break;
    }
}
