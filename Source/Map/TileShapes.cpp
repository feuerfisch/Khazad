#include <TileShapes.h>
#include <Coordinates.h>

#include <Renderer.h>

void CreateAllEntities()
{
    CreateFlatTiles();
    CreateFaceTiles();

    CreateSlopedTiles();
}

void CreateFaceTiles()
{
    Ogre::ManualObject* ManualObject;

    for (FaceShape Shape = FACESHAPE_START; Shape < NUM_FACESHAPES; ++Shape)
    {
        ManualObject = RENDERER->getSceneManager()->createManualObject("ManualFaceTile");
        ManualObject->setDynamic(false);

        uint8_t HeightData = (uint8_t) Shape;

        uint8_t LeftCorner = HeightData & (3 << 0);
        uint8_t RightCorner = HeightData & (3 << 2);    RightCorner >>= 2;

        uint8_t DirectionData = HeightData & (3 << 4);    DirectionData >>= 4;
        Direction DirectionType = (Direction) (DirectionData + 2);


        ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        {
            float XLeft; float XRight; float YLeft; float YRight;

            switch (DirectionType)
            {
                case DIRECTION_SOUTH:
                    XLeft = -HALFCUBE;  YLeft = -HALFCUBE;  XRight =  HALFCUBE;  YRight = -HALFCUBE;
                    break;
                case DIRECTION_NORTH:
                    XLeft =  HALFCUBE;  YLeft =  HALFCUBE;  XRight = -HALFCUBE;  YRight =  HALFCUBE;
                    break;
                case DIRECTION_WEST:
                    XLeft = -HALFCUBE;  YLeft =  HALFCUBE;  XRight = -HALFCUBE;  YRight = -HALFCUBE;
                    break;
                case DIRECTION_EAST:
                    XLeft =  HALFCUBE;  YLeft = -HALFCUBE;  XRight =  HALFCUBE;  YRight =  HALFCUBE;
                    break;
            }

            ManualObject->position(XLeft,  YLeft, -HALFCUBE);  // Left Bottom
            ManualObject->colour(Ogre::ColourValue::White);
            ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

            ManualObject->position(XLeft, YLeft, (LeftCorner / 3.0f) -HALFCUBE);  // Left Top
            ManualObject->colour(Ogre::ColourValue::White);
            ManualObject->textureCoord(Ogre::Vector2(0.0f, (LeftCorner / 3.0f)));


            ManualObject->position(XRight, YRight, -HALFCUBE);  // Right Bottom
            ManualObject->colour(Ogre::ColourValue::White);
            ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

            ManualObject->position(XRight, YRight, (RightCorner / 3.0f) -HALFCUBE);  // Right Top
            ManualObject->colour(Ogre::ColourValue::White);
            ManualObject->textureCoord(Ogre::Vector2(1.0f, (RightCorner / 3.0f)));

            ManualObject->triangle(0, 1, 3);

            if (LeftCorner != 0 && RightCorner != 0)
            {
                ManualObject->triangle(0, 3, 2);
            }
        }

        char buffer[64];
        sprintf(buffer, "Face%i", (uint16_t) Shape);

        ManualObject->end();
        ManualObject->convertToMesh(buffer);
        RENDERER->getSceneManager()->destroyManualObject(ManualObject);
    }
}

void CreateFlatTiles()
{
    // Flat tiles need 6
    Ogre::ManualObject* ManualObject;

    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualUpTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position( HALFCUBE,  HALFCUBE, -HALFCUBE);  // North East Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->normal(Ogre::Vector3::UNIT_Z);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        ManualObject->position(-HALFCUBE,  HALFCUBE, -HALFCUBE);  // South East Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->normal(Ogre::Vector3::UNIT_Z);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position( HALFCUBE, -HALFCUBE, -HALFCUBE);  // North West Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->normal(Ogre::Vector3::UNIT_Z);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

        ManualObject->position(-HALFCUBE, -HALFCUBE, -HALFCUBE);  // South West Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->normal(Ogre::Vector3::UNIT_Z);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));


        //ManualObject->index(0);
        //ManualObject->index(1);
        //ManualObject->index(2);
        //ManualObject->index(3);

        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("UpTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualDownTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position( HALFCUBE,  HALFCUBE, -HALFCUBE);  // North East Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        ManualObject->position(-HALFCUBE,  HALFCUBE, -HALFCUBE);  // South East Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position( HALFCUBE, -HALFCUBE, -HALFCUBE);  // North West Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

        ManualObject->position(-HALFCUBE, -HALFCUBE, -HALFCUBE);  // South West Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

        //ManualObject->index(0);
        //ManualObject->index(1);
        //ManualObject->index(2);
        //ManualObject->index(3);
        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("DownTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualNorthTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position( HALFCUBE, -HALFCUBE, HALFCUBE);  // East South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

        ManualObject->position( HALFCUBE, -HALFCUBE, -HALFCUBE);  // East South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position( -HALFCUBE, -HALFCUBE, HALFCUBE);  // West South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

        ManualObject->position( -HALFCUBE, -HALFCUBE, -HALFCUBE);  // West South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        /*ManualObject->index(0);
        ManualObject->index(1);
        ManualObject->index(2);
        ManualObject->index(3);*/
        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("NorthTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualSouthTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position(-HALFCUBE, -HALFCUBE,  HALFCUBE);  //  West South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

        ManualObject->position(-HALFCUBE, -HALFCUBE,  -HALFCUBE);  // West South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position( HALFCUBE, -HALFCUBE,  HALFCUBE);  // East South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

        ManualObject->position( HALFCUBE, -HALFCUBE, -HALFCUBE);  // East South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        //ManualObject->index(0);
        //ManualObject->index(1);
        //ManualObject->index(2);
        //ManualObject->index(3);
        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("SouthTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualWestTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position(-HALFCUBE,  HALFCUBE, HALFCUBE);  // West North Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position(-HALFCUBE,  HALFCUBE, -HALFCUBE);  // West North Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        ManualObject->position(-HALFCUBE, -HALFCUBE,  HALFCUBE);  //  West South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

        ManualObject->position(-HALFCUBE, -HALFCUBE, -HALFCUBE);  // West South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));


        //ManualObject->index(0);
        //ManualObject->index(1);
        //ManualObject->index(2);
        //ManualObject->index(3);
        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("WestTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualEastTile");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    {
        ManualObject->position( -HALFCUBE, -HALFCUBE, HALFCUBE);  //  West South Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

        ManualObject->position( -HALFCUBE, -HALFCUBE, -HALFCUBE);  // West South Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

        ManualObject->position( -HALFCUBE,  HALFCUBE, HALFCUBE);  // West North Up
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

        ManualObject->position( -HALFCUBE,  HALFCUBE, -HALFCUBE);  // West North Down
        ManualObject->colour(Ogre::ColourValue::White);
        ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

        //ManualObject->index(0);
        //ManualObject->index(1);
        //ManualObject->index(2);
        //ManualObject->index(3);
        ManualObject->triangle(0, 1, 2);
        ManualObject->triangle(3, 2, 1);
    }
    ManualObject->end();
    ManualObject->convertToMesh("EastTile");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);



    // A 3 pronged marker to show the XYZ axies
    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualAxialMarker");
    ManualObject->setDynamic(false);

    ManualObject->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
    {
        float lAxeSize = 10;
        // X Red
        ManualObject->position(0.0f, 0.0f, 0.0f);
        ManualObject->colour(Ogre::ColourValue::Red);

        ManualObject->position(lAxeSize, 0.0f, 0.0f);
        ManualObject->colour(Ogre::ColourValue::Red);

        // Y Green
        ManualObject->position(0.0f, 0.0f, 0.0f);
        ManualObject->colour(Ogre::ColourValue::Green);

        ManualObject->position(0.0, lAxeSize, 0.0);
        ManualObject->colour(Ogre::ColourValue::Green);

        // Z Blue
        ManualObject->position(0.0f, 0.0f, 0.0f);
        ManualObject->colour(Ogre::ColourValue::Blue);

        ManualObject->position(0.0, 0.0, lAxeSize);
        ManualObject->colour(Ogre::ColourValue::Blue);

        ManualObject->index(0);
        ManualObject->index(1);
        ManualObject->index(2);
        ManualObject->index(3);
        ManualObject->index(4);
        ManualObject->index(5);
    }
    ManualObject->end();
    ManualObject->convertToMesh("AxialMarker");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    // A 3 pronged marker to show the XYZ axies
    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualWireFrame");
    ManualObject->setDynamic(true);

    ManualObject->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_LIST);
    {
        // TOP
        ManualObject->position(HALFCUBE, HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(HALFCUBE, -HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(-HALFCUBE, -HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(-HALFCUBE, HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        // BOTTOM
        ManualObject->position(HALFCUBE, HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(HALFCUBE, -HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(-HALFCUBE, -HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        ManualObject->position(-HALFCUBE, HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue::White);

        // TOP
        ManualObject->index(0);
        ManualObject->index(1);

        ManualObject->index(1);
        ManualObject->index(2);

        ManualObject->index(2);
        ManualObject->index(3);

        ManualObject->index(3);
        ManualObject->index(0);

        // BOTTOM
        ManualObject->index(4);
        ManualObject->index(5);

        ManualObject->index(5);
        ManualObject->index(6);

        ManualObject->index(6);
        ManualObject->index(7);

        ManualObject->index(7);
        ManualObject->index(4);

        // SIDES
        ManualObject->index(0);
        ManualObject->index(4);

        ManualObject->index(1);
        ManualObject->index(5);

        ManualObject->index(2);
        ManualObject->index(6);

        ManualObject->index(3);
        ManualObject->index(7);
    }
    ManualObject->end();
    ManualObject->convertToMesh("WhiteWireFrame");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);


    // A 3 pronged marker to show the XYZ axies
    ManualObject = RENDERER->getSceneManager()->createManualObject("ManualWireFrame");
    ManualObject->setDynamic(true);

    ManualObject->begin("BaseWhiteNoLighting",Ogre::RenderOperation::OT_LINE_LIST);
    {
        // TOP
        ManualObject->position(HALFCUBE, HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(HALFCUBE, -HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(-HALFCUBE, -HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(-HALFCUBE, HALFCUBE, HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        // BOTTOM
        ManualObject->position(HALFCUBE, HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(HALFCUBE, -HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(-HALFCUBE, -HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        ManualObject->position(-HALFCUBE, HALFCUBE, -HALFCUBE);
        ManualObject->colour(Ogre::ColourValue(1.0, 1.0, 0.0, 1.0));

        // TOP
        ManualObject->index(0);
        ManualObject->index(1);

        ManualObject->index(1);
        ManualObject->index(2);

        ManualObject->index(2);
        ManualObject->index(3);

        ManualObject->index(3);
        ManualObject->index(0);

        // BOTTOM
        ManualObject->index(4);
        ManualObject->index(5);

        ManualObject->index(5);
        ManualObject->index(6);

        ManualObject->index(6);
        ManualObject->index(7);

        ManualObject->index(7);
        ManualObject->index(4);

        // SIDES
        ManualObject->index(0);
        ManualObject->index(4);

        ManualObject->index(1);
        ManualObject->index(5);

        ManualObject->index(2);
        ManualObject->index(6);

        ManualObject->index(3);
        ManualObject->index(7);
    }

    ManualObject->end();
    ManualObject->convertToMesh("YellowWireFrame");
    RENDERER->getSceneManager()->destroyManualObject(ManualObject);
}

void CreateSlopedTiles()
{
    Ogre::ManualObject* ManualObject;

    for (TileShape Shape = TILESHAPE_START; Shape < TILESHAPE_WALL; ++Shape)
    {
        ManualObject = RENDERER->getSceneManager()->createManualObject("ManualRampTile");
        ManualObject->setDynamic(false);

        uint16_t HeightData = (uint16_t) Shape;
        uint16_t Divisor = ((uint16_t) TILESHAPE_WALL) + 1;

        bool Triangle1 = false;
        bool Triangle2 = false;

        Divisor /= (HEIGHT_FRACTIONS + 2);
        float NorthEastCorner = HeightData / Divisor;
        HeightData %= Divisor;

        Divisor /= (HEIGHT_FRACTIONS + 2);
        float NorthWestCorner = HeightData / Divisor;
        HeightData %= Divisor;

        Divisor /= (HEIGHT_FRACTIONS + 2);
        float SouthEastCorner = HeightData / Divisor;
        HeightData %= Divisor;

        Divisor /= (HEIGHT_FRACTIONS + 2);
        float SouthWestCorner = HeightData / Divisor;
        HeightData %= Divisor;

        Divisor /= 2;
        bool Split = HeightData / Divisor;


        ManualObject->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
        {
            Ogre::Vector3 Vertex0 = Ogre::Vector3(-HALFCUBE, -HALFCUBE, ((SouthWestCorner - 1) / HEIGHT_FRACTIONS) -HALFCUBE);
            Ogre::Vector3 Vertex1 = Ogre::Vector3( HALFCUBE, -HALFCUBE, ((SouthEastCorner - 1) / HEIGHT_FRACTIONS) -HALFCUBE);
            Ogre::Vector3 Vertex2 = Ogre::Vector3(-HALFCUBE,  HALFCUBE, ((NorthWestCorner - 1) / HEIGHT_FRACTIONS) -HALFCUBE);
            Ogre::Vector3 Vertex3 = Ogre::Vector3( HALFCUBE,  HALFCUBE, ((NorthEastCorner - 1) / HEIGHT_FRACTIONS) -HALFCUBE);

            if (Split)
            {
                if (SouthEastCorner > 0 && NorthEastCorner > 0 && NorthWestCorner > 0)
                {
                    ManualObject->position(Vertex1);  // South East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex3 - Vertex1 ).crossProduct( ( Vertex2 - Vertex1 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

                    ManualObject->position(Vertex3);  // North East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex2 - Vertex3 ).crossProduct( ( Vertex1 - Vertex3 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

                    ManualObject->position(Vertex2);  // North West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex1 - Vertex2 ).crossProduct( ( Vertex3 - Vertex2 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

                    ManualObject->triangle(0, 1, 2);  // SE->NE->NW
                    Triangle1 = true;
                }

                if (NorthWestCorner > 0 && SouthWestCorner > 0 && SouthEastCorner > 0)
                {
                    ManualObject->position(Vertex2);  // North West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex0 - Vertex2 ).crossProduct( ( Vertex1 - Vertex2 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

                    ManualObject->position(Vertex0);  // South West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex1 - Vertex0 ).crossProduct( ( Vertex2 - Vertex0 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

                    ManualObject->position(Vertex1);  // South East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex2 - Vertex1 ).crossProduct( ( Vertex0 - Vertex1 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

                    ManualObject->triangle(3, 4, 5);  // NW->SW->SE
                    Triangle2 = true;
                }
            }
            else
            {
                if (NorthEastCorner > 0 && NorthWestCorner > 0 && SouthWestCorner > 0)
                {
                    ManualObject->position(Vertex3);  // North East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex2 - Vertex3 ).crossProduct( ( Vertex0 - Vertex3 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

                    ManualObject->position(Vertex2);  // North West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex0 - Vertex2 ).crossProduct( ( Vertex3 - Vertex2 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 1.0f));

                    ManualObject->position(Vertex0);  // South West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex3 - Vertex0 ).crossProduct( ( Vertex2 - Vertex0 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

                    ManualObject->triangle(0, 1, 2);  // NE->NW->SW
                    Triangle1 = true;
                }

                if (SouthWestCorner > 0 && SouthEastCorner > 0 && NorthEastCorner > 0)
                {
                    ManualObject->position(Vertex0);  // South West
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal( ( Vertex1 - Vertex0 ).crossProduct( ( Vertex3 - Vertex0 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(0.0f, 0.0f));

                    ManualObject->position(Vertex1);  // South East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal(( Vertex3 - Vertex1 ).crossProduct( ( Vertex0 - Vertex1 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 0.0f));

                    ManualObject->position(Vertex3);  // North East
                    ManualObject->colour(Ogre::ColourValue::White);
                    ManualObject->normal(( Vertex0 - Vertex3 ).crossProduct( ( Vertex1 - Vertex3 ) ).normalisedCopy());
                    ManualObject->textureCoord(Ogre::Vector2(1.0f, 1.0f));

                    ManualObject->triangle(3, 4, 5);  // SW->SE->NE
                    Triangle2 = true;
                }
            }
        }

        if (Triangle1 || Triangle2)
        {
            char buffer[64];
            sprintf(buffer, "Slope%i", (uint16_t) Shape);

            ManualObject->end();
            ManualObject->convertToMesh(buffer);
        }

        RENDERER->getSceneManager()->destroyManualObject(ManualObject);
    }
}

TileShape getTileShapeFromCornerHeight(uint8_t SWCorner, uint8_t SECorner, uint8_t NWCorner, uint8_t NECorner, bool SplittingLine)
{
    uint16_t Shape = 0;
    uint16_t Sum = SWCorner + SECorner + NWCorner + NECorner;

    if (SplittingLine)
    {
        if (SWCorner == 0 || NECorner == 0)
        {
            if (Sum <= 3)
            {
                return TILESHAPE_EMPTY;
            }
            if (NWCorner == 0 || SECorner == 0)
            {
                return TILESHAPE_EMPTY;
            }
        }
        else
        {
            if ( NWCorner == 0 || SECorner == 0)
            {
                return TILESHAPE_EMPTY;
            }
            if (Sum <= 4)
            {
                return TILESHAPE_EMPTY;
            }
        }
    }
    else
    {
        if (NWCorner == 0 || SECorner == 0)
        {
            if (Sum <= 3)
            {
                return TILESHAPE_EMPTY;
            }
            if (SWCorner == 0 || NECorner == 0)
            {
                return TILESHAPE_EMPTY;
            }
        }
        else
        {
            if (SWCorner == 0 || NECorner == 0)
            {
                return TILESHAPE_EMPTY;
            }
            if (Sum <= 4)
            {
                return TILESHAPE_EMPTY;
            }
        }
    }

    //if (((NECorner <= SECorner) && (NECorner <= NWCorner))  &&  ((SWCorner >= SECorner) && (SWCorner >= NWCorner)))  // Break quad along a NW-SE line

    uint16_t Multiplier = 1;

    if (SplittingLine)
    {
        Shape += 1;  // Bit 0 holds the triangle split value, 1 is NW-SE splitting, 0 indicates the alternative SW-NE split
    }
    Multiplier *= 2;

    Shape += SWCorner * Multiplier;
    Multiplier *= (HEIGHT_FRACTIONS + 2);

    Shape += SECorner * Multiplier;
    Multiplier *= (HEIGHT_FRACTIONS + 2);

    Shape += NWCorner * Multiplier;
    Multiplier *= (HEIGHT_FRACTIONS + 2);

    Shape += NECorner * Multiplier;

    return (TileShape) Shape;
}

FaceShape getFaceShapeFromCornerHeight(Direction DirectionType, uint8_t NegativeCorner, uint8_t PossitiveCorner)
{
    uint16_t Shape = 0;

    switch (DirectionType)
    {
        case DIRECTION_DOWN_SOUTH:
        case DIRECTION_DOWN_EAST:

            Shape = Shape | (NegativeCorner << 0);
            Shape = Shape | (PossitiveCorner << 2);
            Shape = Shape | ((DirectionType - 2) << 4);

            break;


        case DIRECTION_DOWN_NORTH:  // Reverse Possitive Negative Heights to ensure proper alignment durring drawing
        case DIRECTION_DOWN_WEST:

            Shape = Shape | (PossitiveCorner << 0);
            Shape = Shape | (NegativeCorner << 2);
            Shape = Shape | ((DirectionType - 2) << 4);

            break;
    }

    return (FaceShape) Shape;
}
