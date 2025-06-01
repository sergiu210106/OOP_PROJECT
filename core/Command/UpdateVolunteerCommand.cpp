#include "UpdateVolunteerCommand.h"

UpdateVolunteerCommand::UpdateVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& oldVolunteer, const Volunteer& newVolunteer)
    : m_volunteerRepo(volunteerRepo), m_oldVolunteer(oldVolunteer), m_newVolunteer(newVolunteer) {}

void UpdateVolunteerCommand::execute() {
    m_volunteerRepo->update(m_newVolunteer);
}

void UpdateVolunteerCommand::undo() {
    m_volunteerRepo->update(m_oldVolunteer);
}