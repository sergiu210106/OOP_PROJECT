#include "JSONEventRepository.h"

JSONEventRepository::JSONEventRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

void JSONEventRepository::load() {
    m_events.clear();
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open event file for reading:" << m_filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 4) {
            int id = parts[0].toInt();
            QString title = parts[1];
            QDate date = QDate::fromString(parts[2], Qt::ISODate);
            QString location = parts[3];

            Event e(id, title, date, location);
            for (int i = 4; i < parts.size(); ++i) {
                bool ok;
                int volunteerId = parts[i].toInt(&ok);
                if (ok) {
                    e.addVolunteer(volunteerId);
                } else {
                    qWarning() << "Invalid volunteer ID in event data:" << parts[i];
                }
            }
            m_events.push_back(e);
        } else {
            qWarning() << "Skipping malformed line in event data:" << line;
        }
    }
    file.close();
    qDebug() << "Loaded " << m_events.size() << " events from " << m_filename;
}

void JSONEventRepository::save() const {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open event file for writing:" << m_filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& e : m_events) {
        out << e.getId() << "," << e.getTitle() << "," << e.getDate().toString(Qt::ISODate) << "," << e.getLocation();
        for (int vId : e.getVolunteerIds()) {
            out << "," << vId;
        }
        out << "\n";
    }
    file.close();
    qDebug() << "Saved " << m_events.size() << " events to " << m_filename;
}

void JSONEventRepository::add(const Event& event) {
    for (const auto& e : m_events) {
        if (e.getId() == event.getId()) {
            qWarning() << "Event with ID " << event.getId() << " already exists. Cannot add.";
            return;
        }
    }
    m_events.push_back(event);
    save();
    qDebug() << "Event added: " << event.getTitle();
}

void JSONEventRepository::remove(int id) {
    auto it = std::remove_if(m_events.begin(), m_events.end(),
                             [id](const Event& e) { return e.getId() == id; });
    if (it != m_events.end()) {
        m_events.erase(it, m_events.end());
        save();
        qDebug() << "Event with ID " << id << " removed.";
    } else {
        qWarning() << "Event with ID " << id << " not found for removal.";
    }
}

void JSONEventRepository::update(const Event& event) {
    for (auto& e : m_events) {
        if (e.getId() == event.getId()) {
            e.setTitle(event.getTitle());
            e.setDate(event.getDate());
            e.setLocation(event.getLocation());
            e = event; // Simple update
            save();
            qDebug() << "Event with ID " << event.getId() << " updated.";
            return;
        }
    }
    qWarning() << "Event with ID " << event.getId() << " not found for update.";
}

std::vector<Event> JSONEventRepository::getAll() const {
    return m_events;
}