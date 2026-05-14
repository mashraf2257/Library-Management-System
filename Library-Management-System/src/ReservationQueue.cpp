#include "ReservationQueue.h"

// Constructor
ReservationQueue::ReservationQueue() {
    front = nullptr;
    rear = nullptr;
}

// Destructor
ReservationQueue::~ReservationQueue() {
    while (!isEmpty()) {
        ReservationNode* temp = front;
        front = front->next;
        delete temp;
    }
}


// Check if empty
bool ReservationQueue::isEmpty() {
    return front == nullptr;
}

// Add a reservation (enqueue)
void ReservationQueue::enqueueReservation(const string& memberID, int bookID) {
    ReservationNode* newNode = new ReservationNode;
    newNode->MemberID = memberID;
    newNode->BookID = bookID;
    newNode->next = nullptr;

    if (rear == nullptr) {  // empty queue
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

// Assign book to next person (dequeue)
bool ReservationQueue::dequeueReservation(string& outMemberID, int& outBookID) {
    if (isEmpty()) {
        return false;
    }

    ReservationNode* temp = front;
    front = front->next;

    if (front == nullptr) {  // queue became empty
        rear = nullptr;
    }

    outMemberID = temp->MemberID;
    outBookID = temp->BookID;

    delete temp;
    return true;
}

std::vector<ReservationNode> ReservationQueue::getAllReservations() {
    std::vector<ReservationNode> reservations;
    ReservationNode* temp = front;
    while(temp != nullptr) {
        reservations.push_back(*temp);
        temp = temp->next;
    }
    return reservations;
}
