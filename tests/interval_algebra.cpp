#include <gtest/gtest.h>

#include "boolean_algebra.h"

TEST(IntervalAlgebraTest, BasicTest) {

    EXPECT_TRUE(true);

}

/*
TEST(IntervalAlgebraTest, SatisfiabilityTest) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>();

    IntervalPredicate<double> int_pred = IntervalPredicate<double>({{0.0, false, 1.0, true}});

    EXPECT_TRUE(ba.isSatisfiable(int_pred));
    EXPECT_TRUE(int_pred.isSatisfiedBy(0.5));
    EXPECT_TRUE(int_pred.isSatisfiedBy(0.0));
    EXPECT_FALSE(int_pred.isSatisfiedBy(1.0));

    IntervalPredicate<double> int_pred2 = IntervalPredicate<double>(-1);
    EXPECT_TRUE(ba.isSatisfiable(int_pred2));
    EXPECT_FALSE(int_pred2.isSatisfiedBy(0.0));

}
*/


TEST(IntervalAlgebraTest, TestMkAtom) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>();

    IntervalPredicate<double> int_pred = ba.mkAtom(1);

    EXPECT_TRUE(int_pred.intervals.size() == 1);

}


TEST(IntervalAlgebraTest, TestMkNot) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>(-std::numeric_limits<double>::infinity(), true, std::numeric_limits<double>::infinity(), true);

    IntervalPredicate<double> int_pred = ba.mkNot(ba.mkFalse());

    EXPECT_EQ(int_pred.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[0]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred.intervals[0]));

    IntervalPredicate<double> int_pred2 = ba.mkNot(ba.mkTrue());

    EXPECT_EQ(int_pred2.intervals.size(), 0);

    IntervalPredicate<double> int_pred3 = ba.mkNot(IntervalPredicate<double>({{0.0, false, 1.0, false}}));

    EXPECT_EQ(int_pred3.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred3.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred3.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred3.intervals[0]), 0);
    EXPECT_TRUE(std::get<3>(int_pred3.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred3.intervals[1]), 1);
    EXPECT_TRUE(std::get<1>(int_pred3.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred3.intervals[1]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred3.intervals[1]));

    IntervalPredicate<double> int_pred4 = ba.mkNot(IntervalPredicate<double>({{0.0, false, 1.0, true}}));

    EXPECT_EQ(int_pred4.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred4.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred4.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred4.intervals[0]), 0);
    EXPECT_TRUE(std::get<3>(int_pred4.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred4.intervals[1]), 1);
    EXPECT_FALSE(std::get<1>(int_pred4.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred4.intervals[1]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred4.intervals[1]));

    IntervalPredicate<double> int_pred5 = ba.mkNot(IntervalPredicate<double>({{0.0, true, 1.0, false}}));

    EXPECT_EQ(int_pred5.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred5.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred5.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred5.intervals[0]), 0);
    EXPECT_FALSE(std::get<3>(int_pred5.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred5.intervals[1]), 1);
    EXPECT_TRUE(std::get<1>(int_pred5.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred5.intervals[1]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred5.intervals[1]));

    IntervalPredicate<double> int_pred6 = ba.mkNot(IntervalPredicate<double>({{0.0, true, 1.0, true}}));

    EXPECT_EQ(int_pred6.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred6.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred6.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred6.intervals[0]), 0);
    EXPECT_FALSE(std::get<3>(int_pred6.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred6.intervals[1]), 1);
    EXPECT_FALSE(std::get<1>(int_pred6.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred6.intervals[1]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred6.intervals[1]));

    IntervalPredicate<double> int_pred7 = ba.mkNot(IntervalPredicate<double>({{0.0, false, 1.0, false}, {2.0, false, 3.0, false}}));

    EXPECT_EQ(int_pred7.intervals.size(), 3);
    EXPECT_EQ(std::get<0>(int_pred7.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred7.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred7.intervals[0]), 0);
    EXPECT_TRUE(std::get<3>(int_pred7.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred7.intervals[1]), 1);
    EXPECT_TRUE(std::get<1>(int_pred7.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred7.intervals[1]), 2);
    EXPECT_TRUE(std::get<3>(int_pred7.intervals[1]));
    EXPECT_EQ(std::get<0>(int_pred7.intervals[2]), 3);
    EXPECT_TRUE(std::get<1>(int_pred7.intervals[2]));
    EXPECT_EQ(std::get<2>(int_pred7.intervals[2]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred7.intervals[2]));

    IntervalPredicate<double> int_pred8 = ba.mkNot(IntervalPredicate<double>({{0.0, false, 2.5, false}, {2.0, false, 3.0, true}}));

    EXPECT_EQ(int_pred8.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred8.intervals[0]), -std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<1>(int_pred8.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred8.intervals[0]), 0);
    EXPECT_TRUE(std::get<3>(int_pred8.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred8.intervals[1]), 3);
    EXPECT_FALSE(std::get<1>(int_pred8.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred8.intervals[1]), std::numeric_limits<double>::infinity());
    EXPECT_TRUE(std::get<3>(int_pred8.intervals[1]));

    IntervalAlgebra<double> ba2 = IntervalAlgebra<double>(-2, false, 2, false);

    IntervalPredicate<double> int_pred9 = ba2.mkNot(IntervalPredicate<double>({{-1.0, false, 1.0, false}}));

    EXPECT_EQ(int_pred9.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred9.intervals[0]), -2);
    EXPECT_FALSE(std::get<1>(int_pred9.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred9.intervals[0]), -1);
    EXPECT_TRUE(std::get<3>(int_pred9.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred9.intervals[1]), 1);
    EXPECT_TRUE(std::get<1>(int_pred9.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred9.intervals[1]), 2);
    EXPECT_FALSE(std::get<3>(int_pred9.intervals[1]));

    IntervalPredicate<double> int_pred10 = ba2.mkNot(IntervalPredicate<double>({{-2.0, true, 2.0, true}}));

    EXPECT_EQ(int_pred10.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred10.intervals[0]), -2);
    EXPECT_FALSE(std::get<1>(int_pred10.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred10.intervals[0]), -2);
    EXPECT_FALSE(std::get<3>(int_pred10.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred10.intervals[1]), 2);
    EXPECT_FALSE(std::get<1>(int_pred10.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred10.intervals[1]), 2);
    EXPECT_FALSE(std::get<3>(int_pred10.intervals[1]));

    IntervalPredicate<double> int_pred11 = ba2.mkNot(IntervalPredicate<double>({{-2.0, true, 2.0, false}}));

    EXPECT_EQ(int_pred11.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred11.intervals[0]), -2);
    EXPECT_FALSE(std::get<1>(int_pred11.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred11.intervals[0]), -2);
    EXPECT_FALSE(std::get<3>(int_pred11.intervals[0]));

    IntervalPredicate<double> int_pred12 = ba2.mkNot(IntervalPredicate<double>({{-2.0, false, 2.0, true}}));

    EXPECT_EQ(int_pred12.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred12.intervals[0]), 2);
    EXPECT_FALSE(std::get<1>(int_pred12.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred12.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred12.intervals[0]));

    IntervalPredicate<double> int_pred13 = ba2.mkNot(IntervalPredicate<double>({{-2.0, false, 2.0, false}}));

    EXPECT_EQ(int_pred13.intervals.size(), 0);

}


TEST(IntervalAlgebraTest, TestMkAnd) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>(-std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity());

    IntervalPredicate<double> int_pred = ba.mkAnd(ba.mkAtom(1), ba.mkAtom(2));

    EXPECT_EQ(int_pred.intervals.size(), 0);

    IntervalPredicate<double> int_pred2 = ba.mkAnd(ba.mkAtom(1), ba.mkAtom(1));

    EXPECT_EQ(int_pred2.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred2.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred2.intervals[0]));

    IntervalPredicate<double> int_pred3 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 1.0, false}}), IntervalPredicate<double>({{1.0, false, 2.0, false}}));

    EXPECT_EQ(int_pred3.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred3.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred3.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred3.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred3.intervals[0]));

    IntervalPredicate<double> int_pred4 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 1.0, false}}), IntervalPredicate<double>({{1.0, true, 2.0, false}}));

    EXPECT_EQ(int_pred4.intervals.size(), 0);

    IntervalPredicate<double> int_pred5 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 1.0, true}}), IntervalPredicate<double>({{1.0, false, 2.0, false}}));

    EXPECT_EQ(int_pred5.intervals.size(), 0);

    IntervalPredicate<double> int_pred6 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 1.0, true}}), IntervalPredicate<double>({{1.0, true, 2.0, false}}));

    EXPECT_EQ(int_pred6.intervals.size(), 0);

    IntervalPredicate<double> int_pred7 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 1.0, true}}), IntervalPredicate<double>({{2.0, true, 4.0, true}}));

    EXPECT_EQ(int_pred7.intervals.size(), 0);

    IntervalPredicate<double> int_pred8 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 2.0, false}}), IntervalPredicate<double>({{1.0, false, 3.0, true}}));

    EXPECT_EQ(int_pred8.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred8.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred8.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred8.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred8.intervals[0]));
   
    IntervalPredicate<double> int_pred9 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 2.0, false}}), IntervalPredicate<double>({{1.0, true, 3.0, false}}));

    EXPECT_EQ(int_pred9.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred9.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(int_pred9.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred9.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred9.intervals[0]));

    IntervalPredicate<double> int_pred10 = ba.mkAnd(IntervalPredicate<double>({{0.0, false, 2.0, false}, {3, false, 4, false}}), IntervalPredicate<double>({{1.0, true, 3.0, true}, {3.5, false, 4.5, false}}));

    EXPECT_EQ(int_pred10.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred10.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(int_pred10.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred10.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred10.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred10.intervals[1]), 3.5);
    EXPECT_FALSE(std::get<1>(int_pred10.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred10.intervals[1]), 4);

    IntervalAlgebra<double> ba2 = IntervalAlgebra<double>(0, true, 10, true);

    IntervalPredicate<double> int_pred11 = ba2.mkAnd(IntervalPredicate<double>({{0.0, true, 2.0, false}, {3, false, 10, true}}), IntervalPredicate<double>({{1.0, true, 3.0, true}, {3.5, false, 10, true}}));

    EXPECT_EQ(int_pred11.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred11.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(int_pred11.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred11.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred11.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred11.intervals[1]), 3.5);
    EXPECT_FALSE(std::get<1>(int_pred11.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred11.intervals[1]), 10);
    EXPECT_TRUE(std::get<3>(int_pred11.intervals[1]));

    IntervalPredicate<double> int_pred12 = ba2.mkAnd(IntervalPredicate<double>({{0.0, true, 2.0, false}, {3, false, 7, true}}), IntervalPredicate<double>({{0.0, true, 3.0, true}, {3.5, false, 7, false}}));

    EXPECT_EQ(int_pred12.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred12.intervals[0]), 0);
    EXPECT_TRUE(std::get<1>(int_pred12.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred12.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(int_pred12.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred12.intervals[1]), 3.5);
    EXPECT_FALSE(std::get<1>(int_pred12.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred12.intervals[1]), 7);
    EXPECT_TRUE(std::get<3>(int_pred12.intervals[1]));

    IntervalAlgebra <double> ba3 = IntervalAlgebra<double>(0, false, 10, false);

    IntervalPredicate<double> int_pred13 = ba3.mkAnd(IntervalPredicate<double>(10.0, false, 10.0, false), IntervalPredicate<double>({{0.0, true, 3.0, true}, {3.5, false, 10, false}}));

    EXPECT_EQ(int_pred13.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred13.intervals[0]), 10.0);
    EXPECT_FALSE(std::get<1>(int_pred13.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred13.intervals[0]), 10.0);
    EXPECT_FALSE(std::get<3>(int_pred13.intervals[0]));

    IntervalPredicate<double> int_pred14 = ba3.mkAnd(IntervalPredicate<double>(10.0, false, 10.0, false), IntervalPredicate<double>(6.0, true, 10.0, true));

    EXPECT_EQ(int_pred14.intervals.size(), 0);

    IntervalPredicate<double> int_pred15 = ba3.mkAnd(IntervalPredicate<double>(2.0, false, 10.0, false), IntervalPredicate<double>(6.0, true, 7.0, false));

    EXPECT_EQ(int_pred15.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred15.intervals[0]), 6.0);
    EXPECT_TRUE(std::get<1>(int_pred15.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred15.intervals[0]), 7.0);
    EXPECT_FALSE(std::get<3>(int_pred15.intervals[0]));

}



TEST(IntervalAlgebraTest, TestMkOr) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>(-std::numeric_limits<double>::infinity(), true, std::numeric_limits<double>::infinity(), true);

    IntervalPredicate<double> int_pred = ba.mkOr(ba.mkAtom(1), ba.mkAtom(2));

    EXPECT_EQ(int_pred.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(int_pred.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred.intervals[1]), 2);
    EXPECT_FALSE(std::get<1>(int_pred.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[1]), 2);
    EXPECT_FALSE(std::get<3>(int_pred.intervals[1]));

    IntervalPredicate<double> int_pred2 = ba.mkOr(ba.mkAtom(1), ba.mkAtom(1));
    
    EXPECT_EQ(int_pred2.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred2.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred2.intervals[0]));

    IntervalPredicate<double> int_pred15 = ba.mkOr(IntervalPredicate<double>(2.0, false, 10.0, false), IntervalPredicate<double>(6.0, true, 7.0, false));

    EXPECT_EQ(int_pred15.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred15.intervals[0]), 2.0);
    EXPECT_FALSE(std::get<1>(int_pred15.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred15.intervals[0]), 10.0);
    EXPECT_FALSE(std::get<3>(int_pred15.intervals[0]));

}


TEST(IntervalAlgebra, TestMkOrVector) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>(-std::numeric_limits<double>::infinity(), true, std::numeric_limits<double>::infinity(), true);

    std::vector<IntervalPredicate<double>> int_preds = {ba.mkAtom(1), ba.mkAtom(2), ba.mkAtom(3)};

    IntervalPredicate<double> int_pred = ba.mkOr(int_preds);

    EXPECT_EQ(int_pred.intervals.size(), 3);
    EXPECT_EQ(std::get<0>(int_pred.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred.intervals[0]));
    EXPECT_EQ(std::get<0>(int_pred.intervals[1]), 2);
    EXPECT_FALSE(std::get<1>(int_pred.intervals[1]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[1]), 2);
    EXPECT_FALSE(std::get<3>(int_pred.intervals[1]));
    EXPECT_EQ(std::get<0>(int_pred.intervals[2]), 3);
    EXPECT_FALSE(std::get<1>(int_pred.intervals[2]));
    EXPECT_EQ(std::get<2>(int_pred.intervals[2]), 3);
    EXPECT_FALSE(std::get<3>(int_pred.intervals[2]));

    std::vector<IntervalPredicate<double>> int_preds2 = {ba.mkAtom(1), ba.mkAtom(1), ba.mkAtom(1)};
    
    IntervalPredicate<double> int_pred2 = ba.mkOr(int_preds2);

    EXPECT_EQ(int_pred2.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred2.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred2.intervals[0]));

}


TEST(IntervalAlgebra, TestMkAndVector) {

    IntervalAlgebra<double> ba = IntervalAlgebra<double>(-std::numeric_limits<double>::infinity(), true, std::numeric_limits<double>::infinity(), true);

    std::vector<IntervalPredicate<double>> int_preds = {ba.mkAtom(1), ba.mkAtom(2), ba.mkAtom(3)};

    IntervalPredicate<double> int_pred = ba.mkAnd(int_preds);

    EXPECT_EQ(int_pred.intervals.size(), 0);

    std::vector<IntervalPredicate<double>> int_preds2 = {ba.mkAtom(1), ba.mkAtom(1), ba.mkAtom(1)};
    
    IntervalPredicate<double> int_pred2 = ba.mkAnd(int_preds2);

    EXPECT_EQ(int_pred2.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(int_pred2.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred2.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(int_pred2.intervals[0]));

    std::vector<IntervalPredicate<double>> int_preds3 = {IntervalPredicate<double>(2.0, false, 10.0, false), IntervalPredicate<double>(6.0, true, 7.0, false), IntervalPredicate<double>(6.5, false, 9.0, false)};

    IntervalPredicate<double> int_pred3 = ba.mkAnd(int_preds3);

    EXPECT_EQ(int_pred3.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(int_pred3.intervals[0]), 6.5);
    EXPECT_FALSE(std::get<1>(int_pred3.intervals[0]));
    EXPECT_EQ(std::get<2>(int_pred3.intervals[0]), 7.0);
    EXPECT_FALSE(std::get<3>(int_pred3.intervals[0]));

}


