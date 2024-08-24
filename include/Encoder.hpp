/**
 * @file Encoder.hpp
 * @author TL044CN
 * @brief Encoder class header file
 * @version 0.1
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once
#include "FrequencyModel.hpp"
#include <vector>

namespace KestrelPack {

/**
 * @brief Encoder class for Adaptive Arithmetic Coding
 * @details The Encoder class uses the FrequencyModel to encode symbols
 *          using the adaptive arithmetic coding algorithm.
 *          The FrequencyModel should only contain the symbols that are
 *          to be encoded and each should only have a frequency of 1.
 * @ingroup Coding
 * @code {.cpp}
 * // Example usage
 * //  define Symbols to encode
 * std::string symbols = "AABBBCCDDE";
 * 
 * // Create a model for the Symbols
 * FrequencyModel model(symbols);
 * 
 * // Create an encoder with the model
 * Encoder encoder(model);
 * 
 * // Create a stream to write the encoded data to
 * std::vector<char> stream;
 * 
 * // Encode the symbols
 * encoder.encodeSymbol(symbol, stream);
 * encoder.finishEncoding(stream);
 * 
 * // stream now contains the encoded data
 * @endcode
 * 
 */
class Encoder {
public:
    using stream_t = std::vector<char>;     ///< Type that represents a stream of symbols
private:
    FrequencyModel& mModel;                 ///< Reference to the underlying encoding model
    uint64_t mLow           = 0;            ///< Low value of the current range
    uint64_t mHigh          = UINT64_MAX;   ///< High value of the current range
    uint8_t mBitBuffer      = 0;            ///< Buffer for the output
    uint8_t mBitCount       = 0;            ///< Number of bits in the buffer
    uint32_t mTotalBits     = 0;            ///< Total number of bits encoded

public:
    /**
     * @brief Construct a new Encoder object
     * @note The model will be reset to have frequencies of 1
     * @param model the model to use for encoding
     */
    Encoder(FrequencyModel& model);

private:
    /**
     * @brief Encode a bit
     * 
     * @param bit the bit to encode
     * @param stream the stream to write the encoded data to
     */
    void encodeBit(bool bit, stream_t& stream);

public:
    /**
     * @brief Encode a symbol
     * 
     * @param symbol the symbol to encode
     * @param stream the stream to write the encoded data to
     */
    void encodeSymbol(FrequencyModel::symbol_t symbol, stream_t& stream);

    /**
     * @brief Finish encoding
     * @note This function will reset the models frequencies to 1
     * @param stream the stream to write the encoded data to
     */
    void finishEncoding(stream_t& stream);

};

} // namespace KestrelPack