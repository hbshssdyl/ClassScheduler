#pragma once

#include <QObject>
#include <iostream>
#include <cstdio>
#include <vector>
#include <QString>
#include <QVariant>

#include "Utils/DataUtils.h"

using namespace std;
using namespace PictureManager;

class Controller : public QObject {
    Q_OBJECT
public:
    Q_PROPERTY(OperateMode operateMode MEMBER mOperateMode NOTIFY operateModeChanged)
    Q_PROPERTY(QString loadedView MEMBER mLoadedView NOTIFY operateModeChanged)
    Q_PROPERTY(QVariantList actionItemsList MEMBER mActionItemsList NOTIFY actionItemsListChanged)
    Q_PROPERTY(QVariantList groupRepeatedImages MEMBER mGroupRepeatedImages NOTIFY repeatedImagesChanged)

    enum class OperateMode
    {
        None,
        WelcomePage,
        Deduplication,
        Enhancement,
        Compression
    };
    Q_ENUM(OperateMode)
    using OperateModes = std::vector<OperateMode>;

public:
    explicit Controller(QObject* parent = nullptr);
    void initialize();

public slots:
    void onOperateModeSelected(OperateMode mode);
    void onForderPathReceived(QString dirPath);
    void onDeleteRepeatedImagesAction();

signals:
    void operateModeChanged();
    void actionItemsListChanged();
    void repeatedImagesChanged();

private:
    std::string toOperateModeString(OperateMode mode);
    void refreshOperateMode(OperateMode mode);
    void handleDeduplicationMode(string dirPath);

private:
    OperateMode mOperateMode { OperateMode::None };
    QString mLoadedView { "" };
    QVariantList mActionItemsList;
    OperateModes mAllOperateMode;
    QVariantList mGroupRepeatedImages;
    vector<RepeatedImages> mRepeatedImagesGroup;

};

