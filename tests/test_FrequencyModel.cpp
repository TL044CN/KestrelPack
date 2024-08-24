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
        THEN("The range of the symbol 'a' should be 0 to 1/3 of the range") {
            auto [low, high] = model.getSymbolRange('a');
            CHECK(low  == 0);
            CHECK(high == 0x5555555555555555);
        }
        THEN("The range of the symbol 'b' should be 1/3 to 2/3 of the range") {
            auto [low, high] = model.getSymbolRange('b');
            CHECK(low  == 0x5555555555555555);
            CHECK(high == 0xAAAAAAAAAAAAAAAA);
        }
        THEN("The range of the symbol 'c' should be 2/3 to the end of the range") {
            auto [low, high] = model.getSymbolRange('c');
            CHECK(low  == 0xAAAAAAAAAAAAAAAA);
            CHECK(high == 0xFFFFFFFFFFFFFFFF);
        }
        WHEN("A symbol ('a') is added") {
            model.updateModel('a');
            THEN("The total frequency should be 4") {
                REQUIRE(model.getTotalFrequency() == 4);
            }
            THEN("The range of the symbol should be 0 to half of the range") {
                auto [low, high] = model.getSymbolRange('a');
                CHECK(low  == 0);
                CHECK(high == 0x7ffffffffffffffe);
            }
        }
        WHEN("The symbol 'b' is added") {
            model.updateModel('b');
            THEN("The total frequency should be 4") {
                REQUIRE(model.getTotalFrequency() == 4);
            }
            THEN("The range of the symbol should be 1/4 to 3/4 of the range") {
                auto [low, high] = model.getSymbolRange('b');
                CHECK(low  == 0x3fffffffffffffff);
                CHECK(high == 0xbffffffffffffffd);
            }
        }
        WHEN("Two symbols are added ('a','b')") {
            model.updateModel('a');
            model.updateModel('b');
            THEN("The total frequency should be 5") {
                REQUIRE(model.getTotalFrequency() == 5);
            }
            THEN("The range of the symbol 'a' should be 0 to 2/5 of the range") {
                auto [low, high] = model.getSymbolRange('a');
                CHECK(low  == 0);
                CHECK(high == 0x6666666666666666);
            }
            THEN("The range of the symbol 'b' should be 2/5 to 4/5 of the range") {
                auto [low, high] = model.getSymbolRange('b');
                CHECK(low  == 0x6666666666666666);
                CHECK(high == 0xCCCCCCCCCCCCCCCC);
            }
        }
    }
}
