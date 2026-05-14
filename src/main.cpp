#include <iostream>
#include <fstream>
#include "httplib.h"
#include "json.hpp"
#include "MemberSLL.h"
#include "BookBST.h"
#include "BorrowDLL.h"
#include "ReturnStack.h"
#include "ReservationQueue.h"

using namespace std;
using json = nlohmann::json;

MemberSLL memberList;
BookBST bookTree;
BorrowDLL borrowHistory;
ReturnStack returnStack;
ReservationQueue reservationQueue;

const string DATA_FILE  = "data/data.json";
const string USERS_FILE = "data/users.json";

void saveData() {
    json j;

    // Members
    j["members"] = json::array();
    for (const auto& m : memberList.getAllMembers()) {
        j["members"].push_back({
            {"ID", m.ID},
            {"Name", m.Name},
            {"Email", m.Email},
            {"Phone", m.Phone},
            {"Address", m.Address}
        });
    }

    // Books
    j["books"] = json::array();
    for (const auto& b : bookTree.getAllBooks()) {
        j["books"].push_back({
            {"BookID", b.BookID},
            {"Title", b.Title},
            {"Author", b.Author},
            {"Genre", b.Genre},
            {"CopiesAvailable", b.CopiesAvailable}
        });
    }

    // Borrow History
    j["borrowHistory"] = json::array();
    for (const auto& b : borrowHistory.getAllHistory()) {
        j["borrowHistory"].push_back({
            {"BookID", b.BookID},
            {"MemberID", b.MemberID},
            {"BorrowDate", b.BorrowDate},
            {"ReturnDate", b.ReturnDate}
        });
    }

    // Return Stack
    j["returnStack"] = json::array();
    vector<ReturnNode> returns = returnStack.getAllReturns();
    // Reverse because we extracted from top to bottom, but to restore we want bottom to top
    for (auto it = returns.rbegin(); it != returns.rend(); ++it) {
        j["returnStack"].push_back({
            {"BookID", it->BookID},
            {"MemberID", it->MemberID}
        });
    }

    // Reservations
    j["reservations"] = json::array();
    for (const auto& r : reservationQueue.getAllReservations()) {
        j["reservations"].push_back({
            {"MemberID", r.MemberID},
            {"BookID", r.BookID}
        });
    }

    ofstream o(DATA_FILE);
    o << j.dump(4);
}

void loadData() {
    ifstream i(DATA_FILE);
    if (!i.is_open()) return;

    json j;
    i >> j;

    if (j.contains("members")) {
        for (const auto& m : j["members"]) {
            memberList.addMember(m["ID"].get<string>(), m["Name"], m["Email"], m["Phone"], m["Address"]);
        }
    }

    if (j.contains("books")) {
        for (const auto& b : j["books"]) {
            bookTree.addBook(b["BookID"], b["Title"], b["Author"], b["Genre"], b["CopiesAvailable"]);
        }
    }

    if (j.contains("borrowHistory")) {
        for (const auto& b : j["borrowHistory"]) {
            borrowHistory.addBorrowRecord(bookTree, b["MemberID"].get<string>(), b["BookID"], b["BorrowDate"], b["ReturnDate"]);
        }
    }

    if (j.contains("returnStack")) {
        for (const auto& r : j["returnStack"]) {
            returnStack.pushReturn(r["MemberID"].get<string>(), r["BookID"]);
        }
    }

    if (j.contains("reservations")) {
        for (const auto& r : j["reservations"]) {
            reservationQueue.enqueueReservation(r["MemberID"].get<string>(), r["BookID"]);
        }
    }
}

int main() {
    loadData();

    httplib::Server svr;

    // Serve frontend from public/
    svr.set_mount_point("/", "./public");

    // Enable CORS
    auto cors = [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    };
    svr.Options(R"(.*)", [&](const httplib::Request& req, httplib::Response& res) { cors(req, res); });

    auto wrap = [&](auto handler) {
        return [handler, cors](const httplib::Request& req, httplib::Response& res) {
            cors(req, res);
            handler(req, res);
            saveData();
        };
    };

    // ----- Login -----
    svr.Post("/api/login", [](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        auto body = json::parse(req.body);
        string username = body["username"].get<string>();
        string password = body["password"].get<string>();

        ifstream f(USERS_FILE);
        if (!f.is_open()) {
            res.status = 500;
            res.set_content("{\"status\":\"error\"}", "application/json");
            return;
        }
        json users;
        f >> users;
        for (const auto& u : users) {
            if (u["username"].get<string>() == username &&
                u["password"].get<string>() == password) {
                res.set_content("{\"status\":\"success\"}", "application/json");
                return;
            }
        }
        res.status = 401;
        res.set_content("{\"status\":\"invalid\"}", "application/json");
    });

    // ----- Members -----
    svr.Get("/api/members", wrap([&](const httplib::Request& req, httplib::Response& res) {
        json j = json::array();
        for (const auto& m : memberList.getAllMembers()) {
            j.push_back({{"id", m.ID}, {"name", m.Name}, {"email", m.Email}, {"phone", m.Phone}, {"address", m.Address}});
        }
        res.set_content(j.dump(), "application/json");
    }));

    svr.Post("/api/members", wrap([&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        memberList.addMember(body["id"].get<string>(), body["name"], body["email"], body["phone"], body["address"]);
        res.set_content("{\"status\":\"success\"}", "application/json");
    }));

    // Delete by string ID in path
    svr.Delete(R"(/api/members/([A-Za-z0-9]+))", wrap([&](const httplib::Request& req, httplib::Response& res) {
        string id = req.matches[1];
        if (memberList.removeMember(id)) {
            res.set_content("{\"status\":\"success\"}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"status\":\"not found\"}", "application/json");
        }
    }));

    // ----- Books -----
    svr.Get("/api/books", wrap([&](const httplib::Request& req, httplib::Response& res) {
        json j = json::array();
        for (const auto& b : bookTree.getAllBooks()) {
            j.push_back({{"id", b.BookID}, {"title", b.Title}, {"author", b.Author}, {"genre", b.Genre}, {"copies", b.CopiesAvailable}});
        }
        res.set_content(j.dump(), "application/json");
    }));

    svr.Get(R"(/api/books/(\d+))", wrap([&](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        Book* b = bookTree.getBook(id);
        if (b) {
            json j = {{"id", b->BookID}, {"title", b->Title}, {"author", b->Author}, {"genre", b->Genre}, {"copies", b->CopiesAvailable}};
            res.set_content(j.dump(), "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"status\":\"not found\"}", "application/json");
        }
    }));

    svr.Post("/api/books", wrap([&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        if (bookTree.addBook(body["id"], body["title"], body["author"], body["genre"], body["copies"])) {
            res.set_content("{\"status\":\"success\"}", "application/json");
        } else {
            res.status = 400;
            res.set_content("{\"status\":\"exists\"}", "application/json");
        }
    }));

    svr.Delete(R"(/api/books/(\d+))", wrap([&](const httplib::Request& req, httplib::Response& res) {
        int id = stoi(req.matches[1]);
        if (bookTree.removeBook(id)) {
            res.set_content("{\"status\":\"success\"}", "application/json");
        } else {
            res.status = 404;
            res.set_content("{\"status\":\"not found\"}", "application/json");
        }
    }));

    // ----- Borrow -----
    svr.Post("/api/borrow", wrap([&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        if (borrowHistory.addBorrowRecord(bookTree, body["memberId"].get<string>(), body["bookId"], "Today", "")) {
            res.set_content("{\"status\":\"success\"}", "application/json");
        } else {
            res.status = 400;
            res.set_content("{\"status\":\"failed\"}", "application/json");
        }
    }));

    svr.Get(R"(/api/borrow/([A-Za-z0-9]+))", wrap([&](const httplib::Request& req, httplib::Response& res) {
        string id = req.matches[1];
        json j = json::array();
        for (const auto& b : borrowHistory.getHistory(id)) {
            j.push_back({{"bookId", b.BookID}, {"memberId", b.MemberID}, {"borrowDate", b.BorrowDate}, {"returnDate", b.ReturnDate}});
        }
        res.set_content(j.dump(), "application/json");
    }));

    // ----- Return -----
    svr.Post("/api/return", wrap([&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        returnStack.pushReturn(body["memberId"].get<string>(), body["bookId"]);
        res.set_content("{\"status\":\"success\"}", "application/json");
    }));

    svr.Post("/api/process-return", wrap([&](const httplib::Request& req, httplib::Response& res) {
        string mId;
        int bId;
        if (returnStack.processReturn(bookTree, mId, bId)) {
            borrowHistory.removeBorrowRecord(mId, bId);
            json j = {{"memberId", mId}, {"bookId", bId}};
            res.set_content(j.dump(), "application/json");
        } else {
            res.status = 400;
            res.set_content("{\"status\":\"empty\"}", "application/json");
        }
    }));

    // ----- Reservations -----
    svr.Post("/api/reserve", wrap([&](const httplib::Request& req, httplib::Response& res) {
        auto body = json::parse(req.body);
        reservationQueue.enqueueReservation(body["memberId"].get<string>(), body["bookId"]);
        res.set_content("{\"status\":\"success\"}", "application/json");
    }));

    svr.Get("/api/reservations", wrap([&](const httplib::Request& req, httplib::Response& res) {
        json j = json::array();
        for (const auto& r : reservationQueue.getAllReservations()) {
            j.push_back({{"memberId", r.MemberID}, {"bookId", r.BookID}});
        }
        res.set_content(j.dump(), "application/json");
    }));

    svr.Post("/api/process-reservation", wrap([&](const httplib::Request& req, httplib::Response& res) {
        string mId;
        int bId;
        if (reservationQueue.dequeueReservation(mId, bId)) {
            Book* b = bookTree.getBook(bId);
            if (b && b->CopiesAvailable > 0) {
                b->CopiesAvailable--;
                borrowHistory.addBorrowRecord(bookTree, mId, bId, "Today", "");
                json j = {{"memberId", mId}, {"bookId", bId}, {"status", "success"}};
                res.set_content(j.dump(), "application/json");
            } else {
                json j = {{"memberId", mId}, {"bookId", bId}, {"status", "unavailable"}};
                res.set_content(j.dump(), "application/json");
            }
        } else {
            res.status = 400;
            res.set_content("{\"status\":\"empty\"}", "application/json");
        }
    }));

    cout << "Starting server on http://localhost:8080..." << endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
