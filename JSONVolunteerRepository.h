#ifndef JSONVOLUNTEERREPOSITORY_H
#define JSONVOLUNTEERREPOSITORY_H

#include "BaseRepository.h"
#include "Volunteer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QString>
#include <vector>
#include <algorithm>
#include <QDebug>

class JSONVolunteerRepository : public BaseRepository<Volunteer> {
public:
    JSONVolunteerRepository(const QString& filename);
    ~JSONVolunteerRepository() override = default;

    void add(const Volunteer& volunteer) override;
    void remove(int id) override;
    void update(const Volunteer& volunteer) override;
    std::vector<Volunteer> getAll() const override;

private:
    QString m_filename;
    std::vector<Volunteer> m_volunteers;

    void load();
    void save() const;
};

#endif // JSONVOLUNTEERREPOSITORY_H