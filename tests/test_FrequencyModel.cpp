#include "catch2/catch_test_macros.hpp"

#define private public
#define protected public
#include "FrequencyModel.hpp"
#undef private
#undef protected

SCENARIO("FrequencyModel") {
    GIVEN("A FrequencyModel") {
        std::vector<char> symbols = {'a', 'b', 'c'};
        KestrelPack::FrequencyModel model(symbols);
        THEN("The total frequency should be 3") {
            REQUIRE(model.getTotalFrequency() == 3);
        }
        THEN("The range of the symbol 'a' should be 0, 1") {
            auto [low, high] = model.getSymbolRange('a');
            REQUIRE(low == 0);
            REQUIRE(high == 1);
        }
        THEN("The range of the symbol 'b' should be 1, 2") {
            auto [low, high] = model.getSymbolRange('b');
            REQUIRE(low == 1);
            REQUIRE(high == 2);
        }
        THEN("The range of the symbol 'c' should be 2, 3") {
            auto [low, high] = model.getSymbolRange('c');
            REQUIRE(low == 2);
            REQUIRE(high == 3);
        }
        WHEN("A symbol ('a') is added") {
            model.updateModel('a');
            THEN("The total frequency should be 4") {
                REQUIRE(model.getTotalFrequency() == 4);
            }
            THEN("The range of the symbol should be 0, 2") {
                auto [low, high] = model.getSymbolRange('a');
                REQUIRE(low == 0);
                REQUIRE(high == 2);
            }
        }
        WHEN("The symbol 'b' is added") {
            model.updateModel('b');
            THEN("The total frequency should be 4") {
                REQUIRE(model.getTotalFrequency() == 4);
            }
            THEN("The range of the symbol should be 1, 3") {
                auto [low, high] = model.getSymbolRange('b');
                REQUIRE(low == 1);
                REQUIRE(high == 3);
            }
        }
        WHEN("Two symbols are added ('a','b')") {
            model.updateModel('a');
            model.updateModel('b');
            THEN("The total frequency should be 5") {
                REQUIRE(model.getTotalFrequency() == 5);
            }
            THEN("The range of the symbol 'a' should be 0, 2") {
                auto [low, high] = model.getSymbolRange('a');
                REQUIRE(low == 0);
                REQUIRE(high == 2);
            }
            THEN("The range of the symbol 'b' should be 2, 4") {
                auto [low, high] = model.getSymbolRange('b');
                REQUIRE(low == 2);
                REQUIRE(high == 4);
            }
        }
    }
}
