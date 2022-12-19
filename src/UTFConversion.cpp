#include"UTFConversion.h"

std::string toUTF8(std::wstring str)
{
    std::string data;
    for(wchar_t& c : str)
    {
        if(c <= 0x007F)
        {
            data += (char)c;
        }
        else if(c <= 0x07FF)
        {
            int v = 0xC080;
            int finalChar = v + (c & 0x3F) + ((c & 0x7C0) << 2);

            data += (finalChar >> 8) & 0xFF;
            data += (finalChar) & 0xFF;
        }
        else if(c < 0xFFFF)
        {
            int v = 0xE08080;
            int finalChar = v + (c & 0x3F) + ((c & 0xFC0) << 2) + ((c & 0xF000) << 4);
            
            data += (finalChar >> 16) & 0xFF;
            data += (finalChar >> 8) & 0xFF;
            data += (finalChar) & 0xFF;
        }
    }
    return data;
}

std::wstring toWideString(std::string str)
{
    std::wstring convertedString = L"";
    for(char& c : str)
    {
        convertedString += (wchar_t)c;
    }
    return convertedString;
}

std::wstring UTF8toWideString(std::string str)
{
    std::wstring convertedString = L"";
    int index = 0;
    while (index < str.size())
    {
        if((unsigned char)str[index] <= 0x007F)
        {
            convertedString += (wchar_t)str[index];
            index++;
        }
        else if((unsigned char)str[index] <= 0b11000000)
        {
            if(index+1 >= str.size())
                break;
            int mask = 0xC080;
            int byte1 = str[index] & 0b00011111;
            int byte2 = str[index+1] & 0b00111111;
            mask += (byte1 << 8) + (byte2);
            convertedString += (wchar_t)mask;
            index+=2;
        }
        else if((unsigned char)str[index] <= 0b11100000)
        {
            if(index+2 >= str.size())
                break;
            int mask = 0xE08080;
            int byte1 = str[index] & 0b00001111;
            int byte2 = str[index+1] & 0b00111111;
            int byte3 = str[index+2] & 0b00111111;
            mask += (byte1 << 16) + (byte2 << 8) + (byte3);
            convertedString += (wchar_t)mask;
            index+=3;
        }
        else if((unsigned char)str[index] <= 0b11110000)
        {
            index+=4;
        }
    }
    
    return convertedString;
}

