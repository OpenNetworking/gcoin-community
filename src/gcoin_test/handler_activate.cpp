// Copyright (c) 2015 The gCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

#include <stdint.h>

#include <map>
#include <string>
#include <iostream>
#include "policy/licenseinfo.h"

#include "test_util.h"

using namespace std;

BOOST_FIXTURE_TEST_SUITE(GlobalSetup, GlobalSetupFixture)

BOOST_FIXTURE_TEST_SUITE(CacheSetup, CacheSetupFixture)

struct ActivateHandlerFixture : public GlobalSetupFixture, public CacheSetupFixture
{
    ActivateHandlerFixture()
    {
        handler = type_transaction_handler::GetHandler(ACTIVATE);
    }
    ~ActivateHandlerFixture()
    {
        handler = NULL;
    }
};

struct ActivateHandlerCheckValidFixture : public ActivateHandlerFixture
{
    ActivateHandlerCheckValidFixture()
    {
        mint_main_hash = ArithToUint256(arith_uint256(1));
        activate_hash = ArithToUint256(arith_uint256(2));
        out_hash = ArithToUint256(arith_uint256(3));

        color = 0xE0000000;

        issuer = CreateAddress();
        member = CreateAddress();

        pinfo = new CLicenseInfo();
        pinfo->fMemberControl = true;
        plicense->SetOwner(color, issuer, pinfo);
        CreateTransaction(mint_main_hash, MINT);
        CreateTransaction(activate_hash, ACTIVATE);
        CreateTransaction(out_hash, ACTIVATE);
        ConnectTransactions(mint_main_hash, activate_hash, COIN, issuer, color);
        ConnectTransactions(activate_hash, out_hash, COIN, member, color);
    }

    ~ActivateHandlerCheckValidFixture()
    {
    }

    void CheckFalse(int ndos, const std::string &msg)
    {
        bool ret;
        int v;

        ret = handler->CheckValid(
                CTransaction(transactions[activate_hash]), state, NULL);
        ret &= handler->CheckFormat(
                CTransaction(transactions[activate_hash]), state, NULL);

        BOOST_CHECK_MESSAGE(
                ret == false && state.IsInvalid(v) && v == ndos, msg);
    }

    uint256 mint_main_hash, activate_hash, out_hash;
    std::string member, issuer;
    type_Color color;
    CValidationState state;
    CLicenseInfo *pinfo;
};


BOOST_FIXTURE_TEST_CASE(ActivateHandlerCheckValidPass, ActivateHandlerCheckValidFixture)
{
    BOOST_CHECK(handler->CheckValid(
                CTransaction(transactions[activate_hash]), state, NULL) == true);
}


BOOST_FIXTURE_TEST_CASE(ActivateHandlerSubColor, ActivateHandlerCheckValidFixture)
{
    color++;
    ConnectTransactions(mint_main_hash, activate_hash, COIN, issuer, color);
    ConnectTransactions(activate_hash, out_hash, COIN, member, color);
    CheckFalse(100, __func__);
}


BOOST_FIXTURE_TEST_CASE(ActivateHandlerNoColorOwner, ActivateHandlerCheckValidFixture)
{
    plicense->RemoveOwner(color);
    CheckFalse(100, __func__);
}


BOOST_FIXTURE_TEST_CASE(ActivateHandlerNotMemberOnly, ActivateHandlerCheckValidFixture)
{
    plicense->RemoveColor(color);
    pinfo->fMemberControl = false;
    plicense->SetOwner(color, issuer, pinfo);
    CheckFalse(30, __func__);
}


BOOST_FIXTURE_TEST_CASE(ActivateHandlerApply, ActivateHandlerCheckValidFixture)
{
    BOOST_CHECK(pactivate->IsActivated(color, member) == false);
    BOOST_CHECK(handler->Apply(CTransaction(transactions[activate_hash]), NULL));
    BOOST_CHECK(pactivate->IsActivated(color, member));
}


BOOST_FIXTURE_TEST_CASE(ActivateHandlerUndo, ActivateHandlerCheckValidFixture)
{
    pactivate->Activate(color, member);
    BOOST_CHECK(pactivate->IsActivated(color, member));
    BOOST_CHECK(handler->Undo(CTransaction(transactions[activate_hash]), NULL));
    BOOST_CHECK(pactivate->IsActivated(color, member) == false);
}


BOOST_AUTO_TEST_SUITE_END(); // CacheSetup

BOOST_AUTO_TEST_SUITE_END(); // GlobalSetup
