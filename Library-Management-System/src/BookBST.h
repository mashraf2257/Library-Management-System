#ifndef BOOKBST_H
#define BOOKBST_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Node for a book
struct Book {
    int BookID;
    string Title;
    string Author;
    string Genre;
    int CopiesAvailable;
    Book* left;
    Book* right;
};

// BST for managing books
class BookBST {
private:
    Book* root;

    // Helper functions
    Book* addBookHelper(Book* node, Book* newBook);
    Book* removeBookHelper(Book* node, int bookID);
    Book* findMin(Book* node);
    void collectInOrder(Book* node, std::vector<Book>& books);
    Book* searchBookHelper(Book* node, int bookID);

public:
    BookBST();  // Constructor
    ~BookBST(); // Destructor

    bool addBook(int id, const string& title, const string& author, const string& genre, int copies);
    bool removeBook(int id);
    std::vector<Book> getAllBooks();
    Book* getBook(int bookID);
};

#endif
