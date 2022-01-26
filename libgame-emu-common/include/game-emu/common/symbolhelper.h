#pragma once

#ifdef _LIBGAMEEMU_COMMON_DLL_EXPORT_SYMBOLS
#define LIBGAMEEMU_COMMON_DLL_EXPORT __declspec(dllexport)
#else
#define LIBGAMEEMU_COMMON_DLL_EXPORT __declspec(dllimport)
#endif
