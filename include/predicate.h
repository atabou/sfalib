#ifndef PREDICATE_H

    #define PREDICATE_H

    #include <algorithm>
    #include <limits>
    #include <vector>
    #include <iostream>

    struct Predicate {};

    template <typename domain> struct IntervalPredicate : public Predicate {

        std::vector<std::tuple<domain, bool, domain, bool>> intervals;


        IntervalPredicate() {}
       

        IntervalPredicate(domain lower_bound) {

            intervals.push_back({lower_bound, false, lower_bound, false});


        } 

        IntervalPredicate(domain lower_bound, domain upper_bound) {

            intervals.push_back({lower_bound, false, upper_bound, false});
            
        }
        
        IntervalPredicate(domain lower_bound, bool lower_bound_inclusive, domain upper_bound, bool upper_bound_inclusive) {

            intervals.push_back({lower_bound, lower_bound_inclusive, upper_bound, upper_bound_inclusive});

        }

        /**
         * @brief Construct a new Interval Predicate object from a vector of intervals.
         *
         * The intervals are given as a vector of tuples, where each tuple is of the form
         * (lower_bound, lower_bound_inclusive, upper_bound, upper_bound_inclusive).
         *
         * For example: 
         *      - The interval [0, 1] would be represented as (0, true, 1, true).
         *      - The interval (0, 1) would be represented as (0, false, 1, false).
         * 
         * @param intervals 
         *
         * @note The intervals are assumed to be disjoint.
         * @note The lower bound is assumed to be less than or equal to the upper bound.
         */
        IntervalPredicate(std::vector<std::tuple<domain, bool, domain, bool>> intervals) : intervals(intervals) {}
 

        IntervalPredicate<domain> normalizeIntervals() const {

            std::vector<std::tuple<domain, bool, domain, bool>> sorted_intervals = this->sortIntervals(intervals);
            std::vector<std::tuple<domain, bool, domain, bool>> normalized_intervals;

        for (int i=0; i<sorted_intervals.size(); i++) {

                domain lower_bound = std::get<0>(sorted_intervals[i]);
                domain upper_bound = std::get<2>(sorted_intervals[i]);
                bool lower_bound_is_open = std::get<1>(sorted_intervals[i]);
                bool upper_bound_is_open = std::get<3>(sorted_intervals[i]);
                
                for(int j=i+1; j<sorted_intervals.size(); j++) {

                    // If the lower bound of the next interval is greater than the upper bound of the current interval, then we can stop.
                    if(! (IntervalPredicate::lt(std::get<0>(sorted_intervals[j]), upper_bound)
                            || (IntervalPredicate::eq(std::get<0>(sorted_intervals[j]), upper_bound) && (!std::get<1>(sorted_intervals[j]) || !upper_bound_is_open)))) {
                        
                        break;

                    }

                    // If the upper bound of the next interval is greater than the upper bound of the current interval, then we update the upper bound.
                    // If the upper bound of the next interval is equal to the upper bound of the current interval, then we update the inclusivity of the upper bound.
                    if(lt(upper_bound, std::get<2>(sorted_intervals[j]))) {

                        upper_bound = std::get<2>(sorted_intervals[j]);
                        upper_bound_is_open = std::get<3>(sorted_intervals[j]);

                    } else if (IntervalPredicate::eq(upper_bound, std::get<2>(sorted_intervals[j]))) {

                        upper_bound_is_open = upper_bound_is_open && std::get<3>(sorted_intervals[j]);

                    }

                    // We move to the next interval.
                    i++;

                }

                // We create the new interval and add it to the results
                normalized_intervals.push_back({lower_bound, lower_bound_is_open, upper_bound, upper_bound_is_open});

            }

            return IntervalPredicate<domain>(normalized_intervals);

        }


        bool isSatisfiedBy(domain value) {

            std::vector<std::tuple<domain, bool, domain, bool>> sorted_intervals = this->sortIntervals(intervals);

            for (int i=0; i<sorted_intervals.size(); i++) {

                if (this->inInterval(value, sorted_intervals[i])) {

                    return true;

                }

            }

            return false;

        }


        friend std::ostream& operator<<(std::ostream& os, const IntervalPredicate<domain>& predicate) {

            std::vector<std::tuple<domain, bool, domain, bool>> sorted_intervals = predicate.sortIntervals(predicate.intervals);

            for (int i=0; i<sorted_intervals.size(); i++) {

                os << (std::get<1>(sorted_intervals[i]) ? "(" : "[");

                if(std::get<0>(sorted_intervals[i]) == std::numeric_limits<domain>::lowest()) {

                    // Print minus infinity sign
                    os << "-\u221E";

                } else {

                    os << std::get<0>(sorted_intervals[i]);

                }

                os << ", ";
                
                if(std::get<2>(sorted_intervals[i]) == std::numeric_limits<domain>::max()) {

                    // Print infinity sign
                    os << "\u221E";

                } else {

                    os << std::get<2>(sorted_intervals[i]);

                }

                os << (std::get<3>(sorted_intervals[i]) ? ")" : "]");

                if (i < sorted_intervals.size() - 1) {

                    os << " U ";

                }

            }

            return os;

        }


        static bool eq(domain v1, domain v2)  { return v1 == v2; }
        static bool lt(domain v1, domain v2)  { return v1 < v2;  }
        static bool gt(domain v1, domain v2)  { return v1 > v2;  }
        static bool leq(domain v1, domain v2) { return v1 <= v2; }
        static bool geq(domain v1, domain v2) { return v1 >= v2; }
        static bool neq(domain v1, domain v2) { return v1 != v2; }

        private:

            std::vector<std::tuple<domain, bool, domain, bool>> sortIntervals(std::vector<std::tuple<domain, bool, domain, bool>> intervals) const { 

                std::sort(intervals.begin(), intervals.end(), [](std::tuple<domain, bool, domain, bool> a, std::tuple<domain, bool, domain, bool> b) {

                    if (IntervalPredicate::eq(std::get<0>(a), std::get<0>(b)) && !std::get<1>(a) && std::get<1>(b)) {

                        return true;

                    } else if (IntervalPredicate::eq(std::get<0>(a), std::get<0>(b)) && std::get<1>(a) && !std::get<1>(b)) {

                        return false;

                    } else {

                        return IntervalPredicate::leq(std::get<0>(a), std::get<0>(b));

                    }


                });

                return intervals;

            }

            inline bool inInterval(domain value, std::tuple<domain, bool, domain, bool> interval) {

                return (IntervalPredicate::gt(value, std::get<0>(interval)) || (IntervalPredicate::eq(value, std::get<0>(interval)) && !std::get<1>(interval)))
                        && 
                       (IntervalPredicate::lt(value, std::get<2>(interval)) || (IntervalPredicate::eq(value, std::get<2>(interval)) && !std::get<3>(interval)));

            }

    };

    /*****************************************************************/
    /* Specializations for the different types of the domain         */
    /*****************************************************************/ 

    template <> inline bool IntervalPredicate<float>::eq(float v1, float v2)  { 
        
        return (std::abs(v1) ==  std::numeric_limits<float>::infinity() || std::abs(v2) == std::numeric_limits<float>::infinity()) ? v1 == v2 : std::abs(v1 - v2) < 1e-2; 

    }

    template <> inline bool IntervalPredicate<float>::lt(float v1, float v2)  { 

        return (std::abs(v1) ==  std::numeric_limits<float>::infinity() || std::abs(v2) == std::numeric_limits<float>::infinity()) ? v1 < v2 : v1 - v2 < -1e-2;

    }

    template <> inline bool IntervalPredicate<float>::gt(float v1, float v2)  { 

        return (std::abs(v1) ==  std::numeric_limits<float>::infinity() || std::abs(v2) == std::numeric_limits<float>::infinity()) ? v1 > v2 : v1 - v2 > 1e-2;

    }

    template <> inline bool IntervalPredicate<float>::leq(float v1, float v2) { 

        return eq(v1, v2) || lt(v1, v2); 

    }
    
    template <> inline bool IntervalPredicate<float>::geq(float v1, float v2) { 

        return eq(v1, v2) || gt(v1, v2); 

    }
    
    template <> inline bool IntervalPredicate<float>::neq(float v1, float v2) { 

        return !eq(v1, v2); 

    }

    template <> inline bool IntervalPredicate<double>::eq(double v1, double v2)  { 

        return (std::abs(v1) ==  std::numeric_limits<double>::infinity() || std::abs(v2) == std::numeric_limits<double>::infinity()) ? v1 == v2 : std::abs(v1 - v2) < 1e-2; 

    }
    
    template <> inline bool IntervalPredicate<double>::lt(double v1, double v2)  { 

        return (std::abs(v1) ==  std::numeric_limits<double>::infinity() || std::abs(v2) == std::numeric_limits<double>::infinity()) ? v1 < v2 : v1 - v2 < -1e-2; 

    }
    
    template <> inline bool IntervalPredicate<double>::gt(double v1, double v2)  { 

        return (std::abs(v1) ==  std::numeric_limits<double>::infinity() || std::abs(v2) == std::numeric_limits<double>::infinity()) ? v1 > v2 : v1 - v2 > 1e-2; 

    }
    
    template <> inline bool IntervalPredicate<double>::leq(double v1, double v2) { 

        return eq(v1, v2) || lt(v1, v2); 

    }

    template <> inline bool IntervalPredicate<double>::geq(double v1, double v2) { 

        return eq(v1, v2) || gt(v1, v2); 

    }
    
    template <> inline bool IntervalPredicate<double>::neq(double v1, double v2) { 

        return !eq(v1, v2); 

    } 


#endif // PREDICATE_H
