/**
 * @file Decoder.hpp
 * @author TL044CN
 * @brief Decoder class definition
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
 * @brief Decoder class
 * @details This class is used to decode a symbol from a stream of bits
 * @ingroup Coder
 */
class Decoder {
public:
    using stream_t = std::vector<uint8_t>;           ///< Type that represents a stream of bits
    using stream_iter_t = stream_t::const_iterator;  ///< Type that represents an iterator for the stream of bits
private:
    FrequencyModel& mModel;         ///< The model to use for decoding
    uint64_t mLow = 0;              ///< The lower bound of the range
    uint64_t mHigh = 0;             ///< The upper bound of the range
    uint64_t mCode = 0;             ///< The current code
    stream_iter_t mByteIter;        ///< The current position in the stream
    stream_iter_t mStreamEnd;       ///< The end of the stream
    uint8_t mBitBuffer = 0;         ///< The bit buffer
    uint8_t mBitCount = 0;          ///< The number of bits in the buffer

    bool mIsEndOfStream = false;    ///< Flag to indicate if the end of the stream has been reached

public:
    /**
     * @brief Construct a new Decoder object
     * @note The model should be the same model that was used to encode the data
     * @note The model will be reset to have frequencies of 1
     */
    Decoder(FrequencyModel& model, const stream_t& streamIter);

    /**
     * @brief Construct a new Decoder object
     * @note The model should be the same model that was used to encode the data
     * @note The model will be reset to have frequencies of 1
     * @warning If the range between streamBegin and streamEnd doesnt hold valid data
     *          the decoder will not be able to decode the data and might crash
     */
    Decoder(FrequencyModel& model, stream_iter_t streamBegin, stream_iter_t streamEnd);

private:
    /**
     * @brief Read a bit from the stream
     * @details This function will read a bit from the stream and update the bit buffer
     * @return true if the bit is 1
     * @return false if the bit is 0
     */
    bool readBit();

public:

    /**
     * @brief Decode a symbol from a stream of bits
     * @note when the end of the stream is reached the function will return 0 and
     *       the model will be reset
     * @param stream the stream of bits
     * @return FrequencyModel::symbol_t the decoded symbol
     */
    FrequencyModel::symbol_t decodeSymbol();

    /**
     * @brief Check if the end of the stream has been reached
     * 
     * @return true if the end of the stream has been reached
     * @return false if the end of the stream has not been reached
     */
    bool isEndOfStream() const;
};

} // namespace KestrelPack
