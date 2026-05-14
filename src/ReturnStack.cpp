#include "ReturnStack.h"
#include "BookBST.h"

// Constructor
ReturnStack::ReturnStack() {
    top = nullptr;
}

// Destructor
ReturnStack::~ReturnStack() {
    while (!isEmpty()) {
        ReturnNode* temp = top;
        if (temp) {
            top = top->next;
            delete temp;
        }
    }
}

// Check if stack is empty
bool ReturnStack::isEmpty() {
    return top == nullptr;
}

// Push a returned book onto the stack
void ReturnStack::pushReturn(const string& memberID, int bookID) {
    ReturnNode* newNode = new ReturnNode;

    newNode->MemberID = memberID;
    newNode->BookID = bookID;

    newNode->next = top;
    top = newNode;
}

// Pop and process a return
bool ReturnStack::processReturn(BookBST& bookTree, string& outMemberID, int& outBookID) {
    if (isEmpty()) {
        return false;
    }

    ReturnNode* temp = top;
    top = top->next;

    outMemberID = temp->MemberID;
    outBookID = temp->BookID;

    // Increment book copies
    Book* book = bookTree.getBook(temp->BookID);
    if (book != nullptr) {
        book->CopiesAvailable++;
    }

    delete temp;
    return true;
}

std::vector<ReturnNode> ReturnStack::getAllReturns() {
    std::vector<ReturnNode> returns;
    ReturnNode* temp = top;
    while(temp != nullptr) {
        returns.push_back(*temp);
        temp = temp->next;
    }
    return returns;
}
