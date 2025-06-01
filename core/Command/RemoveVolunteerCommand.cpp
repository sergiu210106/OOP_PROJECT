#include "RemoveVolunteerCommand.h"

RemoveVolunteerCommand::RemoveVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& volunteer)
    : m_volunteerRepo(volunteerRepo), m_volunteer(volunteer) {}

void RemoveVolunteerCommand::execute() {
    m_volunteerRepo->remove(m_volunteer.getId());
}

void RemoveVolunteerCommand::undo() {
    m_volunteerRepo->add(m_volunteer);
}