#pragma once
#include "MenuUiListener.hpp"
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/StringHash.h>
#include <StatesEngine/StateObject.hpp>

#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/Sprite.h>
#include <Urho3D/UI/Text.h>

namespace FrontendCXX
{
class MenuUi : public StatesEngine::StateObject
{
URHO3D_OBJECT (MenuUi, StatesEngine::StateObject)
friend class MenuUiListener;
protected:
    int lastWidth_;
    int lastHeight_;
    MenuUiListener listener_;

    Urho3D::Button *level1Button_;
    Urho3D::Text *level1ButtonText_;
    Urho3D::Button *level2Button_;
    Urho3D::Text *level2ButtonText_;
    Urho3D::Button *level3Button_;
    Urho3D::Text *level3ButtonText_;
    Urho3D::Button *exitButton_;
    Urho3D::Text *exitButtonText_;
    Urho3D::Sprite *background_;

    void InitUI ();
    void ResizeUI (int width, int height);
    void UpdateUI (float timeStep);
    void HideUI ();
public:
    MenuUi (Urho3D::Context *context);
    virtual bool Init ();
    virtual bool Update (float timeStep);
    virtual bool Dispose ();
    virtual ~MenuUi ();
};
}
