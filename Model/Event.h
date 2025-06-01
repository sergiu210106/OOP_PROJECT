#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QDate>
#include <QList>

class Event
{
private:
    int m_id;
    QString m_title;
    QDate m_date;
    QString m_location;
    QList<int> m_volunteerIds;

public:
    Event(int id, const QString& title, const QDate& date, const QString& location);
    int getId() const;

    QString getTitle() const;
    void setTitle(const QString& title);

    QDate getDate() const;
    void setDate(const QDate& date);

    QString getLocation() const;
    void setLocation(const QString& location);

    QList<int> getVolunteerIds() const;
    void addVolunteer(int volunteerId);
    void removeVolunteer(int volunteerId);
};

#endif // EVENT_H
