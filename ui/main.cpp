#include <QApplication> // Changed from QCoreApplication for GUI
#include <QDebug>
#include <memory> // For std::make_unique
#include <QDate>  // For QDate

// Project specific headers
#include "../Controller/Controller.h"
#include "../Repository/CSV/CSVVolunteerRepository.h"
#include "../Repository/CSV/CSVEventRepository.h"
#include "../Repository/JSON/JSONVolunteerRepository.h"
#include "../Repository/JSON/JSONEventRepository.h"
#include "mainwindow.h" // Include your MainWindow header
#include <QDir>

int main(int argc, char *argv[]) {
    // Initialize QApplication for GUI applications
    QApplication a(argc, argv);

    qDebug() << "Starting Volunteer Management Application (GUI)...";

    // Define filenames for the CSV repositories
    QString volunteerCsvFile = "volunteers.csv";
    QString eventCsvFile = "events.csv";

    // Create instances of the CSV repositories
    auto volunteerRepo = std::make_unique<CSVVolunteerRepository>(volunteerCsvFile);
    auto eventRepo = std::make_unique<CSVEventRepository>(eventCsvFile);

    // Create the Controller, passing in the repositories
    // The Controller takes ownership of the repository unique_ptr's
    // We'll keep the controller on the stack here, or you could manage it with a smart pointer
    // if MainWindow takes ownership or a shared pointer.
    // For simplicity, assuming MainWindow takes a reference or a raw pointer to a stack-allocated controller.
    Controller controller(std::move(volunteerRepo), std::move(eventRepo));

    qDebug() << "Controller and repositories initialized.";

    // Create and show the MainWindow
    // This assumes your MainWindow constructor can take a Controller reference or pointer.
    // Adjust if your MainWindow constructor is different.
    // Example: MainWindow mainWindow(&controller); or MainWindow mainWindow(controller);
    MainWindow mainWindow(&controller); // Pass controller by pointer
    mainWindow.setWindowTitle("Volunteer and Event Management"); // Optional: Set a window title
    mainWindow.show();

    qDebug() << "MainWindow shown. Starting event loop...";



    // Start the Qt event loop. This is essential for GUI applications.
    return a.exec();
}
