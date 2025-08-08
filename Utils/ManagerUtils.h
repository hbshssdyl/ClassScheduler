#pragma once

#include "DataUtils.h"
#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <iostream>
#include <io.h>

using namespace std;

namespace ClassScheduler
{
    class MUtils
    {
    public:
        static std::string getCurrentDate();
        static std::string getCurrentWeekFirstDay();
        static std::string getCurrentWeekLastDay();
        static std::string getCurrentMonthFirstDay();
        static std::string getCurrentMonthLastDay();
};

} // namespace ClassScheduler
