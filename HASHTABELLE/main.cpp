#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "StockManager.cpp"

using namespace std;

int main() {
    StockManager stockManager;
    char choice;

    do {
        cout << "1 - ADD" << endl;
        cout << "2 - DEL" << endl;
        cout << "3 - IMPORT a csv file" << endl;
        cout << "4 - SEARCH" << endl;
        cout << "5 - PLOT" << endl;
        cout << "6 - SAVE <filename>" << endl;
        cout << "7 - LOAD <filename>" << endl;
        cout << "8 - QUIT" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        //Eingabe Handling
        switch (choice) {
            case '1':{
                string name, wkn, symbol;
                cout << "Enter stock name: ";
                cin >> name;
                cout << "Enter WKN: ";
                cin >> wkn;
                cout << "Enter symbol: ";
                cin >> symbol;
                stockManager.addStock(name, wkn, symbol);
                break;
            }
            case '2': stockManager.deleteStock(); break;
            case '3': stockManager.importStockData(); break;
            case '4': stockManager.searchStock();break;
            case '5': stockManager.plotStock(); break;
            case '6': stockManager.saveToFile(); break;
            case '7': stockManager.loadFromFile(); break;
            //Hier nur Ausgabe
            case '8': cout << "Quitting program." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }
        //wird hier beendet falls 8(eingeben wird)
    } while (choice != '8');

    return 0;
}
