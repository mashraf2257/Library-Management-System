#ifndef RESERVATIONQUEUE_H
#define RESERVATIONQUEUE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Node for reservation queue
struct ReservationNode {
    string MemberID;    // Alphanumeric e.g. "AB1234"
    int BookID;
    ReservationNode* next;
};

// Queue class
class ReservationQueue {
private:
    ReservationNode* front;
    ReservationNode* rear;

public:
    ReservationQueue();   // constructor
    ~ReservationQueue();  // destructor

    void enqueueReservation(const string& memberID, int bookID);
    bool dequeueReservation(string& outMemberID, int& outBookID);
    bool isEmpty();
    std::vector<ReservationNode> getAllReservations();
};

#endif
