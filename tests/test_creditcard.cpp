#include "CreditCard.h"

#include <cstdlib>
#include <iostream>

static int failures = 0;

static void expect(bool condition, const char* message) {
    if (!condition) {
        ++failures;
        std::cerr << "FAIL: " << message << '\n';
    }
}

int main() {
    // Valid examples
    {
        CreditCard visa("4111111111111111");
        expect(visa.isValid(), "Visa should be valid");
        expect(visa.brand().has_value() && *visa.brand() == CreditCard::Brand::Visa, "Visa brand detection");
    }

    {
        CreditCard mc("5555 5555 5555 4444");
        expect(mc.isValid(), "MasterCard should be valid (formatted input)");
        expect(mc.brand().has_value() && *mc.brand() == CreditCard::Brand::MasterCard, "MasterCard brand detection");
    }

    {
        CreditCard amex("378282246310005");
        expect(amex.isValid(), "Amex should be valid");
        expect(amex.brand().has_value() && *amex.brand() == CreditCard::Brand::AmericanExpress, "Amex brand detection");
    }

    {
        CreditCard discover("6011-1111-1111-1117");
        expect(discover.isValid(), "Discover should be valid (hyphenated input)");
        expect(discover.brand().has_value() && *discover.brand() == CreditCard::Brand::Discover, "Discover brand detection");
    }

    // Invalid Luhn
    {
        CreditCard bad("4111111111111112");
        expect(!bad.isValid(), "Invalid Luhn should fail");
    }

    // Invalid characters should throw
    {
        bool threw = false;
        try {
            CreditCard x("4111abcd");
            (void)x;
        } catch (const CreditCard::Error&) {
            threw = true;
        }
        expect(threw, "Invalid characters should throw");
    }

    // Operator== compares normalized digits
    {
        CreditCard a("5555 5555 5555 4444");
        CreditCard b("5555555555554444");
        expect(a == b, "operator== should match normalized digits");
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    }

    std::cerr << failures << " test(s) failed.\n";
    return 1;
}
