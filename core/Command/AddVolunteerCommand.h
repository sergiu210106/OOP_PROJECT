#ifndef ADDVOLUNTEERCOMMAND_H
#define ADDVOLUNTEERCOMMAND_H

#include "Command.h"
#include "../../Model/Volunteer.h"
#include "../../Repository/BaseRepository.h" // Assuming your Controller uses this

class AddVolunteerCommand : public Command {
public:
    AddVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& volunteer);
    void execute() override;
    void undo() override;

private:
    BaseRepository<Volunteer>* m_volunteerRepo;
    Volunteer m_volunteer;
};

#endif // ADDVOLUNTEERCOMMAND_H