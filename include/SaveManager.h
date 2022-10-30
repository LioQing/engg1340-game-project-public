#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <Serializable.h>
#include <SaveData.h>
#include <Pause.h>

/**
 * @brief Static class for managing saves
 * 
 */
struct SaveManager
{
    static void Init(const std::string& exe_path);

    static std::vector<std::string> Entries();

    static void Save(const std::string& name, const SaveData& save);

    static SaveData Load(const std::string& name);

private:

    static std::unordered_map<std::string, SaveData> LoadAll();

    static std::string save_file_path;
};