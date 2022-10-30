#pragma once

#include <cstdint>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <array>
#include <type_traits>
#include <Serializable.h>

template <typename T>
struct Serial
{
    /**
     * @brief Serialize a value type data structure
     * 
     * @tparam T The value type
     * @param i The value to serialize
     * @return std::ostringstream The serialized data
     */
    static std::ostringstream Serialize(const T& i)
    {
        if constexpr (std::is_base_of_v<Serializable, T>)
        {
            return i.Serialize();
        }

        std::ostringstream oss;
        oss.write(reinterpret_cast<const char*>(&i), sizeof(i));
        return oss;
    }

    /**
     * @brief Deserialize a value type data structure
     * 
     * @tparam T The value type
     * @param data The serialized data
     * @return T The deserialized value
     */
    static T Deserialize(std::istream& data)
    {
        if constexpr (std::is_base_of_v<Serializable, T>)
        {
            T i;
            i.Deserialize(data);
            return i;
        }

        T i;
        data.read(reinterpret_cast<char*>(&i), sizeof(i));
        if (data.gcount() < static_cast<std::streamsize>(sizeof(i)))
        {
            throw std::runtime_error("Corrupted serialized data");
        }
        return i;
    }
};

template <>
struct Serial<std::string>
{
    /**
     * @brief Serialize a string
     * 
     * @param str The string to serialize
     * @return std::ostringstream The serialized data
     */
    static std::ostringstream Serialize(const std::string& str)
    {
        std::ostringstream oss;

        uint32_t size = static_cast<uint32_t>(str.size());
        oss.write(reinterpret_cast<char*>(&size), sizeof(size));
        oss.write(str.c_str(), size);

        return oss;
    }

    /**
     * @brief Deserialize a string
     * 
     * @param data The serialized data
     * @return std::string The deserialized string
     */
    static std::string Deserialize(std::istream& data)
    {
        uint32_t size;
        data.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (data.gcount() < static_cast<std::streamsize>(sizeof(size)))
        {
            throw std::runtime_error("Corrupted serialized data");
        }

        char* str_buffer = new char[size + 1]();
        data.read(str_buffer, size);
        if (data.gcount() < static_cast<std::streamsize>(size))
        {
            throw std::runtime_error("Corrupted serialized data");
        }

        std::string str = str_buffer;
        delete[] str_buffer;

        return str;
    }
};

template <typename K, typename V, typename H>
struct Serial<std::unordered_map<K, V, H>>
{
    /**
     * @brief Serialize a unordered_map
     * 
     * @param map The unordered_map to serialize
     * @return std::ostringstream The serialized data
     */
    static std::ostringstream Serialize(const std::unordered_map<K, V, H>& map)
    {
        std::ostringstream oss;

        uint32_t size = static_cast<uint32_t>(map.size());
        oss.write(reinterpret_cast<char*>(&size), sizeof(size));

        for (const auto& [key, value] : map)
        {
            oss << Serial<K>::Serialize(key).str();
            oss << Serial<V>::Serialize(value).str();
        }

        return oss;
    }

    /**
     * @brief Deserialize a unordered_map
     * 
     * @param data The serialized data
     * @return std::unordered_map<K, V> The deserialized string
     */
    static std::unordered_map<K, V, H> Deserialize(std::istream& data)
    {
        std::unordered_map<K, V, H> map;

        uint32_t size;
        data.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (data.gcount() < static_cast<std::streamsize>(sizeof(size)))
        {
            throw std::runtime_error("Corrupted serialized data");
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            K key = Serial<K>::Deserialize(data);
            V value = Serial<V>::Deserialize(data);
            map.emplace(key, value);
        }

        return map;
    }
};

template <typename T>
struct Serial<std::vector<T>>
{
    /**
     * @brief Serialize a vector
     * 
     * @param vec The vector to serialize
     * @return std::ostringstream The serialized data
     */
    static std::ostringstream Serialize(const std::vector<T>& vec)
    {
        std::ostringstream oss;

        uint32_t size = static_cast<uint32_t>(vec.size());
        oss.write(reinterpret_cast<char*>(&size), sizeof(size));

        for (const auto& e : vec)
        {
            oss << Serial<T>::Serialize(e).str();
        }

        return oss;
    }

    /**
     * @brief Deserialize a vector
     * 
     * @param data The serialized data
     * @return std::vector<T> The deserialized vector
     */
    static std::vector<T> Deserialize(std::istream& data)
    {
        std::vector<T> vec;

        uint32_t size;
        data.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (data.gcount() < static_cast<std::streamsize>(sizeof(size)))
        {
            throw std::runtime_error("Corrupted serialized data");
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            T e = Serial<T>::Deserialize(data);
            vec.emplace_back(e);
        }

        return vec;
    }
};

template <typename T, size_t N>
struct Serial<std::array<T, N>>
{
    /**
     * @brief Serialize an array
     * 
     * @param arr The array to serialize
     * @return std::ostringstream The serialized data
     */
    static std::ostringstream Serialize(const std::array<T, N>& arr)
    {
        std::ostringstream oss;

        uint32_t size = static_cast<uint32_t>(arr.size());
        oss.write(reinterpret_cast<char*>(&size), sizeof(size));

        for (const auto& e : arr)
        {
            oss << Serial<T>::Serialize(e).str();
        }

        return oss;
    }

    /**
     * @brief Deserialize a array
     * 
     * @param data The serialized data
     * @return std::array<T, N> The deserialized array
     */
    static std::array<T, N> Deserialize(std::istream& data)
    {
        std::array<T, N> arr;

        uint32_t size;
        data.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (data.gcount() < static_cast<std::streamsize>(sizeof(size)))
        {
            throw std::runtime_error("Corrupted serialized data");
        }

        if (size != N)
        {
            throw std::runtime_error("Array size mismatch");
        }

        for (uint32_t i = 0; i < size; ++i)
        {
            T e = Serial<T>::Deserialize(data);
            arr.at(i) = e;
        }

        return arr;
    }
};