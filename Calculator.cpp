#include "Calculator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <<algorithm>

bool Calculator::isValidDouble(const std::string& s) {
	if (s.empty()) return false;

	size_t i = 0;
	if (s[i] == '+' || s[i] == '-') {
		i++;
	}

	if (i >= s.length()) return false;

	bool hasDigitsBefore = false;
	bool hasDecimal = false;
	bool hasDigitsAfter = false;

	while (i < s.length()) {
		if (s[i] >= '0' && s[i] <= '9') {
		   if (!hasDecimal) {
		   	hasDigitsBefore = true;
			} else {
			hasDigitsAfter = true;
			}
		} else if (s[i] == '.') {
			if (hasDecimal) return false;
			hasDecimal = true;
			} else {
			return false;
			}
	 		i++;
	}

	if (!hasDigitsBefore) return false;
	if (hasDecimal && !hasDigitsAfter) return false;

	return true;
}

void Calculator::parseNumber(const std::string& s, bool& isNeg, std::string& intPart, std::string& fracPart) {
    size_t start = 0;
    isNeg = false;
    if (s[0] == '-') {
        isNeg = true;
        start = 1;
    } else if (s[0] == '+') {
        start = 1;
    }

    size_t dotPos = s.find('.', start);
    if (dotPos == std::string::npos) {
        intPart = s.substr(start);
        fracPart = "";
    } else {
        intPart = s.substr(start, dotPos - start);
        fracPart = s.substr(dotPos + 1);
    }

    size_t firstNonZero = intPart.find_first_not_of('0');
    if (firstNonZero == std::string::npos) {
        intPart = "0";
    } else {
        intPart = intPart.substr(firstNonZero);
    }
}

int Calculator::compareAbsolute(const std::string& i1, const std::string& f1,
                                const std::string& i2, const std::string& f2) {
    if (i1.length() != i2.length()) {
        return i1.length() < i2.length() ? -1 : 1;
    }
    if (i1 != i2) {
        return i1 < i2 ? -1 : 1;
    }

    size_t maxLen = std::max(f1.length(), f2.length());
    std::string pf1 = f1, pf2 = f2;
    while (pf1.length() < maxLen) pf1 += "0";
    while (pf2.length() < maxLen) pf2 += "0";

    if (pf1 != pf2) {
        return pf1 < pf2 ? -1 : 1;
    }
    return 0;
}

std::string Calculator::addAbsolute(const std::string& i1, const std::string& f1,
                                    const std::string& i2, const std::string& f2) {
    size_t maxFrac = std::max(f1.length(), f2.length());
    std::string pf1 = f1, pf2 = f2;
    while (pf1.length() < maxFrac) pf1 += "0";
    while (pf2.length() < maxFrac) pf2 += "0";

    std::string resFrac = "";
    int carry = 0;
    for (int i = (int)maxFrac - 1; i >= 0; i--) {
        int sum = (pf1[i] - '0') + (pf2[i] - '0') + carry;
        carry = sum / 10;
        resFrac += (char)('0' + (sum % 10));
    }
    std::reverse(resFrac.begin(), resFrac.end());

    std::string resInt = "";
    int iIdx = (int)i1.length() - 1;
    int jIdx = (int)i2.length() - 1;
    while (iIdx >= 0 || jIdx >= 0 || carry > 0) {
        int sum = carry;
        if (iIdx >= 0) { sum += (i1[iIdx] - '0'); iIdx--; }
        if (jIdx >= 0) { sum += (i2[jIdx] - '0'); jIdx--; }
        carry = sum / 10;
        resInt += (char)('0' + (sum % 10));
    }
    std::reverse(resInt.begin(), resInt.end());

    return cleanResult(resInt, resFrac, false);
}

std::string Calculator::subtractAbsolute(const std::string& i1, const std::string& f1,
                                         const std::string& i2, const std::string& f2,
                                         bool resultNegative) {
    size_t maxFrac = std::max(f1.length(), f2.length());
    std::string pf1 = f1, pf2 = f2;
    while (pf1.length() < maxFrac) pf1 += "0";
    while (pf2.length() < maxFrac) pf2 += "0";

    std::string resFrac = "";
    int borrow = 0;
    for (int i = (int)maxFrac - 1; i >= 0; i--) {
        int sub = (pf1[i] - '0') - (pf2[i] - '0') - borrow;
        if (sub < 0) { sub += 10; borrow = 1; } else { borrow = 0; }
        resFrac += (char)('0' + sub);
    }
    std::reverse(resFrac.begin(), resFrac.end());

    std::string resInt = "";
    int iIdx = (int)i1.length() - 1;
    int jIdx = (int)i2.length() - 1;
    while (iIdx >= 0 || jIdx >= 0) {
        int sub = borrow;
        if (iIdx >= 0) { sub += (i1[iIdx] - '0'); iIdx--; }
        if (jIdx >= 0) { sub -= (i2[jIdx] - '0'); jIdx--; }
        if (sub < 0) { sub += 10; borrow = 1; } else { borrow = 0; }
        resInt += (char)('0' + sub);
    }
    std::reverse(resInt.begin(), resInt.end());

    return cleanResult(resInt, resFrac, resultNegative);
}

std::string Calculator::cleanResult(const std::string& intPart, const std::string& fracPart, bool isNeg) {
    size_t firstNonZero = intPart.find_first_not_of('0');
    std::string cleanInt = (firstNonZero == std::string::npos) ? "0" : intPart.substr(firstNonZero);

    std::string cleanFrac = fracPart;
    while (!cleanFrac.empty() && cleanFrac.back() == '0') {
        cleanFrac.pop_back();
    }

    std::string result = "";
    if (isNeg && !(cleanInt == "0" && cleanFrac.empty())) {
        result += "-";
    }
    result += cleanInt;
    if (!cleanFrac.empty()) {
        result += "." + cleanFrac;
    }
    return result;
}

std::string Calculator::addStrings(const std::string& num1, const std::string& num2) {
    bool neg1, neg2;
    std::string i1, f1, i2, f2;
    parseNumber(num1, neg1, i1, f1);
    parseNumber(num2, neg2, i2, f2);

    if (neg1 == neg2) {
        std::string absRes = addAbsolute(i1, f1, i2, f2);
        if (neg1 && absRes != "0") {
            return "-" + absRes;
        }
        return absRes;
    } else {
        int cmp = compareAbsolute(i1, f1, i2, f2);
        if (cmp == 0) {
            return "0";
        } else if (cmp > 0) {
            return subtractAbsolute(i1, f1, i2, f2, neg1);
        } else {
            return subtractAbsolute(i2, f2, i1, f1, neg2);
        }
    }
}

void Calculator::processFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token1, token2, extra;
        ss >> token1 >> token2 >> extra;

        if (token1.empty() || token2.empty() || !extra.empty()) {
            std::cout << "ERROR: expected exactly two numbers\n";
            continue;
        }

        bool valid1 = isValidDouble(token1);
        bool valid2 = isValidDouble(token2);

        if (!valid1 && !valid2) {
            std::cout << "ERROR: invalid number \"" << token1 << "\" \"" << token2 << "\"\n";
        } else if (!valid1) {
            std::cout << "ERROR: invalid number \"" << token1 << "\"\n";
        } else if (!valid2) {
            std::cout << "ERROR: invalid number \"" << token2 << "\"\n";
        } else {
            std::string result = addStrings(token1, token2);
            std::cout << token1 << " + " << token2 << " = " << result << "\n";
        }
    }
}


