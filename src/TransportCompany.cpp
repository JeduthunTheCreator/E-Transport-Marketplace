#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "include/TransportCompany.h"

using namespace std;

// Define a callback function to handle SQLite query results
static int callback(void* data, int argc, char** argv, char** azColName) {
    // Print out the query results
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << argv[i] << endl;
    }
    return 0;
}

void registerTransport() {

    string Name, Email, Username, Password, MobileNumber, Address, CompanyName;

    cout << "Enter your name: ";
    getline(cin, Name);
    cout << "Enter your email: ";
    getline(cin, Email);
    cout << "Enter a username: ";
    getline(cin, Username);

    bool validPassword = false;
    while (!validPassword) {
        cout << "Enter a password (at least 6 characters with at least one capital letter, one lowercase letter, one digit, and one special symbol): ";
        getline(cin, Password);

        if (Password.length() < 6) {
            cout << "Password must be at least 6 characters long." << endl;
            continue;
        }

        bool hasCapital = false, hasLowercase = false, hasDigit = false, hasSpecial = false;
        for (char c : Password) {
            if (isupper(c)) {
                hasCapital = true;
            } else if (islower(c)) {
                hasLowercase = true;
            } else if (isdigit(c)) {
                hasDigit = true;
            } else if (ispunct(c)) {
                hasSpecial = true;
            }
        }

        if (!(hasCapital && hasLowercase && hasDigit && hasSpecial)) {
            cout << "Password must contain at least one capital letter, one lowercase letter, one digit, and one special symbol." << endl;
            continue;
        }

        validPassword = true;
    }

    bool isMobileNumberValid = false;
    do {
        cout << "Enter a mobile number (must be at least 11 digits long): ";
        getline(cin, MobileNumber);

        // Check phone number length
        if (MobileNumber.length() < 11) {
            cerr << "Mobile number must be at least 11 digits long." << endl;
            continue;
        }

        isMobileNumberValid = true;
    } while (!isMobileNumberValid);
    cout << "Enter your address: ";
    getline(cin, Address);
    cout << "Enter your company name: ";
    getline(cin, CompanyName);

    // Create new CargoOwner object
    TransportCompany transportCompany = TransportCompany(Name, Email, Address, Username, Password, MobileNumber, CompanyName);

    // Open SQLite database
    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Create table if it doesn't exist
    const char *createTableSql = "CREATE TABLE IF NOT EXISTS TransportCompany ("
                                 "UserID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT NOT NULL,"
                                 "Email TEXT NOT NULL,"
                                 "Username TEXT NOT NULL UNIQUE,"
                                 "Password TEXT NOT NULL,"
                                 "MobileNumber TEXT NOT NULL,"
                                 "Address TEXT NOT NULL)";

    char *errMsg;
    rc = sqlite3_exec(db, createTableSql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to insert CargoOwner data
    const char *insertSql = "INSERT INTO TransportCompany (Name, Email, Username, Password, MobileNumber, Address, CompanyName) VALUES (?, ?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind CargoOwner data to prepared statement
    rc = sqlite3_bind_text(stmt, 1, transportCompany.getName().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 2, transportCompany.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 3, transportCompany.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 4, transportCompany.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 5, transportCompany.getMobileNumber().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 6, transportCompany.getAddress().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 7, transportCompany.getCompanyName().c_str(), -1, SQLITE_TRANSIENT);


    // Execute the statement
    if (rc != SQLITE_OK) {
        cerr << "Error binding values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
    }
    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
};
// Creating user account for Cargo Owner END!!!

// Viewing user account for Transport Company Start!!!
void viewTransportProfile(string UserID) {

    // Open SQLite database
    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to select user data
    const char *selectSql = "SELECT * FROM TransportCompany WHERE UserID=?";
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind ID parameter to prepared statement
    rc = sqlite3_bind_text(stmt, 1, UserID.c_str(), -1, SQLITE_TRANSIENT);
    cout << "Bound parameter: " << UserID << endl; // debug

    // Execute the statement
    rc = sqlite3_step(stmt);
    cout << "Step result: " << rc << endl; // debug


    if (rc == SQLITE_ROW) {
        // Print out the user data
        cout << "UserID: " << sqlite3_column_int(stmt, 0) << endl;
        cout << "Name: " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Email: " << sqlite3_column_text(stmt, 2) << endl;
        cout << "Username: " << sqlite3_column_text(stmt, 3) << endl;
        cout << "Password: " << sqlite3_column_text(stmt, 4) << endl;
        cout << "Mobile Number: " << sqlite3_column_text(stmt, 5) << endl;
        cout << "Address: " << sqlite3_column_text(stmt, 6) << endl;
        cout << "CompanyName: " << sqlite3_column_text(stmt, 7) << endl;
    } else {
        cout << "User not found" << endl;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Viewing user account for Transport Company END!!!

// Editing user account for Transport Company START!!!
void editTransportProfile(string UserID, string Name, string Email, string MobileNumber, string Address, string CompanyName, string Password) {
    // Open SQLite database
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to update user data
    const char *updateSql = "UPDATE TransportCompany SET Name=?, Email=?, MobileNumber=?, Address=?, CompanyName=?, Password=? WHERE ID=?";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, updateSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind parameters to prepared statement
    rc = sqlite3_bind_text(stmt, 1, Name.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, Email.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 3, MobileNumber.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 4, Address.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 5, CompanyName.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 6, Password.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 7, UserID.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        cerr << "Error updating user data: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Editing user account for Transport Company END!!!



void viewCustomerOrders() {
    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve all orders from the table
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Iterate over the results and display the order details
    int orderCount = 0;
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        orderCount++;
        int UserID = sqlite3_column_int(stmt, 0);
        int OrderID = sqlite3_column_int(stmt, 1);
        string Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int Quantity = sqlite3_column_int(stmt, 3);
        double Weight = sqlite3_column_double(stmt, 4);
        int Length = sqlite3_column_int(stmt, 5);
        int Width = sqlite3_column_int(stmt, 6);
        int Height = sqlite3_column_int(stmt, 7);
        int IsFrozen = sqlite3_column_int(stmt, 8);
        int IsFragile = sqlite3_column_int(stmt, 9);
        string Source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        string Destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        int Distance = sqlite3_column_int(stmt, 12);
        string ShippingMethod = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        string LorryType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));

        cout << "UserID: " << UserID << endl;
        cout << "Order ID: " << OrderID << endl;
        cout << "Name: " << Name << endl;
        cout << "Quantity: " << Quantity << endl;
        cout << "Weight: " << Weight << endl;
        cout << "Length: " << Length << endl;
        cout << "Height: " << Height << endl;
        cout << "Width: " << Width << endl;
        cout << "IsFrozen: " << IsFrozen << endl;
        cout << "IsFragile: " << IsFragile << endl;
        cout << "Source: " << Source << endl;
        cout << "Destination: " << Destination << endl;
        cout << "Distance: " << Distance << endl;
        cout << "ShippingMethod: " << ShippingMethod << endl;
        cout << "Shipping Rate: " << sqlite3_column_double(stmt, 14) << endl;
        cout << "LorryType: " << LorryType << endl;
        cout << "---------------------------------------" << endl;

        rc = sqlite3_step(stmt);
    }

    // Print the order count
    cout << "Total number of orders: " << orderCount << endl;

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void viewAvailableDriver() {
    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve all orders from the table
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Iterate over the results and display the order details
    int orderCount = 0;
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        orderCount++;
        int UserID = sqlite3_column_int(stmt, 0);
        int OrderID = sqlite3_column_int(stmt, 1);
        string Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int Quantity = sqlite3_column_int(stmt, 3);
        double Weight = sqlite3_column_double(stmt, 4);
        int Length = sqlite3_column_int(stmt, 5);
        int Width = sqlite3_column_int(stmt, 6);
        int Height = sqlite3_column_int(stmt, 7);
        int IsFrozen = sqlite3_column_int(stmt, 8);
        int IsFragile = sqlite3_column_int(stmt, 9);
        string Source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        string Destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        int Distance = sqlite3_column_int(stmt, 12);
        string ShippingMethod = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        string LorryType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));

        cout << "UserID: " << UserID << endl;
        cout << "Order ID: " << OrderID << endl;
        cout << "Source: " << Source << endl;
        cout << "Destination: " << Destination << endl;
        cout << "---------------------------------------" << endl;

        rc = sqlite3_step(stmt);
    }

    // Print the order count
    cout << "Total number of orders: " << orderCount << endl;

    // Prompt the user if they want to view a specific order
    cout << "Do you want to view a specific order? (y/n): ";
    cin.ignore();
    string response;
    getline(cin, response);
    if (response == "y") {
        viewOrder();
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void viewOrder() {

    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Ask user for Order ID to search for
    int orderID;
    cout << "Enter an Order ID to view details: ";
    cin >> orderID;
    cin.ignore(); // Consume the newline character left in the input stream


    // Prepare the SQL statement to retrieve the order with the given Order ID
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders WHERE OrderID = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the Order ID parameter to the statement
    rc = sqlite3_bind_int(stmt, 1, orderID);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Retrieve the details of the order with the given Order ID
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int UserID = sqlite3_column_int(stmt, 0);
        int OrderID = sqlite3_column_int(stmt, 1);
        string Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int Quantity = sqlite3_column_int(stmt, 3);
        double Weight = sqlite3_column_double(stmt, 4);
        int Length = sqlite3_column_int(stmt, 5);
        int Width = sqlite3_column_int(stmt, 6);
        int Height = sqlite3_column_int(stmt, 7);
        int IsFrozen = sqlite3_column_int(stmt, 8);
        int IsFragile = sqlite3_column_int(stmt, 9);
        string Source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        string Destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        int Distance = sqlite3_column_int(stmt, 12);
        string ShippingMethod = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        string LorryType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));

        cout << "UserID: " << UserID << endl;
        cout << "Name: " << Name << endl;
        cout << "Quantity: " << Quantity << endl;
        cout << "Weight: " << Weight << endl;
        cout << "Length: " << Length << endl;
        cout << "Height: " << Height << endl;
        cout << "Width: " << Width << endl;
        cout << "IsFrozen: " << IsFrozen << endl;
        cout << "IsFragile: " << IsFragile << endl;
        cout << "Source: " << Source << endl;
        cout << "Destination: " << Destination << endl;
        cout << "Distance: " << Distance << endl;
        cout << "ShippingMethod: " << ShippingMethod << endl;
        cout << "Shipping Rate: " << sqlite3_column_double(stmt, 14) << endl;
        cout << "LorryType: " << LorryType << endl;
        cout << "---------------------------------------" << endl;

        rc = sqlite3_step(stmt);
    }

    // Prompt the user if they want to view a specific order
    cout << "Do you want to view available drivers (y/n): ";
    string response;
    getline(cin, response);
    if (response == "y") {
        viewDriversNearLocation();
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void viewCustomerInvoices(int OrderID) {
    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement
    const char* sql = "SELECT OrderID, Name, Source, Destination, Distance, Weight, Status FROM CargoOrders WHERE Status IN ('In Transit', 'Delivered');";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Execute the statement and print the results
    int result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        cout << "No orders currently in transit or delivered" << endl;
    } else {
        double ratePerKg = 0.5;
        double ratePerKm = 0.01;

        cout << "Invoice" << endl;
        cout << "-------------------------" << endl;
        while (result == SQLITE_ROW) {
            int orderID = sqlite3_column_int(stmt, 0);
            const unsigned char* name = sqlite3_column_text(stmt, 1);
            const unsigned char* source = sqlite3_column_text(stmt, 2);
            const unsigned char* destination = sqlite3_column_text(stmt, 3);
            int distance = sqlite3_column_int(stmt, 4);
            int weight = sqlite3_column_int(stmt, 5);
            const unsigned char* status = sqlite3_column_text(stmt, 6);

            double totalCost = weight * ratePerKg + distance * ratePerKm;

            cout << "Order ID: " << orderID << endl;
            cout << "Name: " << name << endl;
            cout << "Source: " << source << endl;
            cout << "Destination: " << destination << endl;
            cout << "Weight: " << weight << " kg" << endl;
            cout << "Distance: " << distance << " km" << endl;
            cout << "Rate per kg: $" << ratePerKg << endl;
            cout << "Rate per km: $" << ratePerKm << endl;
            cout << "Total cost: $" << totalCost << endl;
            cout << "Status: " << status << endl;
            cout << "-------------------------" << endl;

            result = sqlite3_step(stmt);
        }
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// Calculate commision for all orders START!!!
void calculateCommissions() {

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement
    const char* sql = "SELECT OrderID, Name, Source, Destination, Distance, Weight, ShippingRate FROM CargoOrders WHERE Status = 'In Transit';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Update the Commissions and FinalCost columns for each order
    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int orderID = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* source = sqlite3_column_text(stmt, 2);
        const unsigned char* destination = sqlite3_column_text(stmt, 3);
        int distance = sqlite3_column_int(stmt, 4);
        int weight = sqlite3_column_int(stmt, 5);
        double shippingRate = sqlite3_column_double(stmt, 6);

        double commission = shippingRate * 0.1;
        double finalCost = shippingRate + commission;

        // Prepare the SQL statement to update the Commissions and FinalCost columns
        const char* updateSql = "UPDATE CargoOrders SET Commission = ?, FinalCost = ? WHERE OrderID = ?;";
        sqlite3_stmt* updateStmt;
        rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
        if (rc != SQLITE_OK) {
            cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Bind the parameters and execute the update statement
        rc = sqlite3_bind_double(updateStmt, 1, commission);
        if (rc != SQLITE_OK) {
            cerr << "Error binding commission parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_double(updateStmt, 2, finalCost);
        if (rc != SQLITE_OK) {
            cerr << "Error binding final cost parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_int(updateStmt, 3, orderID);
        if (rc != SQLITE_OK) {
            cerr << "Error binding order ID parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_step(updateStmt);
        if (rc != SQLITE_DONE) {
            cerr << "Error updating Commission and FinalCost columns: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Finalize the update statement
        sqlite3_finalize(updateStmt);

        cout << "Order ID: " << orderID << endl;
        cout << "Name: " << name << endl;
        cout << "Source: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Weight: " << weight << " kg" << endl;
        cout << "Distance: " << distance << " km" << endl;
        cout << "Commission: £" << commission << endl;
        cout << "Total cost: £" << finalCost << endl;
        cout << "-------------------------" << endl;

    }

// Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
// Calculate commission for all orders END!!!

void calculateCommissionsForDeliveredOrders() {

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement
    const char* sql = "SELECT OrderID, Name, Source, Destination, Distance, Weight, ShippingRate FROM CargoOrders WHERE Status = 'Delivered';";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Update the Commissions and FinalCost columns for each order
    while (sqlite3_step(stmt) == SQLITE_ROW) {

        int orderID = sqlite3_column_int(stmt, 0);
        const unsigned char* name = sqlite3_column_text(stmt, 1);
        const unsigned char* source = sqlite3_column_text(stmt, 2);
        const unsigned char* destination = sqlite3_column_text(stmt, 3);
        int distance = sqlite3_column_int(stmt, 4);
        int weight = sqlite3_column_int(stmt, 5);
        double shippingRate = sqlite3_column_double(stmt, 6);

        double commission = shippingRate * 0.1;
        double finalCost = shippingRate + commission;

        // Prepare the SQL statement to update the Commissions and FinalCost columns
        const char* updateSql = "UPDATE CargoOrders SET Commission = ?, FinalCost = ? WHERE OrderID = ?;";
        sqlite3_stmt* updateStmt;
        rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
        if (rc != SQLITE_OK) {
            cerr << "Error preparing update statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Bind the parameters and execute the update statement
        rc = sqlite3_bind_double(updateStmt, 1, commission);
        if (rc != SQLITE_OK) {
            cerr << "Error binding commission parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_double(updateStmt, 2, finalCost);
        if (rc != SQLITE_OK) {
            cerr << "Error binding final cost parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_int(updateStmt, 3, orderID);
        if (rc != SQLITE_OK) {
            cerr << "Error binding order ID parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_step(updateStmt);
        if (rc != SQLITE_DONE) {
            cerr << "Error updating Commission and FinalCost columns: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        // Finalize the update statement
        sqlite3_finalize(updateStmt);

        cout << "Order ID: " << orderID << endl;
        cout << "Name: " << name << endl;
        cout << "Source: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Weight: " << weight << " kg" << endl;
        cout << "Distance: " << distance << " km" << endl;
        cout << "Commission: £" << commission << endl;
        cout << "Total cost: £" << finalCost << endl;
        cout << "-------------------------" << endl;

    }

// Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
// Calculate commission for all orders END!!!
