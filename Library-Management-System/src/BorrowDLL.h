#ifndef BORROWDLL_H
#define BORROWDLL_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Node for borrow record
struct BorrowNode {
    int BookID;
    string MemberID;    // Alphanumeric e.g. "AB1234"
    string BorrowDate;
    string ReturnDate;
    BorrowNode* prev;
    BorrowNode* next;
};

// Forward declaration
class BookBST;

// Doubly Linked List for borrow history
class BorrowDLL {
private:
    BorrowNode* head;

public:
    BorrowDLL();  // Constructor
    ~BorrowDLL(); // Destructor

    bool addBorrowRecord(BookBST& bookTree, const string& memberID, int bookID, const string& borrowDate, const string& returnDate);
    std::vector<BorrowNode> getHistory(const string& memberID);
    std::vector<BorrowNode> getAllHistory();
    bool removeBorrowRecord(const string& memberID, int bookID);
};

#endif
