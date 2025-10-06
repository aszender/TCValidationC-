#include <iostream>
#include <vector>
#include "Credit.h"

int getSumOfDigits(int n) {
    int total = 0;
    while (n > 0) {
        total += n % 10;
        n /= 10;
    }
    return total;
}

bool startsWith(const std::string& cardnumber, const std::string& prefix) {
	if (prefix.size() > cardnumber.size()) { return false; };
	std::string beginning = cardnumber.substr(0, prefix.size());
	return beginning == prefix;
}

bool hasValidPrefix(const std::string& cardNumber) {
    for (const std::string& prefix : Constants::validPrefixes) { //for each 
        if (startsWith(cardNumber, prefix))
            return true;
    }
    return false;
}

int sumOddDigitsRightToLeft(const std::string& cardnumber) {
    int sum = 0;
    for (int i = cardnumber.size() - 1; i >= 0; i -= 2) {
        char c = cardnumber[i];
        int d = c - '0'; //convert from ascii code of letters to int
        sum += d;
    }
    return sum;
}

int sumEvenDigitsRightToLeft(const std::string& cardNumber) {
    int sum = 0;
    // Start from the second-to-last digit, move left by 2
    for (int i = cardNumber.size() - 2; i >= 0; i -= 2) {
        char c = cardNumber[i];
        int d = c - '0';
        int doubled = d * 2;

        // if the doubled number has 2 digits, add them (like 12 → 1 + 2 = 3)
        sum += getSumOfDigits(doubled);
    }

    return sum;
}

bool isCardValid(const std::string& cardNumber) {
    int length = cardNumber.size();

    // 1. Check card length
    if (length < 13 || length > 16) {
        return false;
    }

    // 2. Check prefix
    if (!hasValidPrefix(cardNumber)) {
        return false;
    }

    // 3. Add up even + odd digits
    int total = sumEvenDigitsRightToLeft(cardNumber)
        + sumOddDigitsRightToLeft(cardNumber);

    // 4. If total % 10 == 0, it’s valid
    return (total % 10 == 0);
}

