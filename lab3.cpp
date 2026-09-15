#include <iostream>
#include <string>
#include <cstring>

namespace lab3 {
    long long add(long long a, long long b, long long mod) {
        a %= mod; b %= mod;
        return (a + b) % mod;
    }

    long long sub(long long a, long long b, long long mod) {
        a %= mod; b %= mod;
        return (a - b + mod) % mod;
    }

    long long mul(long long a, long long b, long long mod) {
        a %= mod; b %= mod;
        return (long long)(((unsigned long long)a * b) % mod);
    }

    long long pow(long long base, long long exp, long long mod) {
        long long res = 1;
        base %= mod;
        while (exp > 0) {
            if (exp % 2 == 1) res = mul(res, base, mod);
            base = mul(base, base, mod);
            exp /= 2;
        }
        return res;
    }

    long long inverse(long long b, long long mod) {
        return pow(b, mod - 2, mod);
    }

    long long div(long long a, long long b, long long mod) {
        a %= mod;
        long long inv = inverse(b, mod);
        
        if (inv == -1) {
            return -1; 
        }
        
        return mul(a, inv, mod);
    }

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
            const unsigned long long collisionsNumber{997};
            const size_t lengthSource{length()};
            const size_t lengthSought{stringSought.length()};

            if (lengthSought > lengthSource || lengthSought == 0) return -1;

            unsigned long long hashSought{0};
            unsigned long long hashWindowSource{0};
            const unsigned long long rootGorner{17};

            unsigned long long lastCoefficent = 1;
            for (size_t i = 0; i < lengthSought - 1; i++)
                lastCoefficent = (lastCoefficent * rootGorner) % collisionsNumber;

            for (size_t i = 0; i < lengthSought; i++) {
                unsigned long long coefficentGorner(
                    pow(rootGorner, lengthSought - i - 1, collisionsNumber)
                );

                hashWindowSource = add(hashWindowSource,
                        mul(at(i), coefficentGorner,
                            collisionsNumber), collisionsNumber);

                hashSought = add(hashSought, mul(stringSought.at(i),
                            coefficentGorner, collisionsNumber),
                        collisionsNumber);
            }

            if (hashSought == hashWindowSource && 0 == strncmp(c_str(),
                    stringSought.c_str(), lengthSought)) return 0;

            for (size_t i = 0; i < lengthSource - lengthSought; i++) {
                const unsigned long long lastElement(
                    mul(at(i), lastCoefficent, collisionsNumber)
                );

                hashWindowSource = sub(hashWindowSource,
                        lastElement, collisionsNumber);
                hashWindowSource = mul(hashWindowSource,
                        rootGorner, collisionsNumber);
                hashWindowSource = add(hashWindowSource,
                        at(i + lengthSought), collisionsNumber);

                if (hashSought == hashWindowSource && 0 == strncmp(c_str() + i + 1,
                        stringSought.c_str(), lengthSought)) return i + 1;
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
