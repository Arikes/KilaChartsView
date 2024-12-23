#pragma once
#include "kilachartsview_global.h"
#include <QWidget>
#include <QLocale>
namespace Ui
{
    class CLiveQosLineChart;
}

class KILACHARTSVIEW_EXPORT CLiveQosLineChart : public QWidget
{
    Q_OBJECT

public:
    CLiveQosLineChart(QWidget* parent = nullptr);
    ~CLiveQosLineChart();

public:
    void        SetLossPacketData(QList<QPointF> audioSendLossPacket, QList<QPointF> audioReceLossPacket, QList<QPointF> videoSendLossPacket,
                                  QList<QPointF> videoReceLossPacket);
    void        SetRealTimeLossPacketData(QPointF audioSendLossPacket, QPointF audioReceLossPacket, QPointF videoSendLossPacket, QPointF videoReceLossPacket);
    void        SetRttData(QList<QPointF> audioRtt, QList<QPointF> videoRtt);
    void        SetRealTimeRttData(QPointF audioRtt, QPointF videoRtt, int nMaxRtt);
    void        ClearLossPacketData(bool isAudioLive = true);
    void        SetMaxRtt(int ms);
    void        SetRttLineSeriesHidden(bool isHidden);
    static void SelLocal(const QLocale& local);

private:
    void InitLossPacketLineSeries();
    void InitRttLineSeries();

private:
    Ui::CLiveQosLineChart* ui;
};
