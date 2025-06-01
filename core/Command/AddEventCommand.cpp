#include "AddEventCommand.h"

AddEventCommand::AddEventCommand(BaseRepository<Event>* eventRepo, const Event& event)
    : m_eventRepo(eventRepo), m_event(event) {}

void AddEventCommand::execute() {
    m_eventRepo->add(m_event);
}

void AddEventCommand::undo() {
    m_eventRepo->remove(m_event.getId());
}