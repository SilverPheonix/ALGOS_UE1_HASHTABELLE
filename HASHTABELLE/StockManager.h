#ifndef STOCKMANAGER_H_INCLUDED
#define STOCKMANAGER_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

//Struktur für Stock History Data
struct StockData {
    string date;
    double open;
    double high;
    double low;
    double close;
    double volume;
    double adjClose;
};

//Struktur für Stock Files
struct Stock {
    string name;
    string wkn;
    string symbol;
    //Speicherung der Daten
    //Vektor lässt sich besser erweitern als array
    vector<StockData> history;
};

class StockManager{
public:
    //Hashfunktion
    int customHash(const string& word);
    void addStock(const string& name, const string& wkn, const string& symbol);
    void deleteStock();
    void importStockData();
    void searchStock();
    void plotStock();
    void saveToFile();
    void loadFromFile();
private:
    //Es dürfen Maximal 1000 Stocks in der Hashtabelle sein
    static const int MAX_STOCKS = 1000;
    Stock stocks[MAX_STOCKS];
    int numStocks = 0;
};
#endif // STOCKMANAGER_H_INCLUDED
