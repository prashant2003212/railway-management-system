#pragma once
#include <vector>
#include <string>
#include <map>
#include "Train.h"
#include "Passenger.h"
#include "Ticket.h"

class RailwaySystem {
private:
    std::vector<Train> trains;
    std::vector<Passenger> passengers;
    std::vector<Ticket> tickets;
    bool isAdminLoggedIn;

    // Admin credentials
    static const std::string ADMIN_USERNAME;
    static const std::string ADMIN_PASSWORD;

    // Internal helpers
    int findTrainIndex(int trainNumber) const;
    int findPassengerIndex(int passengerId) const;
    int findTicketIndex(const std::string& pnr) const;
    int generatePassengerId() const;
    void saveAllData() const;

    // Menus
    void showMainMenu();
    void showAdminMenu();
    void showUserMenu();

    // Admin operations
    void adminLogin();
    void addTrain();
    void removeTrain();
    void updateTrain();
    void viewAllTrains() const;
    void viewAllPassengers() const;
    void viewAllTickets() const;
    void viewRevenueReport() const;

    // User operations
    void searchTrains() const;        // Option B: search + sort
    void showSeatMap() const;         // Option A: seat map
    void bookTicket();
    void cancelTicket();              // Option C: refund logic
    void viewTicketByPNR() const;
    void viewPassengerTickets() const;
    void registerPassenger();
    void updatePassengerInfo();

    // Validation helpers
    bool isValidDate(const std::string& date) const;
    bool isValidContact(const std::string& contact) const;
    bool isValidEmail(const std::string& email) const;

public:
    RailwaySystem();
    void run();
    void loadSampleData();
};
