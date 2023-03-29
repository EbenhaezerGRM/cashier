#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
using std::cout;
using std::cin;
using std::string;
using std::vector;

std::map<int, std::pair < string, double >> menu = {
    {1, {"Bottle of water  ", 1.99}},
    {2, {"Chicken burger   ", 3.99}},
    {3, {"Carbonara pasta  ", 4.0}},
    {4, {"Chicken soup     ", 2.99}},
    {5, {"Fried chicken    ", 3.5}},
    {6, {"Fried rice       ", 2.5}},
    {7, {"Orange juice     ", 2.9}},
    {8, {"Pudding          ", 3.0}},
    {9, {"Sushi            ", 4.0}}
};

int main() {
    cout << "======== Welcome to our restaurant ========\n";
    cout << "===========================================\n\n";
    string reservationName;
    cout << "Reservation name: ";
    getline(cin, reservationName);

    const int MIN_ACCOUNT_BALANCE = 10;
    const int MAX_ACCOUNT_BALANCE = 1000;
    double accountBalance = 0;
    while (accountBalance < MIN_ACCOUNT_BALANCE || accountBalance  > MAX_ACCOUNT_BALANCE || cin.fail()) {
        cout << "Min. $10 || Max. $1000\n";
        cout << "Top up balance: ";
        cin >> accountBalance;
        cin.clear();
        cin.ignore(256, '\n');
    }
    cout << "Balance: $" << accountBalance;

    cout << "\n=========== Menu ===========\n";
    for (const auto& item : menu) {
        cout << item.first << ". " << item.second.first << "    price: $" << item.second.second << "\n";
    }
    cout << "==============================\n";

    const int MAX_ITEMS = 100;
    const int MAX_ORDER = 25;
    const int MIN_ORDER = 1;
    int chooseMenu;
    int quantity[MAX_ITEMS] = {};
    string item;
    double purchaseTotal = 0;
    int i = 0;

    vector<string> menuOrdered;
    vector<double> menuPrice;
    vector<int> quantityOrdered;

    while (true) {
        cout << "Choose menu (0 to exit): ";
        cin >> chooseMenu;

        if (cin.fail() || chooseMenu < 0 || chooseMenu > 9) {
            cout << "Invalid menu item number!\n";
            cin.clear();
            cin.ignore(256, '\n');
            continue;
        }

        if (chooseMenu == 0) {
            break;
        }

        while (true) {
            cout << "Quantity of " << menu[chooseMenu].first << " (1-25): ";
            cin >> quantity[chooseMenu];

            if (cin.fail() || quantity[chooseMenu] < MIN_ORDER || quantity[chooseMenu] > MAX_ORDER) {
                cout << "Invalid quantity!\n";
                cin.clear();
                cin.ignore(256, '\n');
                continue;
            }

            break;
        }

        menuOrdered.push_back(menu[chooseMenu].first);
        menuPrice.push_back(menu[chooseMenu].second);
        quantityOrdered.push_back(quantity[chooseMenu]);
    }

    cout << "No	Item menu    	     Price($)		Quantity        Sub Total($)" << "\n";
    for (int i = 0; i < menuOrdered.size(); i++) {
        cout << i + 1 << "	" << menuOrdered[i] << "	" << menuPrice[i] << "		" << quantityOrdered[i] << "		" << menuPrice[i] * quantityOrdered[i] << "\n";
    }

    while (!menuOrdered.empty()) {
        int item_number;
        cout << "\nEnter the number of an item to remove, or 0 to finalize your order: ";
        cin >> item_number;

        if (item_number == 0) {
            break;
        }
        else if (item_number < 1 || item_number > menuOrdered.size()) {
            cout << "Invalid item number. Please try again." << "\n";
        }
        else {
            string removed_item = menuOrdered[item_number - 1];
            menuOrdered.erase(menuOrdered.begin() + (item_number - 1));
            cout << "Removed " << removed_item << "\n";

            cout << "\nNo	Item menu    	     Price($)		Quantity        Sub Total($)" << "\n";
            for (int i = 0; i < menuOrdered.size(); i++) {
                cout << i + 1 << "	" << menuOrdered[i] << "	" << menuPrice[i] << "		" << quantityOrdered[i] << "		" << menuPrice[i] * quantityOrdered[i] << "\n";
            }
        }
    }

    if (menuOrdered.size() == 0) {
        cout << "\nNo Order\n";
        cout << "Balance: $" << accountBalance;
    }
    else {
        cout << "\nReservation name: " << reservationName;
        cout << "\n================================== INVOICE ==================================\n";
        cout << "=============================================================================\n";
        cout << "No	Item menu    	     Price($)		Quantity        Sub Total($)" << "\n";
        for (int i = 0; i < menuOrdered.size(); i++) {
            cout << i + 1 << "	" << menuOrdered[i] << "	" << menuPrice[i] << "		" << quantityOrdered[i] << "		" << menuPrice[i] * quantityOrdered[i] << "\n";
            double subtotal = menuPrice[i] * quantityOrdered[i];
            purchaseTotal += subtotal;
        }
        cout << "=============================================================================\n";

        cout << "Purchase total (without tax) : $" << purchaseTotal << "\n";
        double totalWithTax = purchaseTotal * 0.1 + purchaseTotal;
        cout << "Purchase total (with tax 10%): $" << totalWithTax << "\n";

        double topUpBalance;
        double maxTopUp = MAX_ACCOUNT_BALANCE - accountBalance;
        bool checkBalance = totalWithTax > accountBalance;
        while (checkBalance) {
            cout << "\nBalance: $" << accountBalance;
            cout << "\nYour balance is not enough, please top up your balance";
            cout << "\nTop up balance: $";

            while (!(cin >> topUpBalance) || topUpBalance <= 0 || topUpBalance > maxTopUp) {
                cout << "\nInvalid input. (Min. $1 || Max. $" << maxTopUp << "): ";
                cin.clear();
                cin.ignore(256, '\n');
            }

            accountBalance += topUpBalance;
            maxTopUp = MAX_ACCOUNT_BALANCE - accountBalance;

            if (accountBalance >= totalWithTax) {
                checkBalance = false;
            }
        }
        cout << "\nBalance: $" << accountBalance - totalWithTax << "\n";
        cout << "Thank you for your order\n";

        string filename = "Ordered by " + reservationName;
        std::ofstream orderFile(filename + ".txt");

        orderFile << "\nReservation name: " << reservationName;
        orderFile << "\n================================== INVOICE ==================================\n";
        orderFile << "=============================================================================\n";

        orderFile << "No	Item menu    	     Price($)		Quantity        Sub Total($)" << "\n";
        for (int i = 0; i < menuOrdered.size(); i++) {
            orderFile << i + 1 << "	" << menuOrdered[i] << "	" << menuPrice[i] << "		" << quantityOrdered[i] << "		" << menuPrice[i] * quantityOrdered[i] << "\n";
        }
        orderFile << "=============================================================================\n";

        orderFile << "Purchase total (without tax) : $" << purchaseTotal << "\n";
        orderFile << "Purchase total (with tax 10%): $" << totalWithTax << "\n";
        orderFile << "Balance before: $" << accountBalance;
        orderFile << "\nBalance left: $" << accountBalance - totalWithTax;
        orderFile.close();
    }
};