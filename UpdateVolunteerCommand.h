#ifndef UPDATEVOLUNTEERCOMMAND_H
#define UPDATEVOLUNTEERCOMMAND_H

#include "Command.h"
#include "Volunteer.h"
#include "BaseRepository.h"

class UpdateVolunteerCommand : public Command {
public:
    UpdateVolunteerCommand(std::unique_ptr<BaseRepository<Volunteer>>& volunteerRepo, const Volunteer& oldVolunteer, const Volunteer& newVolunteer);
    void execute() override;
    void undo() override;

private:
    std::unique_ptr<BaseRepository<Volunteer>>& m_volunteerRepo;
    Volunteer m_oldVolunteer;
    Volunteer m_newVolunteer;
};

#endif // UPDATEVOLUNTEERCOMMAND_H