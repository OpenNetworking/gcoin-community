// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AMOUNT_H
#define BITCOIN_AMOUNT_H

#include "serialize.h"

#include <iostream>
#include <stdexcept>
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

    bool operator>(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        CColorAmount::const_iterator itl(lhs.begin());
        CColorAmount::const_iterator itr(rhs.begin());
        if (itl == lhs.end())
            return false;
        for (; itr != rhs.end(); itr++) {
            for (; itl != lhs.end() && itl->first != itr->first; itl++);
            if (itl == lhs.end())
                return false;
            if (itl->second <= itr->second)
                return false;
        }

        return true;
    }

    bool operator>=(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        CColorAmount::const_iterator itl(lhs.begin());
        CColorAmount::const_iterator itr(rhs.begin());
        for (; itr != rhs.end(); itr++) {
            for (; itl != lhs.end() && itl->first != itr->first; itl++);
            if (itl == lhs.end())
                return false;
            if (itl->second < itr->second)
                return false;
        }

        return true;
    }

    bool operator<(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        CColorAmount::const_iterator itl(lhs.begin());
        CColorAmount::const_iterator itr(rhs.begin());
        if (itr == rhs.end())
            return false;
        for (; itl != lhs.end(); itl++) {
            for (; itr != rhs.end() && itl->first != itr->first; itr++);
            if (itr == rhs.end())
                return false;
            if (itl->second >= itr->second)
                return false;
        }

        return true;
    }

    bool operator<=(const CColorAmount& rhs) const
    {
        CColorAmount lhs = (*this);
        CColorAmount::const_iterator itl(lhs.begin());
        CColorAmount::const_iterator itr(rhs.begin());
        for (; itl != lhs.end(); itl++) {
            for (; itr != rhs.end() && itl->first != itr->first; itr++);
            if (itr == rhs.end())
                return false;
            if (itl->second > itr->second)
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

    CColorAmount& operator+=(const CColorAmount &rhs)
    {
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            std::pair<CColorAmount::iterator, bool> ret;
            ret = this->insert(*it);
            if (!ret.second)
                ret.first->second += it->second;
        }
        return *this;
    }

    CColorAmount& operator-=(const CColorAmount& rhs)
    {
        for (CColorAmount::const_iterator it(rhs.begin()); it != rhs.end(); it++) {
            if (!this->count(it->first))
                (*this)[it->first] = -it->second;
            else
                (*this)[it->first] -= it->second;
            if ((*this)[it->first] == 0)
                this->erase(it->first);
        }
        return *this;
    }

    CColorAmount& operator+=(const CAmount& rhs)
    {
        if (this->size() == 1)
            this->begin()->second += rhs;
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
            (*this) += 1;
        else
            throw std::runtime_error("CColorAmount::operator--(): The size of color amount should be 1.");
        return *this;
    }

    type_Color Color() const
    {
        if (this->size() != 1)
            throw std::runtime_error("CColorAmount::Color(): The size of color amount should be 1.");
        return this->begin()->first;
    }

    CAmount Value() const
    {
        if (this->size() == 0)
            return 0;
        if (this->size() != 1)
            throw std::runtime_error("CColorAmount::Value(): The size of color amount should be 0 or 1.");
        return this->begin()->second;
    }

    CAmount TotalValue() const
    {
        CAmount value = 0;
        for (CColorAmount::const_iterator it(this->begin()); it != this->end(); it++)
            value += it->second;
        return value;
    }

    bool IsPositive() const
    {
        for (CColorAmount::const_iterator it(this->begin()); it != this->end(); it++)
            if (it->second < 0)
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const CColorAmount& ca);
};
CColorAmount const operator+(const CColorAmount &lhs, const CColorAmount &rhs);
CColorAmount const operator-(const CColorAmount &lhs, const CColorAmount &rhs);
CColorAmount const operator+(const CColorAmount &lhs, const CAmount &rhs);
CColorAmount const operator-(const CColorAmount &lhs, const CAmount &rhs);
CColorAmount const operator*(const CColorAmount &lhs, const CAmount &rhs);
CColorAmount const operator/(const CColorAmount &lhs, const CAmount &rhs);
CColorAmount const operator%(const CColorAmount &lhs, const CAmount &rhs);

/** Type-safe wrapper class to for fee rates
 * (how much to pay based on transaction size)
 */
class CFeeRate
{
private:
    CColorAmount mSatoshisPerK; // unit is satoshis-per-1,000-bytes
public:
    CFeeRate() : mSatoshisPerK(CColorAmount(1, 0)) { }
    explicit CFeeRate(const CColorAmount& _mSatoshisPerK): mSatoshisPerK(_mSatoshisPerK) { }
    explicit CFeeRate(const CAmount& _nSatoshisPerK): mSatoshisPerK(CColorAmount(1, _nSatoshisPerK)) { }
    CFeeRate(const CColorAmount& mFeePaid, size_t nSize);
    CFeeRate(const CAmount& nFeePaid, size_t nSize);
    CFeeRate(const CFeeRate& other) { mSatoshisPerK = other.mSatoshisPerK; }

    CColorAmount GetFee(size_t size) const; // unit returned is satoshis
    CColorAmount GetFeePerK() const { return GetFee(1000); } // satoshis-per-1000-bytes

    friend bool operator<(const CFeeRate& a, const CFeeRate& b) { return a.mSatoshisPerK < b.mSatoshisPerK; }
    friend bool operator>(const CFeeRate& a, const CFeeRate& b) { return a.mSatoshisPerK > b.mSatoshisPerK; }
    friend bool operator==(const CFeeRate& a, const CFeeRate& b) { return a.mSatoshisPerK == b.mSatoshisPerK; }
    friend bool operator<=(const CFeeRate& a, const CFeeRate& b) { return a.mSatoshisPerK <= b.mSatoshisPerK; }
    friend bool operator>=(const CFeeRate& a, const CFeeRate& b) { return a.mSatoshisPerK >= b.mSatoshisPerK; }
    std::string ToString() const;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        colorAmount_t mTemp = mSatoshisPerK;
        READWRITE(mTemp);
    }
};

#endif //  BITCOIN_AMOUNT_H

