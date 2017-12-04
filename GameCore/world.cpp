#include "world.hpp"

#include <stdio.h>

namespace Game
{

World::World() : gravity(), actors(), terrain(), terrainIsLoaded(false)
{
    this->SetGravity(Vector2(0, -200));
}

World::~World()
{

}

Vector2 World::GetGravity() const
{
    return this->gravity;
}

void World::SetGravity(const Vector2 &gravity)
{
    this->gravity = gravity;
}

void World::Update(double deltaT)
{
    if (!this->actors.empty())
    {
        for (auto actor : this->actors)
        {
            Vector2 size = actor->GetSize();
            Vector2 before = actor->GetPosition();
            actor->AddVelocity(this->gravity * deltaT);
            actor->Update(deltaT);
            Vector2 after = actor->GetPosition();

            Vector2 delta = after - before;
            Vector2 velocity = actor->GetVelocity();

            for (Terrain *tObj : this->terrain)
            {
                Vector2 objPos = tObj->GetPosition();
                Vector2 objSize = tObj->GetSize();

                Vector2 topRight(objPos.x + objSize.x, objPos.y);
                Vector2 bottomLeft(objPos.x, objPos.y - objSize.y);
                Vector2 bottomRight(topRight.x, bottomLeft.y);

                if (tObj->GetTerrainType() == TerrainType::TYPE_PLATFORM)
                {
                    if (velocity.y < 0 && this->SegmentsOverlap(after.x, after.x + size.x, objPos.x, topRight.x))
                    {
                        if (before.y - size.y >= objPos.y)
                        {
                            if (objPos.y > (after.y - size.y))
                            {
                                after.y = objPos.y + size.y;
                                velocity.y = 0;
                            }
                        }
                    }
                }
                else if (tObj->GetTerrainType() == TerrainType::TYPE_WALL)
                {
                    if (this->SegmentsOverlap(after.x, after.x + size.x, objPos.x, topRight.x))
                    {
                        if (velocity.y < 0)
                        {
                            if (before.y - size.y >= objPos.y)
                            {
                                if (objPos.y > (after.y - size.y))
                                {
                                    after.y = objPos.y + size.y;
                                    velocity.y = 0;
                                }
                            }
                        }
                        else if (velocity.y > 0)
                        {
                            if (bottomLeft.y >= before.y)
                            {
                                if (after.y > bottomLeft.y)
                                {
                                    after.y = bottomLeft.y;
                                    //velocity.y = 0;
                                }
                            }
                        }
                    }

                    if (this->SegmentsOverlap(after.y, after.y - size.y, objPos.y, bottomLeft.y))
                    {
                        if (velocity.x > 0)
                        {

                        }
                    }
                }
            }

            actor->SetPosition(after);
            actor->SetVelocity(velocity);
        }
    }
}

void World::AddActor(Actor *actor)
{
    this->actors.push_back(actor);
}

void World::RemoveActor(Actor *actor)
{
    this->actors.remove(actor);
}

std::list<Actor*> World::GetActors()
{
    return this->actors;
}

void World::LoadWorld()
{
    Terrain *terrainObj;

    terrainObj = new Platform();
    terrainObj->SetSize(Vector2(400, 20));
    terrainObj->SetPosition(Vector2(-200, 0));
    this->AddTerrain(terrainObj);

    terrainObj = new Wall();
    terrainObj->SetSize(Vector2(40, 200));
    terrainObj->SetPosition(Vector2(-200, 200));
    this->AddTerrain(terrainObj);

    terrainObj = new Platform();
    terrainObj->SetSize(Vector2(600, 20));
    terrainObj->SetPosition(Vector2(300, 20));
    this->AddTerrain(terrainObj);

    this->SetTerrainIsLoaded(true);
}

void World::ClearTerrain()
{
    for (Terrain *terrainObj : this->terrain)
    {
        delete terrainObj;
    }

    this->terrain.clear();
}

void World::AddTerrain(Terrain *terrain)
{
    this->terrain.push_back(terrain);
}

std::list<Terrain*> World::GetTerrain()
{
    return this->terrain;
}

bool World::IsTerrainLoaded() const
{
    return this->terrainIsLoaded;
}

void World::SetTerrainIsLoaded(bool loaded)
{
    this->terrainIsLoaded = loaded;
}

bool World::SegmentsOverlap(double x1, double x2, double y1, double y2) const
{
    return (x2 >= y1 && y2 >= x1);
}

}
