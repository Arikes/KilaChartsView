#include "ChartHandleManger.h"

ChartHandleManger::ChartHandleManger(QObject* parent)
    : QObject(parent)
{
}

ChartHandleManger::~ChartHandleManger()
{
    m_pAxisX                      = nullptr;
    m_pAxisY                      = nullptr;
    m_pAudioSendLossPacketDseries = nullptr;
    m_pVideoSendLossPacketDseries = nullptr;
    m_pAudioReceLossPacketDseries = nullptr;
    m_pVideoReceLossPacketDseries = nullptr;
    m_pChart                      = nullptr;

    m_pRttAxisX        = nullptr;
    m_pRttAxisY        = nullptr;
    m_pAudioRttDseries = nullptr;
    m_pVideoRttDseries = nullptr;
    m_pRttChart        = nullptr;
}
