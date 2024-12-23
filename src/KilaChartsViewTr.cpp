#include "KilaChartsViewTr.h"
#include <QTranslator>
#include <QCoreApplication>

CKilaChartsViewTr::CKilaChartsViewTr(QObject* parent)
    : QObject(parent)
{
}

CKilaChartsViewTr::~CKilaChartsViewTr() {}

void CKilaChartsViewTr::LoadBaseUiQmFile(bool kilaApp)
{
    QTranslator* trans = new QTranslator;

    if (kilaApp)
    {
        trans->load(":/translators/KilaChartsView_zh.qm");
    }
    else
    {
        trans->load(":/translators/KilaChartsView_ja.qm");
    }

    qApp->installTranslator(trans);
}
