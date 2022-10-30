#include <Random.h>
#include <Serial.h>

/**
 * @brief Construct a new Random:: Random object
 * 
 * @param seed Random seed generated with random device
 */
Random::Random()
{
    seed = std::random_device()();
    rng.seed(seed);
}

/**
 * @brief Construct a new Random:: Random object
 * 
 * @param seed Putting seed parameter into Random object
 */
Random::Random(uint32_t seed)
{
    this->seed = seed;
    rng.seed(seed);
}
/**
 * @brief Set seed for Random library functions to run
 * 
 * @param seed 
 */
void Random::SetSeed(uint32_t seed)
{
    this->seed = seed;
    rng.seed(seed);
}

/**
 * @brief Get seed
 * 
 * @return uint32_t 
 */
uint32_t Random::GetSeed() const
{
    return seed;
}

/**
 * @brief to return a int value 
 * 
 * @param lower 
 * @param upper 
 * @return int in range of [lower, upper]
 */

int Random::GetInt(int lower, int upper)
{   
    std::uniform_int_distribution<>dist(lower, upper);
    return dist(rng);
}

/**
 * @brief return a float value 
 * 
 * @param lower 
 * @param upper 
 * @return double in range of (lower, upper]
 */
double Random::GetFloat(double lower, double upper)
{
    std::uniform_real_distribution<double>dist(lower, upper);
    return dist(rng);
}

/**
 * @brief Overloading  << Operator
 * 
 * @param os 
 * @return std::ostream& 
 */

std::ostream& Random::operator<<(std::ostream& os) const
{
    os << rng;
    return os;
}

/**
 * @brief Overloading >> Operatior
 * 
 * @param is 
 * @return std::istream& 
 */
std::istream& Random::operator>>(std::istream& is)
{
    is >> rng;
    return is;
}

/**
 * @brief Serialize of seed and rng value
 * 
 * @return std::ostringstream 
 */
std::ostringstream Random::Serialize() const
{
    std::ostringstream rng_ss;
    rng_ss << rng;

    std::ostringstream oss;

    oss
        << Serial<uint32_t>::Serialize(seed).str()
        << Serial<std::string>::Serialize(rng_ss.str()).str();

    return oss;
}

/**
 * @brief To deserialize random objects
 * 
 * @param data 
 */
void Random::Deserialize(std::istream& data)
{
    seed = Serial<uint32_t>::Deserialize(data);
    std::string rng_str = Serial<std::string>::Deserialize(data);
    std::istringstream rng_ss(rng_str);
    rng_ss >> rng;
}