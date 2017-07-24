#include <gtest/gtest.h>
#include <string>
#include "feerie/type/frtype.h"

using namespace feerie;
using namespace std;

TEST(FrType_Test, TestType_Constructor_GetName_GetHash)
{
    FrType<string> newType("newType");
    ASSERT_EQ(newType.getName(), "newType");
    ASSERT_EQ(newType.getHash(), 14959258511514873162ull);

    FrType<string> newType1("newType1");
    ASSERT_EQ(newType1.getName(), "newType1");
    ASSERT_EQ(newType1.getHash(), 14684671929671717627ull);
}
