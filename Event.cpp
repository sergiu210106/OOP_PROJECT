#include "Event.h"

Event::Event(int id, const QString& title, const QDate& date, const QString& location) :
    m_id(id), m_title(title), m_date(date), m_location(location) {}


int Event::getId() const {
    return m_id;
}

QString Event::getTitle() const {
    return m_title;
}

void Event::setTitle(const QString& title) {
    m_title = title;
}

QDate Event::getDate() const {
    return m_date;
}
void Event::setDate(const QDate& date) {
    m_date = date;
}

QString Event::getLocation() const {
    return m_location;
}

void Event::setLocation(const QString& location) {
    m_location = location;
}

QList<int> Event::getVolunteerIds() const {
    return m_volunteerIds;
}

void Event::addVolunteer(int volunteerId) {
    if (! m_volunteerIds.contains(volunteerId)) {
        m_volunteerIds.append(volunteerId);
    }
}

void Event::removeVolunteer(int volunteerId) {
    m_volunteerIds.removeAll(volunteerId);
}


