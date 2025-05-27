// Controller.cpp
#include "Controller.h"
#include <QDebug>
#include <algorithm>

// Include concrete command headers once they are updated to use generic repo methods
// #include "AddVolunteerCommand.h"
// #include "RemoveVolunteerCommand.h"
// #include "UpdateVolunteerCommand.h"
// #include "AddEventCommand.h"
// #include "RemoveEventCommand.h"
// #include "UpdateEventCommand.h"


Controller::Controller(std::unique_ptr<BaseRepository<Volunteer>> volunteerRepo,
                       std::unique_ptr<BaseRepository<Event>> eventRepo)
    : m_volunteerRepo(std::move(volunteerRepo)), m_eventRepo(std::move(eventRepo)) {
    if (!m_volunteerRepo) {
        qWarning() << "Volunteer Repository not available.";
    }
    if (!m_eventRepo) {
        qWarning() << "Event Repository not available.";
    }
}

Controller::~Controller() {
    qDebug() << "Controller destroyed.";
}

// --- Volunteer Management Implementations ---

void Controller::addVolunteer(const Volunteer& volunteer) {
    if (m_volunteerRepo) {
        // Here you would typically create and execute a Command object
        // For now, directly call the repository method
        m_volunteerRepo->add(volunteer);
        qDebug() << "Volunteer added:" << volunteer.getName();
        // Push command to undo stack: m_undoStack.push(std::make_unique<AddVolunteerCommand>(*m_volunteerRepo, volunteer));
        // Clear redo stack
    } else {
        qWarning() << "Volunteer Repository not available. Cannot add volunteer.";
    }
}

void Controller::removeVolunteer(int id) {
    if (m_volunteerRepo) {
        // Create and execute RemoveVolunteerCommand, push to undo stack, clear redo
        m_volunteerRepo->remove(id);
        qDebug() << "Volunteer removed with ID:" << id;
    } else {
        qWarning() << "Volunteer Repository not available. Cannot remove volunteer.";
    }
}

void Controller::updateVolunteer(int oldId, const Volunteer& newVolunteer) {
    if (m_volunteerRepo) {
        // Create and execute UpdateVolunteerCommand, push to undo stack, clear redo
        // The repository update method might need the oldId to find the volunteer
        m_volunteerRepo->update(newVolunteer);
        qDebug() << "Volunteer updated:" << newVolunteer.getName();
    } else {
        qWarning() << "Volunteer Repository not available. Cannot update volunteer.";
    }
}

std::vector<Volunteer> Controller::getAllVolunteers() const {
    if (m_volunteerRepo) {
        return m_volunteerRepo->getAll();
    } else {
        qWarning() << "Volunteer Repository not available. Cannot get all volunteers.";
        return std::vector<Volunteer>(); // Return an empty vector
    }
}

// --- Event Management Implementations ---

void Controller::addEvent(const Event& event) {
    if (m_eventRepo) {
        // Create and execute AddEventCommand, push to undo stack, clear redo
        m_eventRepo->add(event);
        qDebug() << "Event added:" << event.getTitle();
    } else {
        qWarning() << "Event Repository not available. Cannot add event.";
    }
}

void Controller::removeEvent(int id) {
    if (m_eventRepo) {
        // Create and execute RemoveEventCommand, push to undo stack, clear redo
        m_eventRepo->remove(id);
        qDebug() << "Event removed with ID:" << id;
    } else {
        qWarning() << "Event Repository not available. Cannot remove event.";
    }
}

void Controller::updateEvent(int oldId, const Event& newEvent) {
    if (m_eventRepo) {
        // Create and execute UpdateEventCommand, push to undo stack, clear redo
        // The repository update method might need the oldId to find the event
        m_eventRepo->update(newEvent);
        qDebug() << "Event updated:" << newEvent.getTitle();
    } else {
        qWarning() << "Event Repository not available. Cannot update event.";
    }
}

std::vector<Event> Controller::getAllEvents() const {
    if (m_eventRepo) {
        return m_eventRepo->getAll();
    } else {
        qWarning() << "Event Repository not available. Cannot get all events.";
        return std::vector<Event>(); // Return an empty vector
    }
}

// --- Volunteer-Event Association Implementations ---

void Controller::addVolunteerToEvent(int volunteerId, int eventId) {
    if (!m_eventRepo || !m_volunteerRepo) {
        qWarning() << "Repository not available. Cannot add volunteer to event.";
        return;
    }

    // Find the volunteer and event (for validation, if needed)
    std::vector<Volunteer> volunteers = m_volunteerRepo->getAll();
    auto volunteerIt = std::find_if(volunteers.begin(), volunteers.end(),
                                  [volunteerId](const Volunteer& v) { return v.getId() == volunteerId; });

    std::vector<Event> events = m_eventRepo->getAll();
    auto eventIt = std::find_if(events.begin(), events.end(),
                               [eventId](const Event& e) { return e.getId() == eventId; });

    if (volunteerIt == volunteers.end()) {
        qWarning() << "Volunteer with ID" << volunteerId << "not found. Cannot associate with event.";
        return;
    }

    if (eventIt == events.end()) {
        qWarning() << "Event with ID" << eventId << "not found.";
        return;
    }

    // Create a modifiable copy of the event
    Event eventToUpdate = *eventIt;
    eventToUpdate.addVolunteer(volunteerId);
    m_eventRepo->update(eventToUpdate); // Update the event in the repository
    qDebug() << "Volunteer" << volunteerId << "added to Event" << eventId;
}

void Controller::removeVolunteerFromEvent(int volunteerId, int eventId) {
    if (!m_eventRepo || !m_volunteerRepo) {
        qWarning() << "Repository not available. Cannot remove volunteer from event.";
        return;
    }

    // Find the event
    std::vector<Event> events = m_eventRepo->getAll();
    auto eventIt = std::find_if(events.begin(), events.end(),
                               [eventId](const Event& e) { return e.getId() == eventId; });

    if (eventIt == events.end()) {
        qWarning() << "Event with ID" << eventId << "not found.";
        return;
    }

    // Create a modifiable copy of the event
    Event eventToUpdate = *eventIt;
    eventToUpdate.removeVolunteer(volunteerId);
    m_eventRepo->update(eventToUpdate); // Update the event in the repository
    qDebug() << "Volunteer" << volunteerId << "removed from Event" << eventId;
}

// Undo/Redo will be implemented using Command Pattern,
// which will then call the generic repository methods.
void Controller::undo() {
    qDebug() << "Undo functionality to be implemented using Command Pattern.";
    // Example:
    // if (!m_undoStack.empty()) {
    //     std::unique_ptr<Command> command = std::move(m_undoStack.top());
    //     m_undoStack.pop();
    //     command->undo();
    //     m_redoStack.push(std::move(command));
    // }
}

void Controller::redo() {
    qDebug() << "Redo functionality to be implemented using Command Pattern.";
    // Example:
    // if (!m_redoStack.empty()) {
    //     std::unique_ptr<Command> command = std::move(m_redoStack.top());
    //     m_redoStack.pop();
    //     command->execute();
    //     m_undoStack.push(std::move(command));
    // }
}