#ifndef BOOLEAN_ALGEBRA_H

    #define BOOLEAN_ALGEBRA_H

    #include "predicate.h"
    #include <cassert>
#include <unordered_set>
    #include <vector>
    #include <map>
    
    template <typename pred, typename domain> struct BooleanAlgebra {

        virtual pred mkAtom(domain a) = 0;
        virtual pred mkNot(pred p) = 0;
        virtual pred mkOr(pred p, pred q) = 0;
        virtual pred mkOr(std::vector<pred> lst) = 0;
        virtual pred mkAnd(pred p, pred q) = 0;
        virtual pred mkAnd(std::vector<pred> lst) = 0;
        virtual pred mkTrue() = 0;
        virtual pred mkFalse() = 0;

        virtual bool areEquivalent(pred p, pred q) = 0;

        virtual bool isSatisfiable(pred p) = 0;

        virtual std::vector<pred> getSeparatingPredicates(std::vector<std::unordered_set<domain>> groups) = 0;

    };

    
    template <typename domain> struct IntervalAlgebra : public BooleanAlgebra<IntervalPredicate<domain>, domain> {


        domain lower_bound_limit;
        bool lower_bound_limit_is_open;
        domain upper_bound_limit;
        bool upper_bound_limit_is_open;


        IntervalAlgebra(domain lower_bound_limit=std::numeric_limits<domain>::min(), domain upper_bound_limit=std::numeric_limits<domain>::max()) {

            this->lower_bound_limit = lower_bound_limit;
            this->upper_bound_limit = upper_bound_limit;
            this->lower_bound_limit_is_open = false;
            this->upper_bound_limit_is_open = false;

        }

        IntervalAlgebra(domain lower_bound_limit, bool lb_open, domain upper_bound_limit, bool ub_open) {

            this->lower_bound_limit = lower_bound_limit;
            this->upper_bound_limit = upper_bound_limit;
            this->lower_bound_limit_is_open = lb_open;
            this->upper_bound_limit_is_open = ub_open;


        }

        IntervalPredicate<domain> mkAtom(domain a) override {
            
            bool valid_lower_bound = IntervalPredicate<domain>::lt(this->lower_bound_limit, a) || (IntervalPredicate<domain>::leq(this->lower_bound_limit, a) && !this->lower_bound_limit_is_open);
            bool valid_upper_bound = IntervalPredicate<domain>::gt(this->upper_bound_limit, a) || (IntervalPredicate<domain>::geq(this->upper_bound_limit, a) && !this->upper_bound_limit_is_open);

            if(!valid_lower_bound || !valid_upper_bound) {

                std::string s = "Cannot create atom with value outside of the domain. The domain is [" 
                        + std::to_string(this->lower_bound_limit) 
                        + ", " 
                        + std::to_string(this->upper_bound_limit) 
                        + "]. The atom is " 
                        + std::to_string(a)
                        + ".";

                throw std::invalid_argument(s);

            }

            return IntervalPredicate<domain>(a);

        }


        IntervalPredicate<domain> mkNot(IntervalPredicate<domain> p) override {

            IntervalPredicate<domain> normalized = p.normalizeIntervals();

            if(normalized.intervals.size() == 0) {

                return IntervalPredicate<domain>(lower_bound_limit, lower_bound_limit_is_open, upper_bound_limit, upper_bound_limit_is_open);

            }

            IntervalPredicate<domain> result;

            domain end_point = this->lower_bound_limit;
            bool end_point_is_open = this->lower_bound_limit_is_open;

            for (int i=0; i<normalized.intervals.size(); i++) {

                domain lower_bound = std::get<0>(normalized.intervals[i]);
                domain upper_bound = std::get<2>(normalized.intervals[i]);
                bool lower_bound_is_open = std::get<1>(normalized.intervals[i]);
                bool upper_bound_is_open = std::get<3>(normalized.intervals[i]);

                if (IntervalPredicate<domain>::gt(lower_bound, this->lower_bound_limit) || (IntervalPredicate<domain>::eq(lower_bound, this->lower_bound_limit) && !this->lower_bound_limit_is_open && lower_bound_is_open)) {
 
                    result.intervals.push_back({end_point, end_point_is_open, lower_bound, !lower_bound_is_open});

                    if (IntervalPredicate<domain>::eq(upper_bound, this->upper_bound_limit) && (this->upper_bound_limit_is_open || (!upper_bound_is_open && !this->upper_bound_limit_is_open))) {

                        break;

                    }

                }

                if(IntervalPredicate<domain>::lt(upper_bound, this->upper_bound_limit) || (IntervalPredicate<domain>::eq(upper_bound, this->upper_bound_limit) && !this->upper_bound_limit_is_open && upper_bound_is_open)) {
 
                    end_point = upper_bound;
                    end_point_is_open = !upper_bound_is_open;

                }

                if(i==normalized.intervals.size()-1 && 
                        (IntervalPredicate<domain>::lt(upper_bound, this->upper_bound_limit) 
                            || (IntervalPredicate<domain>::eq(upper_bound, this->upper_bound_limit) && !this->upper_bound_limit_is_open && upper_bound_is_open))) {

                    result.intervals.push_back({end_point, end_point_is_open, this->upper_bound_limit, this->upper_bound_limit_is_open});

                } 

            }
            
            return result;

        }


        IntervalPredicate<domain> mkOr(IntervalPredicate<domain> p, IntervalPredicate<domain> q) override {

            return this->mkNot(this->mkAnd(this->mkNot(p), this->mkNot(q)));

        }

        IntervalPredicate<domain> mkOr(std::vector<IntervalPredicate<domain>> lst) override {

            IntervalPredicate<domain> result = this->mkFalse();

            for (int i=0; i<lst.size(); i++) {

                result = this->mkOr(result, lst[i]);

            }

            return result;

        }

        IntervalPredicate<domain> mkAnd(IntervalPredicate<domain> p, IntervalPredicate<domain> q) override { 

            if (p.intervals.size() == 0 || q.intervals.size() == 0) {

                return this->mkFalse();

            }

            int i = 0;
            int j = 0;

            IntervalPredicate<domain> result;

            while (i < p.intervals.size() && j < q.intervals.size()) {

                domain p_lower_bound = std::get<0>(p.intervals[i]);
                domain p_upper_bound = std::get<2>(p.intervals[i]);
                bool p_lower_bound_is_open = std::get<1>(p.intervals[i]);
                bool p_upper_bound_is_open = std::get<3>(p.intervals[i]);

                domain q_lower_bound = std::get<0>(q.intervals[j]);
                domain q_upper_bound = std::get<2>(q.intervals[j]);
                bool q_lower_bound_is_open = std::get<1>(q.intervals[j]);
                bool q_upper_bound_is_open = std::get<3>(q.intervals[j]);

                domain lower_bound = this->lower_bound_limit; 
                bool lower_bound_is_open = this->lower_bound_limit_is_open;

                if (IntervalPredicate<domain>::neq(p_lower_bound, this->lower_bound_limit)) {
                    
                    if(IntervalPredicate<domain>::neq(q_lower_bound, this->lower_bound_limit)) {

                        if (IntervalPredicate<domain>::gt(p_lower_bound, q_lower_bound)) {

                            lower_bound = p_lower_bound;
                            lower_bound_is_open = p_lower_bound_is_open;

                        } else if (IntervalPredicate<domain>::lt(p_lower_bound, q_lower_bound)) {

                            lower_bound = q_lower_bound;
                            lower_bound_is_open = q_lower_bound_is_open;

                        } else {

                            lower_bound = p_lower_bound;
                            lower_bound_is_open = p_lower_bound_is_open || q_lower_bound_is_open;

                        }

                    } else {

                        lower_bound = p_lower_bound;
                        lower_bound_is_open = p_lower_bound_is_open;

                    }

                } else {

                    lower_bound = q_lower_bound;
                    lower_bound_is_open = q_lower_bound_is_open;

                }

                domain upper_bound = this->upper_bound_limit;
                bool upper_bound_is_open = this->upper_bound_limit_is_open;

                if (IntervalPredicate<domain>::neq(p_upper_bound, this->upper_bound_limit)) {

                    if (IntervalPredicate<domain>::neq(q_upper_bound, this->upper_bound_limit)) {

                        if (IntervalPredicate<domain>::gt(p_upper_bound, q_upper_bound)) {

                            upper_bound = q_upper_bound;
                            upper_bound_is_open = q_upper_bound_is_open;

                        } else if (IntervalPredicate<domain>::lt(p_upper_bound, q_upper_bound)) {

                            upper_bound = p_upper_bound;
                            upper_bound_is_open = p_upper_bound_is_open;

                        } else {

                            upper_bound = p_upper_bound;
                            upper_bound_is_open = p_upper_bound_is_open || q_upper_bound_is_open;

                        }

                    } else {

                        upper_bound = p_upper_bound;
                        upper_bound_is_open = p_upper_bound_is_open;

                    }

                } else {

                    upper_bound = q_upper_bound;
                    upper_bound_is_open = q_upper_bound_is_open;

                }

                if (IntervalPredicate<domain>::lt(lower_bound, upper_bound) 
                        || (IntervalPredicate<domain>::eq(lower_bound, upper_bound) && !lower_bound_is_open && !upper_bound_is_open)) {

                    result.intervals.push_back({lower_bound, lower_bound_is_open, upper_bound, upper_bound_is_open});

                }


                if (IntervalPredicate<domain>::eq(p_upper_bound, upper_bound)) {

                    i++;

                } else {

                    j++;

                }

            }

            return result;

        }

        IntervalPredicate<domain> mkAnd(std::vector<IntervalPredicate<domain>> lst) override {

            IntervalPredicate<domain> result = this->mkTrue();

            for (auto p : lst) {

                result = this->mkAnd(result, p);

            }

            return result;

        }


        IntervalPredicate<domain> mkTrue() override { 

            return IntervalPredicate<domain>(lower_bound_limit, lower_bound_limit_is_open, upper_bound_limit, upper_bound_limit_is_open);

        }
        

        IntervalPredicate<domain> mkFalse() override { 

            return IntervalPredicate<domain>();

        }


        bool areEquivalent(IntervalPredicate<domain> p, IntervalPredicate<domain> q) override {

            // p and q are equivalent if and only if p <=> q is a tautology
            bool b1 = this->isSatisfiable(this->mkAnd(p, this->mkNot(q)));
            bool b2 = this->isSatisfiable(this->mkAnd(this->mkNot(p), q));

            return !b1 && !b2;

        }


        bool isSatisfiable(IntervalPredicate<domain> p) override {

            return p.intervals.size() > 0;
        

        }


        /**
         * @brief Returns a list of disjoint predicates [p1, p2, ..., pn] such that p1 \/ p2 \/ ... \/ pn is true.
         * 
         * @param groups
         * @return std::vector<IntervalPredicate<domain>> 
         */
        std::vector<IntervalPredicate<domain>> getSeparatingPredicates(std::vector<std::unordered_set<domain>> groups) override {

            std::vector<IntervalPredicate<domain>> result;

            // if there is only one group, then the predicate is true.
            if(groups.size() <= 1) {

                result.push_back(this->mkTrue());
                return result;

            }


            bool empty = true;
            for (int i=0; i<groups.size(); i++) {

                if (groups[i].size() > 0) {

                    empty = false;
                    break;

                }

            }

            // if there is an empty group, then the predicate is false.
            if (empty) {

                result.push_back(this->mkTrue());

                for(int i=1; i<groups.size(); i++) {

                    result.push_back(this->mkFalse()); 

                }

                return result;

            }

            // maps each element to its group index

            std::map<domain, int> indices; // maps each element to its group index
            std::vector<domain> sorted_evidence;

            for (int i=0; i<groups.size(); i++) {

                result.push_back(this->mkFalse());

                for(auto e : groups[i]) sorted_evidence.push_back(e);
                
                for(auto e : groups[i]) indices[e] = i;

            }

            // sort the evidence

            std::sort(sorted_evidence.begin(), sorted_evidence.end());

            // create the predicate
            
            domain lower_bound = this->lower_bound_limit;
            domain upper_bound = this->upper_bound_limit;
            
            bool lower_bound_is_open = this->lower_bound_limit_is_open;
            
            for (int i=0; i<sorted_evidence.size()-1; i++) {

                domain e1 = sorted_evidence[i];
                domain e2 = sorted_evidence[i+1];

                if (IntervalPredicate<domain>::neq(indices[e1], indices[e2])) {

                    upper_bound = e2;
                    result[indices[e1]] = this->mkOr(result[indices[e1]], IntervalPredicate<domain>(lower_bound, lower_bound_is_open, upper_bound, true));
                    lower_bound = upper_bound;
                    lower_bound_is_open = false;

                }

            }

            result[indices[sorted_evidence[sorted_evidence.size()-1]]] = this->mkOr(
                result[indices[sorted_evidence[sorted_evidence.size()-1]]], 
                IntervalPredicate<domain>(lower_bound, false, this->upper_bound_limit, this->upper_bound_limit_is_open)
            );

            return result;

        }



    };

#endif 
