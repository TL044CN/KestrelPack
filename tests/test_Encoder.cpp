#include "catch2/catch_test_macros.hpp"

#define private public
#define protected public
#include "Encoder.hpp"
#undef private
#undef protected

#include <bitset>

SCENARIO("Encoder") {
    GIVEN("An Encoder") {
        std::vector<char> symbols = {'a', 'b', 'c'};
        KestrelPack::FrequencyModel model(symbols);
        KestrelPack::Encoder encoder(model);
        std::vector<char> stream;
        THEN("The low value should be 0") {
            REQUIRE(encoder.mLow == 0);
        }
        THEN("The high value should be 0xFFFFFFFF") {
            REQUIRE(encoder.mHigh == 0xFFFFFFFF);
        }
        THEN("The bit buffer should be 0") {
            REQUIRE(encoder.mBitBuffer == 0);
        }
        THEN("The bit count should be 0") {
            REQUIRE(encoder.mBitCount == 0);
        }
        THEN("The total bits should be 0") {
            REQUIRE(encoder.mTotalBits == 0);
        }
        WHEN("A symbol is encoded"){
            encoder.encodeSymbol('a', stream);
            THEN("The stream should not have any data yet") {
                REQUIRE(stream.size() == 0);
            }
            AND_WHEN("The encoding is finished") {
                encoder.finishEncoding(stream);
                THEN("The stream should have data") {
                    REQUIRE(stream.size() > 0);
                }
                THEN("the stream should contain the encoded data") {
//                    REQUIRE(std::bitset<8>(stream[0])[0] == 0);
                }
            }
        }
    }
}
