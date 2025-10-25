#ifndef HOPPER_H
#define HOPPER_H

#define MAX 100

struct User {
    char username[MAX];
    char password[20];
    char phone[MAX];
    char email[MAX];
    char gender[MAX];
    char emergency[2][MAX];
};

struct Ride {
    char username[MAX];
    char pickup[MAX];
    char drop[MAX];
    char vehiclePreference[MAX];
    char seatPreference[MAX];
    char rideDate[12];      
    char rideTime[6];      
    char additionalNotes[MAX];
};

void validatePhone(char phone[]);
void validateEmail(char email[]);
void saveUser(struct User u);
void inputAdditionalInfo(struct User *user);
struct User signin();
int loginUser(struct User *loggedUser);
void searchRides(char pickup[], char drop[], char date[]);

#endif 
  
