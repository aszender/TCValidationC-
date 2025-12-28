#include "CreditCard.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const std::string inputPath = (argc >= 2) ? argv[1] : "cards.txt";
    const std::string outputPath = (argc >= 3) ? argv[2] : "validation_results.txt";

    std::ifstream fin(inputPath);
    if (!fin) {
        std::cerr << "Could not open input file: " << inputPath << '\n';
        return 1;
    }

    std::ofstream fout(outputPath);
    if (!fout) {
        std::cerr << "Could not open output file: " << outputPath << '\n';
        return 1;
    }

    int validCount = 0;
    int invalidCount = 0;

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) {
            continue;
        }

        try {
            CreditCard card(line);
            const bool ok = card.isValid();

            fout << card.digits() << " : "
                 << (card.brand() ? std::string(to_string(*card.brand())) : std::string("Unknown"))
                 << " : " << (ok ? "valid" : "invalid")
                 << '\n';

            if (ok) {
                ++validCount;
            } else {
                ++invalidCount;
            }
        } catch (const CreditCard::Error& e) {
            fout << line << " : invalid_input : " << e.what() << '\n';
            ++invalidCount;
        } catch (const std::exception& e) {
            fout << line << " : error : " << e.what() << '\n';
            ++invalidCount;
        }
    }

    std::cout << "Processed file: " << inputPath << '\n'
              << "Wrote results:  " << outputPath << "\n\n"
              << "Totals\n"
              << "Valid:   " << validCount << '\n'
              << "Invalid: " << invalidCount << '\n';

    return 0;
}
