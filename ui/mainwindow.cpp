// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QDate>
#include <QDebug>

MainWindow::MainWindow(Controller* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
{
    ui->setupUi(this);

    if (!m_controller) {
        qWarning() << "MainWindow initialized with a null controller pointer!";
    } else {
        qDebug() << "MainWindow initialized with controller.";
    }

    connect(ui->addVolunteerButton, &QPushButton::clicked, this, &MainWindow::addVolunteer);
    connect(ui->removeVolunteerButton, &QPushButton::clicked, this, &MainWindow::removeSelectedVolunteer);
    connect(ui->updateVolunteerButton, &QPushButton::clicked, this, &MainWindow::updateSelectedVolunteer);
    connect(ui->addEventButton, &QPushButton::clicked, this, &MainWindow::addEvent);
    connect(ui->removeEventButton, &QPushButton::clicked, this, &MainWindow::removeSelectedEvent);
    connect(ui->updateEventButton, &QPushButton::clicked, this, &MainWindow::updateSelectedEvent);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::undo);
    connect(ui->redoButton, &QPushButton::clicked, this, &MainWindow::redo);
    connect(ui->assignVolunteerToEventButton, &QPushButton::clicked, this, &MainWindow::assignVolunteerToEvent);
    connect(ui->removeVolunteerFromEventButton, &QPushButton::clicked, this, &MainWindow::removeVolunteerFromEvent);
    connect(ui->eventListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::updateVolunteersInEventList);

    connect(ui->filterEventsByDateButton, &QPushButton::clicked, this, &MainWindow::filterEventsByDate);
    connect(ui->filterEventsByLocationButton, &QPushButton::clicked, this, &MainWindow::filterEventsByLocation);

    populateVolunteerList();
    populateEventList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addVolunteer() {
    int id = ui->volunteerIdLineEdit->text().toInt();
    QString name = ui->volunteerNameLineEdit->text();
    QString contactInfo = ui->volunteerContactLineEdit->text();

    if (name.isEmpty() or contactInfo.isEmpty()) {
        QMessageBox::warning(this, "Add Volunteer", "Please fill in all volunteer details.");
        return;
    }

    m_controller->addVolunteer(Volunteer(id, name, contactInfo));
    populateVolunteerList();
    clearVolunteerInputFields();
}

void MainWindow::removeSelectedVolunteer() {
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        int idToRemove = selectedItem->text().split(" - ").first().toInt();
        m_controller->removeVolunteer(idToRemove);
        populateVolunteerList();
    } else {
        QMessageBox::warning(this, "Remove Volunteer", "Please select a volunteer to remove.");
    }
}

void MainWindow::updateSelectedVolunteer() {
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        int oldId = selectedItem->text().split(" - ").first().toInt();
        int newId = ui->volunteerIdLineEdit->text().toInt();
        QString newName = ui->volunteerNameLineEdit->text();
        QString newContactInfo = ui->volunteerContactLineEdit->text();

        if (newName.isEmpty() || newContactInfo.isEmpty()) {
            QMessageBox::warning(this, "Update Volunteer", "Please fill in all volunteer details.");
            return;
        }

        m_controller->updateVolunteer(oldId, Volunteer(newId, newName, newContactInfo));
        populateVolunteerList();
        clearVolunteerInputFields();
    } else {
        QMessageBox::warning(this, "Update Volunteer", "Please select a volunteer to update.");
    }
}

void MainWindow::addEvent() {
    int id = ui->eventIdLineEdit->text().toInt();
    QString title = ui->eventTitleLineEdit->text();
    QDate date = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd");
    QString location = ui->eventLocationLineEdit->text();

    if (title.isEmpty() || location.isEmpty() || !date.isValid()) {
        QMessageBox::warning(this, "Add Event", "Please fill in all event details and ensure the date is valid (yyyy-MM-dd).");
        return;
    }

    m_controller->addEvent(Event(id, title, date, location));
    populateEventList();
    clearEventInputFields();
}

void MainWindow::removeSelectedEvent()
{
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        int idToRemove = selectedItem->text().split(" - ").first().toInt();
        m_controller->removeEvent(idToRemove);
        populateEventList();
    } else {
        QMessageBox::warning(this, "Remove Event", "Please select an event to remove.");
    }
}

void MainWindow::updateSelectedEvent() {
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        int oldId = selectedItem->text().split(" - ").first().toInt();
        int newId = ui->eventIdLineEdit->text().toInt();
        QString newTitle = ui->eventTitleLineEdit->text();
        QDate newDate = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd");
        QString newLocation = ui->eventLocationLineEdit->text();

        if (newTitle.isEmpty() || newLocation.isEmpty() || !newDate.isValid()) {
            QMessageBox::warning(this, "Update Event", "Please fill in all event details and ensure the date is valid (yyyy-MM-dd).");
            return;
        }

        m_controller->updateEvent(oldId, Event(newId, newTitle, newDate, newLocation));
        populateEventList();
        clearEventInputFields();
    } else {
        QMessageBox::warning(this, "Update Event", "Please select an event to update.");
    }
}

void MainWindow::undo()
{
    m_controller->undo();
    populateVolunteerList();
    populateEventList();
}

void MainWindow::redo()
{
    m_controller->redo();
    populateVolunteerList();
    populateEventList();
}

void MainWindow::populateVolunteerList()
{
    ui->volunteerListWidget->clear();
    std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
    for (const auto& volunteer : volunteers) {
        ui->volunteerListWidget->addItem(QString::number(volunteer.getId()) + " - " + volunteer.getName());
    }
}

void MainWindow::populateEventList()
{
    ui->eventListWidget->clear();
    std::vector<Event> events = m_controller->getAllEvents();
    for (const auto& event : events) {
        ui->eventListWidget->addItem(QString::number(event.getId()) + " - " + event.getTitle() + " (" + event.getDate().toString("yyyy-MM-dd") + ")");
    }
}

void MainWindow::clearVolunteerInputFields()
{
    ui->volunteerIdLineEdit->clear();
    ui->volunteerNameLineEdit->clear();
    ui->volunteerContactLineEdit->clear();
}

void MainWindow::clearEventInputFields()
{
    ui->eventIdLineEdit->clear();
    ui->eventTitleLineEdit->clear();
    ui->eventDateLineEdit->clear();
    ui->eventLocationLineEdit->clear();
}

void MainWindow::populateVolunteerInputFields() {
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        QStringList parts = selectedItem->text().split(" - ");
        if (parts.size() == 2) {
            const int id = parts[0].toInt();
            std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
            for (const auto& volunteer : volunteers) {
                if (volunteer.getId() == id) {
                    ui->volunteerIdLineEdit->setText(QString::number(volunteer.getId()));
                    ui->volunteerNameLineEdit->setText(volunteer.getName());
                    ui->volunteerContactLineEdit->setText(volunteer.getContactInfo());
                    return; // Exit after finding the volunteer
                }
            }
        }
    } else {
        clearVolunteerInputFields();
    }
}
void MainWindow::populateEventInputFields() {
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        QStringList parts = selectedItem->text().split(" - ");
        if (parts.size() == 2) {
            int id = parts[0].toInt();
            QString titleAndDate = parts[1];
            int openParenIndex = titleAndDate.indexOf("(");
            int closeParenIndex = titleAndDate.indexOf(")");
            if (openParenIndex != -1 && closeParenIndex != -1) {
                QString title = titleAndDate.left(openParenIndex).trimmed();
                QString dateString = titleAndDate.mid(openParenIndex + 1, closeParenIndex - openParenIndex - 1);
                QDate date = QDate::fromString(dateString, "yyyy-MM-dd");

                std::vector<Event> events = m_controller->getAllEvents();
                for (const auto& event : events) {
                    if (event.getId() == id) {
                        ui->eventIdLineEdit->setText(QString::number(event.getId()));
                        ui->eventTitleLineEdit->setText(event.getTitle());
                        ui->eventDateLineEdit->setText(event.getDate().toString("yyyy-MM-dd"));
                        ui->eventLocationLineEdit->setText(event.getLocation());
                        return; // Exit after finding the event
                    }
                }
            }
        }
    } else {
        clearEventInputFields();
    }
}

void MainWindow::assignVolunteerToEvent() {
    QListWidgetItem* selectedVolunteerItem = ui->volunteerListWidget->currentItem();
    QListWidgetItem* selectedEventItem = ui->eventListWidget->currentItem();

    if (!selectedVolunteerItem || !selectedEventItem) {
        QMessageBox::warning(this, "Assign Volunteer", "Please select a volunteer and an event.");
        return;
    }

    int volunteerId = selectedVolunteerItem->text().split(" - ").first().toInt();
    int eventId = selectedEventItem->text().split(" - ").first().toInt();

    m_controller->addVolunteerToEvent(volunteerId, eventId);
    updateVolunteersInEventList(); // Refresh the list
}

void MainWindow::removeVolunteerFromEvent() {
    QListWidgetItem* selectedVolunteerItem = ui->volunteersInEventListWidget->currentItem();
    QListWidgetItem* selectedEventItem = ui->eventListWidget->currentItem();

    if (!selectedVolunteerItem || !selectedEventItem) {
        QMessageBox::warning(this, "Remove Volunteer", "Please select a volunteer from the event's volunteer list.");
        return;
    }

    if (!selectedEventItem) {
        QMessageBox::warning(this, "Remove Volunteer", "Please select an event.");
        return;
    }

    int volunteerId = selectedVolunteerItem->text().split(" - ").first().toInt();
    int eventId = selectedEventItem->text().split(" - ").first().toInt();

    m_controller->removeVolunteerFromEvent(volunteerId, eventId);
    updateVolunteersInEventList(); // Refresh the list
}

void MainWindow::updateVolunteersInEventList() {
    ui->volunteersInEventListWidget->clear();
    QListWidgetItem* selectedEventItem = ui->eventListWidget->currentItem();

    if (selectedEventItem) {
        int eventId = selectedEventItem->text().split(" - ").first().toInt();
        std::vector<Event> events = m_controller->getAllEvents();
        for (const auto& event : events) {
            if (event.getId() == eventId) {
                QList<int> volunteerIds = event.getVolunteerIds();
                std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
                for (const auto& volunteer : volunteers) {
                    if (volunteerIds.contains(volunteer.getId())) {
                        ui->volunteersInEventListWidget->addItem(QString::number(volunteer.getId()) + " - " + volunteer.getName());
                    }
                }
                break; // Event found, exit loop
            }
        }
    }
}

void MainWindow::filterEventsByDate() {
    QString dateFilter = ui->eventDateFilterLineEdit->text();
    qDebug() << "filterEventsByDate: dateFilter =" << dateFilter;
    QDate filterDate = QDate::fromString(dateFilter, "yyyy-MM-dd");
    qDebug() << "filterEventsByDate: filterDate =" << filterDate;

    if (!filterDate.isValid() && !dateFilter.isEmpty()) {
        QMessageBox::warning(this, "Invalid Date", "Please enter the date in yyyy-MM-dd format.");
        return;
    }

    std::vector<Event> filteredEvents = m_controller->filterEventsByDate(dateFilter);
    qDebug() << "filterEventsByDate: filteredEvents.size() =" << filteredEvents.size();

    ui->eventListWidget->clear();
    int i = 0; // Add an index counter
    for (const auto& event : filteredEvents) {
        qDebug() << "  Event " << i++ << ": id=" << event.getId() << ", title=" << event.getTitle(); // Detailed event info
        ui->eventListWidget->addItem(QString::number(event.getId()) + " - " + event.getTitle() + " (" + event.getDate().toString("yyyy-MM-dd") + ")");
    }
    qDebug() << "filterEventsByDate:  Finished loop"; // Add a final log
}

void MainWindow::filterEventsByLocation() {
    QString locationFilter = ui->eventLocationFilterLineEdit->text();
    qDebug() << "filterEventsByLocation: locationFilter =" << locationFilter; // ADDED

    std::vector<Event> filteredEvents = m_controller->filterEventsByLocation(locationFilter);
    qDebug() << "filterEventsByLocation: filteredEvents.size() =" << filteredEvents.size(); // ADDED

    ui->eventListWidget->clear();
    int i = 0; // Add an index counter
    for (const auto& event : filteredEvents) {
        qDebug() << "  Event " << i++ << ": id=" << event.getId() << ", title=" << event.getTitle() << ", location=" << event.getLocation(); // Detailed event info
        ui->eventListWidget->addItem(QString::number(event.getId()) + " - " + event.getTitle() + " (" + event.getDate().toString("yyyy-MM-dd") + " - " + event.getLocation() + ")");
    }
    qDebug() << "filterEventsByLocation: Finished loop"; // Add a final log
}
