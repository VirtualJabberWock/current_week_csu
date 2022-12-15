#include "TestTranslator.h"
#include <sstream>

vector<BYTE> TestTranslator::fromHexString(string str)
{
    vector<string> bytes_s = split(str, " ");
    ostringstream os;
    for (int i = 0; i < bytes_s.size(); i++) {
        if(bytes_s[i].length() == 1)
            bytes_s[i] = "0" + bytes_s[i];
        os << bytes_s[i];
    }
    string s = os.str();
    return HexToBytes(s);
}

vector<BYTE> TestTranslator::HexToBytes(string hex)
{
    vector<BYTE> bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

vector<string> TestTranslator::split(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
