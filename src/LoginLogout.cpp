#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

string loginUser()
{
    string Username, Password;

    cout << "Enter your username: ";
    cin >> Username;
    cout << "Enter your password: ";
    cin >> Password;

    // Open SQLite database
    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Prepare SQL statement to retrieve CargoOwner data for the entered username and password
    const char *selectSql = "SELECT * FROM CargoOwner WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Bind username and password to prepared statement
    rc = sqlite3_bind_text(stmt, 1, Username.c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 2, Password.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    // Execute the statement and check if any data is returned
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Account exists and user is logged in
        string UserID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return UserID;
    } else {
        // Account does not exist or credentials are incorrect
        cerr << "Invalid username or password." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    void profile();
}

string loginDriver()
{
    string Username, Password;

    cout << "Enter your username: ";
    cin >> Username;
    cout << "Enter your password: ";
    cin >> Password;

    // Open SQLite database
    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Prepare SQL statement to retrieve Driver data for the entered username and password
    const char *selectSql = "SELECT * FROM Driver WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Bind username and password to prepared statement
    rc = sqlite3_bind_text(stmt, 1, Username.c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 2, Password.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    // Execute the statement and check if any data is returned
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Account exists and user is logged in
        string UserID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return UserID;
    } else {
        // Account does not exist or credentials are incorrect
        cerr << "Invalid username or password." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    void profile();
}

string loginTransport()
{
    string Username, Password;

    cout << "Enter your username: ";
    cin >> Username;
    cout << "Enter your password: ";
    cin >> Password;

    // Open SQLite database
    sqlite3_stmt *stmt;
    sqlite3 *db;
    int rc = sqlite3_open("db.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Error opening SQLite database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Prepare SQL statement to retrieve Driver data for the entered username and password
    const char *selectSql = "SELECT * FROM TransportCompany WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, selectSql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return "";
    }

    // Bind username and password to prepared statement
    rc = sqlite3_bind_text(stmt, 1, Username.c_str(), -1, SQLITE_TRANSIENT);
    rc |= sqlite3_bind_text(stmt, 2, Password.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        cerr << "Error binding values: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    // Execute the statement and check if any data is returned
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Account exists and user is logged in
        string UserID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return UserID;
    } else {
        // Account does not exist or credentials are incorrect
        cerr << "Invalid username or password." << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return "";
    }

    void profile();
}

//int main() {
//    if (login()) {
//        cout << "Logged in successfully!" << endl;
//    } else {
//        cout << "Login failed." << endl;
//    }
//    return 0;
//}
