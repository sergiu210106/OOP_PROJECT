#include "JSONVolunteerRepository.h"

// Constructor: Initializes filename and loads data
JSONVolunteerRepository::JSONVolunteerRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

// Loads volunteer data from the JSON file into memory
void JSONVolunteerRepository::load() {
    m_volunteers.clear(); // Clear existing data before loading
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open volunteer JSON file for reading:" << m_filename;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse volunteer JSON:" << parseError.errorString();
        return;
    }

    if (!doc.isArray()) {
        qWarning() << "Volunteer JSON is not an array.";
        return;
    }

    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            int id = obj["id"].toInt();
            QString name = obj["name"].toString();
            QString contactInfo = obj["contactInfo"].toString();
            m_volunteers.emplace_back(id, name, contactInfo);
        } else {
            qWarning() << "Skipping non-object value in volunteer JSON array.";
        }
    }
    qDebug() << "Loaded" << m_volunteers.size() << "volunteers from" << m_filename;
}

// Saves current volunteer data from memory to the JSON file
void JSONVolunteerRepository::save() const {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open volunteer JSON file for writing:" << m_filename;
        return;
    }

    QJsonArray jsonArray;
    for (const auto& v : m_volunteers) {
        QJsonObject obj;
        obj["id"] = v.getId();
        obj["name"] = v.getName();
        obj["contactInfo"] = v.getContactInfo();
        jsonArray.append(obj);
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson(QJsonDocument::Indented)); // Use Indented for readability
    file.close();
    qDebug() << "Saved" << m_volunteers.size() << "volunteers to" << m_filename;
}

// Adds a new volunteer to the repository and saves changes
void JSONVolunteerRepository::add(const Volunteer& volunteer) {
    for (const auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            qWarning() << "Volunteer with ID" << volunteer.getId() << "already exists. Cannot add.";
            return;
        }
    }
    m_volunteers.push_back(volunteer);
    save();
    qDebug() << "Volunteer added:" << volunteer.getName();
}

// Removes a volunteer by ID from the repository and saves changes
void JSONVolunteerRepository::remove(int id) {
    auto it = std::remove_if(m_volunteers.begin(), m_volunteers.end(),
                             [id](const Volunteer& v) { return v.getId() == id; });
    if (it != m_volunteers.end()) {
        m_volunteers.erase(it, m_volunteers.end());
        save();
        qDebug() << "Volunteer with ID" << id << "removed.";
    } else {
        qWarning() << "Volunteer with ID" << id << "not found for removal.";
    }
}

// Updates an existing volunteer in the repository and saves changes
void JSONVolunteerRepository::update(const Volunteer& volunteer) {
    for (auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            v.setName(volunteer.getName());
            v.setContactInfo(volunteer.getContactInfo());
            save();
            qDebug() << "Volunteer with ID" << volunteer.getId() << "updated.";
            return;
        }
    }
    qWarning() << "Volunteer with ID" << volunteer.getId() << "not found for update.";
}

// Retrieves all volunteers from the repository
std::vector<Volunteer> JSONVolunteerRepository::getAll() const {
    return m_volunteers;
}