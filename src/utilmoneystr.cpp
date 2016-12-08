// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "utilmoneystr.h"

#include "primitives/transaction.h"
#include "tinyformat.h"
#include "utilstrencodings.h"

using namespace std;

string FormatMoney(const CAmount& n, bool fPlus)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    int64_t n_abs = (n > 0 ? n : -n);
    int64_t quotient = n_abs/COIN;
    int64_t remainder = n_abs%COIN;
    std::string str = strprintf("%d.%08d", quotient, remainder);

    // Right-trim excess zeros before the decimal point:
    int nTrim = 0;
    for (int i = str.size()-1; (str[i] == '0' && isdigit(str[i-2])); --i)
        ++nTrim;
    if (nTrim)
        str.erase(str.size()-nTrim, nTrim);

    if (n < 0)
        str.insert((unsigned int)0, 1, '-');
    else if (fPlus && n > 0)
        str.insert((unsigned int)0, 1, '+');
    return str;
}

string FormatColor(const type_Color& n)
{
    // Note: not using straight sprinVtf here because we do NOT want
    // localized number formatting.
    return strprintf("%d", n);
}

string FormatMoney(const CColorAmount& m, bool fPlus)
{
    std::string str;
    for (CColorAmount::const_iterator it(m.begin()); it != m.end(); it++) {
        str += strprintf("%d:", it->first);
        str += FormatMoney(it->second, fPlus);
        str += std::string(",");
    }
    str.erase(str.end() - 1);
    return str;
}

bool ParseMoney(const string& str, CAmount& nRet)
{
    return ParseMoney(str.c_str(), nRet);
}

bool ParseMoney(const char* pszIn, CAmount& nRet)
{
    string strWhole;
    int64_t nUnits = 0;
    const char* p = pszIn;
    while (isspace(*p))
        p++;
    for (; *p; p++)
    {
        if (*p == '.')
        {
            p++;
            int64_t nMult = CENT*10;
            while (isdigit(*p) && (nMult > 0))
            {
                nUnits += nMult * (*p++ - '0');
                nMult /= 10;
            }
            break;
        }
        if (isspace(*p))
            break;
        if (!isdigit(*p))
            return false;
        strWhole.insert(strWhole.end(), *p);
    }
    for (; *p; p++)
        if (!isspace(*p))
            return false;
    if (strWhole.size() > 10) // guard against 63 bit overflow
        return false;
    if (nUnits < 0 || nUnits > COIN)
        return false;
    int64_t nWhole = atoi64(strWhole);
    CAmount nValue = nWhole*COIN + nUnits;

    nRet = nValue;
    return true;
}

bool ParseMoney(const string& str, CColorAmount& mRet)
{
    size_t found;
    for (size_t i = 0; i < str.size(); i = found + 1) {
        found = str.find(",", i);
        if (found == string::npos)
            found = str.size();
        string part = str.substr(i, found - 1);

        size_t c = part.find(":");
        if (c == string::npos)
            return false;
        type_Color color;
        CAmount nValue;
        if (!ParseColor(part.substr(0, c - 1).c_str(), color))
            return false;
        if (!ParseMoney(part.substr(c + 1, part.size() - 1).c_str(), nValue))
            return false;
        mRet[color] = nValue;
    }
    return true;
}

bool ParseColor(const char* pszIn, type_Color& nRet)
{
    string strColor;
    const char* p = pszIn;
    while (isspace(*p))
        p++;
    for (; *p; p++)
    {
        if (isspace(*p))
            break;
        if (!isdigit(*p))
            return false;
        strColor.insert(strColor.end(), *p);
    }
    for (; *p; p++)
        if (!isspace(*p))
            return false;
    if (strColor.size() > 10) // guard against 32 bit overflow
        return false;
    nRet = atoi(strColor);
    return true;
}
