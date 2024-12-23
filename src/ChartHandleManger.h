#pragma once

#include <QObject>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QChartView>
#include <QSplineSeries>
#include <QChart>
QT_CHARTS_USE_NAMESPACE
class ChartHandleManger : public QObject
{
    Q_OBJECT

public:
    ChartHandleManger(QObject* parent);
    ~ChartHandleManger();

public:
    QDateTimeAxis* m_pAxisX                      = nullptr;
    QValueAxis*    m_pAxisY                      = nullptr;
    QSplineSeries* m_pAudioSendLossPacketDseries = nullptr;
    QSplineSeries* m_pVideoSendLossPacketDseries = nullptr;
    QSplineSeries* m_pAudioReceLossPacketDseries = nullptr;
    QSplineSeries* m_pVideoReceLossPacketDseries = nullptr;
    QChart*        m_pChart                      = nullptr;

    QDateTimeAxis* m_pRttAxisX        = nullptr;
    QValueAxis*    m_pRttAxisY        = nullptr;
    QSplineSeries* m_pAudioRttDseries = nullptr;
    QSplineSeries* m_pVideoRttDseries = nullptr;
    QChart*        m_pRttChart        = nullptr;
};
