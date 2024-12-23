#pragma once

#include <QObject>
#include "kilachartsview_global.h"

class KILACHARTSVIEW_EXPORT CKilaChartsViewTr : public QObject
{
    Q_OBJECT

public:
    CKilaChartsViewTr(QObject *parent);
    ~CKilaChartsViewTr();

    static void LoadBaseUiQmFile(bool kilaApp = true);
};
