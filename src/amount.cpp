// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"

#include "tinyformat.h"


std::ostream& operator<<(std::ostream& os, const CColorAmount& ca)
{
    CColorAmount::const_iterator it(ca.begin());
    if (it == ca.end())
        return os;
    while(true) {
        os << it->first << ":" << it->second;
        it++;
        if (it != ca.end())
            os << ", ";
        else
            break;
    }
    return os;
}

CColorAmount const operator+(const CColorAmount &lhs, const CColorAmount &rhs)
{
    return CColorAmount(lhs) += rhs;
}

CColorAmount const operator-(const CColorAmount &lhs, const CColorAmount &rhs)
{
    return CColorAmount(lhs) -= rhs;
}

CColorAmount const operator+(const CColorAmount &lhs, const CAmount &rhs)
{
    return CColorAmount(lhs) += rhs;
}

CColorAmount const operator-(const CColorAmount &lhs, const CAmount &rhs)
{
    return CColorAmount(lhs) -= rhs;
}

CColorAmount const operator*(const CColorAmount &lhs, const CAmount &rhs)
{
    return CColorAmount(lhs) *= rhs;
}

CColorAmount const operator/(const CColorAmount &lhs, const CAmount &rhs)
{
    return CColorAmount(lhs) /= rhs;
}

CColorAmount const operator%(const CColorAmount &lhs, const CAmount &rhs)
{
    return CColorAmount(lhs) %= rhs;
}

CFeeRate::CFeeRate(const CColorAmount& mFeePaid, size_t nSize)
{
    if (nSize > 0)
        mSatoshisPerK = mFeePaid*1000/nSize;
    else
        mSatoshisPerK.init(1, 0);
}

CFeeRate::CFeeRate(const CAmount& nFeePaid, size_t nSize)
{
    CFeeRate(CColorAmount(1, nFeePaid), nSize);
}

CColorAmount CFeeRate::GetFee(size_t nSize) const
{
    return CColorAmount(1, 0);
}

std::string CFeeRate::ToString() const
{
    return strprintf("%d.%08d GCOIN/kB", mSatoshisPerK.Value() / COIN, mSatoshisPerK.Value() % COIN);
}

