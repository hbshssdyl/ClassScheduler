#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include <QtConcurrent>
#include <QFutureWatcher>
#include "Managers/CoreFramework.h"

class DatabaseController : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString dataCount MEMBER mDataCount NOTIFY dataCountChanged)

public:
    explicit DatabaseController(CoreFrameworkPtr coreFramework, QObject *parent = nullptr);

public slots:
    void onFileUploaded(QString filePath);

private:
    void initialize();
    void refreshDataCount();

signals:
    void refreshDatabaseFinished();

signals:
    void dataCountChanged();

private:
    QString mDataCount = 0;
    std::weak_ptr<CoreFramework> mCoreFramework;

    QFutureWatcher<void> mFutureWatcher; // 跟踪异步任务
};

#endif // DATABASECONTROLLER_H
