#include "RemoveEventCommand.h"

RemoveEventCommand::RemoveEventCommand(BaseRepository<Event>* eventRepo, const Event& event)
    : m_eventRepo(eventRepo), m_event(event) {}

void RemoveEventCommand::execute() {
    m_eventRepo->remove(m_event.getId());
}

void RemoveEventCommand::undo() {
    m_eventRepo->add(m_event);
}