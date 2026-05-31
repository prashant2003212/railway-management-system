#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#define MAKE_DIR(path) _mkdir(path)
#else
#define MAKE_DIR(path) mkdir(path, 0777)
#endif

const std::string FileManager::TRAINS_FILE     = "data/trains.dat";
const std::string FileManager::PASSENGERS_FILE = "data/passengers.dat";
const std::string FileManager::TICKETS_FILE    = "data/tickets.dat";

bool FileManager::fileExists(const std::string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

void FileManager::ensureDataDirectory() {
    MAKE_DIR("data");
}

void FileManager::saveTrains(const std::vector<Train>& trains) {
    ensureDataDirectory();
    std::ofstream file(TRAINS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save trains data.\n";
        return;
    }
    for (const auto& t : trains)
        file << t.serialize() << "\n";
    file.close();
}

std::vector<Train> FileManager::loadTrains() {
    std::vector<Train> trains;
    if (!fileExists(TRAINS_FILE)) return trains;
    std::ifstream file(TRAINS_FILE);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            trains.push_back(Train::deserialize(line));
    }
    return trains;
}

void FileManager::savePassengers(const std::vector<Passenger>& passengers) {
    ensureDataDirectory();
    std::ofstream file(PASSENGERS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save passengers data.\n";
        return;
    }
    for (const auto& p : passengers)
        file << p.serialize() << "\n";
    file.close();
}

std::vector<Passenger> FileManager::loadPassengers() {
    std::vector<Passenger> passengers;
    if (!fileExists(PASSENGERS_FILE)) return passengers;
    std::ifstream file(PASSENGERS_FILE);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            passengers.push_back(Passenger::deserialize(line));
    }
    return passengers;
}

void FileManager::saveTickets(const std::vector<Ticket>& tickets) {
    ensureDataDirectory();
    std::ofstream file(TICKETS_FILE);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not save tickets data.\n";
        return;
    }
    for (const auto& t : tickets)
        file << t.serialize() << "\n";
    file.close();
}

std::vector<Ticket> FileManager::loadTickets() {
    std::vector<Ticket> tickets;
    if (!fileExists(TICKETS_FILE)) return tickets;
    std::ifstream file(TICKETS_FILE);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty())
            tickets.push_back(Ticket::deserialize(line));
    }
    return tickets;
}
