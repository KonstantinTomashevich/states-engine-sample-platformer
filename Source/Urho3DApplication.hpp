#pragma once
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Container/Vector.h>

class Urho3DApplication : public Urho3D::Application
{
URHO3D_OBJECT (Urho3DApplication, Application)
protected:
    bool isDrawDebug_;
public:
    Urho3DApplication (Urho3D::Context *context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();

    bool IsDrawDebug ();
    void SetIsDrawDebug (bool isDrawDebug);
    void DrawDebug (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual ~Urho3DApplication();
};
