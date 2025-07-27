//
// Created by John Charalambous on 07/04/2023.
//

#ifndef DASHBOARD_CARGOOWNER_H
#define DASHBOARD_CARGOOWNER_H
#include "string"

using namespace std;

void registerUser();

// Define the Driver class
class CargoOwner {
private:
    string Name;
    string Email;
    string Address;
    string Username;
    string Password;
    string MobileNumber;
public:
    CargoOwner(string Name, string Email, string Address, string Username, string Password, string MobileNumber) {
        this->Name = Name;
        this->Email = Email;
        this->Address = Address;
        this->Username = Username;
        this->Password = Password;
        this->MobileNumber = MobileNumber;
    }
    string getName() {
        return Name;
    }
    string getEmail() {
        return Email;
    }
    string getAddress() {
        return Address;
    }
    string getUsername() {
        return Username;
    }
    string getPassword() {
        return Password;
    }
    string getMobileNumber() {
        return MobileNumber;
    }
};

void viewProfile(string UserID);

void editProfile(string UserID, string Name, string Email, string Password, string MobileNumber, string Address);

void placeOrder(string UserID);

void viewOrders(string UserID);

void viewfulfilledOrders(string UserID);

void viewNotifications();

void viewCustomerCargoInvoices(string UserID);

#endif //DASHBOARD_CARGOOWNER_H
