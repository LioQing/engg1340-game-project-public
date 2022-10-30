#pragma once

#include <Resources.h>
#include <Pause.h>
#include <Serializable.h>

struct SaveData : Serializable
{
    Resources res;
    Pause::ResumeType resume_type;

    SaveData() = default;
    SaveData(const SaveData&) = default;
    SaveData(const Resources& res, Pause::ResumeType resume_type);
    
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
};