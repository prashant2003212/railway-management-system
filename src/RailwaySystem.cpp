#include "RailwaySystem.h"
#include "FileManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <sstream>
#include <regex>

const std::string RailwaySystem::ADMIN_USERNAME = "admin";
const std::string RailwaySystem::ADMIN_PASSWORD = "railway123";

RailwaySystem::RailwaySystem() : isAdminLoggedIn(false) {
    trains     = FileManager::loadTrains();
    passengers = FileManager::loadPassengers();
    tickets    = FileManager::loadTickets();
    if (trains.empty()) {
        std::cout << "[INFO] No existing data found. Loading sample data...\n";
        loadSampleData();
    }
}

void RailwaySystem::saveAllData() const {
    FileManager::saveTrains(trains);
    FileManager::savePassengers(passengers);
    FileManager::saveTickets(tickets);
}

void RailwaySystem::loadSampleData() {
    trains = {
        {12301, "Rajdhani Express",  "New Delhi", "Mumbai",    "16:25", "08:15", 20, 1.80, 1384},
        {12951, "Mumbai Rajdhani",   "Mumbai",    "New Delhi", "17:00", "08:35", 20, 1.80, 1384},
        {12259, "Duronto Express",   "Howrah",    "New Delhi", "20:05", "17:25", 20, 1.50, 1447},
        {11077, "Jhelum Express",    "Jammu",     "Pune",      "23:55", "05:15", 20, 1.20, 2075},
        {22691, "Rajdhani Exp BLR",  "Bangalore", "New Delhi", "20:00", "05:55", 20, 1.70, 2366},
    };
    saveAllData();
    std::cout << "[INFO] Sample train data loaded.\n\n";
}

// ─── Helpers ────────────────────────────────────
int RailwaySystem::findTrainIndex(int trainNumber) const {
    for (int i = 0; i < (int)trains.size(); ++i)
        if (trains[i].getTrainNumber() == trainNumber) return i;
    return -1;
}
int RailwaySystem::findPassengerIndex(int passengerId) const {
    for (int i = 0; i < (int)passengers.size(); ++i)
        if (passengers[i].getId() == passengerId) return i;
    return -1;
}
int RailwaySystem::findTicketIndex(const std::string& pnr) const {
    for (int i = 0; i < (int)tickets.size(); ++i)
        if (tickets[i].getPNR() == pnr) return i;
    return -1;
}
int RailwaySystem::generatePassengerId() const {
    int maxId = 1000;
    for (const auto& p : passengers) maxId = std::max(maxId, p.getId());
    return maxId + 1;
}

// ─── Validation ─────────────────────────────────
bool RailwaySystem::isValidDate(const std::string& date) const {
    std::regex dateRegex(R"(\d{2}/\d{2}/\d{4})");
    return std::regex_match(date, dateRegex);
}
bool RailwaySystem::isValidContact(const std::string& contact) const {
    std::regex phoneRegex(R"(\d{10})");
    return std::regex_match(contact, phoneRegex);
}
bool RailwaySystem::isValidEmail(const std::string& email) const {
    std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailRegex);
}

// ─── Menus ──────────────────────────────────────
void RailwaySystem::run() {
    std::cout << "\n";
    std::cout << "  ╔══════════════════════════════════════════╗\n";
    std::cout << "  ║    INDIAN RAILWAY MANAGEMENT SYSTEM      ║\n";
    std::cout << "  ║         Welcome Aboard!  🚂               ║\n";
    std::cout << "  ╚══════════════════════════════════════════╝\n\n";
    showMainMenu();
}

void RailwaySystem::showMainMenu() {
    int choice;
    do {
        std::cout << "\n========== MAIN MENU ==========\n"
                  << " 1. User Panel\n"
                  << " 2. Admin Panel\n"
                  << " 0. Exit\n"
                  << "================================\n"
                  << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: showUserMenu(); break;
            case 2: showAdminMenu(); break;
            case 0: std::cout << "\nThank you for using Railway Management System!\n"; break;
            default: std::cout << "[!] Invalid choice.\n";
        }
    } while (choice != 0);
}

void RailwaySystem::showAdminMenu() {
    if (!isAdminLoggedIn) {
        adminLogin();
        if (!isAdminLoggedIn) return;
    }
    int choice;
    do {
        std::cout << "\n========== ADMIN PANEL ==========\n"
                  << " 1. Add Train\n"
                  << " 2. Remove Train\n"
                  << " 3. Update Train Info\n"
                  << " 4. View All Trains\n"
                  << " 5. View All Passengers\n"
                  << " 6. View All Tickets\n"
                  << " 7. Revenue Report\n"
                  << " 0. Logout\n"
                  << "==================================\n"
                  << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: addTrain(); break;
            case 2: removeTrain(); break;
            case 3: updateTrain(); break;
            case 4: viewAllTrains(); break;
            case 5: viewAllPassengers(); break;
            case 6: viewAllTickets(); break;
            case 7: viewRevenueReport(); break;
            case 0: isAdminLoggedIn = false;
                    std::cout << "[INFO] Admin logged out.\n"; break;
            default: std::cout << "[!] Invalid choice.\n";
        }
    } while (choice != 0);
}

void RailwaySystem::showUserMenu() {
    int choice;
    do {
        std::cout << "\n========== USER PANEL ==========\n"
                  << " 1. Register as Passenger\n"
                  << " 2. Search & Sort Trains\n"      // Option B
                  << " 3. View Seat Map\n"             // Option A
                  << " 4. Book Ticket\n"
                  << " 5. Cancel Ticket\n"             // Option C
                  << " 6. View Ticket by PNR\n"
                  << " 7. View My Tickets\n"
                  << " 8. Update My Info\n"
                  << " 0. Back to Main Menu\n"
                  << "=================================\n"
                  << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 1: registerPassenger(); break;
            case 2: searchTrains(); break;
            case 3: showSeatMap(); break;
            case 4: bookTicket(); break;
            case 5: cancelTicket(); break;
            case 6: viewTicketByPNR(); break;
            case 7: viewPassengerTickets(); break;
            case 8: updatePassengerInfo(); break;
            case 0: break;
            default: std::cout << "[!] Invalid choice.\n";
        }
    } while (choice != 0);
}

// ─── Admin Operations ───────────────────────────
void RailwaySystem::adminLogin() {
    std::string uname, pwd;
    std::cout << "\n--- Admin Login ---\n";
    std::cout << "Username: "; std::getline(std::cin, uname);
    std::cout << "Password: "; std::getline(std::cin, pwd);
    if (uname == ADMIN_USERNAME && pwd == ADMIN_PASSWORD) {
        isAdminLoggedIn = true;
        std::cout << "[✓] Admin login successful!\n";
    } else {
        std::cout << "[✗] Invalid credentials.\n";
    }
}

void RailwaySystem::addTrain() {
    std::cout << "\n--- Add New Train ---\n";
    int num, seats; double fare, dist;
    std::string name, src, dest, dep, arr;
    std::cout << "Train Number  : "; std::cin >> num; std::cin.ignore();
    if (findTrainIndex(num) != -1) { std::cout << "[!] Train already exists.\n"; return; }
    std::cout << "Train Name    : "; std::getline(std::cin, name);
    std::cout << "Source        : "; std::getline(std::cin, src);
    std::cout << "Destination   : "; std::getline(std::cin, dest);
    std::cout << "Departure Time: "; std::getline(std::cin, dep);
    std::cout << "Arrival Time  : "; std::getline(std::cin, arr);
    std::cout << "Total Seats   : "; std::cin >> seats;
    std::cout << "Fare/km (Rs.) : "; std::cin >> fare;
    std::cout << "Distance (km) : "; std::cin >> dist; std::cin.ignore();
    trains.emplace_back(num, name, src, dest, dep, arr, seats, fare, dist);
    saveAllData();
    std::cout << "[✓] Train added successfully!\n";
}

void RailwaySystem::removeTrain() {
    std::cout << "\n--- Remove Train ---\n";
    int num; std::cout << "Train number: "; std::cin >> num; std::cin.ignore();
    int idx = findTrainIndex(num);
    if (idx == -1) { std::cout << "[!] Train not found.\n"; return; }
    trains.erase(trains.begin() + idx);
    saveAllData();
    std::cout << "[✓] Train removed.\n";
}

void RailwaySystem::updateTrain() {
    std::cout << "\n--- Update Train ---\n";
    int num; std::cout << "Train number: "; std::cin >> num; std::cin.ignore();
    int idx = findTrainIndex(num);
    if (idx == -1) { std::cout << "[!] Train not found.\n"; return; }
    trains[idx].display();
    int seats; std::cout << "New available seats: "; std::cin >> seats; std::cin.ignore();
    trains[idx].setAvailableSeats(seats);
    saveAllData();
    std::cout << "[✓] Train updated.\n";
}

void RailwaySystem::viewAllTrains() const {
    std::cout << "\n========== ALL TRAINS (" << trains.size() << ") ==========\n";
    if (trains.empty()) { std::cout << "No trains.\n"; return; }
    for (const auto& t : trains) t.display();
}

void RailwaySystem::viewAllPassengers() const {
    std::cout << "\n========== ALL PASSENGERS (" << passengers.size() << ") ==========\n";
    if (passengers.empty()) { std::cout << "No passengers.\n"; return; }
    for (const auto& p : passengers) p.display();
}

void RailwaySystem::viewAllTickets() const {
    std::cout << "\n========== ALL TICKETS (" << tickets.size() << ") ==========\n";
    if (tickets.empty()) { std::cout << "No tickets.\n"; return; }
    for (const auto& t : tickets) t.display();
}

void RailwaySystem::viewRevenueReport() const {
    double total = 0; int confirmed = 0, cancelled = 0;
    for (const auto& t : tickets) {
        if (t.getStatus() == TicketStatus::CONFIRMED) { total += t.getTotalFare(); confirmed++; }
        else if (t.getStatus() == TicketStatus::CANCELLED) cancelled++;
    }
    std::cout << "\n========== REVENUE REPORT ==========\n"
              << "  Total Tickets   : " << tickets.size() << "\n"
              << "  Confirmed       : " << confirmed << "\n"
              << "  Cancelled       : " << cancelled << "\n"
              << "  Total Revenue   : Rs. " << std::fixed << std::setprecision(2) << total << "\n"
              << "=====================================\n";
}

// ─── Option A: Seat Map ─────────────────────────
void RailwaySystem::showSeatMap() const {
    std::cout << "\n--- View Seat Map ---\n";
    int num; std::cout << "Enter Train Number: "; std::cin >> num; std::cin.ignore();
    int idx = findTrainIndex(num);
    if (idx == -1) { std::cout << "[!] Train not found.\n"; return; }
    trains[idx].displaySeatMap();
}

// ─── Option B: Search + Sort ────────────────────
void RailwaySystem::searchTrains() const {
    std::cout << "\n--- Search & Sort Trains ---\n"
              << " 1. Search by Source & Destination\n"
              << " 2. Search by Train Number\n"
              << " 3. Sort by Fare (Low to High)\n"
              << " 4. Sort by Available Seats (High to Low)\n"
              << " 5. Show All Trains\n"
              << "Choice: ";
    int c; std::cin >> c; std::cin.ignore();

    auto toUpper = [](std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    };

    if (c == 1) {
        std::string src, dest;
        std::cout << "Source     : "; std::getline(std::cin, src);
        std::cout << "Destination: "; std::getline(std::cin, dest);
        bool found = false;
        for (const auto& t : trains) {
            if (toUpper(t.getSource()).find(toUpper(src)) != std::string::npos &&
                toUpper(t.getDestination()).find(toUpper(dest)) != std::string::npos) {
                t.display(); found = true;
            }
        }
        if (!found) std::cout << "[!] No trains found for this route.\n";

    } else if (c == 2) {
        int num; std::cout << "Train #: "; std::cin >> num; std::cin.ignore();
        int idx = findTrainIndex(num);
        if (idx == -1) std::cout << "[!] Train not found.\n";
        else trains[idx].display();

    } else if (c == 3) {
        // Sort by fare — make a copy to not disturb original order
        std::vector<Train> sorted = trains;
        std::sort(sorted.begin(), sorted.end(),
            [](const Train& a, const Train& b) {
                return a.getTotalFare() < b.getTotalFare();
            });
        std::cout << "\n=== Trains Sorted by Fare (Cheapest First) ===\n";
        for (const auto& t : sorted) t.display();

    } else if (c == 4) {
        std::vector<Train> sorted = trains;
        std::sort(sorted.begin(), sorted.end(),
            [](const Train& a, const Train& b) {
                return a.getAvailableSeats() > b.getAvailableSeats();
            });
        std::cout << "\n=== Trains Sorted by Availability ===\n";
        for (const auto& t : sorted) t.display();

    } else {
        viewAllTrains();
    }
}

// ─── Book Ticket ────────────────────────────────
void RailwaySystem::bookTicket() {
    std::cout << "\n--- Book Ticket ---\n";
    int pid, tnum; std::string date;

    std::cout << "Passenger ID   : "; std::cin >> pid; std::cin.ignore();
    int pidx = findPassengerIndex(pid);
    if (pidx == -1) { std::cout << "[!] Passenger not found. Register first.\n"; return; }

    std::cout << "Train Number   : "; std::cin >> tnum; std::cin.ignore();
    int tidx = findTrainIndex(tnum);
    if (tidx == -1) { std::cout << "[!] Train not found.\n"; return; }

    std::cout << "Travel Date (DD/MM/YYYY): "; std::getline(std::cin, date);
    if (!isValidDate(date)) { std::cout << "[!] Invalid date. Use DD/MM/YYYY.\n"; return; }

    // Show seat map before booking
    trains[tidx].displaySeatMap();

    if (trains[tidx].getAvailableSeats() == 0) {
        std::cout << "[!] No seats available. Adding to waitlist.\n";
        Ticket ticket(tnum, pid, date, 0, trains[tidx].getTotalFare());
        ticket.setStatus(TicketStatus::WAITLISTED);
        tickets.push_back(ticket);
        saveAllData();
        std::cout << "[✓] Waitlisted! PNR: " << ticket.getPNR() << "\n";
        return;
    }

    // Ask for preferred seat
    int preferredSeat = 0;
    std::cout << "Preferred seat number (0 for auto): "; std::cin >> preferredSeat; std::cin.ignore();

    int assignedSeat = trains[tidx].bookSpecificSeat(preferredSeat);
    if (assignedSeat == -1) {
        std::cout << "[!] Seat " << preferredSeat << " already booked. Auto-assigning...\n";
        assignedSeat = trains[tidx].bookSpecificSeat(0);
    }

    Ticket ticket(tnum, pid, date, assignedSeat, trains[tidx].getTotalFare());
    tickets.push_back(ticket);
    saveAllData();

    std::cout << "[✓] Ticket booked!\n";
    ticket.display();
    std::cout << "Passenger: " << passengers[pidx].getName()
              << " | Seat: " << assignedSeat << "\n";

    // Show updated seat map
    trains[tidx].displaySeatMap();
}

// ─── Option C: Cancel with Refund Logic ─────────
void RailwaySystem::cancelTicket() {
    std::cout << "\n--- Cancel Ticket ---\n";
    std::string pnr;
    std::cout << "Enter PNR: "; std::getline(std::cin, pnr);

    int tidx = findTicketIndex(pnr);
    if (tidx == -1) { std::cout << "[!] Ticket not found.\n"; return; }
    if (tickets[tidx].getStatus() == TicketStatus::CANCELLED) {
        std::cout << "[!] Already cancelled.\n"; return;
    }

    // Show refund info BEFORE confirming
    double refund = tickets[tidx].calculateRefund();
    std::string policy = tickets[tidx].getRefundPolicy();

    std::cout << "\n  Ticket : " << pnr << "\n"
              << "  Fare   : Rs. " << std::fixed << std::setprecision(2)
              << tickets[tidx].getTotalFare() << "\n"
              << "  Policy : " << policy << "\n"
              << "  Refund : Rs. " << refund << "\n"
              << "\n  Confirm cancellation? (y/n): ";

    char confirm; std::cin >> confirm; std::cin.ignore();
    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "[INFO] Cancellation aborted.\n"; return;
    }

    // Free the seat in train
    int trainIdx = findTrainIndex(tickets[tidx].getTrainNumber());
    if (trainIdx != -1 && tickets[tidx].getStatus() == TicketStatus::CONFIRMED) {
        trains[trainIdx].cancelSpecificSeat(tickets[tidx].getSeatNumber());
    }

    tickets[tidx].setStatus(TicketStatus::CANCELLED);
    saveAllData();

    std::cout << "[✓] Ticket cancelled. Refund of Rs. " << refund
              << " will be processed.\n";

    // Check if any waitlisted ticket can now be confirmed
    for (auto& wt : tickets) {
        if (wt.getStatus() == TicketStatus::WAITLISTED &&
            wt.getTrainNumber() == tickets[tidx].getTrainNumber()) {
            if (trainIdx != -1 && trains[trainIdx].getAvailableSeats() > 0) {
                int seat = trains[trainIdx].bookSpecificSeat(0);
                wt.setSeatNumber(seat);
                wt.setStatus(TicketStatus::CONFIRMED);
                std::cout << "[INFO] PNR " << wt.getPNR()
                          << " moved from WAITLIST to CONFIRMED (Seat " << seat << ")!\n";
                saveAllData();
                break;
            }
        }
    }
}

void RailwaySystem::viewTicketByPNR() const {
    std::string pnr; std::cout << "Enter PNR: "; std::getline(std::cin, pnr);
    int idx = findTicketIndex(pnr);
    if (idx == -1) { std::cout << "[!] Not found.\n"; return; }
    tickets[idx].display();
    int pidx = findPassengerIndex(tickets[idx].getPassengerId());
    if (pidx != -1) passengers[pidx].display();
    int trainIdx = findTrainIndex(tickets[idx].getTrainNumber());
    if (trainIdx != -1) trains[trainIdx].display();
}

void RailwaySystem::viewPassengerTickets() const {
    int pid; std::cout << "Passenger ID: "; std::cin >> pid; std::cin.ignore();
    if (findPassengerIndex(pid) == -1) { std::cout << "[!] Not found.\n"; return; }
    bool found = false;
    for (const auto& t : tickets)
        if (t.getPassengerId() == pid) { t.display(); found = true; }
    if (!found) std::cout << "[!] No tickets.\n";
}

void RailwaySystem::registerPassenger() {
    std::cout << "\n--- Register Passenger ---\n";
    std::string name, gender, contact, email; int age;
    std::cout << "Name    : "; std::getline(std::cin, name);
    std::cout << "Age     : "; std::cin >> age; std::cin.ignore();
    std::cout << "Gender  : "; std::getline(std::cin, gender);
    std::cout << "Contact : "; std::getline(std::cin, contact);
    if (!isValidContact(contact)) { std::cout << "[!] Invalid contact.\n"; return; }
    std::cout << "Email   : "; std::getline(std::cin, email);
    if (!isValidEmail(email)) { std::cout << "[!] Invalid email.\n"; return; }
    int id = generatePassengerId();
    passengers.emplace_back(id, name, age, gender, contact, email);
    saveAllData();
    std::cout << "[✓] Registered! Your Passenger ID: " << id << "\n";
}

void RailwaySystem::updatePassengerInfo() {
    int pid; std::cout << "Passenger ID: "; std::cin >> pid; std::cin.ignore();
    int idx = findPassengerIndex(pid);
    if (idx == -1) { std::cout << "[!] Not found.\n"; return; }
    passengers[idx].display();
    std::cout << " 1. Name  2. Contact  3. Email\nChoice: ";
    int c; std::cin >> c; std::cin.ignore();
    std::string val;
    if (c == 1) { std::cout << "New Name: "; std::getline(std::cin, val); passengers[idx].setName(val); }
    else if (c == 2) {
        std::cout << "New Contact: "; std::getline(std::cin, val);
        if (!isValidContact(val)) { std::cout << "[!] Invalid.\n"; return; }
        passengers[idx].setContact(val);
    } else if (c == 3) {
        std::cout << "New Email: "; std::getline(std::cin, val);
        if (!isValidEmail(val)) { std::cout << "[!] Invalid.\n"; return; }
        passengers[idx].setEmail(val);
    }
    saveAllData();
    std::cout << "[✓] Updated.\n";
}
