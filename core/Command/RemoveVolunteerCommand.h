#ifndef REMOVEVOLUNTEERCOMMAND_H
#define REMOVEVOLUNTEERCOMMAND_H

#include "Command.h"
#include "../../Model/Volunteer.h"
#include "../../Repository/BaseRepository.h"

class RemoveVolunteerCommand : public Command {
public:
    RemoveVolunteerCommand(BaseRepository<Volunteer>* volunteerRepo, const Volunteer& volunteer);
    void execute() override;
    void undo() override;

private:
    BaseRepository<Volunteer>* m_volunteerRepo;
    Volunteer m_volunteer;
};

#endif // REMOVEVOLUNTEERCOMMAND_H