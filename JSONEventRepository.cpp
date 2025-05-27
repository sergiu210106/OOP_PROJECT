#include "JSONEventRepository.h"

// Constructor: Initializes filename and loads data
JSONEventRepository::JSONEventRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

// Loads event data from the JSON file into memory
void JSONEventRepository::load() {
    m_events.clear(); // Clear existing data before loading
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open event JSON file for reading:" << m_filename;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse event JSON:" << parseError.errorString();
        return;
    }

    if (!doc.isArray()) {
        qWarning() << "Event JSON is not an array.";
        return;
    }

    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            int id = obj["id"].toInt();
            QString title = obj["title"].toString();
            QDate date = QDate::fromString(obj["date"].toString(), Qt::ISODate); // Assuming ISO date format
            QString location = obj["location"].toString();

            Event e(id, title, date, location);
            // Load associated volunteer IDs
            if (obj.contains("volunteerIds") && obj["volunteerIds"].isArray()) {
                QJsonArray volunteerIdsArray = obj["volunteerIds"].toArray();
                for (const QJsonValue& vIdValue : volunteerIdsArray) {
                    if (vIdValue.isDouble()) { // JSON numbers are doubles
                        e.addVolunteer(vIdValue.toInt());
                    } else {
                        qWarning() << "Invalid volunteer ID type in event JSON array.";
                    }
                }
            }
            m_events.push_back(e);
        } else {
            qWarning() << "Skipping non-object value in event JSON array.";
        }
    }
    qDebug() << "Loaded" << m_events.size() << "events from" << m_filename;
}

// Saves current event data from memory to the JSON file
void JSONEventRepository::save() const {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open event JSON file for writing:" << m_filename;
        return;
    }

    QJsonArray jsonArray;
    for (const auto& e : m_events) {
        QJsonObject obj;
        obj["id"] = e.getId();
        obj["title"] = e.getTitle();
        obj["date"] = e.getDate().toString(Qt::ISODate); // Save date in ISO format
        obj["location"] = e.getLocation();

        QJsonArray volunteerIdsArray;
        for (int vId : e.getVolunteerIds()) {
            volunteerIdsArray.append(vId);
        }
        obj["volunteerIds"] = volunteerIdsArray;
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson(QJsonDocument::Indented)); // Use Indented for readability
    file.close();
    qDebug() << "Saved" << m_events.size() << "events to" << m_filename;
}

// Adds a new event to the repository and saves changes
void JSONEventRepository::add(const Event& event) {
    for (const auto& e : m_events) {
        if (e.getId() == event.getId()) {
            qWarning() << "Event with ID" << event.getId() << "already exists. Cannot add.";
            return;
        }
    }
    m_events.push_back(event);
    save();
    qDebug() << "Event added:" << event.getTitle();
}

// Removes an event by ID from the repository and saves changes
void JSONEventRepository::remove(int id) {
    auto it = std::remove_if(m_events.begin(), m_events.end(),
                             [id](const Event& e) { return e.getId() == id; });
    if (it != m_events.end()) {
        m_events.erase(it, m_events.end());
        save();
        qDebug() << "Event with ID" << id << "removed.";
    } else {
        qWarning() << "Event with ID" << id << "not found for removal.";
    }
}

// Updates an existing event in the repository and saves changes
void JSONEventRepository::update(const Event& event) {
    for (auto& e : m_events) {
        if (e.getId() == event.getId()) {
            e.setTitle(event.getTitle());
            e.setDate(event.getDate());
            e.setLocation(event.getLocation());
            // Assuming Event::addVolunteer/removeVolunteer are used for volunteer IDs
            save();
            qDebug() << "Event with ID" << event.getId() << "updated.";
            return;
        }
    }
    qWarning() << "Event with ID" << event.getId() << "not found for update.";
}

// Retrieves all events from the repository
std::vector<Event> JSONEventRepository::getAll() const {
    return m_events;
}