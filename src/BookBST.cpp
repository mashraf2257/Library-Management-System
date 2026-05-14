#include "bookbst.h"

// Constructor
BookBST::BookBST() {
    root = nullptr;
}

// Destructor
BookBST::~BookBST() {
    // Simple post-order deletion
    while (root != nullptr) {
        root = removeBookHelper(root, root->BookID);
    }
}

// Helper to find minimum node (used in deletion)
Book* BookBST::findMin(Book* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Add a new book
Book* BookBST::addBookHelper(Book* node, Book* newBook) {
    if (node == nullptr) return newBook;

    if (newBook->BookID < node->BookID) {
        node->left = addBookHelper(node->left, newBook);
    } else if (newBook->BookID > node->BookID) {
        node->right = addBookHelper(node->right, newBook);
    } else {
        cout << "BookID already exists!\n";
    }

    return node;
}

bool BookBST::addBook(int id, const string& title, const string& author, const string& genre, int copies) {
    if (searchBookHelper(root, id) != nullptr) {
        return false; // BookID already exists
    }

    Book* newBook = new Book;
    newBook->BookID = id;
    newBook->Title = title;
    newBook->Author = author;
    newBook->Genre = genre;
    newBook->CopiesAvailable = copies;
    newBook->left = newBook->right = nullptr;

    root = addBookHelper(root, newBook);
    return true;
}

// Search for a book by ID
Book* BookBST::searchBookHelper(Book* node, int bookID) {
    if (node == nullptr || node->BookID == bookID) return node;

    if (bookID < node->BookID) return searchBookHelper(node->left, bookID);
    else return searchBookHelper(node->right, bookID);
}

// Search book is handled by getBook
// Get Book pointer by ID (for internal use by other classes like BorrowDLL)
Book* BookBST::getBook(int bookID) {
    return searchBookHelper(root, bookID);
}

// Remove a book by ID
Book* BookBST::removeBookHelper(Book* node, int bookID) {
    if (node == nullptr) return nullptr;

    if (bookID < node->BookID) {
        node->left = removeBookHelper(node->left, bookID);
    } else if (bookID > node->BookID) {
        node->right = removeBookHelper(node->right, bookID);
    } else {
        // Node found
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        } else if (node->left == nullptr) {
            Book* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Book* temp = node->left;
            delete node;
            return temp;
        } else {
            Book* temp = findMin(node->right);
            node->BookID = temp->BookID;
            node->Title = temp->Title;
            node->Author = temp->Author;
            node->Genre = temp->Genre;
            node->CopiesAvailable = temp->CopiesAvailable;
            node->right = removeBookHelper(node->right, temp->BookID);
        }
    }
    return node;
}

bool BookBST::removeBook(int id) {
    if (searchBookHelper(root, id) == nullptr) {
        return false;
    }
    root = removeBookHelper(root, id);
    return true;
}
// Collect books in sorted order (In-Order Traversal)
void BookBST::collectInOrder(Book* node, std::vector<Book>& books) {
    if (node == nullptr) return;

    collectInOrder(node->left, books);
    books.push_back(*node);
    collectInOrder(node->right, books);
}

std::vector<Book> BookBST::getAllBooks() {
    std::vector<Book> books;
    collectInOrder(root, books);
    return books;
}
