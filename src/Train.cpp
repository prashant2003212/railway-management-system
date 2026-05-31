#include "Train.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Train::Train(int num, const std::string& name, const std::string& src,
             const std::string& dest, const std::string& dep,
             const std::string& arr, int seats, double fare, double dist)
    : trainNumber(num), trainName(name), source(src), destination(dest),
      departureTime(dep), arrivalTime(arr), totalSeats(seats),
      availableSeats(seats), farePerKm(fare), distanceKm(dist) {
    seatMap.assign(seats, false);
}

int Train::getTrainNumber() const { return trainNumber; }
std::string Train::getTrainName() const { return trainName; }
std::string Train::getSource() const { return source; }
std::string Train::getDestination() const { return destination; }
std::string Train::getDepartureTime() const { return departureTime; }
std::string Train::getArrivalTime() const { return arrivalTime; }
int Train::getTotalSeats() const { return totalSeats; }
int Train::getAvailableSeats() const { return availableSeats; }
double Train::getFare() const { return farePerKm; }
double Train::getDistance() const { return distanceKm; }
double Train::getTotalFare() const { return farePerKm * distanceKm; }
const std::vector<bool>& Train::getSeatMap() const { return seatMap; }

void Train::setAvailableSeats(int seats) { availableSeats = seats; }
void Train::setSeatMap(const std::vector<bool>& map) { seatMap = map; }

bool Train::bookSeat() {
    for (int i = 0; i < (int)seatMap.size(); ++i) {
        if (!seatMap[i]) {
            seatMap[i] = true;
            availableSeats--;
            return true;
        }
    }
    return false;
}

bool Train::cancelSeat() {
    if (availableSeats < totalSeats) {
        availableSeats++;
        return true;
    }
    return false;
}

int Train::bookSpecificSeat(int seatNum) {
    if (seatNum < 1 || seatNum > totalSeats) {
        for (int i = 0; i < (int)seatMap.size(); ++i) {
            if (!seatMap[i]) {
                seatMap[i] = true;
                availableSeats--;
                return i + 1;
            }
        }
        return -1;
    }
    if (seatMap[seatNum - 1]) return -1;
    seatMap[seatNum - 1] = true;
    availableSeats--;
    return seatNum;
}

bool Train::cancelSpecificSeat(int seatNum) {
    if (seatNum < 1 || seatNum > totalSeats) return false;
    if (!seatMap[seatNum - 1]) return false;
    seatMap[seatNum - 1] = false;
    availableSeats++;
    return true;
}

void Train::displaySeatMap() const {
    std::cout << "\n  === SEAT MAP: " << trainName << " (#" << trainNumber << ") ===\n";
    std::cout << "  [ O ] = Available   [ X ] = Booked\n\n";

    int cols = 4;
    for (int i = 0; i < (int)seatMap.size(); ++i) {
        if (i % cols == 0) {
            std::cout << "  Row " << std::setw(2) << (i / cols + 1) << " | ";
        }
        if (seatMap[i])
            std::cout << "[" << std::setw(2) << (i+1) << " X]";
        else
            std::cout << "[" << std::setw(2) << (i+1) << " O]";

        if (i % cols == 1) std::cout << "  ||  ";
        else if (i % cols == cols - 1) std::cout << "\n";
        else std::cout << " ";
    }
    std::cout << "\n  Total: " << totalSeats << " seats | "
              << availableSeats << " available | "
              << (totalSeats - availableSeats) << " booked\n";
}

void Train::display() const {
    std::cout << std::left
              << "+-------------------------------------------------+\n"
              << "| Train #: " << std::setw(38) << trainNumber << "|\n"
              << "| Name   : " << std::setw(38) << trainName << "|\n"
              << "| Route  : " << std::setw(18) << source << " -> "
              << std::setw(16) << destination << "|\n"
              << "| Departs: " << std::setw(18) << departureTime
              << " Arrives: " << std::setw(11) << arrivalTime << "|\n"
              << "| Seats  : " << std::setw(5) << availableSeats
              << " available / " << std::setw(5) << totalSeats << " total           |\n"
              << "| Fare   : Rs. " << std::fixed << std::setprecision(2)
              << std::setw(34) << getTotalFare() << "|\n"
              << "+-------------------------------------------------+\n";
}

std::string Train::serialize() const {
    std::ostringstream oss;
    oss << trainNumber << "|" << trainName << "|" << source << "|"
        << destination << "|" << departureTime << "|" << arrivalTime << "|"
        << totalSeats << "|" << availableSeats << "|" << farePerKm << "|" << distanceKm << "|";
    for (bool b : seatMap) oss << (b ? "1" : "0");
    return oss.str();
}

Train Train::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);

    Train t;
    t.trainNumber    = std::stoi(tokens[0]);
    t.trainName      = tokens[1];
    t.source         = tokens[2];
    t.destination    = tokens[3];
    t.departureTime  = tokens[4];
    t.arrivalTime    = tokens[5];
    t.totalSeats     = std::stoi(tokens[6]);
    t.availableSeats = std::stoi(tokens[7]);
    t.farePerKm      = std::stod(tokens[8]);
    t.distanceKm     = std::stod(tokens[9]);

    t.seatMap.resize(t.totalSeats, false);
    if (tokens.size() > 10 && !tokens[10].empty()) {
        for (int i = 0; i < (int)tokens[10].size() && i < t.totalSeats; ++i)
            t.seatMap[i] = (tokens[10][i] == '1');
    }
    return t;
}