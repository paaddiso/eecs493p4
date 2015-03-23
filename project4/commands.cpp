
#include "commands.h"
#include "boximage.h"
#include <utility>
#include <iostream>

/********************************************************************/

AddCommand::AddCommand(std::vector<QString> &in_vector,int i)
{
    filenames = in_vector;
    index = i;
}

AddCommand::~AddCommand(){}

void AddCommand::redo()
{
    globalBoxImg_ptr->addVectorOfCheckedFiles(filenames,index);
}

void AddCommand::undo()
{
    globalBoxImg_ptr->removeRange(index,filenames.size());
}

/********************************************************************/

RemoveCommand::RemoveCommand(std::vector<QString> &in_vector,int i)
{
    filenames = in_vector;
    index = i;
    std::cout << "construct RemoveCommand with index=" << index << std::endl;
}

RemoveCommand::~RemoveCommand(){}

void RemoveCommand::redo()
{
    std::cout << "call RemoveCommand::redo()" << std::endl;
    globalBoxImg_ptr->removeRange(index,filenames.size());
}

void RemoveCommand::undo()
{
    globalBoxImg_ptr->addVectorOfCheckedFiles(filenames,index);
}

/********************************************************************/

SetTimerCommand::SetTimerCommand(int a,int b)
{
    a++;
    b++;
}

SetTimerCommand::~SetTimerCommand(){}

void SetTimerCommand::undo(){}

void SetTimerCommand::redo(){}

