#pragma once
#include <map>
#include <vector>
#include <locale>
#include <codecvt>
#include <string>

using namespace std;

static vector<string> split(string raw, string delimiter)
{
	vector<string> v = vector<string>();
	string s = raw + "";
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		v.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	if (v.size() > 0) {
		v.push_back(s);
	}
	else {
		v.push_back(raw);
	}
	return v;
}
static char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static string ToHex(string data) {
	string s = "";
	for (int i = 0; i < data.length(); ++i)
	{
		char const byte = data[i];

		s += hex_chars[(byte & 0xF0) >> 4];
		s += hex_chars[(byte & 0x0F) >> 0];
		s += " ";
	}
	return s;
}

static map<string, long long> parseJson(string json_) {
	map<string, long long> null = {};
	map<string, long long> jsmap = {};
	int end = json_.size()-1;
	if (json_.at(0) != '{') return null;
	if (json_.at(end) != '}') return null;
	string json = json_.substr(1, end);
	vector<string> parts = split(json, ",");
	try {
		for (int i = 0; i < parts.size(); i++) {
			vector<string> obj = split(parts[i], ":");
			if (obj.size() != 2) return null;
			jsmap.emplace(std::make_pair(obj[0], stoll(obj[1])));
		}
		return jsmap;
	}
	catch (...) {
		return null;
	}
}

static LPWSTR StringToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, ((__int64)res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}


