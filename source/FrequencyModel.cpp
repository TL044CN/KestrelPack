/**
 * @file FrequencyModel.cpp
 * @author TL044CN
 * @brief FrequencyModel class implementation file
 * @version 0.1
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "FrequencyModel.hpp"

namespace KestrelPack {

FrequencyModel::FrequencyModel(std::span<const char> symbols) {
    for (const auto& symbol : symbols)
        if(!mFrequencyMap.contains(symbol))
            addSymbol(symbol);
    mIsReset = true;
}


void FrequencyModel::addSymbol(symbol_t symbol) {
    if(mFrequencyMap.contains(symbol)) return;
    mFrequencyMap[symbol] = {.frequency = 1};
    mTotalFrequency++;
    mModelChanged = true;
}

void FrequencyModel::recalculateSymbolData() {
    if(!mModelChanged) return;

    uint64_t previousBoundary = 0;
    uint64_t partitionSize = UINT64_MAX / mTotalFrequency;

    for (auto& [symbol, data] : mFrequencyMap) {
        auto& [symbolLow, symbolHigh] = data.range;
        symbolLow = previousBoundary;
        symbolHigh = symbolLow + (data.frequency * partitionSize);
        previousBoundary = symbolHigh;
    }

    if(!mFrequencyMap.empty())
        mFrequencyMap.rbegin()->second.range.second = UINT64_MAX;

    mModelChanged = false;
}


void FrequencyModel::updateModel(symbol_t symbol) {
    if(!mFrequencyMap.contains(symbol)) return;
    mFrequencyMap[symbol].frequency++;
    mTotalFrequency++;
    mModelChanged = true;
    mIsReset = false;
}

std::pair<uint64_t, uint64_t> FrequencyModel::getSymbolRange(symbol_t symbol) const {
    // This just ensures lazy evaluation.
    const_cast<FrequencyModel*>(this)->recalculateSymbolData();

    if(!mFrequencyMap.contains(symbol))
        return {0, 0};
    return mFrequencyMap.at(symbol).range;
}

FrequencyModel::symbol_t FrequencyModel::getSymbol(uint64_t value) const {
    for (const auto& [symbol, data] : mFrequencyMap) {
        if (value >= data.range.first && value < data.range.second)
            return symbol;
    }
    return 0;
}

uint32_t FrequencyModel::getTotalFrequency() const {
    return mTotalFrequency;
}

void FrequencyModel::reset() {
    if(mIsReset) return;

    mTotalFrequency = 0;
    for (auto& [symbol, data] : mFrequencyMap){
        data.frequency = 1;
        ++mTotalFrequency;
    }
    mModelChanged = true;
    mIsReset = true;
}

bool FrequencyModel::isReset() const {
    return mIsReset;
}

} // namespace KestrelPack