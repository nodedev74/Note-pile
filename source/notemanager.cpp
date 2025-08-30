#include "notemanager.h"

NoteManager::NoteManager(QObject *parent)
    : QObject{parent}
{}

NoteManager& NoteManager::instance() {
    static NoteManager inst;
    return inst;
}

void NoteManager::createNote()
{

}
