#ifndef ALPHABET_H

    #define ALPHABET_H

    template <typename domain> struct Alphabet {

        virtual domain random_element() const = 0; /** Returns a random element of the alphabet */

    };

#endif
