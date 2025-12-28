# Copilot instructions (TC Validator C++)

## Big picture
- Portfolio-ready implementation is a small **library + CLI**:
  - Library: `CreditCard` class (brand detection + Luhn validation)
  - CLI: reads card numbers from a file and writes validation results to an output file

## Key files
- Class API: `include/CreditCard.h`
- Implementation: `src/CreditCard.cpp`
- CLI entry point: `src/main.cpp`
- Unit tests: `tests/test_creditcard.cpp`
- Sample input: `cards.txt`

## Project conventions
- Prefer **encapsulation** via `CreditCard` instead of free functions.
- Card brand detection is represented as `std::optional<CreditCard::Brand>`:
  - Known prefix → `Brand`
  - Unknown prefix → `std::nullopt`
- Validation contract: `CreditCard::isValid()` checks length (13–16), then known-brand prefix, then Luhn.
- Input normalization accepts digits plus common formatting (`' '` and `'-'`); any other non-digit throws `CreditCard::Error`.
- Rule of Three is intentionally demonstrated: `CreditCard` owns a `char*` digits buffer and implements deep copy.

## Build & test
- Recommended: CMake
  - Configure/build: `cmake -S . -B build && cmake --build build`
  - Tests: `ctest --test-dir build --output-on-failure`
- Manual build (no CMake available):
  - `clang++ -std=c++20 -O2 -Iinclude src/main.cpp src/CreditCard.cpp -o tc_validator`
  - `clang++ -std=c++20 -O2 -Iinclude tests/test_creditcard.cpp src/CreditCard.cpp -o tc_validator_tests`

## Making changes safely
- If updating brand rules or validation rules, update `CreditCard::detectBrand()` and/or `CreditCard::isValid()` together so tests remain consistent.
- Add new examples to `tests/test_creditcard.cpp` for any new brand prefixes or normalization behavior.
