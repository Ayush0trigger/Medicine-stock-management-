#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <regex>  
#include <conio.h> 
using namespace std;


struct Medicine {
    int id;
    string name;
    int quantity;
    double price;
    string expiryDate; 
};


void adminLogin();
void mainMenu();
void addMedicine();
void viewMedicines();
void updateMedicine();
void deleteMedicine();
void generateBill();
void searchMedicine();
void saveToFile(const vector<Medicine>& medicines);
void loadFromFile(vector<Medicine>& medicines);
bool validateDate(const string& date);

vector<Medicine> medicineList; 

// Main 
int main() {
    loadFromFile(medicineList);
    adminLogin();
    return 0;
}

// Admin Login
void adminLogin() {
    string username, password, storedUsername = "admin", storedPassword = "1234";
    cout << "===WELCOME TO MEDICINE STOCK MANAGEMENT===\n";
    cout << "=== Admin Login ===\n";
    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    char ch;
    while ((ch = _getch()) != '\r') { 
        if (ch == '\b' && !password.empty()) { 
            password.pop_back(); 
            cout << "\b \b"; 
        } else if (ch != '\b') {
            password += ch;
            cout << "*"; 
        }
    }
    cout << endl;

    if (username == storedUsername && password == storedPassword) {
        cout << "Login successful!\n";
        mainMenu();
    } else {
        cout << "Invalid credentials. Try again.\n";
        adminLogin();
    }
}
void mainMenu() {
    int choice;
    string input; 

    do {
        cout << "\n=== Medicine Stock Management ===\n";
        cout << "1. Add Medicine\n";
        cout << "2. View Medicines\n";
        cout << "3. Update Medicine\n";
        cout << "4. Delete Medicine\n";
        cout << "5. Generate Bill\n";
        cout << "6. Search Medicine\n";  
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> input;

        
        if (regex_match(input, regex("^[0-9]+$"))) {
            choice = stoi(input); 
        } else {
            cout << "Invalid input. Please enter a number.\n";
            continue; 
        }

        switch (choice) {
            case 1: addMedicine(); break;
            case 2: viewMedicines(); break;
            case 3: updateMedicine(); break;
            case 4: deleteMedicine(); break;
            case 5: generateBill(); break;
            case 6: searchMedicine(); break; 
            case 7: saveToFile(medicineList); cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please enter a number between 1 and 7.\n";
        }
    } while (choice != 7);
}


// Validate Date
bool validateDate(const string& date) {
    regex datePattern(R"(^\d{2}/\d{2}/\d{4}$)");
    return regex_match(date, datePattern);
}

bool isNumeric(const string& str) {
    return regex_match(str, regex("^[0-9]+$"));
}


bool isFloat(const string& str) {
    return regex_match(str, regex("^[0-9]*\\.?[0-9]+$"));
}

void addMedicine() {
    Medicine med;
    bool isUnique;
    string input;

   
    do {
        cout << "Enter Medicine ID (numbers only): ";
        cin >> input;

        // Check if the input is numeric
        if (!isNumeric(input)) {
            cout << "Invalid input. ID should be a number.\n";
            continue;
        }

        med.id = stoi(input);
        isUnique = true;

       
        for (const auto& existingMed : medicineList) {
            if (existingMed.id == med.id) {
                isUnique = false;
                cout << "Medicine ID already exists. Please enter a unique ID.\n";
                break;
            }
        }
    } while (!isUnique);

    
    cout << "Enter Medicine Name: ";
    cin.ignore();
    getline(cin, med.name);

    cout << "Enter Medicine Price: ";
    cin >> med.price;

    cout << "Enter Medicine Quantity: ";
    cin >> med.quantity;

    // Add expiry date with validation
    do {
        cout << "Enter Expiry Date (DD/MM/YYYY): ";
        cin >> med.expiryDate;

    
        if (!validateDate(med.expiryDate)) {
            cout << "Invalid date format. Please enter the date in DD/MM/YYYY format.\n";
        }
    } while (!validateDate(med.expiryDate));


    medicineList.push_back(med);
    cout << "Medicine added successfully!\n";
}



// View Medicines
void viewMedicines() {
    cout << "\n=== Medicine List ===\n";
    cout << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Quantity" 
         << setw(10) << "Price" << setw(15) << "Expiry Date\n";
    for (const auto& med : medicineList) {
        cout << setw(10) << med.id << setw(20) << med.name << setw(10) 
             << med.quantity << setw(10) << med.price << setw(15) << med.expiryDate << "\n";
    }
}

// Update Medicine

void updateMedicine() {
    string input;
    int id, found = -1;

    
    do {
        cout << "Enter Medicine ID to update (numbers only): ";
        cin >> input;
        if (!isNumeric(input)) {
            cout << "Invalid input. ID should be a number.\n";
        }
    } while (!isNumeric(input));
    id = stoi(input); 

    // Search for the medicine in the list
    for (size_t i = 0; i < medicineList.size(); i++) {
        if (medicineList[i].id == id) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        cout << "Enter new quantity: ";
        cin >> medicineList[found].quantity;
        cout << "Enter new price: ";
        cin >> medicineList[found].price;

        // Validate expiry date
        do {
            cout << "Enter new expiry date (DD/MM/YYYY): ";
            cin >> medicineList[found].expiryDate;
            if (!validateDate(medicineList[found].expiryDate)) {
                cout << "Invalid date format. Please try again.\n";
            }
        } while (!validateDate(medicineList[found].expiryDate));

        cout << "Medicine updated successfully!\n";
    } else {
        cout << "Medicine not found.\n";
    }
}

// Delete Medicine

void deleteMedicine() {
    string input;
    int id;
    bool isValid = false;

    // Input validation for numeric ID
    do {
        cout << "Enter Medicine ID to delete (numbers only): ";
        cin >> input;

        if (isNumeric(input)) {
            id = stoi(input); 
            isValid = true;
        } else {
            cout << "Invalid input. Please enter a valid numeric ID.\n";
        }
    } while (!isValid);

   
    int found = -1;
    for (size_t i = 0; i < medicineList.size(); i++) {
        if (medicineList[i].id == id) {
            found = i;
            break;
        }
    }

   
    if (found != -1) {
        medicineList.erase(medicineList.begin() + found);
        cout << "Medicine deleted successfully!\n";
    } else {
        cout << "Medicine not found.\n";
    }
}

// Generate Bill

void generateBill() {
    string inputId, inputQty;
    int id, qty;
    double total = 0;
    char choice;

    cout << "\n=== Generate Bill ===\n";
    do {
        
        while (true) {
            cout << "Enter Medicine ID (numbers only): ";
            cin >> inputId;

            if (!isNumeric(inputId)) {
                cout << "Invalid input. Medicine ID should be a number.\n";
                continue; 
            }

            id = stoi(inputId); 
            break;
        }

      
        while (true) {
            cout << "Enter Quantity (numbers only): ";
            cin >> inputQty;

            if (!isNumeric(inputQty)) {
                cout << "Invalid input. Quantity should be a number.\n";
                continue; 
            }

            qty = stoi(inputQty);
            break;
        }

        bool found = false;
        for (auto& med : medicineList) {
            if (med.id == id) {
                found = true;
                if (med.quantity >= qty) {
                    med.quantity -= qty;
                    total += med.price * qty;
                } else {
                    cout << "Insufficient stock.\n";
                }
                break;
            }
        }

        if (!found) {
            cout << "Medicine ID not found.\n";
        }

        cout << "Add more items? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    cout << "Total Bill: " << total << "\n";
}

void searchMedicine() {
    string searchTerm;
    cout << "Enter Medicine ID or Name to search: ";
    cin.ignore(); 
    getline(cin, searchTerm);
    
    bool found = false;
    for (const auto& med : medicineList) {
        if (to_string(med.id) == searchTerm || med.name == searchTerm) {
            cout << "Found Medicine: ID: " << med.id << "\n Name: " << med.name << "\n Quantity: " 
                 << med.quantity << "\n Price: " << med.price << "\n Expiry Date: " << med.expiryDate << "\n";
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "Medicine not found.\n";
    }
}


// Save data to file
void saveToFile(const vector<Medicine>& medicines) {
    ofstream file("medicine_data.txt");
    for (const auto& med : medicines) {
        file << med.id << " " << med.name << " " << med.quantity << " " 
             << med.price << " " << med.expiryDate << "\n";
    }
    file.close();
}

// Load data from file
void loadFromFile(vector<Medicine>& medicines) {
    ifstream file("medicine_data.txt");
    Medicine med;
    while (file >> med.id >> med.name >> med.quantity >> med.price >> med.expiryDate) {
        medicines.push_back(med);
    }
    file.close();
}

