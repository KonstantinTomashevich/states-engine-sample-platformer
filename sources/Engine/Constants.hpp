#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/Str.h>

#define STRING_HASH_CONSTANT(constant_name, constant_value) \
    static const Urho3D::StringHash constant_name (constant_value) \

#define STRING_CONSTANT(constant_name, constant_value) \
    static const Urho3D::String constant_name (constant_value) \

#define FLOAT_CONSTANT(constant_name, constant_value) \
    static const float constant_name = constant_value

namespace Constants
{
STRING_HASH_CONSTANT (OBJECT_TYPE_VAR_HASH, "type");
STRING_HASH_CONSTANT (COIN_CASH_VAR_HASH, "cash");
STRING_HASH_CONSTANT (LEVEL_NAME_VAR_HASH, "levelName");
STRING_HASH_CONSTANT (PLAYER_TEAM_ID, "playerTeam");
STRING_HASH_CONSTANT (ORCS_TEAM_ID, "orcsTeam");

STRING_HASH_CONSTANT (EVENT_ATTACK_IN_AREA, "attackInArea");
namespace AttackInAreaData
{
    STRING_HASH_CONSTANT (AREA_RECT, "area_rect"); /// Urho3D::Vector4.
    STRING_HASH_CONSTANT (ATTACKER_TEAM_ID, "attackerTeamId"); /// Urho3D::StringHash.
    STRING_HASH_CONSTANT (DAMAGE, "damage"); /// float.
}

STRING_CONSTANT (OBJECT_TYPE_PLAYER, "player");
STRING_CONSTANT (OBJECT_TYPE_COIN, "coin");
STRING_CONSTANT (OBJECT_TYPE_ENEMY, "enemy");
STRING_CONSTANT (OBJECT_TYPE_WALL, "wall");

FLOAT_CONSTANT (PLAYER_ATTACK_ANIMATION_TIME, 0.45f);
FLOAT_CONSTANT (DAMAGE_ANIMATION_TIME, 0.2f);
FLOAT_CONSTANT (PLAYER_ATTACK_TIME_IN_ANIMATION, 0.15f);
}
