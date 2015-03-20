#ifndef COMMANDS_H
#define COMMANDS_H

#include "globals.h"
#include "clickablelabel.h"
#include <QUndoCommand>
#include <QString>
#include <vector>

class AddCommand : public QUndoCommand
{
    public:
        AddCommand(std::vector<QString> &in_vector,int i);
        ~AddCommand();
        void undo();
        void redo();

    private:
        std::vector<QString> filenames;
        int index;
};

class RemoveCommand : public QUndoCommand
{
    public:
        RemoveCommand(std::vector<QString> &in_vector,int i);
        ~RemoveCommand();
        void undo();
        void redo();

    private:
        std::vector<QString> filenames;
        int index;
};

#endif // COMMANDS_H

