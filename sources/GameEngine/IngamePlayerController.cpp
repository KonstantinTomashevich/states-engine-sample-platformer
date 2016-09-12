#include "IngamePlayerController.hpp"
#include "Utils.hpp"
#include "Constants.hpp"

#include <Urho3D/Input/Input.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Urho2D/AnimationSet2D.h>
#include <Urho3D/Urho2D/SpriterData2D.h>
#include <Urho3D/Core/Variant.h>

void IngamePlayerController::KeyboardToJoystickCoordinates()
{
    lastJoystickCoordinates_ = Urho3D::Vector2::ZERO;
    isAttackPressed_ = false;
    isBlockPressed_ = false;

    Urho3D::Input *input = context_->GetSubsystem <Urho3D::Input> ();
    if (input->GetKeyDown (Urho3D::KEY_A))
        lastJoystickCoordinates_.x_ -= 1;
    if (input->GetKeyDown (Urho3D::KEY_D))
        lastJoystickCoordinates_.x_ += 1;
    if (input->GetKeyDown (Urho3D::KEY_W) || input->GetKeyDown (Urho3D::KEY_SPACE))
        lastJoystickCoordinates_.y_ += 1;
    if (input->GetKeyDown (Urho3D::KEY_S))
        lastJoystickCoordinates_.y_ -= 1;

    if (input->GetKeyDown (Urho3D::KEY_E))
        isBlockPressed_ = true;
    else if (input->GetKeyDown (Urho3D::KEY_Q))
        isAttackPressed_ = true;
}

void IngamePlayerController::UpdateAnimation ()
{
    Urho3D::RigidBody2D *body = node_->GetComponent <Urho3D::RigidBody2D> ();
    Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
    if (Urho3D::Abs (body->GetLinearVelocity ().x_) > maxSpeed_ / 20.0f)
        sprite->SetFlipX (body->GetLinearVelocity ().x_ < 0.0f);

    if (lives_ >= 0.0f)
    {
        if (timeFromLastDamage_ >= Constants::DAMAGE_ANIMATION_TIME)
        {
            float distanceToGround = DistanceToGround ();
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
            else if (distanceToGround > 0.1f)
            {
                sprite->SetSpeed (1.0f);
                if (distanceToGround > 1.0f)
                    sprite->SetAnimation ("in_jump");
                else if (body->GetLinearVelocity ().y_ > 0.0f)
                    sprite->SetAnimation ("jump_start");
                else
                    sprite->SetAnimation ("jump_end");
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

IngamePlayerController::IngamePlayerController (Urho3D::Context *context) : UnitBasis (context)
{
    node_ = 0;
    useKeyboardInsteadOfJoystick_ = false;
    lastJoystickCoordinates_ = Urho3D::Vector2::ZERO;
    coins_ = 0.0f;
    attackDamage_ = 35.0f;
    maxSpeed_ = 6.0f;
    lives_ = 100.0f;
}

bool IngamePlayerController::Init()
{
    if (node_)
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

bool IngamePlayerController::Update(float timeStep)
{
    if (timeFromDie_ <= 0.0f)
    {
        if (timeFromLastDamage_ < Constants::DAMAGE_ANIMATION_TIME)
            timeFromLastDamage_ += timeStep;

        if (useKeyboardInsteadOfJoystick_)
            KeyboardToJoystickCoordinates ();

        if (isBlockPressed_)
            SetToBlock ();
        else if (isAttackPressed_)
            SetToAttack ();
        else
            SetToMoving ();

        Urho3D::RigidBody2D *body = node_->GetComponent <Urho3D::RigidBody2D> ();
        body->SetLinearVelocity (
                    Urho3D::Vector2 (maxSpeed_ * lastJoystickCoordinates_.x_,
                                     body->GetLinearVelocity ().y_));

        if (lastJoystickCoordinates_.y_ > 0.1f)
            Jump ();

        UpdateAnimation ();
        if (isAttackingNow_)
            UpdateAttack (timeStep);
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
}

bool IngamePlayerController::Dispose()
{
    ready_ = false;
    return true;
}

bool IngamePlayerController::IsUseKeyboardInsteadOfJoystick ()
{
    return useKeyboardInsteadOfJoystick_;
}

void IngamePlayerController::SetIsUseKeyboardInsteadOfJoystick (bool useKeyboardInsteadOfJoystick)
{
    useKeyboardInsteadOfJoystick_ = useKeyboardInsteadOfJoystick;
}

Urho3D::Vector2 IngamePlayerController::GetLastJoystickCoordinates()
{
    return lastJoystickCoordinates_;
}

bool IngamePlayerController::IsAttackPressed ()
{
    return isAttackPressed_;
}

bool IngamePlayerController::IsBlockPressed ()
{
    return isBlockPressed_;
}

void IngamePlayerController::SetControlls (
        bool isAttackPressed, bool isBlockPressed, Urho3D::Vector2 lastJoystickCoordinates)
{
    isAttackPressed_ = isAttackPressed;
    isBlockPressed_ = isBlockPressed;
    lastJoystickCoordinates_ = lastJoystickCoordinates;
}

bool IngamePlayerController::OnAtttack(Urho3D::StringHash attackerTeam, float damage)
{
    if (team_ != attackerTeam && !IsBlockingNow () && lives_ >= 0)
    {
        lives_ -= damage;
        Urho3D::AnimatedSprite2D *sprite = node_->GetComponent <Urho3D::AnimatedSprite2D> ();
        sprite->SetSpeed (1.0f);
        if (Urho3D::Random () >= 0.5f)
            sprite->SetAnimation ("damaged0");
        else
            sprite->SetAnimation ("damaged1");
        timeFromLastDamage_ = 0.0f;
    }
}

float IngamePlayerController::GetCoins()
{
    return coins_;
}

void IngamePlayerController::SetCoins(float coins)
{
    coins_ = coins;
}

IngamePlayerController::~IngamePlayerController()
{
    Dispose ();
}
