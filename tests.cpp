#include <QtTest/QtTest>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QDate>
#include <QDebug>
#include <memory>
#include <algorithm>

// Include your project headers
#include "Volunteer.h"
#include "Event.h"
#include "CSVVolunteerRepository.h"
#include "CSVEventRepository.h"
#include "Controller.h"

class TestVolunteerManagement : public QObject
{
    Q_OBJECT

private slots:
    // Test setup and cleanup
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Volunteer class tests
    void testVolunteerCreation();
    void testVolunteerGettersSetters(); // Corrected: Removed duplicate 'void'

    // Event class tests
    void testEventCreation();
    void testEventGettersSetters();
    void testEventVolunteerManagement();

    // CSVVolunteerRepository tests
    void testVolunteerRepositoryAdd();
    void testVolunteerRepositoryRemove();
    void testVolunteerRepositoryUpdate();
    void testVolunteerRepositoryGetAll();
    void testVolunteerRepositoryPersistence();
    void testVolunteerRepositoryDuplicateHandling();

    // CSVEventRepository tests
    void testEventRepositoryAdd();
    void testEventRepositoryRemove();
    void testEventRepositoryUpdate();
    void testEventRepositoryGetAll();
    void testEventRepositoryPersistence();
    void testEventRepositoryVolunteerAssociations();

    // Controller tests
    void testControllerVolunteerOperations();
    void testControllerEventOperations();
    void testControllerVolunteerEventAssociations();
    void testControllerUndoRedo();
    void testControllerFilterEventsByDate(); // NEW
    void testControllerFilterEventsByLocation(); // NEW

    // Integration tests
    void testCompleteWorkflow();
    void testDataPersistenceAcrossApplicationRestart();

private:
    QString m_tempDir;
    QString m_volunteerCsvPath;
    QString m_eventCsvPath;
    std::unique_ptr<Controller> m_controller;

    // Helper methods
    void createTestVolunteers();
    void createTestEvents();
    void verifyVolunteerInFile(int id, const QString& name, const QString& contact);
    void verifyEventInFile(int id, const QString& title, const QDate& date, const QString& location);
};

void TestVolunteerManagement::initTestCase()
{
    qDebug() << "Starting Volunteer Management System Tests";
}

void TestVolunteerManagement::cleanupTestCase()
{
    qDebug() << "Finished Volunteer Management System Tests";
}

void TestVolunteerManagement::init()
{
    // Create temporary directory for test files
    QTemporaryDir tempDir;
    tempDir.setAutoRemove(false); // Keep files for debugging if needed
    m_tempDir = tempDir.path();

    m_volunteerCsvPath = m_tempDir + "/test_volunteers.csv";
    m_eventCsvPath = m_tempDir + "/test_events.csv";

    qDebug() << "Test files will be created in:" << m_tempDir;
}

void TestVolunteerManagement::cleanup()
{
    // Clean up test files
    QFile::remove(m_volunteerCsvPath);
    QFile::remove(m_eventCsvPath);
    m_controller.reset();
}

// ============================================================================
// VOLUNTEER CLASS TESTS
// ============================================================================

void TestVolunteerManagement::testVolunteerCreation()
{
    Volunteer volunteer(1, "John Doe", "john@example.com");

    QCOMPARE(volunteer.getId(), 1);
    QCOMPARE(volunteer.getName(), QString("John Doe"));
    QCOMPARE(volunteer.getContactInfo(), QString("john@example.com"));
}

void TestVolunteerManagement::testVolunteerGettersSetters()
{
    Volunteer volunteer(1, "John Doe", "john@example.com");

    // Test setters
    volunteer.setName("Jane Smith");
    volunteer.setContactInfo("jane@example.com");

    // Test getters
    QCOMPARE(volunteer.getName(), QString("Jane Smith"));
    QCOMPARE(volunteer.getContactInfo(), QString("jane@example.com"));
    QCOMPARE(volunteer.getId(), 1); // ID should remain unchanged
}

// ============================================================================
// EVENT CLASS TESTS
// ============================================================================

void TestVolunteerManagement::testEventCreation()
{
    QDate testDate(2024, 6, 15);
    Event event(1, "Community Cleanup", testDate, "Central Park");

    QCOMPARE(event.getId(), 1);
    QCOMPARE(event.getTitle(), QString("Community Cleanup"));
    QCOMPARE(event.getDate(), testDate);
    QCOMPARE(event.getLocation(), QString("Central Park"));
    QVERIFY(event.getVolunteerIds().isEmpty());
}

void TestVolunteerManagement::testEventGettersSetters()
{
    QDate originalDate(2024, 6, 15);
    QDate newDate(2024, 7, 20);
    Event event(1, "Community Cleanup", originalDate, "Central Park");

    // Test setters
    event.setTitle("Beach Cleanup");
    event.setDate(newDate);
    event.setLocation("Sunset Beach");

    // Test getters
    QCOMPARE(event.getTitle(), QString("Beach Cleanup"));
    QCOMPARE(event.getDate(), newDate);
    QCOMPARE(event.getLocation(), QString("Sunset Beach"));
    QCOMPARE(event.getId(), 1); // ID should remain unchanged
}

void TestVolunteerManagement::testEventVolunteerManagement()
{
    QDate testDate(2024, 6, 15);
    Event event(1, "Community Cleanup", testDate, "Central Park");

    // Test adding volunteers
    event.addVolunteer(101);
    event.addVolunteer(102);
    event.addVolunteer(103);

    QList<int> volunteerIds = event.getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 3);
    QVERIFY(volunteerIds.contains(101));
    QVERIFY(volunteerIds.contains(102));
    QVERIFY(volunteerIds.contains(103));

    // Test adding duplicate volunteer (should not duplicate)
    event.addVolunteer(101);
    QCOMPARE(event.getVolunteerIds().size(), 3);

    // Test removing volunteer
    event.removeVolunteer(102);
    volunteerIds = event.getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 2);
    QVERIFY(!volunteerIds.contains(102));
    QVERIFY(volunteerIds.contains(101));
    QVERIFY(volunteerIds.contains(103));

    // Test removing non-existent volunteer
    event.removeVolunteer(999);
    QCOMPARE(event.getVolunteerIds().size(), 2); // Should remain unchanged
}

// ============================================================================
// CSV VOLUNTEER REPOSITORY TESTS
// ============================================================================

void TestVolunteerManagement::testVolunteerRepositoryAdd()
{
    CSVVolunteerRepository repo(m_volunteerCsvPath);

    Volunteer volunteer1(1, "Alice Johnson", "alice@example.com");
    Volunteer volunteer2(2, "Bob Smith", "bob@example.com");

    repo.add(volunteer1);
    repo.add(volunteer2);

    std::vector<Volunteer> volunteers = repo.getAll();
    QCOMPARE(volunteers.size(), 2);

    // Find volunteers by ID
    auto alice = std::find_if(volunteers.begin(), volunteers.end(),
                             [](const Volunteer& v) { return v.getId() == 1; });
    auto bob = std::find_if(volunteers.begin(), volunteers.end(),
                           [](const Volunteer& v) { return v.getId() == 2; });

    QVERIFY(alice != volunteers.end());
    QVERIFY(bob != volunteers.end());
    QCOMPARE(alice->getName(), QString("Alice Johnson"));
    QCOMPARE(bob->getName(), QString("Bob Smith"));
}

void TestVolunteerManagement::testVolunteerRepositoryRemove()
{
    CSVVolunteerRepository repo(m_volunteerCsvPath);

    Volunteer volunteer1(1, "Alice Johnson", "alice@example.com");
    Volunteer volunteer2(2, "Bob Smith", "bob@example.com");

    repo.add(volunteer1);
    repo.add(volunteer2);

    QCOMPARE(repo.getAll().size(), 2);

    repo.remove(1);
    std::vector<Volunteer> volunteers = repo.getAll();
    QCOMPARE(volunteers.size(), 1);
    QCOMPARE(volunteers[0].getId(), 2);
    QCOMPARE(volunteers[0].getName(), QString("Bob Smith"));
}

void TestVolunteerManagement::testVolunteerRepositoryUpdate()
{
    CSVVolunteerRepository repo(m_volunteerCsvPath);

    Volunteer volunteer(1, "Alice Johnson", "alice@example.com");
    repo.add(volunteer);

    Volunteer updatedVolunteer(1, "Alice Smith", "alice.smith@example.com");
    repo.update(updatedVolunteer);

    std::vector<Volunteer> volunteers = repo.getAll();
    QCOMPARE(volunteers.size(), 1);
    QCOMPARE(volunteers[0].getName(), QString("Alice Smith"));
    QCOMPARE(volunteers[0].getContactInfo(), QString("alice.smith@example.com"));
}

void TestVolunteerManagement::testVolunteerRepositoryGetAll()
{
    CSVVolunteerRepository repo(m_volunteerCsvPath);

    // Test empty repository
    QVERIFY(repo.getAll().empty());

    // Add volunteers and test getAll
    repo.add(Volunteer(1, "Alice", "alice@example.com"));
    repo.add(Volunteer(2, "Bob", "bob@example.com"));
    repo.add(Volunteer(3, "Charlie", "charlie@example.com"));

    std::vector<Volunteer> volunteers = repo.getAll();
    QCOMPARE(volunteers.size(), 3);
}

void TestVolunteerManagement::testVolunteerRepositoryPersistence()
{
    // Create and populate repository
    {
        CSVVolunteerRepository repo(m_volunteerCsvPath);
        repo.add(Volunteer(1, "Alice Johnson", "alice@example.com"));
        repo.add(Volunteer(2, "Bob Smith", "bob@example.com"));
    } // Repository goes out of scope, should save to file

    // Create new repository instance and verify data persistence
    CSVVolunteerRepository newRepo(m_volunteerCsvPath);
    std::vector<Volunteer> volunteers = newRepo.getAll();

    QCOMPARE(volunteers.size(), 2);

    auto alice = std::find_if(volunteers.begin(), volunteers.end(),
                             [](const Volunteer& v) { return v.getId() == 1; });
    auto bob = std::find_if(volunteers.begin(), volunteers.end(),
                           [](const Volunteer& v) { return v.getId() == 2; });

    QVERIFY(alice != volunteers.end());
    QVERIFY(bob != volunteers.end());
    QCOMPARE(alice->getName(), QString("Alice Johnson"));
    QCOMPARE(bob->getName(), QString("Bob Smith"));
}

void TestVolunteerManagement::testVolunteerRepositoryDuplicateHandling()
{
    CSVVolunteerRepository repo(m_volunteerCsvPath);

    Volunteer volunteer1(1, "Alice Johnson", "alice@example.com");
    Volunteer volunteer2(1, "Alice Smith", "alice.smith@example.com"); // Same ID

    repo.add(volunteer1);
    repo.add(volunteer2); // Should not add due to duplicate ID

    std::vector<Volunteer> volunteers = repo.getAll();
    QCOMPARE(volunteers.size(), 1);
    QCOMPARE(volunteers[0].getName(), QString("Alice Johnson")); // Original should remain
}

// ============================================================================
// CSV EVENT REPOSITORY TESTS
// ============================================================================

void TestVolunteerManagement::testEventRepositoryAdd()
{
    CSVEventRepository repo(m_eventCsvPath);

    QDate date1(2024, 6, 15);
    QDate date2(2024, 7, 20);
    Event event1(1, "Community Cleanup", date1, "Central Park");
    Event event2(2, "Food Drive", date2, "Community Center");

    repo.add(event1);
    repo.add(event2);

    std::vector<Event> events = repo.getAll();
    QCOMPARE(events.size(), 2);

    auto cleanup = std::find_if(events.begin(), events.end(),
                               [](const Event& e) { return e.getId() == 1; });
    auto foodDrive = std::find_if(events.begin(), events.end(),
                                 [](const Event& e) { return e.getId() == 2; });

    QVERIFY(cleanup != events.end());
    QVERIFY(foodDrive != events.end());
    QCOMPARE(cleanup->getTitle(), QString("Community Cleanup"));
    QCOMPARE(foodDrive->getTitle(), QString("Food Drive"));
}

void TestVolunteerManagement::testEventRepositoryRemove()
{
    CSVEventRepository repo(m_eventCsvPath);

    QDate date1(2024, 6, 15);
    QDate date2(2024, 7, 20);
    Event event1(1, "Community Cleanup", date1, "Central Park");
    Event event2(2, "Food Drive", date2, "Community Center");

    repo.add(event1);
    repo.add(event2);

    QCOMPARE(repo.getAll().size(), 2);

    repo.remove(1);
    std::vector<Event> events = repo.getAll();
    QCOMPARE(events.size(), 1);
    QCOMPARE(events[0].getId(), 2);
    QCOMPARE(events[0].getTitle(), QString("Food Drive"));
}

void TestVolunteerManagement::testEventRepositoryUpdate()
{
    CSVEventRepository repo(m_eventCsvPath);

    QDate originalDate(2024, 6, 15);
    QDate newDate(2024, 6, 20);
    Event event(1, "Community Cleanup", originalDate, "Central Park");
    repo.add(event);

    Event updatedEvent(1, "Beach Cleanup", newDate, "Sunset Beach");
    repo.update(updatedEvent);

    std::vector<Event> events = repo.getAll();
    QCOMPARE(events.size(), 1);
    QCOMPARE(events[0].getTitle(), QString("Beach Cleanup"));
    QCOMPARE(events[0].getDate(), newDate);
    QCOMPARE(events[0].getLocation(), QString("Sunset Beach"));
}

void TestVolunteerManagement::testEventRepositoryGetAll()
{
    CSVEventRepository repo(m_eventCsvPath);

    // Test empty repository
    QVERIFY(repo.getAll().empty());

    // Add events and test getAll
    QDate date(2024, 6, 15);
    repo.add(Event(1, "Event 1", date, "Location 1"));
    repo.add(Event(2, "Event 2", date, "Location 2"));
    repo.add(Event(3, "Event 3", date, "Location 3"));

    std::vector<Event> events = repo.getAll();
    QCOMPARE(events.size(), 3);
}

void TestVolunteerManagement::testEventRepositoryPersistence()
{
    QDate testDate(2024, 6, 15);

    // Create and populate repository
    {
        CSVEventRepository repo(m_eventCsvPath);
        repo.add(Event(1, "Community Cleanup", testDate, "Central Park"));
        repo.add(Event(2, "Food Drive", testDate, "Community Center"));
    } // Repository goes out of scope, should save to file

    // Create new repository instance and verify data persistence
    CSVEventRepository newRepo(m_eventCsvPath);
    std::vector<Event> events = newRepo.getAll();

    QCOMPARE(events.size(), 2);

    auto cleanup = std::find_if(events.begin(), events.end(),
                               [](const Event& e) { return e.getId() == 1; });
    auto foodDrive = std::find_if(events.begin(), events.end(),
                                 [](const Event& e) { return e.getId() == 2; });

    QVERIFY(cleanup != events.end());
    QVERIFY(foodDrive != events.end());
    QCOMPARE(cleanup->getTitle(), QString("Community Cleanup"));
    QCOMPARE(foodDrive->getTitle(), QString("Food Drive"));
}

void TestVolunteerManagement::testEventRepositoryVolunteerAssociations()
{
    CSVEventRepository repo(m_eventCsvPath);

    QDate testDate(2024, 6, 15);
    Event event(1, "Community Cleanup", testDate, "Central Park");
    event.addVolunteer(101);
    event.addVolunteer(102);
    event.addVolunteer(103);

    repo.add(event);

    // Test persistence of volunteer associations
    std::vector<Event> events = repo.getAll();
    QCOMPARE(events.size(), 1);

    QList<int> volunteerIds = events[0].getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 3);
    QVERIFY(volunteerIds.contains(101));
    QVERIFY(volunteerIds.contains(102));
    QVERIFY(volunteerIds.contains(103));

    // Test updating volunteer associations
    Event updatedEvent = events[0];
    updatedEvent.removeVolunteer(102);
    updatedEvent.addVolunteer(104);

    repo.update(updatedEvent);

    events = repo.getAll();
    volunteerIds = events[0].getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 3);
    QVERIFY(volunteerIds.contains(101));
    QVERIFY(!volunteerIds.contains(102));
    QVERIFY(volunteerIds.contains(103));
    QVERIFY(volunteerIds.contains(104));
}

// ============================================================================
// CONTROLLER TESTS
// ============================================================================

void TestVolunteerManagement::testControllerVolunteerOperations()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Test adding volunteers
    Volunteer volunteer1(1, "Alice Johnson", "alice@example.com");
    Volunteer volunteer2(2, "Bob Smith", "bob@example.com");

    m_controller->addVolunteer(volunteer1);
    m_controller->addVolunteer(volunteer2);

    std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
    QCOMPARE(volunteers.size(), 2);

    // Test updating volunteer
    Volunteer updatedVolunteer(1, "Alice Smith", "alice.smith@example.com");
    m_controller->updateVolunteer(1, updatedVolunteer);

    volunteers = m_controller->getAllVolunteers();
    auto alice = std::find_if(volunteers.begin(), volunteers.end(),
                             [](const Volunteer& v) { return v.getId() == 1; });
    QVERIFY(alice != volunteers.end());
    QCOMPARE(alice->getName(), QString("Alice Smith"));

    // Test removing volunteer
    m_controller->removeVolunteer(2);
    volunteers = m_controller->getAllVolunteers();
    QCOMPARE(volunteers.size(), 1);
    QCOMPARE(volunteers[0].getId(), 1);
}

void TestVolunteerManagement::testControllerEventOperations()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    QDate date1(2024, 6, 15);
    QDate date2(2024, 7, 20);

    // Test adding events
    Event event1(1, "Community Cleanup", date1, "Central Park");
    Event event2(2, "Food Drive", date2, "Community Center");

    m_controller->addEvent(event1);
    m_controller->addEvent(event2);

    std::vector<Event> events = m_controller->getAllEvents();
    QCOMPARE(events.size(), 2);

    // Test updating event
    QDate newDate(2024, 6, 20);
    Event updatedEvent(1, "Beach Cleanup", newDate, "Sunset Beach");
    m_controller->updateEvent(1, updatedEvent);

    events = m_controller->getAllEvents();
    auto cleanup = std::find_if(events.begin(), events.end(),
                               [](const Event& e) { return e.getId() == 1; });
    QVERIFY(cleanup != events.end());
    QCOMPARE(cleanup->getTitle(), QString("Beach Cleanup"));
    QCOMPARE(cleanup->getLocation(), QString("Sunset Beach"));

    // Test removing event
    m_controller->removeEvent(2);
    events = m_controller->getAllEvents();
    QCOMPARE(events.size(), 1);
    QCOMPARE(events[0].getId(), 1);
}

void TestVolunteerManagement::testControllerVolunteerEventAssociations()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Create test data
    m_controller->addVolunteer(Volunteer(1, "Alice", "alice@example.com"));
    m_controller->addVolunteer(Volunteer(2, "Bob", "bob@example.com"));

    QDate testDate(2024, 6, 15);
    m_controller->addEvent(Event(100, "Community Cleanup", testDate, "Central Park"));

    // Test adding volunteers to event
    m_controller->addVolunteerToEvent(1, 100);
    m_controller->addVolunteerToEvent(2, 100);

    std::vector<Event> events = m_controller->getAllEvents();
    QCOMPARE(events.size(), 1);

    QList<int> volunteerIds = events[0].getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 2);
    QVERIFY(volunteerIds.contains(1));
    QVERIFY(volunteerIds.contains(2));

    // Test removing volunteer from event
    m_controller->removeVolunteerFromEvent(1, 100);

    events = m_controller->getAllEvents();
    volunteerIds = events[0].getVolunteerIds();
    QCOMPARE(volunteerIds.size(), 1);
    QVERIFY(!volunteerIds.contains(1));
    QVERIFY(volunteerIds.contains(2));
}

void TestVolunteerManagement::testControllerUndoRedo()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Test undo/redo for volunteer operations
    m_controller->addVolunteer(Volunteer(1, "Alice", "alice@example.com"));
    QCOMPARE(m_controller->getAllVolunteers().size(), 1);

    m_controller->undo();
    QCOMPARE(m_controller->getAllVolunteers().size(), 0);

    m_controller->redo();
    QCOMPARE(m_controller->getAllVolunteers().size(), 1);

    // Test undo/redo for event operations
    QDate testDate(2024, 6, 15);
    m_controller->addEvent(Event(100, "Test Event", testDate, "Test Location"));
    QCOMPARE(m_controller->getAllEvents().size(), 1);

    m_controller->undo();
    QCOMPARE(m_controller->getAllEvents().size(), 0);

    m_controller->redo();
    QCOMPARE(m_controller->getAllEvents().size(), 1);
}

// NEW: Test filtering events by date
void TestVolunteerManagement::testControllerFilterEventsByDate()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Add some test events
    QDate date1(2024, 1, 10);
    QDate date2(2024, 1, 15);
    QDate date3(2024, 2, 10);

    m_controller->addEvent(Event(1, "Event A", date1, "Location X"));
    m_controller->addEvent(Event(2, "Event B", date2, "Location Y"));
    m_controller->addEvent(Event(3, "Event C", date1, "Location Z"));
    m_controller->addEvent(Event(4, "Event D", date3, "Location X"));

    // Test filtering by a specific date
    std::vector<Event> filteredEvents = m_controller->filterEventsByDate("2024-01-10");
    QCOMPARE(filteredEvents.size(), 2);
    QCOMPARE(filteredEvents[0].getId(), 1);
    QCOMPARE(filteredEvents[1].getId(), 3);

    // Test filtering by another date
    filteredEvents = m_controller->filterEventsByDate("2024-01-15");
    QCOMPARE(filteredEvents.size(), 1);
    QCOMPARE(filteredEvents[0].getId(), 2);

    // Test filtering by a date with no matching events
    filteredEvents = m_controller->filterEventsByDate("2024-03-01");
    QCOMPARE(filteredEvents.size(), 0);

    // Test filtering with an empty date string (should return all events)
    filteredEvents = m_controller->filterEventsByDate("");
    QCOMPARE(filteredEvents.size(), 4);
}

// NEW: Test filtering events by location
void TestVolunteerManagement::testControllerFilterEventsByLocation()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Add some test events
    QDate date(2024, 1, 1);
    m_controller->addEvent(Event(1, "Event Alpha", date, "New York"));
    m_controller->addEvent(Event(2, "Event Beta", date, "Los Angeles"));
    m_controller->addEvent(Event(3, "Event Gamma", date, "New Orleans"));
    m_controller->addEvent(Event(4, "Event Delta", date, "San Francisco"));

    // Test filtering by a specific location (case-insensitive)
    std::vector<Event> filteredEvents = m_controller->filterEventsByLocation("new");
    QCOMPARE(filteredEvents.size(), 2);
    // The order might vary depending on the underlying repository's getAll()
    // We'll check for presence of IDs
    bool found1 = false;
    bool found3 = false;
    for (const auto& event : filteredEvents) {
        if (event.getId() == 1) found1 = true;
        if (event.getId() == 3) found3 = true;
    }
    QVERIFY(found1);
    QVERIFY(found3);

    // Test filtering by another location
    filteredEvents = m_controller->filterEventsByLocation("Angeles");
    QCOMPARE(filteredEvents.size(), 1);
    QCOMPARE(filteredEvents[0].getId(), 2);

    // Test filtering by a location with no matching events
    filteredEvents = m_controller->filterEventsByLocation("Chicago");
    QCOMPARE(filteredEvents.size(), 0);

    // Test filtering with an empty location string (should return all events)
    filteredEvents = m_controller->filterEventsByLocation("");
    QCOMPARE(filteredEvents.size(), 4);
}

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

void TestVolunteerManagement::testCompleteWorkflow()
{
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
    auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
    m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));

    // Step 1: Add volunteers
    m_controller->addVolunteer(Volunteer(1, "Alice Johnson", "alice@example.com"));
    m_controller->addVolunteer(Volunteer(2, "Bob Smith", "bob@example.com"));
    m_controller->addVolunteer(Volunteer(3, "Charlie Brown", "charlie@example.com"));

    // Step 2: Add events
    QDate cleanupDate(2024, 6, 15);
    QDate foodDriveDate(2024, 7, 20);
    m_controller->addEvent(Event(100, "Community Cleanup", cleanupDate, "Central Park"));
    m_controller->addEvent(Event(101, "Food Drive", foodDriveDate, "Community Center"));

    // Step 3: Assign volunteers to events
    m_controller->addVolunteerToEvent(1, 100); // Alice to Cleanup
    m_controller->addVolunteerToEvent(2, 100); // Bob to Cleanup
    m_controller->addVolunteerToEvent(2, 101); // Bob to Food Drive
    m_controller->addVolunteerToEvent(3, 101); // Charlie to Food Drive

    // Verify final state
    std::vector<Volunteer> volunteers = m_controller->getAllVolunteers();
    std::vector<Event> events = m_controller->getAllEvents();

    QCOMPARE(volunteers.size(), 3);
    QCOMPARE(events.size(), 2);

    // Check volunteer assignments
    auto cleanup = std::find_if(events.begin(), events.end(),
                               [](const Event& e) { return e.getId() == 100; });
    auto foodDrive = std::find_if(events.begin(), events.end(),
                                 [](const Event& e) { return e.getId() == 101; });

    QVERIFY(cleanup != events.end());
    QVERIFY(foodDrive != events.end());

    QList<int> cleanupVolunteers = cleanup->getVolunteerIds();
    QList<int> foodDriveVolunteers = foodDrive->getVolunteerIds();

    QCOMPARE(cleanupVolunteers.size(), 2);
    QVERIFY(cleanupVolunteers.contains(1)); // Alice
    QVERIFY(cleanupVolunteers.contains(2)); // Bob

    QCOMPARE(foodDriveVolunteers.size(), 2);
    QVERIFY(foodDriveVolunteers.contains(2)); // Bob
    QVERIFY(foodDriveVolunteers.contains(3)); // Charlie
}

void TestVolunteerManagement::testDataPersistenceAcrossApplicationRestart()
{
    // Simulate first application session
    {
        auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
        auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
        Controller controller(std::move(volunteerRepo), std::move(eventRepo));

        // Add test data
        controller.addVolunteer(Volunteer(1, "Alice", "alice@example.com"));
        controller.addVolunteer(Volunteer(2, "Bob", "bob@example.com"));

        QDate testDate(2024, 6, 15);
        controller.addEvent(Event(100, "Test Event", testDate, "Test Location"));
        controller.addVolunteerToEvent(1, 100);
        controller.addVolunteerToEvent(2, 100);
    } // Controller and repositories go out of scope, data should be saved

    // Simulate second application session (restart)
    {
        auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
        auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
        Controller controller(std::move(volunteerRepo), std::move(eventRepo));

        // Verify data persistence
        std::vector<Volunteer> volunteers = controller.getAllVolunteers();
        std::vector<Event> events = controller.getAllEvents();

        QCOMPARE(volunteers.size(), 2);
        QCOMPARE(events.size(), 1);

        // Verify volunteer data
        auto alice = std::find_if(volunteers.begin(), volunteers.end(),
                                 [](const Volunteer& v) { return v.getId() == 1; });
        auto bob = std::find_if(volunteers.begin(), volunteers.end(),
                               [](const Volunteer& v) { return v.getId() == 2; });

        QVERIFY(alice != volunteers.end());
        QVERIFY(bob != volunteers.end());
        QCOMPARE(alice->getName(), QString("Alice"));
        QCOMPARE(bob->getName(), QString("Bob"));

        // Verify event data and volunteer associations
        QCOMPARE(events[0].getId(), 100);
        QCOMPARE(events[0].getTitle(), QString("Test Event"));

        QList<int> volunteerIds = events[0].getVolunteerIds();
        QCOMPARE(volunteerIds.size(), 2);
        QVERIFY(volunteerIds.contains(1));
        QVERIFY(volunteerIds.contains(2));
    }
}

// ============================================================================
// HELPER METHODS
// ============================================================================

void TestVolunteerManagement::createTestVolunteers()
{
    if (!m_controller) {
        auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
        auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
        m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));
    }

    m_controller->addVolunteer(Volunteer(1, "Alice Johnson", "alice@example.com"));
    m_controller->addVolunteer(Volunteer(2, "Bob Smith", "bob@example.com"));
    m_controller->addVolunteer(Volunteer(3, "Charlie Brown", "charlie@example.com"));
    m_controller->addVolunteer(Volunteer(4, "Diana Prince", "diana@example.com"));
    m_controller->addVolunteer(Volunteer(5, "Edward Norton", "edward@example.com"));
}

void TestVolunteerManagement::createTestEvents()
{
    if (!m_controller) {
        auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(m_volunteerCsvPath);
        auto eventRepo = std::make_unique<CSVEventRepository>(m_eventCsvPath);
        m_controller = std::make_unique<Controller>(std::move(volunteerRepo), std::move(eventRepo));
    }

    QDate date1(2024, 6, 15);
    QDate date2(2024, 7, 20);
    QDate date3(2024, 8, 10);

    m_controller->addEvent(Event(100, "Community Cleanup", date1, "Central Park"));
    m_controller->addEvent(Event(101, "Food Drive", date2, "Community Center"));
    m_controller->addEvent(Event(102, "Book Reading", date3, "Public Library"));
}

void TestVolunteerManagement::verifyVolunteerInFile(int id, const QString& name, const QString& contact)
{
    QFile file(m_volunteerCsvPath);
    QVERIFY(file.exists());
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();

    // Check if volunteer data exists in file
    QString expectedLine = QString("%1,%2,%3").arg(id).arg(name).arg(contact);
    QVERIFY2(content.contains(expectedLine),
             QString("Expected volunteer data not found in file: %1").arg(expectedLine).toLocal8Bit()); // Corrected toLocal8Bit()
}

void TestVolunteerManagement::verifyEventInFile(int id, const QString& title, const QDate& date, const QString& location)
{
    QFile file(m_eventCsvPath);
    QVERIFY(file.exists());
    QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));

    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();

    // Check if event data exists in file
    QString dateString = date.toString("yyyy-MM-dd");
    QString expectedData = QString("%1,%2,%3,%4").arg(id).arg(title).arg(dateString).arg(location);
    QVERIFY2(content.contains(QString::number(id)) &&
             content.contains(title) &&
             content.contains(dateString) &&
             content.contains(location),
             QString("Expected event data not found in file: %1").arg(expectedData).toLocal8Bit());
}

// ============================================================================
// MAIN FUNCTION AND MOC INCLUDE
// ============================================================================

QTEST_MAIN(TestVolunteerManagement)
#include "tests.moc"
