#include "WarriorAi.hpp"
#include "Utils.hpp"
#include "Constants.hpp"
#include "UnitInterface.hpp"

#include <Urho3D/Input/Input.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/SpriterData2D.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Math/MathDefs.h>
#include <StatesEngine/StatesEngine.hpp>

namespace GameEngine
{
void WarriorAi::UpdateAnimation ()
{
    Urho3D::RigidBody2D *body = node_->GetComponent <Urho3D::RigidBody2D> ();
    Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
    if (Urho3D::Abs (body->GetLinearVelocity ().x_) > maxSpeed_ / 20.0f)
        sprite->SetFlipX (body->GetLinearVelocity ().x_ < 0.0f);

    if (lives_ >= 0.0f)
    {
        if (timeFromLastDamage_ >= Constants::DAMAGE_ANIMATION_TIME)
        {
            if (isBlockingNow_)
            {
                sprite->SetSpeed (1.0f);
                sprite->SetAnimation ("block");
            }
            else if (isAttackingNow_)
            {
                sprite->SetSpeed (1.0f);
                sprite->SetAnimation ("attack");
            }
            else if (Urho3D::Abs (body->GetLinearVelocity ().x_) > maxSpeed_ / 20.0f)
            {
                sprite->SetAnimation ("walk");
                sprite->SetSpeed (Urho3D::Abs (body->GetLinearVelocity ().x_) / maxSpeed_);
            }
            else
            {
                sprite->SetAnimation ("stay");
                sprite->SetSpeed (1.0f);
            }
        }
    }
    else
    {
        if (sprite->GetAnimation () != "death")
        {
            sprite->SetSpeed (1.0f);
            sprite->SetAnimation ("death");
        }
    }
}

UnitInterface *WarriorAi::GetNearestEnemy ()
{
    StatesEngine::StatesEngineSubsystem *statesEngine = context_->GetSubsystem <StatesEngine::StatesEngineSubsystem> ();
    Urho3D::Vector <Urho3D::SharedPtr <UnitInterface> > units = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->GetAll <UnitInterface> ();

    if (!units.Empty ())
    {
        Urho3D::SharedPtr <UnitInterface> nearest;
        float nearestDistance = 999999;
        for (int index = 0; index < units.Size (); index++)
        {
            Urho3D::SharedPtr <UnitInterface> unit = units.At (index);
            if (unit && unit != this && unit->GetTeam () != this->GetTeam () &&
                    unit->GetLives () >= 0.0f)
            {
                float distance = (unit->GetPosition () - this->GetPosition ()).Length ();
                if (distance < nearestDistance)
                {
                    nearestDistance = distance;
                    nearest = unit;
                }
            }
        }

        if (nearestDistance <= scanningRadius_)
            return nearest;
        else
            return 0;
    }
    else
        return 0;
}

void WarriorAi::UpdateAi (UnitInterface *enemy, float timeStep)
{
    Urho3D::RigidBody2D *body = node_->GetComponent <Urho3D::RigidBody2D> ();
    Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();

    if ((enemy->GetPosition () - this->GetPosition ()).Length () > 2.0f)
    {
        timeFromLastDecinision_ = 999.0f;
        SetToMoving ();

        if (Urho3D::Abs (enemy->GetPosition ().x_ - this->GetPosition ().x_) > 2.0f)
        {
            if (enemy->GetPosition ().x_ > this->GetPosition ().x_)
                body->SetLinearVelocity (
                            Urho3D::Vector2 (maxSpeed_, body->GetLinearVelocity ().y_));
            else
                body->SetLinearVelocity (
                            Urho3D::Vector2 (-maxSpeed_, body->GetLinearVelocity ().y_));
        }

        Urho3D::TileMapLayer2D *wallsLayer = FindWallsLayer ();
        if (wallsLayer)
        {
            Urho3D::IntVector2 currentPosition;
            wallsLayer->GetTileMap ()->PositionToTileIndex (
                        currentPosition.x_, currentPosition.y_,
                        Urho3D::Vector2 (node_->GetPosition ().x_,
                                         node_->GetPosition ().y_ + 0.5f));
            int offset;
            if (sprite->GetFlipX ())
                offset = -1;
            else
                offset = 1;

            if (!IsTileEmpty (currentPosition.x_ + offset, currentPosition.y_, wallsLayer) ||
                    (IsTileEmpty (currentPosition.x_ + offset, currentPosition.y_ - 1, wallsLayer) &&
                     IsTileEmpty (currentPosition.x_ + offset, currentPosition.y_ - 2, wallsLayer) &&
                     IsTileEmpty (currentPosition.x_ + offset, currentPosition.y_ - 3, wallsLayer)))
                Jump ();
        }
    }
    else
    {
        body->SetLinearVelocity (Urho3D::Vector2 (0, body->GetLinearVelocity ().y_));
        if (enemy->GetPosition ().x_ > this->GetPosition ().x_)
            sprite->SetFlipX (false);
        else
            sprite->SetFlipX (true);

        timeFromLastDecinision_ += timeStep;
        if (timeFromLastDecinision_ > newDecinisionTime_)
        {
            timeFromLastDecinision_ = 0.0f;
            if (Urho3D::Random () >= 0.5f)
                SetToAttack ();
            else
                SetToBlock ();
        }
    }
}

Urho3D::TileMapLayer2D *WarriorAi::FindWallsLayer ()
{
    Urho3D::TileMap2D *tileMap = tileMapNode_->GetComponent <Urho3D::TileMap2D> ();
    for (int index = 0; index < tileMap->GetNumLayers (); index++)
    {
        Urho3D::TileMapLayer2D *layer = tileMap->GetLayer (index);
        if (layer->GetLayerType () == Urho3D::LT_TILE_LAYER &&
                         layer->GetProperty ("IsWallsLayer") == "true")
            return layer;
    }
    return 0;
}

WarriorAi::WarriorAi (Urho3D::Context *context) : UnitBasis (context)
{
    scanningRadius_ = 12.0f;
    maxSpeed_ = 4.0f;
    attackDamage_ = 15.0f;
    newDecinisionTime_ = 3.0f;
    timeFromLastDecinision_ = 999.0f;
    team_ = Constants::ORCS_TEAM_ID;
    tileMapNode_ = 0;
}

bool WarriorAi::Init ()
{
    if (node_ && tileMapNode_)
    {
        ready_ = true;
        return true;
    }
    else
    {
        ready_ = false;
        return false;
    }
}

bool WarriorAi::Update (float timeStep)
{
    if (timeFromDie_ <= 0.0f)
    {
        if (timeFromLastDamage_ < Constants::DAMAGE_ANIMATION_TIME)
            timeFromLastDamage_ += timeStep;

        UpdateAnimation ();
        if (isAttackingNow_)
            UpdateAttack (timeStep);

        UnitInterface *nearestEnemy = GetNearestEnemy ();
        if (nearestEnemy)
            UpdateAi (nearestEnemy, timeStep);
        if (lives_ <= 100.0f)
            lives_ += timeStep * 3.0f;
    }

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
    return true;
}

bool WarriorAi::Dispose ()
{
    ready_ = false;
    return true;
}

float WarriorAi::GetScanningRadius ()
{
    return scanningRadius_;
}

void WarriorAi::SetScanningRadius (float scanningRadius)
{
    scanningRadius_ = scanningRadius;
}

float WarriorAi::GetNewDecinisionTime ()
{
    return newDecinisionTime_;
}

void WarriorAi::SetNewDecinisionTime (float newDecinisionTime)
{
    newDecinisionTime_ = newDecinisionTime;
}

Urho3D::Node *WarriorAi::GetTileMapNode ()
{
    return tileMapNode_;
}

void WarriorAi::SetTileMapNode (Urho3D::Node *tileMapNode)
{
    tileMapNode_ = tileMapNode;
}

void WarriorAi::SetTileMapNode (Urho3D::String wayToNode, StatesEngine::SceneContainer *scene)
{
    SetTileMapNode (FindNode (context_, wayToNode, scene));
}

WarriorAi::~WarriorAi ()
{
    Dispose ();
}
}
