
# TC Validator (C++)

A small, production-style C++ console tool that validates credit card numbers with the **Luhn checksum** and performs **card brand detection** (Visa, MasterCard, AmEx, Discover).

## Project layout
- `include/CreditCard.h` — `CreditCard` class (encapsulation, operators, brand detection, validation)
- `src/CreditCard.cpp` — implementation (Luhn + prefix/range rules)
- `src/main.cpp` — CLI: reads an input file and writes results to an output file
- `tests/test_creditcard.cpp` — lightweight unit tests (no external framework)
- `cards.txt` — sample input (one number per line)

The original school-style implementation in `Credit.cpp` / `Credit.h` and `Lab4C++.cpp` is kept for reference; the portfolio-ready version is the CMake + `src/` + `include/` code.

## Build

### Recommended: CMake (cross-platform)
```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

If `cmake` is not installed, install it via your platform package manager (e.g., Homebrew on macOS: `brew install cmake`).

### Manual build (macOS/Linux)
```bash
clang++ -std=c++20 -O2 -Iinclude src/main.cpp src/CreditCard.cpp -o tc_validator
clang++ -std=c++20 -O2 -Iinclude tests/test_creditcard.cpp src/CreditCard.cpp -o tc_validator_tests
```

## Usage

The CLI reads an input file and writes results to an output file.

```bash
./tc_validator cards.txt validation_results.txt
```

Output format (one line per card):
```text
4111111111111111 : Visa : valid
1234567890123456 : Unknown : invalid
```

Notes:
- Inputs may include spaces or hyphens (e.g., `5555 5555 5555 4444`, `6011-1111-1111-1117`).
- Invalid characters throw a `CreditCard::Error` and are reported as `invalid_input` in the output file.

## Architecture notes

`CreditCard` stores the original input and a normalized digits-only representation.

- `CreditCard::brand()` returns `std::optional<Brand>` (unknown prefix → `std::nullopt`).
- `CreditCard::isValid()` checks length (13–16), known brand prefix, then runs Luhn.
- Rule of Three is demonstrated via an owned `char*` buffer with deep copy semantics.

## License

MIT
