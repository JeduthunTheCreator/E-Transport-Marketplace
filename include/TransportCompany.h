//
// Created by John Charalambous on 08/04/2023.
//

#ifndef DASHBOARD_TRANSPORTCOMPANY_H
#define DASHBOARD_TRANSPORTCOMPANY_H
#include "string"

using namespace std;

// Define the Driver class
class TransportCompany {
private:
    string Name;
    string Email;
    string Address;
    string Username;
    string Password;
    string MobileNumber;
    string CompanyName;
public:
    TransportCompany(string Name, string Email, string Address, string Username, string Password, string MobileNumber, string CompanyName) {
        this->Name = Name;
        this->Email = Email;
        this->Address = Address;
        this->Username = Username;
        this->Password = Password;
        this->MobileNumber = MobileNumber;
        this->CompanyName = CompanyName;
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
    string getCompanyName() {
        return CompanyName;
    }
};

void registerTransport();
void viewTransportProfile(string UserID);
void editTransportProfile(string UserID, string Name, string Email, string Password, string MobileNumber, string Address, string CompanyName);

void viewOrder();
void viewCustomerOrders();
void viewAvailableDriver();
void viewDriversNearLocation();
void viewCustomerInvoices(int OrderID);
void calculateCommissions();
void calculateCommissionsForDeliveredOrders();

#endif //DASHBOARD_TRANSPORTCOMPANY_H
