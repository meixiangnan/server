#include "id_card_validator.h"

#include <chrono>
#include <map>
#include <random>
#include <set>
#include <trantor/utils/Date.h>

bool IdCardVailidater::ValidatorLength(const std::string& idCard)
{
    return idCard.length() == 18;
}

bool IdCardVailidater::ValidatorDigit(const std::string& idCard)
{
    for(size_t i = 0; i < idCard.size(); ++i)
    {
        char c = idCard[i];
        if (i == idCard.size() - 1 && (c == 'X' || c == 'x'))
        {
            continue;
        }

        if(!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

bool IdCardVailidater::ValidatorAge(int age)
{    
    return age >= 0 && age <= 120;
}

bool IdCardVailidater::ValidatorAreaCode(const std::string& idCard)
{
    static std::map<std::string, std::string> VaildAreaCode(
        {
            {"11", "北京"}, {"12", "天津"}, {"13", "河北"}, {"14", "山西"}, {"15", "内蒙古"},
            {"21", "辽宁"}, {"22", "吉林"}, {"23", "黑龙江"},
            {"31", "上海"}, {"32", "江苏"}, {"33", "浙江"}, {"34", "安徽"}, {"35", "福建"}, {"36", "江西"}, {"37", "山东"},
            {"41", "河南"}, {"42", "湖北"}, {"43", "湖南"}, {"44", "广东"}, {"45", "广西"}, {"46", "海南"},
            {"50", "重庆"}, {"51", "四川"}, {"52", "贵州"}, {"53", "云南"}, {"54", "西藏"},
            {"61", "陕西"}, {"62", "甘肃"}, {"63", "青海"}, {"64", "宁夏"}, {"65", "新疆"}
        }); 
    
    // 验证身份证号的地区码是否有效
    std::string areaCode = idCard.substr(0, 2);
    return VaildAreaCode.find(areaCode) != VaildAreaCode.end();
}

bool IdCardVailidater::ValidatorCheckSum(const std::string& idCard)
{
    // 校验身份证号的校验和是否有效
    // 校验和的计算方法：将前17位数字分别乘以对应的权重（7、9、10、5、8、4、2、1、6、3、7、9、10、5、8、4、2），将乘积相加，取模11得到校验位
    const int weights[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    int checkSum = 0;
    for(int i = 0; i < 17; i++)
    {
        checkSum += (idCard[i] - '0') * weights[i];
    }
    checkSum %= 11;
    char checkDigit = "10X98765432"[checkSum];
    return checkDigit == idCard[17];
    
}
////
bool TokenGenerator::Gen(std::string& token, int tokenLen /*= 32*/)
{
    // 定义令牌中可能包含的字符集
    const std::string charset = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "-_"; // 添加URL安全的特殊字符
    
    // 使用当前时间作为随机种子
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    
    // 使用高质量随机数生成器
    std::mt19937_64 rng(static_cast<uint64_t>(seed));
    
    // 均匀分布，用于从字符集中选择字符
    std::uniform_int_distribution<size_t> dist(0, charset.size() - 1);
    
    token.reserve(tokenLen);
    for (size_t i = 0; i < tokenLen; ++i) {
        token += charset[dist(rng)];
    }
    
    return true;
}
