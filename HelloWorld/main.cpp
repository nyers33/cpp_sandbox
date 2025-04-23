#include <iostream>
#include <string>
#include <sstream>

#include <algorithm>
#include <functional>
#include <numeric> // accumulate
#include <ranges>

#include <list>
#include <map>
#include <set>
#include <vector>

#include <utility> // ipair
constexpr auto& ipair = std::ranges::views::enumerate;

auto print_subrange = [](std::ranges::viewable_range auto&& r)
{
    std::cout << '[';
    for (char space[]{ 0,0 }; auto elem : r)
        std::cout << space << elem, * space = ' ';
    std::cout << "] ";
};

template <typename Comparator = std::not_equal_to<char>>
int changeInString(const std::string& s, Comparator comp = Comparator{})
{
    if (s.empty())
        return 0;

    return std::accumulate(s.begin() + 1, s.end(), 0,
        [prev = s[0], comp](int count, char current) mutable {
            if (comp(prev, current)) {
                ++count;
                prev = current;
            }
            return count;
        });
};

template<typename rangeType>
void show(std::string_view msg, rangeType r)
{
    std::cout << msg;
    std::ranges::for_each(r,
        [](auto e)
        {
            std::cout << e << ' ';
        });
    std::cout << '\n';
}

template<typename T>
static std::ostream& operator<<(std::ostream& os, const std::list<T>& list)
{
    for (const char* delim = ""; auto && v : list)
    {
        os << std::exchange(delim, ", ") << v;
    }

    return os;
}

template<typename T>
static std::ostream& operator<<(std::ostream& os, const std::vector<T>& list)
{
    os << "[ ";
    for (const char* delim = ""; auto && v : list)
    {
        os << std::exchange(delim, ", ") << v;
    }
    os << " ]";

    return os;
}

template<>
static std::ostream& operator<< <char>(std::ostream& os, const std::vector<char>& list)
{
    os << "[ ";
    for (const char* delim = ""; auto && v : list)
    {
        os << std::exchange(delim, "-") << v;
    }
    os << " ]";

    return os;
}

// iterator for generating k-element subsets lazily
template<typename T>
class subset_iterator {
private:
    const std::vector<T>& v;
    std::vector<bool> selector;
    bool valid;

public:
    using value_type = std::vector<T>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

    subset_iterator(const std::vector<T>& vec, std::size_t k, bool is_end = false)
        : v(vec), selector(vec.size(), false), valid(!is_end) {
        if (!is_end) {
            std::fill(selector.begin(), selector.begin() + k, true);
        }
    }

    // dereference: returns the current subset as a filtered range
    auto operator*() const {
        return v | std::views::filter([&, i = 0](auto) mutable { return selector[i++]; });
    }

    // prefix increment: generate the next subset
    subset_iterator& operator++() {
        valid = std::prev_permutation(selector.begin(), selector.end());
        return *this;
    }

    // equality comparison for stopping iteration
    bool operator!=(std::default_sentinel_t) const {
        return valid;
    }
};

// custom range view for k-element subsets
template<typename T>
class subset_view : public std::ranges::view_interface< subset_view<T> > {
private:
    const std::vector<T>& v;
    std::size_t k;

public:
    subset_view(const std::vector<T>& vec, std::size_t subset_size) : v(vec), k(subset_size) {}

    auto begin() const { return subset_iterator(v, k); }
    auto end() const { return std::default_sentinel; }
};

template <typename T>
class squared_accumulate
{
public:
    squared_accumulate(void) :
        sum(0)
    {
    }

    const T& result(void) const
    {
        return sum;
    }

    void operator()(const T& val)
    {
        sum += val * val;
    }

private:
    T sum;
};

struct functor_squared_accumulate {
    int operator()(int sum, int x) const {
        return sum + x * x;
    }
};

template<typename T>
struct sumTotal {
    sumTotal(T inInitTotal = 0) : total(inInitTotal) {}
    T total;
    void operator ()(const int x) { total += x; }
};

template<typename T>
struct sumSeq {
    std::vector<T> result;
    auto operator ()(const T x) {
        const auto seq = std::views::repeat(x, x);
        const auto seqVec = std::ranges::to<std::vector>(seq);
        result.insert(result.end(), seqVec.begin(), seqVec.end());
    }
};

template<typename T>
struct addX {
    addX(T val) : x(val) {}
    T operator()(T y) const { return x + y; }

private:
    T x;
};

template<typename T>
static T minDiffInVec(std::vector<T> v, bool bIsSorted = false)
{
    T minDiff = std::numeric_limits<T>::max();

    if (!bIsSorted)
    {
        std::sort(v.begin(), v.end());
    }

    for (auto it = v.begin(); it != v.end() - 1; ++it)
    {
        minDiff = std::min(abs(*it - *(it + 1)), minDiff);
    }

    return minDiff;
}

template<class Iter>
void merge_sort(Iter first, Iter last)
{
    if (last - first > 1)
    {
        Iter middle = first + (last - first) / 2;
        merge_sort(first, middle);
        merge_sort(middle, last);
        // merges two consecutive sorted ranges
        show("l | ", std::ranges::subrange(first, middle));
        show("r | ", std::ranges::subrange(middle, last));
        std::inplace_merge(first, middle, last);
        show("merged | ", std::ranges::subrange(first, last));
    }
}

template<typename T>
// SetB is a subset of SetA if every element in SetB is also in SetA
bool isSubset(std::vector<T>& a, std::vector<T>& b) {

    // sort both arrays
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    int i = 0, j = 0;

    // traverse both arrays
    while (i < a.size() && j < b.size()) {
        if (a[i] < b[j]) {
            i++;
        }
        else if (a[i] == b[j]) {
            i++;
            j++;
        }
        else {
            return false; // element in SetB is not found in SetA
        }
    }

    // traversed all elements in SetB, it is a subset
    return (j == b.size());
}

bool lexiGreater(const std::string& sX, const std::string& sY)
{
    size_t maxSize = std::max(sX.size(), sY.size());

    for (int i = 0; i < maxSize; ++i)
    {
        if (static_cast<uint8_t>(sX[i]) > static_cast<uint8_t>(sY[i]))
        {
            return false;
        }
    }

    return true;
}

namespace geometry
{
    struct Circle2D
    {
        double x{ 0.0 };
        double y{ 0.0 };
        double rad{ 1.0 };
    };

    std::ostream& operator<<(std::ostream& os, const Circle2D& circle)
    {
        os << "Circle2D(x[" << circle.x << "], y[" << circle.y << "], rad[" << circle.rad << "])";
        return os;
    }
}

int main()
{
    std::cout << "string manip" << std::endl;
    {
        std::string s = "bab";
        std::string t = "ab";

        std::cout << "original: " << s << std::endl;
        int counter = 0;

        for (int i = 0; i < s.size(); i++)
        {
            for (int j = i; j < s.size(); j++)
            {
                std::string subsequence(s);
                subsequence.erase(i, j);
                bool comp = lexiGreater(subsequence, t);
                std::cout << subsequence << " - " << comp << std::endl;
            }
        }

    }
    
    std::cout << "string manip" << std::endl;
    {
        // int std::stoi( str ) & long std::stol( str )
        // float std::stof( str ) & double std::stod( str )

        const std::string s0123 = "0123456789";
        int idxStart = 2;
        int idxEnd = 4;
        std::cout << s0123 << " " << std::vector<int>({ idxStart,idxEnd }) << " - " << s0123.substr(idxStart, idxEnd - 1) << std::endl;


        const std::string sAmz = "***|**|*|***|****";
        idxStart = static_cast<int>(sAmz.find_first_of('|'));
        idxEnd = static_cast<int>(sAmz.find_last_of('|'));
        if (idxStart == -1)
        {
            std::cout << sAmz << " " << std::vector<int>({ idxStart,idxEnd }) << " - 0" << std::endl;
        }
        else
        {
            int counter = 0;
            const std::string sAmzSubstr = sAmz.substr(idxStart, idxEnd - idxStart + 1);

            for (char c : sAmzSubstr)
            {
                if (static_cast<uint8_t>(c) == 42)
                    ++counter;
            }

            std::cout << sAmz << " " << std::vector<int>({ idxStart,idxEnd }) << " - " << sAmzSubstr << " - " << counter << std::endl;
        }

        {
            const std::vector< std::string > sAmzLogs = { "88 99 200", "71 12 400", "88 99 300", "99 32 100", "12 12 150", "12 12 450" };
            int treshold = 2;
            std::cout << sAmzLogs << " - " << treshold << std::endl;
            
            std::map<std::string, int> userMap;

            for (int i = 0; i < static_cast<int>(sAmzLogs.size()); ++i)
            {
                std::string sender;
                std::string recip;
                std::string amount;

                std::stringstream ss(sAmzLogs[i]);
                ss >> sender;
                ss >> recip;
                ss >> amount;

                std::cout << sender << " " << recip << " " << amount << std::endl;

                auto umapInsert = userMap.insert({ sender, 1 });
                if (!umapInsert.second) {
                    // key was already present; increment the value at the key
                    (umapInsert.first->second) += 1;
                }

                if (sender != recip)
                {
                    umapInsert = userMap.insert({ recip, 1 });
                    if (!umapInsert.second) {
                        // key was already present; increment the value at the key
                        ++(umapInsert.first->second);
                    }
                }

                for (auto entry : userMap) {
                    std::cout << entry.first << " - " << entry.second << std::endl;
                }
            }

            std::vector< std::pair<std::string, int> > filteredUsers;
            for (const auto& entry : userMap) {
                if (entry.second >= treshold) {
                    filteredUsers.push_back(entry);
                }
            }

            std::sort(filteredUsers.begin(), filteredUsers.end(),
                [](const auto& a, const auto& b)
                {
                    return a.first < b.first;
                });

            std::cout << "filteredUsers: " << filteredUsers.size() << std::endl;
            for (auto entry : filteredUsers) {
                std::cout << entry.first << " - " << entry.second << std::endl;
            }
        }
    }
    std::cout << std::endl;

    std::cout << "range-based for loop with indices" << std::endl;
    {
        const std::vector v = { "never", "gonna", "let", "you", "down" };

        for (auto&& [idx, str] : ipair(v))
        {
            std::cout << "idx: " << idx << " - " << str << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "sum elements in a vector" << std::endl;
    {
        const auto v = std::vector{ 1, 2, 3, };
        const auto [iter, func] = std::ranges::for_each(v, sumTotal<int>(0));
        std::cout << v << " = " << func.total << std::endl;
    }
    std::cout << std::endl;

    std::cout << "transform elements in a vector - add" << std::endl;
    {
        auto v = std::ranges::to<std::vector>(std::views::iota(1, 5));
        std::cout << v << std::endl;

        show("v | pplnTransform: ", v | std::views::transform([](int i) { return i+3; }));

        std::transform(v.begin(), v.end(), v.begin(), addX(3));
        std::cout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "squared sum - for_each returns (a copy of) the functor that it was using" << std::endl;
    {
        const std::vector<int> v = { 0, 1, 2, 5, 1 };
        int vSum = std::for_each(v.begin(), v.end(), squared_accumulate<int>()).result();
        std::cout << v << " = " << vSum << std::endl;

        vSum = 0;
        std::for_each(v.begin(), v.end(), [&](int i) { vSum += i * i; });

        vSum = std::accumulate(v.begin(), v.end(), 0, functor_squared_accumulate());
    }
    std::cout << std::endl;

    std::cout << "for_each with function and a predicate (even elements of vector ++)" << std::endl;
    {
        std::vector<int> v = { 0, 1, 2, 5, 1 };
        auto even = [](int i) { return i % 2 == 0; };
        std::cout << v << std::endl;
        std::ranges::for_each(v | std::views::filter(even), [](int& i) { i += 1; });
        std::cout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "for_each with function and a predicate (sum of every third element)" << std::endl;
    {
        std::vector<int> v = { 0, 1, 2, 5, 9, 13 };
        std::cout << v << std::endl;

        sumTotal<int> functorSum(0);
        auto r = std::views::zip(v, std::views::iota(0))
            | std::views::filter([](const auto& p) { return std::get<1>(p) % 3 == 2; }) | std::views::elements<0>;
        
        auto [_, func] = std::ranges::for_each(r, functorSum);
        std::cout << func.total << std::endl;
    }
    std::cout << std::endl;

    std::cout << "remove from container (even elements removed)" << std::endl;
    {
        std::vector<int> v = { 0, 1, 2, 5, 1 };
        std::cout << v << std::endl;
        std::erase_if(v, [](int x) { return 0 == x % 2; });
        std::cout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "reusing view pipeline - taking first 3 elements and reversing" << std::endl;
    {
        std::vector v{ 1, 2, 3, 4 };
        show("v: ", v);

        auto pplnRev3 = std::views::take(3) | std::views::reverse;

        show("v | pplnRev3: ", v | pplnRev3);

        v.insert(v.begin(), 0);
        show("v: ", v);
        // rebuild the view reusing the pipeline
        show("v | pplnRev3(v): ", pplnRev3(v));
    }
    std::cout << std::endl;
    

    std::cout << "minimum difference between elements" << std::endl;
    {
        std::vector<int> v = { 1, 29, 7, 41, 12, 20 };
        std::cout << v << std::endl;
        std::cout << minDiffInVec(v) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "max element in multi vector" << std::endl;
    {
        const std::vector<std::vector<int>> v{ {1, 2}, {3, 4, 5}, {6}, {7, 8, 9} };
        auto jv = std::ranges::join_view(v);
    }
    std::cout << std::endl;

    std::cout << "run-length encoding" << std::endl;
    {
        std::vector<char> v = { 'a', 'a', 'a', 'b', 'c', 'c', 'x', 'a', 'd', 'd' };
        std::cout << v << std::endl;

        std::string result("");
        auto it = v.begin();
        int counter = 1;
        while (it != (v.end() - 1))
        {
            if (*it == *(it + 1))
            {
                ++counter;
            }
            else
            {
                result += std::to_string(counter);
                result += std::string{ *it }; // std::string(1, *it)
                counter = 1;
            }
            ++it;
        }
        result += std::to_string(counter);
        result += std::string{ *it }; // std::string(1, *it)

        std::cout << result << std::endl;
    }
    std::cout << std::endl;

    std::cout << "run-length decoding" << std::endl;
    {
        const std::vector<char> v = { '2', 'a', '1', 'b', '3', 'c', '1', 'a', '4', 'x' };
        std::cout << v << std::endl;

        std::string result("");
        for (auto it = v.begin(); it != v.end(); it += 2)
        {
            result += std::string(static_cast<int>(*it) - 48, *(it + 1));
        }
        std::cout << result << std::endl;
    }
    std::cout << std::endl;

    std::cout << "sequence generation" << std::endl;
    {
        const auto vRange = std::views::iota(1, 5);
        const auto v = std::ranges::to<std::vector>(vRange);
        const auto [iter, func] = std::ranges::for_each(v, sumSeq<int>());
        std::cout << func.result << std::endl;
    }
    std::cout << std::endl;

    std::cout << "nth_element" << std::endl;
    {
        std::vector<int> v{ 5, 10, 6, 4, 3, 2, 6, 7, 9, 3 };

        auto m = v.begin() + v.size() / 2;
        std::nth_element(v.begin(), m, v.end());
        std::cout << "median value is " << v[v.size() / 2] << std::endl;
        std::cout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "subset creation [int]" << std::endl;
    {
        const std::vector<int> subsetCreateIntVec = { 0, 1, 2, 5, 1 };
        int k = 3;

        for (auto subset : subset_view(subsetCreateIntVec, k)) {
            std::vector<int> subsetVec;
            for (const auto num : subset) {
                subsetVec.push_back(num);
            }

            std::cout << subsetVec;
            std::cout << " = " << std::accumulate(subsetVec.begin(), subsetVec.end(), 0);
            std::cout << " | " << minDiffInVec(subsetVec);
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "subset creation [char]" << std::endl;
    {
        const std::vector<char> subsetCreateCharVec = { 'a', 'b', 'c', 'x', 'a' };
        int k = 3;

        auto dash_fold = [](std::string a, char b) { return std::move(a) + '-' + std::string(1, b); };

        for (auto subset : subset_view(subsetCreateCharVec, k)) {
            std::vector<char> subsetVec;
            for (const auto num : subset) {
                subsetVec.push_back(num);
            }

            std::cout << subsetVec << " / ";
            std::cout << std::accumulate(std::next(subsetVec.begin()), subsetVec.end(), std::string(1, subsetVec[0]), dash_fold) << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "filter a container using a predicate and transform it" << std::endl;
    {
        auto const ints = { 0, 1, 2, 3, 4, 5 };
        auto even = [](int i) { return 0 == i % 2; };
        auto square = [](int i) { return i * i; };

        // the "pipe" syntax of composing the views:
        for (int i : ints | std::views::filter(even) | std::views::transform(square))
            std::cout << i << ' ';

        std::cout << std::endl;

        // a traditional "functional" composing syntax:
        for (int i : std::views::transform(std::views::filter(ints, even), square))
            std::cout << i << ' ';

        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "doubly-linked list sorted insertion" << std::endl;
    {
        std::list<int> myList = { 7, 5, 16, 8 };
        std::cout << myList;
        std::cout << std::endl;

        myList.sort();
        std::cout << myList;
        std::cout << std::endl;

        myList.sort([](const int& a, const int& b) { return a < b; });
        std::cout << myList;
        std::cout << std::endl;

        myList.insert(std::lower_bound(myList.begin(), myList.end(), 1), 1);
        myList.insert(std::lower_bound(myList.begin(), myList.end(), 9), 9);
        std::cout << myList;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "filter by index using zip" << std::endl;
    {
        const std::vector v = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3 };
        auto r = std::views::zip(v, std::views::iota(0))
            | std::views::filter([](const auto& p) { return std::get<0>(p) < std::get<1>(p); })
            | std::views::elements<0>;

        const auto v_filtered = std::ranges::to<std::vector>(r);
        std::cout << v_filtered;
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "sliding window" << std::endl;
    {
        const auto vec = { 1, 2, 3, 4, 5 };
        std::cout << "All sliding windows of width:" << std::endl;
        for (const unsigned width : std::views::iota(1U, 1U + vec.size()))
        {
            auto const windows = vec | std::views::slide(width);
            std::cout << "W = " << width << ": ";
            std::ranges::for_each(windows, print_subrange);
            std::cout << std::endl;
        }
        
        constexpr std::array arr{ 1, 2, 3, 4, 5, 6 };
        for (int i{}; std::tuple t : arr | std::views::adjacent<3>)
        {
            auto [t0, t1, t2] = t;
            std::cout << std::format("e = {:<{}}[{} {} {}]\n", "", 2 * i++, t0, t1, t2);
        }
    }
    std::cout << std::endl;

    std::cout << "merge sort" << std::endl;
    {
        std::vector<int> v{ 8, 2, -2, 0, 11, 11, 1, 7, 3 };
        std::cout << v << std::endl;

        merge_sort(v.begin(), v.end());
        std::cout << v << std::endl;
    }
    std::cout << std::endl;

    std::cout << "subset" << std::endl;
    {
        std::vector<int> a = { 11, 1, 13, 21, 3, 7 };
        std::vector<int> b = { 11, 3, 7, 2 };

        std::cout << a << std::endl;
        std::cout << b << std::endl;
        if (isSubset(a, b)) {
            std::cout << "true" << std::endl;
        }
        else {
            std::cout << "false" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "adjacent_difference" << std::endl;
    {
        std::vector v{ 4, 6, 9, 13, 18, 19, 19, 15, 10 };
        show("v = ", v);
        std::adjacent_difference(v.begin(), v.end(), v.begin());
        show("adj v = ", v);
    }
    std::cout << std::endl;

    std::cout << "transitions in string" << std::endl;
    {
        std::string s1 = "aaabbaabba";
        std::string s2 = "bbaa";
        std::string s3 = "aaa";
        std::string s4 = "aba";

        std::cout << s1 << " - " << changeInString(s1) << std::endl;
        std::cout << s2 << " - " << changeInString(s2) << std::endl;
        std::cout << s3 << " - " << changeInString(s3) << std::endl;
        std::cout << s4 << " - " << changeInString(s4) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "transitions in string" << std::endl;
    {
        std::string s1 = "aaaabbccceeex";
        std::string s2 = "bbbaaabbb";
        std::string s3 = "xyzax";
        std::string s4 = "dcba";

        // increasing transitions
        std::cout << s1 << " - " << changeInString(s1, std::greater<char>{}) << std::endl;
        std::cout << s2 << " - " << changeInString(s2, std::greater<char>{}) << std::endl;
        std::cout << s3 << " - " << changeInString(s3, std::greater<char>{}) << std::endl;
        std::cout << s4 << " - " << changeInString(s4, std::greater<char>{}) << std::endl;
    }
    std::cout << std::endl;

    std::cout << "designated initializers" << std::endl;
    // only work with aggregate types in c++20
    // a class is an aggregate if:
    // - has no user-declared constructors
    // no private or protected non-static data members
    // no base classes or virtual functions
    {
        // default initialization
        geometry::Circle2D c1;
        std::cout << "default initialized: " << c1 << std::endl;
        
        // c++20 designated initializer
        geometry::Circle2D c2{ .x = 5.0, .y = 3.5, .rad = 2.0 };
        std::cout << "designated initialized: " << c2 << std::endl;

        // only update some values
        geometry::Circle2D c3{ .y = 4.2 }; // x and rad will take default values
        std::cout << "partially designated initialized: " << c3 << std::endl;

        // aggregate initialization using parentheses
        geometry::Circle2D c4(1.1, 2.2, 3.3);
        std::cout << "constructor initialized: " << c4 << std::endl;
    }

    return 0;
}
