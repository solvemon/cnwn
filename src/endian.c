#include "cnwn/endian.h"

int64_t cnwn_endian_swap64(int64_t i)
{
    return ((i >> 56)
            | ((i & 0xff000000000000) >> 40)
            | ((i & 0xff0000000000) >> 24)
            | ((i & 0xff00000000) >> 8)
            | ((i & 0xff000000) << 8)
            | ((i & 0xff0000) << 24)
            | ((i & 0xff00) << 40)
            | ((i & 0xff) << 56));
}

int32_t cnwn_endian_swap32(int32_t i)
{
    return ((i >> 24)
            | ((i & 0xff0000) >> 8)
            | ((i & 0xff00) << 8)
            | ((i & 0xff) << 24));
}

int16_t cnwn_endian_swap16(int16_t i)
{
    return ((i >> 8) | ((i & 0xff) << 8));
}

int64_t cnwn_endian_big64(int64_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return i;
#else
    return ((i >> 56)
            | ((i & 0xff000000000000) >> 40)
            | ((i & 0xff0000000000) >> 24)
            | ((i & 0xff00000000) >> 8)
            | ((i & 0xff000000) << 8)
            | ((i & 0xff0000) << 24)
            | ((i & 0xff00) << 40)
            | ((i & 0xff) << 56));
#endif
}

int32_t cnwn_endian_big32(int32_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return i;
#else
    return ((i >> 24)
            | ((i & 0xff0000) >> 8)
            | ((i & 0xff00) << 8)
            | ((i & 0xff) << 24));
#endif
}

int16_t cnwn_endian_big16(int16_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return i;
#else
    return ((i >> 8) | ((i & 0xff) << 8));
#endif
}

int64_t cnwn_endian_little64(int64_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return ((i >> 56)
            | ((i & 0xff000000000000) >> 40)
            | ((i & 0xff0000000000) >> 24)
            | ((i & 0xff00000000) >> 8)
            | ((i & 0xff000000) << 8)
            | ((i & 0xff0000) << 24)
            | ((i & 0xff00) << 40)
            | ((i & 0xff) << 56));
#else
    return i;
#endif
}

int32_t cnwn_endian_little32(int32_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return ((i >> 24)
            | ((i & 0xff0000) >> 8)
            | ((i & 0xff00) << 8)
            | ((i & 0xff) << 24));
#else
    return i;
#endif
}

int16_t cnwn_endian_little16(int16_t i)
{
#ifdef BUILD_BIG_ENDIAN
    return ((i >> 8) | ((i & 0xff) << 8));
#else
    return i;
#endif
}
