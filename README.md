
````markdown
# 💳 TC Validator (C++)

Validate credit card numbers using the **Luhn algorithm** and **prefix check**.

## 🧩 Features
- Luhn checksum validation  
- Prefix validation (`4`, `5`, `6`, `37`)  
- Clean modular code (`Credit.cpp`, `Credit.h`)  
- Ready for testing with sample numbers  

## ⚙️ Build

### 🪟 Visual Studio
Open the project → Build → Run.

### 🐧 Command line (g++)
```bash
g++ -std=c++17 -O2 src/*.cpp -o tc_validator
./tc_validator
````

## ▶️ Usage

```bash
Enter a credit card number: 4111111111111111
Valid (prefix=4, Luhn OK)
```

## 🧪 Test Numbers

| Brand      | Number              | Result    |
| ---------- | ------------------- | --------- |
| Visa       | 4111 1111 1111 1111 | ✅ Valid   |
| Mastercard | 5555 5555 5555 4444 | ✅ Valid   |
| Discover   | 6011 1111 1111 1117 | ✅ Valid   |
| AmEx       | 3714 4963 5398 431  | ✅ Valid   |
| Any        | 1234 5678 9012 3456 | ❌ Invalid |


```

## 🪪 License

MIT License © 2025
