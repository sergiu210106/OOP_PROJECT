#include "CSVVolunteerRepository.h"

// Constructor: Initializes filename and loads data
CSVVolunteerRepository::CSVVolunteerRepository(const QString& filename)
    : m_filename(filename) {
    load();
}

// Loads volunteer data from the CSV file into memory
void CSVVolunteerRepository::load() {
    m_volunteers.clear(); // Clear existing data before loading
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open volunteer CSV file for reading:" << m_filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() == 3) { // Expecting ID, Name, ContactInfo
            int id = parts[0].toInt();
            QString name = parts[1];
            QString contact = parts[2];
            m_volunteers.emplace_back(id, name, contact);
        } else {
            qWarning() << "Skipping malformed line in volunteer CSV:" << line;
        }
    }
    file.close();
    qDebug() << "Loaded" << m_volunteers.size() << "volunteers from" << m_filename;
}

// Saves current volunteer data from memory to the CSV file
void CSVVolunteerRepository::save() const {
    QFile file(m_filename);
    // Truncate ensures the file is overwritten, not appended
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Could not open volunteer CSV file for writing:" << m_filename;
        return;
    }

    QTextStream out(&file);
    for (const auto& v : m_volunteers) {
        out << v.getId() << "," << v.getName() << "," << v.getContactInfo() << "\n";
    }
    file.close();
    qDebug() << "Saved" << m_volunteers.size() << "volunteers to" << m_filename;
}

// Adds a new volunteer to the repository and saves changes
void CSVVolunteerRepository::add(const Volunteer& volunteer) {
    // Basic validation: check if ID already exists
    for (const auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            qWarning() << "Volunteer with ID" << volunteer.getId() << "already exists. Cannot add.";
            return;
        }
    }
    m_volunteers.push_back(volunteer);
    save(); // Persist changes to file
    qDebug() << "Volunteer added:" << volunteer.getName();
}

// Removes a volunteer by ID from the repository and saves changes
void CSVVolunteerRepository::remove(int id) {
    auto it = std::remove_if(m_volunteers.begin(), m_volunteers.end(),
                             [id](const Volunteer& v) { return v.getId() == id; });
    if (it != m_volunteers.end()) {
        m_volunteers.erase(it, m_volunteers.end());
        save(); // Persist changes to file
        qDebug() << "Volunteer with ID" << id << "removed.";
    } else {
        qWarning() << "Volunteer with ID" << id << "not found for removal.";
    }
}

// Updates an existing volunteer in the repository and saves changes
void CSVVolunteerRepository::update(const Volunteer& volunteer) {
    for (auto& v : m_volunteers) {
        if (v.getId() == volunteer.getId()) {
            v.setName(volunteer.getName());
            v.setContactInfo(volunteer.getContactInfo());
            save(); // Persist changes to file
            qDebug() << "Volunteer with ID" << volunteer.getId() << "updated.";
            return;
        }
    }
    qWarning() << "Volunteer with ID" << volunteer.getId() << "not found for update.";
}

// Retrieves all volunteers from the repository
std::vector<Volunteer> CSVVolunteerRepository::getAll() const {
    return m_volunteers;
}