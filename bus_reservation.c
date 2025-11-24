/*
 * Project: Bus/Train Ticket Reservation System
 * Language: C
 * Features: Admin/User Modes, File Handling (buses.dat), Seat Matrix Display, Booking/Cancellation
 * Note: This program uses standard C libraries and is compatible with GCC/Clang/MSVC.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_SEATS 32
#define DATA_FILE "buses.dat"
#define ADMIN_PASS "admin123"

// --- Structures ---

typedef struct {
    int bus_id;
    char bus_name[50];
    char source[50];
    char destination[50];
    float fare;
    int seats[MAX_SEATS]; // 0 for Empty, 1 for Booked
    int available_seats;
} Bus;

// --- Function Prototypes ---
void adminMenu();
void userMenu();
void addBus();
void viewAllBuses();
void bookTicket();
void cancelTicket();
void showSeatMap(Bus b);
void saveData();
void loadData();
void clearInputBuffer();
int getBusIndex(int id);

// --- Global Variables ---
Bus buses[50]; // Max 50 buses supported for this demo
int bus_count = 0;

// --- Main Function ---
int main() {
    loadData(); // Load existing data from file on startup
    int choice;

    while (1) {
        printf("\n==========================================\n");
        printf("    BUS RESERVATION SYSTEM MAIN MENU\n");
        printf("==========================================\n");
        printf("1. User Mode (Book/Cancel)\n");
        printf("2. Admin Mode (Add Routes)\n");
        printf("3. Exit\n");
        printf("Enter Choice: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1: userMenu(); break;
            case 2: adminMenu(); break;
            case 3: 
                saveData(); // Save before exit
                printf("Exiting... Data Saved.\n");
                exit(0);
            default: printf("Invalid Choice!\n");
        }
    }
    return 0;
}

// --- Admin Functions ---

void adminMenu() {
    char pass[20];
    printf("\nEnter Admin Password: ");
    scanf("%s", pass);

    if (strcmp(pass, ADMIN_PASS) != 0) {
        printf("Access Denied! Wrong Password.\n");
        return;
    }

    int choice;
    while (1) {
        printf("\n--- ADMIN PANEL ---\n");
        printf("1. Add New Bus Route\n");
        printf("2. View All Routes\n");
        printf("3. Delete All Data (Reset)\n");
        printf("4. Back to Main Menu\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBus(); break;
            case 2: viewAllBuses(); break;
            case 3: 
                bus_count = 0; 
                saveData(); 
                printf("All data deleted.\n"); 
                break;
            case 4: return;
            default: printf("Invalid Choice.\n");
        }
    }
}

void addBus() {
    if (bus_count >= 50) {
        printf("Memory Full! Cannot add more buses.\n");
        return;
    }

    Bus newBus;
    newBus.bus_id = 1001 + bus_count; // Auto-increment ID
    
    printf("Enter Bus Name (e.g. Volvo-900): ");
    clearInputBuffer();
    fgets(newBus.bus_name, 50, stdin);
    newBus.bus_name[strcspn(newBus.bus_name, "\n")] = 0; // Remove newline

    printf("Enter Source: ");
    fgets(newBus.source, 50, stdin);
    newBus.source[strcspn(newBus.source, "\n")] = 0;

    printf("Enter Destination: ");
    fgets(newBus.destination, 50, stdin);
    newBus.destination[strcspn(newBus.destination, "\n")] = 0;

    printf("Enter Ticket Fare: ");
    scanf("%f", &newBus.fare);

    // Initialize seats
    for (int i = 0; i < MAX_SEATS; i++) {
        newBus.seats[i] = 0; // 0 = Empty
    }
    newBus.available_seats = MAX_SEATS;

    buses[bus_count++] = newBus;
    saveData();
    printf("Bus Added Successfully! Bus ID is %d\n", newBus.bus_id);
}

// --- User Functions ---

void userMenu() {
    int choice;
    while (1) {
        printf("\n--- USER PANEL ---\n");
        printf("1. View Available Buses\n");
        printf("2. Book Ticket\n");
        printf("3. Cancel Ticket\n");
        printf("4. Back to Main Menu\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewAllBuses(); break;
            case 2: bookTicket(); break;
            case 3: cancelTicket(); break;
            case 4: return;
            default: printf("Invalid Choice.\n");
        }
    }
}

void viewAllBuses() {
    if (bus_count == 0) {
        printf("No buses available currently.\n");
        return;
    }
    printf("\n%-10s %-20s %-15s %-15s %-10s %-10s\n", "Bus ID", "Name", "Source", "Dest", "Fare", "Seats Left");
    printf("-------------------------------------------------------------------------------------\n");
    for (int i = 0; i < bus_count; i++) {
        printf("%-10d %-20s %-15s %-15s %-10.2f %-10d\n", 
            buses[i].bus_id, buses[i].bus_name, buses[i].source, buses[i].destination, buses[i].fare, buses[i].available_seats);
    }
}

void showSeatMap(Bus b) {
    printf("\n--- Seat Map for %s (ID: %d) ---\n", b.bus_name, b.bus_id);
    printf("(0 = Empty, 1 = Booked)\n\n");
    
    for (int i = 0; i < MAX_SEATS; i++) {
        if (b.seats[i] == 0) 
            printf("[ %02d ] ", i + 1);
        else 
            printf("[ XX ] "); // Booked

        if ((i + 1) % 4 == 0) printf("\n"); // New row every 4 seats
    }
    printf("\n");
}

void bookTicket() {
    int bus_id, seat_num, index;
    viewAllBuses();
    printf("\nEnter Bus ID to Book: ");
    scanf("%d", &bus_id);

    index = getBusIndex(bus_id);
    if (index == -1) {
        printf("Bus ID not found!\n");
        return;
    }

    if (buses[index].available_seats == 0) {
        printf("Sorry! This bus is fully booked.\n");
        return;
    }

    showSeatMap(buses[index]);
    printf("\nEnter Seat Number (1-%d): ", MAX_SEATS);
    scanf("%d", &seat_num);

    if (seat_num < 1 || seat_num > MAX_SEATS) {
        printf("Invalid Seat Number.\n");
        return;
    }

    if (buses[index].seats[seat_num - 1] == 1) {
        printf("Error: Seat %d is already booked!\n", seat_num);
    } else {
        buses[index].seats[seat_num - 1] = 1; // Mark as booked
        buses[index].available_seats--;
        saveData();
        printf("\nBooking Confirmed!\nBus: %s\nSeat: %d\nFare: %.2f\n", buses[index].bus_name, seat_num, buses[index].fare);
    }
}

void cancelTicket() {
    int bus_id, seat_num, index;
    printf("\nEnter Bus ID: ");
    scanf("%d", &bus_id);

    index = getBusIndex(bus_id);
    if (index == -1) {
        printf("Bus ID not found!\n");
        return;
    }

    printf("\nEnter Seat Number to Cancel: ");
    scanf("%d", &seat_num);

    if (seat_num < 1 || seat_num > MAX_SEATS) {
        printf("Invalid Seat Number.\n");
        return;
    }

    if (buses[index].seats[seat_num - 1] == 0) {
        printf("Error: Seat %d was not booked.\n", seat_num);
    } else {
        buses[index].seats[seat_num - 1] = 0; // Mark as empty
        buses[index].available_seats++;
        saveData();
        printf("Ticket Cancelled Successfully. Refund Amount: %.2f\n", buses[index].fare);
    }
}

// --- Helper Functions ---

int getBusIndex(int id) {
    for (int i = 0; i < bus_count; i++) {
        if (buses[i].bus_id == id) return i;
    }
    return -1;
}

void saveData() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp != NULL) {
        fwrite(&bus_count, sizeof(int), 1, fp);
        fwrite(buses, sizeof(Bus), bus_count, fp);
        fclose(fp);
    } else {
        printf("Error saving data!\n");
    }
}

void loadData() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp != NULL) {
        fread(&bus_count, sizeof(int), 1, fp);
        fread(buses, sizeof(Bus), bus_count, fp);
        fclose(fp);
    }
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}