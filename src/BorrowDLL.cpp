#include "BorrowDLL.h"
#include "BookBST.h"

// Constructor
BorrowDLL::BorrowDLL() {
    head = nullptr;
}

// Destructor
BorrowDLL::~BorrowDLL() {
    BorrowNode* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// Add a borrow record
bool BorrowDLL::addBorrowRecord(BookBST& bookTree, const string& memberID, int bookID, const string& borrowDate, const string& returnDate) {
    // Check if book exists and has copies
    Book* book = bookTree.getBook(bookID);
    if (book == nullptr || book->CopiesAvailable <= 0) {
        return false;
    }

    BorrowNode* newNode = new BorrowNode;
    newNode->MemberID = memberID;
    newNode->BookID = bookID;
    newNode->BorrowDate = borrowDate;
    newNode->ReturnDate = returnDate;

    // Only decrement copies if the book is NOT returned immediately (ReturnDate is empty)
    if (newNode->ReturnDate.empty()) {
        book->CopiesAvailable--;
    }

    newNode->prev = nullptr;
    newNode->next = nullptr;

    // Insert at end of DLL
    if (head == nullptr) {
        head = newNode;
    } else {
        BorrowNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }

    return true;
}

// View borrow history for a member
std::vector<BorrowNode> BorrowDLL::getHistory(const string& memberID) {
    std::vector<BorrowNode> history;
    BorrowNode* temp = head;
    while (temp != nullptr) {
        if (temp->MemberID == memberID) {
            history.push_back(*temp);
        }
        temp = temp->next;
    }
    return history;
}

std::vector<BorrowNode> BorrowDLL::getAllHistory() {
    std::vector<BorrowNode> history;
    BorrowNode* temp = head;
    while (temp != nullptr) {
        history.push_back(*temp);
        temp = temp->next;
    }
    return history;
}

bool BorrowDLL::removeBorrowRecord(const string& memberID, int bookID) {
    BorrowNode* temp = head;
    while (temp != nullptr) {
        if (temp->MemberID == memberID && temp->BookID == bookID && temp->ReturnDate.empty()) {
            if (temp->prev) {
                temp->prev->next = temp->next;
            } else {
                head = temp->next;
            }
            if (temp->next) {
                temp->next->prev = temp->prev;
            }
            delete temp;
            return true;
        }
        temp = temp->next;
    }
    return false;
}
