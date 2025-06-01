#ifndef REMOVEEVENTCOMMAND_H
#define REMOVEEVENTCOMMAND_H

#include "Command.h"
#include "../../Model/Event.h"
#include "../../Repository/BaseRepository.h"

class RemoveEventCommand : public Command {
public:
    RemoveEventCommand(BaseRepository<Event>* eventRepo, const Event& event);
    void execute() override;
    void undo() override;

private:
    BaseRepository<Event>* m_eventRepo;
    Event m_event;
};

#endif // REMOVEEVENTCOMMAND_H