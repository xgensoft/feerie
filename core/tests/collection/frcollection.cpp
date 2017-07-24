#include <gtest/gtest.h>
#include <string>
#include <tuple>
#include "feerie/collection/frcollection.h"

using namespace feerie;
using namespace std;

class TestCollection1 : public FrCollection<unsigned int, string, string>
{
public:
    static const std::size_t NAME = 0;

    TestCollection1() : FrCollection<unsigned int, string, string>() {}
    TestCollection1(const string& defaultValue) : FrCollection<unsigned int, string, string>(defaultValue) {}

    bool checkSize(size_t size)
    {
        return (values.size() == size) && checkSize<0>(aliases, size);
    }

protected:
    template <std::size_t Offset, class Basic, class Alias>
    typename std::enable_if<Offset == 0, bool>::type
    checkSize(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases, size_t size)
    {
        return collectionAliases.values.size() == size;
    }
};

class TestCollection2 : public FrCollection<unsigned int, string, string, int>
{
public:
    static const std::size_t NAME = 0;
    static const std::size_t HASH = 1;

    TestCollection2() : FrCollection<unsigned int, string, string, int>() {}
    TestCollection2(const string& defaultValue) : FrCollection<unsigned int, string, string, int>(defaultValue) {}

    bool checkSize(size_t size)
    {
        return (values.size() == size) && checkSize<0>(aliases, size);
    }

protected:
    template <std::size_t Offset, class Basic, class... Aliases>
    bool checkSize(FrCollectionAliases<Offset, Basic, Aliases...>&, size_t)
    {
        return false;
    }

    template <std::size_t Offset, class Basic, class Alias>
    bool checkSize(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases, size_t size)
    {
        return collectionAliases.values.size() == size;
    }

    template <std::size_t Offset, class Basic, class Alias, class... Aliases>
    typename std::enable_if<(Offset < MaxOffset), bool>::type
    checkSize(FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases, size_t size)
    {
        if(collectionAliases.values.size() == size)
        {
            FrCollectionAliases<Offset + 1, Basic, Aliases...>& base = collectionAliases;
            return checkSize<Offset + 1>(base, size);
        }
        return false;
    }
};

class TestCollection5 : public FrCollection<unsigned int, string, int, string, unsigned int, wstring, char>
{
public:
    static const std::size_t ALIAS1 = 0;
    static const std::size_t ALIAS2 = 1;
    static const std::size_t ALIAS3 = 2;
    static const std::size_t ALIAS4 = 3;
    static const std::size_t ALIAS5 = 4;

    TestCollection5() : FrCollection<unsigned int, string, int, string, unsigned int, wstring, char>() {}
    TestCollection5(const string& defaultValue) : FrCollection<unsigned int, string, int, string, unsigned int, wstring, char>(defaultValue) {}

    bool checkSize(size_t size)
    {
        return (values.size() == size) && checkSize<0>(aliases, size);
    }

protected:
    template <std::size_t Offset, class Basic, class... Aliases>
    bool checkSize(FrCollectionAliases<Offset, Basic, Aliases...>&, size_t)
    {
        return false;
    }

    template <std::size_t Offset, class Basic, class Alias>
    bool checkSize(FrCollectionAliases<Offset, Basic, Alias>& collectionAliases, size_t size)
    {
        return collectionAliases.values.size() == size;
    }

    template <std::size_t Offset, class Basic, class Alias, class... Aliases>
    typename std::enable_if<(Offset < MaxOffset), bool>::type
    checkSize(FrCollectionAliases<Offset, Basic, Alias, Aliases...>& collectionAliases, size_t size)
    {
        if(collectionAliases.values.size() == size)
        {
            FrCollectionAliases<Offset + 1, Basic, Aliases...>& base = collectionAliases;
            return checkSize<Offset + 1>(base, size);
        }
        return false;
    }
};


TEST(FrCollection_Test, TestCollectionWithoutAliases_Constructor_DefaultValue)
{
    std::tuple<int> t1;
    std::get<0>(t1);
    FrCollection<unsigned int, string> testCollection0;
    ASSERT_EQ(testCollection0.getDefaultValue(), "");
    ASSERT_EQ(testCollection0.size(), 0);

    FrCollection<unsigned int, string> testCollection1("default");
    ASSERT_EQ(testCollection1.getDefaultValue(), "default");
    ASSERT_EQ(testCollection1.size(), 0);
}

TEST(FrCollection_Test, TestCollectionWithoutAliases_Add_Size_Clear)
{
    FrCollection<unsigned int, string> testCollection;
    ASSERT_TRUE(testCollection.add(1, ""));
    ASSERT_EQ(testCollection.size(), 1);
    ASSERT_TRUE(testCollection.add(2, ""));
    ASSERT_EQ(testCollection.size(), 2);
    ASSERT_TRUE(testCollection.add(3, ""));
    ASSERT_EQ(testCollection.size(), 3);
    ASSERT_FALSE(testCollection.add(3, ""));
    ASSERT_EQ(testCollection.size(), 3);

    testCollection.clear();
    EXPECT_EQ(testCollection.size(), 0);
}

TEST(FrCollection_Test, TestCollectionWithoutAliases_IndexOperator_Get_Exists)
{
    FrCollection<unsigned int, string> testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1"));
    ASSERT_TRUE(testCollection.add(2, "value2"));
    ASSERT_TRUE(testCollection.add(3, "value3"));

    EXPECT_EQ(testCollection[3], "value3");
    EXPECT_EQ(testCollection[2], "value2");
    EXPECT_EQ(testCollection[1], "value1");
    EXPECT_EQ(testCollection[4], "");

    string value;
    EXPECT_TRUE(testCollection.get(1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_TRUE(testCollection.get(2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get(3, value));
    EXPECT_EQ(value, "value3");
    EXPECT_FALSE(testCollection.get(4, value));
    EXPECT_EQ(value, "value3");

    EXPECT_TRUE(testCollection.exists(1));
    EXPECT_TRUE(testCollection.exists(2));
    EXPECT_TRUE(testCollection.exists(3));
    EXPECT_FALSE(testCollection.exists(4));
}

TEST(FrCollection_Test, TestCollectionWithoutAliases_Update_Remove)
{
    FrCollection<unsigned int, string> testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1"));
    ASSERT_TRUE(testCollection.add(2, "value2"));
    ASSERT_TRUE(testCollection.add(3, "value3"));

    EXPECT_TRUE(testCollection.update(2, "newValue2"));
    EXPECT_EQ(testCollection[2], "newValue2");

    EXPECT_TRUE(testCollection.update(3, "newValue3"));
    EXPECT_EQ(testCollection[3], "newValue3");

    EXPECT_EQ(testCollection[3], "newValue3");
    EXPECT_EQ(testCollection[2], "newValue2");
    EXPECT_EQ(testCollection[1], "value1");
    ASSERT_EQ(testCollection.size(), 3);

    const string* pointerValue1 = &testCollection[1];
    const string* pointerValue3 = &testCollection[3];

    EXPECT_FALSE(testCollection.remove(5));
    ASSERT_EQ(testCollection.size(), 3);

    EXPECT_TRUE(testCollection.remove(2));
    ASSERT_EQ(testCollection.size(), 2);
    EXPECT_EQ(&testCollection[1], pointerValue1);
    EXPECT_EQ(&testCollection[3], pointerValue3);

    EXPECT_TRUE(testCollection.remove(1));
    ASSERT_EQ(testCollection.size(), 1);
    EXPECT_EQ(&testCollection[3], pointerValue3);

    EXPECT_TRUE(testCollection.remove(3));
    ASSERT_EQ(testCollection.size(), 0);
}

// ====================================== One Alias

TEST(FrCollection_Test, TestCollectionWithAlias_Constructor_DefaultValue)
{
    TestCollection1 testCollection0;
    ASSERT_EQ(testCollection0.getDefaultValue(), "");
    ASSERT_TRUE(testCollection0.checkSize(0));

    TestCollection1 testCollection1("default");
    ASSERT_EQ(testCollection1.getDefaultValue(), "default");
    ASSERT_TRUE(testCollection0.checkSize(0));
}

TEST(FrCollection_Test, TestCollectionWithAlias_Add_Size_Clear)
{
    TestCollection1 testCollection;
    ASSERT_TRUE(testCollection.add(1, "", "1"));
    ASSERT_TRUE(testCollection.checkSize(1));
    ASSERT_TRUE(testCollection.add(2, "", "2"));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_TRUE(testCollection.add(3, "", "3"));
    ASSERT_TRUE(testCollection.checkSize(3));
    ASSERT_FALSE(testCollection.add(3, "", "4"));
    ASSERT_TRUE(testCollection.checkSize(3));
    ASSERT_FALSE(testCollection.add(4, "", "3"));
    ASSERT_TRUE(testCollection.checkSize(3));

    testCollection.clear();
    ASSERT_TRUE(testCollection.checkSize(0));
}

TEST(FrCollection_Test, TestCollectionWithAlias_Find_IndexOperator_Get_Exists)
{
    TestCollection1 testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1", "name1"));
    ASSERT_TRUE(testCollection.add(2, "value2", "name2"));
    ASSERT_TRUE(testCollection.add(3, "value3", "name3"));

    unsigned int key = 0;
    ASSERT_TRUE(testCollection.find<TestCollection1::NAME>("name1", key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection1::NAME>("name2", key));
    ASSERT_EQ(key, 2);
    ASSERT_TRUE(testCollection.find<TestCollection1::NAME>("name3", key));
    ASSERT_EQ(key, 3);

    EXPECT_EQ(testCollection[3], "value3");
    EXPECT_EQ(testCollection[2], "value2");
    EXPECT_EQ(testCollection[1], "value1");
    EXPECT_EQ(testCollection[4], "");

    EXPECT_EQ(testCollection.get<TestCollection1::NAME>("name3"), "value3");
    EXPECT_EQ(testCollection.get<TestCollection1::NAME>("name2"), "value2");
    EXPECT_EQ(testCollection.get<TestCollection1::NAME>("name1"), "value1");
    EXPECT_EQ(testCollection.get<TestCollection1::NAME>("name4"), "");

    string value;
    EXPECT_TRUE(testCollection.get(1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_TRUE(testCollection.get(2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get(3, value));
    EXPECT_EQ(value, "value3");
    EXPECT_FALSE(testCollection.get(4, value));
    EXPECT_EQ(value, "value3");

    EXPECT_TRUE(testCollection.get<TestCollection1::NAME>("name3", value));
    EXPECT_EQ(value, "value3");
    EXPECT_TRUE(testCollection.get<TestCollection1::NAME>("name2", value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection1::NAME>("name1", value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection1::NAME>("name0", value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.exists(1));
    EXPECT_TRUE(testCollection.exists(2));
    EXPECT_TRUE(testCollection.exists(3));
    EXPECT_FALSE(testCollection.exists(4));

    EXPECT_TRUE(testCollection.exists<TestCollection1::NAME>("name1"));
    EXPECT_TRUE(testCollection.exists<TestCollection1::NAME>("name2"));
    EXPECT_TRUE(testCollection.exists<TestCollection1::NAME>("name3"));
    EXPECT_FALSE(testCollection.exists<TestCollection1::NAME>("name4"));
}

TEST(FrCollection_Test, TestCollectionWithAlias_Update_Remove)
{
    TestCollection1 testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1", "name1"));
    ASSERT_TRUE(testCollection.add(2, "value2", "name2"));
    ASSERT_TRUE(testCollection.add(3, "value3", "name3"));

    EXPECT_TRUE(testCollection.update(2, "newValue2"));
    EXPECT_EQ(testCollection[2], "newValue2");

    EXPECT_TRUE(testCollection.update<TestCollection1::NAME>("name3", "newValue3"));
    EXPECT_EQ(testCollection[3], "newValue3");

    EXPECT_EQ(testCollection[3], "newValue3");
    EXPECT_EQ(testCollection[2], "newValue2");
    EXPECT_EQ(testCollection[1], "value1");
    ASSERT_EQ(testCollection.size(), 3);

    const string* pointerValue1 = &testCollection[1];
    const string* pointerValue3 = &testCollection[3];

    EXPECT_FALSE(testCollection.remove(5));
    ASSERT_EQ(testCollection.size(), 3);

    EXPECT_FALSE(testCollection.remove<TestCollection1::NAME>("name5"));
    ASSERT_EQ(testCollection.size(), 3);

    EXPECT_TRUE(testCollection.remove(2));
    ASSERT_EQ(testCollection.size(), 2);
    EXPECT_EQ(&testCollection[1], pointerValue1);
    EXPECT_EQ(&testCollection[3], pointerValue3);

    EXPECT_TRUE(testCollection.remove<TestCollection1::NAME>("name1"));
    ASSERT_EQ(testCollection.size(), 1);
    EXPECT_EQ(&testCollection[3], pointerValue3);

    EXPECT_TRUE(testCollection.remove(3));
    ASSERT_EQ(testCollection.size(), 0);
}

// ====================================== Aliases

TEST(FrCollection_Test, TestCollectionWithAliases_Constructor_DefaultValue)
{
    TestCollection2 testCollection0;
    ASSERT_EQ(testCollection0.getDefaultValue(), "");
    ASSERT_TRUE(testCollection0.checkSize(0));

    TestCollection2 testCollection1("default");
    ASSERT_EQ(testCollection1.getDefaultValue(), "default");
    ASSERT_TRUE(testCollection0.checkSize(0));
}

TEST(FrCollection_Test, TestCollectionWithAliases_Add_Size_Clear)
{
    TestCollection2 testCollection;
    ASSERT_TRUE(testCollection.add(1, "", "1", -1));
    ASSERT_TRUE(testCollection.checkSize(1));
    ASSERT_TRUE(testCollection.add(2, "", "2", -2));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_TRUE(testCollection.add(3, "", "3", -3));
    ASSERT_TRUE(testCollection.checkSize(3));
    ASSERT_FALSE(testCollection.add(3, "", "4", -4));
    ASSERT_TRUE(testCollection.checkSize(3));
    ASSERT_FALSE(testCollection.add(4, "", "3", -4));
    ASSERT_TRUE(testCollection.checkSize(3));
    ASSERT_FALSE(testCollection.add(4, "", "4", -3));
    ASSERT_TRUE(testCollection.checkSize(3));

    testCollection.clear();
    ASSERT_TRUE(testCollection.checkSize(0));
}

TEST(FrCollection_Test, TestCollectionWithAliases_Find_IndexOperator_Get_Exists)
{
    TestCollection2 testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1", "name1", -1));
    ASSERT_TRUE(testCollection.add(2, "value2", "name2", -2));
    ASSERT_TRUE(testCollection.add(3, "value3", "name3", -3));

    unsigned int key = 0;
    ASSERT_TRUE(testCollection.find<TestCollection2::NAME>("name1", key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection2::NAME>("name2", key));
    ASSERT_EQ(key, 2);
    ASSERT_TRUE(testCollection.find<TestCollection2::NAME>("name3", key));
    ASSERT_EQ(key, 3);

    ASSERT_TRUE(testCollection.find<TestCollection2::HASH>(-1, key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection2::HASH>(-2, key));
    ASSERT_EQ(key, 2);
    ASSERT_TRUE(testCollection.find<TestCollection2::HASH>(-3, key));
    ASSERT_EQ(key, 3);

    EXPECT_EQ(testCollection[3], "value3");
    EXPECT_EQ(testCollection[2], "value2");
    EXPECT_EQ(testCollection[1], "value1");
    EXPECT_EQ(testCollection[4], "");

    EXPECT_EQ(testCollection.get<TestCollection2::NAME>("name3"), "value3");
    EXPECT_EQ(testCollection.get<TestCollection2::NAME>("name2"), "value2");
    EXPECT_EQ(testCollection.get<TestCollection2::NAME>("name1"), "value1");
    EXPECT_EQ(testCollection.get<TestCollection2::NAME>("name4"), "");

    EXPECT_EQ(testCollection.get<TestCollection2::HASH>(-3), "value3");
    EXPECT_EQ(testCollection.get<TestCollection2::HASH>(-2), "value2");
    EXPECT_EQ(testCollection.get<TestCollection2::HASH>(-1), "value1");
    EXPECT_EQ(testCollection.get<TestCollection2::HASH>(0), "");

    string value;
    EXPECT_TRUE(testCollection.get(1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_TRUE(testCollection.get(2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get(3, value));
    EXPECT_EQ(value, "value3");
    EXPECT_FALSE(testCollection.get(4, value));
    EXPECT_EQ(value, "value3");

    EXPECT_TRUE(testCollection.get<TestCollection2::NAME>("name3", value));
    EXPECT_EQ(value, "value3");
    EXPECT_TRUE(testCollection.get<TestCollection2::NAME>("name2", value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection2::NAME>("name1", value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection2::NAME>("name0", value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.get<TestCollection2::HASH>(-1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_TRUE(testCollection.get<TestCollection2::HASH>(-2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection2::HASH>(-3, value));
    EXPECT_EQ(value, "value3");
    EXPECT_FALSE(testCollection.get<TestCollection2::HASH>(-4, value));
    EXPECT_EQ(value, "value3");

    EXPECT_TRUE(testCollection.exists(1));
    EXPECT_TRUE(testCollection.exists(2));
    EXPECT_TRUE(testCollection.exists(3));
    EXPECT_FALSE(testCollection.exists(4));

    EXPECT_TRUE(testCollection.exists<TestCollection2::NAME>("name1"));
    EXPECT_TRUE(testCollection.exists<TestCollection2::NAME>("name2"));
    EXPECT_TRUE(testCollection.exists<TestCollection2::NAME>("name3"));
    EXPECT_FALSE(testCollection.exists<TestCollection2::NAME>("name4"));

    EXPECT_TRUE(testCollection.exists<TestCollection2::HASH>(-1));
    EXPECT_TRUE(testCollection.exists<TestCollection2::HASH>(-2));
    EXPECT_TRUE(testCollection.exists<TestCollection2::HASH>(-3));
    EXPECT_FALSE(testCollection.exists<TestCollection2::HASH>(1));

}

// ====================================== Five Aliases

TEST(FrCollection_Test, TestCollectionWithFiveAliases_Constructor_DefaultValue)
{
    TestCollection5 testCollection0;
    ASSERT_EQ(testCollection0.getDefaultValue(), "");
    ASSERT_TRUE(testCollection0.checkSize(0));

    TestCollection5 testCollection1("default");
    ASSERT_EQ(testCollection1.getDefaultValue(), "default");
    ASSERT_TRUE(testCollection0.checkSize(0));
}

TEST(FrCollection_Test, TestCollectionWithFiveAliases_Add_Size_Clear)
{
    TestCollection5 testCollection;
    ASSERT_TRUE(testCollection.add(1, "", -10, "1", 10, L"10", -1));
    ASSERT_TRUE(testCollection.checkSize(1));
    ASSERT_TRUE(testCollection.add(2, "", -20, "2", 20, L"20", -2));
    ASSERT_TRUE(testCollection.checkSize(2));

    ASSERT_FALSE(testCollection.add(2, "", -30, "3", 30, L"30", -3));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_FALSE(testCollection.add(3, "", -10, "3", 30, L"30", -3));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_FALSE(testCollection.add(3, "", -30, "1", 30, L"30", -3));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_FALSE(testCollection.add(3, "", -30, "3", 10, L"30", -3));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_FALSE(testCollection.add(3, "", -30, "3", 30, L"20", -3));
    ASSERT_TRUE(testCollection.checkSize(2));
    ASSERT_FALSE(testCollection.add(3, "", -30, "3", 30, L"30", -2));
    ASSERT_TRUE(testCollection.checkSize(2));

    testCollection.clear();
    ASSERT_TRUE(testCollection.checkSize(0));
}


TEST(FrCollection_Test, TestCollectionWithFiveAliases_Find_IndexOperator_Get_Exists)
{
    TestCollection5 testCollection;
    ASSERT_TRUE(testCollection.add(1, "value1", -10, "1", 10, L"10", -1));
    ASSERT_TRUE(testCollection.checkSize(1));
    ASSERT_TRUE(testCollection.add(2, "value2", -20, "2", 20, L"20", -2));
    ASSERT_TRUE(testCollection.checkSize(2));

    unsigned int key = 0;
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS1>(-10, key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS1>(-20, key));
    ASSERT_EQ(key, 2);
    ASSERT_FALSE(testCollection.find<TestCollection5::ALIAS1>(-30, key));
    ASSERT_EQ(key, 2);

    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS2>("1", key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS2>("2", key));
    ASSERT_EQ(key, 2);
    ASSERT_FALSE(testCollection.find<TestCollection5::ALIAS2>("3", key));
    ASSERT_EQ(key, 2);

    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS3>(10, key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS3>(20, key));
    ASSERT_EQ(key, 2);
    ASSERT_FALSE(testCollection.find<TestCollection5::ALIAS3>(30, key));
    ASSERT_EQ(key, 2);

    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS4>(L"10", key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS4>(L"20", key));
    ASSERT_EQ(key, 2);
    ASSERT_FALSE(testCollection.find<TestCollection5::ALIAS4>(L"30", key));
    ASSERT_EQ(key, 2);

    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS5>(-1, key));
    ASSERT_EQ(key, 1);
    ASSERT_TRUE(testCollection.find<TestCollection5::ALIAS5>(-2, key));
    ASSERT_EQ(key, 2);
    ASSERT_FALSE(testCollection.find<TestCollection5::ALIAS5>(-3, key));
    ASSERT_EQ(key, 2);

    EXPECT_EQ(testCollection[2], "value2");
    EXPECT_EQ(testCollection[1], "value1");
    EXPECT_EQ(testCollection[4], "");

    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS1>(-20), "value2");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS1>(-10), "value1");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS1>(10), "");

    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS2>("1"), "value1");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS2>("2"), "value2");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS2>("vdf"), "");

    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS3>(10), "value1");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS3>(20), "value2");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS3>(100), "");

    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS4>(L"10"), "value1");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS4>(L"20"), "value2");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS4>(L"25"), "");

    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS5>(-1), "value1");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS5>(-2), "value2");
    EXPECT_EQ(testCollection.get<TestCollection5::ALIAS5>(0), "");

    string value;
    EXPECT_TRUE(testCollection.get(1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_TRUE(testCollection.get(2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_FALSE(testCollection.get(4, value));
    EXPECT_EQ(value, "value2");

    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS1>(-20, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS1>(-10, value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection5::ALIAS1>(0, value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS2>("2", value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS2>("1", value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection5::ALIAS2>("0", value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS3>(20, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS3>(10, value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection5::ALIAS3>(0, value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS4>(L"20", value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS4>(L"10", value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection5::ALIAS4>(L"0", value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS5>(-2, value));
    EXPECT_EQ(value, "value2");
    EXPECT_TRUE(testCollection.get<TestCollection5::ALIAS5>(-1, value));
    EXPECT_EQ(value, "value1");
    EXPECT_FALSE(testCollection.get<TestCollection5::ALIAS5>(0, value));
    EXPECT_EQ(value, "value1");

    EXPECT_TRUE(testCollection.exists(1));
    EXPECT_TRUE(testCollection.exists(2));
    EXPECT_FALSE(testCollection.exists(4));

    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS1>(-10));
    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS1>(-20));
    EXPECT_FALSE(testCollection.exists<TestCollection5::ALIAS1>(-50));

    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS2>("1"));
    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS2>("2"));
    EXPECT_FALSE(testCollection.exists<TestCollection5::ALIAS2>("name3"));

    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS3>(10));
    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS3>(20));
    EXPECT_FALSE(testCollection.exists<TestCollection5::ALIAS3>(30));

    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS4>(L"10"));
    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS4>(L"20"));
    EXPECT_FALSE(testCollection.exists<TestCollection5::ALIAS4>(L"30"));

    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS5>(-1));
    EXPECT_TRUE(testCollection.exists<TestCollection5::ALIAS5>(-2));
    EXPECT_FALSE(testCollection.exists<TestCollection5::ALIAS5>(-5));
}
