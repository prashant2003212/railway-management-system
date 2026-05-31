#pragma once
#include <string>

class Passenger {
private:
    int passengerId;
    std::string name;
    int age;
    std::string gender;
    std::string contactNumber;
    std::string email;

public:
    Passenger() = default;
    Passenger(int id, const std::string& name, int age,
              const std::string& gender, const std::string& contact,
              const std::string& email);

    // Getters
    int getId() const;
    std::string getName() const;
    int getAge() const;
    std::string getGender() const;
    std::string getContact() const;
    std::string getEmail() const;

    // Setters
    void setName(const std::string& name);
    void setContact(const std::string& contact);
    void setEmail(const std::string& email);

    void display() const;
    std::string serialize() const;
    static Passenger deserialize(const std::string& data);
};
