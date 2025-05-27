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

    populateVolunteerList();
    populateEventList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addVolunteer()
{
    int id = ui->volunteerIdLineEdit->text().toInt();
    QString name = ui->volunteerNameLineEdit->text();
    QString contactInfo = ui->volunteerContactLineEdit->text();
    m_controller->addVolunteer(Volunteer(id, name, contactInfo));
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
        QString newContactInfo = ui->volunteerContactLineEdit->text();
        m_controller->updateVolunteer(oldId, Volunteer(newId, newName, newContactInfo));
        populateVolunteerList();
        clearVolunteerInputFields();
    } else {
        QMessageBox::warning(this, "Update Volunteer", "Please select a volunteer to update.");
    }
}

void MainWindow::addEvent()
{
    int id = ui->eventIdLineEdit->text().toInt();
    QString title = ui->eventTitleLineEdit->text();
    QDate date = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd");
    QString location = ui->eventLocationLineEdit->text();
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

void MainWindow::updateSelectedEvent()
{
    QListWidgetItem* selectedItem = ui->eventListWidget->currentItem();
    if (selectedItem) {
        int oldId = selectedItem->text().split(" - ").first().toInt();
        int newId = ui->eventIdLineEdit->text().toInt();
        QString newTitle = ui->eventTitleLineEdit->text();
        QDate newDate = QDate::fromString(ui->eventDateLineEdit->text(), "yyyy-MM-dd");
        QString newLocation = ui->eventLocationLineEdit->text();
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