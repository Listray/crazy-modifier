#pragma once

#include <string>
#include <list>

// forward declaration
namespace Json {
    class Value;
};

enum InventoryUnitRarity {
    Common, 
    Rare,
    Epic
};


enum InventoryUnitType {
    Melee,
    Range,
    Armour,
    Other
};


enum CharacteristicType {
    Damage,
    Protection,
    Speed
};

struct Characteristic {
    CharacteristicType type;
    double value;
};


struct Modifier {
    std::string ident;
    CharacteristicType type;
    double value;
};

class InventoryUnit {
public:
    InventoryUnit(const Json::Value& root);
    ~InventoryUnit() {};

    void printUnitData() const;
    void applyModifiers(const Json::Value& root);

private:
    std::string m_ident;
    InventoryUnitType m_type;
    InventoryUnitRarity m_rarity;
    uint16_t m_level; // there is no level limits in the task so let it be uint16
    std::list<Characteristic> m_characteristics;
    std::list<Modifier> m_modifiers;
};