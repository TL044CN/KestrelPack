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
 * @details The FrequencyModel class is used to create a frequency model for
 *          adaptive arithmetic coding. The model is used to store the frequency
 *          of each symbol and the range of each symbol.
 *          The model should only contain the symbols that are to be encoded
 * @ingroup Models
 * @code {.cpp}
 * // Example usage
 * //  define Symbols to encode
 * std::string symbols = "AABBBCCDDE";
 * 
 * // Create a model for the Symbols
 * FrequencyModel model(symbols);
 * 
 * // The model should now contain each symbol in symbols with a frequency of 1
 * // update the model with the symbols to encode
 * for (char symbol : symbols) {
 *    model.updateModel(symbol);
 * }
 * 
 * // Get the range of a symbol
 * auto range = model.getSymbolRange('A');
 * // range.first is the lower bound of the range
 * // range.second is the upper bound of the range
 * auto& [lower, upper] = range;
 * 
 * // Get the total frequency of the model
 * uint32_t totalFrequency = model.getTotalFrequency();
 * @endcode
 */
class FrequencyModel {
public:
    using symbol_t = char;  ///< Type that represents a symbol

    /**
     * @brief SymbolData struct
     */
    struct SymbolData {
        uint32_t frequency;                     ///< Frequency of the symbol
        std::pair<uint64_t, uint64_t> range;    ///< Range of the symbol
    };

private:
    std::map<char, SymbolData> mFrequencyMap;   ///< Frequency map
    uint32_t mTotalFrequency = 0;               ///< Count of all symbols that were addad to the model
    bool mModelChanged = false;                 ///< Flag to indicate if the model has changed
    bool mIsReset = false;                      ///< Flag to indicate if the model has been reset

public:
    /**
     * @brief Construct a new Frequency Model object
     * @details This constructor will add all the symbols in the span to the model
     * @note This constructor will add the symbols to the model with a frequency of 1
     * @note This constructor will flag the model as changed
     * @note This constructor will flag the model as reset
     * @param symbols the symbols to add to the model
     */
    FrequencyModel(std::span<const char> symbols);

private:
    /**
     * @brief Add a symbol to the model
     * @details This function will add a symbol to the model with a frequency of 1
     * @note This function will flag the model as changed
     * @param symbol the symbol to add
     */
    void addSymbol(symbol_t symbol);

    /**
     * @brief Recalculate the ranges of the symbols
     * @details This function will recalculate the ranges of the symbols
     * @note This function will only recalculate the ranges if the model has changed
     */
    void recalculateSymbolData();

public:
    /**
     * @brief Increment the frequency of a symbol
     * @note This function will flag the model as changed
     * @note This function might flag the model as not reset
     * @note This function will not add the symbol to the model
     * @param symbol the symbol to get the data for
     */
    void updateModel(symbol_t symbol);

    /**
     * @brief Get the range of a symbol
     * @details This function will recalculate the ranges if the model has changed
     * @note This function might lazily evaluate the ranges despite being const.
     *       (the ranges should already count as being recalculated)
     * 
     * @param symbol the symbol to get the range for
     * @return std::pair<uint32_t, uint32_t> the range of the symbol
     */
    std::pair<uint64_t, uint64_t> getSymbolRange(symbol_t symbol) const;

    /**
     * @brief Get the symbol that corresponds to a value
     * @details This function will return the symbol that corresponds to the value
     * @param value the value to get the symbol for
     * @return symbol_t the symbol that corresponds to the value
     */
    symbol_t getSymbol(uint64_t value) const;

    /**
     * @brief Get the Number of Symbols that were added to the model
     * 
     * @return uint32_t the number of symbols
     */
    uint32_t getTotalFrequency() const;

    /**
     * @brief Reset all the frequencies and the total frequency count
     * @details This function will reset the frequency of all symbols in the model to 1
     *          and recount the total frequency count. If the model is already reset
     *          this function will do nothing
     * @note This function will flag the model as changed
     * @note This function will flag the model as reset
     */
    void reset();

    /**
     * @brief Check if the model is in a reset state
     * @details The model is in a reset state if the frequencies of all symbols are 1
     * 
     * @return true if the model has been reset
     * @return false if the model has not been reset
     */
    bool isReset() const;

};

} // namespace KestrelPack