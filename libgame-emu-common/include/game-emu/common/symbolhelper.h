#pragma once

#ifdef _WIN32
#define GAMEEMU_INTERFACE_EXPORT __declspec(dllexport)
#else
#define GAMEEMU_INTERFACE_EXPORT __attribute__((visibility("default")))
#endif

#ifdef _LIBGAMEEMU_COMMON_DLL_EXPORT_SYMBOLS

#ifdef _WIN32
#define LIBGAMEEMU_COMMON_DLL_EXPORT __declspec(dllexport)
#else
#define LIBGAMEEMU_COMMON_DLL_EXPORT __attribute__((visibility("default")))
#endif

#else

#ifdef _WIN32
#define LIBGAMEEMU_COMMON_DLL_EXPORT __declspec(dllimport)
#else
#define LIBGAMEEMU_COMMON_DLL_EXPORT
#endif

#endif
