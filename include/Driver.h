//
// Created by John Charalambous on 08/04/2023.
//

#ifndef DASHBOARD_DRIVER_H
#define DASHBOARD_DRIVER_H
#include "string"

using namespace std;

// Define the Driver class
class Driver {
private:
    string Name;
    string Email;
    string Address;
    string Username;
    string Password;
    string MobileNumber;
    string NINumber;
    string DriversLicence;
    string DriverLocation;
    string LorryLength;
    string LorryWidth;
    string LorryHeight;
    string LorryWeight;
public:
    Driver(string Name, string Email, string Address, string Username, string Password, string MobileNumber, string NINumber, string DriversLicence, string DriverLocation, string LorryLength, string LorryWidth, string LorryHeight, string LorryWeight) {
        this->Name = Name;
        this->Email = Email;
        this->Address = Address;
        this->Username = Username;
        this->Password = Password;
        this->MobileNumber = MobileNumber;
        this->NINumber = NINumber;
        this->DriversLicence = DriversLicence;
        this->DriverLocation = DriverLocation;
        this->LorryLength = LorryLength;
        this->LorryWidth = LorryWidth;
        this->LorryHeight = LorryHeight;
        this->LorryWeight = LorryWeight;

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
    string getNINumber() {
        return NINumber;
    }
    string getDriversLicence() {
        return DriversLicence;
    }
    string getDriverLocation() {
        return DriverLocation;
    }
    string getLorryLength() {
        return LorryLength;
    }
    string getLorryWidth() {
        return LorryWidth;
    }
    string getLorryHeight() {
        return LorryHeight;
    }
    string getLorryWeight() {
        return LorryWeight;
    }
};

void registerDriver();
void viewDriverProfile(string UserID);
void editDriverProfile(string UserID, string Name, string Email, string Username, string Password, string MobileNumber, string Address, string NINumber, string DriversLicence, string DriverLocation, string LorryLength, string LorryWidth, string LorryHeight, string LorryWeight);
//void DriverActivity();
void viewDriversActiveOrders(string UserID);
void viewDriversCompletedOrders();
void viewActiveOrderPT1();
void viewActiveOrderPT2();
void viewDriversNearLocation();
void viewDriverOrders();
//void viewDriverNotifications(int driverId);


#endif //DASHBOARD_DRIVER_H
