#ifndef __PALETTE_H
#define __PALETTE_H

//Halloween
DECLARE_GRADIENT_PALETTE( halloween_gp);

//Autumn 
DECLARE_GRADIENT_PALETTE( es_autumn_01_gp );
DECLARE_GRADIENT_PALETTE( es_autumn_02_gp );
DECLARE_GRADIENT_PALETTE( es_autumn_03_gp );
DECLARE_GRADIENT_PALETTE( es_autumn_04_gp );
DECLARE_GRADIENT_PALETTE( es_autumn_20_gp );
DECLARE_GRADIENT_PALETTE( es_autumn_16_gp );

//Christmas
DECLARE_GRADIENT_PALETTE( christmattree1_gp );

//US Patriot
DECLARE_GRADIENT_PALETTE( patriot_gp );
DECLARE_GRADIENT_PALETTE( patriot_black_gp );
DECLARE_GRADIENT_PALETTE( july4_2_gp );

//Other usefull palettes
extern const TProgmemRGBPalette16 UkraineFlag_p FL_PROGMEM;
extern const TProgmemRGBPalette16 PortugalFlag_p FL_PROGMEM;
extern const TProgmemRGBPalette16 RedGreenWhite_p FL_PROGMEM;
extern const TProgmemRGBPalette16 Holly_p FL_PROGMEM;
extern const TProgmemRGBPalette16 RedWhite_p FL_PROGMEM;
extern const TProgmemRGBPalette16 BlueWhite_p FL_PROGMEM; 
extern const TProgmemRGBPalette16 FairyLight_p FL_PROGMEM;
extern const TProgmemRGBPalette16 Snow_p FL_PROGMEM;
extern const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM;
extern const TProgmemRGBPalette16 Ice_p FL_PROGMEM;

//Pacifica
extern const TProgmemRGBPalette16 Pacifica_1_p FL_PROGMEM;
extern const TProgmemRGBPalette16 Pacifica_2_p FL_PROGMEM;
extern const TProgmemRGBPalette16 Pacifica_3_p FL_PROGMEM;

//Valentines day
DECLARE_GRADIENT_PALETTE(firstlove_gp); 
DECLARE_GRADIENT_PALETTE(roseedan_gp); 
DECLARE_GRADIENT_PALETTE(passionata_gp); 


//////////////////////////////////////////////////////
// Color schemes - set of transforming theme palettes 
#define DECLARE_TRANSFORM_SCHEMA_TYPE(FunctionName) void FunctionName(CRGBPalette16 &pal);


#define BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, Type) \
void FunctionName(CRGBPalette16 &pal){ \
  struct _ts{ \
    const Type &item; \
  } const ts[] = {


#define BEGIN_TRANSFORM_SCHEMA_GRADIENT_PALETTE(FunctionName)  BEGIN_TRANSFORM_SCHEMA_TYPE(FunctionName, TProgmemRGBGradientPalettePtr)
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
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_UkraineFlag)
DECLARE_TRANSFORM_SCHEMA_TYPE(getPal_PortugalFlag)

///////////////////////////////////////////////////
// Retrieving transform function
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
  tl_UkrainianFlag,
  tl_PortugalFlag,
  tl_Total
};

//Tranform description
struct TRANSFORM_DESCRIPTION{ 
  uint8_t          transformId;             //Trasnform id from TranformList
  FuncGetPalette_t tFunc;                   //Pointer to an tranform function  
#ifdef NTF_ENABLED  
  const char *transformName;                //Name of the tranform
#endif  
};

bool getPalTransform(uint8_t transformId, TRANSFORM_DESCRIPTION &td);


#endif //_PALETTE_H
