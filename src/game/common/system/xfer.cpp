////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: XFER.CPP
//
//        Author:: OmniBlade
//
//  Contributors:: 
//
//   Description:: Some transfer thing interface.
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////
#include "xfer.h"
#include "gamestate.h"
#include "hooker.h"
#include "matrix3d.h"

void Xfer::Open(AsciiString filename)
{
    m_filename = filename;
}

void Xfer::xferVersion(uint8_t *thing, uint8_t check)
{
    xferImplementation(thing, sizeof(*thing));

    ASSERT_THROW_PRINT(*thing > check, 0xD, "Xfer version %d greater than expected, %d.\n", *thing, check);
}

void Xfer::xferByte(int8_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferUnsignedByte(uint8_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferBool(bool *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferInt(int32_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferInt64(int64_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferUnsignedInt(uint32_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferShort(int16_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferUnsignedShort(uint16_t *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferReal(float *thing)
{
    xferImplementation(thing, sizeof(*thing));
}

void Xfer::xferMarkerLabel(AsciiString thing)
{
}

void Xfer::xferAsciiString(AsciiString *thing)
{
    xferImplementation(const_cast<char*>(thing->Str()), thing->Get_Length());
}

void Xfer::xferUnicodeString(UnicodeString *thing)
{
    xferImplementation(const_cast<wchar_t*>(thing->Str()), thing->Get_Length() * 2);
}

void Xfer::xferCoord3D(Coord3D *thing)
{
    xferReal(&thing->x);
    xferReal(&thing->y);
    xferReal(&thing->z);
}

void Xfer::xferICoord3D(ICoord3D *thing)
{
    xferInt(&thing->x);
    xferInt(&thing->y);
    xferInt(&thing->z);
}

void Xfer::xferRegion3D(Region3D *thing)
{
    xferCoord3D(&thing->lo);
    xferCoord3D(&thing->hi);
}

void Xfer::xferIRegion3D(IRegion3D *thing)
{
    xferICoord3D(&thing->lo);
    xferICoord3D(&thing->hi);
}

void Xfer::xferCoord2D(Coord2D *thing)
{
    xferReal(&thing->x);
    xferReal(&thing->y);
}

void Xfer::xferICoord2D(ICoord2D *thing)
{
    xferInt(&thing->x);
    xferInt(&thing->y);
}

void Xfer::xferRegion2D(Region2D *thing)
{
    xferCoord2D(&thing->lo);
    xferCoord2D(&thing->hi);
}

void Xfer::xferIRegion2D(IRegion2D *thing)
{
    xferICoord2D(&thing->lo);
    xferICoord2D(&thing->hi);
}

void Xfer::xferRealRange(RealRange *thing)
{
    xferReal(&thing->lo);
    xferReal(&thing->hi);
}

void Xfer::xferColor(int32_t thing)
{
    xferInt(&thing);
}

void Xfer::xferRGBColor(RGBColor *thing)
{
    xferReal(&thing->red);
    xferReal(&thing->green);
    xferReal(&thing->blue);
}

void Xfer::xferRGBAColorReal(RGBAColorReal *thing)
{
    xferReal(&thing->red);
    xferReal(&thing->green);
    xferReal(&thing->blue);
    xferReal(&thing->alpha);
}

void Xfer::xferRGBAColorInt(RGBAColorInt *thing)
{
    xferUnsignedInt(&thing->red);
    xferUnsignedInt(&thing->green);
    xferUnsignedInt(&thing->blue);
    xferUnsignedInt(&thing->alpha);
}

void Xfer::xferObjectID(ObjectID *thing)
{
    xferInt(reinterpret_cast<int32_t*>(thing));
}

void Xfer::xferDrawableID(DrawableID *thing)
{
    xferInt(reinterpret_cast<int32_t*>(thing));
}

void Xfer::xferSTLObjectIDVector(std::vector<ObjectID> *thing)
{
    uint8_t ver = 1;
    xferVersion(&ver, 1);

    uint16_t count = (uint16_t)thing->size();
    xferUnsignedShort(&count);

    if ( Get_Mode() == XFER_SAVE || Get_Mode() == XFER_CRC ) {
        for ( auto it = thing->begin(); it != thing->end(); ++it ) {
            xferObjectID(&(*it));
        }
    } else {
        ASSERT_THROW_PRINT(Get_Mode() == XFER_LOAD, 0x8, "Xfer mode unknown.\n");
        ASSERT_THROW_PRINT(thing->size(), 0xF, "Trying to xfer load to none empty vector.\n");

        ObjectID val;

        for ( int i = 0; i < count; ++i ) {
            xferObjectID(&val);
            thing->insert(thing->end(), val);
        }
    }
}

void Xfer::xferSTLObjectIDList(std::list<ObjectID> *thing)
{
    uint8_t ver = 1;
    xferVersion(&ver, 1);

    uint16_t count = (uint16_t)thing->size();
    xferUnsignedShort(&count);

    if ( Get_Mode() == XFER_SAVE || Get_Mode() == XFER_CRC ) {
        for ( auto it = thing->begin(); it != thing->end(); ++it ) {
            xferObjectID(&(*it));
        }
    } else {
        ASSERT_THROW_PRINT(Get_Mode() == XFER_LOAD, 0x8, "Xfer mode unknown.\n");
        ASSERT_THROW_PRINT(thing->size() == 0, 0xF, "Trying to xfer load to none empty vector.\n");

        ObjectID val;

        for ( int i = 0; i < count; ++i ) {
            xferObjectID(&val);
            thing->insert(thing->end(), val);
        }
    }
}

void Xfer::xferSTLIntList(std::list<int32_t> *thing)
{
    if ( thing == nullptr ) {
        return;
    }

    uint8_t ver = 1;
    xferVersion(&ver, 1);

    uint16_t count = (uint16_t)thing->size();
    xferUnsignedShort(&count);

    if ( Get_Mode() == XFER_SAVE || Get_Mode() == XFER_CRC ) {
        for ( auto it = thing->begin(); it != thing->end(); ++it ) {
            xferInt(&(*it));
        }
    } else {
        ASSERT_THROW_PRINT(Get_Mode() == XFER_LOAD, 0x8, "Xfer mode unknown.\n");
        ASSERT_THROW_PRINT(thing->size() == 0, 0xF, "Trying to xfer load to none empty vector.\n");

        int32_t val;

        for ( int i = 0; i < count; ++i ) {
            xferInt(&val);
            thing->insert(thing->end(), val);
        }
    }
}

void Xfer::xferScienceType(ScienceType *thing)
{
    // TODO, needs parts of ScienceStore
}

void Xfer::xferScienceVec(std::vector<ScienceType> *thing)
{
    // TODO, needs parts of ScienceStore
}

void Xfer::xferKindOf(KindOfType *thing)
{
    uint8_t ver = 1;
    xferVersion(&ver, 1);

    AsciiString kind;
    const char *kindc;

    switch ( Get_Mode() ) {
        case XFER_SAVE:
            if ( *thing >= KINDOF_FIRST || *thing < KINDOF_COUNT ) {
                kind = BitFlags<KINDOF_COUNT>::s_bitNamesList[*thing];
            }

            xferAsciiString(&kind);

            break;
        case XFER_LOAD:
            xferAsciiString(&kind);
            kindc = kind.Str();

            for ( int i = 0; BitFlags<KINDOF_COUNT>::s_bitNamesList[i] != nullptr; ++i ) {
                if ( strcasecmp(kindc, BitFlags<KINDOF_COUNT>::s_bitNamesList[i]) == 0 ) {
                    *thing = (KindOfType)i;
                    break;
                }
            }

            break;
        case XFER_CRC:
            xferImplementation(thing, sizeof(*thing));
            break;
        default:
            ASSERT_THROW_PRINT(false, 0x8, "Xfer mode unknown.\n");
            break;
    }
}

void Xfer::xferUpgradeMask(BitFlags<128> *thing)
{
    // TODO, needs part of UpgradeCenter
}

void Xfer::xferUser(void *thing, int size)
{
    xferImplementation(thing, size);
}

void Xfer::xferMatrix3D(Matrix3D *thing)
{
    xferReal(&(*thing)[0][0]);
    xferReal(&(*thing)[0][1]);
    xferReal(&(*thing)[0][2]);
    xferReal(&(*thing)[0][3]);
    xferReal(&(*thing)[1][0]);
    xferReal(&(*thing)[1][1]);
    xferReal(&(*thing)[1][2]);
    xferReal(&(*thing)[1][3]);
    xferReal(&(*thing)[2][0]);
    xferReal(&(*thing)[2][1]);
    xferReal(&(*thing)[2][2]);
    xferReal(&(*thing)[2][3]);
}

void Xfer::xferMapName(AsciiString *thing)
{
    AsciiString map;

    if ( Get_Mode() == XFER_SAVE ) {
        map = g_theGameState->Real_To_Portable_Map_Path(*thing);
        xferAsciiString(&map);
    } else if ( Get_Mode() == XFER_LOAD ) {
        xferAsciiString(&map);
        *thing = g_theGameState->Portable_To_Real_Map_Path(map);
    }
}
