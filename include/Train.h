#pragma once
#include <string>
#include <vector>

class Train {
private:
    int trainNumber;
    std::string trainName;
    std::string source;
    std::string destination;
    std::string departureTime;
    std::string arrivalTime;
    int totalSeats;
    int availableSeats;
    double farePerKm;
    double distanceKm;
    std::vector<bool> seatMap; // true = booked, false = available

public:
    Train() = default;
    Train(int num, const std::string& name, const std::string& src,
          const std::string& dest, const std::string& dep,
          const std::string& arr, int seats, double fare, double dist);

    // Getters
    int getTrainNumber() const;
    std::string getTrainName() const;
    std::string getSource() const;
    std::string getDestination() const;
    std::string getDepartureTime() const;
    std::string getArrivalTime() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    double getFare() const;
    double getDistance() const;
    double getTotalFare() const;
    const std::vector<bool>& getSeatMap() const;

    // Setters
    void setAvailableSeats(int seats);
    void setSeatMap(const std::vector<bool>& map);

    // Utility
    bool bookSeat();
    bool cancelSeat();
    int bookSpecificSeat(int seatNum); // returns booked seat number, -1 if unavailable
    bool cancelSpecificSeat(int seatNum);
    void displaySeatMap() const;
    void display() const;
    std::string serialize() const;
    static Train deserialize(const std::string& data);
};
