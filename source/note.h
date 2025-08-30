#ifndef NOTE_H
#define NOTE_H

#include <QObject>
#include <QList>

class Note : public QObject
{
    Q_OBJECT
public:
    explicit Note(QString &title, QList<QString> &tags, QString &content, const QString &uuid = QString(), QObject *parent = nullptr);

     // Head

    const QString id;

    QString title;
    QList<QString> tags;


    // Body

    QString content;

signals:
    void headChanged();
    void bodyChanged();
};

#endif // NOTE_H
