#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Controller* controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addVolunteer();
    void removeSelectedVolunteer();
    void updateSelectedVolunteer();
    void addEvent();
    void removeSelectedEvent();
    void updateSelectedEvent();
    void undo();
    void redo();
    void populateVolunteerInputFields();
    void populateEventInputFields();

    void assignVolunteerToEvent();
    void removeVolunteerFromEvent();
    void updateVolunteersInEventList();

private:
    Ui::MainWindow *ui;
    Controller* m_controller;

    void populateVolunteerList();
    void populateEventList();
    void clearVolunteerInputFields();
    void clearEventInputFields();
};

#endif // MAINWINDOW_H