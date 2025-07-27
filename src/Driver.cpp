#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "../include/Driver.h"

using namespace std;

// Define a callback function to handle SQLite query results
static int callback(void* data, int argc, char** argv, char** azColName) {
    // Print out the query results
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << argv[i] << endl;
    }
    return 0;
}

void registerDriver() {
    string Name, Email, Username, Password, MobileNumber, Address, NINumber, DriversLicence, DriverLocation, LorryLength, LorryWidth, LorryHeight, LorryWeight;

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
    cout << "Enter NI Number: ";
    getline(cin, NINumber);
    cout << "Enter Drivers Licence: ";
    getline(cin, DriversLicence);
    cout << "Enter Drivers Location: ";
    getline(cin, DriverLocation);
    cout << "Lorrys Length in Meters ";
    getline(cin, LorryLength);
    cout << "Lorrys Width in Meters ";
    getline(cin, LorryWidth);
    cout << "Lorrys Height in Meters ";
    getline(cin, LorryHeight);
    cout << "Enter Drivers Lorry Weight in Tons";
    getline(cin, LorryWeight);

    // Create new driver object
    Driver driver = Driver(Name, Email, Address, Username, Password, MobileNumber, NINumber, DriversLicence, DriverLocation, LorryLength, LorryWidth, LorryHeight, LorryWeight);

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
    const char *createTableSql = "CREATE TABLE IF NOT EXISTS Driver ("
                                 "UserID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                 "Name TEXT NOT NULL,"
                                 "Email TEXT NOT NULL,"
                                 "Username TEXT NOT NULL UNIQUE,"
                                 "Password TEXT NOT NULL,"
                                 "MobileNumber TEXT NOT NULL,"
                                 "Address TEXT NOT NULL,"
                                 "NINumber TEXT NOT NULL UNIQUE,"
                                 "DriversLicence TEXT NOT NULL UNIQUE,"
                                 "DriverLocation TEXT NOT NULL,"
                                 "LorryLength INT NOT NULL,"
                                 "LorryWidth INT NOT NULL,"
                                 "LorryHeight INT NOT NULL,"
                                 "LorryWeight INT NOT NULL);";

    char *errMsg;
    rc = sqlite3_exec(db, createTableSql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to insert CargoOwner data
    const char *insertSql = "INSERT INTO Driver (Name, Email, Username, Password, MobileNumber, Address, NINumber, DriversLicence, DriverLocation, LorryLength, LorryWidth, LorryHeight, LorryWeight) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind values to SQL statement
    sqlite3_bind_text(stmt, 1, driver.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, driver.getEmail().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, driver.getUsername().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, driver.getPassword().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, driver.getMobileNumber().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, driver.getAddress().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, driver.getNINumber().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, driver.getDriversLicence().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, driver.getDriverLocation().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, driver.getLorryLength().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 11, driver.getLorryWidth().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 12, driver.getLorryHeight().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 13, driver.getLorryWeight().c_str(), -1, SQLITE_STATIC);

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
}

// Viewing user account for Driver Start!!!
void viewDriverProfile(string UserID) {

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
    const char *selectSql = "SELECT * FROM Driver WHERE UserID=?";
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind UserID parameter to prepared statement
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
        cout << "MobileNumber: " << sqlite3_column_text(stmt, 5) << endl;
        cout << "Address: " << sqlite3_column_text(stmt, 6) << endl;
        cout << "NINumber: " << sqlite3_column_int(stmt, 7) << endl;
        cout << "DriversLicence: " << sqlite3_column_text(stmt, 8) << endl;
        cout << "DriverLocation: " << sqlite3_column_text(stmt, 9) << endl;
        cout << "LorryLength: " << sqlite3_column_text(stmt, 10) << endl;
        cout << "LorryWidth: " << sqlite3_column_text(stmt, 11) << endl;
        cout << "LorryHeight: " << sqlite3_column_text(stmt, 12) << endl;
        cout << "Weight: " << sqlite3_column_text(stmt, 13) << endl;

    } else {
        cout << "User not found" << endl;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Viewing user account for Driver END!!!

// Editing user account for Driver START!!!
void editDriverProfile(string UserID, string Name, string Email, string Username, string Password, string MobileNumber, string Address, string NINumber, string DriversLicence, string DriverLocation, string LorryLength, string LorryWidth, string LorryHeight, string LorryWeight) {
    // Open SQLite database
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to update user data
    const char *updateSql = "UPDATE Driver SET Name=?, Email=?, Username=?, Password=?, MobileNumber=?, Address=?, NINumber=?, DriversLicence=?, DriverLocation=?, LorryLength=?, LorryWidth=?, LorryHeight=?, LorryWeight=? WHERE UserID=?";
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
    rc = sqlite3_bind_text(stmt, 3, Username.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 4, Password.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 5, MobileNumber.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 6, Address.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 7, NINumber.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 8, DriversLicence.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 9, DriverLocation.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 10, LorryLength.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 11, LorryWidth.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 12, LorryHeight.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 13, LorryWeight.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 14, UserID.c_str(), -1, SQLITE_TRANSIENT);

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
// Editing user account for Driver END!!!

// Driver Activity START!!!
void viewDriversNearLocation() {
// Get the user input for the location
    string location;
    cout << "Enter the location: ";
    cin >> location;

// Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

// Prepare the SQL statement to retrieve the drivers in the given location
    sqlite3_stmt *stmt;
    const char *sql = "SELECT UserID, Name FROM Driver WHERE DriverLocation = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

// Bind the user input to the parameter in the SQL statement
    rc = sqlite3_bind_text(stmt, 1, location.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

// Execute the SQL statement and retrieve the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result set
        found = true;
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        cout << "See drivers available to pick up from this location" << endl;
        cout << "Driver UserID: " << id << ", Name: " << name << endl;
    }

// Check if any drivers were found
    if (!found) {
        cout << "No drivers available at this location." << endl;
    } else {
        // Prompt the user to select a driver
        int driverId;
        cout << "Enter the UserID of the driver you want to select: ";
        cin >> driverId;

        // Prompt the user to enter the cargo order ID
        int cargoOrderId;
        cout << "Enter the ID of the cargo order: ";
        cin >> cargoOrderId;

        // Prepare the SQL statement to update the Driver table
        const char *updateSql = "UPDATE CargoOrders SET DriverAssigned = ? WHERE UserID = ?";
        sqlite3_stmt *updateStmt;
        rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
        if (rc != SQLITE_OK) {
            cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }

        // Bind the cargo order ID and driver UserID to the parameters in the SQL statement
        rc = sqlite3_bind_int(updateStmt, 1, driverId);
        if (rc != SQLITE_OK) {
            cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_int(updateStmt, 2, cargoOrderId);
        if (rc != SQLITE_OK) {
            cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(updateStmt);
            sqlite3_close(db);
            return;
        }

        // Execute the SQL statement
        rc = sqlite3_step(updateStmt);
        if (rc != SQLITE_DONE) {
            cerr << "Error updating database: " << sqlite3_errmsg(db) << endl;
        } else {
            cout << "Cargo order assigned to driver successfully." << endl;
        }

        // Print the selected driver's UserID and Name
        cout << "Selected driver for this shipment - UserID: " << driverId << ", Name: ";
        const char *selectSql = "SELECT Name FROM Driver WHERE UserID = ?";
        sqlite3_stmt *selectStmt;
        rc = sqlite3_prepare_v2(db, selectSql, -1, &selectStmt, NULL);
        if (rc != SQLITE_OK) {
            cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_bind_int(selectStmt, 1, driverId);
        if (rc != SQLITE_OK) {
            cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(selectStmt);
            sqlite3_close(db);
            return;
        }
        if (sqlite3_step(selectStmt) == SQLITE_ROW) {
            const unsigned char *name = sqlite3_column_text(selectStmt, 0);
            cout << name;
        }
        sqlite3_finalize(selectStmt);
        cout << endl;

        // Clean up
        sqlite3_finalize(updateStmt);
    }

// Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Driver Activity END!!!

// View You're INCOMING ORDERS START!!!
void viewDriverOrders() {
    // Get the user input for the UserID
    int userID;
    cout << "Enter your UserID: ";
    cin >> userID;

    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve the assigned orders for the driver
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders WHERE DriverAssigned = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the user input to the parameter in the SQL statement
    rc = sqlite3_bind_int(stmt, 1, userID);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the SQL statement and retrieve the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result set
        found = true;
        int id = sqlite3_column_int(stmt, 1);
        const unsigned char *description = sqlite3_column_text(stmt, 2);
        const unsigned char *origin = sqlite3_column_text(stmt, 10);
        const unsigned char *destination = sqlite3_column_text(stmt, 11);
        int weight = sqlite3_column_int(stmt, 4);
        int driverAssigned = sqlite3_column_int(stmt, 5);
        int length = sqlite3_column_int(stmt, 5);
        int width = sqlite3_column_int(stmt, 6);
        int height = sqlite3_column_int(stmt, 7);
        cout << "Cargo Order ID: " << id << endl;
        cout << "Description: " << description << endl;
        cout << "Origin: " << origin << endl;
        cout << "Destination: " << destination << endl;
        cout << "Dimensions: " << length << "cm x " << width << "cm x " << height << "cm" << endl;
        cout << "Weight: " << weight << "kg" << endl;
        cout << endl;

// Prompt the driver to accept or reject the order
        char choice;
        do {
            cout << "Do you accept this order? (Y/N): ";
            cin >> choice;
        } while (toupper(choice) != 'Y' && toupper(choice) != 'N');

// Debugging statement
        cout << "User input: " << choice << endl;

// Update the order status based on the driver's choice
        if (toupper(choice) == 'Y') {
            // Debugging statement
            cout << "Driver accepted the order" << endl;

            // Update the order status to "In Transit"
            const char *updateSql = "UPDATE CargoOrders SET Status = ? WHERE DriverAssigned = ?";
            sqlite3_stmt *updateStmt;
            rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
            if (rc != SQLITE_OK) {
                // error handling code
            }
            rc = sqlite3_bind_text(updateStmt, 1, "In Transit", -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_int(updateStmt, 2, id);
            if (rc != SQLITE_OK) {
                // error handling code
            }

            rc = sqlite3_step(updateStmt);
            if (rc != SQLITE_DONE) {
                // error handling code
            }

            rc = sqlite3_step(updateStmt);
            if (rc != SQLITE_DONE) {
                cerr << "Error updating order status: " << sqlite3_errmsg(db) << endl;
                sqlite3_finalize(updateStmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_finalize(updateStmt);
        } else {
            // Debugging statement
            cout << "Driver rejected the order" << endl;

            // Update the order status to "Available"
            const char *updateSql = "UPDATE CargoOrders SET DriverAssigned = NULL, Status = ? WHERE OrderID = ?";
            sqlite3_stmt *updateStmt;
            rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
            if (rc != SQLITE_OK) {
                cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
                sqlite3_close(db);
                return;
            }
            rc = sqlite3_bind_text(updateStmt, 1, "Available", -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_int(updateStmt, 2, id);
            if (rc != SQLITE_OK) {
                cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
                sqlite3_finalize(updateStmt);
                sqlite3_close(db);
                return;
            }

            rc = sqlite3_step(updateStmt);
            if (rc != SQLITE_DONE) {
                cerr << "Error updating order status: " << sqlite3_errmsg(db) << endl;
                sqlite3_finalize(updateStmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_finalize(updateStmt);
        }
    }

    if (!found) {
        cout << "No orders assigned to you." << endl;
    }

// Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
// View You're INCOMING ORDERS END!!!

// View You're ACTIVE ORDERS START!!!
void viewDriversActiveOrders(string UserID) {
    // Open the database connection
    sqlite3* db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve orders for the driver with 'In Transit' status
    sqlite3_stmt* stmt;
    const char* sql = "SELECT * FROM CargoOrders WHERE Status = 'In Transit' AND UserID = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the driverID parameter to the statement
    rc = sqlite3_bind_text(stmt, 1, UserID.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
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

    // Prompt the user if they want to view a specific order (if there are orders presented)
    if (orderCount > 0) {
        cout << "Do you want to view a specific order? (y/n): ";
        cin.ignore();
        string response;
        getline(cin, response);
        if (response == "y") {
            viewActiveOrderPT1();
        }
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// View You're ACTIVE ORDERS END!!!


void viewActiveOrderPT1() {
    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Get the user input for the UserID
    int userID;
    cout << "Enter your UserID: ";
    cin >> userID;

    // Prepare the SQL statement to retrieve the assigned orders for the driver
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders WHERE DriverAssigned = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the user input to the parameter in the SQL statement
    rc = sqlite3_bind_int(stmt, 1, userID);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the SQL statement and retrieve the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result set
        found = true;
        int id = sqlite3_column_int(stmt, 1);
        const unsigned char *description = sqlite3_column_text(stmt, 2);
        const unsigned char *origin = sqlite3_column_text(stmt, 10);
        const unsigned char *destination = sqlite3_column_text(stmt, 11);
        int weight = sqlite3_column_int(stmt, 4);
        int driverAssigned = sqlite3_column_int(stmt, 5);
        int length = sqlite3_column_int(stmt, 5);
        int width = sqlite3_column_int(stmt, 6);
        int height = sqlite3_column_int(stmt, 7);
        cout << "Cargo Order ID: " << id << endl;
        cout << "Description: " << description << endl;
        cout << "Origin: " << origin << endl;
        cout << "Destination: " << destination << endl;
        cout << "Dimensions: " << length << "cm x " << width << "cm x " << height << "cm" << endl;
        cout << "Weight: " << weight << "kg" << endl;
        cout << endl;

        // Prompt the driver to accept or reject the order
        char choice;
        do {
            cout << "Has this order been delivered? (Y/N): ";
            cin >> choice;
        } while (toupper(choice) != 'Y' && toupper(choice) != 'N');

        // Debugging statement
        cout << "User input: " << choice << endl;

        // Update the order status based on the driver's choice
        if (toupper(choice) == 'Y') {
            // Debugging statement
            cout << "Driver has delivered the order" << endl;

            // Update the order status to "Delivered"
            const char *updateSql = "UPDATE CargoOrders SET Status = ? WHERE DriverAssigned = ?";
            sqlite3_stmt *updateStmt;
            rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, NULL);
            if (rc != SQLITE_OK) {
                // error handling code
            }
            rc = sqlite3_bind_text(updateStmt, 1, "Delivered", -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_int(updateStmt, 2, id);
            if (rc != SQLITE_OK) {
                // error handling code
            }

            rc = sqlite3_step(updateStmt);
            if (rc != SQLITE_DONE) {
                // error handling code
            }

            rc = sqlite3_step(updateStmt);
            if (rc != SQLITE_DONE) {
                cerr << "Error updating order status: " << sqlite3_errmsg(db) << endl;
                sqlite3_finalize(updateStmt);
                sqlite3_close(db);
                return;
            }
            sqlite3_finalize(updateStmt);
        }

    }

    if (!found) {
        cout << "No orders assigned to you." << endl;
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// View You're COMPLETED ORDERS START!!!
void viewDriversCompletedOrders() {
// Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

// Get the user input for the UserID
    int userID;
    cout << "Enter your UserID: ";
    cin >> userID;

// Prepare the SQL statement to retrieve the assigned orders for the driver
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders WHERE DriverAssigned = ? AND Status = 'Delivered'";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

// Bind the user input to the parameter in the SQL statement
    rc = sqlite3_bind_int(stmt, 1, userID);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

// Execute the SQL statement and retrieve the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result set
        found = true;
        int id = sqlite3_column_int(stmt, 1);
        const unsigned char *description = sqlite3_column_text(stmt, 2);
        const unsigned char *origin = sqlite3_column_text(stmt, 10);
        const unsigned char *destination = sqlite3_column_text(stmt, 11);
        int weight = sqlite3_column_int(stmt, 4);
        int driverAssigned = sqlite3_column_int(stmt, 5);
        int length = sqlite3_column_int(stmt, 5);
        int width = sqlite3_column_int(stmt, 6);
        int height = sqlite3_column_int(stmt, 7);
        cout << "Cargo Order ID: " << id << endl;
        cout << "Description: " << description << endl;
        cout << "Origin: " << origin << endl;
        cout << "Destination: " << destination << endl;
        cout << "Dimensions: " << length << "cm x " << width << "cm x " << height << "cm" << endl;
        cout << "Weight: " << weight << "kg" << endl;
        cout << endl;
    }

    if (!found) {
        cout << "No delivered orders were found for the given UserID." << endl;
    }

// Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}



















void viewDriverNotifications(int driverId) {
    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve the orders assigned to the driver
    sqlite3_stmt *stmt;
    const char *sql = "SELECT CargoOrders.OrderID, CargoOrders.Source, CargoOrders.Destination, CargoOrders.Weight FROM CargoOrders JOIN DriverOrders ON CargoOrders.OrderID = DriverOrders.OrderID WHERE DriverOrders.UserID = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the driver UserID to the parameter in the SQL statement
    rc = sqlite3_bind_int(stmt, 1, driverId);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the SQL statement and retrieve the results
    bool found = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Process each row of the result set
        found = true;
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *source = sqlite3_column_text(stmt, 1);
        const unsigned char *destination = sqlite3_column_text(stmt, 2);
        double weight = sqlite3_column_double(stmt, 3);
        cout << "Order ID: " << id << ", Source: " << source << ", Destination: " << destination << ", Cargo Weight: " << weight << endl;
    }

    // Check if any orders were found
    if (!found) {
        cout << "No orders assigned to this driver." << endl;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}




