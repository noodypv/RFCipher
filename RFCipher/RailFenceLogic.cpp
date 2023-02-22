#include "RFCipher.h"

//  ######################################
//  ##                                  ##
//  ##  MY ANCIENT CODE FROM CODEWARS   ##
//  ##                                  ##
//  ######################################

std::string RFCipher::RailFenceDecode(const char* chStr, const int size) {
    std::vector<std::string> v;
    std::string str(chStr, size - 1);

    v.resize(this->choosenCountOfRails);

    std::string strc = str;
    std::string sometext;

    while (sometext.length() != str.length())
        sometext += "a";

    bool flag = true;
    int pointer = 0;

    for (int i = 0; i < str.length(); i++) {
        v[pointer] += str[i];

        pointer += flag ? 1 : -1;
        flag = pointer == (this->choosenCountOfRails - 1) ? false : (pointer ? flag : true);
    }

    std::vector<int> strsizes;
    strsizes.resize(this->choosenCountOfRails);

    for (int i = 0; i < this->choosenCountOfRails; i++)
        strsizes[i] = v[i].length();

    std::vector<std::string> v2;
    v2.resize(this->choosenCountOfRails);

    for (int i = 0; i < strsizes.size(); i++) {
        v2[i] += strc.substr(0, strsizes[i]);
        strc.erase(0, strsizes[i]);
    }

    std::string total;
    std::vector<int> indexes;

    indexes.resize(this->choosenCountOfRails);

    for (int length = 0, flag = true, pointer = 0; length != str.length(); ++length) {
        total += v2[pointer][indexes[pointer]];
        ++indexes[pointer];
        pointer += flag ? 1 : -1;
        flag = pointer == (this->choosenCountOfRails - 1) ? false : (pointer ? flag : true);
    }
    return total;
}

std::string RFCipher::RailFenceEncode(const char* chStr, const int size) {
    std::vector<std::string> v;
    std::string str(chStr, size - 1);

    v.resize(this->choosenCountOfRails);

    bool flag = true;
    int pointer = 0;

    for (int i = 0; i < str.length(); i++) {
        v[pointer] += str[i];

        pointer += flag ? 1 : -1;
        flag = pointer == (this->choosenCountOfRails - 1) ? false : (pointer ? flag : true);
    }

    std::string total;

    for (auto& index : v) 
        total += index;

    return total;
}