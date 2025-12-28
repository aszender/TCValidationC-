#include "CreditCard.h"

#include <algorithm>
#include <charconv>
#include <cstring>
#include <ostream>
#include <utility>

CreditCard::Error::Error(const std::string& message)
    : std::invalid_argument(message) {}

CreditCard::CreditCard() = default;

CreditCard::CreditCard(std::string_view input) {
    setInput(input);
}

CreditCard::CreditCard(const CreditCard& other)
    : input_(other.input_), digits_(nullptr), length_(0) {
    assignDigits(other.digits());
}

CreditCard& CreditCard::operator=(const CreditCard& other) {
    if (this == &other) {
        return *this;
    }

    input_ = other.input_;
    assignDigits(other.digits());
    return *this;
}

CreditCard::~CreditCard() {
    reset();
}

CreditCard::CreditCard(CreditCard&& other) noexcept
    : input_(std::move(other.input_)), digits_(other.digits_), length_(other.length_) {
    other.digits_ = nullptr;
    other.length_ = 0;
}

CreditCard& CreditCard::operator=(CreditCard&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    reset();
    input_ = std::move(other.input_);
    digits_ = other.digits_;
    length_ = other.length_;

    other.digits_ = nullptr;
    other.length_ = 0;

    return *this;
}

void CreditCard::setInput(std::string_view input) {
    input_ = std::string(input);

    const std::string digitsOnly = normalizeToDigits(input);
    if (digitsOnly.empty()) {
        throw Error("card number contains no digits");
    }

    assignDigits(digitsOnly);
}

const std::string& CreditCard::input() const noexcept {
    return input_;
}

std::string_view CreditCard::digits() const noexcept {
    return std::string_view(digits_, length_);
}

std::size_t CreditCard::length() const noexcept {
    return length_;
}

std::optional<CreditCard::Brand> CreditCard::brand() const noexcept {
    return detectBrand(digits());
}

bool CreditCard::isValid() const noexcept {
    // Industry typical lengths; this project historically validates 13–16.
    if (length_ < 13 || length_ > 16) {
        return false;
    }

    // Must match a known brand prefix.
    if (!detectBrand(digits()).has_value()) {
        return false;
    }

    return luhnCheck(digits());
}

void CreditCard::reset() noexcept {
    delete[] digits_;
    digits_ = nullptr;
    length_ = 0;
}

void CreditCard::assignDigits(std::string_view digitsOnly) {
    reset();

    length_ = digitsOnly.size();
    if (length_ == 0) {
        return;
    }

    digits_ = new char[length_];
    std::memcpy(digits_, digitsOnly.data(), length_);
}

std::string CreditCard::normalizeToDigits(std::string_view input) {
    std::string out;
    out.reserve(input.size());

    for (const char ch : input) {
        if (ch >= '0' && ch <= '9') {
            out.push_back(ch);
            continue;
        }
        if (ch == ' ' || ch == '-') {
            continue; // common formatting
        }

        throw Error("card number contains invalid character");
    }

    return out;
}

int CreditCard::sumOfDigits(int n) noexcept {
    int total = 0;
    while (n > 0) {
        total += (n % 10);
        n /= 10;
    }
    return total;
}

bool CreditCard::luhnCheck(std::string_view digits) {
    // Luhn: sum alternating digits from right; even positions doubled.
    int sum = 0;
    bool doubleDigit = false;

    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        const int d = (*it - '0');
        int add = d;
        if (doubleDigit) {
            add = sumOfDigits(d * 2);
        }
        sum += add;
        doubleDigit = !doubleDigit;
    }

    return (sum % 10) == 0;
}

static std::optional<int> parsePrefixInt(std::string_view digits, std::size_t n) noexcept {
    if (digits.size() < n) {
        return std::nullopt;
    }

    int value = 0;
    auto [ptr, ec] = std::from_chars(digits.data(), digits.data() + n, value);
    if (ec != std::errc{} || ptr != digits.data() + n) {
        return std::nullopt;
    }

    return value;
}

std::optional<CreditCard::Brand> CreditCard::detectBrand(std::string_view digits) noexcept {
    // Visa: prefix 4, length 13/16 (we allow 13–16 overall; length is checked elsewhere)
    if (!digits.empty() && digits.front() == '4') {
        return Brand::Visa;
    }

    // American Express: 34 or 37 and length 15 (length is checked in isValid)
    if (digits.size() >= 2) {
        const auto p2 = parsePrefixInt(digits, 2);
        if (p2 && (*p2 == 34 || *p2 == 37)) {
            return Brand::AmericanExpress;
        }
    }

    // Discover: 6011, 65, 644-649, 622126-622925
    if (digits.size() >= 2) {
        const auto p2 = parsePrefixInt(digits, 2);
        if (p2 && *p2 == 65) {
            return Brand::Discover;
        }
    }
    if (digits.size() >= 4) {
        const auto p4 = parsePrefixInt(digits, 4);
        if (p4 && *p4 == 6011) {
            return Brand::Discover;
        }
    }
    if (digits.size() >= 3) {
        const auto p3 = parsePrefixInt(digits, 3);
        if (p3 && (*p3 >= 644 && *p3 <= 649)) {
            return Brand::Discover;
        }
    }
    if (digits.size() >= 6) {
        const auto p6 = parsePrefixInt(digits, 6);
        if (p6 && (*p6 >= 622126 && *p6 <= 622925)) {
            return Brand::Discover;
        }
    }

    // MasterCard: 51-55 or 2221-2720
    if (digits.size() >= 2) {
        const auto p2 = parsePrefixInt(digits, 2);
        if (p2 && (*p2 >= 51 && *p2 <= 55)) {
            return Brand::MasterCard;
        }
    }
    if (digits.size() >= 4) {
        const auto p4 = parsePrefixInt(digits, 4);
        if (p4 && (*p4 >= 2221 && *p4 <= 2720)) {
            return Brand::MasterCard;
        }
    }

    return std::nullopt;
}

const char* CreditCard::brandName(Brand brand) noexcept {
    switch (brand) {
        case Brand::Visa:
            return "Visa";
        case Brand::MasterCard:
            return "MasterCard";
        case Brand::AmericanExpress:
            return "American Express";
        case Brand::Discover:
            return "Discover";
    }
    return "Unknown";
}

std::string_view to_string(CreditCard::Brand brand) noexcept {
    switch (brand) {
        case CreditCard::Brand::Visa:
            return "Visa";
        case CreditCard::Brand::MasterCard:
            return "MasterCard";
        case CreditCard::Brand::AmericanExpress:
            return "American Express";
        case CreditCard::Brand::Discover:
            return "Discover";
    }
    return "Unknown";
}

std::ostream& operator<<(std::ostream& os, const CreditCard& card) {
    const auto digits = card.digits();
    const std::size_t show = std::min<std::size_t>(4, digits.size());
    const std::string_view tail = digits.substr(digits.size() - show, show);

    os << "CreditCard{";
    if (const auto b = card.brand()) {
        os << "brand=\"" << CreditCard::brandName(*b) << "\", ";
    } else {
        os << "brand=\"Unknown\", ";
    }

    os << "digits=\"";
    for (std::size_t i = 0; i < digits.size() - show; ++i) {
        os << '*';
    }
    os << tail << "\", valid=" << (card.isValid() ? "true" : "false") << '}';
    return os;
}

bool operator==(const CreditCard& a, const CreditCard& b) noexcept {
    if (a.length_ != b.length_) {
        return false;
    }
    return std::memcmp(a.digits_, b.digits_, a.length_) == 0;
}
