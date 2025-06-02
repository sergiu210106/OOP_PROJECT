#include "JSONVolunteerRepository.h"

JSONVolunteerRepository::JSONVolunteerRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

void JSONVolunteerRepository::load() {
    m_volunteers.clear();
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open volunteer file for reading:" << m_filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() == 3) {
            int id = parts[0].toInt();
            QString name = parts[1];
            QString contactInfo = parts[2];
            m_volunteers.emplace_back(id, name, contactInfo);
        } else {
            qWarning() << "Skipping malformed line in volunteer data:" << line;
        }
    }
    file.close();
    qDebug() << "Loaded " << m_volunteers.size() << " volunteers from " << m_filename;
}

void JSONVolunteerRepository::save() const {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open volunteer file for writing:" << m_filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& v : m_volunteers) {
        out << v.getId() << "," << v.getName() << "," << v.getContactInfo() << "\n";
    }
    file.close();
    qDebug() << "Saved " << m_volunteers.size() << " volunteers to " << m_filename;
}

void JSONVolunteerRepository::add(const Volunteer& volunteer) {
    for (const auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            qWarning() << "Volunteer with ID " << volunteer.getId() << " already exists. Cannot add.";
            return;
        }
    }
    m_volunteers.push_back(volunteer);
    save();
    qDebug() << "Volunteer added: " << volunteer.getName();
}

void JSONVolunteerRepository::remove(int id) {
    auto it = std::remove_if(m_volunteers.begin(), m_volunteers.end(),
                             [id](const Volunteer& v) { return v.getId() == id; });
    if (it != m_volunteers.end()) {
        m_volunteers.erase(it, m_volunteers.end());
        save();
        qDebug() << "Volunteer with ID " << id << " removed.";
    } else {
        qWarning() << "Volunteer with ID " << id << " not found for removal.";
    }
}

void JSONVolunteerRepository::update(const Volunteer& volunteer) {
    for (auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            v.setName(volunteer.getName());
            v.setContactInfo(volunteer.getContactInfo());
            save();
            qDebug() << "Volunteer with ID " << volunteer.getId() << " updated.";
            return;
        }
    }
    qWarning() << "Volunteer with ID " << volunteer.getId() << " not found for update.";
}

std::vector<Volunteer> JSONVolunteerRepository::getAll() const {
    return m_volunteers;
}