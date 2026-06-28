
#pragma once

enum GameErrorCode
{
    Fail = -1,
    Succ = 0,
    InvaildAccount,
    PasswordError,
    InvaildUserId,
    InvaildToken,
    DBError,
    IdCardLengthError,
    IdCardDigitError,
    IdCardAgeError,
    IdCardAreaError,
    IdCardCheckSumError,
    NickNameIsNull,
    HeadIconIsNull,
    AccountExists,
    UserOrPwdNull,
    AgeCannotLoginNow,
    AgeDayDuringMoreThanOneHour,
};

class UserBaseInfo
{
public:
    std::string NickName;
    std::string HeadIcon;
};