#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <Weapon.h>
#include <Armor.h>
#include <Enemy.h>
#include <Random.h>

/**
 * @brief Static struct for storing and obtaining predefined game (armor, weapon, enemies) data.
 * 
 */
struct GameData
{
    struct WeaponModifier
    {
        std::string adj;
        std::function<Weapon(Weapon)> modifier;
    };

    template <typename T>
    struct Data
    {
        int level;
        T data;
    };

    static const std::unordered_map<std::string, Data<Weapon>> weapons;
    static const std::unordered_map<std::string, Data<Armor>> armors;
    static const std::unordered_map<std::string, Data<Enemy>> enemies;

    static const std::vector<WeaponModifier> weapon_mods;

    /**
     * @brief Get a list of weapon names
     * 
     * @param level The level player is in
     * @return std::vector<std::string> The list of weapon names
     */
    static std::vector<std::string> GetWeapons(int level);

    /**
     * @brief Get a list of armor names
     * 
     * @param level The level player is in
     * @return std::vector<std::string> The list of armor names
     */
    static std::vector<std::string> GetArmors(int level);

    /**
     * @brief Get a list of enemy names
     * 
     * @param level The level player is in
     * @return std::vector<std::string> The list of enemy names
     */
    static std::vector<std::string> GetEnemies(int level);

    /**
     * @brief Get the weapon by its name scaled with the level
     * 
     * @param name The name of the weapon
     * @param level The level player is in
     * @param rng The random number generator
     * @return const Weapon& The weapon
     */
    static Weapon GetWeapon(const std::string& name, int level, Random& rng);

    /**
     * @brief Get the armor by its name scaled with the level
     * 
     * @param name The name of the armor
     * @param level The level player is in
     * @param rng The random number generator
     * @return const Armor& The armor
     */
    static Armor GetArmor(const std::string& name, int level, Random& rng);

    /**
     * @brief Get the enemy by its name scaled with the level
     * 
     * @param name The name of the enemy
     * @param level The level player is in
     * @param rng The random number generator
     * @return const Enemy& The enemy
     */
    static Enemy GetEnemy(const std::string& name, int level, Random& rng);
};