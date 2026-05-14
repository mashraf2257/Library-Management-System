#ifndef MemberSLL_H
#define MemberSLL_H
#include <string>
#include <iostream>
#include <vector>
using namespace std;

// Node for a library member
struct Member {
    string ID;      // Alphanumeric ID e.g. "AB1234"
    string Name;
    string Email;
    string Phone;
    string Address;
    Member* next;
};

// Singly Linked List for members
class MemberSLL {
private:
    Member* head;

public:
    MemberSLL(); // Constructor
    ~MemberSLL(); // Destructor to free memory

    void addMember(const string& id, const string& name, const string& email, const string& phone, const string& address);
    bool removeMember(const string& id);
    std::vector<Member> getAllMembers();
    Member* getMember(const string& id);
};

#endif
