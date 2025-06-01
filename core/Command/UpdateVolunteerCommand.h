#ifndef UPDATEVOLUNTEERCOMMAND_H
#define UPDATEVOLUNTEERCOMMAND_H

#include "Command.h"
#include "../../Model/Volunteer.h"
#include "../../Repository/BaseRepository.h"

class UpdateVolunteerCommand : public Command {
public:
    UpdateVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& oldVolunteer, const Volunteer& newVolunteer);
    void execute() override;
    void undo() override;

private:
    BaseRepository<Volunteer>* m_volunteerRepo;
    Volunteer m_oldVolunteer;
    Volunteer m_newVolunteer;
};

#endif // UPDATEVOLUNTEERCOMMAND_H