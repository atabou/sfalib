#ifndef WORD_H

    #define WORD_H

    #include <iostream>
    #include <sstream>
    #include <vector>
    #include <functional>

    template <typename domain> struct word {

        bool isEpsilon;
        std::vector<domain> w;

        word() : isEpsilon(true) {};

        word(domain a) : isEpsilon(false) { w.push_back(a); };
        
        word(std::vector<domain> w) : w(w) {

            if(w.size() == 0) isEpsilon = true;
            else isEpsilon = false;

        };
        
        static word<domain> epsilon() { return word<domain>(); };

        int length() const;

        bool is_prefix_of(const word<domain>& other) const;

        bool operator==(const word<domain>& other) const;
        bool operator!=(const word<domain>& other) const;


        /**********************************************************************
         * @brief Returns the suffix of the supplied word given this word.
         * @param wxxx The word to get the prefix from.
         * @return The prefix of the word.
         * @note Checks for the validity of the prefix are not performed.
         * @note If this word is not a prefix of the supplied word, the 
         *      result is undefined.
         *********************************************************************/
        word getsuffix(word<domain> wxxx);

        word concat(const word<domain>& other) const;

        word concat(const domain& a) const;

        std::string to_string() const {

            std::stringstream s;

            if(isEpsilon) s << "epsilon";

            else {

                s << "(";

                for(size_t i=0; i<w.size(); i++) {

                    s << w[i];

                    if(i != w.size()-1) s << ", ";

                }

                s << ")";

            }

            return s.str();

        }

    };


    template <typename domain> int word<domain>::length() const {

        if(isEpsilon) return 0;

        return w.size(); 

    };


    template <typename domain> bool word<domain>::is_prefix_of(const word<domain>& other) const {

        // If this is epsilon, it is a prefix of everything
        if(isEpsilon) return true;

        // If this is not epsilon, check the length
        if(this->w.size() > other.w.size()) return false;

        // If this is not epsilon, check the vectors
        for(size_t i=0; i<this->w.size(); i++) {

            if(this->w[i] != other.w[i]) return false;

        }

        return true;

    }


    template <typename domain> bool word<domain>::operator==(const word<domain>& other) const {

        // If both are epsilon, they are equal
        if(isEpsilon && other.isEpsilon) return true;

        // If one is epsilon and the other is not, they are not equal
        if(isEpsilon || other.isEpsilon) return false;

        // If both are not epsilon, compare the vectors
        return this->w == other.w;

    }

    template <typename domain> bool word<domain>::operator!=(const word<domain>& other) const {

        return !(*this == other);

    }

    template <typename domain> word<domain> word<domain>::getsuffix(word<domain> wxxx) {

        return word<domain>(std::vector<domain>(wxxx.w.begin()+w.size(), wxxx.w.end()));

    }


    template <typename domain> word<domain> word<domain>::concat(const word<domain>& other) const {

    // If this is epsilon, return the other
        if(isEpsilon) return other;

        // If the other is epsilon, return this
        if(other.isEpsilon) return *this;

        // If both are not epsilon, concatenate the vectors
        std::vector<domain> w = this->w;
        w.insert(w.end(), other.w.begin(), other.w.end());

        return word<domain>(w);

    }


    template <typename domain> word<domain> word<domain>::concat(const domain& a) const {

        // If this is epsilon, return the word formed by a.
        if(isEpsilon) return word<domain>(a);

        // If this is not epsilon, concatenate the vector with a.
        std::vector<domain> w = this->w;
        w.push_back(a);

        return word<domain>(w);

    }

    template<typename T>
    struct vectorHash {
        std::size_t operator()(const std::vector<T>& vec) const {
            std::size_t seed = vec.size();
            for (const T& elem : vec) {
                seed ^= std::hash<T>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
    
    namespace std {
        template <typename domain>
        struct hash<word<domain> > {
            size_t operator()(const word<domain>& obj) const {
                // Implement your custom hash logic here
                vectorHash<domain> vh;
                return vh(obj.w);
            }
        };
    }

#endif
