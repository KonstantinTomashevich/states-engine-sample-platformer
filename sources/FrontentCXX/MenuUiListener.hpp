#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/Variant.h>
#include <Urho3D/Core/Object.h>

class MenuUi;

class MenuUiListener : public Urho3D::Object
{
URHO3D_OBJECT (MenuUiListener, Object)
friend class MenuUi;
protected:
    MenuUi *ui_;
    bool isSubscribed_;
public:
    MenuUiListener (MenuUi *ui);
    void Subscribe ();
    void Unsubscribe ();
    virtual ~MenuUiListener ();

    void LevelClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void ExitClicked (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
