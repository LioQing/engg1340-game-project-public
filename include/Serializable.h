#pragma once

#include <cstdint>
#include <sstream>

/**
 * @brief Interface for serializable data structures
 * 
 */
struct Serializable
{
    virtual ~Serializable() = default;

    /**
     * @brief Serialize the data to an ostringstream
     * 
     * @return std::ostringstream The serialized data
     */
    virtual std::ostringstream Serialize() const = 0;

    /**
     * @brief Deserialize the data from an istringstream
     * 
     * @param data The serialized data
     */
    virtual void Deserialize(std::istream& data) = 0;
};