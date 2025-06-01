#ifndef VOLUNTEER_H
#define VOLUNTEER_H

#include <QString>

class Volunteer {
private:
    int m_id;
    QString m_name;
    QString m_contactInfo;

public:
    Volunteer(int id, const QString& name, const QString& contactInfo);

    int getId() const;

    QString getName() const;
    void setName(const QString& name);

    QString getContactInfo() const;
    void setContactInfo(const QString& contactInfo);
};


#endif // VOLUNTEER_H
