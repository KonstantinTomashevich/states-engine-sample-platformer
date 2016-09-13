#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Core/Object.h>

namespace GameEngine
{
namespace Constants
{
const Urho3D::StringHash ObjectTypeVarHash ("type");
const Urho3D::StringHash CoinCashVarHash ("cash");

const Urho3D::StringHash PlayerTeamId ("playerTeam");
const Urho3D::StringHash OrcsTeamId ("orcsTeam");

URHO3D_EVENT (E_ATTACK_IN_AREA, AttackInArea)
{
    /// Urho3D::Vector4.
    URHO3D_PARAM (P_AREA_RECT, AreaRect);
    /// Urho3D::StringHash.
    URHO3D_PARAM (P_ATTACKER_TEAM_ID, AttackerTeamId);
    // float.
    URHO3D_PARAM (P_DAMAGE, Damage);
}

namespace ObjectTypes
{
const Urho3D::String Player ("player");
const Urho3D::String Coin ("coin");
const Urho3D::String Enemy ("enemy");
const Urho3D::String Wall ("wall");
}

const float PlayerAttackAnimationTime = 0.45f;
const float DamageAnimationTime = 0.2f;
const float PlayerSendAttackDelayAfterStartOfAttackAnimation = 0.15f;
}
}
