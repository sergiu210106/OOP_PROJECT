#include "AddVolunteerCommand.h"

AddVolunteerCommand::AddVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& volunteer)
    : m_volunteerRepo(volunteerRepo), m_volunteer(volunteer) {}

void AddVolunteerCommand::execute() {
    m_volunteerRepo->add(m_volunteer);
}

void AddVolunteerCommand::undo() {
    m_volunteerRepo->remove(m_volunteer.getId());
}