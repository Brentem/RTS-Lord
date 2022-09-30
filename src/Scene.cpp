#include "../include/Scene.h"

#include "../include/Types.h"

Scene::Scene(Texture2D characterTexture)
{
    CreatingWorkers(characterTexture);
    CreatingBuildings();
    CreatingResources();
}

Scene::~Scene()
{
    registry.clear();
}

// This function should not be in the endproduct.
void Scene::CreatingWorkers(Texture2D characterTexture)
{
    for(int i = 0; i < 40; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<EntityPosition>(entity);
        registry.emplace<Texture2D>(entity);
        registry.emplace<EntitySize>(entity);
        registry.emplace<bool>(entity, false);
        registry.emplace<EntityType>(entity);
    }

    float posX = 0.0f;
    float posY = 0.0f;

    int counter = 0;

    auto view = registry.view<EntityPosition, Texture2D, EntitySize, bool, EntityType>();
    for(auto entity : view)
    {
        if(counter % 10 == 0)
        {
            posX = 0.0f;
            posY += 64.0f;
        }

        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Texture2D& texture = view.get<Texture2D>(entity);
        EntitySize& size = view.get<EntitySize>(entity);
        EntityType& type = view.get<EntityType>(entity);

        entityPosition = {{posX, posY}, {posX, posY}};
        texture = characterTexture;
        size = {32.0f, 32.0f};
        type.Value = EntityType::Worker;

        posX += 64.0f;
        counter++;
    }
}

// This function should not be in the endproduct.
void Scene::CreatingBuildings()
{
    // Home base
    Image image = GenImageColor(32, 32, BLACK);
    Texture2D baseTexture = LoadTextureFromImage(image);

    entt::entity entity = registry.create();
    registry.emplace<EntityPosition>(entity);
    registry.emplace<Texture2D>(entity);
    registry.emplace<EntitySize>(entity);
    registry.emplace<EntityType>(entity);

    EntityPosition& entityPosition = registry.get<EntityPosition>(entity);
    Texture2D& texture = registry.get<Texture2D>(entity);
    EntitySize& size = registry.get<EntitySize>(entity);
    EntityType& type = registry.get<EntityType>(entity);

    entityPosition = {{-32, -32}, {-32, -32}};
    texture = baseTexture;
    size = {32.0f, 32.0f};
    type.Value = EntityType::Building;

    UnloadImage(image);
}

void Scene::CreatingResources()
{
    Image image = GenImageColor(32, 32, YELLOW);
    Texture2D resourceTexture = LoadTextureFromImage(image);

    entt::entity entity = registry.create();
    registry.emplace<EntityPosition>(entity);
    registry.emplace<Texture2D>(entity);
    registry.emplace<EntitySize>(entity);
    registry.emplace<EntityType>(entity);

    EntityPosition& entityPosition = registry.get<EntityPosition>(entity);
    Texture2D& texture = registry.get<Texture2D>(entity);
    EntitySize& size = registry.get<EntitySize>(entity);
    EntityType& type = registry.get<EntityType>(entity);

    entityPosition = {{-256, -32}, {-256, -32}};
    texture = resourceTexture;
    size = {32.0f, 32.0f};
    type.Value = EntityType::Resource;

    UnloadImage(image);
}