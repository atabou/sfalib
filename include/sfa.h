#ifndef SFA_H

    #define SFA_H

    #include "boolean_algebra.h"
    #include <vector>
    #include <string>

    template <typename pred, typename domain> struct sfa {

        std::vector<std::string> states;
        BooleanAlgebra<pred, domain>* ba;
        std::vector<pred> predicates;
        int initial_state;
        std::vector<int> final_states;
        std::vector<std::vector<int>> transitions;

        sfa() {}

        sfa(std::vector<std::string> states, BooleanAlgebra<pred, domain>* ba, std::vector<pred> predicates, int initial_state, std::vector<int> final_states, std::vector<std::vector<int>> transitions) 
            : states(states)
            , ba(ba)
            , predicates(predicates)
            , initial_state(initial_state)
            , final_states(final_states)
            , transitions(transitions) {}


        /**
         * @brief Construct a new sfa object from a vector of states, a Boolean algebra, an initial state, a vector of final states, and a vector of transitions.
         *
         * @param states The names of the states.
         * @param ba The Boolean algebra to use.
         * @param initial_state The index of the initial state in the transition vector supplied.
         * @param final_states The indices of the final states in the transition vector supplied.
         * @param transition The transitions of the sfa. Each transition is a tuple of the form (from_state, predicate, to_state).
         */
        sfa(std::vector<std::string> states, BooleanAlgebra<pred, domain>* ba, int initial_state, std::vector<int> final_states, std::vector<std::tuple<int, pred, int>> transition) 
            : states(states)
            , ba(ba)
            , initial_state(initial_state)
            , final_states(final_states) 
        {

            // Initialize the transition table.

            this->transitions = std::vector<std::vector<int>>(states.size(), std::vector<int>(transition.size(), -1));

            // Add the transitions and save all predicates.

            for(int i=0; i<transition.size(); i++) {

                if(this->ba->isSatisfiable(std::get<1>(transition[i]))) {

                    this->predicates.push_back(std::get<1>(transition[i]));

                    this->transitions[std::get<0>(transition[i])][this->predicates.size() - 1] = std::get<2>(transition[i]);

                }

            }

            // print predicests
            
            for(int i=0; i < predicates.size(); i++) {

                std::cout << i << " " << predicates[i] << std::endl;

            }

            // Print transition table.
            
            for(int i=0; i < transitions.size(); i++) {

                for(int j=0; j < predicates.size(); j++) {

                    std::cout << transitions[i][j] << " ";

                }

                std::cout << std::endl;

            }

        }


        friend std::ostream& operator<<(std::ostream& os, const sfa<pred, domain>& automaton) {

            // Print the automaton as a graphviz dot string
            
            os << "digraph G {" << std::endl;
            os << "    rankdir=LR;" << std::endl;
            os << "    size=\"8,5\"" << std::endl;
            os << "    init [label=\"\", shape=none,height=.0,width=.0];" << std::endl;

            for(int i=0; i<automaton.states.size(); i++) {
                // Determine if automaton state is final.
                // If so, add a double circle around the state.
                // Otherwise, add a single circle.
                bool is_final = std::find(automaton.final_states.begin(), automaton.final_states.end(), i) != automaton.final_states.end();
                
                if(is_final) os << "    " << i << " [label=\"" << automaton.states[i] << "\", shape=doublecircle];" << std::endl;
                else os << "    " << i << " [label=\"" << automaton.states[i] << "\"];" << std::endl;

            }

            for(int i=0; i<automaton.transitions.size(); i++) {

                for(int j=0; j<automaton.transitions[i].size(); j++) {
                    // If the transition is -1, then there is no transition.
                    if(automaton.transitions[i][j] != -1) {
                        os << "    " << i << " -> " << automaton.transitions[i][j] << " [label=\"" << automaton.predicates[j] << "\"];" << std::endl; 
                    }
                }

                // If the state is the initial state, add an arrow without a label and without a head.
                if(i == automaton.initial_state) os << "    init" << " -> " << i << ";" << std::endl;

            }

            os << "}" << std::endl;

            

            return os;



        }


        


    };



#endif
