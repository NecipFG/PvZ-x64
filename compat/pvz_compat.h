/*
 * pvz_compat.h — Force-included compatibility header for MinGW cross-build.
 *
 * Provides mixed-type min/max overloads matching MSVC's Windows.h macro
 * behaviour (e.g. max(double,int), min(int,SeedType)).
 *
 * Uses SFINAE-constrained templates instead of macros so that libstdc++
 * internals that call std::min/max with matching types are NOT affected.
 * For same-type calls std::min/std::max win via overload resolution;
 * for mixed-type calls our overloads are the only viable candidates.
 */
#pragma once

/* Tell libjpeg that INT32 is already defined by Windows headers */
#define XMD_H

/* SHANDLE_PTR missing from older MinGW exdisp.h */
#ifndef SHANDLE_PTR
#ifdef _WIN64
typedef long long SHANDLE_PTR;
#else
typedef long SHANDLE_PTR;
#endif
#endif

#define NOMINMAX          /* keep windows.h from defining min/max macros */
#include <windows.h>
#include <math.h>
#include <float.h>
#include <time.h>

// MinGW time defines cause conflicts, so we simply redefine the usages
// inline inside the implementation. Wait, instead of defines here that
// leak into sec_api, let's fix it by defining __time32_t and _time32
// more carefully or rely on standard time_t.
#define _time32 time
#define __time32_t long

#ifdef __cplusplus
#include <algorithm>      /* std::min / std::max */
#include <type_traits>    /* std::enable_if, std::is_same, std::common_type */

/* Bring std::min/max into global namespace for same-type calls */
using std::min;
using std::max;

/*
 * Mixed-type overloads — only enabled when A != B (SFINAE).
 * Converts both args to the common type, then delegates to std::max/min.
 * These live in global namespace alongside the using-declarations above.
 */
template<typename A, typename B>
inline typename std::enable_if<
    !std::is_same<A, B>::value,
    typename std::common_type<A, B>::type
>::type
max(A a, B b)
{
    typedef typename std::common_type<A, B>::type C;
    return std::max(static_cast<C>(a), static_cast<C>(b));
}

template<typename A, typename B>
inline typename std::enable_if<
    !std::is_same<A, B>::value,
    typename std::common_type<A, B>::type
>::type
min(A a, B b)
{
    typedef typename std::common_type<A, B>::type C;
    return std::min(static_cast<C>(a), static_cast<C>(b));
}
#endif /* __cplusplus */
