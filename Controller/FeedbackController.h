#ifndef FEEDBACKCONTROLLER_H
#define FEEDBACKCONTROLLER_H

#include <QObject>
#include "Managers/CoreFramework.h"

class FeedbackController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList feedbackAppList MEMBER mFeedbackAppList NOTIFY feedbackAppListChanged)
    Q_PROPERTY(QVariantList feedbackCompanyList MEMBER mFeedbackCompanyList NOTIFY feedbackCompanyListChanged)
public:
    explicit FeedbackController(CoreFrameworkPtr& coreFramework, QObject *parent = nullptr);

private:
    void initialize();
    void refreshfeedbackList();

public slots:
    void addFeedback(QString feedbackType, QString username, bool isAnonymous, QString feedbackMessage, QString dateAndTime);
    void likeFeedback(int feedbackId);
    void approveFeedback(int feedbackId);
    void rejectFeedback(int feedbackId);
    void deleteFeedback(int feedbackId);

signals:
    void feedbackAppListChanged();
    void feedbackCompanyListChanged();

private:
    CoreFrameworkPtr mCoreFramework;
    QVariantList mFeedbackAppList;
    QVariantList mFeedbackCompanyList;
    FeedbackInfos mFeedbackInfos;
};

#endif // FEEDBACKCONTROLLER_H
