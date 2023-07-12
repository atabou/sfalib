#ifndef ORACLE_H

    #define ORACLE_H

    #include "word.h"

    #include "evidence_automaton.h"

    template <typename domain> struct Oracle {

        virtual std::vector<int> membership(const std::vector<word<domain> >& words) = 0;
        /* virtual std::pair<word<domain>, int> equivalence(EvidenceAutomata<domain> hypothesis) = 0; */

    }; 

#endif
