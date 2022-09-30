#include "../include/Scene.h"

#include "../include/Types.h"

Scene::Scene(Texture2D characterTexture)
{
    for(int i = 0; i < 40; i++)
    {
        entt::entity entity = registry.create();
        registry.emplace<EntityPosition>(entity);
        registry.emplace<Texture2D>(entity);
        registry.emplace<EntitySize>(entity);
        registry.emplace<bool>(entity, false);
    }

    float posX = 0.0f;
    float posY = 0.0f;

    int counter = 0;

    auto view = registry.view<EntityPosition, Texture2D, EntitySize, bool>();
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

        entityPosition = {{posX, posY}, {posX, posY}};

        texture = characterTexture;

        size = {32.0f, 32.0f};

        posX += 64.0f;
        counter++;
    }
}

Scene::~Scene()
{
    registry.clear();
}