#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/StringHash.h>
#include <StatesEngine/StateObject.hpp>
#include <GameEngine/PlayerController.hpp>
#include "IngameHudListener.hpp"

#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>

namespace FrontendCXX
{
class IngameHud : public StatesEngine::StateObject
{
URHO3D_OBJECT (IngameHud, StatesEngine::StateObject)
friend class IngameHudListener;
protected:
    GameEngine::PlayerController *playerController_;
    IngameHudListener listener_;
    int lastWidth_;
    int lastHeight_;
    bool showTouchControls_;

    Urho3D::Button *screenBackground_;
    Urho3D::Sprite *coinIcon_;
    Urho3D::Text *coinsText_;
    Urho3D::Sprite *livesIcon_;
    Urho3D::Text *livesText_;

    Urho3D::Button *pauseButton_;
    Urho3D::Text *pauseButtonText_;
    Urho3D::Sprite *pauseShadow_;
    Urho3D::UIElement *pauseWindow_;
    Urho3D::Sprite *pauseWindowBackground_;
    Urho3D::Button *resumeButton_;
    Urho3D::Text *resumeButtonText_;
    Urho3D::Button *menuButton_;
    Urho3D::Text *menuButtonText_;
    Urho3D::Text *levelFailedText_;

    Urho3D::Button *attackButton_;
    Urho3D::Button *blockButton_;
    Urho3D::UIElement *joystick_;
    Urho3D::Sprite *joystickBackground_;
    Urho3D::Button *joystickKnob_;

    void InitUI ();
    void ResizeUI (int width, int height);
    void UpdateUI (float timeStep);
    void HideUI ();
public:
    IngameHud (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();

    GameEngine::PlayerController *GetPlayerController ();
    void SetPlayerController (GameEngine::PlayerController *playerController);
    void SetPauseWindowVisible (bool isEnabled);

    bool IsSnowTouchControls ();
    void SetIsShowTouchControls (bool showTouchControls);
    ~IngameHud ();
};
}
