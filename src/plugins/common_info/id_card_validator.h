
#pragma once
#include <string>

class IdCardVailidater
{
public:
    
    static bool ValidatorLength(const std::string& idCard);
    static bool ValidatorDigit(const std::string& idCard);
    static bool ValidatorAge(int age);
    static bool ValidatorAreaCode(const std::string& idCard);
    static bool ValidatorCheckSum(const std::string& idCard);
};



class TokenGenerator
{
public:
    static bool Gen(std::string& token, int tokenLen = 32);
};