#include "Passenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

Passenger::Passenger(int id, const std::string& n, int a,
                     const std::string& g, const std::string& contact,
                     const std::string& mail)
    : passengerId(id), name(n), age(a), gender(g),
      contactNumber(contact), email(mail) {}

int Passenger::getId() const { return passengerId; }
std::string Passenger::getName() const { return name; }
int Passenger::getAge() const { return age; }
std::string Passenger::getGender() const { return gender; }
std::string Passenger::getContact() const { return contactNumber; }
std::string Passenger::getEmail() const { return email; }

void Passenger::setName(const std::string& n) { name = n; }
void Passenger::setContact(const std::string& c) { contactNumber = c; }
void Passenger::setEmail(const std::string& e) { email = e; }

void Passenger::display() const {
    std::cout << std::left
              << "+------------------------------------------+\n"
              << "| Passenger ID : " << std::setw(26) << passengerId << "|\n"
              << "| Name         : " << std::setw(26) << name << "|\n"
              << "| Age / Gender : " << age << " / " << std::setw(22) << gender << "|\n"
              << "| Contact      : " << std::setw(26) << contactNumber << "|\n"
              << "| Email        : " << std::setw(26) << email << "|\n"
              << "+------------------------------------------+\n";
}

std::string Passenger::serialize() const {
    std::ostringstream oss;
    oss << passengerId << "|" << name << "|" << age << "|"
        << gender << "|" << contactNumber << "|" << email;
    return oss.str();
}

Passenger Passenger::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);

    Passenger p;
    p.passengerId   = std::stoi(tokens[0]);
    p.name          = tokens[1];
    p.age           = std::stoi(tokens[2]);
    p.gender        = tokens[3];
    p.contactNumber = tokens[4];
    p.email         = tokens[5];
    return p;
}
