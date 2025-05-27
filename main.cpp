#include "mainwindow.h"
#include "ui_mainwindow.h" // Include the generated UI header
#include <QMessageBox>
#include <QDebug>
#include <QDate>

MainWindow::MainWindow(Controller* controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(controller)
{
    ui->setupUi(this); // Initialize the UI from the .ui file

    // Connect signals and slots for Volunteers
    connect(ui->addVolunteerButton, &QPushButton::clicked, this, &MainWindow::addVolunteer);
    connect(ui->removeVolunteerButton, &QPushButton::clicked, this, &MainWindow::removeSelectedVolunteer);
    connect(ui->updateVolunteerButton, &QPushButton::clicked, this, &MainWindow::updateSelectedVolunteer);
    connect(ui->volunteerListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::populateVolunteerInputFields);

    // Connect signals and slots for Events
    connect(ui->addEventButton, &QPushButton::clicked, this, &MainWindow::addEvent);
    connect(ui->removeEventButton, &QPushButton::clicked, this, &MainWindow::removeSelectedEvent);
    connect(ui->updateEventButton, &QPushButton::clicked, this, &MainWindow::updateSelectedEvent);
    connect(ui->eventListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::populateEventInputFields);

    // Initial population of lists (if needed)
    populateVolunteerList();
    populateEventList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Volunteer Slots ---

void MainWindow::addVolunteer()
{
    int id = ui->volunteerIdLineEdit->text().toInt();
    QString name = ui->volunteerNameLineEdit->text();
    QString contact = ui->volunteerContactLineEdit->text();
    m_controller->addVolunteer(id, name, contact);
    populateVolunteerList();
    clearVolunteerInputFields();
}

void MainWindow::removeSelectedVolunteer()
{
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        int idToRemove = selectedItem->text().split(" - ").first().toInt();
        m_controller->removeVolunteer(idToRemove);
        populateVolunteerList();
    } else {
        QMessageBox::warning(this, "Remove Volunteer", "Please select a volunteer to remove.");
    }
}

void MainWindow::updateSelectedVolunteer()
{
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        int oldId = selectedItem->text().split(" - ").first().toInt();
        int newId = ui->volunteerIdLineEdit->text().toInt();
        QString newName = ui->volunteerNameLineEdit->text();
        QString newContact = ui->volunteerContactLineEdit->text();
        m_controller->updateVolunteer(oldId, newId, newName, newContact);
        populateVolunteerList();
        clearVolunteerInputFields();
    } else {
        QMessageBox::warning(this, "Update Volunteer", "Please select a volunteer to update.");
    }
}

void MainWindow::populateVolunteerList()
{
    ui->volunteerListWidget->clear();
    std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
    for (const auto& volunteer : volunteers) {
        ui->volunteerListWidget->addItem(QString("%1 - %2").arg(volunteer.getId()).arg(volunteer.getName()));
    }
}

void MainWindow::populateVolunteerInputFields()
{
    QListWidgetItem* selectedItem = ui->volunteerListWidget->currentItem();
    if (selectedItem) {
        QStringList parts = selectedItem->text().split(" - ");
        if (parts.size() == 2) {
            int id = parts[0].toInt();
            QString name = parts[1];
            // Find the volunteer in the list to get the contact info
            for (const auto& volunteer : m_controller->getAllVolunteers()) {
                if (volunteer.getId() == id) {
                    ui->volunteerIdLineEdit->setText(QString::number(id));
                    ui->volunteerNameLineEdit->setText(name);
                    ui->volunteerContactLineEdit->setText(volunteer.getContactInfo());
                    break;
                }
            }
        }
    }
}

void MainWindow::clearVolunteerInputFields()
{
    ui->volunteerIdLineEdit->clear();
    ui->volunteerNameLineEdit->clear();
    ui->volunteerContactLineEdit->clear();
}

// --- Event Slots ---

void MainWindow::addEvent()
{
    int id = ui->eventIdLineEdit->text().toInt();
    QString title = ui->eventTitleLineEdit->text();
    QDate date = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd"); // Assuming date format
    QString location = ui->eventLocationLineEdit->text();
    m_controller->addEvent(id, title, date, location);
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

void MainWindow::updateSelectedEvent()
{
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        int oldId = selectedItem->text().split(" - ").first().toInt();
        int newId = ui->eventIdLineEdit->text().toInt();
        QString newTitle = ui->eventTitleLineEdit->text();
        QDate newDate = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd");
        QString newLocation = ui->eventLocationLineEdit->text();
        m_controller->updateEvent(oldId, newTitle, newDate, newLocation);
        populateEventList();
        clearEventInputFields();
    } else {
        QMessageBox::warning(this, "Update Event", "Please select an event to update.");
    }
}

void MainWindow::populateEventList()
{
    ui->eventListWidget->clear();
    std::vector<Event> events = m_controller->getAllEvents();
    for (const auto& event : events) {
        ui->eventListWidget->addItem(QString("%1 - %2").arg(event.getId()).arg(event.getTitle()));
    }
}

void MainWindow::populateEventInputFields()
{
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        QStringList parts = selectedItem->text().split(" - ");
        if (parts.size() == 2) {
            int id = parts[0].toInt();
            QString title = parts[1];
            // Find the event in the list to populate the input fields
            for (const auto& event : m_controller->getAllEvents()) {
                if (event.getId() == id) {
                    ui->eventIdLineEdit->setText(QString::number(id));
                    ui->eventTitleLineEdit->setText(title);
                    ui->eventDateLineEdit->setText(event.getDate().toString("yyyy-MM-dd"));
                    ui->eventLocationLineEdit->setText(event.getLocation());
                    break;
                }
            }
        }
    }
}

void MainWindow::clearEventInputFields()
{
    ui->eventIdLineEdit->clear();
    ui->eventTitleLineEdit->clear();
    ui->eventDateLineEdit->clear();
    ui->eventLocationLineEdit->clear();
}