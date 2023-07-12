#include <gtest/gtest.h>

#include "word.h"

TEST(WordTest, BasicTest) {

    EXPECT_TRUE(true);

}


TEST(WordTest, DefaultConstructor) {

    word<int> w;

    EXPECT_TRUE(w.isEpsilon);
    EXPECT_EQ(w.w.size(), 0);

}

TEST(WordTest, SingleInputConstructor) {

    word<int> w(1);

    EXPECT_FALSE(w.isEpsilon);
    EXPECT_EQ(w.w.size(), 1);
    EXPECT_EQ(w.w[0], 1);

}

TEST(WordTest, VectorInputConstructor) {

    std::vector<int> v = {1,2,3,4,5};
    word<int> w(v);

    EXPECT_FALSE(w.isEpsilon);
    EXPECT_EQ(w.w.size(), 5);
    EXPECT_EQ(w.w[0], 1);
    EXPECT_EQ(w.w[1], 2);
    EXPECT_EQ(w.w[2], 3);
    EXPECT_EQ(w.w[3], 4);
    EXPECT_EQ(w.w[4], 5);

}

TEST(WordTest, EmptyVectorInputConstructor) {

    std::vector<int> v = {};
    word<int> w(v);

    EXPECT_TRUE(w.isEpsilon);
    EXPECT_EQ(w.w.size(), 0);

}


TEST(WordTest, StaticEpsilonCreateMethod) {

    word<int> w = word<int>::epsilon();

    EXPECT_TRUE(w.isEpsilon);
    EXPECT_EQ(w.w.size(), 0);

}

TEST(WordTest, MethodLength) {

    word<int> w = word<int>::epsilon();

    EXPECT_EQ(w.length(), 0);

    w = word<int>(1);

    EXPECT_EQ(w.length(), 1);

    w = word<int>({1,2,3,4,5});

    EXPECT_EQ(w.length(), 5);

}


TEST(WordTest, MethodIsPrefixOf) {

    word<int> w1 = word<int>::epsilon();
    word<int> w2 = word<int>::epsilon();

    EXPECT_TRUE(w1.is_prefix_of(w2));

    w1 = word<int>(1);
    w2 = word<int>::epsilon();

    EXPECT_FALSE(w1.is_prefix_of(w2));

    w1 = word<int>::epsilon();
    w2 = word<int>(1);

    EXPECT_TRUE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5});

    EXPECT_TRUE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6});

    EXPECT_TRUE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4});

    EXPECT_FALSE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,6});

    EXPECT_FALSE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,5,6});

    EXPECT_FALSE(w1.is_prefix_of(w2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,4,5,6});

    EXPECT_FALSE(w1.is_prefix_of(w2));

    w1 = word<int>({51, 0});
    w2 = word<int>();

    EXPECT_FALSE(w1.is_prefix_of(w2));

}


TEST(WordTest, OperatorEqual) {

    word<int> w1 = word<int>::epsilon();
    word<int> w2 = word<int>::epsilon();

    EXPECT_TRUE(w1 == w2);

    w1 = word<int>(1);
    w2 = word<int>::epsilon();

    EXPECT_FALSE(w1 == w2);

    w1 = word<int>::epsilon();
    w2 = word<int>(1);

    EXPECT_FALSE(w1 == w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5});

    EXPECT_TRUE(w1 == w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6});

    EXPECT_FALSE(w1 == w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4});

    EXPECT_FALSE(w1 == w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,6});

    EXPECT_FALSE(w1 == w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,5,6});

    EXPECT_FALSE(w1 == w2);

}


TEST(WordTest, OperatorNotEqual) {

    word<int> w1 = word<int>::epsilon();
    word<int> w2 = word<int>::epsilon();

    EXPECT_FALSE(w1 != w2);

    w1 = word<int>(1);
    w2 = word<int>::epsilon();

    EXPECT_TRUE(w1 != w2);

    w1 = word<int>::epsilon();
    w2 = word<int>(1);

    EXPECT_TRUE(w1 != w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5});

    EXPECT_FALSE(w1 != w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6});

    EXPECT_TRUE(w1 != w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4});

    EXPECT_TRUE(w1 != w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,6});

    EXPECT_TRUE(w1 != w2);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,5,6});

    EXPECT_TRUE(w1 != w2);

}


TEST(WordTest, GetSuffix) {

    word<int> w1 = word<int>::epsilon();
    word<int> w2 = word<int>::epsilon();

    EXPECT_EQ(w1.getsuffix(w2), w1);

    w1 = word<int>::epsilon();
    w2 = word<int>(1);

    EXPECT_EQ(w1.getsuffix(w2), w2);

    w1 = word<int>(1);
    w2 = word<int>({1,2});

    EXPECT_EQ(w1.getsuffix(w2), word<int>(2));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5});

    EXPECT_EQ(w1.getsuffix(w2), word<int>::epsilon());

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6});

    EXPECT_EQ(w1.getsuffix(w2), word<int>(6));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6,6,6});

    EXPECT_EQ(w1.getsuffix(w2), word<int>({6, 6, 6}));

    w1 = word<int>(1);
    w2 = word<int>({1,2,3,4,5,6,6,6});

    EXPECT_EQ(w1.getsuffix(w2), word<int>({2,3,4,5,6,6,6}));

}


TEST(WordTest, MethodConcat) {

    word<int> w1 = word<int>::epsilon();
    word<int> w2 = word<int>::epsilon();

    EXPECT_EQ(w1.concat(w2), w1);

    w1 = word<int>::epsilon();
    w2 = word<int>(1);

    EXPECT_EQ(w1.concat(w2), w2);

    w1 = word<int>(1);
    w2 = word<int>::epsilon();

    EXPECT_EQ(w1.concat(w2), w1);

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5});

    EXPECT_EQ(w1.concat(w2), word<int>({1,2,3,4,5,1,2,3,4,5}));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6});

    EXPECT_EQ(w1.concat(w2), word<int>({1,2,3,4,5,1,2,3,4,5,6}));

    w1 = word<int>({1,2,3,4,5});
    w2 = word<int>({1,2,3,4,5,6,6,6});

    EXPECT_EQ(w1.concat(w2), word<int>({1,2,3,4,5,1,2,3,4,5,6,6,6}));

    w1 = word<int>(1);
    w2 = word<int>({1,2,3,4,5,6,6,6});

    EXPECT_EQ(w1.concat(w2), word<int>({1,1,2,3,4,5,6,6,6}));

}


TEST(WordTest, MethodConcatWithSingleInputFromDomain) {

    word<int> w1 = word<int>::epsilon();
    int a = 1;

    EXPECT_EQ(w1.concat(a), word<int>(1));

    w1 = word<int>(1);
    a = 2;

    EXPECT_EQ(w1.concat(a), word<int>({1,2}));

    w1 = word<int>({1,2,3,4,5});
    a = 6;

    EXPECT_EQ(w1.concat(a), word<int>({1,2,3,4,5,6}));

}
