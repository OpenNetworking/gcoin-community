// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"

#include "tinyformat.h"


std::ostream& operator<<(std::ostream& os, const CColorAmount& ca)
{
    CColorAmount::const_iterator it(ca.begin());
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

CFeeRate::CFeeRate(const CAmount& nFeePaid, size_t nSize)
{
    if (nSize > 0)
        nSatoshisPerK = nFeePaid*1000/nSize;
    else
        nSatoshisPerK = 0;
}

CAmount CFeeRate::GetFee(size_t nSize) const
{
    return 0;
}

std::string CFeeRate::ToString() const
{
    return strprintf("%d.%08d GCOIN/kB", nSatoshisPerK / COIN, nSatoshisPerK % COIN);
}

