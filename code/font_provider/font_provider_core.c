////////////////////////////////
//~ rjf: Basic Handle Functions

root_function FP_Handle
FP_HandleZero(void)
{
 FP_Handle result = {0};
 return result;
}

root_function B32
FP_HandleMatch(FP_Handle a, FP_Handle b)
{
 return (a.u64[0] == b.u64[0] &&
         a.u64[1] == b.u64[1] &&
         a.u64[2] == b.u64[2] &&
         a.u64[3] == b.u64[3]);
}
