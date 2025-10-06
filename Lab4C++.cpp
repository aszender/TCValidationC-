#include <fstream>
#include <iostream>
#include <string>
#include "Credit.h"   

int main() {
    std::ifstream fin("cards.txt");
    if (!fin) {
        std::cerr << "Could not open cards.txt\n";
        return 1;
    }

    std::string ccNumber;
    int validCount = 0;
    int invalidCount = 0;

    // read each card number
    while (fin >> ccNumber) {
        bool ok = isCardValid(ccNumber);
        std::cout << ccNumber << " : " << (ok ? "valid" : "invalid") << '\n';
        if (ok) ++validCount; else ++invalidCount;
    }

    fin.close();

    std::cout << "\nTotals\n";
    std::cout << "Valid:   " << validCount << '\n';
    std::cout << "Invalid: " << invalidCount << '\n';

    return 0;
}

