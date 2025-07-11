#ifndef PROTOCOL_CONSTANTS_H
#define PROTOCOL_CONSTANTS_H

#define USERNAME_HEADER 0x01
#define PROTOCOL_HEADER 0x06

#define BINARY_PROTOCOL 0x07
#define TEXT_PROTOCOL 0x08

#define BUY_WEAPON_BINARY 0x02
#define BUY_AMMO_BINARY 0x03
#define LOADOUT_BINARY 0x07

#define PRIMARY_WEAPON_BINARY 0x01
#define SECONDARY_WEAPON_BINARY 0x02

#define KNIFE_EQUIPPED_BINARY 0x01

#define NONE_WEAPON_BINARY 0x00
#define GLOCK_BINARY 0x01
#define AK47_BINARY 0x02
#define M3_BINARY 0x03
#define AWP_BINARY 0x04

#define BUY_TEXT "buy"
#define AMMO_TEXT "ammo"
#define WEAPON_TEXT "weapon"

#define LOADOUT_TEXT "equipment"
#define MONEY_TEXT "money"
#define KNIFE_TEXT "knife"
#define KNIFE_EQUIPPED_TEXT "true"
#define PRIMARY_WEAPON_TEXT "primary"
#define SECONDARY_WEAPON_TEXT "secondary"

#define NONE_WEAPON_TEXT "none"
#define GLOCK_TEXT "glock"
#define AK47_TEXT "ak-47"
#define M3_TEXT "m3"
#define AWP_TEXT "awp"

#define WORD_DELIMITER_TEXT "."
#define VALUE_SEPARATOR_TEXT ":"
#define AMMO_SEPARATOR_TEXT ","
#define LINE_DELIMITER_TEXT '\n'

#endif
