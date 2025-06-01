#include "Volunteer.h"

Volunteer::Volunteer(int id, const QString& name, const QString& contactInfo) :
    m_id(id), m_name(name), m_contactInfo(contactInfo){}

int Volunteer::getId() const {
    return m_id;
}

QString Volunteer::getName() const {
    return m_name;
}

void Volunteer::setName(const QString& name) {
    m_name = name;
}

QString Volunteer::getContactInfo() const {
    return m_contactInfo;
}

void Volunteer::setContactInfo(const QString& contactInfo) {
    m_contactInfo = contactInfo;
}

