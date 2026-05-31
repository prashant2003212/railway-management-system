#pragma once
#include <string>
#include "Passenger.h"
#include "Train.h"

enum class TicketStatus { CONFIRMED, WAITLISTED, CANCELLED };

class Ticket {
private:
    std::string pnrNumber;
    int trainNumber;
    int passengerId;
    std::string travelDate;
    int seatNumber;
    double totalFare;
    TicketStatus status;
    std::string bookingDate;

    static int ticketCounter;

public:
    Ticket() = default;
    Ticket(int trainNum, int passengerID, const std::string& date,
           int seat, double fare);

    // Getters
    std::string getPNR() const;
    int getTrainNumber() const;
    int getPassengerId() const;
    std::string getTravelDate() const;
    int getSeatNumber() const;
    double getTotalFare() const;
    TicketStatus getStatus() const;
    std::string getBookingDate() const;

    // Setters
    void setStatus(TicketStatus s);
    void setSeatNumber(int seat);

    // Refund logic
    double calculateRefund() const; // Based on days before travel
    std::string getRefundPolicy() const;

    std::string statusToString() const;
    void display() const;
    std::string serialize() const;
    static Ticket deserialize(const std::string& data);
    static void setTicketCounter(int count);
};
