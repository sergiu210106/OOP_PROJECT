# Volunteer Management System

This project is a simple Volunteer Management System built in C++ using Qt. It allows for the management of volunteers and events, including associating volunteers with specific events. The system uses CSV files for data persistence.

## Features

* Manage volunteers (add, view, update, remove).
* Manage events (add, view, update, remove).
* Associate volunteers with events.
* Filter events by date.
* Filter events by location.
* Undo/redo functionality for operations.
* Data persistence using CSV files.

## Prerequisites

* C++ compiler (e.g., g++)
* CMake (for building the project)
* Qt 6 (for the Qt framework)

## Building the Project

1.  Clone the repository.
2.  Navigate to the project directory.
3.  Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
4.  Run CMake to generate the build files:
    ```bash
    cmake ..
    ```
5.  Build the project:
    ```bash
    cmake --build .
    ```

## Running the Tests

To run the unit tests (which you've been working with):

1.  Navigate to the `build` directory.
2.  Run the test executable (the name might vary slightly depending on your system, but it's likely something like `volunteer_tests` or `volunteer_tests.exe`):
    ```bash
    ./volunteer_tests # On Linux/macOS
    volunteer_tests.exe # On Windows
    ```

## Project Structure

* `Volunteer.h`: Defines the `Volunteer` class.
* `Event.h`: Defines the `Event` class.
* `CSVVolunteerRepository.h/cpp`: Implements the volunteer repository using CSV files.
* `CSVEventRepository.h/cpp`: Implements the event repository using CSV files.
* `Controller.h/cpp`: Contains the business logic and manages the repositories.
* `tests.cpp`: Contains the unit tests for the system components.
* `CMakeLists.txt`: CMake build configuration file.
* `README.md`: This file.

## Contributing

Contributions are welcome! Please feel free to submit pull requests.

## License

[Your License Here (e.g., MIT License)]
