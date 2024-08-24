/**
 * @file Decoder.cpp
 * @author TL044CN
 * @brief Decoder class implementation file
 * @version 0.1
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Decoder.hpp"

constexpr uint64_t ceHALF           = UINT64_MAX / 2;
constexpr uint64_t ceQUARTER        = UINT64_MAX / 4;
constexpr uint64_t ceTHREE_QUARTERS = ceHALF + ceQUARTER;

namespace KestrelPack {

Decoder::Decoder(FrequencyModel& model,const stream_t& stream)
: mModel(model), mByteIter(stream.begin()), mStreamEnd(stream.end()), mIsEndOfStream(false) {
    mModel.reset();
}

Decoder::Decoder(FrequencyModel& model, stream_iter_t streamBegin, stream_iter_t streamEnd)
: mModel(model), mByteIter(streamBegin), mStreamEnd(streamEnd), mIsEndOfStream(false) {
    mModel.reset();
}

bool Decoder::readBit() {
    if(mIsEndOfStream) return false;

    if (mBitCount == 0) {
        if (mByteIter == mStreamEnd) {
            mIsEndOfStream = true;
            return false;
        }
        mBitBuffer = *mByteIter;
        mBitCount = 8;
        ++mByteIter;
    }
    bool bit = mBitBuffer & 0b10000000;
    mBitBuffer <<= 1;
    --mBitCount;
    return bit;
}

FrequencyModel::symbol_t Decoder::decodeSymbol() {
    if (mIsEndOfStream) {
        mModel.reset();
        return 0;
    }

    uint64_t rangeSize = mHigh - mLow + 1;
    uint64_t value = ((mCode - mLow + 1) * mModel.getTotalFrequency() - 1) / rangeSize;

    char symbol = mModel.getSymbol(value);
    auto [low, high] = mModel.getSymbolRange(symbol);

    mHigh = mLow + (rangeSize * high) / mModel.getTotalFrequency() - 1;
    mLow = mLow + (rangeSize * low)   / mModel.getTotalFrequency();

    while (true) {
        if (mHigh < ceHALF) {
            // do nothing
        } else if (mLow >= ceHALF) {
            mCode -= ceHALF;
            mLow  -= ceHALF;
            mHigh -= ceHALF;
        } else if (mLow >= ceQUARTER && mHigh < ceTHREE_QUARTERS) {
            mCode -= ceQUARTER;
            mLow  -= ceQUARTER;
            mHigh -= ceQUARTER;
        } else {
            break;
        }

        mLow <<= 1;
        mHigh <<= 1;
        mHigh |= 1;
        mCode <<= 1;
        mCode |= readBit();
    }

    mModel.updateModel(symbol);
    return symbol;
}

bool Decoder::isEndOfStream() const {
    return mIsEndOfStream;
}

} // namespace KestrelPack