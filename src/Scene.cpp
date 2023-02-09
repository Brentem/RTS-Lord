#include "../include/Scene.h"

#include "../include/AssetManager.h"
#include "../include/Types.h"
#include "../include/Timer.h"

const Vector2 BASE = {128, -32};
const Vector2 RESOURCE = {-224, -256};

Scene::Scene()
{
    CreatingWorkers();
    CreatingBuildings();
    CreatingResources();

    gold = 0;
}

Scene::~Scene()
{
    registry.clear();
}

// This function should not be in the endproduct.
void Scene::CreatingWorkers()
{
    for(int i = 0; i < 5; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<EntityPosition>(entity);
        // registry.emplace<Texture2D>(entity);
        registry.emplace<Animation>(entity);
        registry.emplace<EntitySize>(entity);
        registry.emplace<IsSelected>(entity, false);
        registry.emplace<EntityType>(entity);
        registry.emplace<Path>(entity);
        registry.emplace<TaskPositions>(entity);
        registry.emplace<SelectedCell>(entity);
        registry.emplace<Timer>(entity);
        registry.emplace<UnitState>(entity);
        registry.emplace<GatheringFlags>(entity);
        registry.emplace<EventQueue>(entity);
    }

    float posX = 0.0f;
    float posY = 0.0f;

    int counter = 0;

    AssetManager* manager = AssetManager::GetInstance();
    Texture2D characterTexture = manager->GetTexture("spritesheet_peasant_idle_S.png");

    auto view = registry.view<EntityPosition, Animation, EntitySize, EntityType, TaskPositions, SelectedCell>();
    for(auto entity : view)
    {
        if(counter % 10 == 0)
        {
            posX = 0.0f;
            posY += 64.0f;
        }

        EntityPosition& entityPosition = view.get<EntityPosition>(entity);
        Animation& animation = view.get<Animation>(entity);
        //Texture2D& texture = view.get<Texture2D>(entity);
        EntitySize& size = view.get<EntitySize>(entity);
        EntityType& type = view.get<EntityType>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        SelectedCell& cell = view.get<SelectedCell>(entity);

        size = {32.0f, 32.0f};
        entityPosition = {{posX, posY}, {posX, posY}};

        animation.texture = characterTexture;
        animation.frameCount = characterTexture.width/size.width;
        srand(time(0));
        int random_number = rand() % animation.frameCount + 1;
        animation.currentFrame = random_number;
        //texture = characterTexture;

        type.Value = EntityType::Worker;
        taskPositions = {BASE, {0.0f, 0.0f}};
        cell.pair = Pair(0, 0);

        posX += 64.0f;
        counter++;
    }
}

// This function should not be in the endproduct.
void Scene::CreatingBuildings()
{
    // Home base
    Image image = GenImageColor(64, 64, BLACK);
    Texture2D baseTexture = LoadTextureFromImage(image); // This texture isn't deleted ever.

    std::vector<entt::entity> entities;

    for(int i = 0; i < 2; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<EntityPosition>(entity);
        registry.emplace<Texture2D>(entity);
        registry.emplace<EntitySize>(entity);
        registry.emplace<EntityType>(entity);

        entities.push_back(entity);
    }

    Vector2 newPosition = BASE;

    for(auto entity : entities)
    {
        EntityPosition& entityPosition = registry.get<EntityPosition>(entity);
        Texture2D& texture = registry.get<Texture2D>(entity);
        EntitySize& size = registry.get<EntitySize>(entity);
        EntityType& type = registry.get<EntityType>(entity);

        entityPosition.currentPosition = newPosition;
        entityPosition.targetPosition = newPosition; // If target position is set to zeroes, the resources will move to the middle of the screen.
        texture = baseTexture;
        size = {64.0f, 64.0f};
        type.Value = EntityType::Building;

        newPosition.x -= 512;
    }

    UnloadImage(image);
}

void Scene::CreatingResources()
{
    Image image = GenImageColor(32, 32, YELLOW);
    Texture2D resourceTexture = LoadTextureFromImage(image); // This texture isn't deleted ever.

    std::vector<entt::entity> entities;

    for(int i = 0; i < 10; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<EntityPosition>(entity);
        registry.emplace<Texture2D>(entity);
        registry.emplace<EntitySize>(entity);
        registry.emplace<EntityType>(entity);

        entities.push_back(entity);
    }

    Vector2 newPosition = RESOURCE;
    int counter = 0;

    for(auto entity : entities)
    {
        EntityPosition& entityPosition = registry.get<EntityPosition>(entity);
        Texture2D& texture = registry.get<Texture2D>(entity);
        EntitySize& size = registry.get<EntitySize>(entity);
        EntityType& type = registry.get<EntityType>(entity);

        if((counter != 0) && (counter % 5 == 0))
        {
            newPosition = RESOURCE;
            newPosition.x -= 256;
        }

        entityPosition.currentPosition = newPosition;
        entityPosition.targetPosition = newPosition; // If target position is set to zeroes, the resources will move to the middle of the screen.
        texture = resourceTexture;
        size = {32.0f, 32.0f};
        type.Value = EntityType::Resource;

        newPosition.x -= -64.0f;
        counter++;
    }

    UnloadImage(image);
}