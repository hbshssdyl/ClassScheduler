#ifndef ACCOUNTVIEWCONTROLLER_H
#define ACCOUNTVIEWCONTROLLER_H

#include <QObject>

class AccountViewController : public QObject
{
    Q_OBJECT
public:
    explicit AccountViewController(QObject *parent = nullptr);

signals:
};

#endif // ACCOUNTVIEWCONTROLLER_H
