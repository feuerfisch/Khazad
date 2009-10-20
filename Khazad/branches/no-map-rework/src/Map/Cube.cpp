#include <stdafx.h>

#include <Cube.h>
#include <Cell.h>
#include <Map.h>
#include <Random.h>
#include <TextureManager.h>
#include <DataManager.h>
#include <ModelManager.h>

Cube::Cube()
{
    //setType(CUBE_ACTOR);

    Visible = false;
    Hidden = true;
    Initalized = false;
    data.whole = 0;

    setPosition(0.0, 0.0, 0.0);
    MAP->ChangeCubeCount(1);
}

void Cube::SetOwner(Cell* NewOwner, Uint8 X, Uint8 Y)
{
    Owner = NewOwner;
    CellX = X % CELLEDGESIZE;
    CellY = Y % CELLEDGESIZE;

    Vector3 OwnerPosition = Owner->getPosition();

    float RealX = OwnerPosition.x - (float)(CELLEDGESIZE / 2) + (float)X + (float)HALFCUBE;
    float RealY = OwnerPosition.y - (float)(CELLEDGESIZE / 2) + (float)Y + (float)HALFCUBE;
	setPosition(RealX, RealY, OwnerPosition.z);
}

Cube::~Cube()
{
    MAP->ChangeCubeCount(-1);

    if(Initalized)
    {
        MAP->ChangeInitedCubeCount(-1);
    }
}

bool Cube::Init(Uint16 MaterialType)
{
    Initalized = true;

    Material = MaterialType;

    MAP->ChangeInitedCubeCount(1);

    return true;
}


bool Cube::isSolid()
{
    return data.solid;
}
bool Cube::isEmpty()
{
    return TileShape == DATA->getLabelIndex("TILESHAPE_EMPTY");
}
bool Cube::isSlope()
{
    return TileShape == DATA->getLabelIndex("TILESHAPE_RAMP") || TileShape == DATA->getLabelIndex("TILESHAPE_STAIR");
}



/*
bool Cube::DrawLiquid(float xTranslate, float yTranslate)
{
    Uint16 texture;
    Uint16 Liquid= data.liquid;
    Uint16 LiquidType= data.liquidtype;
    if(Liquid > 0)
    {
        // bind the right texture
        if(LiquidType == 1) // magma
        {
            glColor4f(1, 1, 1, 0.3);
            texture = DATA->getLabelIndex("MATERIAL_LAVA");
        }
        else
        {
            glColor4f(0.5, 0.5, 0.5, 0.7);
            texture = DATA->getLabelIndex("MATERIAL_WATER");
        }
        TEXTURE->BindTexture(texture);
        float xx= xTranslate - 0.5;
        float yy = yTranslate - 0.5;
        float z = 0.141 * Liquid - 0.5;
        float zt = 0.141 * Liquid;
        // draw quad in the appropriate height
        {
            glNormal3f(0.0,0.0,1.0);
            glTexCoord2f(0,0);
            glVertex3f(xx     ,yy    ,z);
            glTexCoord2f(0,1);
            glVertex3f(xx     ,yy + 1,z);
            glTexCoord2f(1,1);
            glVertex3f(xx + 1 ,yy + 1,z);

            glTexCoord2f(1,1);
            glVertex3f(xx + 1 ,yy + 1 ,z);
            glTexCoord2f(1,0);
            glVertex3f(xx + 1 ,yy     ,z);
            glTexCoord2f(0,0);
            glVertex3f(xx     ,yy     ,z);
        }
        Cube * north = getAdjacentCube(FACET_NORTH);
        Cube * east = getAdjacentCube(FACET_EAST);
        Cube * west = getAdjacentCube(FACET_WEST);
        Cube * south = getAdjacentCube(FACET_SOUTH);
        float nl = 0;
        if(north)
        {
            nl = north->getLiquid();
        }
        if (nl < Liquid)
        {
            float z2 =  (0.141 * nl) - 0.5;
            float zt2 =  (0.141 * nl);
            // draw a quad to patch hole north
            glTexCoord2f(0,zt2);
            glVertex3f(xx +1     ,yy    ,z2);
            glTexCoord2f(1,zt2);
            glVertex3f(xx     ,yy     ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx    ,yy    ,z);

            glTexCoord2f(0,zt2);
            glVertex3f(xx +1     ,yy    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx    ,yy    ,z);
            glTexCoord2f(0,zt);
            glVertex3f(xx +1 ,yy    ,z);

        }

        nl = 0;
        if(east)
        {
            nl = east->getLiquid();
        }
        if (nl < Liquid)
        {
            float z2 =  0.141 * nl - 0.5;
            float zt2 =  (0.141 * nl);
            // draw a quad to patch hole east
            glTexCoord2f(0,zt2);
            glVertex3f(xx +1     ,yy +1    ,z2);
            glTexCoord2f(1,zt2);
            glVertex3f(xx +1     ,yy    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx +1     ,yy     ,z);

            glTexCoord2f(0,zt2);
            glVertex3f(xx +1     ,yy +1    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx +1     ,yy     ,z);
            glTexCoord2f(0,zt);
            glVertex3f(xx +1     ,yy +1    ,z);
        }
        if(south)
        {
            nl = south->getLiquid();
        }
        if (nl < Liquid)
        {
            float z2 =  0.141 * nl - 0.5;
            float zt2 =  (0.141 * nl);
            // draw a quad to patch hole south
            glTexCoord2f(0,zt2);
            glVertex3f(xx      ,yy +1    ,z2);
            glTexCoord2f(1,zt2);
            glVertex3f(xx +1     ,yy +1    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx +1   ,yy +1     ,z);

            glTexCoord2f(0,zt2);
            glVertex3f(xx      ,yy +1    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx +1     ,yy +1     ,z);
            glTexCoord2f(0,zt);
            glVertex3f(xx      ,yy +1    ,z);
        }
        if(west)
        {
            nl = west->getLiquid();
        }
        if (nl < Liquid)
        {
            float z2 =  0.141 * nl - 0.5;
            float zt2 =  (0.141 * nl);
            // draw a quad to patch hole west
            glTexCoord2f(0,zt2);
            glVertex3f(xx      ,yy    ,z2);
            glTexCoord2f(1,zt2);
            glVertex3f(xx      ,yy +1    ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx   ,yy +1     ,z);

            glTexCoord2f(0,zt2);
            glVertex3f(xx      ,yy     ,z2);
            glTexCoord2f(1,zt);
            glVertex3f(xx      ,yy +1     ,z);
            glTexCoord2f(0,zt);
            glVertex3f(xx      ,yy     ,z);
        }
    }
}
*/
void Cube::setLiquid(Uint8 liquidtype,Uint8 NewValue)
{
    data.liquidtype=liquidtype;
    data.liquid = NewValue;
    if(NewValue > 0)
        Owner->setLiquidActive(true);
}

bool Cube::setMaterial(Uint16 MaterialType)
{
    Initalized = true;
    Material = MaterialType;
}
Cube* Cube::getAdjacentCube(Facet Type)
{
    Sint32 x = Position.x;
    Sint32 y = Position.y;
    Sint32 z = Position.z;

    switch(Type)
    {
        case FACET_TOP:
            z += 1;
            break;
        case FACET_BOTTOM:
            z -= 1;
            break;
        case FACET_NORTH:
            y -= 1;
            break;
        case FACET_EAST:
            x += 1;
            break;
        case FACET_SOUTH:
            y += 1;
            break;
        case FACET_WEST:
            x -= 1;
            break;
    }

    return MAP->getCube(x, y, z);
}

bool Cube::getAdjacentCubeOutOfBounds(Facet Type)
{
    Sint32 x = Position.x;
    Sint32 y = Position.y;
    Sint32 z = Position.z;
    
    switch(Type)
    {
        case FACET_TOP:
            z += 1;
            break;
        case FACET_BOTTOM:
            z -= 1;
            break;
        case FACET_NORTH:
            y -= 1;
            break;
        case FACET_EAST:
            x += 1;
            break;
        case FACET_SOUTH:
            y += 1;
            break;
        case FACET_WEST:
            x -= 1;
            break;
    }
    
    return MAP->getIsInBounds(x, y, z);
}

Cube* Cube::getNeighborCube(Direction Type)
{
    Sint32 x = Position.x;
    Sint32 y = Position.y;
    Sint32 z = Position.z;

    switch(Type)
    {
        case DIRECTION_NORTH:
            y -= 1;
            break;
        case DIRECTION_EAST:
            x += 1;
            break;
        case DIRECTION_SOUTH:
            y += 1;
            break;
        case DIRECTION_WEST:
            x -= 1;
            break;

        case DIRECTION_NORTHEAST:
            y -= 1;
            x += 1;
            break;
        case DIRECTION_SOUTHEAST:
            y += 1;
            x += 1;
            break;
        case DIRECTION_SOUTHWEST:
            y += 1;
            x -= 1;
            break;
        case DIRECTION_NORTHWEST:
            y -= 1;
            x -= 1;
            break;

        case DIRECTION_UP:
            z += 1;
            break;
        case DIRECTION_DOWN:
            z -= 1;
            break;
    }

    return MAP->getCube(x, y, z);
}

Cell* Cube::getAdjacentCell(Facet Type)
{
    Sint32 x = Position.x;
    Sint32 y = Position.y;
    Sint32 z = Position.z;

    switch(Type)
    {
        case FACET_TOP:
            z += 1;
            break;
        case FACET_BOTTOM:
            z -= 1;
            break;
        case FACET_NORTH:
            y -= 1;
            break;
        case FACET_EAST:
            x += 1;
            break;
        case FACET_SOUTH:
            y += 1;
            break;
        case FACET_WEST:
            x -= 1;
            break;
    }

    return MAP->getCubeOwner(x, y, z);
}

bool Cube::Update()
{
	return true;
}

void Cube::DigChannel()
{
    setHidden(false);
    setShape(DATA->getLabelIndex("TILESHAPE_EMPTY"));
    // reveal tiles around, deig below
    for(Direction DirectionType = DIRECTIONS_START; DirectionType < NUM_DIRECTIONS; ++DirectionType)
    {
        Cube* NeighborCube = getNeighborCube(DirectionType);
        
        if(NeighborCube != NULL)
        {
            if(DirectionType != DIRECTION_DOWN && DirectionType != DIRECTION_UP)
            {
                NeighborCube->setHidden(false);
                NeighborCube->getCellOwner()->setNeedsRedraw(true);
            }
            else if(DirectionType == DIRECTION_DOWN)
            {
                NeighborCube->Dig();
                NeighborCube->getCellOwner()->setNeedsRedraw(true);
            }
        }
    }
}

void Cube::DigSlope()
{    
    if(isSolid())
    {
        setHidden(false);
        // set to floor
        
        // reveal tiles around
        for(Direction DirectionType = DIRECTIONS_START; DirectionType < NUM_DIRECTIONS; ++DirectionType)
        {
            Cube* NeighborCube = getNeighborCube(DirectionType);
            if(DirectionType != DIRECTION_DOWN && DirectionType != DIRECTION_UP)
            {
                
                if(NeighborCube != NULL)
                {
                    NeighborCube->setHidden(false);
                    NeighborCube->getCellOwner()->setNeedsRedraw(true);
                }
            }
            if(DirectionType == DIRECTION_UP)
            {
                NeighborCube->DigChannel();
                NeighborCube->getCellOwner()->setNeedsRedraw(true);
            }
        }
        setShape(DATA->getLabelIndex("TILESHAPE_RAMP"));
        // update draw list of parent cell(s)
        getCellOwner()->setNeedsRedraw(true);
    }
}

void Cube::Dig()
{    
    if(isSolid() || isSlope())
    {
        setHidden(false);
        // set to floor
        setShape(DATA->getLabelIndex("TILESHAPE_FLOOR"));
        // reveal tiles around
        for(Direction DirectionType = DIRECTIONS_START; DirectionType < NUM_DIRECTIONS; ++DirectionType)
        {
            if(DirectionType != DIRECTION_DOWN && DirectionType != DIRECTION_UP)
            {
                Cube* NeighborCube = getNeighborCube(DirectionType);
                if(NeighborCube != NULL)
                {
                    NeighborCube->setHidden(false);
                    NeighborCube->getCellOwner()->setNeedsRedraw(true);
                }
            }
        }
        // update draw list of parent cell(s)
        getCellOwner()->setNeedsRedraw(true);
    }
}

bool Cube::hasFace(Facet FacetType)
{
    return data.facets & FacetType;
}
bool Cube::Draw(float xTranslate, float yTranslate,
                     std::map< int16_t, vector< vertex >* >& normal,
                     std::map< int16_t, vector< vertex >* >& tops)
{
    if(isSlope())
    {
        return DrawSlope(xTranslate,yTranslate,normal,tops);
    }
    else if (isEmpty())
    {
        return false;
    }
    else
    {
        return DrawFaces(xTranslate,yTranslate, normal, tops);
    }
}

//TODO: pre-generating all possible configs and using them as templates would be faster. deferred
bool Cube::DrawFaces(float xTranslate, float yTranslate,
                     std::map< int16_t, vector< vertex >* >& normal,
                     std::map< int16_t, vector< vertex >* >& tops)
{
    // cached quads
    static const vertex vertices[6][4] =
    {
        // position, uv texture coords, normal vector - see vertex in Renderer.h
        // FACET_TOP
        vertex(-0.5f,-0.5f, 0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f ),
        vertex( 0.5f,-0.5f, 0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f ),
        vertex( 0.5f, 0.5f, 0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f ),
        vertex(-0.5f, 0.5f, 0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f ),
        // FACET_BOTTOM
        vertex(-0.5f,-0.5f,-0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f ),
        vertex( 0.5f,-0.5f,-0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f ),
        vertex( 0.5f, 0.5f,-0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f ),
        vertex(-0.5f, 0.5f,-0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f ),
        // FACET_NORTH
        vertex( 0.5f,-0.5f, 0.5f,  0.0f, 1.0f,  0.0f,-1.0f, 0.0f ),
        vertex(-0.5f,-0.5f, 0.5f,  1.0f, 1.0f,  0.0f,-1.0f, 0.0f ),
        vertex(-0.5f,-0.5f,-0.5f,  1.0f, 0.0f,  0.0f,-1.0f, 0.0f ),
        vertex( 0.5f,-0.5f,-0.5f,  0.0f, 0.0f,  0.0f,-1.0f, 0.0f ),
        // FACET_SOUTH
        vertex(-0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f ),
        vertex( 0.5f, 0.5f, 0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f ),
        vertex( 0.5f, 0.5f,-0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f ),
        vertex(-0.5f, 0.5f,-0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f ),
        // FACET_EAST
        vertex( 0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f ),
        vertex( 0.5f,-0.5f, 0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f ),
        vertex( 0.5f,-0.5f,-0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f ),
        vertex( 0.5f, 0.5f,-0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f ),
        // FACET_WEST
        vertex(-0.5f,-0.5f, 0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f ),
        vertex(-0.5f, 0.5f, 0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f ),
        vertex(-0.5f, 0.5f,-0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f ),
        vertex(-0.5f,-0.5f,-0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f )
    };
    // work vector ptr
    vector< vertex >* vec;
    vertex test = vertices[3][3];
    if(!Visible)
    {
        return false;
    }
    for(Facet f = FACET_TOP; f <= FACET_WEST;++f)
    {
        if(!hasFace(f))
        {
            continue; // skip faces we don't have here
        }
        Cube * c = getAdjacentCube(f);
        // floors are always generated in the normal way, they face the opposite direction!
        uint8_t idx = FacetToArrayIndex(f) - 1;
        if(f != FACET_BOTTOM)
        {
            // blocked top facets are sent to the top vertex vector
            bool blocked =
            c &&
            (!c->isHidden() || c->isHidden() && Hidden) &&
            (c->hasFace(OppositeFacet(f)) || c->isSolid() || c->isSlope());
            
            bool totallyblocked = false;
            if(f == FACET_TOP)
            {
                totallyblocked = c && Hidden  && isSolid() && !c->isEmpty();
            }
            else
            {
                totallyblocked = !getAdjacentCubeOutOfBounds(f);
            }
            if (totallyblocked)
            {
                continue;
            }
            
            if(blocked)
            {
                if(f== FACET_TOP)
                {
                    if(!tops.count(Material))
                    {
                        vec = new vector< vertex >;
                        tops[Material] = vec;
                    }
                    else
                    {
                        vec = tops[Material];
                    }
                    vertex v3 = vertices[idx][3];
                    v3.translate(xTranslate,yTranslate);
                    vertex v2 = vertices[idx][2];
                    v2.translate(xTranslate,yTranslate);
                    vertex v1 = vertices[idx][1];
                    v1.translate(xTranslate,yTranslate);
                    vertex v0 = vertices[idx][0];
                    v0.translate(xTranslate,yTranslate);
                    vec->push_back(v3);
                    vec->push_back(v1);
                    vec->push_back(v0);

                    vec->push_back(v3);
                    vec->push_back(v2);
                    vec->push_back(v1);
                }
                continue; // skip blocked face
            }
        }
        // normal visible geometry
        vector< vertex >* vec;
        if(!normal.count(Material))
        {
            vec = new vector< vertex >;
            normal[Material] = vec;
        }
        else
        {
            vec = normal[Material];
        }
        vertex v3 = vertices[idx][3];
        v3.translate(xTranslate,yTranslate);
        vertex v2 = vertices[idx][2];
        v2.translate(xTranslate,yTranslate);
        vertex v1 = vertices[idx][1];
        v1.translate(xTranslate,yTranslate);
        vertex v0 = vertices[idx][0];
        v0.translate(xTranslate,yTranslate);
        vec->push_back(v3);
        vec->push_back(v1);
        vec->push_back(v0);

        vec->push_back(v3);
        vec->push_back(v2);
        vec->push_back(v1);
    }
}
// DRAW SLOPE
/// TODO: normal vectors. these are required for lighting
/// TODO: separate this from khazad, use for mesh generation
/// FIXME: waste of CPU cycles
bool Cube::DrawSlope(float xTranslate, float yTranslate,
                     std::map< int16_t, vector< vertex >* >& normal,
                     std::map< int16_t, vector< vertex >* >& tops)
{
    if(!Visible)
    {
        return false;
    }
    SlopeIndex surroundings;
    surroundings.value = 0;
    
    uint8_t solid;
    // copy surroundings
    for(Direction i = DIRECTION_NORTHWEST; i <= DIRECTION_WEST; ++i)
    {
        solid = 0;
        Cube *neighbor = getNeighborCube(i);
        if( neighbor != NULL)
        {
            if(neighbor->isSolid())
            {
                solid = 2;
            }
            else if (neighbor->isSlope())
            {
                solid = 1;
            }
        }
        surroundings.value |= solid << (2 * i);
    }
    
    // create output vector if needed
    // FIXME: should be part of cell?
    vector <vertex>* vec;
    if(!normal.count(Material))
    {
        vec = new vector< vertex >;
        normal[Material] = vec;
        vec->reserve(256);
    }
    else
    {
        vec = normal[Material];
    }
    // get slope geometry and mix it in
    vector <vertex> * slope = RENDERER->ModelMan->getSlope(surroundings);
    MixVertexVectorsOffset(slope, vec, xTranslate, yTranslate);
    return true;
}

// ugly hack is ugly
void Cube::setShape(Sint16 TileShape)
{
    static Sint16 FloorID = DATA->getLabelIndex("TILESHAPE_FLOOR");
    static Sint16 WallID = DATA->getLabelIndex("TILESHAPE_WALL");
    static Sint16 SlopeID = DATA->getLabelIndex("TILESHAPE_RAMP");
    static Sint16 StairID = DATA->getLabelIndex("TILESHAPE_STAIR");
    static Sint16 FortificationID = DATA->getLabelIndex("TILESHAPE_FORTIFICATION");
    bool IsFloor = (TileShape == FloorID);
    bool IsWall = (TileShape == WallID) || (TileShape == FortificationID);
    bool IsSlope = (TileShape == SlopeID);
    bool IsStairs = (TileShape == StairID);
    
    if(IsWall)
    {
        data.facets = FACET_TOP | FACET_NORTH | FACET_SOUTH | FACET_EAST | FACET_WEST;
        data.solid = true;
        Owner->setActive(true);
        Owner->setTopActive(true);
    }
    else if(IsSlope)
    {
        data.facets = FACET_BOTTOM;
        Owner->setActive(true);
        data.solid = false;
    }
    else if(IsFloor)
    {
        data.facets = FACET_BOTTOM;
        Owner->setActive(true);
        data.solid = false;
    }
    else if(IsStairs)
    {
        data.facets = 0;
        Owner->setActive(true);
        data.solid = false;
    }
    else // empty
    {
        data.facets = 0;
        data.solid = false;
        //Owner->setActive(true);
    }
    
    this->TileShape =  TileShape;
}


//FIXME: stubs
Sint16 Cube::getFacetMaterialType(Facet FacetType)
{
    return Material;
}
Sint16 Cube::getFacetSurfaceType(Facet FacetType)
{
    return 0;
}