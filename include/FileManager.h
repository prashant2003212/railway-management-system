#pragma once
#include <string>
#include <vector>
#include "Train.h"
#include "Passenger.h"
#include "Ticket.h"

class FileManager {
private:
    static const std::string TRAINS_FILE;
    static const std::string PASSENGERS_FILE;
    static const std::string TICKETS_FILE;

public:
    // Train persistence
    static void saveTrains(const std::vector<Train>& trains);
    static std::vector<Train> loadTrains();

    // Passenger persistence
    static void savePassengers(const std::vector<Passenger>& passengers);
    static std::vector<Passenger> loadPassengers();

    // Ticket persistence
    static void saveTickets(const std::vector<Ticket>& tickets);
    static std::vector<Ticket> loadTickets();

    // Utility
    static bool fileExists(const std::string& filename);
    static void ensureDataDirectory();
};
