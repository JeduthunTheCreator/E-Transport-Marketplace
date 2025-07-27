#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../include/LoginLogout.h"
#include "../include/CargoOwner.h"
#include "../include/Driver.h"
#include "../include/TransportCompany.h"

using namespace std;

void dash() {

    cout << "Welcome to our E Transportation Service!" << endl;
    cout << "Please choose the type of account you have:" << endl;
    cout << "1. Cargo Owner" << endl;
    cout << "2. Driver" << endl;
    cout << "3. Transport Company User" << endl;

    // Get user account type
    int account_type;
    cin >> account_type;

    cout << "Please choose an option:" << endl;
    cout << "1. Login" << endl;
    cout << "2. Create a new account" << endl;

    // Get user choice
    int choice;
    cin >> choice;

    string UserID, Username;
    if (choice == 1) {
        if (account_type == 1) {
            UserID = loginUser();
            if (!UserID.empty()) {
                cout << "Login successful, You're a Cargo Owner!" << endl;
                cout << "Please choose an option:" << endl;
                cout << "1. View profile" << endl;
                cout << "2. Edit profile" << endl;
                cout << "3. Place Order" << endl;
                cout << "4. View Orders Placed" << endl;
                cout << "5. View fulfilled" << endl;
                cout << "6. View Notfications" << endl;
                cout << "7. View Invoices" << endl;
                int profile_choice;
                cin >> profile_choice;
                if (profile_choice == 1) {
                    cout << "Please see your Account Details" << endl;
                    viewProfile(UserID);
                }
                else if (profile_choice == 2) {
                    cout << "You can make changes to your Account Details Here" << endl;
                    string Name; string Email; string Password; string MobileNumber; string Address;
                    cout << "Name" << endl; cin >> Name;
                    cout << "Email" << endl; cin >> Email;
                    cout << "Password" << endl; cin >> Password;
                    cout << "Mobile Number" << endl; cin >> MobileNumber;
                    cout << "Address" << endl; cin >> Address;

                    editProfile(UserID, Name, Email, Password, MobileNumber, Address);
                }
                else if (profile_choice == 3){
                    cout << "Please Provide us with some information about your cargo" << endl;
                    placeOrder(UserID);
                }
                else if (profile_choice == 4) {
                    cout << "Please see your active orders" << endl;
                    viewOrders(UserID);

                }
                else if (profile_choice == 5) {
                    cout << "Please see your fulfilled orders" << endl;
                    viewfulfilledOrders(UserID);
                }
                else if (profile_choice == 6) {
                    cout << "Please see any notifictions" << endl;
                    viewNotifications();
                }
                else if (profile_choice == 7) {
                    cout << "Please see any invoices" << endl;
                    viewCustomerCargoInvoices(UserID);
                }
                else {
                    cout << "Invalid choice." << endl;
                }
            } else {
                cout << "Login failed." << endl;
            }
        }

        else if (account_type == 2) {
            UserID = loginDriver();
            if (!UserID.empty()) {
                cout << "Login successful, You're a Driver!" << endl;
                cout << "Please choose an option:" << endl;
                cout << "1. View profile" << endl;
                cout << "2. Edit profile" << endl;
                cout << "3. View Cargo Order" << endl;
                cout << "4. Mark order as delivered" << endl;
                cout << "5. View Drivers completed Cargo Orders" << endl;
                int profile_choice;
                cin >> profile_choice;
                if (profile_choice == 1) {
                    cout << "Please see your Account Details" << endl;
                    viewDriverProfile(UserID);
                }
                else if (profile_choice == 2) {
                    cout << "You can make changes to your Driver Details Here" << endl;
                    string Name; string Email; string Username; string Password; string MobileNumber; string Address; string NINumber; string DriversLicence; string DriverLocation; string LorryLength; string LorryWidth; string LorryHeight; string LorryWeight;
                    cout << "Name" << endl; cin >> Name;
                    cout << "Email" << endl; cin >> Email;
                    cout << "Username" << endl; cin >> Username;
                    cout << "Password" << endl; cin >> Password;
                    cout << "MobileNumber" << endl; cin >> MobileNumber;
                    cout << "Address" << endl; cin >> Address;
                    cout << "NINumber" << endl; cin >> NINumber;
                    cout << "DriversLicence" << endl; cin >> DriversLicence;
                    cout << "DriverLocation" << endl; cin >> DriverLocation;
                    cout << "LorryLength" << endl; cin >> LorryLength;
                    cout << "LorryWidth" << endl; cin >> LorryWidth;
                    cout<< "LorryHeight" << endl; cin >> LorryHeight;
                    cout<< "LorryWeight" << endl; cin >> LorryWeight;

                    editDriverProfile(UserID, Name, Email, Username, Password, MobileNumber, Address, NINumber, DriversLicence, DriverLocation, LorryLength, LorryWidth, LorryHeight, LorryWeight);
                }
                else if (profile_choice == 3) {
                    cout << "Please choose if you're active to accept orders" << endl;
                    viewDriverOrders();
                }
                else if (profile_choice == 4) {
                    cout << "Please see you're active orders" << endl;
                    int driverId;
                    viewDriversActiveOrders(UserID);
                }
                else if (profile_choice == 5) {
                    cout << "Please see you're fulfilled orders" << endl;
                    viewDriversCompletedOrders();
                }
                else {
                    cout << "Invalid choice." << endl;
                }
            } else {
                cout << "Login failed." << endl;
            }
        }

        else if (account_type == 3) {
            UserID = loginTransport();
            if (!UserID.empty()) {
                cout << "Login successful, You're a Transportation Company!" << endl;
                cout << "Please choose an option:" << endl;
                cout << "1. View Profile" << endl;
                cout << "2. Edit Profile" << endl;
                cout << "3. View Customer Orders" << endl;
                cout << "4. Send Order to Available Driver" << endl;
                cout << "5. Customer Invoices" << endl;
                cout << "6. Calculate Commissions per In-Transit Order" << endl;
                cout << "7. Calculate Commissions per Delivered Order" << endl;

                int profile_choice;
                cin >> profile_choice;
                if (profile_choice == 1) {
                    cout << "Please see your Account Details" << endl;
                    viewTransportProfile(UserID);
                }
                else if (profile_choice == 2) {
                    cout << "Editing profile..." << endl;
                    string UserID; string Name; string Email; string Password; string MobileNumber; string Address; string CompanyName;
                    cout << "Name" << endl; cin >> Name;
                    cout << "Email" << endl; cin >> Email;
                    cout << "Password" << endl; cin >> Password;
                    cout << "Mobile Number" << endl; cin >> MobileNumber;
                    cout << "Address" << endl; cin >> Address;
                    cout << "Company Name" << endl; cin >> CompanyName;

                    editTransportProfile(UserID, Name, Email, Password, MobileNumber, Address, CompanyName);
                }
                else if (profile_choice == 3) {
                    cout << "Please see customer orders" << endl;
                    viewDriverOrders();
                }
                else if (profile_choice == 4) {
                    cout << "Invoices issued to cargo owners" << endl;
                    int OrderID;
                    viewAvailableDriver();
                }
                else if (profile_choice == 5) {
                    cout << "Invoices issued to cargo owners" << endl;
                    int OrderID;
                    viewCustomerInvoices(OrderID);
                }
                else if (profile_choice == 6) {
                    cout << "Commisions from cargo deliveries In Transit" << endl;
                    calculateCommissions();
                }
                else if (profile_choice == 7) {
                    cout << "Commisions from cargo deliveries Delivered" << endl;
                    calculateCommissionsForDeliveredOrders();
                }
                else {
                    cout << "Invalid choice." << endl;
                }
            } else {
                cout << "Login failed." << endl;
            }
        }

    } else if (choice == 2) {
        if (account_type == 1) {
            registerUser();
        }
        else if (account_type == 2) {
            registerDriver();
        }
        else if (account_type == 3) {
            registerTransport();
        }
        else {
            cout << "Invalid account type. Please try again." << endl;
        }

    } else {
        cout << "Invalid choice." << endl;
    }

    cout << "Do you want to log out? (y/n)";
    char logout_choice;
    cin >> logout_choice;
}
