#include "inventoryUnit.h"

#include "json/json.h"

#include <iostream>

const std::map<std::string, InventoryUnitType> typeMap {
    { "Melee", InventoryUnitType::Melee },
    { "Range", InventoryUnitType::Range },
    { "Armour", InventoryUnitType::Armour },
};

const std::map<std::string, InventoryUnitRarity> rarityMap {
    { "Common", InventoryUnitRarity::Common },
    { "Rare", InventoryUnitRarity::Rare },
    { "Epic", InventoryUnitRarity::Epic },
};

const std::string strDamage = "damage";
const std::string strSpeed = "speed";
const std::string strProtection = "protection";
const std::string strDamageBuff = "DamageBuff";
const std::string strSpeedBuff = "SpeedBuff";
const std::string strProtectionBuff = "ProtectionBuff";

std::map<std::string, CharacteristicType> characteristicBuffMap {
    { strDamageBuff, CharacteristicType::Damage },
    { strSpeedBuff, CharacteristicType::Speed },
    { strProtectionBuff, CharacteristicType::Protection },
};


std::map<std::string, CharacteristicType> characteristicMap {
    { strDamage, CharacteristicType::Damage },
    { strSpeed, CharacteristicType::Speed },
    { strProtection, CharacteristicType::Protection },
};


InventoryUnitType strTypeToUnitType(const std::string& str) {
    InventoryUnitType result;

    const auto it = typeMap.find(str);
    if (it != typeMap.end())
        result = it->second;
    else
        result = InventoryUnitType::Other;

    return result;
}


std::string unitTypeToStrType(const InventoryUnitType type) {
    for (const auto& it : typeMap) {
        if (it.second == type)
            return it.first;
    }
}


InventoryUnitRarity strRarityToUnitRarity(const std::string& str) {
    InventoryUnitRarity result;
    const auto it = rarityMap.find(str);
    if (it != rarityMap.end())
        result = it->second;

    return result;
}


std::string unitRarityToStrRarity(const InventoryUnitRarity type) {
    for (const auto& it : rarityMap) {
        if (it.second == type)
            return it.first;
    }
}


std::string characteristicToStr(const CharacteristicType type) {
    for (const auto& it : characteristicMap) {
        if (it.second == type)
            return it.first;
    }
}


InventoryUnit::InventoryUnit(const Json::Value& root) {
    // general
    m_ident = root["ident"].asString();
    m_type = strTypeToUnitType(root["type"].asString());
    m_rarity = strRarityToUnitRarity(root["rarity"].asString());
    m_level = root["level"].asInt();

    // stats
    if(root.isMember(strDamage))
        m_characteristics.push_back({ CharacteristicType::Damage, root[strDamage].asDouble() });
    else
        m_characteristics.push_back({ CharacteristicType::Damage, 0 });
    if(root.isMember(strProtection))
        m_characteristics.push_back({ CharacteristicType::Protection, root[strProtection].asDouble() });
    else
        m_characteristics.push_back({ CharacteristicType::Protection, 0 });
    if(root.isMember(strSpeed))
        m_characteristics.push_back({ CharacteristicType::Speed, root[strSpeed].asDouble() });
    else
        m_characteristics.push_back({ CharacteristicType::Speed, 0 });
}


void InventoryUnit::printUnitData() const {
    std::cout << m_ident << std::endl
              << unitTypeToStrType(m_type) << std::endl
              << "Level: " << m_level << std::endl
              << unitRarityToStrRarity(m_rarity) << std::endl;

    std::map<CharacteristicType, double> bonus;

    for (const auto& mod : m_modifiers) {
        bonus[mod.type] += mod.value;
        std::cout << mod.ident << " modifier\n";
    }

    for(const auto& stat : m_characteristics) {
        if (stat.value + bonus[stat.type] != 0)
            std::cout << characteristicToStr(stat.type) << ": " << stat.value + bonus[stat.type] << std::endl;
    }

    std::cout << std::endl;
}


void InventoryUnit::applyModifiers(const Json::Value& root) {
    for (const auto& mod : root) {
        const auto& filters = mod["filters"];

        bool typeFilter = false;
        bool typeFilterOk = false;

        bool levelFilter = false;
        bool levelFilterOk = false;

        bool rarityFilter = false;
        bool rarityFilterOk = false;

        // check all conditions
        for (const auto& filter : filters) {
            const auto& filterType = filter[0].asString();
            if (filterType == "level") {
                levelFilter = true;
                if (filter[1].asString() == ">=") {
                    if (m_level >= filter[2].asInt())
                        levelFilterOk = true;
                }
                else {
                    // not sure about other level conditions
                }
            }
            else if (filterType == "type") {
                typeFilter = true;
                if (m_type == strTypeToUnitType(filter[1].asString()))
                    typeFilterOk = true;
            }
            else if (filterType == "rarity") {
                rarityFilter = true;
                if (m_rarity == strRarityToUnitRarity(filter[1].asString()))
                    rarityFilterOk = true;
            }

        }
        
        // lets check if we can apply the modifier
        if (typeFilterOk | !typeFilter &&
            levelFilterOk | !levelFilter &&
            rarityFilterOk | !rarityFilter) {
            m_modifiers.push_back({ mod["ident"].asString(),
                                    characteristicBuffMap[mod["type"].asString()],
                                    mod["value"].asDouble()});
        }
    }
}