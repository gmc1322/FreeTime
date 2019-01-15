//---------------------------------------------------------
// file:	CP_Export.h
// author:	Justin Chambers
// brief:	Header used to manage the DLL export/import declarations
//
//	DO NOT INCLUDE THIS HEADER IN YOUR PROJECTS DIRECTLY
//	SEE: C_Processing.h
//
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once

//---------------------------------------------------------
// If you have questions about what this is doing, ask Justin to explain.
//---------------------------------------------------------

#ifdef CPROCESSING_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif