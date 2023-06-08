/* date = April 2nd 2023 0:12 pm */

#ifndef BASE_RANDOM_H
#define BASE_RANDOM_H

typedef struct Dist Dist;
struct Dist
{
 U64 u64[8];
};

core_function Dist DistMakeUniform(U64 seed);

core_function U32 SampleFromDist1U32(Dist *dist, Rng1U32 range);
#define SampleFromDist1S32(dist, range) (S32)SampleFromDist1U32((dist), (range))

#endif // BASE_RANDOM_H
