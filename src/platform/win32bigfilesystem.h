////////////////////////////////////////////////////////////////////////////////
//                               --  THYME  --                                //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: WIN32BIGFILESYSTEM.H
//
//        Author:: OmniBlade
//
//  Contributors:: 
//
//   Description:: Implements the ArchiveFileSystem interface on the BIG file
//                 format.
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
#pragma once

#ifndef WIN32BIGFILESYSTEM_H
#define WIN32BIGFILESYSTEM_H

#include "archivefilesystem.h"

class Win32BIGFileSystem : public ArchiveFileSystem
{
    enum
    {
        BIG_PATH_MAX = 260,
    };
public:
    Win32BIGFileSystem() {}
    virtual ~Win32BIGFileSystem() {}

    // SubsystemInterface implementations
    virtual void Init();
    virtual void Reset() {}
    virtual void Update() {}

    // ArchiveFileSystem implementations
    virtual ArchiveFile *Open_Archive_File(const char *filename);
    virtual void Close_Archive_File(const char *filename);
    virtual void Close_All_Archives() {}
    virtual void Close_All_Files() {}
    virtual void Load_Archives_From_Dir(AsciiString dir, AsciiString filter, bool read_subdirs);
};

#endif
