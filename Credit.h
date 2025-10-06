#pragma once
#include <array>
#include <string>

// Given an int, return the sum of its digits (e.g., 123 -> 6)
int getSumOfDigits(int n);

// Return true if cardNumber starts with prefix
bool startsWith(const std::string& cardNumber, const std::string& prefix);

namespace Constants {
   inline const std::array<std::string, 4> validPrefixes = { "4", "5", "6", "37" }; //C++ 20
}

bool hasValidPrefix(const std::string& cardNumber);
int sumOddDigitsRightToLeft(const std::string& cardnumber);
int sumEvenDigitsRightToLeft(const std::string& cardNumber);
bool isCardValid(const std::string& cardNumber);

