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
}


void FrequencyModel::addSymbol(symbol_t symbol) {
    mFrequencyMap[symbol] = {.frequency = 1};
    mTotalFrequency++;
    mModelChanged = true;
}

void FrequencyModel::recalculateSymbolData() {
    if(!mModelChanged) return;

    uint32_t previousBoundary = 0;
    for (auto& [symbol, data] : mFrequencyMap){
        auto& [symbolLow, symbolHigh] = data.range;
        symbolLow = previousBoundary;
        symbolHigh = symbolLow + data.frequency;
        previousBoundary = symbolHigh;
    }
    mModelChanged = false;
}


void FrequencyModel::updateModel(symbol_t symbol) {
    if(!mFrequencyMap.contains(symbol))
        return;
    else
        mFrequencyMap[symbol].frequency++;
    mTotalFrequency++;
    mModelChanged = true;
}

std::pair<uint32_t, uint32_t> FrequencyModel::getSymbolRange(symbol_t symbol) const {
    if(!mFrequencyMap.contains(symbol))
        return {0, 0};
    return mFrequencyMap.at(symbol).range;
}

uint32_t FrequencyModel::getTotalFrequency() const {
    return mTotalFrequency;
}

} // namespace KestrelPack