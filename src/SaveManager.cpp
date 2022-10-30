#include <SaveManager.h>

#include <iostream>
#include <fstream>
#include <Serial.h>

std::string SaveManager::save_file_path;

void SaveManager::Init(const std::string& exe_path)
{
    auto dir_path = exe_path.substr(0, exe_path.find_last_of("/\\"));
    save_file_path = dir_path + "/saves.dat";
}

std::vector<std::string> SaveManager::Entries()
{
    std::vector<std::string> entries;

    auto saves = LoadAll();
    for (auto& entry : saves)
    {
        entries.push_back(entry.first);
    }

    return entries;
}

void SaveManager::Save(const std::string& name, const SaveData& save)
{
    auto saves = LoadAll();
    saves[name] = save;

    std::ofstream outfile(save_file_path, std::ios_base::binary | std::ios::trunc);
    
    if (!outfile.good())
    {
        throw std::runtime_error("Cannot open file `saves`");
    }

    outfile << Serial<std::unordered_map<std::string, SaveData>>::Serialize(saves).str();
}

SaveData SaveManager::Load(const std::string& name)
{
    auto saves = LoadAll();
    if (saves.find(name) == saves.end())
    {
        throw std::runtime_error("No save with name \"" + name + "\" found");
    }

    return saves[name];
}

std::unordered_map<std::string, SaveData> SaveManager::LoadAll()
{
    std::ifstream infile(save_file_path, std::ios_base::binary);
    if (!infile.good())
    {
        infile.close();

        std::ofstream outfile(save_file_path, std::ios_base::binary);
        outfile.close();

        infile.open(save_file_path, std::ios_base::binary);
    }

    if (!infile.good())
    {
        throw std::runtime_error("Cannot open file `saves`");
    }

    if (infile.peek() == std::ifstream::traits_type::eof())
    {
        infile.close();
        return {};
    }

    return Serial<std::unordered_map<std::string, SaveData>>::Deserialize(infile);
}