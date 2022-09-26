#ifndef _AS_D3D_UTILS_
#define _AS_D3D_UTILS_

#pragma once

#include <math.h>
#include <D3DX9Math.h>

//
// forward declares (prevents includes thus reducing compiletime)
//
class asIScriptEngine;
class asIScriptContext;



//---------------------------------------------------------------------------------------------------------------------
//      Utility functions
//---------------------------------------------------------------------------------------------------------------------


//
// gClamp - clamps variables between a min and a max value
//
template<typename T>
__forceinline const T&  gClamp(const T& inValue, const T& inLow, const T& inHigh){ if (inValue<inLow) return inLow; else if (inValue>inHigh) return inHigh; else return inValue; }
// Template uses references : for floats it's faster not using references
__forceinline float             gClamp(float inValue, float inLow, float inHigh)                { if (inValue<inLow) return inLow; else if (inValue>inHigh) return inHigh; else return inValue; }




//---------------------------------------------------------------------------------------------------------------------
//
//      Vector math :
//
//      Vectors are derived from the D3DX structs, so all D3DX-VectorMath functions will accept them as input.
//
//---------------------------------------------------------------------------------------------------------------------



//
// Vector2 : 2 dimensional vector
//
struct Vector2 : public D3DXVECTOR2
{
 Vector2() : D3DXVECTOR2() { }
 Vector2(const float * inData) : D3DXVECTOR2(inData) { }
 Vector2(const D3DXFLOAT16 * inData) : D3DXVECTOR2(inData) { }
 Vector2(float inX, float inY) : D3DXVECTOR2(inX, inY) { }
                                                        
  float                                 SquareLength() const                            { return x*x + y*y; }
  float                                 Length() const                                          { return float(sqrt(SquareLength())); }
  float                                 Dot(const Vector2 &inRHS) const         { return x*inRHS.x + y*inRHS.y; }
  float                                 Cross(const Vector2 &inRHS) const       { return x * inRHS.y - y * inRHS.x; }
                                                        
  void                                  Normalize()                                                     { float length = Length(); if (length != 0.0f) { x/=length;y/=length; } }
  Vector2                                       Normalized() const                                      { Vector2 v(*this); v.Normalize(); return v; }

  Vector2                                       GetPerpendicular() const                        { return Vector2(-y, x); }
  Vector2                                       GetNormalizedPerpendicular() const      { return Vector2(-y, x).Normalized(); }

  // script functions
  static void   _cdecl          ASConstruct(Vector2 & inThis);
  static Vector2 & _cdecl       ASAssign(const Vector2 & inSource, Vector2 & inDest);
  static Vector2 _cdecl ASOperatorAdd(const Vector2 & inLHS, const Vector2 & inRHS);
  static Vector2 _cdecl ASOperatorSub(const Vector2 & inLHS, const Vector2 & inRHS);
  static Vector2 _cdecl ASOperatorMul(const Vector2 & inLHS, const float inRHS);
  static Vector2 _cdecl ASOperatorDiv(const Vector2 & inLHS, const float inRHS);
  static bool   _cdecl          ASOperatorEqual(const Vector2 & inLHS, const Vector2 &inRHS);
  static bool   _cdecl          ASOperatorNotEqual(const Vector2 & inLHS, const Vector2 &inRHS);

  void                                  Set(float inX, float inY)                       { x = inX; y = inY; }
  void                                  Add(Vector2 & inRHS)                            { x+= inRHS.x; y+=inRHS.y; }
  void                                  Sub(Vector2 & inRHS)                            { x-= inRHS.x; y-=inRHS.y; }
  void                                  Scale(float inScale)                            { x*= inScale; y*=inScale; }
  void                                  Blend(const Vector2 & inOther, float inFactor)          { x = x + (inOther.x - x) * inFactor; y = y + (inOther.y - y) * inFactor; }

  inline bool                           operator == (const Vector2 & inRHS) const       { return ((inRHS.x == x) && (inRHS.y == y)); }
  inline bool                           operator != (const Vector2 & inRHS) const       { return ((inRHS.x != x) || (inRHS.y != y)); }
  inline const Vector2  operator - () const                                                     { return Vector2(-x, -y); }
  inline const Vector2  operator + (const Vector2 & inRHS) const        { return Vector2(x+inRHS.x, y+inRHS.y); }
  inline const Vector2  operator - (const Vector2 & inRHS) const        { return Vector2(x-inRHS.x, y-inRHS.y); }
  inline const Vector2  operator * (const float & inF) const            { return Vector2(x*inF, y*inF); }
  inline const Vector2  operator / (const float & inF) const            { return Vector2(x/inF, y/inF); }
  inline Vector2&                       operator += (const Vector2 & inRHS)                     { x+=inRHS.x; y+=inRHS.y; return *this; }
  inline Vector2&                       operator -= (const Vector2 & inRHS)                     { x-=inRHS.x; y-=inRHS.y; return *this; }
  inline Vector2&                       operator *= (const float & inF)                         { x*=inF; y*=inF; return *this; }
  inline Vector2&                       operator /= (const float & inF)                         { x/=inF; y/=inF; return *this; }

  static const Vector2  sZero;
  static const Vector2  sOne;
};



//
// Vector3 : 3 dimensional vector
//
struct Vector3 : public D3DXVECTOR3
{
 Vector3() : D3DXVECTOR3() { }
 Vector3(const float * inData) : D3DXVECTOR3(inData) { }
 Vector3(const D3DXFLOAT16 * inData) : D3DXVECTOR3(inData) { }
 Vector3(float inX, float inY, float inZ) : D3DXVECTOR3(inX, inY, inZ) { }
                                                        
  float                                 SquareLength() const                            { return x*x + y*y + z*z; }
  float                                 Length() const                                          { return float(sqrt(SquareLength())); }
  float                                 Dot(const Vector3 &inRHS) const         { return x*inRHS.x + y*inRHS.y + z*inRHS.z; }
  Vector3                                       Cross(const Vector3 &inRHS) const       { return Vector3(y * inRHS.z - z * inRHS.y,     z * inRHS.x - x * inRHS.z,      x * inRHS.y - y * inRHS.x); }
                                                
  void                                  Normalize()                                                     { float length = Length(); if (length != 0.0f) { x/=length;y/=length;z/=length; } }
  Vector3                                       Normalized() const                                      { Vector3 v(*this); v.Normalize(); return v; }

  // script functions
  static void   _cdecl          ASConstruct(Vector3 & inThis);
  static Vector3&       _cdecl  ASAssign(const Vector3 & inSource, Vector3 & inDest);
  static Vector3 _cdecl ASOperatorAdd(const Vector3 & inLHS, const Vector3 & inRHS);
  static Vector3 _cdecl ASOperatorSub(const Vector3 & inLHS, const Vector3 & inRHS);
  static Vector3 _cdecl ASOperatorMul(const Vector3 & inLHS, const float inRHS);
  static Vector3 _cdecl ASOperatorDiv(const Vector3 & inLHS, const float inRHS);
  static bool   _cdecl          ASOperatorEqual(const Vector3 & inLHS, const Vector3 &inRHS);
  static bool _cdecl            ASOperatorNotEqual(const Vector3 & inLHS, const Vector3 &inRHS);

  void                                  Set(float inX, float inY, float inZ)    { x = inX; y = inY; z = inZ; }
  void                                  Add(Vector3 & inRHS)                            { x+= inRHS.x; y+=inRHS.y; z+=inRHS.z; }
  void                                  Sub(Vector3 & inRHS)                            { x-= inRHS.x; y-=inRHS.y; z-=inRHS.z;}
  void                                  Scale(float inScale)                            { x*= inScale; y*=inScale; z*=inScale;}
  void                                  Blend(const Vector3 & inOther, float inFactor)          { x = x + (inOther.x - x) * inFactor; y = y + (inOther.y - y) * inFactor; z = z + (inOther.z - z) * inFactor; }

  inline bool                           operator == (const Vector3 & inRHS) const       { return ((inRHS.x == x) && (inRHS.y == y) && (inRHS.z == z)); }
  inline bool                           operator != (const Vector3 & inRHS) const       { return ((inRHS.x != x) || (inRHS.y != y) || (inRHS.z != z)); }
  inline const Vector3  operator - () const                                                     { return Vector3(-x, -y, -z); }
  inline const Vector3  operator + (const Vector3 & inRHS) const        { return Vector3(x+inRHS.x, y+inRHS.y, z+inRHS.z); }
  inline const Vector3  operator - (const Vector3 & inRHS) const        { return Vector3(x-inRHS.x, y-inRHS.y, z-inRHS.z); }
  inline const Vector3  operator * (const float & inF) const            { return Vector3(x*inF, y*inF, z*inF); }
  inline const Vector3  operator / (const float & inF) const            { return Vector3(x/inF, y/inF, z/inF); }
  inline Vector3&                       operator += (const Vector3 & inRHS)                     { x+=inRHS.x; y+=inRHS.y; z+=inRHS.z; return *this; }
  inline Vector3&                       operator -= (const Vector3 & inRHS)                     { x-=inRHS.x; y-=inRHS.y; z-=inRHS.z; return *this; }
  inline Vector3&                       operator *= (const float & inF)                         { x*=inF; y*=inF; z*=inF; return *this; }
  inline Vector3&                       operator /= (const float & inF)                         { x/=inF; y/=inF; z/=inF; return *this; }

  static const Vector3  sZero;
  static const Vector3  sOne;
};


//---------------------------------------------------------------------------------------------------------------------
//      Colors :
//---------------------------------------------------------------------------------------------------------------------

struct RGBAColor
{
RGBAColor() : mRGBA(0xFFFFFFFF) {}
RGBAColor(DWORD inColor) : mRGBA(inColor) { }
RGBAColor(const RGBAColor & inOther) : mRGBA(inOther.mRGBA) { }

RGBAColor(unsigned char inR, unsigned char inG, unsigned char inB, unsigned char inA = 255) : mR(inR), mG(inG), mB(inB), mA(inA) { }
RGBAColor(float inR, float inG, float inB, float inA) : mR(gClamp(int(inR*255.0f), 0, 255)), mG(gClamp(int(inG*255.0f), 0, 255)), mB(gClamp(int(inB*255.0f), 0, 255)), mA(gClamp(int(inA*255.0f), 0, 255)) { }

  inline void                           Set(DWORD inColor)                                                                                                                                                              { mRGBA = inColor; }
  void                                  SetComponents(unsigned char inRed, unsigned char inGreen, unsigned char inBlue, unsigned char inAlpha)  { mR=inRed; mG=inGreen; mB=inBlue; mA=inAlpha; }
  void                                  SetComponentsF(float inRed, float inGreen, float inBlue, float inAlpha) { mR=(unsigned char)(inRed*255.0f); mG=(unsigned char)(inGreen*255.0f); mB=(unsigned char)(inBlue*255.0f); mA=(unsigned char)(inAlpha*255.0f); }

  void                                  Set(unsigned char inRed, unsigned char inGreen, unsigned char inBlue, unsigned char inAlpha)    { mR=inRed; mG=inGreen; mB=inBlue; mA=inAlpha; }
  void                                  Blend(const RGBAColor & inOther, float inFactor);

  inline operator                       DWORD() const                                                                           { return mRGBA; } ///< Cast RGBAColor to a DWORD

  // script functions
  static void   _cdecl          ASConstruct(RGBAColor & inData);
  static RGBAColor & _cdecl     ASAssign(const RGBAColor & inSource, RGBAColor & inDest);
  static RGBAColor _cdecl       ASOperatorAdd(const RGBAColor & inLHS, const RGBAColor & inRHS);
  static RGBAColor _cdecl       ASOperatorSub(const RGBAColor & inLHS, const RGBAColor & inRHS);
  static RGBAColor _cdecl       ASOperatorMul(const RGBAColor & inLHS, const float inRHS);
  static RGBAColor _cdecl       ASOperatorDiv(const RGBAColor & inLHS, const float inRHS);
  static bool   _cdecl          ASOperatorEqual(const RGBAColor & inLHS, const RGBAColor &inRHS);
  static bool   _cdecl          ASOperatorNotEqual(const RGBAColor & inLHS, const RGBAColor &inRHS);

  inline bool                           operator == (const RGBAColor & inRHS) const     { return inRHS.mRGBA == mRGBA; }
  inline bool                           operator != (const RGBAColor & inRHS) const     { return inRHS.mRGBA != mRGBA; }
  inline const RGBAColor        operator + (const RGBAColor & inRHS) const      { return RGBAColor((unsigned char)gClamp(int(mR) + int(inRHS.mR), 0, 255), (unsigned char)gClamp(int(mG) + int(inRHS.mG), 0, 255), (unsigned char)gClamp(int(mB) + int(inRHS.mB), 0, 255), (unsigned char)gClamp(int(mA) + int(inRHS.mA), 0, 255)); }
  inline const RGBAColor        operator - (const RGBAColor & inRHS) const      { return RGBAColor((unsigned char)gClamp(int(mR) - int(inRHS.mR), 0, 255), (unsigned char)gClamp(int(mG) - int(inRHS.mG), 0, 255), (unsigned char)gClamp(int(mB) - int(inRHS.mB), 0, 255), (unsigned char)gClamp(int(mA) - int(inRHS.mA), 0, 255)); }
  inline const RGBAColor        operator * (const float & inF) const            { float fac = (1.0f / 255.0f) * inF; return RGBAColor(gClamp(float(mR) * fac, 0.0f, 1.0f), gClamp(float(mG) * fac, 0.0f, 1.0f), gClamp(float(mB) * fac, 0.0f, 1.0f), gClamp(float(mA) * fac, 0.0f, 1.0f)); }
  inline const RGBAColor        operator / (const float & inF) const            { float fac = (1.0f / 255.0f) / inF; return RGBAColor(gClamp(float(mR) * fac, 0.0f, 1.0f), gClamp(float(mG) * fac, 0.0f, 1.0f), gClamp(float(mB) * fac, 0.0f, 1.0f), gClamp(float(mA) * fac, 0.0f, 1.0f)); }
  inline RGBAColor&             operator += (const RGBAColor & inRHS)           { SetComponents((unsigned char)gClamp(int(mR) + int(inRHS.mR), 0, 255), (unsigned char)gClamp(int(mG) + int(inRHS.mG), 0, 255), (unsigned char)gClamp(int(mB) + int(inRHS.mB), 0, 255), (unsigned char)gClamp(int(mA) + int(inRHS.mA), 0, 255)); return *this; }
  inline RGBAColor&             operator -= (const RGBAColor & inRHS)           { SetComponents((unsigned char)gClamp(int(mR) - int(inRHS.mR), 0, 255), (unsigned char)gClamp(int(mG) - int(inRHS.mG), 0, 255), (unsigned char)gClamp(int(mB) - int(inRHS.mB), 0, 255), (unsigned char)gClamp(int(mA) - int(inRHS.mA), 0, 255)); return *this; }
  inline RGBAColor&             operator *= (const float & inF)                         { float fac = (1.0f / 255.0f) * inF; SetComponentsF(gClamp(float(mR) * fac, 0.0f, 1.0f), gClamp(float(mG) * fac, 0.0f, 1.0f), gClamp(float(mB) * fac, 0.0f, 1.0f), gClamp(float(mA) * fac, 0.0f, 1.0f)); return *this; }
  inline RGBAColor&             operator /= (const float & inF)                         { float fac = (1.0f / 255.0f) / inF; SetComponentsF(gClamp(float(mR) * fac, 0.0f, 1.0f), gClamp(float(mG) * fac, 0.0f, 1.0f), gClamp(float(mB) * fac, 0.0f, 1.0f), gClamp(float(mA) * fac, 0.0f, 1.0f)); return *this; }

  union
  {
    struct
    {
      unsigned char     mB;                                                                                                     ///< Blue component
      unsigned char     mG;                                                                                                     ///< Green component
      unsigned char     mR;                                                                                                     ///< Red component
      unsigned char     mA;                                                                                                     ///< Alpha component
    };
    DWORD                               mRGBA;                                                                                          ///< RGBA data
  };
};


//---------------------------------------------------------------------------------------------------------------------
// Vertex : custom render vertex
//---------------------------------------------------------------------------------------------------------------------


//
// Vertex
//
struct Vertex
{
  enum
    {
      VERTEX_FORMAT = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1),
    };

Vertex() : mPos(0.0f, 0.0f, 0.0f), mColor(0xFFFFFFFF), mUV(0.0f, 0.0f) { }
  Vertex(const Vertex & inOther)                                { mPos = inOther.mPos; mColor = inOther.mColor; mUV = inOther.mUV; }


  inline bool                           operator == (const Vertex & inRHS) const        { return mPos == inRHS.mPos && mColor == inRHS.mColor && mUV == inRHS.mUV; }
  inline bool                           operator != (const Vertex & inRHS) const        { return mPos != inRHS.mPos || mColor != inRHS.mColor || mUV != inRHS.mUV; }

  static void _cdecl            ASConstruct(Vertex & inThis);
  static Vertex& _cdecl ASAssign(const Vertex & inSource, Vertex & inDest);

  Vector3               mPos;
  RGBAColor     mColor;
  Vector2               mUV;

  static const Vertex sEmpty;
};



//---------------------------------------------------------------------------------------------------------------------
//      Engine registration
//---------------------------------------------------------------------------------------------------------------------

extern void                                     as_RegisterD3DUtils(asIScriptEngine * outEngine);       // Register all Vector-functionality to AngelScript

#endif

