#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iomanip>

using namespace std;

struct StockData {
    string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
    double adjClose;
};

struct Stock {
    string name;
    string wkn;
    string symbol;
    vector<StockData> history;
};

class StockManager {
private:
    unordered_map<string, Stock> stocks;

public:
     void addStock(const string& name, const string& wkn, const string& symbol) {
        Stock stock;
        stock.name = name;
        stock.wkn = wkn;
        stock.symbol = symbol;
        stocks[symbol] = stock;
    }

    void deleteStock() {
        string symbol;
        cout << "Enter symbol of stock to delete: ";
        cin >> symbol;
        stocks.erase(symbol);
    }

    void importStockData() {
        string symbol, filename;
        cout << "Enter symbol of stock to import data for: ";
        cin >> symbol;
        cout << "Enter filename: ";
        cin >> filename;
        ifstream file(filename);
        string line;

        // Skip header
        getline(file, line);

        while (getline(file, line)) {
            stringstream ss(line);
            string o, h, l,c,v,a;
            StockData data;
            getline(ss, data.date, ',');
            getline(ss, o, ',');
            data.open = stold(o);
            getline(ss, h, ',');
            data.high = stold(h);
            getline(ss, l, ',');
            data.low = stold(l);
            getline(ss, c, ',');
            data.close = stold(c);
            getline(ss, a, ',');
            data.adjClose = stold(a);
            getline(ss, v, ',');
            data.volume = stold(v);
            stocks[symbol].history.push_back(data);
        }

        file.close();
    }

    void searchStock() {
        string query;
        cout << "Enter stock name or symbol to search: ";
        cin >> query;
        auto it = stocks.find(query);
        if (it != stocks.end()) {
            Stock& stock = it->second;
            if (!stock.history.empty()) {
                StockData& latestData = stock.history.back();
                cout << "Latest data for " << stock.name << " (" << stock.symbol << "):" << endl;
                cout << "Date: " << latestData.date << endl;
                cout << "Open: " << latestData.open << endl;
                cout << "High: " << latestData.high << endl;
                cout << "Low: " << latestData.low << endl;
                cout << "Close: " << latestData.close << endl;
                cout << "Volume: " << latestData.volume << endl;
                cout << "Adj Close: " << latestData.adjClose << endl;
            } else {
                cout << "No data available for " << stock.name << " (" << stock.symbol << ")" << endl;
            }
        } else {
            cout << "Stock symbol or name not found." << endl;
        }
    }

    void plotStock() {
        //Still has to plot
        string symbol;
        cout << "Enter symbol of stock to plot: ";
        cin >> symbol;
        auto it = stocks.find(symbol);
        if (it != stocks.end()) {
            Stock& stock = it->second;
            cout << "Plot for " << stock.name << " (" << stock.symbol << ")" << endl;
            cout << "Date       Close" << endl;
            StockData& latestData = stock.history.back();
            int divider;
            for(divider = 10; latestData.close/divider > 70; divider=divider*10){

            }
            int c = 0;
            int count_30 = stock.history.size()-30;
            for (const StockData& data : stock.history) {
                c ++;
                if(c >= count_30){
                    cout << data.date << " " << setw(10) << data.close << " |";
                    for (int i = 0; i < data.close / divider; ++i) cout << "*";
                    cout << endl;
                };
            }
        } else {
            cout << "Stock symbol not found." << endl;
        }
    }

    void saveToFile() {
        string filename;
        cout << "Enter filename to save: ";
        cin >> filename;
        ofstream file(filename);
        for (const auto& pair : stocks) {
            const Stock& stock = pair.second;
            file << stock.name << "," << stock.wkn << "," << stock.symbol << endl;
            for (const StockData& data : stock.history) {
                file << data.date << "," << data.open << "," << data.high << ","
                     << data.low << "," << data.close << ","<< data.adjClose << ","
                     << data.volume << endl;
            }
            file << endl;
        }
        file.close();
    }

    void loadFromFile() {
        string filename;
        cout << "Enter filename to load: ";
        cin >> filename;
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string name, wkn, symbol;
            getline(ss, name, ',');
            getline(ss, wkn, ',');
            getline(ss, symbol, ',');
            addStock(name, wkn, symbol);
            while (getline(file, line) && !line.empty()) {
                stringstream ss(line);
                string o, h, l,c,v,a;
                StockData data;
                getline(ss, data.date, ',');
                getline(ss, o, ',');
                data.open = stold(o);
                getline(ss, h, ',');
                data.high = stold(h);
                getline(ss, l, ',');
                data.low = stold(l);
                getline(ss, c, ',');
                data.close = stold(c);
                getline(ss, a, ',');
                data.adjClose = stold(a);
                getline(ss, v, ',');
                data.volume = stold(v);
                stocks[symbol].history.push_back(data);
            }
        }
        file.close();
    }
};

int main() {
    StockManager stockManager;
    char choice;

    do {
        cout << "1 - ADD" << endl;
        cout << "2 - DEL" << endl;
        cout << "3 - IMPORT" << endl;
        cout << "4 - SEARCH" << endl;
        cout << "5 - PLOT" << endl;
        cout << "6 - SAVE <filename>" << endl;
        cout << "7 - LOAD <filename>" << endl;
        cout << "8 - QUIT" << endl;
        cout << "Enter your choice: ";
        cin >> choice;


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
            case '8': cout << "Quitting program." << endl; break;
            default: cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != '8');

    return 0;
}
