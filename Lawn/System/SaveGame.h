#ifndef __SAVEGAMECONTEXT_H__
#define __SAVEGAMECONTEXT_H__

#include <string>
#include <cstdint>
#include "../../Sexy.TodLib/TodList.h"
#include "../../SexyAppFramework/Buffer.h"

class Board;
class Trail;
class TodEmitterDefinition;
#include "../../ConstEnums.h"
  // defines GameMode enum (replaces invalid forward decl)
class Reanimation;
class TodParticleSystem;
class TodParticleEmitter;
class ReanimatorDefinition;
class TodParticleDefinition;
class TrailDefinition;
namespace Sexy
{
    class Image;
}
using namespace Sexy;

struct SaveFileHeader
{
    unsigned int    mMagicNumber;
    unsigned int    mBuildVersion;
    unsigned int    mBuildDate;
};

class SaveGameContext
{
public:
    Buffer          mBuffer;            //+0x0
    bool            mFailed;            //+0x20
    bool            mReading;           //+0x21

public:
    inline int      ByteLeftToRead() { return (mBuffer.mDataBitSize - mBuffer.mReadBitPos + 7) / 8; }
    void            SyncBytes(void* theDest, int theReadSize);
    void            SyncInt(int& theInt);
    inline void     SyncUint(unsigned int& theInt) { SyncInt((signed int&)theInt); }
    void            SyncReanimationDef(ReanimatorDefinition*& theDefinition);
    void            SyncParticleDef(TodParticleDefinition*& theDefinition);
    void            SyncEmitterDef(TodEmitterDefinition*& theDefinition);
    void            SyncTrailDef(TrailDefinition*& theDefinition);
    void            SyncImage(Image*& theImage);

    // Helper methods for field-by-field synchronization
    inline void     SyncBool(bool& theBool) { int aVal = theBool; SyncInt(aVal); if (mReading) theBool = (aVal != 0); }
    inline void     SyncFloat(float& theFloat) { 
        if (mReading) { 
            uint32_t aVal = (uint32_t)mBuffer.ReadLong(); 
            theFloat = *(float*)&aVal; 
        } else { 
            uint32_t aVal = *(uint32_t*)&theFloat; 
            mBuffer.WriteLong((long)aVal); 
        } 
    }
    template <typename T> inline void SyncEnum(T& theEnum) { int aVal = (int)theEnum; SyncInt(aVal); if (mReading) theEnum = (T)aVal; }
    // Recursively sync every scalar element of a possibly MULTI-dimensional array.
    // (The old flat version did `SyncInt((int&)theArray[i])`; for a 2-D+ array the
    //  element type T deduces to a whole sub-array row, so `(int&)` aliased an entire
    //  row to one int and only the FIRST column was synced. On load the remaining
    //  columns kept uninitialized heap bytes -> e.g. garbage zombie types in
    //  mZombiesInWave[MAX_ZOMBIE_WAVES][MAX_ZOMBIES_IN_WAVE] crashing the spawner.)
    template <typename T> inline void SyncArrayElement(T& theVal) { SyncInt((int&)theVal); }
    template <typename T, int N> inline void SyncArrayElement(T(&theSubArray)[N]) { for (int i = 0; i < N; i++) SyncArrayElement(theSubArray[i]); }
    template <typename T, int N> void SyncArray(T(&theArray)[N]) { for (int i = 0; i < N; i++) SyncArrayElement(theArray[i]); }
    template <typename T, int N> void SyncFloatArray(T(&theArray)[N]) { for (int i = 0; i < N; i++) SyncFloat((float&)theArray[i]); }
    template <typename T, int N> void SyncBoolArray(T(&theArray)[N]) { for (int i = 0; i < N; i++) SyncBool((bool&)theArray[i]); }
};

void                SyncDataIDList(TodList<unsigned int>* theDataIDList, SaveGameContext& theContext, TodAllocator* theAllocator);
void                SyncParticleEmitter(TodParticleSystem* theParticleSystem, TodParticleEmitter* theParticleEmitter, SaveGameContext& theContext);
void                SyncParticleSystem(Board* theBoard, TodParticleSystem* theParticleSystem, SaveGameContext& theContext);
void                SyncReanimation(Board* theBoard, Reanimation* theReanimation, SaveGameContext& theContext);
void                SyncTrail(Board* theBoard, Trail* theTrail, SaveGameContext& theContext);
void                SyncBoard(SaveGameContext& theContext, Board* theBoard);
void				FixBoardAfterLoad(Board* theBoard);
bool				LawnLoadGame(Board* theBoard, const std::string& theFilePath);
bool				LawnSaveGame(Board* theBoard, const std::string& theFilePath);

#endif
