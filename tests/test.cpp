#include <iostream>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

//using::testing::AtLeast;
//using::testing::Return;
//using::testing::NiceMock;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance):Account(id, balance){};
    MOCK_METHOD(void, Unlock, ());
    MOCK_METHOD(void, Lock, ());
    MOCK_METHOD(int, id, (), (const));
    MOCK_METHOD(void, ChangeBalance, (int diff), ());
    MOCK_METHOD(int, GetBalance, (), ());
};

class MockTransaction: public Transaction {
public:
    MOCK_METHOD(bool, Make, (Account& from, Account& to, int sum), ());
    MOCK_METHOD(void, set_fee, (int fee), ());
    MOCK_METHOD(int, fee, (), ());
};

TEST(Account, Balance_ID_Change) {
    MockAccount acc(1, 100);
    EXPECT_CALL(acc, GetBalance()).Times(3);
    EXPECT_CALL(acc, Lock()).Times(1);
    EXPECT_CALL(acc, Unlock()).Times(1);
    EXPECT_CALL(acc, ChangeBalance(testing::_)).Times(2);
    EXPECT_CALL(acc, id()).Times(1);
    acc.GetBalance();
    acc.id();
    acc.Unlock();
    acc.ChangeBalance(1000);
    acc.GetBalance();
    acc.ChangeBalance(2);
    acc.GetBalance();
    acc.Lock();
    //EXPECT_EQ(acc.GetBalance(), 100);
}

TEST(Account, Balance_ID_Change_2) {
    Account acc(0, 100);
    EXPECT_THROW(acc.ChangeBalance(50), std::runtime_error);
    acc.Lock();
    acc.ChangeBalance(50);
    EXPECT_EQ(acc.GetBalance(), 150);
    EXPECT_THROW(acc.Lock(), std::runtime_error);
    acc.Unlock();
}

TEST(Transaction, TransTest) {
    MockTransaction trans;
    MockAccount first(1, 100);
    MockAccount second(2, 250);
    MockAccount flat_org(3, 10000);
    MockAccount org(4, 5000);
    EXPECT_CALL(trans, set_fee(testing::_)).Times(1);
    EXPECT_CALL(trans, fee()).Times(1);
    EXPECT_CALL(trans, Make(testing::_, testing::_, testing::_)).Times(2);
    EXPECT_CALL(first, GetBalance()).Times(1);
    EXPECT_CALL(second, GetBalance()).Times(1);
    trans.set_fee(300);
    trans.Make(first, second, 2000);
    trans.fee();
    first.GetBalance();
    second.GetBalance();
    trans.Make(org, first, 1000);
}
/*
TEST(Account, Unlock) {
NiceMock<MockAccount> acc(0, 100);
EXPECT_CALL(acc, Unlock()).Times(1);
acc.Unlock();
}
/*
TEST(Transaction, SaveToDataBase) {
NiceMock<MockAccount> f_acc(0, 200);
NiceMock<MockAccount> t_acc(1, 300);
MockTransaction tr;
EXPECT_CALL(tr, SaveToDataBase(f_acc, t_acc, 150)).Times(1);
tr.SaveToDataBase(f_acc, t_acc, 150);
}
*/