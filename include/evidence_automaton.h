#ifndef EVIDENCE_AUTOMATON_H

    #define EVIDENCE_AUTOMATON_H

    #include "word.h"
    #include <vector>
    #include <iostream>
    
    template <typename domain> struct EvidenceAutomaton {

        std::vector<word<domain> > state_names;
        std::vector<domain> alphabet;
        int initial_state;
        std::vector<int> final_states;
        std::vector<std::vector<int>> transitions;

        EvidenceAutomaton(std::vector<word<domain> > state_names, std::vector<domain> alphabet, int initial_state, std::vector<int> final_states, std::vector<std::vector<int> > transitions)
            : state_names(state_names)
            , alphabet(alphabet)
            , initial_state(initial_state)
            , final_states(final_states)
            , transitions(transitions) {}

        friend std::ostream& operator<<(std::ostream& os, const EvidenceAutomaton& automaton) {
           
            // Print the automaton as a graphviz dot string
            
            os << "digraph G {" << std::endl;
            os << "    rankdir=LR;" << std::endl;
            os << "    size=\"8,5\"" << std::endl;
            os << "    init [label=\"\", shape=none,height=.0,width=.0];" << std::endl;

            for(int i=0; i<automaton.state_names.size(); i++) {
                std::stringstream name;
                name << automaton.state_names[i].to_string(); 
                // Determine if automaton state is final.
                // If so, add a double circle around the state.
                // Otherwise, add a single circle.
                bool is_final = std::find(automaton.final_states.begin(), automaton.final_states.end(), i) != automaton.final_states.end();
                
                if(is_final) os << "    " << i << " [label=\"" << name.str() << "\", shape=doublecircle];" << std::endl;
                else os << "    " << i << " [label=\"" << name.str() << "\"];" << std::endl;
            }

            for(int i=0; i<automaton.transitions.size(); i++) {

                for(int j=0; j<automaton.transitions[i].size(); j++) {
                    // If the transition is -1, then there is no transition.
                    if(automaton.transitions[i][j] != -1) {
                        os << "    " << i << " -> " << automaton.transitions[i][j] << " [label=\"" << automaton.alphabet[j] << "\"];" << std::endl; 
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
