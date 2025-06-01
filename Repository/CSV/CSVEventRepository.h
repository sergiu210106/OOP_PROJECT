#ifndef CSVEVENTREPOSITORY_H
#define CSVEVENTREPOSITORY_H

#include "../BaseRepository.h" // Inherit from the generic BaseRepository
#include "../../Model/Event.h"          // Include the specific type
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QDate> // For QDate operations

class CSVEventRepository : public BaseRepository<Event> {
public:
    // Constructor takes the filename for the CSV
    CSVEventRepository(const QString& filename);
    ~CSVEventRepository() override = default;

    // Implement the generic CRUD operations for Event
    void add(const Event& event) override;
    void remove(int id) override;
    void update(const Event& event) override;
    std::vector<Event> getAll() const override;

private:
    QString m_filename;
    std::vector<Event> m_events; // In-memory storage

    // Helper methods for file I/O
    void load();
    void save() const;
};

#endif // CSVEVENTREPOSITORY_H