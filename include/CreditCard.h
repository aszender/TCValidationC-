#pragma once

#include <cstddef>
#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

class CreditCard {
public:
    enum class Brand {
        Visa,
        MasterCard,
        AmericanExpress,
        Discover,
    };

    class Error : public std::invalid_argument {
    public:
        explicit Error(const std::string& message);
    };

    CreditCard();
    explicit CreditCard(std::string_view input);

    // Rule of Three (demonstration)
    CreditCard(const CreditCard& other);
    CreditCard& operator=(const CreditCard& other);
    ~CreditCard();

    // Modern C++ convenience: moves are safe and cheap here.
    CreditCard(CreditCard&& other) noexcept;
    CreditCard& operator=(CreditCard&& other) noexcept;

    void setInput(std::string_view input);

    // Original input as provided (may include spaces/hyphens).
    const std::string& input() const noexcept;

    // Normalized digits-only view.
    std::string_view digits() const noexcept;
    std::size_t length() const noexcept;

    // Brand is only available if the prefix matches a known brand.
    std::optional<Brand> brand() const noexcept;

    // Validation: length + known brand prefix + Luhn.
    bool isValid() const noexcept;

    // Operators
    friend std::ostream& operator<<(std::ostream& os, const CreditCard& card);
    friend bool operator==(const CreditCard& a, const CreditCard& b) noexcept;

private:
    void reset() noexcept;
    void assignDigits(std::string_view digitsOnly);

    static std::string normalizeToDigits(std::string_view input);

    static bool luhnCheck(std::string_view digits);
    static int sumOfDigits(int n) noexcept;

    static std::optional<Brand> detectBrand(std::string_view digits) noexcept;
    static const char* brandName(Brand brand) noexcept;

    std::string input_;
    char* digits_ = nullptr;     // owned, not null-terminated
    std::size_t length_ = 0;
};

std::string_view to_string(CreditCard::Brand brand) noexcept;
