#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using::testing::AtLeast;
using::testing::Return;
using::testing::NiceMock;

class MockAccount : public Account {
public:
    MockAccount(int id, int balance):Account(id, balance){};
    MOCK_METHOD(void, Unlock, (), (override));
};

class MockTransaction: public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(Account, Balance) {
NiceMock<MockAccount> acc(1, 100);
EXPECT_EQ(acc.Account::GetBalance(), 100);
}
TEST(Account, ChangeBalance) {
NiceMock<MockAccount> acc(0, 100);
EXPECT_THROW(acc.Account::ChangeBalance(50), std::runtime_error);
acc.Account::Lock();
acc.Account::ChangeBalance(50);
EXPECT_EQ(acc.Account::GetBalance(), 150);
}

TEST(Account, Lock) {
NiceMock<MockAccount> acc(0, 100);
acc.Account::Lock();
EXPECT_THROW(acc.Account::Lock(), std::runtime_error);
}

TEST(Account, Unlock) {
NiceMock<MockAccount> acc(0, 100);
EXPECT_CALL(acc, Unlock()).Times(1);
acc.Unlock();
}

TEST(Transaction, SaveToDataBase) {
NiceMock<MockAccount> f_acc(0, 200);
NiceMock<MockAccount> t_acc(1, 300);
MockTransaction tr;
EXPECT_CALL(tr, SaveToDataBase(f_acc, t_acc, 150)).Times(1);
tr.SaveToDataBase(f_acc, t_acc, 150);
}
