#include <gtest/gtest.h>
#include <sys/unistd.h>

#include "boolean_algebra.h"
#include "obstable.h"
#include "oracle.h"
#include "predicate.h"


struct IntAlphabet : public Alphabet<int> {

    int random_element() const override { return 0; };

};

struct TrueOracle : public Oracle<IntervalPredicate<int>, int> {

    std::vector<int> membership(const std::vector<word<int> >& w) override { 

        std::vector<int> v;

        for(size_t i = 0; i < w.size(); i++) {

            v.push_back(1);

        }

        return v;

    }


    bool equivalence(sfa<IntervalPredicate<int>, int> hypothesis, word<int>& ctrex) override {

        return true;

    }

};

struct TrueFalseOracle : public Oracle<IntervalPredicate<int>, int> {

    size_t x = 0;

    std::vector<int> membership(const std::vector<word<int> >& w) override { 

        std::vector<int> v;

        for(size_t i = 0; i < w.size(); i++) {

            if(x % 2 == 0) v.push_back(0);
            else v.push_back(1);

            x++;

        }

        return v;

    }

    bool equivalence(sfa<IntervalPredicate<int>, int> hypothesis, word<int>& ctrex) override {

        return true;

    }

};


struct TableOracle : public Oracle<IntervalPredicate<int>, int> {

    int i=0;

    std::vector<int> membership(const std::vector<word<int> >& w) override { 

        std::vector<int> v;

        std::vector<std::pair<word<int>, int> > table = {

            // Epsilon
            {word<int>::epsilon()  , 1},
            {word<int>(51)         , 0},
            {word<int>({51,0})     , 0},
            {word<int>({51,21})    , 0},
            
            {word<int>(0)          , 1},
            {word<int>(101)        , 1},
            {word<int>({51,0,0})   , 1},
            {word<int>({51,21,0})  , 0},
            {word<int>({0,0})      , 1},
            {word<int>({51,0,0,0}) , 1},
            {word<int>({51,21,0,0}), 0},
            {word<int>({51,0,21})  , 0},

            // 0
            {word<int>(0)            , 1},
            {word<int>({51,0})       , 0},
            {word<int>({51,0,0})     , 1},
            {word<int>({51,21,0})    , 0},
            
            {word<int>({0,0})        , 1},
            {word<int>({101,0})      , 1},
            {word<int>({51,0,0,0})   , 1},
            {word<int>({51,21,0,0})  , 0},
            {word<int>({0,0,0})      , 1},
            {word<int>({51,0,0,0,0}) , 1},
            {word<int>({51,21,0,0,0}), 0},
            {word<int>({51,0,21,0})  , 0},

            // 0,0
            {word<int>({0,0})          , 1},
            {word<int>({51,0,0})       , 1},
            {word<int>({51,0,0,0})     , 1},
            {word<int>({51,21,0,0})    , 0},
            
            {word<int>({0,0,0})        , 1},
            {word<int>({101,0,0})      , 1},
            {word<int>({51,0,0,0,0})   , 1},
            {word<int>({51,21,0,0,0})  , 0},
            {word<int>({0,0,0,0})      , 1},
            {word<int>({51,0,0,0,0,0}) , 1},
            {word<int>({51,21,0,0,0,0}), 0},
            {word<int>({51,0,21,0,0})  , 0},


        };

        for(size_t i = 0; i < w.size(); i++) {

            for(size_t j = 0; j < table.size(); j++) {

                if(w[i] == table[j].first) {

                    v.push_back(table[j].second);
                    break;

                }

            }

        }

        return v;

    }

    bool equivalence(sfa<IntervalPredicate<int>, int> hypothesis, word<int>& ctrex) override {

        std::vector<word<int>> ctrexs = {

            word<int>(51),
            word<int>(101),
            word<int>({51,0,0}),
            word<int>({51,21,0}),
            word<int>({51,0,21})

        };

        if(i < 5) {

            ctrex = ctrexs[i];
            i++;
            return false;
        
        } else {

            return true; 

        }

    }

};


TEST(TestObsTable, BasicTest) {

    EXPECT_TRUE(true);

}


TEST(TestObsTable, Constructor) {

    IntAlphabet A;
    TrueOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    EXPECT_EQ(T.S.size(), 1);
    EXPECT_EQ(T.S[0], word<int>::epsilon());

    EXPECT_EQ(T.E.size(), 1);
    EXPECT_EQ(T.E[0], word<int>::epsilon());

    EXPECT_EQ(T.R.size(), 1);
    EXPECT_NE(T.R[0], word<int>::epsilon());

    EXPECT_EQ(T.St.size(), 1);
    EXPECT_EQ(T.St[0].size(), 1);

    EXPECT_EQ(T.Rt.size(), 1);
    EXPECT_EQ(T.Rt[0].size(), 1);

    EXPECT_EQ(T.unfilled_cols.size(), 1);
    EXPECT_EQ(T.unfilled_cols[0], 0);

    EXPECT_EQ(T.unfilled_rows.size(), 0);

}


TEST(TestObsTable, FillWithTrueOracle) {

    IntAlphabet A;
    TrueOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    T.fill_table();

    EXPECT_EQ(T.St[0][0] , 1);
    EXPECT_EQ(T.Rt[0][0] , 1);

    T.E.push_back(word<int>(1));
    T.E.push_back(word<int>(2));

    T.St[0].push_back(0);
    T.Rt[0].push_back(0);

    T.unfilled_cols.push_back(1);
    T.unfilled_cols.push_back(2);

    T.R.push_back(word<int>(1));
    T.R.push_back(word<int>(2));

    T.Rt.push_back(std::vector<int>(3));
    T.Rt.push_back(std::vector<int>(3));

    T.unfilled_rows.push_back(2);
    T.unfilled_rows.push_back(3);

    T.fill_table();

    EXPECT_EQ(T.St[0][0] , 1);
    EXPECT_EQ(T.St[0][1] , 1);
    EXPECT_EQ(T.St[0][2] , 1);

    EXPECT_EQ(T.Rt[0][0] , 1);
    EXPECT_EQ(T.Rt[0][1] , 1);
    EXPECT_EQ(T.Rt[0][2] , 1);

    EXPECT_EQ(T.Rt[1][0] , 1);
    EXPECT_EQ(T.Rt[1][1] , 1);
    EXPECT_EQ(T.Rt[1][2] , 1);

    EXPECT_EQ(T.Rt[2][0] , 1);
    EXPECT_EQ(T.Rt[2][1] , 1);
    EXPECT_EQ(T.Rt[2][2] , 1);

}

TEST(TestObsTable, FillWithTrueFalseOracle) {

    IntAlphabet A;
    TrueFalseOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    T.fill_table();

    EXPECT_EQ(T.St[0][0] , 0);
    EXPECT_EQ(T.Rt[0][0] , 1);

    T.E.push_back(word<int>(1));
    T.E.push_back(word<int>(2));

    T.St[0].push_back(0);
    T.Rt[0].push_back(0);

    T.unfilled_cols.push_back(1);
    T.unfilled_cols.push_back(2);

    T.R.push_back(word<int>(1));
    T.R.push_back(word<int>(2));

    T.Rt.push_back(std::vector<int>(3));
    T.Rt.push_back(std::vector<int>(3));

    T.unfilled_rows.push_back(2);
    T.unfilled_rows.push_back(3);

    T.fill_table();

    EXPECT_EQ(T.St[0][0] , 0);
    EXPECT_EQ(T.St[0][1] , 0);
    EXPECT_EQ(T.St[0][2] , 0);

    EXPECT_EQ(T.Rt[0][0] , 1);
    EXPECT_EQ(T.Rt[0][1] , 1);
    EXPECT_EQ(T.Rt[0][2] , 1);

    EXPECT_EQ(T.Rt[1][0] , 1);
    EXPECT_EQ(T.Rt[1][1] , 0);
    EXPECT_EQ(T.Rt[1][2] , 0);

    EXPECT_EQ(T.Rt[2][0] , 0);
    EXPECT_EQ(T.Rt[2][1] , 1);
    EXPECT_EQ(T.Rt[2][2] , 1);

}


TEST(TestObsTable, MehodClose) {

    IntAlphabet A;
    TrueOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);
 
    T.fill_table();

    EXPECT_EQ(T.St[0][0] , 1);
    EXPECT_EQ(T.Rt[0][0] , 1);

    T.R.push_back(word<int>(51));
    T.Rt.push_back(std::vector<int>(1));
    T.Rt[1][0] = 0;

    EXPECT_TRUE(T.close());

    EXPECT_EQ(T.S.size(), 2);
    EXPECT_EQ(T.S[1], word<int>(51));
    EXPECT_EQ(T.St[1][0], 0);

    EXPECT_EQ(T.R.size(), 2);
    EXPECT_EQ(T.R[0], word<int>(0));
    EXPECT_EQ(T.R[1], word<int>({51,0}));

    EXPECT_FALSE(T.close());

    T.Rt[1][0] = 0;

    T.E.push_back(word<int>(0));

    T.St[0].push_back(1);
    T.St[1].push_back(0);

    T.Rt[0].push_back(1);
    T.Rt[1].push_back(1);

    T.R.push_back(word<int>(101));
    T.Rt.push_back({1,1});

    T.R.push_back(word<int>({51,0,0}));
    T.Rt.push_back({1,1});

    EXPECT_TRUE(T.close());

    EXPECT_EQ(T.S.size(), 3);

    EXPECT_EQ(T.S[0], word<int>());
    EXPECT_EQ(T.S[1], word<int>(51));
    EXPECT_EQ(T.S[2].w, word<int>({51,0}).w);

    EXPECT_EQ(T.St[0], std::vector<int>({1,1}));
    EXPECT_EQ(T.St[1], std::vector<int>({0,0}));
    EXPECT_EQ(T.St[2], std::vector<int>({0,1}));

    EXPECT_EQ(T.R.size(), 3);

    EXPECT_EQ(T.R[0], word<int>(0));
    EXPECT_EQ(T.R[1], word<int>(101));
    EXPECT_EQ(T.R[2], word<int>({51,0,0}));

    EXPECT_EQ(T.Rt[0], std::vector<int>({1,1}));
    EXPECT_EQ(T.Rt[1], std::vector<int>({1,1}));
    EXPECT_EQ(T.Rt[2], std::vector<int>({1,1}));

    EXPECT_FALSE(T.close());

}


TEST(TestObsTable, MakeConsistent) {

    IntAlphabet A;
    TableOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    T.St[0][0] = 1;
    T.S.push_back(word<int>(51)); T.St.push_back(std::vector<int>({0}));

    T.Rt[0][0] = 1;
    T.R.push_back(word<int>({51,0})); T.Rt.push_back(std::vector<int>({0}));
    T.R.push_back(word<int>(101)); T.Rt.push_back(std::vector<int>({1}));
    T.R.push_back(word<int>({51,0,0})); T.Rt.push_back(std::vector<int>({1}));

    ASSERT_TRUE(T.make_consistent());

}


TEST(TestObsTable, GetAllWordsWithPrefix) {

    IntAlphabet A;
    TableOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>();
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    T.St[0][0] = 1;
    T.S.push_back(word<int>(51)); T.St.push_back(std::vector<int>({0}));

    T.Rt[0][0] = 1;
    T.R.push_back(word<int>({51,0})); T.Rt.push_back(std::vector<int>({0}));
    T.R.push_back(word<int>(101)); T.Rt.push_back(std::vector<int>({1}));
    T.R.push_back(word<int>({51,0,0})); T.Rt.push_back(std::vector<int>({1}));

    std::vector<int> words; 

    T.get_all_words_with_prefix(word<int>(51), words);

    EXPECT_EQ(words.size(), 3);
    EXPECT_EQ(words[0], 1);
    EXPECT_EQ(words[1], 3);
    EXPECT_EQ(words[2], 5);

}


TEST(TestObsTable, Learn) {


    IntAlphabet A;
    TableOracle O;
    IntervalAlgebra<int> I = IntervalAlgebra<int>(0, false, std::numeric_limits<int>::max(), true);
    ObsTable<IntervalPredicate<int>, int> T(&A, &O, &I);

    std::cout << T.learn() << std::endl;

}
