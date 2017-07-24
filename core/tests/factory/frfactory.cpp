#include <memory>
#include <gtest/gtest.h>
#include <string>
#include "feerie/factory/frfactory.h"

using namespace feerie;
using namespace std;

struct SomeObjectWithoutArgs
{
    bool createdFromFunctor = false;

    static SomeObjectWithoutArgs* creator()
    {
        return new SomeObjectWithoutArgs{true};
    }
};

struct SomeObjectWithOneArg
{
    string arg;
    bool createdFromFunctor = false;

    static shared_ptr<SomeObjectWithOneArg> creator(string arg)
    {
        auto tmp = std::make_shared<SomeObjectWithOneArg>(arg);
        tmp->createdFromFunctor = true;
        return tmp;
    }

    SomeObjectWithOneArg(string arg) : arg(arg) {}
};

struct SomeObjectWithTwoArgs
{
    string arg1;
    int arg2;
    bool createdFromFunctor = false;

    static shared_ptr<SomeObjectWithTwoArgs> creator(string arg1, int arg2)
    {
        auto tmp = std::make_shared<SomeObjectWithTwoArgs>(arg1, arg2);
        tmp->createdFromFunctor = true;
        return tmp;
    }

    SomeObjectWithTwoArgs(string arg1, int arg2) : arg1(arg1), arg2(arg2) {}
};

class BaseObject
{
public:
    BaseObject(const string& initArg, const string& name) :
        initArg(initArg),
        name(name)
    {

    }

    const string& getName() const
    {
        return name;
    }

    const string& getInitArg() const
    {
        return initArg;
    }

private:
    string initArg;
    string name;
};

class Object1 : public BaseObject
{
public:
    static bool registry(FrFactory<string>& factory)
    {
        return factory.add("Object1", Object1::creator);
    }

    Object1(string initArg) : BaseObject(initArg, "Object1")
    {

    }

private:
    static BaseObject* creator(string initArg)
    {
        return new Object1(initArg);
    }
};

class Object2 : public BaseObject
{
public:
    static bool registry(FrFactory<string>& factory)
    {
        return factory.add("Object2", Object2::creator);
    }

    Object2(string initArg) : BaseObject(initArg, "Object2")
    {

    }

private:
    static BaseObject* creator(string initArg)
    {
        return new Object2(initArg);
    }
};

TEST(FrFactory_Test, TestFactory_Constructor_Add_GetType_Create_Remove)
{
    FrFactory<string> factory;

    ASSERT_TRUE(factory.add("SomeObjectWithoutArgs", SomeObjectWithoutArgs::creator));
    ASSERT_TRUE(factory.add("SomeObjectWithOneArg", SomeObjectWithOneArg::creator));
    ASSERT_TRUE(factory.add("SomeObjectWithTwoArgs", SomeObjectWithTwoArgs::creator));

    FrType<string>* type0 = factory.getType("SomeObjectWithoutArgs");
    FrType<string>* type1 = factory.getType("SomeObjectWithOneArg");
    FrType<string>* type2 = factory.getType("SomeObjectWithTwoArgs");
    FrType<string>* type3 = factory.getType("SomeObjectWithTwo");

    ASSERT_NE(type0, nullptr);
    ASSERT_EQ(type0->getName(), "SomeObjectWithoutArgs");
    ASSERT_EQ(type0->getHash(), 11376826179110693849ull);

    ASSERT_NE(type1, nullptr);
    ASSERT_EQ(type1->getName(), "SomeObjectWithOneArg");
    ASSERT_EQ(type1->getHash(), 6171905725930884809ull);

    ASSERT_NE(type2, nullptr);
    ASSERT_EQ(type2->getName(), "SomeObjectWithTwoArgs");
    ASSERT_EQ(type2->getHash(), 14455131422779518634ull);

    ASSERT_EQ(type3, nullptr);

    auto object0 = factory.create<SomeObjectWithoutArgs*>(type0);
    ASSERT_NE(object0, nullptr);
    ASSERT_EQ(object0->createdFromFunctor, true);

    auto object1 = factory.create<shared_ptr<SomeObjectWithOneArg>>(type1->getHash(), string("arg"));
    ASSERT_NE(object1, nullptr);
    ASSERT_EQ(object1->createdFromFunctor, true);
    ASSERT_EQ(object1->arg, "arg");

    auto object2 = factory.create<shared_ptr<SomeObjectWithTwoArgs>>(type2->getName(), string("arg1"), 42);
    ASSERT_NE(object2, nullptr);
    ASSERT_EQ(object2->createdFromFunctor, true);
    ASSERT_EQ(object2->arg1, "arg1");
    ASSERT_EQ(object2->arg2, 42);

    auto object3 = factory.create<void*>("SomeObjectWithTwo");
    ASSERT_EQ(object3, nullptr);

    auto object4 = factory.create<void*>(24);
    ASSERT_EQ(object4, nullptr);

    auto object5 = factory.create<void*>(nullptr);
    ASSERT_EQ(object5, nullptr);

    FrType<string> customType("SomeObjectWithoutArgs");
    auto object6 = factory.create<SomeObjectWithoutArgs*>(customType);
    ASSERT_NE(object6, nullptr);
    ASSERT_EQ(object6->createdFromFunctor, true);

    size_t hash0 = type0->getHash();
    ASSERT_TRUE(factory.remove(hash0));
    ASSERT_EQ(factory.getType(hash0), nullptr);

    string name1 = type1->getName();
    ASSERT_TRUE(factory.remove(name1));
    ASSERT_EQ(factory.getType(name1), nullptr);

    ASSERT_TRUE(factory.remove(type2));
    ASSERT_EQ(factory.getType("SomeObjectWithTwoArgs"), nullptr);

    ASSERT_FALSE(factory.remove("SomeObjectWithTwo"));
    ASSERT_FALSE(factory.remove(nullptr));
}


TEST(FrFactory_Test, TestFactory_TypicalUse)
{
    FrFactory<string> factory;

    ASSERT_TRUE(Object1::registry(factory));
    ASSERT_TRUE(Object2::registry(factory));

    auto object1 = factory.create<BaseObject*>("Object1", string("myArg1"));
    ASSERT_NE(object1, nullptr);
    ASSERT_EQ(object1->getName(), "Object1");
    ASSERT_EQ(object1->getInitArg(), "myArg1");

    auto object2 = factory.create<BaseObject*>("Object2", string("myArg2"));
    ASSERT_NE(object2, nullptr);
    ASSERT_EQ(object2->getName(), "Object2");
    ASSERT_EQ(object2->getInitArg(), "myArg2");
}
