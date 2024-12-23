#include "ui_LiveQosLineChart.h"
#include "LiveQosLineChart.h"
#include "chartview.h"
#include <QDateTime>
#include <QLegendMarker>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QChartView>
#include <QSplineSeries>
#include <QChart>
#include "ChartHandleManger.h"

ChartHandleManger *g_chartHandles = nullptr;

CLiveQosLineChart::CLiveQosLineChart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CLiveQosLineChart)
{
    ui->setupUi(this);
    setWindowTitle(tr("Network quality inspection"));
    resize(800, 620);
    this->setMaximumSize(1920, 1080);
    if (nullptr != g_chartHandles)
    {
        delete g_chartHandles;
        g_chartHandles = nullptr;
    }
    g_chartHandles = new ChartHandleManger(this);
    InitLossPacketLineSeries();
    InitRttLineSeries();
}

CLiveQosLineChart::~CLiveQosLineChart() {}

void CLiveQosLineChart::SetLossPacketData(QList<QPointF> audioSendLossPacket, QList<QPointF> audioReceLossPacket, QList<QPointF> videoSendLossPacket,
                                          QList<QPointF> videoReceLossPacket)
{
    if (!g_chartHandles)
    {
        return;
    }

    if (audioSendLossPacket.count() > 0)
    {
        g_chartHandles->m_pAxisX->setRange(QDateTime::fromMSecsSinceEpoch(audioSendLossPacket[0].x()),
                                           QDateTime::fromMSecsSinceEpoch(audioSendLossPacket[audioSendLossPacket.count() - 1].x()));
        g_chartHandles->m_pAudioSendLossPacketDseries->replace(audioSendLossPacket);
    }

    if (audioReceLossPacket.count() > 0)
    {
        g_chartHandles->m_pAudioReceLossPacketDseries->replace(audioReceLossPacket);
        g_chartHandles->m_pAudioReceLossPacketDseries->show();
    }
    else
    {
        g_chartHandles->m_pAudioReceLossPacketDseries->hide();
    }

    if (videoSendLossPacket.count() > 0)
    {
        g_chartHandles->m_pVideoSendLossPacketDseries->show();
        g_chartHandles->m_pVideoSendLossPacketDseries->replace(videoSendLossPacket);
    }
    else
    {
        g_chartHandles->m_pVideoSendLossPacketDseries->hide();
    }

    if (videoReceLossPacket.count() > 0)
    {
        g_chartHandles->m_pVideoReceLossPacketDseries->show();
        g_chartHandles->m_pVideoReceLossPacketDseries->replace(videoReceLossPacket);
    }
    else
    {
        g_chartHandles->m_pVideoReceLossPacketDseries->hide();
    }
}

void CLiveQosLineChart::SetRealTimeLossPacketData(QPointF audioSendLossPacket, QPointF audioReceLossPacket, QPointF videoSendLossPacket,
                                                  QPointF videoReceLossPacket)
{
    if (!g_chartHandles)
    {
        return;
    }
    if (audioSendLossPacket.x() == 0)
    {
        return;
    }

    g_chartHandles->m_pAudioSendLossPacketDseries->append(audioSendLossPacket);
    if (audioReceLossPacket.x() > 0)
    {
        g_chartHandles->m_pAudioReceLossPacketDseries->append(audioReceLossPacket);
    }

    if (videoSendLossPacket.x() > 0)
    {
        g_chartHandles->m_pVideoSendLossPacketDseries->append(videoSendLossPacket);
    }

    if (videoReceLossPacket.x() > 0)
    {
        g_chartHandles->m_pVideoReceLossPacketDseries->append(videoReceLossPacket);
    }

    g_chartHandles->m_pAxisX->setMax(QDateTime::fromMSecsSinceEpoch(audioSendLossPacket.x()));
}

void CLiveQosLineChart::SetRttData(QList<QPointF> audioRtt, QList<QPointF> videoRtt)
{
    if (!g_chartHandles)
    {
        return;
    }
    if (audioRtt.count() > 0)
    {
        g_chartHandles->m_pRttAxisX->setRange(QDateTime::fromMSecsSinceEpoch(audioRtt[0].x()),
                                              QDateTime::fromMSecsSinceEpoch(audioRtt[audioRtt.count() - 1].x()));
        g_chartHandles->m_pAudioRttDseries->replace(audioRtt);
    }

    if (videoRtt.count() > 0)
    {
        g_chartHandles->m_pVideoRttDseries->show();
        g_chartHandles->m_pVideoRttDseries->replace(videoRtt);
    }
    else
    {
        g_chartHandles->m_pVideoRttDseries->hide();
    }
}

void CLiveQosLineChart::SetRealTimeRttData(QPointF audioRtt, QPointF videoRtt, int nMaxRtt)
{
    if (!g_chartHandles)
    {
        return;
    }
    g_chartHandles->m_pAudioRttDseries->append(audioRtt);
    if (videoRtt.x() > 0)
    {
        g_chartHandles->m_pVideoRttDseries->append(videoRtt);
    }
    g_chartHandles->m_pRttAxisX->setMax(QDateTime::fromMSecsSinceEpoch(audioRtt.x()));
    SetMaxRtt(nMaxRtt);
}

void CLiveQosLineChart::ClearLossPacketData(bool isAudioLive)
{
    if (!g_chartHandles)
    {
        return;
    }
    g_chartHandles->m_pAudioSendLossPacketDseries->clear();
    g_chartHandles->m_pVideoSendLossPacketDseries->clear();
    g_chartHandles->m_pAudioReceLossPacketDseries->clear();
    g_chartHandles->m_pVideoReceLossPacketDseries->clear();

    g_chartHandles->m_pAudioRttDseries->clear();
    g_chartHandles->m_pVideoRttDseries->clear();

    QDateTime xMin, xMax;
    xMin = QDateTime::currentDateTime();
    xMax = xMin.addSecs(1800);
    g_chartHandles->m_pAxisX->setRange(xMin, xMax);
    g_chartHandles->m_pRttAxisX->setRange(xMin, xMax);

    if (isAudioLive)
    {
        ui->label_4->setText(tr("A delay of more than 60ms will cause a high delay of live sound"));
        ui->label_2->setText(tr("If the packet loss rate is higher than 10%, the live broadcast sound will be stuck"));
    }
    else
    {
        ui->label_4->setText(tr("Delay of more than 60ms will lead to problems such as live sound and screen delay becoming higher and stalling"));
        ui->label_2->setText(tr("If the packet loss rate is higher than 10%, the broadcast sound and picture will be stuck"));
    }
}

void CLiveQosLineChart::SetMaxRtt(int ms)
{
    if (!g_chartHandles)
    {
        return;
    }
    if (ms > g_chartHandles->m_pRttAxisY->max())
    {
        g_chartHandles->m_pRttAxisY->setRange(0, ms);
    }
}

void CLiveQosLineChart::SetRttLineSeriesHidden(bool isHidden)
{
    ui->RttChartView->setVisible(!isHidden);
    ui->label_3->setVisible(!isHidden);
    ui->label_4->setVisible(!isHidden);
}

void CLiveQosLineChart::InitLossPacketLineSeries()
{
    if (!g_chartHandles)
    {
        return;
    }

    // 设置X轴坐标
    QFont font;
    font.setPointSize(7);
    g_chartHandles->m_pAxisX = new QDateTimeAxis;
    g_chartHandles->m_pAxisX->setTitleVisible(false);
    g_chartHandles->m_pAxisX->setTickCount(11);
    g_chartHandles->m_pAxisX->setFormat("h:mm:ss");
    g_chartHandles->m_pAxisX->setLabelsFont(font);
    // 设置时间坐标轴默认范围
    QDateTime xMin, xMax;
    xMin = QDateTime::currentDateTime();
    xMax = xMin.addSecs(1800);
    g_chartHandles->m_pAxisX->setRange(xMin, xMax);
    // 设置Y坐标轴
    g_chartHandles->m_pAxisY = new QValueAxis;
    // m_pAxisY->setTitleText(tr("Packet Loss Rate (danwei:%)"));
    g_chartHandles->m_pAxisY->setRange(0, 100);
    g_chartHandles->m_pAxisY->setTickCount(6);
    g_chartHandles->m_pAxisY->setMinorTickCount(1);
    g_chartHandles->m_pAxisY->setLabelFormat("%d");
    g_chartHandles->m_pAxisY->setLabelsFont(font);
    // 初始化音频丢包率折线图
    QPen dpen;
    dpen.setWidth(2);
    dpen.setColor(QColor(255, 131, 131));
    g_chartHandles->m_pAudioSendLossPacketDseries = new QSplineSeries;
    g_chartHandles->m_pAudioSendLossPacketDseries->setUseOpenGL(true);
    g_chartHandles->m_pAudioSendLossPacketDseries->setName(tr("Send audio packet loss rate"));
    g_chartHandles->m_pAudioSendLossPacketDseries->setPen(dpen);

    dpen.setColor(QColor(85, 214, 196));
    g_chartHandles->m_pAudioReceLossPacketDseries = new QSplineSeries;
    g_chartHandles->m_pAudioReceLossPacketDseries->setUseOpenGL(true);
    g_chartHandles->m_pAudioReceLossPacketDseries->setName(tr("Receive audio packet loss rate"));
    g_chartHandles->m_pAudioReceLossPacketDseries->setPen(dpen);
    // 初始化视频丢包率折线图
    dpen.setColor(QColor(154, 196, 250));
    g_chartHandles->m_pVideoSendLossPacketDseries = new QSplineSeries;
    g_chartHandles->m_pVideoSendLossPacketDseries->setUseOpenGL(true);
    g_chartHandles->m_pVideoSendLossPacketDseries->setName(tr("Send video packet loss rate"));
    g_chartHandles->m_pVideoSendLossPacketDseries->setPen(dpen);

    dpen.setColor(QColor(168, 139, 109));
    g_chartHandles->m_pVideoReceLossPacketDseries = new QSplineSeries;
    g_chartHandles->m_pVideoReceLossPacketDseries->setUseOpenGL(true);
    g_chartHandles->m_pVideoReceLossPacketDseries->setName(tr("Receive video packet loss rate"));
    g_chartHandles->m_pVideoReceLossPacketDseries->setPen(dpen);

    g_chartHandles->m_pChart = new QChart;
    g_chartHandles->m_pChart->addSeries(g_chartHandles->m_pAudioSendLossPacketDseries);
    g_chartHandles->m_pChart->addSeries(g_chartHandles->m_pAudioReceLossPacketDseries);
    g_chartHandles->m_pChart->addSeries(g_chartHandles->m_pVideoSendLossPacketDseries);
    g_chartHandles->m_pChart->addSeries(g_chartHandles->m_pVideoReceLossPacketDseries);

    g_chartHandles->m_pChart->setAxisX(g_chartHandles->m_pAxisX, g_chartHandles->m_pAudioSendLossPacketDseries);
    g_chartHandles->m_pChart->setAxisY(g_chartHandles->m_pAxisY, g_chartHandles->m_pAudioSendLossPacketDseries);

    g_chartHandles->m_pChart->setAxisX(g_chartHandles->m_pAxisX, g_chartHandles->m_pVideoSendLossPacketDseries);
    g_chartHandles->m_pChart->setAxisY(g_chartHandles->m_pAxisY, g_chartHandles->m_pVideoSendLossPacketDseries);

    g_chartHandles->m_pChart->setAxisX(g_chartHandles->m_pAxisX, g_chartHandles->m_pAudioReceLossPacketDseries);
    g_chartHandles->m_pChart->setAxisY(g_chartHandles->m_pAxisY, g_chartHandles->m_pAudioReceLossPacketDseries);

    g_chartHandles->m_pChart->setAxisX(g_chartHandles->m_pAxisX, g_chartHandles->m_pVideoReceLossPacketDseries);
    g_chartHandles->m_pChart->setAxisY(g_chartHandles->m_pAxisY, g_chartHandles->m_pVideoReceLossPacketDseries);

    g_chartHandles->m_pChart->legend()->setAlignment(Qt::AlignBottom);
    QFont font1;
    font1.setPointSize(9);
    g_chartHandles->m_pChart->legend()->setFont(font1);
    ui->LossChartView->setChart(g_chartHandles->m_pChart);
    ui->LossChartView->setChartType(LossPacketType);
    ui->LossChartView->setRenderHint(QPainter::Antialiasing);
    QList<QLegendMarker *> markers = g_chartHandles->m_pChart->legend()->markers();
    foreach (QLegendMarker *marker, markers)
    {
        QPen pen;
        pen.setBrush(Qt::transparent);
        marker->setPen(pen);  // 设置标记的边框颜色为透明
    }
}

void CLiveQosLineChart::InitRttLineSeries()
{
    if (!g_chartHandles)
    {
        return;
    }
    // 设置X轴坐标
    QFont font;
    font.setPointSize(7);
    g_chartHandles->m_pRttAxisX = new QDateTimeAxis;
    g_chartHandles->m_pRttAxisX->setTitleVisible(false);
    g_chartHandles->m_pRttAxisX->setTickCount(11);
    g_chartHandles->m_pRttAxisX->setFormat("h:mm:ss");
    g_chartHandles->m_pRttAxisX->setLabelsFont(font);
    // 设置时间坐标轴默认范围
    QDateTime xMin, xMax;
    xMin = QDateTime::currentDateTime();
    xMax = xMin.addSecs(1800);
    g_chartHandles->m_pRttAxisX->setRange(xMin, xMax);
    // 设置Y坐标轴
    g_chartHandles->m_pRttAxisY = new QValueAxis;
    // m_pRttAxisY->setTitleText(tr("Network delay (danwei:ms)"));
    g_chartHandles->m_pRttAxisY->setRange(0, 100);
    g_chartHandles->m_pRttAxisY->setMinorTickCount(1);
    g_chartHandles->m_pRttAxisY->setTickCount(6);
    g_chartHandles->m_pRttAxisY->setLabelFormat("%d");
    g_chartHandles->m_pRttAxisY->setLabelsFont(font);

    QPen dpen;
    dpen.setWidth(2);
    dpen.setColor(QColor(255, 131, 131));
    g_chartHandles->m_pAudioRttDseries = new QSplineSeries;
    g_chartHandles->m_pAudioRttDseries->setUseOpenGL(true);
    g_chartHandles->m_pAudioRttDseries->setName(tr("Audio Network delay"));
    g_chartHandles->m_pAudioRttDseries->setPen(dpen);

    dpen.setColor(QColor(154, 196, 250));
    g_chartHandles->m_pVideoRttDseries = new QSplineSeries;
    g_chartHandles->m_pVideoRttDseries->setUseOpenGL(true);
    g_chartHandles->m_pVideoRttDseries->setName(tr("Video Network delay"));
    g_chartHandles->m_pVideoRttDseries->setPen(dpen);

    g_chartHandles->m_pRttChart = new QChart;
    g_chartHandles->m_pRttChart->addSeries(g_chartHandles->m_pAudioRttDseries);
    g_chartHandles->m_pRttChart->addSeries(g_chartHandles->m_pVideoRttDseries);

    g_chartHandles->m_pRttChart->setAxisX(g_chartHandles->m_pRttAxisX, g_chartHandles->m_pAudioRttDseries);
    g_chartHandles->m_pRttChart->setAxisY(g_chartHandles->m_pRttAxisY, g_chartHandles->m_pAudioRttDseries);

    g_chartHandles->m_pRttChart->setAxisX(g_chartHandles->m_pRttAxisX, g_chartHandles->m_pVideoRttDseries);
    g_chartHandles->m_pRttChart->setAxisY(g_chartHandles->m_pRttAxisY, g_chartHandles->m_pVideoRttDseries);

    g_chartHandles->m_pRttChart->legend()->setAlignment(Qt::AlignBottom);
    QFont font1;
    font1.setPointSize(9);
    g_chartHandles->m_pRttChart->legend()->setFont(font1);

    ui->RttChartView->setChart(g_chartHandles->m_pRttChart);
    ui->RttChartView->setChartType(RttType);
    ui->RttChartView->setRenderHint(QPainter::Antialiasing);
    QList<QLegendMarker *> markers = g_chartHandles->m_pRttChart->legend()->markers();
    foreach (QLegendMarker *marker, markers)
    {
        QPen pen;
        pen.setBrush(Qt::transparent);
        marker->setPen(pen);  // 设置标记的边框颜色为透明
    }
}

void CLiveQosLineChart::SelLocal(const QLocale &local)
{
    ChartView::SetLocale(local);
}