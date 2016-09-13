#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Core/Object.h>

namespace FrontendCXX
{
class IngameHud;
class IngameHudListener : public Urho3D::Object
{
URHO3D_OBJECT (IngameHudListener, Object)
friend class IngameHud;
protected:
    IngameHud *hud_;
    bool isSubscribed_;
public:
    IngameHudListener (IngameHud *hud);
    void Subscribe ();
    void Unsubscribe ();
    virtual ~IngameHudListener ();

    void PauseClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void ResumeClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void MenuClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void JoystickDragMove (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void JoystickDragEnd (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
}

