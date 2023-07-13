#ifndef ORACLE_H

    #define ORACLE_H

    #include "sfa.h"
    #include "word.h"

    template <typename pred, typename domain> struct Oracle {

        virtual std::vector<int> membership(const std::vector<word<domain> >& words) = 0;
        virtual bool equivalence(sfa<pred, domain> hypothesis, word<domain>& ctrex) = 0;

    }; 

#endif
