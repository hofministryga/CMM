#pragma once
#include<iostream>

std::string toUTF8(std::wstring str);
std::wstring toWideString(std::string str);
std::wstring UTF8toWideString(std::string str);