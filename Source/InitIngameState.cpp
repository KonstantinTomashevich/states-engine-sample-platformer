#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Urho2D/TileMap2D.h>
#include <Urho3D/Urho2D/AnimatedSprite2D.h>
#include <Urho3D/Urho2D/TmxFile2D.h>
#include <Urho3D/Urho2D/Renderer2D.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Resource/XMLElement.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>

#include <StatesEngine/StatesEngine.hpp>
#include <StatesEngine/SceneContainer.hpp>
#include "InitIngameState.hpp"
#include "IngameCameraUpdater.hpp"
#include "IngamePlayerController.hpp"
#include "IngameHud.hpp"
#include "Constants.hpp"
#include "IngameEventsHandler.hpp"
#include "WarriorAi.hpp"
#include "Utils.hpp"

void IngameStateFunctions::ParseTileMap (Urho3D::Context *context, Urho3D::Node *tileMapNode)
{
    Urho3D::TileMap2D *tileMap = tileMapNode->GetComponent <Urho3D::TileMap2D> ();
    Urho3D::Node *mapObjectsNode = tileMapNode->CreateChild ("objects");
    Urho3D::TileMapInfo2D mapInfo = tileMap->GetInfo ();
    Urho3D::Log *log = context->GetSubsystem <Urho3D::Log> ();

    for (int layerIndex = 0; layerIndex < tileMap->GetNumLayers (); layerIndex++)
    {
        Urho3D::TileMapLayer2D *layer = tileMap->GetLayer (layerIndex);
        if (layer->GetLayerType () == Urho3D::LT_OBJECT_GROUP &&
                layer->GetProperty ("IsObjectsLayer") == "true")
        {
            if (!ParseObjectsLayer (context, mapObjectsNode, layer))
                log->Write (Urho3D::LOG_ERROR, "Error when initializing objects Layer!");
        }
        else if (layer->GetLayerType () == Urho3D::LT_TILE_LAYER &&
                 layer->GetProperty ("IsWallsLayer") == "true")
        {
            if (!ParseWallsLayer (context, layer))
                log->Write (Urho3D::LOG_ERROR, "Error when initializing walls Layer!");
        }
    }
}

bool IngameStateFunctions::ParseObjectsLayer (
        Urho3D::Context *context, Urho3D::Node *objectsNode,
        Urho3D::TileMapLayer2D *layer)
{
    StatesEngine::StatesEngine *statesEngine = context->GetSubsystem <StatesEngine::StatesEngine> ();
    Urho3D::ResourceCache *cache = context->GetSubsystem <Urho3D::ResourceCache> ();
    for (int objectIndex = 0; objectIndex < layer->GetNumObjects (); objectIndex++)
    {
        Urho3D::TileMapObject2D *object = layer->GetObject (objectIndex);
        if (object->GetName () == "coin" && object->GetType () == "coin")
        {
            Urho3D::Node *coin = objectsNode->CreateChild ();
            coin->LoadXML (cache->GetResource <Urho3D::XMLFile> ("Objects/coin.xml")->GetRoot ());
            coin->SetName ("coin");
            coin->SetPosition (Urho3D::Vector3 (object->GetPosition ()));
        }

        if (object->GetName () == "player_spawn" && object->GetType () == "player_spawn")
        {
            Urho3D::Node *player = objectsNode->CreateChild ();
            player->LoadXML (cache->GetResource <Urho3D::XMLFile> ("Objects/knight.xml")->GetRoot ());
            player->SetName ("player");
            player->SetPosition (Urho3D::Vector3 (object->GetPosition ()));

            Urho3D::SharedPtr <IngamePlayerController> playerController =
                    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <IngamePlayerController> ();
            playerController->SetNode (player);
            playerController->SetTeam (Constants::PLAYER_TEAM_ID);
            if (Urho3D::GetPlatform () != "Android" && Urho3D::GetPlatform () != "iOS")
                playerController->SetIsUseKeyboardInsteadOfJoystick (true);
            playerController->Init ();
        }

        if (object->GetName () == "orc" && object->GetType () == "orc")
        {
            Urho3D::Node *orc = objectsNode->CreateChild ();
            orc->LoadXML (cache->GetResource <Urho3D::XMLFile> ("Objects/orc.xml")->GetRoot ());
            orc->SetName ("orc");
            orc->SetPosition (Urho3D::Vector3 (object->GetPosition ()));

            Urho3D::SharedPtr <WarriorAi> warriorAi = statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <WarriorAi> ();
            warriorAi->SetNode (orc);
            warriorAi->SetTeam (Constants::ORCS_TEAM_ID);
            warriorAi->SetTileMapNode (layer->GetTileMap ()->GetNode ());
            warriorAi->Init ();
        }
    }
    return true;
}

bool IngameStateFunctions::ParseWallsLayer (Urho3D::Context *context, Urho3D::TileMapLayer2D *layer)
{
    for (int x = 0; x < layer->GetWidth (); x++)
        for (int y = 0; y < layer->GetHeight (); y++)
        {
            if (!IsTileEmpty (x, y, layer))
            {
                Urho3D::Node *node = layer->GetTileNode (x, y);
                if (CountOfEmptyTilesAround (x, y, layer) > 0)
                {
                    node->CreateComponent <Urho3D::RigidBody2D> ();
                    node->SetVar (
                                Constants::OBJECT_TYPE_VAR_HASH,
                                Urho3D::Variant (Constants::OBJECT_TYPE_WALL));
                    Urho3D::CollisionBox2D *box = node->CreateComponent <Urho3D::CollisionBox2D> ();
                    box->SetCenter (0.6f, 0.6f);
                    box->SetSize (1.28f, 1.28f);
                    box->SetFriction (0.2f);
                }
            }
        }
    return true;
}

bool IngameStateFunctions::IsTileMapCoordinateValid (float x, float y, Urho3D::TileMap2D *tileMap)
{
    return (x >= 0 && y >= 0 && x <= tileMap->GetInfo ().width_ &&
            y <= tileMap->GetInfo ().height_);
}

bool IngameStateFunctions::IsTileEmpty (int x, int y, Urho3D::TileMapLayer2D *layer)
{
    if (IsTileMapCoordinateValid (x, y, layer->GetTileMap ()))
        return (!layer->GetTileNode (x, y));
    else
        return false;
}

int IngameStateFunctions::CountOfEmptyTilesAround (int tileX, int tileY, Urho3D::TileMapLayer2D *layer)
{
    int count = 0;
    for (int x = tileX - 1; x < tileX + 2; x++)
        for (int y = tileY - 1; y < tileY + 2; y++)
            if (x != tileX && y != tileY && IsTileEmpty (x, y, layer))
                count++;
    return count;
}

void IngameStateFunctions::InitIngameState (Urho3D::Context *context, Urho3D::String pathToLevel)
{
    RegisterType <StatesEngine::SceneContainer> (context);
    RegisterType <IngameCameraUpdater> (context);
    RegisterType <IngameEventsHandler> (context);
    RegisterType <IngameHud> (context);
    RegisterType <WarriorAi> (context);
    RegisterType <IngamePlayerController> (context);

    StatesEngine::StatesEngine *statesEngine = context->GetSubsystem <StatesEngine::StatesEngine> ();
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->DisposeAll ("any");
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->RemoveAll ("any");
    Urho3D::ResourceCache *cache = context->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::Graphics *graphics = context->GetSubsystem <Urho3D::Graphics> ();
    Urho3D::Audio *audio = context->GetSubsystem <Urho3D::Audio> ();

    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context));
    scene->CreateComponent <Urho3D::Octree> ();
    scene->CreateComponent <Urho3D::Renderer2D> ();
    scene->CreateComponent <Urho3D::DebugRenderer> ();

    Urho3D::Node *tileMapNode = scene->CreateChild ("tileMap");
    Urho3D::TileMap2D *tileMap = tileMapNode->CreateComponent <Urho3D::TileMap2D> ();
    tileMap->SetTmxFile (cache->GetResource <Urho3D::TmxFile2D> (pathToLevel));

    Urho3D::Node *cameraNode = scene->CreateChild ("camera");
    cameraNode->CreateComponent <Urho3D::SoundListener> ();
    Urho3D::Camera *camera = cameraNode->CreateComponent <Urho3D::Camera> ();
    camera->SetOrthographic (true);
    camera->SetOrthoSize (12);

    Urho3D::SharedPtr <StatesEngine::SceneContainer> sceneContainer =
            statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <StatesEngine::SceneContainer> ();
    sceneContainer->Set (scene);
    sceneContainer->Init ();
    sceneContainer->SetupSceneViewport (0, "/camera/",
                                        Urho3D::IntRect (0, 0, graphics->GetWidth (), graphics->GetHeight ()));
    audio->SetListener (cameraNode->GetComponent <Urho3D::SoundListener> ());

    ParseTileMap (context, tileMapNode);
    Urho3D::SharedPtr <IngameCameraUpdater> cameraUpdater =
            statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <IngameCameraUpdater> ();
    cameraUpdater->SetCameraNode (cameraNode);
    cameraUpdater->SetPlayerNode ("/tileMap/objects/player/");
    cameraUpdater->SetTileMapNode (tileMapNode);
    cameraUpdater->Init ();

    Urho3D::SharedPtr <IngameHud> ingameHud =
            statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <IngameHud> ();
    if (Urho3D::GetPlatform () != "Android" && Urho3D::GetPlatform () != "iOS")
        ingameHud->SetIsShowTouchControls (false);
    else
        ingameHud->SetIsShowTouchControls (true);
    statesEngine->GetState <StatesEngine::StateObjectsManager> ()->Create <IngameEventsHandler> ();
}
