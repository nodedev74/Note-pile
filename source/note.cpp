#include "note.h"

#include <QUuid>

Note::Note(QString &title, QList<QString> &tags, QString &content, const QString &uuid, QObject *parent)
    : QObject(parent)
    , id(uuid.isEmpty() ? QUuid::createUuid().toString(QUuid::WithoutBraces) : uuid)
{
    this->title = title;
    this->tags = tags;
    this->content = content;
}
