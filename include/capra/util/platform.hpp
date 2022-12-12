#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# define CAPRA_PLATFORM_WINDOWS
# if defined(WIN64) || defined(_WIN64)
#   define CAPRA_PLATFORM_WINDOWS64
# else
#   define CAPRA_PLATFORM_WINDOWS32
# endif
#elif __APPLE__
# include <TargetConditionals.h>
# if TARGET_IPHONE_SIMULATOR
#   define CAPRA_PLATFORM_APPLE_IOS_SIMULATOR
# elif TARGET_OS_MACCATALYST
#   define CAPRA_PLATFORM_APPLE_CATALYST
# elif TARGET_OS_IPHONE
#   define CAPRA_PLATFORM_APPLE_IOS
# elif TARGET_OS_MAC
#   define CAPRA_PLATFORM_APPLE
# else
#   error "Unknown Apple platform"
# endif
#elif __linux__
# define CAPRA_PLATFORM_LINUX
#elif __unix__
# define CAPRA_PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
# define CAPRA_PLATFORM_POSIX
#else
# error "Unknown compiler"
#endif
