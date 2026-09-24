#ifndef __PALETTE_ALL_H
#define __PALETTE_ALL_H

/////////////////////////////////////////////////////
// Transform palette list
enum TransformPalList{
  tl_None = 0,
  tl_Default, 
  tl_Plasma,
  tl_Confetti,
  tl_Christmas,
  tl_Autumn,
  tl_Halloween,
  tl_July_4th,
  tl_TwinkleFox,
  tl_SnowAndIce,
  tl_Valentines,
  tl_PortugalFlag,
  tl_IsraelFlag,
  tl_Noise,
  tl_Total
};


//////////////////////////////////////////////////////
// Color schemes - set of transforming theme palettes 
#define DECLARE_TRANSFORM_SCHEMA_TYPE(FunctionName) void FunctionName(CRGBPalette16 &pal);


#define BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, Type) \
void FunctionName(CRGBPalette16 &pal){ \
  struct _ts{ \
    const Type &item; \
  } const ts[] = {


#define BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(FunctionName)  BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, TProgmemRGBGradientPaletteRef)
#define BEGIN_TRANSFORM_SCHEMA_RGB16_PALETTE(FunctionName)  BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, TProgmemRGBPalette16)


#define END_TRANSFORM_SCHEMA() }; \
  pal = ts[random8(0, sizeof(ts) / sizeof(ts[0]))].item; \
} 

#define TRANSOFRM_PALETTE(pal) pal,

/*
Usage of transform shchema: 
BEGIN_TRANFORM_SCHEMA_XXX(YourFunctionName)
  TRANSOFRM_PALETTE(pallete1)
  TRANSOFRM_PALETTE(palette2)
  ...
  TRANSOFRM_PALETTE(paletteN)
END_TRANSFORM_SCHEMA()

It creates 
void YourFunctionName(CRGBPalette16 &pal);
*/


//////////////////////////////////////////////////////
// Transform functions

//Palette change routine
typedef void (*FuncGetPalette_t) (CRGBPalette16 &);

//Defualt
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Default)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Plasma);
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Confetti)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Christmas)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Autunm)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Halloween)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_July4th)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_TwinkleFox)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_SnowAndIce)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_ValentinesDay)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_PortugalFlag)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_Noise)

///////////////////////////////////////////////////
// Retrieving transform function


//Tranform description
struct TRANSFORM_DESCRIPTION{ 
  uint8_t          transformId;             //Trasnform id from TranformList
  FuncGetPalette_t tFunc;                   //Pointer to an tranform function  
#ifdef NTF_ENABLED  
  const char *transformName;                //Name of the tranform
#endif  
};

bool getPalTransform(uint8_t transformId, TRANSFORM_DESCRIPTION &td);



#endif //_PALETTE_ALL_H
