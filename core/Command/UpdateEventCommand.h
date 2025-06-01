#ifndef UPDATEEVENTCOMMAND_H
#define UPDATEEVENTCOMMAND_H

#include "Command.h"
#include "../../Model/Event.h"
#include "../../Repository/BaseRepository.h"

class UpdateEventCommand : public Command {
public:
    UpdateEventCommand(BaseRepository<Event>* eventRepo, const Event& oldEvent, const Event& newEvent);
    void execute() override;
    void undo() override;

private:
    BaseRepository<Event>* m_eventRepo;
    Event m_oldEvent;
    Event m_newEvent;
};

#endif // UPDATEEVENTCOMMAND_H