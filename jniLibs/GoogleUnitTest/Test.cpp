#include <gtest/gtest.h>

TEST(FirstTest, First) { 
    ASSERT_EQ(6, 5);
    ASSERT_EQ(18.0, 5);
    ASSERT_EQ(25.4, 6);
    ASSERT_EQ(0, 7);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
Fatal assertion				Nonfatal assertion			Verifies
ASSERT_TRUE(condition);		EXPECT_TRUE(condition);		condition is true
ASSERT_FALSE(condition);	EXPECT_FALSE(condition);	condition is false


Fatal assertion			Nonfatal assertion		Verifies
ASSERT_EQ(val1,val2);	EXPECT_EQ(val1,val2);	val1 == val2
ASSERT_NE(val1,val2);	EXPECT_NE(val1,val2);	val1 != val2
ASSERT_LT(val1,val2);	EXPECT_LT(val1,val2);	val1 < val2
ASSERT_LE(val1,val2);	EXPECT_LE(val1,val2);	val1 <= val2
ASSERT_GT(val1,val2);	EXPECT_GT(val1,val2);	val1 > val2
ASSERT_GE(val1,val2);	EXPECT_GE(val1,val2);	val1 >= val2


Fatal assertion					Nonfatal assertion				Verifies
ASSERT_STREQ(str1,str2);		EXPECT_STREQ(str1,_str_2);		the two C strings have the same content
ASSERT_STRNE(str1,str2);		EXPECT_STRNE(str1,str2);		the two C strings have different content
ASSERT_STRCASEEQ(str1,str2);	EXPECT_STRCASEEQ(str1,str2);	the two C strings have the same content, ignoring case
ASSERT_STRCASENE(str1,str2);	EXPECT_STRCASENE(str1,str2);	the two C strings have different content, ignoring case




*/
