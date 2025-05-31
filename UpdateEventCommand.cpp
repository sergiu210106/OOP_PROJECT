#include "UpdateEventCommand.h"

UpdateEventCommand::UpdateEventCommand(BaseRepository<Event>* eventRepo, const Event& oldEvent, const Event& newEvent)
    : m_eventRepo(eventRepo), m_oldEvent(oldEvent), m_newEvent(newEvent) {}

void UpdateEventCommand::execute() {
    m_eventRepo->update(m_newEvent);
}

void UpdateEventCommand::undo() {
    m_eventRepo->update(m_oldEvent);
}