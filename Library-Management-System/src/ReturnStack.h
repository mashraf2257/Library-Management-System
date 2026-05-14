#ifndef RETURNSTACK_H
#define RETURNSTACK_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Node for return stack
struct ReturnNode {
    int BookID;
    string MemberID;    // Alphanumeric e.g. "AB1234"
    ReturnNode* next;
};

// Forward declaration
class BookBST;

// Stack for returned books
class ReturnStack {
private:
    ReturnNode* top;

public:
    ReturnStack();   // Constructor
    ~ReturnStack();  // Destructor

    void pushReturn(const string& memberID, int bookID);
    bool processReturn(BookBST& bookTree, string& outMemberID, int& outBookID);
    bool isEmpty();
    std::vector<ReturnNode> getAllReturns();
};

#endif
