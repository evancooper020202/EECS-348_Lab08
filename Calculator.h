#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <vector>

class Calculator {
public:
	static bool isValidDouble(const std::strings& s);
	static std::string addStrings(const std::string& num1, const std::string& num2);
	static void processFile(const std::string& filename);

	static void parseNumber(const std::string& s, bool& isNeg, std::string& fractPart);
	static int compareAbsolute(const std::string& il, const std::string& i2, const std::string& f2);
	static std::string addAbsolute(const std:string& il, const std::string& f1, const std::string& i2, const std::string& f2)
	static std::string subtractAbsolute(const std:string& il, const std::string& f1, const std::string& i2, const std::string& f2, bool resultNegative)
	static std::string cleanResult(const std::string& intPart, const std::string& fracPart, bool isNeg):
};

#endif



