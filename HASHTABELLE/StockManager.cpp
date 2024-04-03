#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include "StockManager.h"

    int StockManager::customHash(const string& word){
        int total = 0;
        int round = 1;
        for (char c : word) {
            // Das ASCII value von den stellen addieren
            // jede stelle mit ihrer position mulitplizieren, um Reihenfolge wichtig zu machen
            total += static_cast<int>(c)*round;
            round ++;
        }
        // Modulo der Maximalen Gr��e, damit alle Werte im Bereich zwischen 0 und 999 liegen
        int hashValue = (total % MAX_STOCKS);
        return hashValue;
    };

    void StockManager::addStock(const string& name, const string& wkn, const string& symbol) {
        //�berpf�fen ob noch platz in der Tabelle ist
        if (numStocks < MAX_STOCKS) {
            int index = customHash(symbol);
            // Kolisionen abfangen
            while (!stocks[index].symbol.empty()) {
                //Wert wird so lange verschoben bis ein freier platz gefunden wird
                index = (index + 1) % MAX_STOCKS;
            }
            stocks[index].name = name;
            stocks[index].wkn = wkn;
            stocks[index].symbol = symbol;
            ++numStocks;
        } else {
            cout << "Maximum number of stocks reached." << endl;
        }
    };

    void StockManager::deleteStock() {
        string symbol;
        cout << "Enter symbol of stock to delete: ";
        cin >> symbol;

        int index = customHash(symbol);
        //Nach Stock suchen, falls der Hash aufgrund von Kollsion verschiben wurde
        //Falls der Stock gefunden wird oder ein leerer Platz gefunden wird geht es aus der schleife
        while (stocks[index].symbol != symbol && !stocks[index].symbol.empty()) {
            index = (index + 1) % MAX_STOCKS;
        }
        //Wenn es gefunden wurde wird es gel�scht
        if (stocks[index].symbol == symbol) {
            // Das Symbol wird auf '\0' gestetzt und so als gel�scht makiert
            stocks[index].symbol = "\0";
            stocks[index].name = "\0";
            stocks[index].wkn = "\0";
            //Die Anzahl an Stocks wird verringert
            --numStocks;
            cout << "Stock deleted successfully." << endl;
        } else {
            cout << "Stock not found." << endl;
        }
    };

    void StockManager::importStockData() {
        string symbol, filename;
        cout << "Enter symbol of stock to import data for: ";
        cin >> symbol;

        int index = customHash(symbol);
        while (stocks[index].symbol != symbol && !stocks[index].symbol.empty()) { // Find the stock
            index = (index + 1) % MAX_STOCKS;
        }
        if (stocks[index].symbol == symbol) {
            cout << "Enter filename: ";
            cin >> filename;
            ifstream file(filename);
            if (!file.is_open())
            {
                cout << "Error opening file." << endl;
                return;
            }
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
                stocks[index].history.push_back(data);
            }

            file.close();

        } else {
            cout << "Stock not found." << endl;
        }
    };

    void StockManager::searchStock() {
        string query;
        cout << "Enter stock name or symbol to search: ";
        cin >> query;
        int index = customHash(query);
        while (!stocks[index].symbol.empty() && stocks[index].symbol != query && stocks[index].name != query) {
            index = (index + 1) % MAX_STOCKS;
        }
        if (stocks[index].symbol == query || stocks[index].name == query) {
            if (!stocks[index].history.empty()) {
                StockData& latestData = stocks[index].history.back();
                cout << "Latest data for " << stocks[index].name << " (" << stocks[index].symbol << "):" << endl;
                cout << "Date: " << latestData.date << endl;
                cout << "Open: " << latestData.open << endl;
                cout << "High: " << latestData.high << endl;
                cout << "Low: " << latestData.low << endl;
                cout << "Close: " << latestData.close << endl;
                cout << "Volume: " << latestData.volume << endl;
                cout << "Adj Close: " << latestData.adjClose << endl;
            } else {
                cout << "No data available for " << stocks[index].name << " (" << stocks[index].symbol << ")" << endl;
            }
        } else {
            cout << "Stock not found." << endl;
        }
    };

    void StockManager::plotStock() {
        string symbol;
        cout << "Enter symbol of stock to plot: ";
        cin >> symbol;
        int index = customHash(symbol);
        while (stocks[index].symbol != symbol && !stocks[index].symbol.empty()) { // Find the stock
            index = (index + 1) % MAX_STOCKS;
        }
        if (stocks[index].symbol == symbol) {
            cout << "Plot for " << stocks[index].name << " (" << stocks[index].symbol << ")" << endl;
            StockData& latestData = stocks[index].history.back();
            int divider;
            for(divider = 10; latestData.close/divider > 70; divider=divider*10){}
            int c = 0;
            int count_30 = stocks[index].history.size()-30;
            for (const StockData& data : stocks[index].history) {
                c ++;
                if(c >= count_30){
                    cout << data.date << " " << setw(10) << data.close << " |";
                    for (int i = 0; i < data.close / divider; ++i) cout << "*";
                    cout << endl;
                };
            }

        } else {
            cout << "Stock not found." << endl;
        }
    };

    void StockManager::saveToFile() {
        string filename;
        cout << "Enter filename to save: ";
        cin >> filename;
        ofstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }
        for (int i = 0; i < MAX_STOCKS; ++i){
            if (!stocks[i].symbol.empty() && stocks[i].symbol != "\0" ) {
                file << stocks[i].name << "," << stocks[i].wkn << "," << stocks[i].symbol << endl;
                for (const StockData& data : stocks[i].history) {
                    file << data.date << "," << data.open << "," << data.high << ","
                        << data.low << "," << data.close << "," << data.volume << ","
                        << data.adjClose << endl;
                }
                file << endl;
            }
        }
        file.close();
    };

    void StockManager::loadFromFile() {
        string filename;
        cout << "Enter filename to load: ";
        cin >> filename;
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            if (numStocks < MAX_STOCKS) {
                stringstream ss(line);
                string name, wkn, symbol;
                getline(ss, name, ',');
                getline(ss, wkn, ',');
                getline(ss, symbol, ',');
                int index = customHash(symbol);
                while (!stocks[index].symbol.empty()) { // Handle collision
                    index = (index + 1) % MAX_STOCKS;
                }
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
                    stocks[index].history.push_back(data);
                }
                ++numStocks;
            } else {
                cout << "Maximum number of stocks reached." << endl;
                break;
            }

        }
        file.close();
    };
