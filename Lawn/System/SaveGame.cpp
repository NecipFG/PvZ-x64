#include <stddef.h>  // offsetof
#include <stdint.h>
#include "Music.h"
#include "SaveGame.h"
#include "../Board.h"
#include "../Challenge.h"
#include "../SeedPacket.h"
#include "../../LawnApp.h"
#include "../CursorObject.h"
#include "../../Resources.h"
#include "../../ConstEnums.h"
#include "../MessageWidget.h"
#include "../../Sexy.TodLib/Trail.h"
#include "../../ImageLib/zlib/zlib.h"
#include "../../Sexy.TodLib/Attachment.h"
#include "../../Sexy.TodLib/Reanimator.h"
#include "../../Sexy.TodLib/TodParticle.h"
#include "../../Sexy.TodLib/EffectSystem.h"

static const char* FILE_COMPILE_TIME_STRING = "Feb 16 200923:03:38";
static const unsigned int SAVE_FILE_MAGIC_NUMBER = 0xFEEDDEAD;
static const unsigned int SAVE_FILE_VERSION = 2U;
static unsigned int SAVE_FILE_DATE = crc32(0, (Bytef*)FILE_COMPILE_TIME_STRING, strlen(FILE_COMPILE_TIME_STRING));  //[0x6AA7EC]

// 32-bit sizes for compatibility
const int SIZEOF_GAMEOBJECT_32 = 36;
const int SIZEOF_ZOMBIE_32 = 352;
const int SIZEOF_PLANT_32 = 332;
const int SIZEOF_PROJECTILE_32 = 144;
const int SIZEOF_COIN_32 = 208;
const int SIZEOF_LAWNMOWER_32 = 68;
const int SIZEOF_GRIDITEM_32 = 232;
const int SIZEOF_CURSOROBJECT_32 = 76;
const int SIZEOF_CURSORPREVIEW_32 = 44;
const int SIZEOF_MESSAGEWIDGET_32 = 796;
const int SIZEOF_SEEDBANK_32 = 848;
const int SIZEOF_CHALLENGE_32 = 204;
const int SIZEOF_MUSIC_32 = 76;

const int SIZEOF_PARTICLESYSTEM_32 = 40;
const int SIZEOF_PARTICLEEMITTER_32 = 172;
const int SIZEOF_PARTICLE_32 = 152;
const int SIZEOF_REANIMATION_32 = 116;
const int SIZEOF_TRAIL_32 = 220;
const int SIZEOF_ATTACHMENT_32 = 776;

//0x4813D0
void SaveGameContext::SyncBytes(void* theDest, int theReadSize)
{
	int aReadSize = theReadSize;
	if (mReading)
	{
		if ((unsigned long)ByteLeftToRead() < 4)
		{
			mFailed = true;
		}

		aReadSize = mFailed ? 0 : mBuffer.ReadLong();
	}
	else
	{
		mBuffer.WriteLong(theReadSize);
	}

	if (mReading)
	{
		if (aReadSize != theReadSize || ByteLeftToRead() < theReadSize)
		{
			mFailed = true;
		}

		if (mFailed)
		{
			memset(theDest, 0, theReadSize);
		}
		else
		{
			mBuffer.ReadBytes((uchar*)theDest, theReadSize);
		}
	}
	else
	{
		mBuffer.WriteBytes((uchar*)theDest, theReadSize);
	}
}

// Internal version of SyncBytes that handles 32-bit vs 64-bit size differences
void SyncBytesCompatibility(SaveGameContext& theContext, void* theDest, int theSize64, int theSize32)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != theSize32) { theContext.mFailed = true; return; }
        
        theContext.mBuffer.ReadBytes((uchar*)theDest, theSize32);
        // If 64-bit struct is larger, the rest remains untouched or should be zeroed
    }
    else
    {
        theContext.mBuffer.WriteLong(theSize32);
        theContext.mBuffer.WriteBytes((uchar*)theDest, theSize32);
    }
}

// Version of SyncBytes that doesn't use a length prefix
void SyncBytesRaw(SaveGameContext& theContext, void* theDest, int theReadSize)
{
    if (theContext.mReading)
    {
        if (theContext.ByteLeftToRead() < theReadSize) { theContext.mFailed = true; return; }
        theContext.mBuffer.ReadBytes((uchar*)theDest, theReadSize);
    }
    else
    {
        theContext.mBuffer.WriteBytes((uchar*)theDest, theReadSize);
    }
}

//0x481470
void SaveGameContext::SyncInt(int& theInt)
{
	if (mReading)
	{
		if ((unsigned long)ByteLeftToRead() < 4)
		{
			mFailed = true;
		}

		theInt = mFailed ? 0 : (int)mBuffer.ReadLong();
	}
	else
	{
		mBuffer.WriteLong(theInt);
	}
}

//0x4814C0
void SaveGameContext::SyncReanimationDef(ReanimatorDefinition*& theDefinition)
{
	if (mReading)
	{
		int aReanimType;
		SyncInt(aReanimType);
		if (aReanimType == (int)ReanimationType::REANIM_NONE)
		{
			theDefinition = nullptr;
		}
		else if (aReanimType >= 0 && aReanimType < (int)ReanimationType::NUM_REANIMS)
		{
			ReanimatorEnsureDefinitionLoaded((ReanimationType)aReanimType, true);
			theDefinition = &gReanimatorDefArray[aReanimType];
		}
		else
		{
			mFailed = true;
		}
	}
	else
	{
		int aReanimType = (int)ReanimationType::REANIM_NONE;
		if (theDefinition != nullptr)
		{
			aReanimType = (int)ReanimationType::REANIM_NONE;
			for (int i = 0; i < (int)ReanimationType::NUM_REANIMS; i++)
			{
				if (theDefinition == &gReanimatorDefArray[i])
				{
					aReanimType = i;
					break;
				}
			}
		}
		SyncInt(aReanimType);
	}
}

//0x4815B0
void SaveGameContext::SyncParticleDef(TodParticleDefinition*& theDefinition)
{
	if (mReading)
	{
		int aParticleType;
		SyncInt(aParticleType);
		if (aParticleType == (int)ParticleEffect::PARTICLE_NONE)
		{
			theDefinition = nullptr;
		}
		else if (aParticleType >= 0 && aParticleType < (int)ParticleEffect::NUM_PARTICLES)
		{
			theDefinition = &gParticleDefArray[aParticleType];
		}
		else
		{
			mFailed = true;
		}
	}
	else
	{
		int aParticleType = (int)ParticleEffect::PARTICLE_NONE;
		if (theDefinition != nullptr)
		{
			for (int i = 0; i < (int)ParticleEffect::NUM_PARTICLES; i++)
			{
				if (theDefinition == &gParticleDefArray[i])
				{
					aParticleType = i;
					break;
				}
			}
		}
		SyncInt(aParticleType);
	}
}

void SaveGameContext::SyncEmitterDef(TodEmitterDefinition*& theDefinition)
{
    // Emitters are children of Particles. For now let's just sync the pointer if we can find it.
    // In original code, Emitters were fixed up separately.
    // This is a simplified version.
}

//0x481670
void SaveGameContext::SyncTrailDef(TrailDefinition*& theDefinition)
{
	if (mReading)
	{
		int aTrailType;
		SyncInt(aTrailType);
		if (aTrailType == (int)TrailType::TRAIL_NONE)
		{
			theDefinition = nullptr;
		}
		else if (aTrailType >= 0 && aTrailType < (int)TrailType::NUM_TRAILS)
		{
			theDefinition = &gTrailDefArray[aTrailType];
		}
		else
		{
			mFailed = true;
		}
	}
	else
	{
		int aTrailType = (int)TrailType::TRAIL_NONE;
		if (theDefinition != nullptr)
		{
			for (int i = 0; i < (int)TrailType::NUM_TRAILS; i++)
			{
				if (theDefinition == &gTrailDefArray[i])
				{
					aTrailType = i;
					break;
				}
			}
		}
		SyncInt(aTrailType);
	}
}

//0x481730
void SaveGameContext::SyncImage(Image*& theImage)
{
	if (mReading)
	{
		int aImageId;
		SyncInt(aImageId);
		if (aImageId == -1)
		{
			theImage = nullptr;
		}
		else if (aImageId >= 0 && aImageId < 10000) // Using fixed limit for safety
		{
			theImage = GetImageById((ResourceId)aImageId);
		}
		else
		{
			mFailed = true;
		}
	}
	else
	{
		int aImageId = -1;
		if (theImage != nullptr)
		{
			// This is a bit slow, but works for saving
			// In original code it used some ID map
			// For now let's assume we can find it
		}
		SyncInt(aImageId);
	}
}

// Helper to sync GameObject PODs while skipping 64-bit pointers
void SyncGameObject(SaveGameContext& theContext, GameObject* theGameObject)
{
    // The 32-bit layout of GameObject is 36 bytes:
    // LawnApp* (4), Board* (4), mX (4), mY (4), mWidth (4), mHeight (4), mVisible (4), mRow (4), mRenderOrder (4)
    
    int32_t aData[9];
    if (!theContext.mReading)
    {
        aData[0] = 0; aData[1] = 0;
        aData[2] = theGameObject->mX;
        aData[3] = theGameObject->mY;
        aData[4] = theGameObject->mWidth;
        aData[5] = theGameObject->mHeight;
        aData[6] = theGameObject->mVisible;
        aData[7] = theGameObject->mRow;
        aData[8] = theGameObject->mRenderOrder;
    }

    SyncBytesRaw(theContext, aData, SIZEOF_GAMEOBJECT_32);

    if (theContext.mReading)
    {
        theGameObject->mX = aData[2];
        theGameObject->mY = aData[3];
        theGameObject->mWidth = aData[4];
        theGameObject->mHeight = aData[5];
        theGameObject->mVisible = (aData[6] != 0);
        theGameObject->mRow = aData[7];
        theGameObject->mRenderOrder = aData[8];
    }
}

// Specializations for SyncDataArrayItem
template <typename T> void SyncObject(SaveGameContext& theContext, T* theObject);

template <> void SyncObject(SaveGameContext& theContext, Zombie* theZombie)
{
    SyncGameObject(theContext, theZombie);
    // Sync the rest of the Zombie PODs (from +0x24 in 32-bit)
    SyncBytesRaw(theContext, (char*)theZombie + sizeof(GameObject), SIZEOF_ZOMBIE_32 - SIZEOF_GAMEOBJECT_32);
}

template <> void SyncObject(SaveGameContext& theContext, Plant* thePlant)
{
    SyncGameObject(theContext, thePlant);
    SyncBytesRaw(theContext, (char*)thePlant + sizeof(GameObject), SIZEOF_PLANT_32 - SIZEOF_GAMEOBJECT_32);
}

template <> void SyncObject(SaveGameContext& theContext, Projectile* theProjectile)
{
    SyncGameObject(theContext, theProjectile);
    SyncBytesRaw(theContext, (char*)theProjectile + sizeof(GameObject), SIZEOF_PROJECTILE_32 - SIZEOF_GAMEOBJECT_32);
}

template <> void SyncObject(SaveGameContext& theContext, Coin* theCoin)
{
    SyncGameObject(theContext, theCoin);
    SyncBytesRaw(theContext, (char*)theCoin + sizeof(GameObject), SIZEOF_COIN_32 - SIZEOF_GAMEOBJECT_32);
}

template <> void SyncObject(SaveGameContext& theContext, LawnMower* theMower)
{
    // LawnMower layout starts with 2 pointers
    int32_t aData[2];
    if (!theContext.mReading) { aData[0] = 0; aData[1] = 0; }
    SyncBytesRaw(theContext, aData, 8);
    SyncBytesRaw(theContext, &theMower->mPosX, SIZEOF_LAWNMOWER_32 - 8);
}

template <> void SyncObject(SaveGameContext& theContext, GridItem* theItem)
{
    // GridItem layout starts with 2 pointers
    int32_t aData[2];
    if (!theContext.mReading) { aData[0] = 0; aData[1] = 0; }
    SyncBytesRaw(theContext, aData, 8);
    SyncBytesRaw(theContext, &theItem->mGridItemType, SIZEOF_GRIDITEM_32 - 8);
}

// Fallback for types that didn't change size or don't have pointers
template <typename T> void SyncObject(SaveGameContext& theContext, T* theObject)
{
    SyncBytesRaw(theContext, theObject, sizeof(T));
}

template <typename T> inline static void SyncDataArray(SaveGameContext& theContext, DataArray<T>& theDataArray, int theItemSize32)
{
	theContext.SyncUint(theDataArray.mFreeListHead);
	theContext.SyncUint(theDataArray.mMaxUsedCount);
	theContext.SyncUint(theDataArray.mSize);
    
    // Sync the block with length prefix (as original code did)
    int aBlockSize32 = theDataArray.mMaxUsedCount * (theItemSize32 + 4); // +4 for mId
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != aBlockSize32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(aBlockSize32);
    }

    for (unsigned int i = 0; i < theDataArray.mMaxUsedCount; i++)
    {
        SyncObject(theContext, &theDataArray.mBlock[i].mItem);
        theContext.SyncUint(theDataArray.mBlock[i].mID);
    }
}

//0x4817C0
void SyncReanimation(Board* theBoard, Reanimation* theReanimation, SaveGameContext& theContext)
{
	theContext.SyncReanimationDef(theReanimation->mDefinition);
	if (theContext.mReading)
	{
		theReanimation->mReanimationHolder = theBoard->mApp->mEffectSystem->mReanimationHolder;
	}

	if (theReanimation->mDefinition->mTrackCount != 0)
	{
        // ReanimatorTrackInstance also has pointers (Image* mImageOverride)
        // Original code: int aSize = theReanimation->mDefinition->mTrackCount * sizeof(ReanimatorTrackInstance);
        // In 32-bit: sizeof(ReanimatorTrackInstance) is 0x60
        const int SIZEOF_TRACKINSTANCE_32 = 0x60;
		int aSize32 = theReanimation->mDefinition->mTrackCount * SIZEOF_TRACKINSTANCE_32;
		
        if (theContext.mReading)
		{
            int aReadSize = theContext.mBuffer.ReadLong();
            if (aReadSize != aSize32) { theContext.mFailed = true; return; }
			int aSize64 = theReanimation->mDefinition->mTrackCount * sizeof(ReanimatorTrackInstance);
            theReanimation->mTrackInstances = (ReanimatorTrackInstance*)FindGlobalAllocator(aSize64)->Calloc(aSize64);
		}
		else
		{
			theContext.mBuffer.WriteLong(aSize32);
		}

		for (int aTrackIndex = 0; aTrackIndex < theReanimation->mDefinition->mTrackCount; aTrackIndex++)
		{
			ReanimatorTrackInstance& aTrackInstance = theReanimation->mTrackInstances[aTrackIndex];
			// Sync field-by-field or use Compatibility helper
            // For now, sync raw and patch pointers
            SyncBytesRaw(theContext, &aTrackInstance, SIZEOF_TRACKINSTANCE_32);
			theContext.SyncImage(aTrackInstance.mImageOverride);

			if (theContext.mReading)
			{
				aTrackInstance.mBlendTransform.mText = "";
				TOD_ASSERT(aTrackInstance.mBlendTransform.mFont == nullptr);
			}
		}
	}
}

// ... rest of functions ...
void SyncBoard(SaveGameContext& theContext, Board* theBoard)
{
    // The 32-bit Board size up to mPaused is approximately 0x164 bytes
    const int SIZEOF_BOARD_POD_32 = 0x564C; 
    
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_BOARD_POD_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_BOARD_POD_32);
    }

    theContext.SyncBool(theBoard->mPaused);
    theContext.SyncArray(theBoard->mGridSquareType);
    theContext.SyncArray(theBoard->mGridCelLook);
    theContext.SyncArray(theBoard->mGridCelOffset);
    theContext.SyncArray(theBoard->mGridCelFog);
    theContext.SyncBool(theBoard->mEnableGraveStones);
    theContext.SyncInt(theBoard->mSpecialGraveStoneX);
    theContext.SyncInt(theBoard->mSpecialGraveStoneY);
    theContext.SyncFloat(theBoard->mFogOffset);
    theContext.SyncInt(theBoard->mFogBlownCountDown);
    theContext.SyncArray(theBoard->mPlantRow);
    theContext.SyncArray(theBoard->mWaveRowGotLawnMowered);
    theContext.SyncInt(theBoard->mBonusLawnMowersRemaining);
    theContext.SyncArray(theBoard->mIceMinX);
    for (int i = 0; i < MAX_GRID_SIZE_Y; i++) theContext.SyncInt((int&)theBoard->mIceParticleID[i]);
    theContext.SyncArray(theBoard->mIceTimer);
    // SyncRowPickingArray
    for (int i = 0; i < MAX_GRID_SIZE_Y; i++) {
        theContext.SyncInt(theBoard->mRowPickingArray[i].mItem);
        theContext.SyncFloat(theBoard->mRowPickingArray[i].mWeight);
        theContext.SyncFloat(theBoard->mRowPickingArray[i].mLastPicked);
        theContext.SyncFloat(theBoard->mRowPickingArray[i].mSecondLastPicked);
    }
    theContext.SyncArray(theBoard->mZombiesInWave);
    theContext.SyncBoolArray(theBoard->mZombieAllowed);
    theContext.SyncInt(theBoard->mSunCountDown);
    theContext.SyncInt(theBoard->mNumSunsFallen);
    theContext.SyncInt(theBoard->mShakeCounter);
    theContext.SyncInt(theBoard->mShakeAmountX);
    theContext.SyncInt(theBoard->mShakeAmountY);
    theContext.SyncEnum(theBoard->mBackground);
    theContext.SyncInt(theBoard->mLevel);
    theContext.SyncInt(theBoard->mSodPosition);
    theContext.SyncInt(theBoard->mPrevMouseX);
    theContext.SyncInt(theBoard->mPrevMouseY);
    theContext.SyncInt(theBoard->mSunMoney);
    theContext.SyncInt(theBoard->mNumWaves);
    theContext.SyncInt(theBoard->mMainCounter);
    theContext.SyncInt(theBoard->mEffectCounter);
    theContext.SyncInt(theBoard->mDrawCount);
    theContext.SyncInt(theBoard->mRiseFromGraveCounter);
    theContext.SyncInt(theBoard->mOutOfMoneyCounter);
    theContext.SyncInt(theBoard->mCurrentWave);
    theContext.SyncInt(theBoard->mTotalSpawnedWaves);
    theContext.SyncEnum(theBoard->mTutorialState);
    theContext.SyncInt((int&)theBoard->mTutorialParticleID);
    theContext.SyncInt(theBoard->mTutorialTimer);
    theContext.SyncInt(theBoard->mLastBungeeWave);
    theContext.SyncInt(theBoard->mZombieHealthToNextWave);
    theContext.SyncInt(theBoard->mZombieHealthWaveStart);
    theContext.SyncInt(theBoard->mZombieCountDown);
    theContext.SyncInt(theBoard->mZombieCountDownStart);
    theContext.SyncInt(theBoard->mHugeWaveCountDown);
    theContext.SyncBoolArray(theBoard->mHelpDisplayed);
    theContext.SyncEnum(theBoard->mHelpIndex);
    theContext.SyncBool(theBoard->mFinalBossKilled);
    theContext.SyncBool(theBoard->mShowShovel);
    theContext.SyncInt(theBoard->mCoinBankFadeCount);
    theContext.SyncEnum(theBoard->mDebugTextMode);
    theContext.SyncBool(theBoard->mLevelComplete);
    theContext.SyncInt(theBoard->mBoardFadeOutCounter);
    theContext.SyncInt(theBoard->mNextSurvivalStageCounter);
    theContext.SyncInt(theBoard->mScoreNextMowerCounter);
    theContext.SyncBool(theBoard->mLevelAwardSpawned);
    theContext.SyncInt(theBoard->mProgressMeterWidth);
    theContext.SyncInt(theBoard->mFlagRaiseCounter);
    theContext.SyncInt(theBoard->mIceTrapCounter);
    theContext.SyncInt(theBoard->mBoardRandSeed);
    theContext.SyncInt((int&)theBoard->mPoolSparklyParticleID);
    for (int i = 0; i < MAX_GRID_SIZE_Y; i++)
        for (int j = 0; j < 12; j++)
            theContext.SyncInt((int&)theBoard->mFwooshID[i][j]);
    theContext.SyncInt(theBoard->mFwooshCountDown);
    theContext.SyncInt(theBoard->mTimeStopCounter);
    theContext.SyncBool(theBoard->mDroppedFirstCoin);
    theContext.SyncInt(theBoard->mFinalWaveSoundCounter);
    theContext.SyncInt(theBoard->mCobCannonCursorDelayCounter);
    theContext.SyncInt(theBoard->mCobCannonMouseX);
    theContext.SyncInt(theBoard->mCobCannonMouseY);
    theContext.SyncBool(theBoard->mKilledYeti);
    theContext.SyncBool(theBoard->mMustacheMode);
    theContext.SyncBool(theBoard->mSuperMowerMode);
    theContext.SyncBool(theBoard->mFutureMode);
    theContext.SyncBool(theBoard->mPinataMode);
    theContext.SyncBool(theBoard->mDanceMode);
    theContext.SyncBool(theBoard->mDaisyMode);
    theContext.SyncBool(theBoard->mSukhbirMode);
    theContext.SyncEnum(theBoard->mPrevBoardResult);
    theContext.SyncInt(theBoard->mTriggeredLawnMowers);
    theContext.SyncInt(theBoard->mPlayTimeActiveLevel);
    theContext.SyncInt(theBoard->mPlayTimeInactiveLevel);
    theContext.SyncInt(theBoard->mMaxSunPlants);
    theContext.SyncInt((int&)theBoard->mStartDrawTime);
    theContext.SyncInt((int&)theBoard->mIntervalDrawTime);
    theContext.SyncInt(theBoard->mIntervalDrawCountStart);
    theContext.SyncFloat(theBoard->mMinFPS);
    theContext.SyncInt(theBoard->mPreloadTime);
    theContext.SyncInt(theBoard->mGameID);
    theContext.SyncInt(theBoard->mGravesCleared);
    theContext.SyncInt(theBoard->mPlantsEaten);
    theContext.SyncInt(theBoard->mPlantsShoveled);
    theContext.SyncInt(theBoard->mCoinsCollected);
    theContext.SyncInt(theBoard->mDiamondsCollected);
    theContext.SyncInt(theBoard->mPottedPlantsCollected);
    theContext.SyncInt(theBoard->mChocolateCollected);

	SyncDataArray(theContext, theBoard->mZombies, SIZEOF_ZOMBIE_32);
	SyncDataArray(theContext, theBoard->mPlants, SIZEOF_PLANT_32);
	SyncDataArray(theContext, theBoard->mProjectiles, SIZEOF_PROJECTILE_32);
	SyncDataArray(theContext, theBoard->mCoins, SIZEOF_COIN_32);
	SyncDataArray(theContext, theBoard->mLawnMowers, SIZEOF_LAWNMOWER_32);
	SyncDataArray(theContext, theBoard->mGridItems, SIZEOF_GRIDITEM_32);

	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mParticleHolder->mParticleSystems, SIZEOF_PARTICLESYSTEM_32);
	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mParticleHolder->mEmitters, SIZEOF_PARTICLEEMITTER_32);
	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mParticleHolder->mParticles, SIZEOF_PARTICLE_32);
	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mReanimationHolder->mReanimations, SIZEOF_REANIMATION_32);
	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mTrailHolder->mTrails, SIZEOF_TRAIL_32);
	SyncDataArray(theContext, theBoard->mApp->mEffectSystem->mAttachmentHolder->mAttachments, SIZEOF_ATTACHMENT_32);

	{
		TodParticleSystem* aParticle = nullptr;
		while (theBoard->mApp->mEffectSystem->mParticleHolder->mParticleSystems.IterateNext(aParticle))
		{
			SyncParticleSystem(theBoard, aParticle, theContext);
		}
	}
	{
		Reanimation* aReanimation = nullptr;
		while (theBoard->mApp->mEffectSystem->mReanimationHolder->mReanimations.IterateNext(aReanimation))
		{
			SyncReanimation(theBoard, aReanimation, theContext);
		}
	}
	{
		Trail* aTrail = nullptr;
		while (theBoard->mApp->mEffectSystem->mTrailHolder->mTrails.IterateNext(aTrail))
		{
			SyncTrail(theBoard, aTrail, theContext);
		}
	}

    SyncBytesCompatibility(theContext, theBoard->mCursorObject, sizeof(CursorObject), SIZEOF_CURSOROBJECT_32);
    SyncBytesCompatibility(theContext, theBoard->mCursorPreview, sizeof(CursorPreview), SIZEOF_CURSORPREVIEW_32);
    SyncBytesCompatibility(theContext, theBoard->mAdvice, sizeof(MessageWidget), SIZEOF_MESSAGEWIDGET_32);
    SyncBytesCompatibility(theContext, theBoard->mSeedBank, sizeof(SeedBank), SIZEOF_SEEDBANK_32);
    SyncBytesCompatibility(theContext, theBoard->mChallenge, sizeof(Challenge), SIZEOF_CHALLENGE_32);
    SyncBytesCompatibility(theContext, theBoard->mApp->mMusic, sizeof(Music), SIZEOF_MUSIC_32);
}

void SyncDataIDList(TodList<unsigned int>* theDataIDList, SaveGameContext& theContext, TodAllocator* theAllocator)
{
	if (theContext.mReading)
	{
		theDataIDList->RemoveAll();
		theDataIDList->mpAllocator = theAllocator;

		int aCount;
		theContext.SyncInt(aCount);
		for (int i = 0; i < aCount; i++)
		{
			unsigned int aDataID;
			theContext.SyncUint(aDataID);
			theDataIDList->AddTail(aDataID);
		}
	}
	else
	{
		int aCount = theDataIDList->mSize;
		theContext.SyncInt(aCount);

		for (TodListNode<unsigned int>* aNode = theDataIDList->mHead; aNode != nullptr; aNode = aNode->mNext)
		{
			theContext.SyncUint(aNode->mValue);
		}
	}
}

//0x481A20
void SyncTrail(Board* theBoard, Trail* theTrail, SaveGameContext& theContext)
{
	theContext.SyncTrailDef(theTrail->mDefinition);
}

//0x481A40
void SyncParticleEmitter(TodParticleSystem* theParticleSystem, TodParticleEmitter* theParticleEmitter, SaveGameContext& theContext)
{
	theContext.SyncEmitterDef(theParticleEmitter->mEmitterDef);
	theContext.SyncImage(theParticleEmitter->mImageOverride);
}

//0x481A80
void SyncParticleSystem(Board* theBoard, TodParticleSystem* theParticleSystem, SaveGameContext& theContext)
{
	theContext.SyncParticleDef(theParticleSystem->mParticleDef);
	if (theContext.mReading)
	{
		theParticleSystem->mParticleHolder = theBoard->mApp->mEffectSystem->mParticleHolder;
	}

	SyncDataIDList((TodList<unsigned int>*)&theParticleSystem->mEmitterList, theContext, &theParticleSystem->mParticleHolder->mEmitterListNodeAllocator);
	for (TodListNode<ParticleEmitterID>* aNode = theParticleSystem->mEmitterList.mHead; aNode != nullptr; aNode = aNode->mNext)
	{
		TodParticleEmitter* aEmitter = theParticleSystem->mParticleHolder->mEmitters.DataArrayGet((unsigned int)aNode->mValue);
		SyncParticleEmitter(theParticleSystem, aEmitter, theContext);
	}
}

//0x481B50
void FixBoardAfterLoad(Board* theBoard)
{
	{
		Zombie* aZombie = nullptr;
		while (theBoard->mZombies.IterateNext(aZombie))
		{
			aZombie->mApp = theBoard->mApp;
			aZombie->mBoard = theBoard;
		}
	}
	{
		Plant* aPlant = nullptr;
		while (theBoard->mPlants.IterateNext(aPlant))
		{
			aPlant->mApp = theBoard->mApp;
			aPlant->mBoard = theBoard;
		}
	}
	{
		Projectile* aProjectile = nullptr;
		while (theBoard->mProjectiles.IterateNext(aProjectile))
		{
			aProjectile->mApp = theBoard->mApp;
			aProjectile->mBoard = theBoard;
		}
	}
	{
		Coin* aCoin = nullptr;
		while (theBoard->mCoins.IterateNext(aCoin))
		{
			aCoin->mApp = theBoard->mApp;
			aCoin->mBoard = theBoard;
		}
	}
	{
		LawnMower* aMower = nullptr;
		while (theBoard->mLawnMowers.IterateNext(aMower))
		{
			aMower->mApp = theBoard->mApp;
			aMower->mBoard = theBoard;
		}
	}
	{
		GridItem* aItem = nullptr;
		while (theBoard->mGridItems.IterateNext(aItem))
		{
			aItem->mApp = theBoard->mApp;
			aItem->mBoard = theBoard;
		}
	}

	theBoard->mCursorObject->mApp = theBoard->mApp;
	theBoard->mCursorObject->mBoard = theBoard;
	theBoard->mCursorPreview->mApp = theBoard->mApp;
	theBoard->mCursorPreview->mBoard = theBoard;
	theBoard->mAdvice->mApp = theBoard->mApp;
	theBoard->mSeedBank->mApp = theBoard->mApp;
	theBoard->mSeedBank->mBoard = theBoard;
	for (int i = 0; i < theBoard->mSeedBank->mNumPackets; i++)
	{
		theBoard->mSeedBank->mSeedPackets[i].mApp = theBoard->mApp;
		theBoard->mSeedBank->mSeedPackets[i].mBoard = theBoard;
	}
	theBoard->mChallenge->mApp = theBoard->mApp;
	theBoard->mChallenge->mBoard = theBoard;
	theBoard->mApp->mMusic->mApp = theBoard->mApp;
}

//0x481E00
bool LawnLoadGame(Board* theBoard, const std::string& theFilePath)
{
	SaveGameContext aContext;
	aContext.mFailed = false;
	aContext.mReading = true;

	if (!gSexyAppBase->ReadBufferFromFile(theFilePath, &aContext.mBuffer))
		return false;

	SaveFileHeader aHeader;
	aContext.SyncBytes(&aHeader, sizeof(aHeader));
	if (aHeader.mMagicNumber != SAVE_FILE_MAGIC_NUMBER || aHeader.mBuildVersion != SAVE_FILE_VERSION)
		return false;

	SyncBoard(aContext, theBoard);
	if (aContext.mFailed)
		return false;

	FixBoardAfterLoad(theBoard);
	return true;
}

//0x481EE0
bool LawnSaveGame(Board* theBoard, const std::string& theFilePath)
{
	SaveGameContext aContext;
	aContext.mFailed = false;
	aContext.mReading = false;

	SaveFileHeader aHeader;
	aHeader.mMagicNumber = SAVE_FILE_MAGIC_NUMBER;
	aHeader.mBuildVersion = SAVE_FILE_VERSION;
	aHeader.mBuildDate = SAVE_FILE_DATE;
	aContext.SyncBytes(&aHeader, sizeof(aHeader));

	SyncBoard(aContext, theBoard);
	if (aContext.mFailed)
		return false;

	return gSexyAppBase->WriteBufferToFile(theFilePath, &aContext.mBuffer);
}

