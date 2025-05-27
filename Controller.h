// Controller.h
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <vector>
#include <memory>
#include <stack>
#include "Volunteer.h"
#include "Event.h"
#include "BaseRepository.h" // Include the BaseRepository header
#include "Command.h"       // Include the Command interface

class Controller {
public:
    Controller(std::unique_ptr<BaseRepository<Volunteer>> volunteerRepo,
               std::unique_ptr<BaseRepository<Event>> eventRepo);
    ~Controller();

    void addVolunteer(const Volunteer& volunteer);
    void removeVolunteer(int id);
    void updateVolunteer(int oldId, const Volunteer& newVolunteer);
    std::vector<Volunteer> getAllVolunteers() const;

    void addEvent(const Event& event);
    void removeEvent(int id);
    void updateEvent(int oldId, const Event& newEvent);
    std::vector<Event> getAllEvents() const;

    void addVolunteerToEvent(int volunteerId, int eventId);
    void removeVolunteerFromEvent(int volunteerId, int eventId);

    void undo();
    void redo();

private:
    std::unique_ptr<BaseRepository<Volunteer>> m_volunteerRepo;
    std::unique_ptr<BaseRepository<Event>> m_eventRepo;
    std::stack<std::unique_ptr<Command>> m_undoStack;
    std::stack<std::unique_ptr<Command>> m_redoStack;
};

#endif // CONTROLLER_H