// Copyright (c) 2014-2016 The Gcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <stdint.h>

#include <map>
#include <string>
#include <iostream>

#include "test_util.h"
#include "policy/licenseinfo.h"

using namespace std;

BOOST_FIXTURE_TEST_SUITE(GlobalSetup, GlobalSetupFixture)

BOOST_FIXTURE_TEST_SUITE(CacheSetup, CacheSetupFixture)

struct NormalHandlerFixture : public GlobalSetupFixture, public CacheSetupFixture
{
    NormalHandlerFixture()
    {
        handler = type_transaction_handler::GetHandler(NORMAL);
    }
    ~NormalHandlerFixture()
    {
        handler = NULL;
    }
};

struct NormalHandlerCheckValidFixture : public NormalHandlerFixture
{
    NormalHandlerCheckValidFixture()
    {
        in_hash = ArithToUint256(arith_uint256(1));
        member_hash = ArithToUint256(arith_uint256(3));
        out_hash = ArithToUint256(arith_uint256(4));
        main_color = 0xE0000000;
        sub_color = 0xE0000001;
        pinfo = new CLicenseInfo();
        pinfo->fMemberControl = true;
        main_issuer = CreateAddress();
        sub_issuer = CreateAddress();
        plicense->SetOwner(main_color, main_issuer, pinfo);
        plicense->SetOwner(sub_color, sub_issuer, pinfo);

        member = CreateAddress();
        receiver = CreateAddress();
        pactivate->Activate(main_color, receiver);
        CreateTransaction(in_hash, MINT);
        CreateTransaction(member_hash, NORMAL);
        CreateTransaction(out_hash, NORMAL);
        ConnectTransactions(in_hash, member_hash, COIN, member, sub_color);
        ConnectTransactions(member_hash, out_hash, COIN, receiver, sub_color);
    }

    ~NormalHandlerCheckValidFixture()
    {
        delete pinfo;
    }

    void CheckFalse(int ndos, const std::string &msg)
    {
        CValidationState state;
        bool ret;
        int v;

        ret = handler->CheckValid(
                CTransaction(transactions[member_hash]), state, NULL);
        ret &= handler->CheckFormat(
                CTransaction(transactions[member_hash]), state, NULL);

        BOOST_CHECK_MESSAGE(
                ret == false && state.IsInvalid(v) && v == ndos, msg);
    }

    uint256 in_hash, member_hash, out_hash;
    std::string main_issuer, sub_issuer, member, receiver;
    type_Color main_color, sub_color;
    CValidationState state;
    CLicenseInfo *pinfo;
};


BOOST_FIXTURE_TEST_CASE(NormalHandlerCheckValidPass, NormalHandlerCheckValidFixture)
{
    BOOST_CHECK(handler->CheckValid(
                CTransaction(transactions[member_hash]), state, NULL) == true);
}


BOOST_FIXTURE_TEST_CASE(NormalHandlerInactivatedMember, NormalHandlerCheckValidFixture)
{
    pactivate->Deactivate(main_color, receiver);
    CheckFalse(10, __func__);
}


BOOST_FIXTURE_TEST_CASE(NormalHandlerNoColor, NormalHandlerCheckValidFixture)
{
    plicense->RemoveColor(sub_color);
    CheckFalse(100, __func__);
}


BOOST_AUTO_TEST_SUITE_END(); // CacheSetup

BOOST_AUTO_TEST_SUITE_END(); // GlobalSetup
