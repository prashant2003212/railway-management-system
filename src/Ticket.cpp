#include "Ticket.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <vector>

int Ticket::ticketCounter = 1000;

Ticket::Ticket(int trainNum, int passengerID, const std::string& date,
               int seat, double fare)
    : trainNumber(trainNum), passengerId(passengerID),
      travelDate(date), seatNumber(seat), totalFare(fare),
      status(TicketStatus::CONFIRMED) {
    pnrNumber = "PNR" + std::to_string(++ticketCounter);
    time_t now = time(nullptr);
    char buf[11];
    strftime(buf, sizeof(buf), "%d/%m/%Y", localtime(&now));
    bookingDate = std::string(buf);
}

std::string Ticket::getPNR() const { return pnrNumber; }
int Ticket::getTrainNumber() const { return trainNumber; }
int Ticket::getPassengerId() const { return passengerId; }
std::string Ticket::getTravelDate() const { return travelDate; }
int Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getTotalFare() const { return totalFare; }
TicketStatus Ticket::getStatus() const { return status; }
std::string Ticket::getBookingDate() const { return bookingDate; }
void Ticket::setStatus(TicketStatus s) { status = s; }
void Ticket::setSeatNumber(int seat) { seatNumber = seat; }

// ──────────────────────────────────────────────
// Option C: Smart Refund Logic
// ──────────────────────────────────────────────
double Ticket::calculateRefund() const {
    // Parse travel date DD/MM/YYYY
    int tDay   = std::stoi(travelDate.substr(0, 2));
    int tMonth = std::stoi(travelDate.substr(3, 2));
    int tYear  = std::stoi(travelDate.substr(6, 4));

    // Get today
    time_t now = time(nullptr);
    localtime(&now); // just advance time pointer

    // Days difference (simple calculation)
    tm travel = {};
    travel.tm_mday = tDay;
    travel.tm_mon  = tMonth - 1;
    travel.tm_year = tYear - 1900;
    time_t travelTime = mktime(&travel);
    double diffSeconds = difftime(travelTime, now);
    int daysLeft = (int)(diffSeconds / 86400);

    // Refund slabs (like IRCTC rules)
    if (daysLeft >= 2)       return totalFare * 0.90; // 90% refund
    else if (daysLeft == 1)  return totalFare * 0.50; // 50% refund
    else                     return totalFare * 0.25; // 25% refund (same day)
}

std::string Ticket::getRefundPolicy() const {
    int tDay   = std::stoi(travelDate.substr(0, 2));
    int tMonth = std::stoi(travelDate.substr(3, 2));
    int tYear  = std::stoi(travelDate.substr(6, 4));

    time_t now = time(nullptr);
    tm travel = {};
    travel.tm_mday = tDay;
    travel.tm_mon  = tMonth - 1;
    travel.tm_year = tYear - 1900;
    time_t travelTime = mktime(&travel);
    double diffSeconds = difftime(travelTime, now);
    int daysLeft = (int)(diffSeconds / 86400);

    if (daysLeft >= 2)
        return "90% refund (cancelled 2+ days before travel)";
    else if (daysLeft == 1)
        return "50% refund (cancelled 1 day before travel)";
    else if (daysLeft == 0)
        return "25% refund (same day cancellation)";
    else
        return "No refund (travel date has passed)";
}

std::string Ticket::statusToString() const {
    switch (status) {
        case TicketStatus::CONFIRMED:  return "CONFIRMED";
        case TicketStatus::WAITLISTED: return "WAITLISTED";
        case TicketStatus::CANCELLED:  return "CANCELLED";
    }
    return "UNKNOWN";
}

void Ticket::display() const {
    std::cout << "\n╔══════════════════════════════════════╗\n"
              << "║         RAILWAY TICKET               ║\n"
              << "╠══════════════════════════════════════╣\n"
              << "║ PNR      : " << std::left << std::setw(26) << pnrNumber << "║\n"
              << "║ Train #  : " << std::setw(26) << trainNumber << "║\n"
              << "║ Pax ID   : " << std::setw(26) << passengerId << "║\n"
              << "║ Travel   : " << std::setw(26) << travelDate << "║\n"
              << "║ Seat     : " << std::setw(26) << seatNumber << "║\n"
              << "║ Fare     : Rs. " << std::fixed << std::setprecision(2)
              << std::setw(22) << totalFare << "║\n"
              << "║ Booked   : " << std::setw(26) << bookingDate << "║\n"
              << "║ Status   : " << std::setw(26) << statusToString() << "║\n"
              << "╚══════════════════════════════════════╝\n";
}

std::string Ticket::serialize() const {
    std::ostringstream oss;
    oss << pnrNumber << "|" << trainNumber << "|" << passengerId << "|"
        << travelDate << "|" << seatNumber << "|" << totalFare << "|"
        << static_cast<int>(status) << "|" << bookingDate;
    return oss.str();
}

Ticket Ticket::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(iss, token, '|')) tokens.push_back(token);

    Ticket t;
    t.pnrNumber   = tokens[0];
    t.trainNumber = std::stoi(tokens[1]);
    t.passengerId = std::stoi(tokens[2]);
    t.travelDate  = tokens[3];
    t.seatNumber  = std::stoi(tokens[4]);
    t.totalFare   = std::stod(tokens[5]);
    t.status      = static_cast<TicketStatus>(std::stoi(tokens[6]));
    t.bookingDate = tokens[7];

    int pnrNum = std::stoi(t.pnrNumber.substr(3));
    if (pnrNum >= ticketCounter) ticketCounter = pnrNum + 1;
    return t;
}

void Ticket::setTicketCounter(int count) { ticketCounter = count; }
