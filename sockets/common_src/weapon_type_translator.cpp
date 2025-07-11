#include "weapon_type_translator.h"

#include <algorithm>
#include <stdexcept>

#include "protocol_constants.h"

WeaponTypeTranslator::WeaponTypeTranslator():
        weapon_type_codes({
                {WeaponType::None, NONE_WEAPON_BINARY},
                {WeaponType::Glock, GLOCK_BINARY},
                {WeaponType::AK47, AK47_BINARY},
                {WeaponType::M3, M3_BINARY},
                {WeaponType::AWP, AWP_BINARY},
        }),
        weapon_type_strings({
                {WeaponType::None, NONE_WEAPON_TEXT},
                {WeaponType::Glock, GLOCK_TEXT},
                {WeaponType::AK47, AK47_TEXT},
                {WeaponType::M3, M3_TEXT},
                {WeaponType::AWP, AWP_TEXT},
        }) {}

const uint8_t WeaponTypeTranslator::get_code_from_weapon(const WeaponType& weapon) const {

    return this->weapon_type_codes.at(weapon);
}

const WeaponType WeaponTypeTranslator::get_weapon_from_code(const uint8_t& weapon_code) const {

    auto matches_weapon_code = [&weapon_code](const auto& pair) {
        return pair.second == weapon_code;
    };

    auto found_weapon = std::find_if(this->weapon_type_codes.begin(), this->weapon_type_codes.end(),
                                     matches_weapon_code);

    if (found_weapon != this->weapon_type_codes.end()) {
        return found_weapon->first;
    } else {
        throw std::out_of_range("Weapon code not found: " + weapon_code);
    }
}

const std::string WeaponTypeTranslator::get_string_from_weapon(const WeaponType& weapon) const {

    return this->weapon_type_strings.at(weapon);
}

const WeaponType WeaponTypeTranslator::get_weapon_from_string(
        const std::string& weapon_string) const {

    auto matches_weapon_string = [&weapon_string](const auto& pair) {
        return pair.second == weapon_string;
    };

    auto found_weapon = std::find_if(this->weapon_type_strings.begin(),
                                     this->weapon_type_strings.end(), matches_weapon_string);

    if (found_weapon != this->weapon_type_strings.end()) {
        return found_weapon->first;
    } else {
        throw std::out_of_range("Weapon not found: " + weapon_string);
    }
}

WeaponTypeTranslator::~WeaponTypeTranslator() {}
