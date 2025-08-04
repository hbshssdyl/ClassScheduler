#pragma once

#include "Utils/DataUtils.h"
#include "Managers/CoreFramework.h"

#include <QObject>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

using namespace std;
using namespace ClassScheduler;

class SearchClassInfoController : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(QVariantMap classInfoMap MEMBER mClassInfoMap NOTIFY classInfoMapChanged)

public:
    explicit SearchClassInfoController(CoreFrameworkPtr coreFramework, QObject* parent = nullptr);
    void refreshSearchClassInfo();

signals:
    void classInfoMapChanged();

public slots:
    void onSearchTriggered(QString searchString);

private:
    void initialize();
    void updateClassInfosList(ClassInfos& infos);

private:
    QVariantMap mClassInfoMap;
    ClassInfos mClassInfosFromDB;
    std::weak_ptr<CoreFramework> mCoreFramework;

};

