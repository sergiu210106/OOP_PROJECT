#ifndef CSVVOLUNTEERREPOSITORY_H
#define CSVVOLUNTEERREPOSITORY_H

#include "../BaseRepository.h" // Inherit from the generic BaseRepository
#include "../../Model/Volunteer.h"      // Include the specific type
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <vector>           // For std::vector
#include <algorithm>        // For std::remove_if
#include <QDebug>           // For qWarning/qDebug

class CSVVolunteerRepository : public BaseRepository<Volunteer> {
public:
    // Constructor takes the filename for the CSV
    CSVVolunteerRepository(const QString& filename);
    ~CSVVolunteerRepository() override = default;

    // Implement the generic CRUD operations for Volunteer
    void add(const Volunteer& volunteer) override;
    void remove(int id) override;
    void update(const Volunteer& volunteer) override;
    std::vector<Volunteer> getAll() const override;

private:
    QString m_filename;
    std::vector<Volunteer> m_volunteers; // In-memory storage

    // Helper methods for file I/O
    void load();
    void save() const;
};

#endif // CSVVOLUNTEERREPOSITORY_H