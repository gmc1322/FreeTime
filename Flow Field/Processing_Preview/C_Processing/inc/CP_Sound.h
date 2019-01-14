//------------------------------------------------------------------------------
//   file:	CP_Stub.h
// author:	Daniel Hamilton
//   date:  5/10/2018
//  brief:	[REPLACE ME] Stub file to be copied to start new files 
//
// Copyright © 2018 DigiPen, All rights reserved.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "CP_Export.h"
#include "fmod_common.h"

//------------------------------------------------------------------------------
// Defines:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Consts:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

typedef struct PSound_struct* PSound;
typedef struct PChannel_struct* PChannel;

//------------------------------------------------------------------------------
// Public Enums:
//------------------------------------------------------------------------------

typedef enum AudioChannel
{
    CP_CHANNEL_MUSIC,
    CP_CHANNEL_SFX,
} AudioChannel;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

DLL_EXPORT PSound createSound(const char * filepath, int loop);
DLL_EXPORT void   sound(PSound sound);
DLL_EXPORT void   soundChannel(PSound sound, PChannel channel);



//////////////////////////////////////////////////////////
//                   CHANNEL CONTROL                    //
//////////////////////////////////////////////////////////
DLL_EXPORT PChannel createChannel(const char * name);

DLL_EXPORT void     stop(PChannel channel);

DLL_EXPORT void     setPaused(PChannel channel, int paused);
DLL_EXPORT int      getPaused(PChannel channel);

DLL_EXPORT void     setVolume(PChannel channel, float volume); // [0, 1]
DLL_EXPORT float    getVolume(PChannel channel);

DLL_EXPORT void     setPitch(PChannel channel, float pitch);
DLL_EXPORT float    getPitch(PChannel channel);

DLL_EXPORT void     setFade(PChannel channel, float end_volume, float fade_time_seconds);
DLL_EXPORT void     clearFade(PChannel channel);

DLL_EXPORT void     setReverb(PChannel channel, float wet); // [0, 1]
DLL_EXPORT float    getReverb(PChannel channel); 

DLL_EXPORT void     setPan(PChannel channel, float pan);
DLL_EXPORT float    getPan(PChannel channel);