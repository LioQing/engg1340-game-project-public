#include <GameData.h>

#include <algorithm>

/**
 * @brief Weapons
 * format:
 * { level, Weapon{ name, {
 *     Attack{ atk name, base dmg, dmg die, base hit, hit die },
 *     Attack{ atk name, base dmg, dmg die, base hit, hit die },
 * } } },
 * 
 */
const std::unordered_map<std::string, GameData::Data<Weapon>> GameData::weapons =
{
    {
        "Wooden Stick",
        { 1, Weapon{ "Wooden Stick", {
            Attack{ "Heavy Hit", 6, 4, 1, 4 },
            Attack{ "Light Hit", 5, 3, 3, 6 },
        } } },
    },
    {
        "Bare Hand",
        { 1, Weapon{ "Bare Hand", {
            Attack{ "Punch", 3, 3, 4, 2 },
            Attack{ "Heavy Punch", 5, 5, 2, 2 },
        } } },
    },
    {
        "Amylase",
        { 1, Weapon{ "Amylase", {
            Attack{ "Dissolve", 7, 5, 0, 5 },
            Attack{ "Absorb", 4, 3, 5, 3 },
        } } },
    },
    {
        "Maltase",
        { 2, Weapon{ "Maltase", {
            Attack{ "Deadly Hit", 9, 4, 1, 6 },
            Attack{ "Bind", 2, 4, 6, 5 },
        } } },
    },
    {
        "Sucrase",
        { 3, Weapon{ "Sucrase", {
            Attack{ "Accurate Shot", 5, 3, 5, 2 },
            Attack{ "Tag", 2, 2, 8, 2 },
        } } },
    },
    {
        "Lactase",
        { 4, Weapon{ "Lactase", {
            Attack{ "Lucky Throughput", 3, 14, 5, 3 },
            Attack{ "Consistent Damage", 5, 5, 5, 5 },
        } } },
    },
};

/**
 * @brief Armors
 * format:
 * { level, Armor{ name, def, agi } },
 * 
 */
const std::unordered_map<std::string, GameData::Data<Armor>> GameData::armors =
{
    { "Zion's Feathers", { 1, Armor{ "Zion's Feathers", 2, 2 } } },
    { "Heavy Feathers", { 1, Armor{ "Heavy Feathers", 3, 2 } } },
    { "Light Feathers", { 1, Armor{ "Light Feathers", 2, 3 } } },
    { "Marshmallow's Squishy Armor", { 1, Armor{ "Marshmallow's Squishy Armor", 4, 1 } } },
    { "Hard Candy Armor", { 2, Armor{ "Hard Candy Armor", 6, 0 } } },
    { "Donut Armor", { 3, Armor{ "Donut Armor", 3, 4 } } },
    { "Lollipop Armor", { 4, Armor{ "Lollipop Armor", 7, 3 } } },
};

/**
 * @brief Enemies
 * format:
 * { level, Enemy{ name, hp, spd,
 *     Armor{ "", def, agi },
 *     Attack{ "", base dmg, dmg die, base hit, hit die },
 * } }
 */
const std::unordered_map<std::string, GameData::Data<Enemy>> GameData::enemies =
{
    {
        "Marshmallow",
        { 1, Enemy{ "Marshmallow", 15, 5,
            Armor{ "", 2, 2 },
            Attack{ "", 6, 2, 2, 4 },
        } },
    },
    {
        "Soft Candy",
        { 1, Enemy{ "Soft Candy", 10, 7,
            Armor{ "", 1, 3 },
            Attack{ "", 4, 3, 3, 1 },
        } },
    },
    {
        "Hard Candy",
        { 2, Enemy{ "Hard Candy", 18, 4,
            Armor{ "", 3, 2 },
            Attack{ "", 7, 3, 1, 4 },
        } },
    },
    {
        "Donut",
        { 3, Enemy{ "Donut", 17, 6,
            Armor{ "", 1, 4 },
            Attack{ "", 6, 5, 2, 5 },
        } },
    },
    {
        "Lollipop",
        { 4, Enemy{ "Lollipop", 18, 4,
            Armor{ "", 3, 2 },
            Attack{ "", 10, 2, 3, 4 },
        } },
    },
    {
        "Sugar",
        { 5, Enemy{ "Sugar", 20, 8,
            Armor{ "", 5, 2 },
            Attack{ "", 10, 3, 3, 7 },
        } },
    },
};

const std::vector<GameData::WeaponModifier> GameData::weapon_mods =
{
    {
        "Sharp",
        [](Weapon w)
        {
            w.attacks[0].base_dmg += 1;
            w.attacks[1].base_dmg += 1;
            return w;
        },
    },
    {
        "Reliable",
        [](Weapon w)
        {
            w.attacks[0].base_hit += 1;
            w.attacks[1].base_hit += 1;
            return w;
        },
    },
    {
        "Blunt",
        [](Weapon w)
        {
            w.attacks[0].dmg_die += 2;
            w.attacks[1].dmg_die += 2;
            return w;
        },
    },
    {
        "Accurate",
        [](Weapon w)
        {
            w.attacks[0].hit_die += 2;
            w.attacks[1].hit_die += 2;
            return w;
        },
    },
    {
        "Stable",
        [](Weapon w)
        {
            for (auto& atk : w.attacks)
            {
                auto dmg = atk.dmg_die / 4;
                atk.dmg_die -= dmg;
                atk.base_dmg += dmg + 1;

                auto hit = atk.hit_die / 4;
                atk.hit_die -= hit;
                atk.base_hit += hit + 1;
            }
            return w;
        },
    },
    {
        "Unstable",
        [](Weapon w)
        {
            for (auto& atk : w.attacks)
            {
                auto dmg = atk.base_dmg / 4;
                atk.dmg_die += dmg + 1;
                atk.base_dmg -= dmg;

                auto hit = atk.base_hit / 4;
                atk.hit_die += hit + 1;
                atk.base_hit -= hit;
            }
            return w;
        },
    },
};

std::vector<std::string> GameData::GetWeapons(int level)
{
    std::vector<std::string> weapon_names;

    for (auto& [name, data] : weapons)
    {
        if (data.level <= level)
        {
            weapon_names.push_back(name);
        }
    }

    return weapon_names;
}

std::vector<std::string> GameData::GetArmors(int level)
{
    std::vector<std::string> armor_names;

    for (auto& [name, data] : armors)
    {
        if (data.level <= level)
        {
            armor_names.push_back(name);
        }
    }

    return armor_names;
}

std::vector<std::string> GameData::GetEnemies(int level)
{
    std::vector<std::string> enemy_names;

    for (auto& [name, data] : enemies)
    {
        if (data.level <= level)
        {
            enemy_names.push_back(name);
        }
    }

    return enemy_names;
}

Weapon GameData::GetWeapon(const std::string& name, int level, Random& rng)
{
    // scaling function
    constexpr auto scale = [](const Data<Weapon>& d, int level)
    {
        auto w = d.data;
        auto l = level - d.level;

        for (auto& atk : w.attacks)
        {
            atk.base_dmg += l * 2;
            atk.dmg_die += l * 1.2;
            atk.base_hit += l * 2;
            atk.hit_die += l * 1.2;
        }
        return w;
    };

    constexpr auto idx_mod = [](Attack& atk, int idx, int mod)
    {
        switch (idx)
        {
            case 0: atk.base_dmg += mod;    return;
            case 1: atk.dmg_die += mod;     return;
            case 2: atk.base_hit += mod;    return;
            case 3: atk.hit_die += mod;     return;
            default:                        return;
        }
    };

    auto weapon = scale(weapons.at(name), level);

    for (auto& atk : weapon.attacks)
    {
        idx_mod(atk, rng.GetInt(0, 5), 1);  // buff one stat
        idx_mod(atk, rng.GetInt(0, 5), -1); // nerf one stat
    }

    // weapon modifiers
    auto mod_idx = rng.GetInt(0, weapon_mods.size());
    if (static_cast<size_t>(mod_idx) < weapon_mods.size())
    {
        auto mod = weapon_mods.at(mod_idx);
        weapon = mod.modifier(weapon);
        weapon.name = mod.adj + " " + weapon.name;
    }

    return weapon;
}

Armor GameData::GetArmor(const std::string& name, int level, Random& rng)
{
    // scaling function
    constexpr auto scale = [](const Data<Armor>& d, int level)
    {
        auto a = d.data;
        auto l = level - d.level;

        a.defence += l * 1;
        a.agility += l * .5;

        return a;
    };

    auto armor = scale(armors.at(name), level);

    return armor;
}

Enemy GameData::GetEnemy(const std::string& name, int level, Random& rng)
{
    // scaling function
    constexpr auto scale = [](const Data<Enemy>& d, int level)
    {
        auto e = d.data;
        auto l = level - d.level;

        e.hp += l * 1;
        e.max_hp += l * 1;
        e.speed += l * 1;
        e.armor.defence += l * .5;
        e.armor.agility += l * .5;
        e.attack.base_dmg += l * 1;
        e.attack.dmg_die += l * .5;
        e.attack.base_hit += l * 1;
        e.attack.hit_die += l * .5;

        return e;
    };

    auto enemy = scale(enemies.at(name), level);

    return enemy;
}