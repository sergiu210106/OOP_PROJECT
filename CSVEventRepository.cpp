#include "CSVEventRepository.h"

// Constructor: Initializes filename and loads data
CSVEventRepository::CSVEventRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

// Loads event data from the CSV file into memory
void CSVEventRepository::load() {
    m_events.clear(); // Clear existing data before loading
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open event CSV file for reading:" << m_filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        // Expecting ID, Title, Date, Location, [Volunteer IDs...]
        if (parts.size() >= 4) {
            int id = parts[0].toInt();
            QString title = parts[1];
            QDate date = QDate::fromString(parts[2], Qt::ISODate); // Assuming ISO date format "yyyy-MM-dd"
            QString location = parts[3];

            Event e(id, title, date, location);
            // Load associated volunteer IDs
            for (int i = 4; i < parts.size(); ++i) {
                bool ok;
                int volunteerId = parts[i].toInt(&ok);
                if (ok) {
                    e.addVolunteer(volunteerId);
                } else {
                    qWarning() << "Invalid volunteer ID in event CSV:" << parts[i];
                }
            }
            m_events.push_back(e);
        } else {
            qWarning() << "Skipping malformed line in event CSV:" << line;
        }
    }
    file.close();
    qDebug() << "Loaded" << m_events.size() << "events from" << m_filename;
}

// Saves current event data from memory to the CSV file
void CSVEventRepository::save() const {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open event CSV file for writing:" << m_filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& e : m_events) {
        out << e.getId() << "," << e.getTitle() << "," << e.getDate().toString(Qt::ISODate) << "," << e.getLocation();
        // Save associated volunteer IDs
        for (int vId : e.getVolunteerIds()) {
            out << "," << vId;
        }
        out << "\n";
    }
    file.close();
    qDebug() << "Saved" << m_events.size() << "events to" << m_filename;
}

// Adds a new event to the repository and saves changes
void CSVEventRepository::add(const Event& event) {
    // Basic validation: check if ID already exists
    for (const auto& e : m_events) {
        if (e.getId() == event.getId()) {
            qWarning() << "Event with ID" << event.getId() << "already exists. Cannot add.";
            return;
        }
    }
    m_events.push_back(event);
    save(); // Persist changes to file
    qDebug() << "Event added:" << event.getTitle();
}

// Removes an event by ID from the repository and saves changes
void CSVEventRepository::remove(int id) {
    auto it = std::remove_if(m_events.begin(), m_events.end(),
                             [id](const Event& e) { return e.getId() == id; });
    if (it != m_events.end()) {
        m_events.erase(it, m_events.end());
        save(); // Persist changes to file
        qDebug() << "Event with ID" << id << "removed.";
    } else {
        qWarning() << "Event with ID" << id << "not found for removal.";
    }
}

// Updates an existing event in the repository and saves changes
void CSVEventRepository::update(const Event& event) {
    for (auto& e : m_events) {
        if (e.getId() == event.getId()) {
            e = event;
            save(); // Persist changes to file
            qDebug() << "Event with ID" << event.getId() << "updated.";
            return;
        }
    }
    qWarning() << "Event with ID" << event.getId() << "not found for update.";
}

// Retrieves all events from the repository
std::vector<Event> CSVEventRepository::getAll() const {
    return m_events;
}