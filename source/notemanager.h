#ifndef NOTEMANAGER_H
#define NOTEMANAGER_H

#include <QObject>

class NoteManager : public QObject
{
    Q_OBJECT

private:
    explicit NoteManager(QObject *parent = nullptr);

public:  
    static NoteManager& instance();

signals:
    void noteDeleted();
    void noteChanged();
    void noteCreated();

};

#endif // NOTEMANAGER_H
