#include <SaveData.h>

#include <Serial.h>

SaveData::SaveData(const Resources& res, Pause::ResumeType resume_type)
    : res(res), resume_type(resume_type)
{
}

std::ostringstream SaveData::Serialize() const
{
    std::ostringstream oss;

    oss
        << Serial<Resources>::Serialize(res).str()
        << Serial<uint32_t>::Serialize(static_cast<uint32_t>(resume_type)).str();

    return oss;
}

void SaveData::Deserialize(std::istream& data)
{
    res = Serial<Resources>::Deserialize(data);
    resume_type = static_cast<Pause::ResumeType>(Serial<uint32_t>::Deserialize(data));
}