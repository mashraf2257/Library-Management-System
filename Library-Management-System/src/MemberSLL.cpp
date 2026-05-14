#include "MemberSLL.h"

// Constructor
MemberSLL::MemberSLL() {
    head = nullptr;
}

// Destructor
MemberSLL::~MemberSLL() {
    Member* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

// Add a new member
void MemberSLL::addMember(const string& id, const string& name, const string& email, const string& phone, const string& address) {
    Member* newMember = new Member;
    newMember->ID = id;
    newMember->Name = name;
    newMember->Email = email;
    newMember->Phone = phone;
    newMember->Address = address;
    newMember->next = nullptr;

    // Insert at the end
    if (head == nullptr) {
        head = newMember;
    } else {
        Member* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newMember;
    }
}

// Remove member by ID
bool MemberSLL::removeMember(const string& id) {
    if (head == nullptr) {
        return false;
    }

    Member* temp = head;
    Member* prev = nullptr;

    while (temp != nullptr && temp->ID != id) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == nullptr) {
        return false;
    }

    if (prev == nullptr) {
        // Removing head
        head = head->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;
    return true;
}

// Get all members
std::vector<Member> MemberSLL::getAllMembers() {
    std::vector<Member> members;
    Member* temp = head;
    while (temp != nullptr) {
        members.push_back(*temp);
        temp = temp->next;
    }
    return members;
}

// Get a specific member
Member* MemberSLL::getMember(const string& id) {
    Member* temp = head;
    while (temp != nullptr) {
        if (temp->ID == id) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}