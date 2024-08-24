/**
 * @file Encoder.cpp
 * @author TL044CN
 * @brief Encoder class implementation file
 * @version 0.1
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Encoder.hpp"
namespace KestrelPack {

constexpr uint64_t ceHALF           = UINT64_MAX / 2;
constexpr uint64_t ceQUARTER        = UINT64_MAX / 4;
constexpr uint64_t ceTHREE_QUARTERS = ceHALF + ceQUARTER;


Encoder::Encoder(FrequencyModel& model) : mModel(model) {
    mModel.reset();
}


void Encoder::encodeBit(bool bit, stream_t& stream) {
    mBitBuffer = (mBitBuffer << 1) | bit;
    ++mBitCount;
    if (mBitCount == 8) {
        stream.push_back(mBitBuffer);
        mBitCount = 0;
    }
}


void Encoder::encodeSymbol(FrequencyModel::symbol_t symbol, stream_t& stream) {
    auto [low, high] = mModel.getSymbolRange(symbol);
    uint64_t range = mHigh - mLow;
    mHigh = mLow + (range * high) / mModel.getTotalFrequency();
    mLow  = mLow + (range * low)  / mModel.getTotalFrequency();
    while (true) {
        if (mHigh < ceHALF) {
            encodeBit(0, stream);
        } else if (mLow >= ceHALF) {
            encodeBit(1, stream);
            mLow  -= ceHALF;
            mHigh -= ceHALF;
        } else if (mLow >= ceQUARTER && mHigh < ceTHREE_QUARTERS) {
            mBitBuffer++;
            mLow  -= ceQUARTER;
            mHigh -= ceQUARTER;
        } else {
            break;
        }
        mLow  <<= 1;
        mHigh <<= 1;
        mHigh++;
    }
    mModel.updateModel(symbol);
}

void Encoder::finishEncoding(stream_t& stream) {
    encodeBit(1, stream);
    
    bool startBit = mLow >= ceQUARTER;
    encodeBit(startBit, stream);
    for(;mBitBuffer > 0; --mBitBuffer) encodeBit(!startBit, stream);

    mModel.reset();
}

} // namespace KestrelPack