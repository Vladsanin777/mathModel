#include <iostream>
#include <string>
#include <cstring>

namespace lab3 {
    class string : public std::string {
    public:
        using std::string::string;
        using std::string::size;
        using std::string::at;

        size_t directSearch(const string& stringSought) const {
            size_t lengthSource{size()};
            size_t lengthSought{stringSought.size()};

            for (size_t i{0}, j{0}; i < lengthSource; i++) {
                if (0 == strncmp(c_str() + i,
                            stringSought.c_str(), lengthSought))
                    return i;
            }

            return -1;
        }

        size_t rabinKarpSearch(const string& stringSought) const {
            size_t countSymbols{256};
            size_t collisionsNumber{997};
            size_t lengthSource{length()};
            size_t lengthSought{stringSought.length()};
            size_t hashSought{0};
            size_t hashWindowSource{0};
            size_t highMultiplier{1};

            for (size_t i = 0; i < lengthSought - 1; i++)
                highMultiplier = (highMultiplier
                        * countSymbols) % collisionsNumber;

            for (size_t i = 0; i < lengthSought; i++) {
                hashSought = (countSymbols * hashSought
                        + stringSought.at(i)) % collisionsNumber;

                hashWindowSource = (countSymbols
                        * hashWindowSource + at(i)) % collisionsNumber;
            }

            for (size_t i = 0; i <= lengthSource - lengthSought; i++) {
                if (hashSought == hashWindowSource) {
                    if (0 == strncmp(c_str() + i,
                                stringSought.c_str(), lengthSought))
                        return i;
                }

                if (i < lengthSource - lengthSought) {
                    size_t charToRemove = (at(i) * highMultiplier) % collisionsNumber;

                    hashWindowSource = (hashWindowSource + collisionsNumber
                            - charToRemove) % collisionsNumber;

                    hashWindowSource = (hashWindowSource * countSymbols
                            + at(i + lengthSought)) % collisionsNumber;
                }
            }

            return -1;
        }
    };
}

int main(void) {
    lab3::string stringSource{};
    lab3::string stringSought{};

    std::cout << "Input string main: " << std::endl;
    std::getline(std::cin, stringSource);

    std::cout << "Input string sought: " << std::endl;
    std::getline(std::cin, stringSought);

    size_t indexDirectSearch{
        stringSource.directSearch(stringSought)
    };
    std::cout << "Index direct serch: " << indexDirectSearch << std::endl;

    size_t indexRabinKarpSearch{
        stringSource.rabinKarpSearch(stringSought)
    };
    std::cout << "Index Rabin-Karp serch: " << indexRabinKarpSearch << std::endl;

    return 0;
}
