# Bus-System
Allow users to booked specific seats and generates a ticket with a unique PNR .
 # Bus / Train Ticket Reservation System (C)

A simple console-based Bus/Train Ticket Reservation System written in C.
It supports Admin and User modes, persistent storage using a binary data file, and a seat matrix for booking and cancellation.
# Features 

Text-based console interface.

Persistent data storage in buses.dat (binary file).

Auto-generated Bus IDs.

Seat matrix for each bus (up to 32 seats).

Maximum 50 buses in this demo implementation.

Admin Mode

Password-protected access (ADMIN_PASS in code, currently "admin123").

Add new bus routes:

Bus name

Source

Destination

Fare

View all existing routes.

Delete all data (reset system).

User Mode

View all available buses with:

Bus ID

Name

Source

Destination

Fare

Seats left

Book ticket on a selected bus:

See seat map (shows booked/empty seats).

Choose seat number.

Cancel ticket:

Provide Bus ID and Seat Number.

Refund amount is displayed (same as fare).

Tech Stack

Language: C

Standard Libraries: stdio.h, stdlib.h, string.h
# Output 
<img width="785" height="358" alt="image" src="https://github.com/user-attachments/assets/711a204d-7fc3-4371-a520-2ce29d5dd3eb" />

Platform-Specific: windows.h (for Windows builds)

Data Storage: Binary file buses.dat
