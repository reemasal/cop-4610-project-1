#include "elevator.h"
#include "system.h"  // Required for printf and thread management

Lock *elevatorLock = new Lock("elevator lock");
Condition *elevatorArrived = new Condition("elevator arrived");

// Implementation of the Elevator function
void Elevator(int numFloors) {
    ElevatorThread elevator;
    elevator.numFloors = numFloors;
    elevator.currentFloor = 1; // Start at the first floor
    elevator.numPeopleIn = 0;
    elevator.isMoving = false;

    while (true) {
        // Simulate the elevator moving between floors
        if (!elevator.isMoving) {
            // Check if there are people waiting or if people need to be dropped off
            if (ThereAreWaitingPeople() || elevator.numPeopleIn > 0) {
                // Move to the next floor
                elevator.isMoving = true;
                for (int i = 0; i < 50; i++) {
                    currentThread->Yield(); // Simulate 50 ticks for moving between floors
                }
                elevator.currentFloor++; // Move up one floor
                if (elevator.currentFloor > elevator.numFloors) {
                    elevator.currentFloor = 1; // Wrap around if at top floor
                }

                printf("Elevator arrives on floor %d.\n", elevator.currentFloor);
                elevator.isMoving = false;

                // Handle picking up and dropping off passengers
                PickUpPassengers(&elevator);
                DropOffPassengers(&elevator);
            }
        }
        currentThread->Yield(); // Yield to other threads
    }
}

// Function to simulate a person requesting the elevator
void ArrivingGoingFromTo(int atFloor, int toFloor) {
    static int nextPersonId = 1; // Unique ID for each person
    PersonThread person;
    person.id = nextPersonId++;
    person.atFloor = atFloor;
    person.toFloor = toFloor;

    printf("Person %d wants to go from floor %d to floor %d.\n", person.id, atFloor, toFloor);

    // Notify elevator that someone is waiting
    NotifyElevator();

    // Wait for the elevator to arrive at the current floor
    WaitForElevatorArrival(person.atFloor, NULL);

    printf("Person %d got into the elevator on floor %d.\n", person.id, atFloor);

    // Wait for the elevator to reach the destination floor
    WaitForElevatorToReachFloor(person.toFloor, NULL);

    printf("Person %d got out of the elevator on floor %d.\n", person.id, toFloor);
}

// Function to notify the elevator that someone is waiting
void NotifyElevator() {
    elevatorLock->Acquire();  // Acquire the lock
    elevatorArrived->Signal(elevatorLock);  // Use the lock with Signal
    elevatorLock->Release();  // Release the lock
}

// Function to wait for the elevator to arrive at a specific floor
void WaitForElevatorArrival(int floor, ElevatorThread *elevator) {
    elevatorLock->Acquire();  // Acquire the lock
    while (elevator->currentFloor != floor) {
        elevatorArrived->Wait(elevatorLock);  // Use the lock with Wait
    }
    elevatorLock->Release();  // Release the lock
}

// Function to wait for the elevator to reach a destination floor
void WaitForElevatorToReachFloor(int floor, ElevatorThread *elevator) {
    elevatorLock->Acquire();  // Acquire the lock
    while (elevator->currentFloor != floor) {
        elevatorArrived->Wait(elevatorLock);  // Use the lock with Wait
    }
    elevatorLock->Release();  // Release the lock
}

// Function to pick up passengers from a floor
void PickUpPassengers(ElevatorThread *elevator) {
    elevatorLock->Acquire(); // Acquire the lock
    if (WaitingAtFloor(elevator->currentFloor)) {
        // Allow people to enter the elevator
        while (elevator->numPeopleIn < 5 && MorePeopleWaiting()) {
            printf("Person %d got into the elevator on floor %d.\n", GetNextWaitingPerson(), elevator->currentFloor);
            elevator->numPeopleIn++;
        }
    }
    elevatorLock->Release(); // Release the lock
}

// Function to drop off passengers at a floor
void DropOffPassengers(ElevatorThread *elevator) {
    elevatorLock->Acquire(); // Acquire the lock
    if (PeopleToDropOff(elevator->currentFloor)) {
        // Drop off passengers
        while (HasPeopleToDropOff()) {
            printf("Person %d got out of the elevator on floor %d.\n", GetNextPersonToDropOff(), elevator->currentFloor);
            elevator->numPeopleIn--;
        }
    }
    elevatorLock->Release(); // Release the lock
}

// Placeholder helper functions
bool ThereAreWaitingPeople() { return false; } // Replace with actual logic
bool WaitingAtFloor(int floor) { return false; } // Replace with actual logic
bool MorePeopleWaiting() { return false; } // Replace with actual logic
int GetNextWaitingPerson() { return 1; } // Replace with actual logic
bool PeopleToDropOff(int floor) { return false; } // Replace with actual logic
bool HasPeopleToDropOff() { return false; } // Replace with actual logic
int GetNextPersonToDropOff() { return 1; } // Replace with actual logic

