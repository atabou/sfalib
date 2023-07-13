#ifndef ORACLE_H

    #define ORACLE_H

    #include "sfa.h"
    #include "word.h"
#include <iostream>

    template <typename pred, typename domain> struct Oracle {

        virtual std::vector<int> membership(const std::vector<word<domain> >& words) = 0;
        virtual bool equivalence(sfa<pred, domain> hypothesis, word<domain>& ctrex) = 0;

    };

    template <typename pred, typename domain> struct TableOracle : public Oracle<pred, domain> {

        int i;
        std::vector<std::pair<word<domain>, int>> membership_table;
        std::vector<word<domain>> ctrexs;
        
        TableOracle(std::vector<std::pair<word<domain>, int>> membership_table, std::vector<word<domain>> ctrexs)
            : i(0)
            , membership_table(membership_table) 
            , ctrexs(ctrexs) { }

        std::vector<int> membership(const std::vector<word<domain> >& w) override { 
   
            std::vector<int> v; 

            for(size_t i = 0; i < w.size(); i++) {

                for(size_t j = 0; j < membership_table.size(); j++) {

                    if(w[i] == membership_table[j].first) {

                        v.push_back(membership_table[j].second);
                        break;

                    }

                }

            }

            return v;

        }

        bool equivalence(sfa<pred, domain> hypothesis, word<domain>& ctrex) override { 

            if(i < ctrexs.size()) {

                ctrex = ctrexs[i];
                i++;
                return false;

            } else {

                return true; 

            }

        }

    };


#endif
