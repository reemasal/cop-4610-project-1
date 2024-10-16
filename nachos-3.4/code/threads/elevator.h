#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "synch.h"  // For Lock and Condition classes

// Struct to represent a person requesting the elevator
struct PersonThread {
    int id;         // Unique identifier for the person
    int atFloor;    // The floor the person is currently on
    int toFloor;    // The destination floor the person wants to go to
};

// Struct to represent the elevator
struct ElevatorThread {
    int numFloors;     // Total number of floors in the building
    int currentFloor;  // The current floor of the elevator
    int numPeopleIn;   // The number of people inside the elevator
    bool isMoving;     // Whether the elevator is currently moving
};

// Function prototypes
void Elevator(int numFloors);
void ArrivingGoingFromTo(int atFloor, int toFloor);
void NotifyElevator();
void WaitForElevatorArrival(int floor, ElevatorThread *elevator);
void WaitForElevatorToReachFloor(int floor, ElevatorThread *elevator);
void PickUpPassengers(ElevatorThread *elevator);
void DropOffPassengers(ElevatorThread *elevator);

// Helper function prototypes
bool ThereAreWaitingPeople();
bool WaitingAtFloor(int floor);
bool MorePeopleWaiting();
int GetNextWaitingPerson();
bool PeopleToDropOff(int floor);
bool HasPeopleToDropOff();
int GetNextPersonToDropOff();

#endif

