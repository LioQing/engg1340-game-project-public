#pragma once

#include <cstdint>
#include <random>
#include <iostream>
#include <Serializable.h>

class Random : Serializable
{
    public:

    Random();

    Random(uint32_t seed);

    void SetSeed(uint32_t seed);

    uint32_t GetSeed() const;

    int GetInt(int lower, int upper);

    double GetFloat(double lower, double upper);

    std::ostream& operator<<(std::ostream& os) const;

    std::istream& operator>>(std::istream& is);
    
    /**
     * @brief Serialize the data to a stream
     * 
     * @return std::ostream The serialized data
     */
    std::ostringstream Serialize() const override;

    /**
     * @brief Deserialize the data from an istream
     * 
     * @param data The serialized data
     */
    void Deserialize(std::istream& data) override;
    
    private:

    uint32_t seed;
    std::mt19937 rng;
};