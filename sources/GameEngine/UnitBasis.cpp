#include "UnitBasis.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/SpriterData2D.h>
#include <Urho3D/Core/Variant.h>

namespace GameEngine
{
float UnitBasis::DistanceToGround ()
{
    Urho3D::PhysicsWorld2D *physicsWorld =
            node_->GetScene ()->GetComponent <Urho3D::PhysicsWorld2D> ();
    Urho3D::PODVector <Urho3D::PhysicsRaycastResult2D> results;
    Urho3D::Vector2 playerPosition;
    playerPosition.x_ = node_->GetPosition ().x_;
    playerPosition.y_ = node_->GetPosition ().y_;
    physicsWorld->Raycast (results, playerPosition, playerPosition + Urho3D::Vector2::DOWN * 100);

    float distanceToGround = 999;
    if (!results.Empty ())
        for (int index = 0; index < results.Size (); index++)
        {
            Urho3D::PhysicsRaycastResult2D &result = results.At (index);
            Urho3D::Node *node = result.body_->GetNode ();
            if ((node->GetVar (Constants::ObjectTypeVarHash).GetString () == Constants::ObjectTypes::Wall ||

                 (node->GetVar (Constants::ObjectTypeVarHash).GetString () == Constants::ObjectTypes::Enemy &&
                  node_->GetVar (Constants::ObjectTypeVarHash).GetString () == Constants::ObjectTypes::Player) ||

                 (node->GetVar (Constants::ObjectTypeVarHash).GetString () == Constants::ObjectTypes::Player &&
                  node_->GetVar (Constants::ObjectTypeVarHash).GetString () == Constants::ObjectTypes::Enemy)) &&

                    distanceToGround > result.distance_)

                distanceToGround = result.distance_;
        }
    return distanceToGround;
}

void UnitBasis::Jump ()
{
    Urho3D::RigidBody2D *body = node_->GetComponent <Urho3D::RigidBody2D> ();
    if (DistanceToGround () < 0.1f && timeBeforeNewJump_ < 0.0f)
    {
        body->ApplyForceToCenter (Urho3D::Vector2::UP * jumpForce_, true);
        timeBeforeNewJump_ = 1.0f;
    }
}

void UnitBasis::SetToBlock ()
{
    isAttackingNow_ = false;
    if (blockEfficiency_ > 0.0f)
        isBlockingNow_ = true;
    timeFromAttackStart_ = 0.0f;
}

void UnitBasis::SetToAttack ()
{
    if (!isAttackingNow_)
    {
        timeFromAttackStart_ = 0.0f;
        isAttackExecuted_ = false;
    }
    isAttackingNow_ = true;
    isBlockingNow_ = false;
}

void UnitBasis::SetToMoving ()
{
    isAttackingNow_ = false;
    isBlockingNow_ = false;
    timeFromAttackStart_ = 0.0f;
}

void UnitBasis::UpdateAttack (float timeStep)
{
    Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
    if (sprite->GetAnimation () == "attack")
        timeFromAttackStart_ += timeStep;

    if (timeFromAttackStart_ >= Constants::AttackAnimationTime)
    {
        timeFromAttackStart_ = 0.0f;
        isAttackExecuted_ = false;
    }

    if (timeFromAttackStart_ > Constants::SendAttackDelayAfterStartOfAttackAnimation && !isAttackExecuted_)
    {
        Urho3D::VariantMap eventData;
        Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
        Urho3D::Rect rect;
        Urho3D::Vector2 playerPosition2D;

        playerPosition2D.x_ = node_->GetPosition ().x_;
        playerPosition2D.y_ = node_->GetPosition ().y_;

        if (!sprite->GetFlipX ())
            rect = Urho3D::Rect (playerPosition2D + Urho3D::Vector2::DOWN * 0.5f,
                                 playerPosition2D + Urho3D::Vector2::UP * 2.5f + Urho3D::Vector2::RIGHT * 2.0f);
        else
            rect = Urho3D::Rect (playerPosition2D - Urho3D::Vector2::RIGHT * 2.0 + Urho3D::Vector2::DOWN * 0.5f,
                                 playerPosition2D + Urho3D::Vector2::UP * 2.5f);

        eventData [Constants::AttackInArea::P_AREA_RECT] = rect.ToVector4 ();
        eventData [Constants::AttackInArea::P_ATTACKER_TEAM_ID] = team_;
        eventData [Constants::AttackInArea::P_DAMAGE] = attackDamage_;

        this->SendEvent (Constants::E_ATTACK_IN_AREA, eventData);
        isAttackExecuted_ = true;
    }
}

UnitBasis::UnitBasis (Urho3D::Context *context) : UnitInterface (context)
{
    node_ = 0;
    jumpForce_ = 1600.0f;
    timeBeforeNewJump_ = -1.0f;
    maxSpeed_ = 4.0f;
    lives_ = 100.0f;
    maxLives_ = 100.0f;

    livesRegeneration_ = 3.0f;
    timeFromAttackStart_ = 0.0f;
    attackDamage_ = 10.0f;
    blockEfficiency_ = 1.0f;
    blockEfficiencyRegeneration_ = 0.1f;

    isBlockingNow_ = false;
    isAttackingNow_ = false;
    isAttackExecuted_ = false;

    timeFromLastDamage_ = 0.0f;
    timeFromDie_ = 0.0f;
    dieTime_ = 20.0f;
}

bool UnitBasis::Init ()
{
    return false;
}

bool UnitBasis::Update (float timeStep)
{
    if (lives_ <= 0.0f)
    {
        timeFromDie_ += timeStep;
        if (node_->GetComponent <Urho3D::RigidBody2D> ())
            node_->GetComponent <Urho3D::RigidBody2D> ()->SetEnabled (false);

        if (timeFromDie_ >= dieTime_)
        {
            node_->Remove ();
            isWillBeDeleted_ = true;
        }
    }
    else
    {
        if (lives_ <= maxLives_)
            lives_ += timeStep * livesRegeneration_;

        if (timeBeforeNewJump_ >= 0.0f)
            timeBeforeNewJump_ -= timeStep;

        if (blockEfficiency_ < 1.0f)
            blockEfficiency_ += blockEfficiencyRegeneration_ * timeStep;
    }

    return true;
}

bool UnitBasis::Dispose ()
{
    return false;
}

Urho3D::Vector3 UnitBasis::GetPosition ()
{
    return node_->GetPosition ();
}

bool UnitBasis::IsAttackingNow ()
{
    return isAttackingNow_;
}

bool UnitBasis::IsBlockingNow ()
{
    return isBlockingNow_;
}

bool UnitBasis::OnAttack (Urho3D::StringHash attackerTeam, float damage)
{
    if (ProcessOnAttack (attackerTeam, damage))
    {
        Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
        sprite->SetSpeed (1.0f);
        sprite->SetAnimation ("damage");
        timeFromLastDamage_ = 0.0f;
        return true;
    }
    else
        return false;
}

bool UnitBasis::ProcessOnAttack (Urho3D::StringHash attackerTeam, float damage)
{
    if (team_ != attackerTeam && lives_ >= 0)
    {
        if (isBlockingNow_)
        {
            lives_ -= damage *(1 - blockEfficiency_);
            blockEfficiency_ -= 0.2f;
        }
        else
            lives_ -= damage;

        if (blockEfficiency_ <= 0.0f)
            isBlockingNow_ = false;

        if (!isBlockingNow_ && lives_ > 0.0f)
                return true;
    }
    return false;
}

Urho3D::StringHash UnitBasis::GetTeam ()
{
    return team_;
}

void UnitBasis::SetTeam (Urho3D::StringHash team)
{
    team_ = team;
}

Urho3D::Rect UnitBasis::GetLocalRect ()
{
    return Urho3D::Rect (-0.2f, 2.0f, 0.2f, 0.0f);
}

Urho3D::Node *UnitBasis::GetNode ()
{
    return node_;
}

void UnitBasis::SetNode (Urho3D::Node *node)
{
    node_ = node;
}

void UnitBasis::SetNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    SetNode (FindNode (context_, wayToNode, scene));
}

float UnitBasis::GetAttackDamage ()
{
    return attackDamage_;
}

void UnitBasis::SetAttackDamage (float attackDamage)
{
    attackDamage_ = attackDamage;
}

float UnitBasis::GetMaxSpeed ()
{
    return maxSpeed_;
}

void UnitBasis::SetMaxSpeed (float maxSpeed)
{
    maxSpeed_ = maxSpeed;
}

float UnitBasis::GetJumpForce ()
{
    return jumpForce_;
}

void UnitBasis::SetJumpForce (float jumpForce)
{
    jumpForce_ = jumpForce;
}

float UnitBasis::GetLives ()
{
    return lives_;
}

float UnitBasis::GetMaxLives ()
{
    return maxLives_;
}

float UnitBasis::GetLivesRegeneration ()
{
    return livesRegeneration_;
}

float UnitBasis::GetBlockEfficiency ()
{
    return blockEfficiency_;
}

float UnitBasis::GetBlockEfficiencyRegeneration ()
{
    return blockEfficiencyRegeneration_;
}

UnitBasis::~UnitBasis ()
{
    Dispose ();
}
}

