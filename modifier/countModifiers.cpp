/*
Напишите консольную программу, которая будет считать модифицированные характеристики предметов.

Есть инвентарь, в котором хранится несколько предметов разного свойства, которые имеют общие характеристики (
название, тип, уровень (целое число), редкость ( Common, Rare, Epic ) )
и числовые <double> характеристики, специфичные для каждого типа ( например, урон у оружия или уровень защиты у
брони ).

Также есть модификаторы (Buffs), которые имеют фильтры по характеристикам предметов. Например "уровень больше N"
или "редкость равна Rare". Фильтров может быть несколько.

Программа должна заполнять инвентарь, создавать модификаторы и выводить на экран значение модифицированных
характеристик для всех предметов.

Входные данные:
Инвентарь:
{ident: "axe_01", type: Melee, level: 3, rarity: Common, damage: 12.0, speed: 0.9 },
{ident: "revolver_01", type: Range, level: 5, rarity: Common, damage: 42.0, speed: 2.0 },
{ident: "revolver_02", type: Range, level: 3, rarity: Rare, damage: 50.0, speed: 2.1 },
{ident: "machinegun_01", type: Range, level: 5, rarity: Epic, damage: 83.1, speed: 10.0 },
{ident: "jacket_01", type: Armour, level: 2, rarity: Common, protection: 2.0 },
{ident: "bulletprof_vest_01", type: Armour, level: 5, rarity: Rare, protection: 30.0 }
Модификаторы:
{ ident: "eagle_eye", filters: [{"level", ">=", 4}, { "type", Range}], type: DamageBuff, value: 10 },
{ ident: "donatium_steel", filters: [{"rarity", Rare}], type: ProtectionBuff, value: 15 },
{ ident: "rage_drink", filters: [{"type", Range}, {"type", Melee}], type: SpeedBuff, value: 0.4 }

P.S. Рекомендуется использовать C++17 или новее
*/

#include "inventoryUnit.h"

#include <json/json.h>

#include <iostream>
#include <fstream>


typedef std::vector<InventoryUnit> Inventory;


int main() {

    Json::Value root;
    std::string filename = "../sample-input.json";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "An error occured while opening file\n";
        return 0;
    }

    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

    if (!parseFromStream(builder, file, &root, &errs)) {
        std::cout << errs << std::endl;
        return -1;
    }

    const auto& json_inventory = root["inventory"];
    Inventory inventory;

    for(auto& json_val : json_inventory)
        inventory.emplace_back(InventoryUnit(json_val));

    const auto& json_modifiers = root["modifiers"];

    for (auto& unit : inventory) {
        unit.applyModifiers(json_modifiers);
        unit.printUnitData();
    }

    return 0;
}