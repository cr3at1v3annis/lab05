## Laboratory work V

Данная лабораторная работа посвещена изучению фреймворков для тестирования на примере **GTest**

```sh
$ open https://github.com/google/googletest
```

## Tasks

- [ ] 1. Создать публичный репозиторий с названием **lab05** на сервисе **GitHub**
- [ ] 2. Выполнить инструкцию учебного материала
- [ ] 3. Ознакомиться со ссылками учебного материала
- [ ] 4. Составить отчет и отправить ссылку личным сообщением в **Slack**


## Homework

### Задание
1. Создайте `CMakeList.txt` для библиотеки *banking*.

Код `banking/CMakeLists.txt` 

```sh
cmake_minimum_required(VERSION 3.4)
project(bank_lib)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(banking STATIC Account.cpp Account.h Transaction.cpp Transaction.h)
```
Код `CMakeLists.txt`

```sh
cmake_minimum_required(VERSION 3.4)
SET(COVERAGE OFF CACHE BOOL "Coverage")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")
project(lab)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/gtest" "gtest")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/banking)
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp)
if (COVERAGE)
    target_compile_options(tests PRIVATE --coverage)
    target_link_libraries(tests PRIVATE --coverage)
endif()
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/banking)
target_link_libraries(tests PRIVATE gtest gtest_main gmock_main banking)
```
2. Создайте модульные тесты на классы `Transaction` и `Account`.
    * Используйте mock-объекты.
    * Покрытие кода должно составлять 100%.  
  
Код `tests.cpp`

```sh
#include <iostream>
#include <Account.h>
#include <Transaction.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

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
```  
  
3. Настройте сборочную процедуру на **GitHub Actions**.
Код `cmake.yml`

```sh
name: Banking

on:
  push:
    branches: 
    - master

jobs:
  Build:
    
    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v2

    - name: Build banking
      run: |
        cd banking
        cmake -H. -B_build
        cmake --build _build
  
  Testing:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    - name: Update
      run: |
        sudo apt install git && git submodule update --init
        sudo apt install lcov
        sudo apt install g++-7
      
    - name: Testing
      run: |
        mkdir _build
        cd _build
        CXX=/usr/bin/g++-7 cmake -DCOVERAGE=1 ..
        cmake --build .
        ./tests
        lcov -t "banking" -o lcov.info -c -d .
        
    - name: Coveralls
      uses: coverallsapp/github-action@master
      with:
        github-token: ${{ secrets.github_token }}
        parallel: true
        path-to-lcov: ./_build/lcov.info
        coveralls-endpoint: https://coveralls.io
        
    - name: Coveralls Finish
      uses: coverallsapp/github-action@master
      with:
        github-token: ${{ secrets.github_token }}
        parallel-finished: true
```

4. Настройте [Coveralls.io](https://coveralls.io/).

## Links

- [C++ CI: Travis, CMake, GTest, Coveralls & Appveyor](http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/)
- [Boost.Tests](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/)
- [Catch](https://github.com/catchorg/Catch2)

```
Copyright (c) 2015-2021 The ISC Authors
```
