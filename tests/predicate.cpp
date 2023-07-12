#include <gtest/gtest.h>

#include "predicate.h"

TEST(IntervalPredicateTest, BasicTest) {

    EXPECT_TRUE(true);

}


TEST(IntervalPredicateTest, TestConstructors) {

    IntervalPredicate<int> ip1;

    EXPECT_EQ(ip1.intervals.size(), 0);

    IntervalPredicate<int> ip2(1);

    EXPECT_EQ(ip2.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(ip2.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(ip2.intervals[0]));
    EXPECT_EQ(std::get<2>(ip2.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(ip2.intervals[0]));

    IntervalPredicate<int> ip3(1, 2);

    EXPECT_EQ(ip3.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(ip3.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(ip3.intervals[0]));
    EXPECT_EQ(std::get<2>(ip3.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(ip3.intervals[0]));

    IntervalPredicate<int> ip4(1, true, 2, false);

    EXPECT_EQ(ip4.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(ip4.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(ip4.intervals[0]));
    EXPECT_EQ(std::get<2>(ip4.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(ip4.intervals[0]));

    IntervalPredicate<int> ip5({{1, true, 2, false}, {3, false, 4, true}});

    EXPECT_EQ(ip5.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(ip5.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(ip5.intervals[0]));
    EXPECT_EQ(std::get<2>(ip5.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(ip5.intervals[0]));
    EXPECT_EQ(std::get<0>(ip5.intervals[1]), 3);
    EXPECT_FALSE(std::get<1>(ip5.intervals[1]));
    EXPECT_EQ(std::get<2>(ip5.intervals[1]), 4);
    EXPECT_TRUE(std::get<3>(ip5.intervals[1]));

}


TEST(IntPredicateTest, NormalizeMethod) {

    IntervalPredicate<int> result;

    IntervalPredicate<int> ip1;

    result = ip1.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 0);

    IntervalPredicate<int> ip2(1);

    result = ip2.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip3(1, 2);

    result = ip3.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip4(1, true, 2, false);

    result = ip4.normalizeIntervals();
    
    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip5({{1, true, 2, false}, {3, false, 4, true}});

    result = ip5.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 2);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));
    EXPECT_EQ(std::get<0>(result.intervals[1]), 3);
    EXPECT_FALSE(std::get<1>(result.intervals[1]));
    EXPECT_EQ(std::get<2>(result.intervals[1]), 4);
    EXPECT_TRUE(std::get<3>(result.intervals[1]));

    IntervalPredicate<int> ip6({{1, true, 2, false}, {2, false, 3, true}});

    result = ip6.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 3);
    EXPECT_TRUE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip7({{1, true, 2, false}, {2, false, 3, false}});

    result = ip7.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 3);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));
        
    IntervalPredicate<int> ip8({{1, false, 2, false}, {2, false, 3, false}});

    result = ip8.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 3);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip9({{1, false, 2, false}, {2, true, 3, false}});

    result = ip9.normalizeIntervals();
        
    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 3);
    EXPECT_FALSE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip10({{1, false, 2, true}, {2, false, 3, true}});

    result = ip10.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 3);
    EXPECT_TRUE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip11({{1, false, 2, true}, {2, true, 3, false}});

    result = ip11.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_FALSE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 2);
    EXPECT_TRUE(std::get<3>(result.intervals[0]));
    EXPECT_EQ(std::get<0>(result.intervals[1]), 2);
    EXPECT_TRUE(std::get<1>(result.intervals[1]));
    EXPECT_EQ(std::get<2>(result.intervals[1]), 3);
    EXPECT_FALSE(std::get<3>(result.intervals[1]));

    IntervalPredicate<int> ip12({{1, true, 3, true}, {2, true, 4, true}});

    result = ip12.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 4);
    EXPECT_TRUE(std::get<3>(result.intervals[0]));

    IntervalPredicate<int> ip13({{1, true, 1, true}, {2, true, 3, false}, {3, false, 4, true}});

    result = ip13.normalizeIntervals();

    EXPECT_EQ(result.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<1>(result.intervals[0]));
    EXPECT_EQ(std::get<2>(result.intervals[0]), 1);
    EXPECT_TRUE(std::get<3>(result.intervals[0]));
    EXPECT_EQ(std::get<0>(result.intervals[1]), 2);
    EXPECT_TRUE(std::get<1>(result.intervals[1]));
    EXPECT_EQ(std::get<2>(result.intervals[1]), 4);
    EXPECT_TRUE(std::get<3>(result.intervals[1]));


    IntervalPredicate<double> dresult;

    IntervalPredicate<double> ip14({{1.0, true, 2.0, false}, {2.0, false, 3.0, true}});

    dresult = ip14.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 3.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[0]));

    IntervalPredicate<double> ip15({{1.0, true, 2.0, false}, {2.0, false, 3.0, false}});
    
    dresult = ip15.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 3.0);
    EXPECT_FALSE(std::get<3>(dresult.intervals[0]));

    IntervalPredicate<double> ip16({{1.0, false, 2.0, false}, {2.0, false, 3.0, false}});

    dresult = ip16.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_FALSE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 3.0);
    EXPECT_FALSE(std::get<3>(dresult.intervals[0]));
    
    IntervalPredicate<double> ip17({{1.0, false, 2.0, false}, {2.0, true, 3.0, false}});

    dresult = ip17.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_FALSE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 3.0);
    EXPECT_FALSE(std::get<3>(dresult.intervals[0]));

    IntervalPredicate<double> ip18({{1.0, false, 2.0, true}, {2.0, false, 3.0, true}});

    dresult = ip18.normalizeIntervals();
    
    EXPECT_EQ(dresult.intervals.size(), 1);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_FALSE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 3.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[0]));

    IntervalPredicate<double> ip19({{1.0, true, 2.0, true}, {2.0, true, 4.0, true}});

    dresult = ip19.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 2.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[0]));
    EXPECT_EQ(std::get<0>(dresult.intervals[1]), 2.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[1]));
    EXPECT_EQ(std::get<2>(dresult.intervals[1]), 4.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[1]));

    IntervalPredicate<double> ip20({{1.0, true, 1.5, true}, {2.0, true, 3.0, false}, {3.0, false, 4.0, true}});

    dresult = ip20.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 1.5);
    EXPECT_TRUE(std::get<3>(dresult.intervals[0]));
    EXPECT_EQ(std::get<0>(dresult.intervals[1]), 2.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[1]));
    EXPECT_EQ(std::get<2>(dresult.intervals[1]), 4.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[1]));

    IntervalPredicate<double> ip21({{3.0, false, 4.0, true}, {1.0, true, 1.5, true}, {2.0, true, 3.0, false}});

    dresult = ip21.normalizeIntervals();

    EXPECT_EQ(dresult.intervals.size(), 2);
    EXPECT_EQ(std::get<0>(dresult.intervals[0]), 1.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[0]));
    EXPECT_EQ(std::get<2>(dresult.intervals[0]), 1.5);
    EXPECT_TRUE(std::get<3>(dresult.intervals[0]));
    EXPECT_EQ(std::get<0>(dresult.intervals[1]), 2.0);
    EXPECT_TRUE(std::get<1>(dresult.intervals[1]));
    EXPECT_EQ(std::get<2>(dresult.intervals[1]), 4.0);
    EXPECT_TRUE(std::get<3>(dresult.intervals[1]));

}


TEST(IntervalPredicateTest, TestIsSatisfiedBy) {

    IntervalPredicate<double> ip1({{1.0, true, 2.0, true}});
    EXPECT_FALSE(ip1.isSatisfiedBy(1.0));
    EXPECT_TRUE(ip1.isSatisfiedBy(1.5));
    EXPECT_FALSE(ip1.isSatisfiedBy(2.0));
    EXPECT_FALSE(ip1.isSatisfiedBy(0.0));
    EXPECT_FALSE(ip1.isSatisfiedBy(3.0));

    IntervalPredicate<double> ip2({{1.0, true, 2.0, false}});
    EXPECT_FALSE(ip2.isSatisfiedBy(1.0));
    EXPECT_TRUE(ip2.isSatisfiedBy(1.5));
    EXPECT_TRUE(ip2.isSatisfiedBy(2.0));
    EXPECT_FALSE(ip2.isSatisfiedBy(0.0));
    EXPECT_FALSE(ip2.isSatisfiedBy(3.0));

    IntervalPredicate<double> ip3({{1.0, false, 2.0, true}});
    EXPECT_TRUE(ip3.isSatisfiedBy(1.0));
    EXPECT_TRUE(ip3.isSatisfiedBy(1.5));
    EXPECT_FALSE(ip3.isSatisfiedBy(2.0));
    EXPECT_FALSE(ip3.isSatisfiedBy(0.0));
    EXPECT_FALSE(ip3.isSatisfiedBy(3.0));

    IntervalPredicate<double> ip4({{1.0, false, 2.0, false}});
    EXPECT_TRUE(ip4.isSatisfiedBy(1.0));
    EXPECT_TRUE(ip4.isSatisfiedBy(1.5));
    EXPECT_TRUE(ip4.isSatisfiedBy(2.0));
    EXPECT_FALSE(ip4.isSatisfiedBy(0.0));
    EXPECT_FALSE(ip4.isSatisfiedBy(3.0));

    IntervalPredicate<double> ip5({{1.0, true, 2.0, true}, {2.0, true, 3.0, true}});
    EXPECT_FALSE(ip5.isSatisfiedBy(1.0));
    EXPECT_TRUE(ip5.isSatisfiedBy(1.5));
    EXPECT_FALSE(ip5.isSatisfiedBy(2.0));
    EXPECT_TRUE(ip5.isSatisfiedBy(2.5));
    EXPECT_FALSE(ip5.isSatisfiedBy(3.0));
    EXPECT_FALSE(ip5.isSatisfiedBy(0.0));
    EXPECT_FALSE(ip5.isSatisfiedBy(3.5));


}
