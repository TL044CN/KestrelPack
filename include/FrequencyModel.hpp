/**
 * @file FrequencyModel.hpp
 * @author TL044CN
 * @brief FrequencyModel class header file
 * @version 0.1
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <tuple>
#include <map>
#include <cstdint>
#include <span>

namespace KestrelPack {
/**
 * @brief FrequencyModel class
 */
class FrequencyModel {
    using symbol_t = char;

    /**
     * @brief SymbolData struct
     */
    struct SymbolData {
        uint32_t frequency;                     ///< Frequency of the symbol
        std::pair<uint32_t, uint32_t> range;    ///< Range of the symbol
    };

private:
    std::map<char, SymbolData> mFrequencyMap;   ///< Frequency map
    uint32_t mTotalFrequency = 0;               ///< Count of all symbols that were addad to the model
    bool mModelChanged = false;                 ///< Flag to indicate if the model has changed

public:
    /**
     * @brief Construct a new Frequency Model object
     * 
     * @param symbols the symbols to add to the model
     */
    FrequencyModel(std::span<const char> symbols);

private:
    /**
     * @brief Add a symbol to the model
     * 
     * @param symbol the symbol to add
     */
    void addSymbol(symbol_t symbol);

    /**
     * @brief Recalculate the ranges of the symbols
     */
    void recalculateSymbolData();

public:
    /**
     * @brief Increment the frequency of a symbol
     * @note This function will update the model if it has changed
     * @note This function will not add the symbol to the model
     * @param symbol the symbol to get the data for
     */
    void updateModel(symbol_t symbol);

    /**
     * @brief Get the Symbol Range object
     * 
     * @param symbol the symbol to get the range for
     * @return std::pair<uint32_t, uint32_t> the range of the symbol
     */
    std::pair<uint32_t, uint32_t> getSymbolRange(symbol_t symbol) const;

    /**
     * @brief Get the Number of Symbols that were added to the model
     * 
     * @return uint32_t the number of symbols
     */
    uint32_t getTotalFrequency() const;

};

} // namespace KestrelPack