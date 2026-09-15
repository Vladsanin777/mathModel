#include <istream>
#include <ostream>
#include <iostream>
#include <vector>
#include <utility>

namespace lab2 {
    template<typename T>
    class vector : public std::vector<T> {
    public:
        using std::vector<T>::vector;
        using std::vector<T>::size;
        using std::vector<T>::push_back;
        using std::vector<T>::pop_back;
        using std::vector<T>::empty;
        using std::vector<T>::at;

        friend std::istream& operator>>(
                std::istream& in, lab2::vector<T>& array) {
            for (T& element : array) in >> element;
            return in;
        }

        friend std::ostream& operator<<(
                std::ostream& out, const lab2::vector<T>& array) {
            for (const T& element : array) out << element << ' ';
            return out;
        }

        size_t linearSearchSentinel(const T &elementSought) {
            size_t last{size()};
            push_back(elementSought);
            size_t i{0};

            for (i = 0; at(i) != elementSought; i++);

            size_t indexElementSought = i == last ? -1 : i;

            (void)pop_back();

            return indexElementSought;
        }

        size_t interpolationSearch(const T &elementSought) const {
            return interpolationSearch(0, size() - 1, elementSought);
        }

        void quickSort() {
            if (this->empty()) return;
            quickSort(0, size() - 1);
        }

    private:
        size_t partition(size_t low, size_t high) {
            T pivot = at(high);
            size_t i = low;

            for (size_t j = low; j < high; j++) {
                if (at(j) <= pivot) {
                    std::swap(at(i), at(j)); i++;
                }
            }

            std::swap(at(i), at(high));

            return i;
        }

        void quickSort(size_t low, size_t high) {
            if (low < high) {
                size_t pi = partition(low, high);
                if (pi > 0) {
                    quickSort(low, pi - 1);
                }
                quickSort(pi + 1, high);
            }

        }

        size_t interpolationSearch(size_t low, size_t high,
                const T &elementSought) const {
            if (low > high || elementSought < at(low)
                    || elementSought > at(high)) return -1;

            size_t pos = low + (((double)(high - low)
                        / (at(high) - at(low))) * (elementSought - at(low)));

            if (at(pos) < elementSought)
                return interpolationSearch(pos + 1, high, elementSought);

            if (at(pos) > elementSought)
                return interpolationSearch(low, pos - 1, elementSought);

            return pos;
        }
    };
}

int main(void) {
    std::cout << "Input Size array: ";
    size_t sizeArray{0};
    std::cin >> sizeArray;

    lab2::vector<int> array {
            lab2::vector<int>(sizeArray)
    };

    std::cout << "Input Array: " << std::endl;
    std::cin >> array;

    std::cout << "Input element sought: ";
    int elementSought{0};
    std::cin >> elementSought;

    std::cout << "Source array: " << std::endl
            << array << std::endl;

    size_t indexLinearSearchSource {
            array.linearSearchSentinel(elementSought)
    };
    std::cout << "Linear search whit a sentinel in source array: "
            << indexLinearSearchSource << std::endl;

    array.quickSort();
    std::cout << "Sorted array: " << std::endl
            << array << std::endl;

    size_t indexLinearSearchSorted {
            array.linearSearchSentinel(elementSought)
    };
    std::cout << "Linear search whit a sentinel in sorted array: "
            << indexLinearSearchSorted << std::endl;

    size_t indexInterpolationSearchSorted {
            array.interpolationSearch(elementSought)
    };
    std::cout << "Interpolation search in sorted array: "
            << indexInterpolationSearchSorted << std::endl;

    return 0;
}
