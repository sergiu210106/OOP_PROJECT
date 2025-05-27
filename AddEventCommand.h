#ifndef ADDEVENTCOMMAND_H
#define ADDEVENTCOMMAND_H

#include "Command.h"
#include "Event.h"
#include "BaseRepository.h"

class AddEventCommand : public Command {
public:
    AddEventCommand(std::unique_ptr<BaseRepository<Event>>& eventRepo, const Event& event);
    void execute() override;
    void undo() override;

private:
    std::unique_ptr<BaseRepository<Event>>& m_eventRepo;
    Event m_event;
};

#endif // ADDEVENTCOMMAND_H