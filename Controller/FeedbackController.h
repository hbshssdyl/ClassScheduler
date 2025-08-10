#ifndef FEEDBACKCONTROLLER_H
#define FEEDBACKCONTROLLER_H

#include <QObject>

class FeedbackController : public QObject
{
    Q_OBJECT
public:
    explicit FeedbackController(QObject *parent = nullptr);

signals:
};

#endif // FEEDBACKCONTROLLER_H
