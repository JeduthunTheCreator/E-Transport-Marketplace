#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include "../include/CargoOwner.h"
#include <cmath>

using namespace std;

// Define a callback function to handle SQLite query results
static int callback(void* data, int argc, char** argv, char** azColName) {
    // Print out the query results
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << argv[i] << endl;
    }
    return 0;
}

// Testings


// Creating user account for Cargo Owner Start!!!
void registerUser() {
    string Name, Email, Username, Password, MobileNumber, Address;

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

    // Create new CargoOwner object
    CargoOwner cargoOwner = CargoOwner(Name, Email, Address, Username, Password, MobileNumber);

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
    const char *createTableSql = "CREATE TABLE IF NOT EXISTS CargoOwner ("
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
    const char *insertSql = "INSERT INTO CargoOwner (Name, Email, Username, Password, MobileNumber, Address) VALUES (?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind CargoOwner data to prepared statement
    rc = sqlite3_bind_text(stmt, 1, cargoOwner.getName().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 2, cargoOwner.getEmail().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 3, cargoOwner.getUsername().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 4, cargoOwner.getPassword().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 5, cargoOwner.getMobileNumber().c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 6, cargoOwner.getAddress().c_str(), -1, SQLITE_TRANSIENT);


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


// Viewing user account for Cargo Owner Start!!!
void viewProfile(string UserID) {

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
    const char *selectSql = "SELECT * FROM CargoOwner WHERE UserID=?";
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
        cout << "ID: " << sqlite3_column_text(stmt, 3) << endl;
        cout << "Password: " << sqlite3_column_text(stmt, 4) << endl;
        cout << "Mobile Number: " << sqlite3_column_text(stmt, 5) << endl;
        cout << "Address: " << sqlite3_column_text(stmt, 6) << endl;
    } else {
        cout << "User not found" << endl;
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Viewing user account for Cargo Owner END!!!


// Editing user account for Cargo Owner START!!!
void editProfile(string UserID, string Name, string Email, string Password, string MobileNumber, string Address) {
    // Open SQLite database
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to update user data
    const char *updateSql = "UPDATE CargoOwner SET Name=?, Email=?, Password=?, MobileNumber=?, Address=? WHERE UserID=?";
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
    rc = sqlite3_bind_text(stmt, 3, Password.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 4, MobileNumber.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 5, Address.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 6, UserID.c_str(), -1, SQLITE_TRANSIENT);

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
// Editing user account for Cargo Owner END!!!


// Placing an order from source to destination START!!!
void placeOrder(string UserID) {

    struct CargoOrder {
        string Name;
        int Quantity;
        int Weight;
        int Length;
        int Width;
        int Height;
        bool IsFrozen;
        bool IsFragile;
        string Source;
        string Destination;
        int Distance;
        string ShippingMethod;
        double ShippingRate;
        string LorryType;
    };

    // Get input from the user to create a new cargo order
    CargoOrder newOrder;
    cout << "Enter the cargo name: ";
    cin >> newOrder.Name;
    cout << "Enter the cargo quantity: ";
    cin >> newOrder.Quantity;
    cout << "Enter the cargo weight (in kg): ";
    cin >> newOrder.Weight;
    cout << "Enter the cargo length (in cm): ";
    cin >> newOrder.Length;
    cout << "Enter the cargo width (in cm): ";
    cin >> newOrder.Width;
    cout << "Enter the cargo height (in cm): ";
    cin >> newOrder.Height;
    cout << "Is the cargo frozen (y/n)?: ";
    char isFrozen;
    cin >> isFrozen;
    newOrder.IsFrozen = (isFrozen == 'y' || isFrozen == 'Y');
    cout << "Is the cargo fragile (y/n)?: ";
    char isFragile;
    cin >> isFragile;
    newOrder.IsFragile = (isFragile == 'y' || isFragile == 'Y');
    cout << "Enter the source address: ";
    cin >> newOrder.Source;
    cout << "Enter the destination address: ";
    cin >> newOrder.Destination;
    cout << "Enter the distance (in km): ";
    cin >> newOrder.Distance;
    cout << "Enter the shipping method (Ground or Air): ";
    cin >> newOrder.ShippingMethod;


    // Determine the lorry type based on the weight of the cargo
    if (newOrder.Weight < 10) {
        newOrder.LorryType = "Small";
    } else if (newOrder.Weight < 30) {
        newOrder.LorryType = "Medium";
    } else {
        newOrder.LorryType = "Large";
    }

    // Calculate shipping rate based on package weight, distance, and cargo characteristics
    newOrder.ShippingRate = 0;
    if (newOrder.ShippingMethod == "Ground") {
        newOrder.ShippingRate = newOrder.Weight * 0.5 + newOrder.Distance * 0.01;
        if (newOrder.IsFrozen) {
            newOrder.ShippingRate += 20.0;
        }
        if (newOrder.IsFragile) {
            newOrder.ShippingRate += 10.0;
        }
        if (newOrder.LorryType == "Small") {
            newOrder.ShippingRate += 5.0;
        } else if (newOrder.LorryType == "Medium") {
            newOrder.ShippingRate += 10.0;
        } else if (newOrder.LorryType == "Large") {
            newOrder.ShippingRate += 15.0;
        }
    } else if (newOrder.ShippingMethod == "Air") {
        newOrder.ShippingRate = newOrder.Weight * 1.5 + newOrder.Distance * 0.05;
        if (newOrder.IsFrozen) {
            newOrder.ShippingRate += 40.0;
        }
        if (newOrder.IsFragile) {
            newOrder.ShippingRate += 30.0;
        }
    }

    // Print shipping rate
    cout << "Shipping Rate: " << newOrder.ShippingRate << endl;
    cout << "Lorry Type: " << newOrder.LorryType << endl;


    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    const char *createTableSql = "CREATE TABLE IF NOT EXISTS CargoOrders ("
                                 "UserID INT NOT NULL,"
                                 "OrderID INTEGER NOT NULL UNIQUE,"
                                 "Name TEXT NOT NULL,"
                                 "Quantity INT NOT NULL,"
                                 "Weight INT NOT NULL,"
                                 "Length INT NOT NULL,"
                                 "Width INT NOT NULL,"
                                 "Height INT NOT NULL,"
                                 "IsFrozen INT NOT NULL,"
                                 "IsFragile INT NOT NULL,"
                                 "Source TEXT NOT NULL,"
                                 "Destination TEXT NOT NULL,"
                                 "Distance INT NOT NULL,"
                                 "ShippingMethod TEXT NOT NULL,"
                                 "ShippingRate REAL NOT NULL,"
                                 "LorryType TEXT NOT NULL)";

    char *errMsg;
    rc = sqlite3_exec(db, createTableSql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement to insert CargoOrders data
    const char *insertSql = "INSERT INTO CargoOrders (OrderID, Name, Quantity, Weight, Length, Width, Height, IsFrozen, IsFragile, Source, Destination, Distance, ShippingMethod, ShippingRate, LorryType, UserID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insertSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Generate a new unique OrderID
    sqlite3_stmt *maxIdStmt;
    const char *maxIdSql = "SELECT MAX(OrderID) FROM CargoOrders";
    rc = sqlite3_prepare_v2(db, maxIdSql, -1, &maxIdStmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }
    rc = sqlite3_step(maxIdStmt);
    int maxId = sqlite3_column_int(maxIdStmt, 0);
    sqlite3_finalize(maxIdStmt);
    int newOrderId = maxId + 1;

    // Bind the parameters
    sqlite3_bind_int(stmt, 1, newOrderId);
    sqlite3_bind_text(stmt, 2, newOrder.Name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, newOrder.Quantity);
    sqlite3_bind_double(stmt, 4, newOrder.Weight);
    sqlite3_bind_double(stmt, 5, newOrder.Length);
    sqlite3_bind_double(stmt, 6, newOrder.Width);
    sqlite3_bind_double(stmt, 7, newOrder.Height);
    sqlite3_bind_int(stmt, 8, newOrder.IsFrozen ? 1 : 0);
    sqlite3_bind_int(stmt, 9, newOrder.IsFragile ? 1 : 0);
    sqlite3_bind_text(stmt, 10, newOrder.Source.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11, newOrder.Destination.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 12, newOrder.Distance);
    sqlite3_bind_text(stmt, 13, newOrder.ShippingMethod.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 14, newOrder.ShippingRate);
    sqlite3_bind_text(stmt, 15, newOrder.LorryType.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 16, UserID.c_str(), -1, SQLITE_TRANSIENT);


// Execute the SQL statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // Handle the error
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

// Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);

}
// Placing an order from source to destination END!!!


// Order status of orders placed by individual user START!!!
void viewOrders(string UserID) {

    // Open the database connection
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement to retrieve the orders for the given user ID
    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM CargoOrders WHERE UserID = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the user ID parameter to the SQL statement
    rc = sqlite3_bind_text(stmt, 1, UserID.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the SQL statement and iterate over the results
    rc = sqlite3_step(stmt);
    while (rc == SQLITE_ROW) {
        int OrderID = sqlite3_column_int(stmt, 1);
        string Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int Quantity = sqlite3_column_int(stmt, 3);
        double Weight = sqlite3_column_double(stmt, 4);
        string Source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        string Destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        int Distance = sqlite3_column_int(stmt, 7);
        string ShippingMethod = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
        int Length = sqlite3_column_int(stmt, 10);
        int Height = sqlite3_column_int(stmt, 11);
        int Width = sqlite3_column_int(stmt, 12);
        int IsFrozen = sqlite3_column_int(stmt, 13);
        int IsFragile = sqlite3_column_int(stmt, 14);
        string LorryType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
        string Status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));

        // Loop through the results and display the order details
        while (rc == SQLITE_ROW) {
            int OrderID = sqlite3_column_int(stmt, 1);
            string Name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int Quantity = sqlite3_column_int(stmt, 3);
            double Weight = sqlite3_column_double(stmt, 4);
            string Source = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            string Destination = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
            int Distance = sqlite3_column_int(stmt, 7);
            string ShippingMethod = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
            int Length = sqlite3_column_int(stmt, 10);
            int Height = sqlite3_column_int(stmt, 11);
            int Width = sqlite3_column_int(stmt, 12);
            int IsFrozen = sqlite3_column_int(stmt, 13);
            int IsFragile = sqlite3_column_int(stmt, 14);
            string LorryType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 15));
            string Status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));

            // Display the order details
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
            cout << "Shipping Rate: " << sqlite3_column_double(stmt, 9) << endl;
            cout << "LorryType: " << LorryType << endl;
            cout << "Status: " << Status << endl;
            cout << "---------------------------------------" << endl;

            rc = sqlite3_step(stmt);
        }

        rc = sqlite3_step(stmt);
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
// Order status of orders placed by individual user END!!!

// Orders fulfilled once driver has accepted the cargo START!!!
void viewfulfilledOrders(string UserID) {
    // Open the database
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement
    const char *sql = "SELECT OrderID, Name, Source, Destination, Status FROM CargoOrders WHERE DriverAssigned = ? AND Status = 'Delivered';";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the parameter
    rc = sqlite3_bind_text(stmt, 1, UserID.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement and print the results
    bool orders_found = false;
    cout << "Fulfilled Orders:" << endl;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        orders_found = true;
        int orderId = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        const unsigned char *source = sqlite3_column_text(stmt, 2);
        const unsigned char *destination = sqlite3_column_text(stmt, 3);
        const unsigned char *status = sqlite3_column_text(stmt, 4);
        cout << "Order ID: " << orderId << endl;
        cout << "Name: " << name << endl;
        cout << "Origin: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Status: " << status << endl;
        cout << endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Display message if no orders are found
    if (!orders_found) {
        cout << "No orders currently fulfilled" << endl;
    }
}
// Orders fulfilled once driver has accepted the cargo END!!!

void viewCustomerCargoInvoices(string UserID) {
    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Prepare the SQL statement
    const char* sql = "SELECT OrderID, Source, Destination, Weight, Status, FinalCost FROM CargoOrders WHERE UserID = ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind the parameter
    rc = sqlite3_bind_text(stmt, 1, UserID.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Execute the statement and print the results
    int result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
        cout << "No invoices found for customer" << endl;
    } else {
        cout << "Invoices for Customer ID " << UserID << endl;
        cout << "-------------------------------------" << endl;
        while (result == SQLITE_ROW) {
            int orderId = sqlite3_column_int(stmt, 0);
            const unsigned char* source = sqlite3_column_text(stmt, 1);
            const unsigned char* destination = sqlite3_column_text(stmt, 2);
            int weight = sqlite3_column_int(stmt, 3);
            const unsigned char* status = sqlite3_column_text(stmt, 4);
            double finalCost = sqlite3_column_double(stmt, 5);

            cout << "Order ID: " << orderId << endl;
            cout << "Source: " << source << endl;
            cout << "Destination: " << destination << endl;
            cout << "Weight: " << weight << " kg" << endl;
            cout << "Status: " << status << endl;
            cout << "FinalCost: £" << finalCost << endl;
            cout << "-------------------------------------" << endl;

            result = sqlite3_step(stmt);
        }
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void viewNotifications() {

}
