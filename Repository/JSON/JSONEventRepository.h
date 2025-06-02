#ifndef JSONEVENTREPOSITORY_H
#define JSONEVENTREPOSITORY_H

#include "../BaseRepository.h"
#include "../../Model/Event.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QDate>

class JSONEventRepository : public BaseRepository<Event> {
public:
    JSONEventRepository(const QString& filename);
    ~JSONEventRepository() override = default;

    void add(const Event& event) override;
    void remove(int id) override;
    void update(const Event& event) override;
    std::vector<Event> getAll() const override;

private:
    QString m_filename;
    std::vector<Event> m_events;

    void load();
    void save() const;
};

#endif // JSONEVENTREPOSITORY_H