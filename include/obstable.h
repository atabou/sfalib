#ifndef OBSTABLE_H

    #define OBSTABLE_H

    #include <cassert>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <iostream>
    #include <algorithm>
    #include <unordered_map>
    #include <unordered_set>
    #include "boolean_algebra.h"
    #include "sfa.h"
    #include "word.h"
    #include "alphabet.h"
    #include "oracle.h"
    #include "evidence_automaton.h"


    template <typename pred, typename domain> struct ObsTable {

        const Alphabet<domain>* alpha;
        BooleanAlgebra<pred, domain>* ba;
        Oracle<pred, domain>* oracle;

        std::vector< word<domain> > S;   /** Prefixes in the alphabet */
        std::vector< word<domain> > R;   /** Suffixes in the alphabet */
        std::vector< word<domain> > E;   /** Suffixes in the alphabet */

        std::vector< std::vector<int> > St; /** Table of prefixes */
        std::vector< std::vector<int> > Rt; /** Table of boundary */

        std::vector<int> unfilled_rows; /** List of unfilled rows in Rt */
        std::vector<int> unfilled_cols; /** List of unfilled columns in Rt */


        ObsTable(const Alphabet<domain>* alpha, Oracle<pred, domain>* oracle, BooleanAlgebra<pred, domain>* ba) : alpha(alpha), oracle(oracle), ba(ba) {

            /******************************************************************
             * Initialize S and E.
             *****************************************************************/
            S.push_back(word<domain>::epsilon());
            R.push_back(word<domain>(alpha->random_element()));
            E.push_back(word<domain>::epsilon());

            /******************************************************************
             * Initialize the tables.
             *****************************************************************/
            St.push_back(std::vector<int>(1));
            Rt.push_back(std::vector<int>(1));

            /*****************************************************************
             * Set the added column as unfilled
             *****************************************************************/
            unfilled_cols.push_back(0);

        }


        sfa<pred, domain> learn() {

            sfa<pred, domain> conjecture;

            word<domain> ctrex;

            while(true) {

                /******************************************************************
                 * Initialize the table with an initial membership query.
                 *****************************************************************/

                fill_table();

                /******************************************************************
                 * Make the table consistent and closed.
                 *****************************************************************/

                bool inconsistent = true; 
                bool unclosed = true; 

                do {

                    if(inconsistent) {

                        inconsistent = make_consistent();

                        if(inconsistent) {

                            unclosed = true;

                        }

                    }

                    if(unclosed) {

                        unclosed = close();

                        if(unclosed) {

                            fill_table();

                            inconsistent = true;

                        }

                    }


                } while (inconsistent || unclosed);

                /******************************************************************
                 * Build a conjecture from the table.
                 *****************************************************************/

                conjecture = this->make_conjecture();

                /******************************************************************
                 * Check if the conjecture is correct.
                 *****************************************************************/

                if(oracle->equivalence(conjecture, ctrex)) {

                    break;

                }

                this->counter_example(ctrex);

            }

            return conjecture;

        }


        void counter_example(const word<domain>& w) {

            std::vector<word<domain> > prefixes;

            for(int i=1; i<=w.length(); i++) {

                word<domain> prefix;

                for(int j=0; j<i; j++) {

                    prefix = prefix.concat(w.w[j]);
                    

                }

                prefixes.push_back(prefix);

            }

            for(int i=0; i<prefixes.size(); i++) {

                bool inS = std::find(S.begin(), S.end(), prefixes[i]) != S.end(); 
                bool inR = std::find(R.begin(), R.end(), prefixes[i]) != R.end();

                if(!inS && !inR) {

                    R.push_back(prefixes[i]);
                    Rt.push_back(std::vector<int>(E.size()));
                    unfilled_rows.push_back(R.size() + S.size() - 1);

                }

            }

        }


        

        void fill_table() {

            std::vector<word<domain> > queries;

            for(int i=unfilled_rows.size() - 1; i >= 0; i--) {

                word<domain> w = (unfilled_rows[i] < S.size()) ? S[unfilled_rows[i]] : R[unfilled_rows[i]-S.size()];

                for(int j=0; j<E.size(); j++) {

                    word<domain> we = w.concat(E[j]);

                    queries.push_back(we);                    

                }
            
            }

            for(int i=unfilled_cols.size() - 1; i >= 0; i--) {

                for(int j=0; j<S.size()+R.size(); j++) {

                    word<domain> w = (j < S.size()) ? S[j] : R[j-S.size()];

                    word<domain> we = w.concat(E[unfilled_cols[i]]);

                    queries.push_back(we);

                }

            }

            // Perform the membership query to get the results.
            std::vector<int> results = oracle->membership(queries);

            // Update the table with the results.
            while(!unfilled_rows.empty()) {

                int row = unfilled_rows[unfilled_rows.size() - 1];

                // Get the row values that were queried.
                std::vector<int> row_values(results.begin(), results.begin() + E.size());

                // Save the extracted vector to the table.
                if(row < S.size()) {

                    St[row] = row_values;
                
                } else {
                
                    Rt[row - S.size()] = row_values;
                
                }

                // Erase the extracted values from the results vector.
                results.erase(results.begin(), results.begin() + E.size());

                // Update unfilled_rows to remove the update row. 
                unfilled_rows.pop_back();

            }

            while(!unfilled_cols.empty()) {

                int col = unfilled_cols[unfilled_cols.size() - 1];

                // Get the row values that were queried.
                std::vector<int> col_values(results.begin(), results.begin() + S.size() + R.size());

                // Save the extracted vector to the table.
                for(int i=0; i<S.size(); i++) St[i][col] = col_values[i];
                for(int i=0; i<R.size(); i++) Rt[i][col] = col_values[i+S.size()];

                // Erase the extracted values from the results vector.
                results.erase(results.begin(), results.begin() + S.size() + R.size());

                // Update unfilled_rows to remove the update row. 
                unfilled_cols.pop_back();

            }

        }



        void get_all_words_with_prefix(const word<domain>& prefix, std::vector<int>& words) {

            for(int i=0; i<S.size(); i++) {

                if(prefix.is_prefix_of(S[i])) {

                    words.push_back(i);

                }

            }

            for(int i=0; i<R.size(); i++) {

                if(prefix.is_prefix_of(R[i])) {

                    words.push_back(i+S.size());

                }

            }

        }



        void distribution(const word<domain>& ae) {

            std::vector<word<domain> > words_to_add;

            for(int i=0; i<S.size()+R.size(); i++) {

                for(int j=i+1; j<S.size()+R.size(); j++) {

                    /**********************************************************
                     * Get the words w1 and w2
                     *********************************************************/
                    word<domain> w1 = (i < S.size()) ? S[i] : R[i-S.size()];
                    word<domain> w2 = (j < S.size()) ? S[j] : R[j-S.size()];

                    /**********************************************************
                     * Get the row of w1 and w2 excluding ae.
                     *********************************************************/
                    std::vector<int> row_w1 = (i < S.size()) 
                        ? std::vector<int>(St[i].begin(), St[i].end()-1) 
                        : std::vector<int>(Rt[i-S.size()].begin(), Rt[i-S.size()].end()-1);

                    std::vector<int> row_w2 = (j < S.size())
                        ? std::vector<int>(St[j].begin(), St[j].end()-1)
                        : std::vector<int>(Rt[j-S.size()].begin(), Rt[j-S.size()].end()-1);

                    if(row_w1 == row_w2) {

                        int w1ae_value = (i < S.size()) ? St[i][E.size() - 1] : Rt[i-S.size()][E.size()-1];
                        int w2ae_value = (j < S.size()) ? St[j][E.size() - 1] : Rt[j-S.size()][E.size()-1];

                        if(w1ae_value != w2ae_value) {

                            /******************************************************************
                             * At this point, we know that:
                             * row(w1) = row(w2) for row(w1) and row(w2) excluding ae but that
                             * the ae column differentiates the states w1 and w2.
                             *****************************************************************/

                            for(int k=0; k<S.size()+R.size(); k++) {

                                word<domain> ub = (k < S.size()) ? S[k] : R[k-S.size()];

                                if(ub.length() == w1.length() + 1 && w1.is_prefix_of(ub)) {

                                    // b is a word of length 1
                                    word<domain> b = w1.getsuffix(ub);
                                    
                                    bool add_u2b = true;

                                    for(int l=0; l<S.size()+R.size(); l++) {

                                        word<domain> w = (l < S.size()) ? S[l] : R[l-S.size()];
                                        
                                        std::vector<int> row_w = (l < S.size()) ? St[l] : Rt[l-S.size()];

                                        std::vector<int> row_w2_full = (j < S.size()) ? St[j] : Rt[j-S.size()];

                                        if(row_w == row_w2_full) {

                                            word<domain> wb = w.concat(b);

                                            bool inS = std::find(S.begin(), S.end(), wb) != S.end(); 
                                            bool inR = std::find(R.begin(), R.end(), wb) != R.end();
                                            bool already_added = std::find(words_to_add.begin(), words_to_add.end(), wb) != words_to_add.end();

                                            if(inS || inR || already_added) {

                                                add_u2b = false;
                                                break;

                                            }

                                        }

                                    }

                                    word<domain> u2b = w2.concat(b);

                                    bool already_added = std::find(words_to_add.begin(), words_to_add.end(), u2b) != words_to_add.end();

                                    if(add_u2b && !already_added) {

                                        words_to_add.push_back(u2b);

                                    }
                                    

                                }

                                if(ub.length() == w2.length() + 1 && w2.is_prefix_of(ub)) {

                                    // b is a word of length 1
                                    word<domain> b = w2.getsuffix(ub);
                                    
                                    bool add_u1b = true;

                                    for(int l=0; l<S.size()+R.size(); l++) {

                                        word<domain> w = (l < S.size()) ? S[l] : R[l-S.size()];

                                        std::vector<int> row_w = (l < S.size()) ? St[l] : Rt[l-S.size()];

                                        std::vector<int> row_w1_full = (i < S.size()) ? St[i] : Rt[i-S.size()];
                                        
                                        if(row_w == row_w1_full) {

                                            word<domain> wb = w.concat(b);

                                            bool inS = std::find(S.begin(), S.end(), wb) != S.end(); 
                                            bool inR = std::find(R.begin(), R.end(), wb) != R.end();
                                            bool already_added = std::find(words_to_add.begin(), words_to_add.end(), wb) != words_to_add.end();

                                            if(inS || inR || already_added) {

                                                add_u1b = false;
                                                break;

                                            }

                                        }

                                    }

                                    word<domain> u1b = w1.concat(b);

                                    bool already_added = std::find(words_to_add.begin(), words_to_add.end(), u1b) != words_to_add.end();

                                    if(add_u1b && !already_added) {

                                        words_to_add.push_back(u1b);

                                    }               

                                }

                            }
                            
                        }
                    
                    } 

                }

            }

            for(int i=0; i<words_to_add.size(); i++) {

                word<domain> w = words_to_add[i];
                
                R.push_back(w);
                Rt.push_back(std::vector<int>(E.size()));
                unfilled_rows.push_back(R.size()-1);

            }


        }



        bool make_consistent() {

            for(int i=0; i<S.size()+R.size(); i++) {

                for(int j=i+1 ; j<S.size()+R.size(); j++) {

                    word<domain> w1 = (i < S.size()) ? S[i] : R[i-S.size()];
                    word<domain> w2 = (j < S.size()) ? S[j] : R[j-S.size()];

                    std::vector<int> row_w1 = (i < S.size()) ? St[i] : Rt[i-S.size()];
                    std::vector<int> row_w2 = (j < S.size()) ? St[j] : Rt[j-S.size()];

                    if(row_w1 == row_w2) {
                    
                        std::vector<int> w1a;
                        std::vector<int> w2a;

                        get_all_words_with_prefix(w1, w1a);
                        get_all_words_with_prefix(w2, w2a);

                        for(int k=0; k<w1a.size(); k++) {

                            for(int l=0; l<w2a.size(); l++) {

                                word<domain> a1 = (w1a[k] < S.size()) ? w1.getsuffix(S[w1a[k]]) : w1.getsuffix(R[w1a[k]-S.size()]);
                                word<domain> a2 = (w2a[l] < S.size()) ? w2.getsuffix(S[w2a[l]]) : w2.getsuffix(R[w2a[l]-S.size()]);

                                std::vector<int> row_w1xxx = (w1a[k] < S.size()) ? St[w1a[k]] : Rt[w1a[k]-S.size()];
                                std::vector<int> row_w2xxx = (w2a[l] < S.size()) ? St[w2a[l]] : Rt[w2a[l]-S.size()];

                                if(a1 == a2 && row_w1xxx != row_w2xxx) {


                                    /****************************************************************
                                     * At this point we know that we satisfy:
                                     *    row(w1) = row(w2) but row(w1a) != row(w2a)
                                     *
                                     * We can know handle the problematic suffix e and add a.e to E.
                                     ***************************************************************/

                                    word<domain> ae;

                                    for(int e=0; e<row_w1xxx.size(); e++) {

                                        if(row_w1xxx[e] != row_w2xxx[e]) {

                                            ae = a1.concat(E[e]);
                                            break;

                                        }

                                    }

                                    E.push_back(ae);

                                    /****************************************************************
                                     * We now add a column in St and Rt for the newly added suffix 
                                     * a.e. We also the columns to the vector of unfilled columns.
                                     ***************************************************************/
                                    
                                    for(int s=0; s<S.size(); s++) {

                                        St[s].push_back(0);

                                    }

                                    for(int r=0; r<R.size(); r++) {

                                        Rt[r].push_back(0);

                                    }

                                    unfilled_cols.push_back(E.size() - 1);
 
                                    /****************************************************************
                                     * We now need to handle evidence-closure:
                                     * forall s in S, forall e in E, s.e in S U R
                                     ***************************************************************/

                                    for(int s=0; s<S.size(); s++) {

                                        word<domain> sae = S[s].concat(ae);
                                        
                                        // Check if sae is in S or R
                                        bool inS = std::find(S.begin(), S.end(), sae) != S.end();
                                        bool inR = std::find(R.begin(), R.end(), sae) != R.end();

                                        // If sae is not in S or R, add it to R and add a new row to Rt.
                                        if(!inS && !inR) {

                                            R.push_back(sae);
                                            Rt.push_back(std::vector<int>(E.size()));
                                            unfilled_rows.push_back(Rt.size() + S.size() - 1);

                                        }

                                    }

                                    /****************************************************************
                                     * We now fill the table with the membership queries.
                                     * (necessary befor the distribution step)
                                     ***************************************************************/
                                    fill_table();


                                    /****************************************************************
                                     * Finally we handle distribution.
                                     ***************************************************************/ 
                                    distribution(ae);

                                    /****************************************************************
                                     * We now fill the newly added rows.
                                     ***************************************************************/
                                    fill_table();

                                    return true;

                                }
                                
                            }

                        }  

                    }

                }

            }

            return false;

        }



        /**********************************************************************
         * @brief Verifies if the table is closed
         * @return true if the table is closed, false otherwise
         * @note A table is said to be closed if: 
         *     - for every r in R, there exists s in S such that row(r)=row(s)
         *********************************************************************/
        bool isclosed(int& sa) {

            int smallest = -1;

            for(int r=0; r < R.size(); r++) {

                bool exists = false;

                for(int s=0; s<S.size(); s++) {

                    exists = exists || (Rt[r] == St[s]);

                }

                if(!exists && (smallest == -1 || R[r].length() < R[smallest].length())) {

                    smallest = r;

                };

            }

            sa = smallest;

            return (sa == -1) ? true : false;

        }


        /***********************************************************************
         * @brief Closes the table
         * @note A table is said to be closed if: 
         *     - for every r in R, there exists s in S such that row(r) = row(s)
         **********************************************************************/
        bool close() {

            int sa;

            if(!isclosed(sa)) {

                /**************************************************************
                 * Add s.a to the set of words S.
                 *************************************************************/
                S.push_back(R[sa]);

                /**************************************************************
                 * Add the s.a row to the S table.
                 *************************************************************/
                St.push_back(Rt[sa]);
                
                /**************************************************************
                 * Remove the s.a from the set of words R.
                 *************************************************************/
                R.erase(R.begin()+sa);

                /**************************************************************
                 * Remove the s.a row from the R table.
                 *************************************************************/
                Rt.erase(Rt.begin()+sa);

                /**************************************************************
                 * Handle evidence-closure.
                 *************************************************************/
                for(int e=0; e<E.size(); e++) {

                    word<domain> sae = S[S.size() - 1].concat(E[e]);
           
                    // Check if sae is in S or R
                    bool inS = std::find(S.begin(), S.end(), sae) != S.end();
                    bool inR = std::find(R.begin(), R.end(), sae) != R.end();

                    if(!inS && !inR) {
                        
                        R.push_back(sae);
                        Rt.push_back(std::vector<int>(E.size()));
                        unfilled_rows.push_back(Rt.size() + S.size() - 1);

                    }

                }

                /**************************************************************
                 * Ensure the continuation of sa by first verifying if a single
                 * char exists in E.
                 * 
                 * If it does then we do not add a new single character to E.
                 * 
                 * If it does not, then we get a random element from the 
                 * alphabet and create a new word sa.r from it.
                 *
                 * We add the new word to the set of words R, and associate
                 * it with a new row in the R table.
                 *************************************************************/

                bool cont = false;

                for(int i=0; i<S.size()+R.size(); i++) {

                    word<domain> w = (i < S.size()) ? S[i] : R[i-S.size()];

                    if(w.length() == S[S.size() - 1].length() + 1 && S[S.size() - 1].is_prefix_of(w)) {

                        cont = true;

                    }

                }

                if(!cont) {

                    R.push_back(S[S.size()-1].concat(alpha->random_element()));
                    Rt.push_back(std::vector<int>(E.size()));
                    unfilled_rows.push_back(Rt.size()-1);

                }

                return true;

            }

            return false;

        }


        sfa<pred, domain> make_conjecture() {

            // Create a map from the value of each row in St to some integer.
            // This will be used to create the transition table of the 
            // conjecture.

            std::unordered_map<std::vector<int>, int, vectorHash<int>> state_map;

            for(int s=0; s<S.size(); s++) {

                state_map[St[s]] = s;

            }

            // Create the transition table of the conjecture.

            std::vector<std::vector<std::unordered_set<domain> > > transition_table = std::vector<std::vector<std::unordered_set<domain> > >(state_map.size(), std::vector<std::unordered_set<domain> >(state_map.size(), std::unordered_set<domain>()));

            for(int i=0; i<S.size()+R.size(); i++) {

                word<domain> w = (i < S.size()) ? S[i] : R[i-S.size()];

                for(int j=0; j<S.size()+R.size(); j++) {

                    word<domain> wa = (j < S.size()) ? S[j] : R[j-S.size()];

                    if(wa.length() == w.length() + 1 && w.is_prefix_of(wa)) {

                        domain evidence = wa.w[wa.length() - 1];

                        int from = (i < S.size()) ? state_map[St[i]] : state_map[Rt[i-S.size()]];
                        int to = (j < S.size()) ? state_map[St[j]] : state_map[Rt[j-S.size()]];

                        transition_table[from][to].insert(evidence);

                    }

                }

            }

            // Generalizing the evidence into predicates.

            std::vector<std::tuple<int, pred, int> > sfa_transtions;

            for(int i=0; i<S.size(); i++) {

                std::vector<int> row = St[i];

                std::vector<std::unordered_set<domain> > groups;

                for(int j=0; j<S.size(); j++) {
                    
                    groups.push_back(transition_table[i][j]);

                }

                std::vector<pred> separating_predicates = ba->getSeparatingPredicates(groups);

                assert(separating_predicates.size() == S.size());

                for(int j=0 ; j < separating_predicates.size(); j++) {

                    sfa_transtions.push_back({i, separating_predicates[j], j});

                }

            }

            //Create the conjecture.

            std::vector<std::string> states;

            for(int i=0; i<S.size(); i++) {

                std::stringstream ss;

                for (int j=0; j<St[i].size(); j++) ss << St[i][j];

                states.push_back(ss.str());

            }
            
            int initial_state = 0;

            std::vector<int> final_states;

            for(int i=0; i<S.size(); i++) {

                if(St[i][0] == 1) {

                    final_states.push_back(i);

                }

            }

            sfa<pred, domain> conjecture = sfa<pred, domain>(states, this->ba, initial_state, final_states, sfa_transtions);

            return conjecture;


        }

    /*

        EvidenceAutomaton<domain> make_conjecture() {

            std::vector<word<domain> > state_names = S;
            int initial_state = 0;
            std::vector<domain> evidence_alphabet;

            // Get Final States.

            std::vector<int> final_state;

            for(int s=0; s<S.size(); s++) {

                if(St[s][0] == 1) {

                    final_state.push_back(s);
                    

                }

            }

            // Get Transition Table

            std::unordered_map<std::vector<int>, int, vectorHash<int>> state_map;

            for(int s=0; s<S.size(); s++) {

                state_map[St[s]] = s;

            }

            std::vector<std::vector<std::pair<domain, int> > > transition_map = std::vector<std::vector<std::pair<domain, int> > >(S.size(), std::vector<std::pair<domain, int> > ());

            for(int i=0; i<S.size() + R.size(); i++) {

                word<domain> w = (i < S.size()) ? S[i] : R[i-S.size()];

                for(int j=0; j<S.size() + R.size(); j++) {

                    word<domain> wa = (j < S.size()) ? S[j] : R[j-S.size()];

                    if(wa.length() == w.length() + 1 && w.is_prefix_of(wa)) {

                        std::vector<int> w_row = (i < S.size()) ? St[i] : Rt[i-S.size()];
                        std::vector<int> wa_row = (j < S.size()) ? St[j] : Rt[j-S.size()];

                        // Get the state the transition maps to.
                        int w_state = state_map[w_row];
                        int wa_state = state_map[wa_row];

                        // Add symbol to evidence alphabet if it is not already there, retrieve the position of the symbol in the alphabet.
                        
                        domain sym = wa.w[wa.length() - 1];
                        
                        int position;

                        auto sym_it = std::find(evidence_alphabet.begin(), evidence_alphabet.end(), sym);

                        bool in_evidence_alphabet = sym_it != evidence_alphabet.end();

                        if(!in_evidence_alphabet) {

                            evidence_alphabet.push_back(sym);

                            position = evidence_alphabet.size() - 1;

                        } else {

                            position = sym_it - evidence_alphabet.begin();

                        }

                        // Insert the transition into the transition map.

                        transition_map[w_state].push_back({position, wa_state});

                    }

                }

            }

            // Convert transition map to a transition table.

            std::vector<std::vector<int> > transition_table = std::vector<std::vector<int> >(S.size(), std::vector<int>(evidence_alphabet.size(), -1));

            for(int i=0; i<transition_map.size(); i++) {

                for(int j=0; j<transition_map[i].size(); j++) {

                    transition_table[i][transition_map[i][j].first] = transition_map[i][j].second;

                }

            }

            // Create Conjecture and return it.

            EvidenceAutomaton<domain> conjecture = EvidenceAutomaton<domain>(state_names, evidence_alphabet, initial_state, final_state, transition_table);

            return conjecture;

        }
    */


        friend std::ostream& operator<<(std::ostream& os, const ObsTable<pred, domain>& ot) {

            os << "S = {";
            for(size_t i=0; i<ot.S.size(); i++) {

                os << ot.S[i].to_string();
                if(i != ot.S.size()-1) os << ", ";

            }
            os << "}" << std::endl;

            os << "R = {";
            for(size_t i=0; i<ot.R.size(); i++) {

                os << ot.R[i].to_string();
                if(i != ot.R.size()-1) os << ", ";

            }
            os << "}" << std::endl;

            os << "E = {";
            for(size_t i=0; i<ot.E.size(); i++) {

                os << ot.E[i].to_string();
                if(i != ot.E.size()-1) os << ", ";

            }
            os << "}" << std::endl;

            os << "SURt = {" << std::endl;
            for(size_t i=0; i<ot.St.size(); i++) {

                for(size_t j=0; j<ot.E.size(); j++) {
                    os << "\t" << ot.St[i][j] << " ";
                }

                os<<std::endl;

            }

            os << "\t-";
            for(size_t i=0; i<(ot.E.size()-1)*8; i++) {
                os << "-";
            }
            os << "\n";

            for(size_t i=0; i<ot.Rt.size(); i++) {
                for(size_t j=0; j<ot.E.size(); j++) {
                    os << "\t" << ot.Rt[i][j] << " ";
                }
                os<<std::endl;
            }
            os << "}" << std::endl;

            return os;

        }

    }; 





#endif // OBSTABLE_H
