#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <StatesEngine/SceneContainer.hpp>
#include <StatesEngine/StateObject.hpp>

Urho3D::Node *FindNode (Urho3D::Context *context, Urho3D::String wayToNode, StatesEngine::SceneContainer *scene = 0);
template <class T> void RegisterType (Urho3D::Context *context)
{
    if (context->CreateObject (T::GetTypeNameStatic ()).Null ())
        context->RegisterFactory <T> ();
}
