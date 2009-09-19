#ifndef ENUM__HEADER
#define ENUM__HEADER

enum ActorType
{
	FACE_ACTOR,
	SLOPE_ACTOR,
	CUBE_ACTOR,
	CELL_ACTOR,
	PAWN_ACTOR,

	NUM_ACTORS
};
/*
enum Facet
{
	FACET_TOP,
	FACET_BOTTOM,
	FACET_NORTH,
	FACET_SOUTH,
	FACET_EAST,
	FACET_WEST,

	NUM_FACETS,
	FACETS_START = 0
};
*/

enum Facet
{
    FACET_INVALID = 0x0,
    FACET_TOP = 0x1,
    FACET_BOTTOM = 0x2,
    FACET_NORTH = 0x4,
    FACET_SOUTH = 0x8,
    FACET_EAST = 0x10,
    FACET_WEST = 0x20,
};

inline Facet &operator++ (Facet &OldFace)      { return OldFace = Facet(OldFace + 1); }
inline Facet OppositeFacet(Facet source)
{
    Facet opposite[] =
    {
        FACET_INVALID,
        FACET_BOTTOM,
        FACET_TOP,
        FACET_INVALID,
        FACET_SOUTH,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_NORTH,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_WEST,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_INVALID,
        FACET_EAST
    };
    return opposite[source];
}
/*
enum Slopping
{
    SLOPE_FLAT,

    SLOPE_NORTH_EAST,
    SLOPE_SOUTH_EAST,
    SLOPE_SOUTH_WEST,
    SLOPE_NORTH_WEST,

    SLOPE_LARGE_NORTH,
    SLOPE_LARGE_EAST,
    SLOPE_LARGE_SOUTH,
    SLOPE_LARGE_WEST,

    SLOPE_SMALL_NORTH,
    SLOPE_SMALL_EAST,
    SLOPE_SMALL_SOUTH,
    SLOPE_SMALL_WEST,

    SLOPE_CRESS_NORTH_SOUTH,
    SLOPE_CRESS_EAST_WEST,

    NUM_SLOPES,
    SLOPES_START = 0
};

inline Slopping &operator++ (Slopping &OldSlope)      { return OldSlope = Slopping(OldSlope + 1); }
*/
enum Direction
{
    DIRECTION_NORTHWEST,
	DIRECTION_NORTH,
	DIRECTION_NORTHEAST,
	DIRECTION_EAST,
	DIRECTION_SOUTHEAST,
	DIRECTION_SOUTH,
	DIRECTION_SOUTHWEST,
	DIRECTION_WEST,
	DIRECTION_UP,
    DIRECTION_DOWN,

	NUM_DIRECTIONS,
    DIRECTIONS_START = 0
};

inline Direction &operator++ (Direction &OldDirection)      { return OldDirection = Direction(OldDirection + 1); }

enum CameraOrientation
{
    CAMERA_DOWN,
	CAMERA_NORTH_WEST,
	CAMERA_NORTH_EAST,
	CAMERA_SOUTH_WEST,
	CAMERA_SOUTH_EAST,

	NUM_ORIENTATIONS,
    ORIENTATIONS_START = 0
};

inline CameraOrientation &operator++ (CameraOrientation &Orientation)      { return Orientation = CameraOrientation(Orientation + 1); }

enum SpacialPoint
{
    SPACIAL_POINT_CENTER,

    SPACIAL_POINT_NORTH_TOP,
    SPACIAL_POINT_EAST_TOP,
    SPACIAL_POINT_SOUTH_TOP,
    SPACIAL_POINT_WEST_TOP,

    SPACIAL_POINT_NORTH_BOTTOM,
    SPACIAL_POINT_EAST_BOTTOM,
    SPACIAL_POINT_SOUTH_BOTTOM,
    SPACIAL_POINT_WEST_BOTTOM,

    NUM_SPACIAL_POINTS
};

#endif // ENUM_HEADER
