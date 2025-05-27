#ifndef ADDVOLUNTEERCOMMAND_H
#define ADDVOLUNTEERCOMMAND_H

#include "Command.h"
#include "Volunteer.h"
#include "BaseRepository.h" // Assuming your Controller uses this

class AddVolunteerCommand : public Command {
public:
    AddVolunteerCommand(std::unique_ptr<BaseRepository<Volunteer>>& volunteerRepo, const Volunteer& volunteer);
    void execute() override;
    void undo() override;

private:
    std::unique_ptr<BaseRepository<Volunteer>>& m_volunteerRepo;
    Volunteer m_volunteer;
};

#endif // ADDVOLUNTEERCOMMAND_H