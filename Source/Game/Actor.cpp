#include <Actor.h>

#include <Cell.h>
#include <Game.h>

#include <TextureManager.h>
#include <DataManager.h>

Actor::Actor()
{
    Visible = false;
    Hidden = false;
    CurrentCell = NULL;
}

Actor::~Actor()
{

}

bool Actor::Init(MapCoordinates SpawnLocation)
{
    ActorBillboard = RENDERER->getSceneManager()->createBillboardSet(1);
    ActorBillboard->createBillboard(LocationCoordinates.X, LocationCoordinates.Y, LocationCoordinates.Z);
    ActorBillboard->setDefaultDimensions(1.0, 1.0);


    Ogre::MaterialPtr Mat = TEXTURE->getOgreMaterial(DATA->getLabelIndex("MATERIAL_DWARF"), DATA->getLabelIndex("SURFACETYPE_ROUGH_FLOOR_2"));
    ActorBillboard->setMaterialName(Mat->getName());

	ActorNode = RENDERER->getSceneManager()->getRootSceneNode()->createChildSceneNode(); //"ActorNode1");
    ActorNode->attachObject(ActorBillboard);

    setLocation(SpawnLocation);

    return true;
}

void Actor::setLocation(MapCoordinates NewPosition)
{
    LocationCoordinates = NewPosition;
    CurrentCubeCoordinates = CubeCoordinates(NewPosition);

    CellCoordinates NewCellCoords = CellCoordinates(NewPosition);
    Cell* NewCell = GAME->getMap()->getCell(NewCellCoords);

    ActorNode->setPosition(NewPosition.X, NewPosition.Y, NewPosition.Z);

    if (CurrentCell != NewCell)
    {
        if (CurrentCell != NULL)
        {
            CurrentCell->removeActor(CellActorIndex);
        }

        if (NewCell != NULL)
        {
            CellActorIndex = NewCell->addActor(this);
            CurrentCell = NewCell;
        }
    }
}

void Actor::setRenderPosition(Ogre::Vector3 NewPosition)
{
    ActorNode->setPosition(NewPosition);
}

void Actor::MoveRenderPosition(Ogre::Vector3 Translation)
{
    ActorNode->translate(Translation);
}
