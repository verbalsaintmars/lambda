/* ******************************
 * Copyleft 2015 Verbalsaint
 * ******************************/
#include <iostream>

#include <functional>
#include <tuple>
#include <utility>

auto lambda_building_block() {
    /*
     *  Return exact what passed in.
     */
    auto identity = [](auto&& x) -> decltype(auto) {
        return std::forward<decltype(x)>(x);
    };

    /*
     *  Apply passed in with itself.
     */
    auto self_apply = [](auto x) {
        x(x);
    };

    /*
     *  Apply passed in func with later on passed in arguments in sequence.
     */
    auto apply = [](auto&& func) {
        return [func=std::forward<decltype(func)>(func)>](auto&&... arg) {
            func(arg)...;
        };
    };

    /*
     * Pass in first, then second, will return first.
     */
    auto select_first = [](auto&& first) {
        return [first=std::forward<decltype(first)>(first)](auto&&)
        {
            return first;
        };
    };

    /*
     *  Pass in first, then second, will return second.
     */
    auto select_second = [](auto&&) -> decltype(auto) {
        return [](auto&& second)
        {
            return std::forward<decltype(second)>(second);
        };
    };

    /*
     * Pass in first, then second, then function,
     * function will process first, with continuation, then second.
     */
    auto make_pair = [](auto&& first) {
        return [first=std::forward<decltype(first)>(first)](auto&& second) {
            return [first=std::forward<decltype(first)>(first),
                    second=std::forward<decltype(second)>(second)] (auto&& func) {
                func(first)(second);
            };
        };
    }

    auto& True = select_first;
    auto& False = select_second;

    auto Not = [](auto&& True_Or_False) -> bool {
        return True_Or_False(false)(true);
    };

    /*
     * To be implemented
     */
    auto And = false;

    /*
     * To be implemented
     */
    auto Or = false;

    return std::make_tuple(
        identity,
        self_apply,
        apply,
        select_first,
        select_second,
        make_pair,
        True,
        False);
}

int main() {
    using namespace std;
    auto lambda_tool = lambda_building_block();

    auto select_first = std::get<3>(lambda_tool);
    auto first = []{cout << "first" << endl;};
    auto second = []{cout << "second" << endl;};
    select_first(first)(second)();

    auto select_second = std::get<4>(lambda_tool);
    select_second(first)(second)();
}
