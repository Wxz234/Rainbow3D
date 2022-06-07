#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// Resource bind info for _HistogramBuffer
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// _HistogramBuffer                      UAV  struct         r/w             u0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_uav_structured u0, 4
dcl_input vThreadID.x
dcl_temps 1
dcl_thread_group 16, 1, 1
ult r0.x, vThreadID.x, l(128)
if_nz r0.x
  store_structured u0.x, vThreadID.x, l(0), l(0)
endif 
ret 
// Approximately 5 instruction slots used
#endif

const BYTE KEyeHistogramClear[] =
{
     68,  88,  66,  67,  65, 118, 
     40,  20, 187,  61,  23, 236, 
    220, 211,  58,  51, 181,  59, 
     67,   5,   1,   0,   0,   0, 
    148,   2,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     72,   1,   0,   0,  88,   1, 
      0,   0, 104,   1,   0,   0, 
    248,   1,   0,   0,  82,  68, 
     69,  70,  12,   1,   0,   0, 
      1,   0,   0,   0, 112,   0, 
      0,   0,   1,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
    228,   0,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     92,   0,   0,   0,   6,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  95,  72, 105, 115, 
    116, 111, 103, 114,  97, 109, 
     66, 117, 102, 102, 101, 114, 
      0, 171, 171, 171,  92,   0, 
      0,   0,   1,   0,   0,   0, 
    136,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0, 176,   0, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 192,   0,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  36,  69, 108, 101, 
    109, 101, 110, 116,   0, 100, 
    119, 111, 114, 100,   0, 171, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 185,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  49, 
     48,  46,  49,   0,  73,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  79,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     83,  72,  69,  88, 136,   0, 
      0,   0,  80,   0,   5,   0, 
     34,   0,   0,   0, 106,   8, 
      0,   1, 158,   0,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     95,   0,   0,   2,  18,   0, 
      2,   0, 104,   0,   0,   2, 
      1,   0,   0,   0, 155,   0, 
      0,   4,  16,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  79,   0,   0,   6, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,   2,   0, 
      1,  64,   0,   0, 128,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 168,   0,   0,   8, 
     18, 224,  17,   0,   0,   0, 
      0,   0,  10,   0,   2,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   1,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0
};
