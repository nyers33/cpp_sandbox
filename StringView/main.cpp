#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

constexpr std::string_view text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";

auto splitByWords(std::string_view input) {
    return input
        | std::views::split(' ')
        | std::views::transform([](auto&& r) -> std::string_view { return { r.begin(), r.end() }; });
}

int main()
{
    {
        using std::operator""sv;
        constexpr auto words{ "Hello^_^C++^_^20^_^!"sv };
        constexpr auto delim{ "^_^"sv };

        for (const auto& word : std::views::split(words, delim))
            std::cout << std::quoted(std::string_view(word)) << ' ';
        std::cout << std::endl;
    }
    std::cout << std::endl;

    {
        for (std::string_view&& word : splitByWords(text)) {
            std::cout << word << std::endl;
        }
    }
    
    return 0;
}
