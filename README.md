# 📚 Library Management System

A C++ backend web application that provides a complete library management solution. The system handles member registration, book cataloguing, borrowing and return workflows, and reservation queuing — all implemented using custom data structures and exposed through a REST API served to a browser-based dashboard.

## ✨ Features

- **👥 Member Directory** - Register, view, and remove library members with auto-generated IDs
- **📖 Book Catalogue** - Add and remove books with copy availability tracking
- **🔄 Borrowing & Returns** - Issue books to members and log returns through a dedicated return stack
- **📋 Reservation Queue** - Enqueue member reservations and auto-assign books when processed
- **💾 Data Persistence** - All data is automatically saved and reloaded from `data/data.json`
- **🌐 Web Dashboard** - Single-page application with tab-based navigation for all operations

## Technology Stack

- **Backend**: C++17, cpp-httplib (single-header HTTP server)
- **Data Layer**: nlohmann/json (single-header JSON parser), file-based persistence
- **Frontend**: HTML5, CSS3, Vanilla JavaScript
- **Styling**: Tailwind CSS (CDN), Google Fonts (Inter), Material Symbols
- **Data Structures**: Custom implementations — SLL, BST, DLL, Stack, Queue

## 🏗️ Data Structures

| Structure | File | Purpose |
|---|---|---|
| Singly Linked List | `MemberSLL` | Stores and traverses member records |
| Binary Search Tree | `BookBST` | Organizes books ordered by Book ID |
| Doubly Linked List | `BorrowDLL` | Tracks full borrowing history |
| Stack | `ReturnStack` | Manages returned books (LIFO processing) |
| Queue | `ReservationQueue` | Manages book reservations (FIFO processing) |

## 📁 Project Structure

```
Library-Management-System/
├── data/
│   ├── data.json                  # Main database (members, books, history, reservations)
│   └── users.json                 # Login credentials
├── public/                        # Frontend served by the HTTP server
│   ├── index.html                 # Single-page application
│   ├── css/
│   │   └── style.css              # Layout overrides and custom component styles
│   └── js/
│       ├── script.js              # All frontend logic and API communication
│       └── tailwind-config.js     # Tailwind CSS design tokens and theme
├── src/                           # C++ backend source code
│   ├── main.cpp                   # HTTP server entry point and all API routes
│   ├── MemberSLL.h / .cpp         # Singly Linked List — member registry
│   ├── BookBST.h / .cpp           # Binary Search Tree — book catalogue
│   ├── BorrowDLL.h / .cpp         # Doubly Linked List — borrow history
│   ├── ReturnStack.h / .cpp       # Stack — return processing
│   ├── ReservationQueue.h / .cpp  # Queue — reservation management
│   ├── httplib.h                  # cpp-httplib (third-party, single-header)
│   └── json.hpp                   # nlohmann/json (third-party, single-header)
└── server.exe                     # Compiled server binary
```

## Getting Started

### 🛠️ Prerequisites

- Windows 10 or later
- GCC compiler via [MSYS2](https://www.msys2.org/) (MinGW-w64)
- A modern web browser (Chrome, Edge, Firefox)

### ⚙️🚀 Building & Running

1. **Clone the repository**
   ```bash
   git clone https://github.com/mashraf2257/Library-Management-System.git
   cd Library-Management-System
   ```

2. **Compile the server**
   ```bash
   g++ -o server.exe src/main.cpp src/BookBST.cpp src/BorrowDLL.cpp src/MemberSLL.cpp src/ReservationQueue.cpp src/ReturnStack.cpp -I src/ -std=c++17 -O2 -D_WIN32_WINNT=0x0A00 -lws2_32
   ```

3. **Run the server**
   ```bash
   .\server.exe
   ```

4. **Open the dashboard**
   ```
   http://localhost:8080
   ```

### 🔑 Login

Default credentials are stored in `data/users.json`:

```json
[
    {"username": "admin",   "password": "admin"},
    {"username": "mohamed", "password": "123"}
]
```

You can add or edit accounts directly in that file.

## Usage

- **Members Tab** — Add new members (auto-generated ID), view the full member list, or remove a member
- **Books Tab** — Register books by ID, title, author, genre and copy count; remove by ID; view the full catalogue
- **Transactions Tab** — Issue books to members, view a member's borrow history, and log returns
- **Reservations Tab** — Queue a reservation for a member/book pair, and process the queue to auto-assign available books

## Developer

- Mohamed Ashraf

## License

This project was developed as part of a university course and later improved for fun.
