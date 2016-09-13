#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>
#include "UnitBasis.hpp"

namespace GameEngine
{
class PlayerController : public UnitBasis
{
URHO3D_OBJECT (PlayerController, UnitBasis)
protected:
    float coins_;
    bool isAttackPressed_;
    bool isBlockPressed_;

    Urho3D::Vector2 lastJoystickCoordinates_;
    bool useKeyboardInsteadOfJoystick_;
    void KeyboardToJoystickCoordinates ();
    void UpdateAnimation ();
public:
    PlayerController (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    bool IsUseKeyboardInsteadOfJoystick ();
    void SetIsUseKeyboardInsteadOfJoystick (bool useKeyboardInsteadOfJoystick);
    Urho3D::Vector2 GetLastJoystickCoordinates ();

    bool IsAttackPressed ();
    bool IsBlockPressed ();
    void SetControlls (bool isAttackPressed, bool isBlockPressed, Urho3D::Vector2 lastJoystickCoordinates);
    virtual bool OnAttack (Urho3D::StringHash attackerTeam, float damage);

    float GetCoins ();
    void SetCoins (float coins);
    virtual ~PlayerController ();
};
}
