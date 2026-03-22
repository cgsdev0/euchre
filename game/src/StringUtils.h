#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <iostream>
#include <random>
#include <sstream>
#include <string>

typedef unsigned int uint;

// Trims a string to a certain amount of characters (unicode aware).
std::string trimString(const std::string &a, int len, bool strict = false);

std::string generateCode(const unsigned int len, std::string seed = "");

unsigned int randomChar(int k = 255);

unsigned int srandom_char(std::mt19937 &gen, int k = 255);

std::string generateCode(const unsigned int len, std::string seed);

std::string nextSymbol(std::string::const_iterator start, std::string::const_iterator end);

bool isVariationSelector(const std::string &a);

std::string toHex(std::string a);

std::string trimString(const std::string &a, int len, bool strict);

#ifdef STRING_UTILS_IMPLEMENTATION

unsigned int randomChar(int k) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, k);
    return dis(gen);
}

unsigned int srandom_char(std::mt19937 &gen, int k) {
    std::uniform_int_distribution<> dis(0, k);
    return dis(gen);
}

std::string generateCode(const unsigned int len, std::string seed) {
    std::seed_seq s(seed.begin(), seed.end());
    std::mt19937 gen(s);
    const std::string chars =
        "abcdefghjkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUV23456789";
    const unsigned int l = chars.length() - 1;
    std::stringstream ss;
    for (uint i = 0; i < len; i++) {
        ss << chars[seed == "" ? randomChar(l) : srandom_char(gen, l)];
    }
    return ss.str();
}

std::string nextSymbol(std::string::const_iterator start, std::string::const_iterator end) {
    std::string res;
    if (start == end) return res;
    auto next = start + 1;
    res += *start;
    while (next != end && ((*next) & 0xC0) == 0x80)
        res += *next++;
    return res;
}

// Zero width joiner
const std::string zwj = "\u200D";

bool isVariationSelector(const std::string &a) {
    return a.length() == 3 && a[0] == (char)239 && a[1] == (char)184 && a[2] <= (char)143;
}

std::string toHex(std::string a) {
    std::stringstream res;
    for (char c : a) {
        int b = (int)c;
        if (b < 0) {
            b += 256;
        }
        res << b;
        res << " ";
    }
    return res.str();
}

std::string trimString(const std::string &a, int len, bool strict) {
    std::string res;
    if (!len) return res;
    res.reserve(len);
    std::string symbol, next;
    int count = 0;
    for (auto it = a.begin(); it != a.end(); it += symbol.length()) {
        symbol = nextSymbol(it, a.end());
        next = nextSymbol(it + symbol.length(), a.end());
        res += symbol;
        // std::cout << "symbol (" << count << "): '" << symbol << "' " << toHex(symbol) << std::endl;
        // std::cout << "\tnext (" << count << "): '" << next << "' " << toHex(next) << std::endl;

        if (strict) {
            count += next.length();
        } else {
            // Eat the ZWJ and another symbol
            if (next == zwj) count -= zwj.length();
            // Eat the variation selector
            else if (!isVariationSelector(next))
                count += next.length();
        }
        if (count >= len) break;
    }
    return res;
}

#endif // STRING_UTILS_IMPLEMENTATION

#endif
