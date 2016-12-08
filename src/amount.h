// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AMOUNT_H
#define BITCOIN_AMOUNT_H

#include "serialize.h"

#include <iostream>
#include <stdlib.h>
#include <string>

typedef int64_t CAmount;

static const CAmount COIN = 100000000;
static const CAmount CENT = 1000000;

/** No amount larger than this (in satoshi) is valid */
static const int64_t MAX_MONEY = 10000000000ll * COIN;

typedef uint32_t type_Color;
typedef uint32_t tx_type;

typedef std::map<type_Color, CAmount> colorAmount_t;

inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

class CColorAmount : public colorAmount_t
{
public:
    CColorAmount() {}

    void init(type_Color color, CAmount value)
    {
        this->clear();
        this->insert(std::pair<type_Color, CAmount>(color, value));
    }

    CColorAmount(type_Color color, CAmount value)
    {
        init(color, value);
    }

    CColorAmount operator-() const
    {
        CColorAmount ret;
        for (CColorAmount::const_iterator it(this->begin()); it != this->end(); it++)
            ret.insert(std::pair<type_Color, CAmount>(it->first, -it->second));
        return ret;
    }

    CColorAmount operator+(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            std::pair<CColorAmount::iterator, bool> ret;
            ret = lhs.insert(*it);
            if (!ret.second)
                ret.first->second += it->second;
        }
        return lhs;
    }

    CColorAmount operator+(const CAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        if (lhs.size() == 1)
            lhs.begin()->second += rhs;
        return lhs;
    }

    CColorAmount operator-(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            if (!lhs.count(it->first))
                lhs[it->first] -= it->second;
            else
                lhs[it->first] = -it->second;
            if (lhs[it->first] == 0)
                lhs.erase(it->first);
        }
        return lhs;
    }

    CColorAmount operator-(const CAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        if (lhs.size() == 1) {
            lhs.begin()->second -= rhs;
            if (lhs[0] == 0)
                lhs.clear();
        }
        else
            return lhs;
    }

    CColorAmount operator*(const CAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::iterator it(lhs.begin()); it != lhs.end(); it++)
            it->second *= rhs;
        return lhs;
    }

    CColorAmount operator/(const CAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::iterator it(lhs.begin()); it != lhs.end(); it++)
            it->second /= rhs;
        return lhs;
    }

    CColorAmount operator%(const CAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::iterator it(lhs.begin()); it != lhs.end(); it++)
            it->second %= rhs;
        return lhs;
    }

    bool operator>(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::const_iterator itr(rhs.begin()); itr != rhs.end(); itr++) {
            CColorAmount::const_iterator itl = lhs.find(itr->first);
            if (itl == rhs.end())
                return false;
            else if (itl->second <= itr->second)
                return false;
        }

        return true;
    }

    bool operator>=(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::const_iterator itr(rhs.begin()); itr != rhs.end(); itr++) {
            CColorAmount::const_iterator itl = lhs.find(itr->first);
            if (itl == rhs.end())
                return false;
            else if (itl->second < itr->second)
                return false;
        }

        return true;
    }

    bool operator<(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        for (CColorAmount::const_iterator itl(lhs.begin()); itl != lhs.end(); itl++) {
            CColorAmount::const_iterator itr = rhs.find(itl->first);
            if (itr == rhs.end())
                return false;
            else if (itl->second >= itr->second)
                return false;
        }

        return true;
    }

    bool operator<=(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);

        for (CColorAmount::const_iterator itl(lhs.begin()); itl != lhs.end(); itl++) {
            CColorAmount::const_iterator itr = rhs.find(itl->first);
            if (itr == rhs.end())
                return false;
            else if (itl->second > itr->second)
                return false;
        }

        return true;
    }

    bool operator==(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        if (lhs.size() != rhs.size()) {
            return false;
        } else {
            CColorAmount::const_iterator itl(lhs.begin());
            CColorAmount::const_iterator itr(rhs.begin());
            for (; itl != lhs.end() && itr != rhs.end(); itl++, itr++) {
                if (itl->first != itr->first || itl->second != itr->second)
                    return false;
            }
        }

        return true;
    }

    bool operator!=(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        return !(lhs == rhs);
    }

    CColorAmount& operator=(const colorAmount_t& rhs)
    {
        this->clear();
        for (colorAmount_t::const_iterator it(rhs.begin()); it != rhs.end(); it++)
            this->insert(*it);
        return *this;
    }

    CColorAmount& operator+=(const CColorAmount& rhs)
    {
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            std::pair<CColorAmount::iterator, bool> ret;
            ret = this->insert(*it);
            if (!ret.second)
                ret.first->second += it->second;
        }
        return *this;
    }

    CColorAmount& operator+=(const CAmount& rhs)
    {
        if (this->size() == 1)
            this->begin()->second += rhs;
        return *this;
    }

    CColorAmount& operator-=(const CColorAmount& rhs)
    {
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            if (this->count(it->first))
                (*this)[it->first] -= it->second;
            else
                (*this)[it->first] = -it->second;
            if ((*this)[it->first] == 0)
                this->erase(it->first);
        }
        return *this;
    }

    CColorAmount& operator-=(const CAmount& rhs)
    {
        if (this->size() == 1)
            this->begin()->second -= rhs;
        if (this->begin()->second == 0)
            this->clear();
        return *this;
    }

    CColorAmount operator*=(const CAmount& rhs)
    {
        for (CColorAmount::iterator it(this->begin()); it != this->end(); it++)
            it->second *= rhs;
        return *this;
    }

    CColorAmount operator/=(const CAmount& rhs)
    {
        for (CColorAmount::iterator it(this->begin()); it != this->end(); it++)
            it->second /= rhs;
        return *this;
    }

    CColorAmount operator%=(const CAmount& rhs)
    {
        for (CColorAmount::iterator it(this->begin()); it != this->end(); it++)
            it->second %= rhs;
        return *this;
    }

    CColorAmount& operator++()
    {
        if (this->size() == 1)
            this->begin()->second += 1;
        return *this;
    }

    CColorAmount& operator--()
    {
        if (this->size() == 1)
            this->begin()->second -= 1;
        return *this;
    }

    type_Color Color() const
    {
        if (this->size() == 1)
            return this->begin()->first;
        else
            return -1;
    }

    CAmount Value() const
    {
        if (this->size() == 1)
            return this->begin()->second;
        else if (this->size() == 0)
            return 0;
        else
            return -1;
    }

    CAmount TotalValue() const
    {
        CAmount value = 0;
        for (CColorAmount::const_iterator it(this->begin()); it != this->end(); it++)
            value += it->second;
        return value;
    }

    bool IsPos() const
    {
        for (CColorAmount::const_iterator it(this->begin()); it != this->end(); it++)
            if (it->second < 0)
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const CColorAmount& ca);
};

/** Type-safe wrapper class to for fee rates
 * (how much to pay based on transaction size)
 */
class CFeeRate
{
private:
    CAmount nSatoshisPerK; // unit is satoshis-per-1,000-bytes
public:
    CFeeRate() : nSatoshisPerK(0) { }
    explicit CFeeRate(const CAmount& _nSatoshisPerK): nSatoshisPerK(_nSatoshisPerK) { }
    CFeeRate(const CAmount& nFeePaid, size_t nSize);
    CFeeRate(const CFeeRate& other) { nSatoshisPerK = other.nSatoshisPerK; }

    CAmount GetFee(size_t size) const; // unit returned is satoshis
    CAmount GetFeePerK() const { return GetFee(1000); } // satoshis-per-1000-bytes

    friend bool operator<(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK < b.nSatoshisPerK; }
    friend bool operator>(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK > b.nSatoshisPerK; }
    friend bool operator==(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK == b.nSatoshisPerK; }
    friend bool operator<=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK <= b.nSatoshisPerK; }
    friend bool operator>=(const CFeeRate& a, const CFeeRate& b) { return a.nSatoshisPerK >= b.nSatoshisPerK; }
    std::string ToString() const;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(nSatoshisPerK);
    }
};

#endif //  BITCOIN_AMOUNT_H

