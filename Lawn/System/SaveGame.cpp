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

#pragma pack(push, 4)
struct PottedPlant_32
{
    int32_t             mSeedType;
    int32_t             mWhichZenGarden;
    int32_t             mX;
    int32_t             mY;
    int32_t             mFacing;
    uint8_t             mPad1[4];
    int64_t             mLastWateredTime;
    int32_t             mDrawVariation;
    int32_t             mPlantAge;
    int32_t             mTimesFed;
    int32_t             mFeedingsPerGrow;
    int32_t             mPlantNeed;
    uint8_t             mPad2[4];
    int64_t             mLastNeedFulfilledTime;
    int64_t             mLastFertilizedTime;
    int64_t             mLastChocolateTime;
    int64_t             mFutureAttribute[1];
};

struct Zombie_32
{
    int32_t             mZombieType;
    int32_t             mZombiePhase;
    float               mPosX;
    float               mPosY;
    float               mVelX;
    int32_t             mAnimCounter;
    int32_t             mGroanCounter;
    int32_t             mAnimTicksPerFrame;
    int32_t             mAnimFrames;
    int32_t             mFrame;
    int32_t             mPrevFrame;
    bool                mVariant;
    bool                mIsEating;
    uint8_t             mPad1[2];
    int32_t             mJustGotShotCounter;
    int32_t             mShieldJustGotShotCounter;
    int32_t             mShieldRecoilCounter;
    int32_t             mZombieAge;
    int32_t             mZombieHeight;
    int32_t             mPhaseCounter;
    int32_t             mFromWave;
    bool                mDroppedLoot;
    uint8_t             mPad2[3];
    int32_t             mZombieFade;
    bool                mFlatTires;
    uint8_t             mPad3[3];
    int32_t             mUseLadderCol;
    int32_t             mTargetCol;
    float               mAltitude;
    bool                mHitUmbrella;
    uint8_t             mPad4[3];
    int32_t             mZombieRect[4];
    int32_t             mZombieAttackRect[4];
    int32_t             mChilledCounter;
    int32_t             mButteredCounter;
    int32_t             mIceTrapCounter;
    bool                mMindControlled;
    bool                mBlowingAway;
    bool                mHasHead;
    bool                mHasArm;
    bool                mHasObject;
    bool                mInPool;
    bool                mOnHighGround;
    bool                mYuckyFace;
    int32_t             mYuckyFaceCounter;
    int32_t             mHelmType;
    int32_t             mBodyHealth;
    int32_t             mBodyMaxHealth;
    int32_t             mHelmHealth;
    int32_t             mHelmMaxHealth;
    int32_t             mShieldType;
    int32_t             mShieldHealth;
    int32_t             mShieldMaxHealth;
    int32_t             mFlyingHealth;
    int32_t             mFlyingMaxHealth;
    bool                mDead;
    uint8_t             mPad5[3];
    int32_t             mRelatedZombieID;
    int32_t             mFollowerZombieID[4];
    bool                mPlayingSong;
    uint8_t             mPad6[3];
    int32_t             mParticleOffsetX;
    int32_t             mParticleOffsetY;
    int32_t             mAttachmentID;
    int32_t             mSummonCounter;
    int32_t             mBodyReanimID;
    float               mScaleZombie;
    float               mVelZ;
    float               mOriginalAnimRate;
    int32_t             mTargetPlantID;
    int32_t             mBossMode;
    int32_t             mTargetRow;
    int32_t             mBossBungeeCounter;
    int32_t             mBossStompCounter;
    int32_t             mBossHeadCounter;
    int32_t             mBossFireBallReanimID;
    int32_t             mSpecialHeadReanimID;
    int32_t             mFireballRow;
    bool                mIsFireBall;
    uint8_t             mPad7[3];
    int32_t             mMoweredReanimID;
    int32_t             mLastPortalX;
    uint8_t             mPadEnd[8];
};

struct Plant_32
{
    int32_t             mSeedType;
    int32_t             mPlantCol;
    int32_t             mAnimCounter;
    int32_t             mFrame;
    int32_t             mFrameLength;
    int32_t             mNumFrames;
    int32_t             mState;
    int32_t             mPlantHealth;
    int32_t             mPlantMaxHealth;
    int32_t             mSubclass;
    int32_t             mDisappearCountdown;
    int32_t             mDoSpecialCountdown;
    int32_t             mStateCountdown;
    int32_t             mLaunchCounter;
    int32_t             mLaunchRate;
    int32_t             mPlantRect[4];
    int32_t             mPlantAttackRect[4];
    int32_t             mTargetX;
    int32_t             mTargetY;
    int32_t             mStartRow;
    int32_t             mParticleID;
    int32_t             mShootingCounter;
    int32_t             mBodyReanimID;
    int32_t             mHeadReanimID;
    int32_t             mHeadReanimID2;
    int32_t             mHeadReanimID3;
    int32_t             mBlinkReanimID;
    int32_t             mLightReanimID;
    int32_t             mSleepingReanimID;
    int32_t             mBlinkCountdown;
    int32_t             mRecentlyEatenCountdown;
    int32_t             mEatenFlashCountdown;
    int32_t             mBeghouledFlashCountdown;
    float               mShakeOffsetX;
    float               mShakeOffsetY;
    struct {
        float           mPosX;
        float           mPosY;
        float           mDestOffsetX;
        float           mDestOffsetY;
        int32_t         mItemType;
    }                   mMagnetItems[5];
    int32_t             mTargetZombieID;
    int32_t             mWakeUpCounter;
    int32_t             mOnBungeeState;
    int32_t             mImitaterType;
    int32_t             mPottedPlantIndex;
    bool                mAnimPing;
    bool                mDead;
    bool                mSquished;
    bool                mIsAsleep;
    bool                mIsOnBoard;
    bool                mHighlighted;
    uint8_t             mPadEnd[6];
};

struct Projectile_32
{
    int32_t             mFrame;
    int32_t             mNumFrames;
    int32_t             mAnimCounter;
    float               mPosX;
    float               mPosY;
    float               mPosZ;
    float               mVelX;
    float               mVelY;
    float               mVelZ;
    float               mAccZ;
    float               mShadowY;
    bool                mDead;
    uint8_t             mPad1[3];
    int32_t             mAnimTicksPerFrame;
    int32_t             mMotionType;
    int32_t             mProjectileType;
    int32_t             mProjectileAge;
    int32_t             mClickBackoffCounter;
    float               mRotation;
    float               mRotationSpeed;
    bool                mOnHighGround;
    uint8_t             mPad2[3];
    int32_t             mDamageRangeFlags;
    int32_t             mHitTorchwoodGridX;
    int32_t             mAttachmentID;
    float               mCobTargetX;
    int32_t             mCobTargetRow;
    int32_t             mTargetZombieID;
    int32_t             mLastPortalX;
};

struct Coin_32
{
    float               mPosX;
    float               mPosY;
    float               mVelX;
    float               mVelY;
    float               mScale;
    bool                mDead;
    uint8_t             mPad1[3];
    int32_t             mFadeCount;
    float               mCollectX;
    float               mCollectY;
    int32_t             mGroundY;
    int32_t             mCoinAge;
    bool                mIsBeingCollected;
    uint8_t             mPad2[3];
    int32_t             mDisappearCounter;
    int32_t             mType;
    int32_t             mCoinMotion;
    int32_t             mAttachmentID;
    float               mCollectionDistance;
    int32_t             mUsableSeedType;
    uint8_t             mPadBeforePottedPlant[4];
    PottedPlant_32      mPottedPlantSpec;
    bool                mNeedsBouncyArrow;
    bool                mHasBouncyArrow;
    bool                mHitGround;
    uint8_t             mPad3;
    int32_t             mTimesDropped;
};

struct TodParticleSystem_32
{
    int32_t                 mEffectType;
    int32_t                 mParticleDef;
    int32_t                 mParticleHolder;
    int32_t                 mEmitterList_mHead;
    int32_t                 mEmitterList_mTail;
    int32_t                 mEmitterList_mSize;
    int32_t                 mEmitterList_mpAllocator;
    bool                    mDead;
    bool                    mIsAttachment;
    uint8_t                 mPad1[2];
    int32_t                 mRenderOrder;
    bool                    mDontUpdate;
    uint8_t                 mPad2[3];
};

struct TodParticleEmitter_32
{
    int32_t                 mEmitterDef;
    int32_t                 mParticleSystem;
    int32_t                 mParticleList_mHead;
    int32_t                 mParticleList_mTail;
    int32_t                 mParticleList_mSize;
    int32_t                 mParticleList_mpAllocator;
    float                   mSpawnAccum;
    float                   mSystemCenter_x;
    float                   mSystemCenter_y;
    int32_t                 mParticlesSpawned;
    int32_t                 mSystemAge;
    int32_t                 mSystemDuration;
    float                   mSystemTimeValue;
    float                   mSystemLastTimeValue;
    bool                    mDead;
    uint8_t                 mPad1[3];
    int32_t                 mColorOverride[4];
    bool                    mExtraAdditiveDrawOverride;
    uint8_t                 mPad2[3];
    float                   mScaleOverride;
    int32_t                 mImageOverride;
    int32_t                 mCrossFadeEmitterID;
    int32_t                 mEmitterCrossFadeCountDown;
    int32_t                 mFrameOverride;
    float                   mTrackInterp[10];
    float                   mSystemFieldInterp[4][2];
};

struct TodParticle_32
{
    int32_t                 mParticleEmitter;
    int32_t                 mParticleDuration;
    int32_t                 mParticleAge;
    float                   mParticleTimeValue;
    float                   mParticleLastTimeValue;
    float                   mAnimationTimeValue;
    float                   mVelocity_x;
    float                   mVelocity_y;
    float                   mPosition_x;
    float                   mPosition_y;
    int32_t                 mImageFrame;
    float                   mSpinPosition;
    float                   mSpinVelocity;
    int32_t                 mCrossFadeParticleID;
    int32_t                 mCrossFadeDuration;
    float                   mParticleInterp[16];
    float                   mParticleFieldInterp[4][2];
};

struct Reanimation_32
{
    int32_t                 mReanimationType;
    float                   mAnimTime;
    float                   mAnimRate;
    int32_t                 mDefinition;
    int32_t                 mLoopType;
    bool                    mDead;
    uint8_t                 mPad1[3];
    int32_t                 mFrameStart;
    int32_t                 mFrameCount;
    int32_t                 mFrameBasePose;
    float                   mOverlayMatrix[9];
    int32_t                 mColorOverride[4];
    int32_t                 mTrackInstances;
    int32_t                 mLoopCount;
    int32_t                 mReanimationHolder;
    bool                    mIsAttachment;
    uint8_t                 mPad2[3];
    int32_t                 mRenderOrder;
};

struct Trail_32
{
    float                   mTrailPoints[20][2];
    int32_t                 mNumTrailPoints;
    bool                    mDead;
    uint8_t                 mPad1[3];
    int32_t                 mRenderOrder;
    int32_t                 mTrailAge;
    int32_t                 mTrailDuration;
    int32_t                 mDefinition;
    int32_t                 mTrailHolder;
    float                   mTrailInterp[4];
    float                   mTrailCenter_x;
    float                   mTrailCenter_y;
    bool                    mIsAttachment;
    uint8_t                 mPad2[3];
};

struct AttachEffect_32
{
    uint32_t                mEffectID;
    int32_t                 mEffectType;
    float                   mOffset[9];
    bool                    mDontDrawIfParentHidden;
    bool                    mDontPropogateColor;
    uint8_t                 mPad1[2];
};

struct Attachment_32
{
    AttachEffect_32         mEffectArray[16];
    int32_t                 mNumEffects;
    bool                    mDead;
    uint8_t                 mPad2[3];
};
#pragma pack(pop)


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
void MapPottedPlantToDisk(const PottedPlant& src, PottedPlant_32& dest)
{
    dest.mSeedType = (int32_t)src.mSeedType;
    dest.mWhichZenGarden = (int32_t)src.mWhichZenGarden;
    dest.mX = src.mX;
    dest.mY = src.mY;
    dest.mFacing = (int32_t)src.mFacing;
    memset(dest.mPad1, 0, sizeof(dest.mPad1));
    dest.mLastWateredTime = (int64_t)src.mLastWateredTime;
    dest.mDrawVariation = (int32_t)src.mDrawVariation;
    dest.mPlantAge = (int32_t)src.mPlantAge;
    dest.mTimesFed = src.mTimesFed;
    dest.mFeedingsPerGrow = src.mFeedingsPerGrow;
    dest.mPlantNeed = (int32_t)src.mPlantNeed;
    memset(dest.mPad2, 0, sizeof(dest.mPad2));
    dest.mLastNeedFulfilledTime = (int64_t)src.mLastNeedFulfilledTime;
    dest.mLastFertilizedTime = (int64_t)src.mLastFertilizedTime;
    dest.mLastChocolateTime = (int64_t)src.mLastChocolateTime;
    dest.mFutureAttribute[0] = (int64_t)src.mFutureAttribute[0];
}

void MapPottedPlantFromDisk(const PottedPlant_32& src, PottedPlant& dest)
{
    dest.mSeedType = (SeedType)src.mSeedType;
    dest.mWhichZenGarden = (GardenType)src.mWhichZenGarden;
    dest.mX = src.mX;
    dest.mY = src.mY;
    dest.mFacing = (PottedPlant::FacingDirection)src.mFacing;
    dest.mLastWateredTime = (__time64_t)src.mLastWateredTime;
    dest.mDrawVariation = (DrawVariation)src.mDrawVariation;
    dest.mPlantAge = (PottedPlantAge)src.mPlantAge;
    dest.mTimesFed = src.mTimesFed;
    dest.mFeedingsPerGrow = src.mFeedingsPerGrow;
    dest.mPlantNeed = (PottedPlantNeed)src.mPlantNeed;
    dest.mLastNeedFulfilledTime = (__time64_t)src.mLastNeedFulfilledTime;
    dest.mLastFertilizedTime = (__time64_t)src.mLastFertilizedTime;
    dest.mLastChocolateTime = (__time64_t)src.mLastChocolateTime;
    dest.mFutureAttribute[0] = (__time64_t)src.mFutureAttribute[0];
}

void MapZombieToDisk(Zombie* src, Zombie_32& dest)
{
    memset(&dest, 0, sizeof(Zombie_32));
    dest.mZombieType = (int32_t)src->mZombieType;
    dest.mZombiePhase = (int32_t)src->mZombiePhase;
    dest.mPosX = src->mPosX;
    dest.mPosY = src->mPosY;
    dest.mVelX = src->mVelX;
    dest.mAnimCounter = src->mAnimCounter;
    dest.mGroanCounter = src->mGroanCounter;
    dest.mAnimTicksPerFrame = src->mAnimTicksPerFrame;
    dest.mAnimFrames = src->mAnimFrames;
    dest.mFrame = src->mFrame;
    dest.mPrevFrame = src->mPrevFrame;
    dest.mVariant = src->mVariant;
    dest.mIsEating = src->mIsEating;
    dest.mJustGotShotCounter = src->mJustGotShotCounter;
    dest.mShieldJustGotShotCounter = src->mShieldJustGotShotCounter;
    dest.mShieldRecoilCounter = src->mShieldRecoilCounter;
    dest.mZombieAge = src->mZombieAge;
    dest.mZombieHeight = (int32_t)src->mZombieHeight;
    dest.mPhaseCounter = src->mPhaseCounter;
    dest.mFromWave = src->mFromWave;
    dest.mDroppedLoot = src->mDroppedLoot;
    dest.mZombieFade = src->mZombieFade;
    dest.mFlatTires = src->mFlatTires;
    dest.mUseLadderCol = src->mUseLadderCol;
    dest.mTargetCol = src->mTargetCol;
    dest.mAltitude = src->mAltitude;
    dest.mHitUmbrella = src->mHitUmbrella;
    dest.mZombieRect[0] = src->mZombieRect.mX;
    dest.mZombieRect[1] = src->mZombieRect.mY;
    dest.mZombieRect[2] = src->mZombieRect.mWidth;
    dest.mZombieRect[3] = src->mZombieRect.mHeight;
    dest.mZombieAttackRect[0] = src->mZombieAttackRect.mX;
    dest.mZombieAttackRect[1] = src->mZombieAttackRect.mY;
    dest.mZombieAttackRect[2] = src->mZombieAttackRect.mWidth;
    dest.mZombieAttackRect[3] = src->mZombieAttackRect.mHeight;
    dest.mChilledCounter = src->mChilledCounter;
    dest.mButteredCounter = src->mButteredCounter;
    dest.mIceTrapCounter = src->mIceTrapCounter;
    dest.mMindControlled = src->mMindControlled;
    dest.mBlowingAway = src->mBlowingAway;
    dest.mHasHead = src->mHasHead;
    dest.mHasArm = src->mHasArm;
    dest.mHasObject = src->mHasObject;
    dest.mInPool = src->mInPool;
    dest.mOnHighGround = src->mOnHighGround;
    dest.mYuckyFace = src->mYuckyFace;
    dest.mYuckyFaceCounter = src->mYuckyFaceCounter;
    dest.mHelmType = (int32_t)src->mHelmType;
    dest.mBodyHealth = src->mBodyHealth;
    dest.mBodyMaxHealth = src->mBodyMaxHealth;
    dest.mHelmHealth = src->mHelmHealth;
    dest.mHelmMaxHealth = src->mHelmMaxHealth;
    dest.mShieldType = (int32_t)src->mShieldType;
    dest.mShieldHealth = src->mShieldHealth;
    dest.mShieldMaxHealth = src->mShieldMaxHealth;
    dest.mFlyingHealth = src->mFlyingHealth;
    dest.mFlyingMaxHealth = src->mFlyingMaxHealth;
    dest.mDead = src->mDead;
    dest.mRelatedZombieID = src->mRelatedZombieID;
    for (int i = 0; i < 4; i++) dest.mFollowerZombieID[i] = src->mFollowerZombieID[i];
    dest.mPlayingSong = src->mPlayingSong;
    dest.mParticleOffsetX = src->mParticleOffsetX;
    dest.mParticleOffsetY = src->mParticleOffsetY;
    dest.mAttachmentID = src->mAttachmentID;
    dest.mSummonCounter = src->mSummonCounter;
    dest.mBodyReanimID = src->mBodyReanimID;
    dest.mScaleZombie = src->mScaleZombie;
    dest.mVelZ = src->mVelZ;
    dest.mOriginalAnimRate = src->mOriginalAnimRate;
    dest.mTargetPlantID = src->mTargetPlantID;
    dest.mBossMode = src->mBossMode;
    dest.mTargetRow = src->mTargetRow;
    dest.mBossBungeeCounter = src->mBossBungeeCounter;
    dest.mBossStompCounter = src->mBossStompCounter;
    dest.mBossHeadCounter = src->mBossHeadCounter;
    dest.mBossFireBallReanimID = src->mBossFireBallReanimID;
    dest.mSpecialHeadReanimID = src->mSpecialHeadReanimID;
    dest.mFireballRow = src->mFireballRow;
    dest.mIsFireBall = src->mIsFireBall;
    dest.mMoweredReanimID = src->mMoweredReanimID;
    dest.mLastPortalX = src->mLastPortalX;
}

void MapZombieFromDisk(const Zombie_32& src, Zombie* dest)
{
    dest->mZombieType = (ZombieType)src.mZombieType;
    dest->mZombiePhase = (ZombiePhase)src.mZombiePhase;
    dest->mPosX = src.mPosX;
    dest->mPosY = src.mPosY;
    dest->mVelX = src.mVelX;
    dest->mAnimCounter = src.mAnimCounter;
    dest->mGroanCounter = src.mGroanCounter;
    dest->mAnimTicksPerFrame = src.mAnimTicksPerFrame;
    dest->mAnimFrames = src.mAnimFrames;
    dest->mFrame = src.mFrame;
    dest->mPrevFrame = src.mPrevFrame;
    dest->mVariant = src.mVariant;
    dest->mIsEating = src.mIsEating;
    dest->mJustGotShotCounter = src.mJustGotShotCounter;
    dest->mShieldJustGotShotCounter = src.mShieldJustGotShotCounter;
    dest->mShieldRecoilCounter = src.mShieldRecoilCounter;
    dest->mZombieAge = src.mZombieAge;
    dest->mZombieHeight = (ZombieHeight)src.mZombieHeight;
    dest->mPhaseCounter = src.mPhaseCounter;
    dest->mFromWave = src.mFromWave;
    dest->mDroppedLoot = src.mDroppedLoot;
    dest->mZombieFade = src.mZombieFade;
    dest->mFlatTires = src.mFlatTires;
    dest->mUseLadderCol = src.mUseLadderCol;
    dest->mTargetCol = src.mTargetCol;
    dest->mAltitude = src.mAltitude;
    dest->mHitUmbrella = src.mHitUmbrella;
    dest->mZombieRect.mX = src.mZombieRect[0];
    dest->mZombieRect.mY = src.mZombieRect[1];
    dest->mZombieRect.mWidth = src.mZombieRect[2];
    dest->mZombieRect.mHeight = src.mZombieRect[3];
    dest->mZombieAttackRect.mX = src.mZombieAttackRect[0];
    dest->mZombieAttackRect.mY = src.mZombieAttackRect[1];
    dest->mZombieAttackRect.mWidth = src.mZombieAttackRect[2];
    dest->mZombieAttackRect.mHeight = src.mZombieAttackRect[3];
    dest->mChilledCounter = src.mChilledCounter;
    dest->mButteredCounter = src.mButteredCounter;
    dest->mIceTrapCounter = src.mIceTrapCounter;
    dest->mMindControlled = src.mMindControlled;
    dest->mBlowingAway = src.mBlowingAway;
    dest->mHasHead = src.mHasHead;
    dest->mHasArm = src.mHasArm;
    dest->mHasObject = src.mHasObject;
    dest->mInPool = src.mInPool;
    dest->mOnHighGround = src.mOnHighGround;
    dest->mYuckyFace = src.mYuckyFace;
    dest->mYuckyFaceCounter = src.mYuckyFaceCounter;
    dest->mHelmType = (HelmType)src.mHelmType;
    dest->mBodyHealth = src.mBodyHealth;
    dest->mBodyMaxHealth = src.mBodyMaxHealth;
    dest->mHelmHealth = src.mHelmHealth;
    dest->mHelmMaxHealth = src.mHelmMaxHealth;
    dest->mShieldType = (ShieldType)src.mShieldType;
    dest->mShieldHealth = src.mShieldHealth;
    dest->mShieldMaxHealth = src.mShieldMaxHealth;
    dest->mFlyingHealth = src.mFlyingHealth;
    dest->mFlyingMaxHealth = src.mFlyingMaxHealth;
    dest->mDead = src.mDead;
    dest->mRelatedZombieID = src.mRelatedZombieID;
    for (int i = 0; i < 4; i++) dest->mFollowerZombieID[i] = src.mFollowerZombieID[i];
    dest->mPlayingSong = src.mPlayingSong;
    dest->mParticleOffsetX = src.mParticleOffsetX;
    dest->mParticleOffsetY = src.mParticleOffsetY;
    dest->mAttachmentID = src.mAttachmentID;
    dest->mSummonCounter = src.mSummonCounter;
    dest->mBodyReanimID = src.mBodyReanimID;
    dest->mScaleZombie = src.mScaleZombie;
    dest->mVelZ = src.mVelZ;
    dest->mOriginalAnimRate = src.mOriginalAnimRate;
    dest->mTargetPlantID = src.mTargetPlantID;
    dest->mBossMode = src.mBossMode;
    dest->mTargetRow = src.mTargetRow;
    dest->mBossBungeeCounter = src.mBossBungeeCounter;
    dest->mBossStompCounter = src.mBossStompCounter;
    dest->mBossHeadCounter = src.mBossHeadCounter;
    dest->mBossFireBallReanimID = src.mBossFireBallReanimID;
    dest->mSpecialHeadReanimID = src.mSpecialHeadReanimID;
    dest->mFireballRow = src.mFireballRow;
    dest->mIsFireBall = src.mIsFireBall;
    dest->mMoweredReanimID = src.mMoweredReanimID;
    dest->mLastPortalX = src.mLastPortalX;
}

void MapPlantToDisk(Plant* src, Plant_32& dest)
{
    memset(&dest, 0, sizeof(Plant_32));
    dest.mSeedType = (int32_t)src->mSeedType;
    dest.mPlantCol = src->mPlantCol;
    dest.mAnimCounter = src->mAnimCounter;
    dest.mFrame = src->mFrame;
    dest.mFrameLength = src->mFrameLength;
    dest.mNumFrames = src->mNumFrames;
    dest.mState = (int32_t)src->mState;
    dest.mPlantHealth = src->mPlantHealth;
    dest.mPlantMaxHealth = src->mPlantMaxHealth;
    dest.mSubclass = src->mSubclass;
    dest.mDisappearCountdown = src->mDisappearCountdown;
    dest.mDoSpecialCountdown = src->mDoSpecialCountdown;
    dest.mStateCountdown = src->mStateCountdown;
    dest.mLaunchCounter = src->mLaunchCounter;
    dest.mLaunchRate = src->mLaunchRate;
    dest.mPlantRect[0] = src->mPlantRect.mX;
    dest.mPlantRect[1] = src->mPlantRect.mY;
    dest.mPlantRect[2] = src->mPlantRect.mWidth;
    dest.mPlantRect[3] = src->mPlantRect.mHeight;
    dest.mPlantAttackRect[0] = src->mPlantAttackRect.mX;
    dest.mPlantAttackRect[1] = src->mPlantAttackRect.mY;
    dest.mPlantAttackRect[2] = src->mPlantAttackRect.mWidth;
    dest.mPlantAttackRect[3] = src->mPlantAttackRect.mHeight;
    dest.mTargetX = src->mTargetX;
    dest.mTargetY = src->mTargetY;
    dest.mStartRow = src->mStartRow;
    dest.mParticleID = src->mParticleID;
    dest.mShootingCounter = src->mShootingCounter;
    dest.mBodyReanimID = src->mBodyReanimID;
    dest.mHeadReanimID = src->mHeadReanimID;
    dest.mHeadReanimID2 = src->mHeadReanimID2;
    dest.mHeadReanimID3 = src->mHeadReanimID3;
    dest.mBlinkReanimID = src->mBlinkReanimID;
    dest.mLightReanimID = src->mLightReanimID;
    dest.mSleepingReanimID = src->mSleepingReanimID;
    dest.mBlinkCountdown = src->mBlinkCountdown;
    dest.mRecentlyEatenCountdown = src->mRecentlyEatenCountdown;
    dest.mEatenFlashCountdown = src->mEatenFlashCountdown;
    dest.mBeghouledFlashCountdown = src->mBeghouledFlashCountdown;
    dest.mShakeOffsetX = src->mShakeOffsetX;
    dest.mShakeOffsetY = src->mShakeOffsetY;
    for (int i = 0; i < 5; i++)
    {
        dest.mMagnetItems[i].mPosX = src->mMagnetItems[i].mPosX;
        dest.mMagnetItems[i].mPosY = src->mMagnetItems[i].mPosY;
        dest.mMagnetItems[i].mDestOffsetX = src->mMagnetItems[i].mDestOffsetX;
        dest.mMagnetItems[i].mDestOffsetY = src->mMagnetItems[i].mDestOffsetY;
        dest.mMagnetItems[i].mItemType = (int32_t)src->mMagnetItems[i].mItemType;
    }
    dest.mTargetZombieID = src->mTargetZombieID;
    dest.mWakeUpCounter = src->mWakeUpCounter;
    dest.mOnBungeeState = (int32_t)src->mOnBungeeState;
    dest.mImitaterType = (int32_t)src->mImitaterType;
    dest.mPottedPlantIndex = src->mPottedPlantIndex;
    dest.mAnimPing = src->mAnimPing;
    dest.mDead = src->mDead;
    dest.mSquished = src->mSquished;
    dest.mIsAsleep = src->mIsAsleep;
    dest.mIsOnBoard = src->mIsOnBoard;
    dest.mHighlighted = src->mHighlighted;
}

void MapPlantFromDisk(const Plant_32& src, Plant* dest)
{
    dest->mSeedType = (SeedType)src.mSeedType;
    dest->mPlantCol = src.mPlantCol;
    dest->mAnimCounter = src.mAnimCounter;
    dest->mFrame = src.mFrame;
    dest->mFrameLength = src.mFrameLength;
    dest->mNumFrames = src.mNumFrames;
    dest->mState = (PlantState)src.mState;
    dest->mPlantHealth = src.mPlantHealth;
    dest->mPlantMaxHealth = src.mPlantMaxHealth;
    dest->mSubclass = src.mSubclass;
    dest->mDisappearCountdown = src.mDisappearCountdown;
    dest->mDoSpecialCountdown = src.mDoSpecialCountdown;
    dest->mStateCountdown = src.mStateCountdown;
    dest->mLaunchCounter = src.mLaunchCounter;
    dest->mLaunchRate = src.mLaunchRate;
    dest->mPlantRect.mX = src.mPlantRect[0];
    dest->mPlantRect.mY = src.mPlantRect[1];
    dest->mPlantRect.mWidth = src.mPlantRect[2];
    dest->mPlantRect.mHeight = src.mPlantRect[3];
    dest->mPlantAttackRect.mX = src.mPlantAttackRect[0];
    dest->mPlantAttackRect.mY = src.mPlantAttackRect[1];
    dest->mPlantAttackRect.mWidth = src.mPlantAttackRect[2];
    dest->mPlantAttackRect.mHeight = src.mPlantAttackRect[3];
    dest->mTargetX = src.mTargetX;
    dest->mTargetY = src.mTargetY;
    dest->mStartRow = src.mStartRow;
    dest->mParticleID = src.mParticleID;
    dest->mShootingCounter = src.mShootingCounter;
    dest->mBodyReanimID = src.mBodyReanimID;
    dest->mHeadReanimID = src.mHeadReanimID;
    dest->mHeadReanimID2 = src.mHeadReanimID2;
    dest->mHeadReanimID3 = src.mHeadReanimID3;
    dest->mBlinkReanimID = src.mBlinkReanimID;
    dest->mLightReanimID = src.mLightReanimID;
    dest->mSleepingReanimID = src.mSleepingReanimID;
    dest->mBlinkCountdown = src.mBlinkCountdown;
    dest->mRecentlyEatenCountdown = src.mRecentlyEatenCountdown;
    dest->mEatenFlashCountdown = src.mEatenFlashCountdown;
    dest->mBeghouledFlashCountdown = src.mBeghouledFlashCountdown;
    dest->mShakeOffsetX = src.mShakeOffsetX;
    dest->mShakeOffsetY = src.mShakeOffsetY;
    for (int i = 0; i < 5; i++)
    {
        dest->mMagnetItems[i].mPosX = src.mMagnetItems[i].mPosX;
        dest->mMagnetItems[i].mPosY = src.mMagnetItems[i].mPosY;
        dest->mMagnetItems[i].mDestOffsetX = src.mMagnetItems[i].mDestOffsetX;
        dest->mMagnetItems[i].mDestOffsetY = src.mMagnetItems[i].mDestOffsetY;
        dest->mMagnetItems[i].mItemType = (MagnetItemType)src.mMagnetItems[i].mItemType;
    }
    dest->mTargetZombieID = src.mTargetZombieID;
    dest->mWakeUpCounter = src.mWakeUpCounter;
    dest->mOnBungeeState = (PlantOnBungeeState)src.mOnBungeeState;
    dest->mImitaterType = (SeedType)src.mImitaterType;
    dest->mPottedPlantIndex = src.mPottedPlantIndex;
    dest->mAnimPing = src.mAnimPing;
    dest->mDead = src.mDead;
    dest->mSquished = src.mSquished;
    dest->mIsAsleep = src.mIsAsleep;
    dest->mIsOnBoard = src.mIsOnBoard;
    dest->mHighlighted = src.mHighlighted;
}

void MapProjectileToDisk(Projectile* src, Projectile_32& dest)
{
    memset(&dest, 0, sizeof(Projectile_32));
    dest.mFrame = src->mFrame;
    dest.mNumFrames = src->mNumFrames;
    dest.mAnimCounter = src->mAnimCounter;
    dest.mPosX = src->mPosX;
    dest.mPosY = src->mPosY;
    dest.mPosZ = src->mPosZ;
    dest.mVelX = src->mVelX;
    dest.mVelY = src->mVelY;
    dest.mVelZ = src->mVelZ;
    dest.mAccZ = src->mAccZ;
    dest.mShadowY = src->mShadowY;
    dest.mDead = src->mDead;
    dest.mAnimTicksPerFrame = src->mAnimTicksPerFrame;
    dest.mMotionType = (int32_t)src->mMotionType;
    dest.mProjectileType = (int32_t)src->mProjectileType;
    dest.mProjectileAge = src->mProjectileAge;
    dest.mClickBackoffCounter = src->mClickBackoffCounter;
    dest.mRotation = src->mRotation;
    dest.mRotationSpeed = src->mRotationSpeed;
    dest.mOnHighGround = src->mOnHighGround;
    dest.mDamageRangeFlags = src->mDamageRangeFlags;
    dest.mHitTorchwoodGridX = src->mHitTorchwoodGridX;
    dest.mAttachmentID = src->mAttachmentID;
    dest.mCobTargetX = src->mCobTargetX;
    dest.mCobTargetRow = src->mCobTargetRow;
    dest.mTargetZombieID = src->mTargetZombieID;
    dest.mLastPortalX = src->mLastPortalX;
}

void MapProjectileFromDisk(const Projectile_32& src, Projectile* dest)
{
    dest->mFrame = src.mFrame;
    dest->mNumFrames = src.mNumFrames;
    dest->mAnimCounter = src.mAnimCounter;
    dest->mPosX = src.mPosX;
    dest->mPosY = src.mPosY;
    dest->mPosZ = src.mPosZ;
    dest->mVelX = src.mVelX;
    dest->mVelY = src.mVelY;
    dest->mVelZ = src.mVelZ;
    dest->mAccZ = src.mAccZ;
    dest->mShadowY = src.mShadowY;
    dest->mDead = src.mDead;
    dest->mAnimTicksPerFrame = src.mAnimTicksPerFrame;
    dest->mMotionType = (ProjectileMotion)src.mMotionType;
    dest->mProjectileType = (ProjectileType)src.mProjectileType;
    dest->mProjectileAge = src.mProjectileAge;
    dest->mClickBackoffCounter = src.mClickBackoffCounter;
    dest->mRotation = src.mRotation;
    dest->mRotationSpeed = src.mRotationSpeed;
    dest->mOnHighGround = src.mOnHighGround;
    dest->mDamageRangeFlags = src.mDamageRangeFlags;
    dest->mHitTorchwoodGridX = src.mHitTorchwoodGridX;
    dest->mAttachmentID = src.mAttachmentID;
    dest->mCobTargetX = src.mCobTargetX;
    dest->mCobTargetRow = src.mCobTargetRow;
    dest->mTargetZombieID = src.mTargetZombieID;
    dest->mLastPortalX = src.mLastPortalX;
}

void MapCoinToDisk(Coin* src, Coin_32& dest)
{
    memset(&dest, 0, sizeof(Coin_32));
    dest.mPosX = src->mPosX;
    dest.mPosY = src->mPosY;
    dest.mVelX = src->mVelX;
    dest.mVelY = src->mVelY;
    dest.mScale = src->mScale;
    dest.mDead = src->mDead;
    dest.mFadeCount = src->mFadeCount;
    dest.mCollectX = src->mCollectX;
    dest.mCollectY = src->mCollectY;
    dest.mGroundY = src->mGroundY;
    dest.mCoinAge = src->mCoinAge;
    dest.mIsBeingCollected = src->mIsBeingCollected;
    dest.mDisappearCounter = src->mDisappearCounter;
    dest.mType = (int32_t)src->mType;
    dest.mCoinMotion = (int32_t)src->mCoinMotion;
    dest.mAttachmentID = src->mAttachmentID;
    dest.mCollectionDistance = src->mCollectionDistance;
    dest.mUsableSeedType = (int32_t)src->mUsableSeedType;
    MapPottedPlantToDisk(src->mPottedPlantSpec, dest.mPottedPlantSpec);
    dest.mNeedsBouncyArrow = src->mNeedsBouncyArrow;
    dest.mHasBouncyArrow = src->mHasBouncyArrow;
    dest.mHitGround = src->mHitGround;
    dest.mTimesDropped = src->mTimesDropped;
}

void MapCoinFromDisk(const Coin_32& src, Coin* dest)
{
    dest->mPosX = src.mPosX;
    dest->mPosY = src.mPosY;
    dest->mVelX = src.mVelX;
    dest->mVelY = src.mVelY;
    dest->mScale = src.mScale;
    dest->mDead = src.mDead;
    dest->mFadeCount = src.mFadeCount;
    dest->mCollectX = src.mCollectX;
    dest->mCollectY = src.mCollectY;
    dest->mGroundY = src.mGroundY;
    dest->mCoinAge = src.mCoinAge;
    dest->mIsBeingCollected = src.mIsBeingCollected;
    dest->mDisappearCounter = src.mDisappearCounter;
    dest->mType = (CoinType)src.mType;
    dest->mCoinMotion = (CoinMotion)src.mCoinMotion;
    dest->mAttachmentID = src.mAttachmentID;
    dest->mCollectionDistance = src.mCollectionDistance;
    dest->mUsableSeedType = (SeedType)src.mUsableSeedType;
    MapPottedPlantFromDisk(src.mPottedPlantSpec, dest->mPottedPlantSpec);
    dest->mNeedsBouncyArrow = src.mNeedsBouncyArrow;
    dest->mHasBouncyArrow = src.mHasBouncyArrow;
    dest->mHitGround = src.mHitGround;
    dest->mTimesDropped = src.mTimesDropped;
}

void MapParticleSystemToDisk(const TodParticleSystem* src, TodParticleSystem_32& dest)
{
    memset(&dest, 0, sizeof(TodParticleSystem_32));
    dest.mEffectType = (int32_t)src->mEffectType;
    dest.mParticleDef = 0;
    dest.mParticleHolder = 0;
    dest.mEmitterList_mHead = 0;
    dest.mEmitterList_mTail = 0;
    dest.mEmitterList_mSize = src->mEmitterList.mSize;
    dest.mEmitterList_mpAllocator = 0;
    dest.mDead = src->mDead;
    dest.mIsAttachment = src->mIsAttachment;
    dest.mRenderOrder = src->mRenderOrder;
    dest.mDontUpdate = src->mDontUpdate;
}

void MapParticleSystemFromDisk(const TodParticleSystem_32& src, TodParticleSystem* dest)
{
    dest->mEffectType = (ParticleEffect)src.mEffectType;
    dest->mParticleDef = nullptr;
    dest->mParticleHolder = nullptr;
    dest->mEmitterList.mHead = nullptr;
    dest->mEmitterList.mTail = nullptr;
    dest->mEmitterList.mSize = 0;
    dest->mEmitterList.mpAllocator = nullptr;
    dest->mDead = src.mDead;
    dest->mIsAttachment = src.mIsAttachment;
    dest->mRenderOrder = src.mRenderOrder;
    dest->mDontUpdate = src.mDontUpdate;
}

void MapParticleEmitterToDisk(const TodParticleEmitter* src, TodParticleEmitter_32& dest)
{
    memset(&dest, 0, sizeof(TodParticleEmitter_32));
    dest.mEmitterDef = 0;
    dest.mParticleSystem = 0;
    dest.mParticleList_mHead = 0;
    dest.mParticleList_mTail = 0;
    dest.mParticleList_mSize = src->mParticleList.mSize;
    dest.mParticleList_mpAllocator = 0;
    dest.mSpawnAccum = src->mSpawnAccum;
    dest.mSystemCenter_x = src->mSystemCenter.x;
    dest.mSystemCenter_y = src->mSystemCenter.y;
    dest.mParticlesSpawned = src->mParticlesSpawned;
    dest.mSystemAge = src->mSystemAge;
    dest.mSystemDuration = src->mSystemDuration;
    dest.mSystemTimeValue = src->mSystemTimeValue;
    dest.mSystemLastTimeValue = src->mSystemLastTimeValue;
    dest.mDead = src->mDead;
    dest.mColorOverride[0] = src->mColorOverride.mRed;
    dest.mColorOverride[1] = src->mColorOverride.mGreen;
    dest.mColorOverride[2] = src->mColorOverride.mBlue;
    dest.mColorOverride[3] = src->mColorOverride.mAlpha;
    dest.mExtraAdditiveDrawOverride = src->mExtraAdditiveDrawOverride;
    dest.mScaleOverride = src->mScaleOverride;
    dest.mImageOverride = 0;
    dest.mCrossFadeEmitterID = (int32_t)src->mCrossFadeEmitterID;
    dest.mEmitterCrossFadeCountDown = src->mEmitterCrossFadeCountDown;
    dest.mFrameOverride = src->mFrameOverride;
    for (int i = 0; i < 10; i++) dest.mTrackInterp[i] = src->mTrackInterp[i];
    for (int i = 0; i < 4; i++)
    {
        dest.mSystemFieldInterp[i][0] = src->mSystemFieldInterp[i][0];
        dest.mSystemFieldInterp[i][1] = src->mSystemFieldInterp[i][1];
    }
}

void MapParticleEmitterFromDisk(const TodParticleEmitter_32& src, TodParticleEmitter* dest)
{
    dest->mEmitterDef = nullptr;
    dest->mParticleSystem = nullptr;
    dest->mParticleList.mHead = nullptr;
    dest->mParticleList.mTail = nullptr;
    dest->mParticleList.mSize = 0;
    dest->mParticleList.mpAllocator = nullptr;
    dest->mSpawnAccum = src.mSpawnAccum;
    dest->mSystemCenter.x = src.mSystemCenter_x;
    dest->mSystemCenter.y = src.mSystemCenter_y;
    dest->mParticlesSpawned = src.mParticlesSpawned;
    dest->mSystemAge = src.mSystemAge;
    dest->mSystemDuration = src.mSystemDuration;
    dest->mSystemTimeValue = src.mSystemTimeValue;
    dest->mSystemLastTimeValue = src.mSystemLastTimeValue;
    dest->mDead = src.mDead;
    dest->mColorOverride = Color(src.mColorOverride[0], src.mColorOverride[1], src.mColorOverride[2], src.mColorOverride[3]);
    dest->mExtraAdditiveDrawOverride = src.mExtraAdditiveDrawOverride;
    dest->mScaleOverride = src.mScaleOverride;
    dest->mImageOverride = nullptr;
    dest->mCrossFadeEmitterID = (ParticleEmitterID)src.mCrossFadeEmitterID;
    dest->mEmitterCrossFadeCountDown = src.mEmitterCrossFadeCountDown;
    dest->mFrameOverride = src.mFrameOverride;
    for (int i = 0; i < 10; i++) dest->mTrackInterp[i] = src.mTrackInterp[i];
    for (int i = 0; i < 4; i++)
    {
        dest->mSystemFieldInterp[i][0] = src.mSystemFieldInterp[i][0];
        dest->mSystemFieldInterp[i][1] = src.mSystemFieldInterp[i][1];
    }
}

void MapParticleToDisk(const TodParticle* src, TodParticle_32& dest)
{
    memset(&dest, 0, sizeof(TodParticle_32));
    dest.mParticleEmitter = 0;
    dest.mParticleDuration = src->mParticleDuration;
    dest.mParticleAge = src->mParticleAge;
    dest.mParticleTimeValue = src->mParticleTimeValue;
    dest.mParticleLastTimeValue = src->mParticleLastTimeValue;
    dest.mAnimationTimeValue = src->mAnimationTimeValue;
    dest.mVelocity_x = src->mVelocity.x;
    dest.mVelocity_y = src->mVelocity.y;
    dest.mPosition_x = src->mPosition.x;
    dest.mPosition_y = src->mPosition.y;
    dest.mImageFrame = src->mImageFrame;
    dest.mSpinPosition = src->mSpinPosition;
    dest.mSpinVelocity = src->mSpinVelocity;
    dest.mCrossFadeParticleID = (int32_t)src->mCrossFadeParticleID;
    dest.mCrossFadeDuration = src->mCrossFadeDuration;
    for (int i = 0; i < 16; i++) dest.mParticleInterp[i] = src->mParticleInterp[i];
    for (int i = 0; i < 4; i++)
    {
        dest.mParticleFieldInterp[i][0] = src->mParticleFieldInterp[i][0];
        dest.mParticleFieldInterp[i][1] = src->mParticleFieldInterp[i][1];
    }
}

void MapParticleFromDisk(const TodParticle_32& src, TodParticle* dest)
{
    dest->mParticleEmitter = nullptr;
    dest->mParticleDuration = src.mParticleDuration;
    dest->mParticleAge = src.mParticleAge;
    dest->mParticleTimeValue = src.mParticleTimeValue;
    dest->mParticleLastTimeValue = src.mParticleLastTimeValue;
    dest->mAnimationTimeValue = src.mAnimationTimeValue;
    dest->mVelocity.x = src.mVelocity_x;
    dest->mVelocity.y = src.mVelocity_y;
    dest->mPosition.x = src.mPosition_x;
    dest->mPosition.y = src.mPosition_y;
    dest->mImageFrame = src.mImageFrame;
    dest->mSpinPosition = src.mSpinPosition;
    dest->mSpinVelocity = src.mSpinVelocity;
    dest->mCrossFadeParticleID = (ParticleID)src.mCrossFadeParticleID;
    dest->mCrossFadeDuration = src.mCrossFadeDuration;
    for (int i = 0; i < 16; i++) dest->mParticleInterp[i] = src.mParticleInterp[i];
    for (int i = 0; i < 4; i++)
    {
        dest->mParticleFieldInterp[i][0] = src.mParticleFieldInterp[i][0];
        dest->mParticleFieldInterp[i][1] = src.mParticleFieldInterp[i][1];
    }
}

void MapReanimationToDisk(const Reanimation* src, Reanimation_32& dest)
{
    memset(&dest, 0, sizeof(Reanimation_32));
    dest.mReanimationType = (int32_t)src->mReanimationType;
    dest.mAnimTime = src->mAnimTime;
    dest.mAnimRate = src->mAnimRate;
    dest.mDefinition = 0;
    dest.mLoopType = (int32_t)src->mLoopType;
    dest.mDead = src->mDead;
    dest.mFrameStart = src->mFrameStart;
    dest.mFrameCount = src->mFrameCount;
    dest.mFrameBasePose = src->mFrameBasePose;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            dest.mOverlayMatrix[i * 3 + j] = src->mOverlayMatrix.m[i][j];
        }
    }
    dest.mColorOverride[0] = src->mColorOverride.mRed;
    dest.mColorOverride[1] = src->mColorOverride.mGreen;
    dest.mColorOverride[2] = src->mColorOverride.mBlue;
    dest.mColorOverride[3] = src->mColorOverride.mAlpha;
    dest.mTrackInstances = 0;
    dest.mLoopCount = src->mLoopCount;
    dest.mReanimationHolder = 0;
    dest.mIsAttachment = src->mIsAttachment;
    dest.mRenderOrder = src->mRenderOrder;
}

void MapReanimationFromDisk(const Reanimation_32& src, Reanimation* dest)
{
    dest->mReanimationType = (ReanimationType)src.mReanimationType;
    dest->mAnimTime = src.mAnimTime;
    dest->mAnimRate = src.mAnimRate;
    dest->mDefinition = nullptr;
    dest->mLoopType = (ReanimLoopType)src.mLoopType;
    dest->mDead = src.mDead;
    dest->mFrameStart = src.mFrameStart;
    dest->mFrameCount = src.mFrameCount;
    dest->mFrameBasePose = src.mFrameBasePose;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            dest->mOverlayMatrix.m[i][j] = src.mOverlayMatrix[i * 3 + j];
        }
    }
    dest->mColorOverride = Color(src.mColorOverride[0], src.mColorOverride[1], src.mColorOverride[2], src.mColorOverride[3]);
    dest->mTrackInstances = nullptr;
    dest->mLoopCount = src.mLoopCount;
    dest->mReanimationHolder = nullptr;
    dest->mIsAttachment = src.mIsAttachment;
    dest->mRenderOrder = src.mRenderOrder;
    dest->mExtraAdditiveColor = Color(0, 0, 0, 0);
    dest->mEnableExtraAdditiveDraw = false;
    dest->mExtraOverlayColor = Color(0, 0, 0, 0);
    dest->mEnableExtraOverlayDraw = false;
    dest->mLastFrameTime = -1.0f;
    dest->mFilterEffect = FilterEffect::FILTER_EFFECT_NONE;
}

void MapTrailToDisk(const Trail* src, Trail_32& dest)
{
    memset(&dest, 0, sizeof(Trail_32));
    for (int i = 0; i < 20; i++)
    {
        dest.mTrailPoints[i][0] = src->mTrailPoints[i].aPos.x;
        dest.mTrailPoints[i][1] = src->mTrailPoints[i].aPos.y;
    }
    dest.mNumTrailPoints = src->mNumTrailPoints;
    dest.mDead = src->mDead;
    dest.mRenderOrder = src->mRenderOrder;
    dest.mTrailAge = src->mTrailAge;
    dest.mTrailDuration = src->mTrailDuration;
    dest.mDefinition = 0;
    dest.mTrailHolder = 0;
    for (int i = 0; i < 4; i++) dest.mTrailInterp[i] = src->mTrailInterp[i];
    dest.mTrailCenter_x = src->mTrailCenter.x;
    dest.mTrailCenter_y = src->mTrailCenter.y;
    dest.mIsAttachment = src->mIsAttachment;
}

void MapTrailFromDisk(const Trail_32& src, Trail* dest)
{
    for (int i = 0; i < 20; i++)
    {
        dest->mTrailPoints[i].aPos.x = src.mTrailPoints[i][0];
        dest->mTrailPoints[i].aPos.y = src.mTrailPoints[i][1];
    }
    dest->mNumTrailPoints = src.mNumTrailPoints;
    dest->mDead = src.mDead;
    dest->mRenderOrder = src.mRenderOrder;
    dest->mTrailAge = src.mTrailAge;
    dest->mTrailDuration = src.mTrailDuration;
    dest->mDefinition = nullptr;
    dest->mTrailHolder = nullptr;
    for (int i = 0; i < 4; i++) dest->mTrailInterp[i] = src.mTrailInterp[i];
    dest->mTrailCenter.x = src.mTrailCenter_x;
    dest->mTrailCenter.y = src.mTrailCenter_y;
    dest->mIsAttachment = src.mIsAttachment;
    dest->mColorOverride = Color::White;
}

void MapAttachmentToDisk(const Attachment* src, Attachment_32& dest)
{
    memset(&dest, 0, sizeof(Attachment_32));
    for (int k = 0; k < 16; k++)
    {
        dest.mEffectArray[k].mEffectID = src->mEffectArray[k].mEffectID;
        dest.mEffectArray[k].mEffectType = (int32_t)src->mEffectArray[k].mEffectType;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                dest.mEffectArray[k].mOffset[i * 3 + j] = src->mEffectArray[k].mOffset.m[i][j];
            }
        }
        dest.mEffectArray[k].mDontDrawIfParentHidden = src->mEffectArray[k].mDontDrawIfParentHidden;
        dest.mEffectArray[k].mDontPropogateColor = src->mEffectArray[k].mDontPropogateColor;
    }
    dest.mNumEffects = src->mNumEffects;
    dest.mDead = src->mDead;
}

void MapAttachmentFromDisk(const Attachment_32& src, Attachment* dest)
{
    for (int k = 0; k < 16; k++)
    {
        dest->mEffectArray[k].mEffectID = src.mEffectArray[k].mEffectID;
        dest->mEffectArray[k].mEffectType = (EffectType)src.mEffectArray[k].mEffectType;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                dest->mEffectArray[k].mOffset.m[i][j] = src.mEffectArray[k].mOffset[i * 3 + j];
            }
        }
        dest->mEffectArray[k].mDontDrawIfParentHidden = src.mEffectArray[k].mDontDrawIfParentHidden;
        dest->mEffectArray[k].mDontPropogateColor = src.mEffectArray[k].mDontPropogateColor;
    }
    dest->mNumEffects = src.mNumEffects;
    dest->mDead = src.mDead;
}

void SyncGameObject(SaveGameContext& theContext, GameObject* theGameObject)
{
    int32_t aDummyApp = 0;
    int32_t aDummyBoard = 0;
    theContext.SyncInt(aDummyApp);
    theContext.SyncInt(aDummyBoard);
    theContext.SyncInt(theGameObject->mX);
    theContext.SyncInt(theGameObject->mY);
    theContext.SyncInt(theGameObject->mWidth);
    theContext.SyncInt(theGameObject->mHeight);
    theContext.SyncBool(theGameObject->mVisible);
    theContext.SyncInt(theGameObject->mRow);
    theContext.SyncInt(theGameObject->mRenderOrder);
}

// Specializations for SyncDataArrayItem
template <typename T> void SyncObject(SaveGameContext& theContext, T* theObject);

template <> void SyncObject(SaveGameContext& theContext, Zombie* theZombie)
{
    SyncGameObject(theContext, theZombie);
    Zombie_32 zombie32;
    if (!theContext.mReading)
    {
        MapZombieToDisk(theZombie, zombie32);
    }
    SyncBytesRaw(theContext, &zombie32, sizeof(Zombie_32));
    if (theContext.mReading)
    {
        MapZombieFromDisk(zombie32, theZombie);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Plant* thePlant)
{
    SyncGameObject(theContext, thePlant);
    Plant_32 plant32;
    if (!theContext.mReading)
    {
        MapPlantToDisk(thePlant, plant32);
    }
    SyncBytesRaw(theContext, &plant32, sizeof(Plant_32));
    if (theContext.mReading)
    {
        MapPlantFromDisk(plant32, thePlant);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Projectile* theProjectile)
{
    SyncGameObject(theContext, theProjectile);
    Projectile_32 proj32;
    if (!theContext.mReading)
    {
        MapProjectileToDisk(theProjectile, proj32);
    }
    SyncBytesRaw(theContext, &proj32, sizeof(Projectile_32));
    if (theContext.mReading)
    {
        MapProjectileFromDisk(proj32, theProjectile);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Coin* theCoin)
{
    SyncGameObject(theContext, theCoin);
    Coin_32 coin32;
    if (!theContext.mReading)
    {
        MapCoinToDisk(theCoin, coin32);
    }
    SyncBytesRaw(theContext, &coin32, sizeof(Coin_32));
    if (theContext.mReading)
    {
        MapCoinFromDisk(coin32, theCoin);
    }
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

template <> void SyncObject(SaveGameContext& theContext, TodParticleSystem* theObject)
{
    TodParticleSystem_32 obj32;
    if (!theContext.mReading)
    {
        MapParticleSystemToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(TodParticleSystem_32));
    if (theContext.mReading)
    {
        MapParticleSystemFromDisk(obj32, theObject);
    }
}

template <> void SyncObject(SaveGameContext& theContext, TodParticleEmitter* theObject)
{
    TodParticleEmitter_32 obj32;
    if (!theContext.mReading)
    {
        MapParticleEmitterToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(TodParticleEmitter_32));
    if (theContext.mReading)
    {
        MapParticleEmitterFromDisk(obj32, theObject);
    }
}

template <> void SyncObject(SaveGameContext& theContext, TodParticle* theObject)
{
    TodParticle_32 obj32;
    if (!theContext.mReading)
    {
        MapParticleToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(TodParticle_32));
    if (theContext.mReading)
    {
        MapParticleFromDisk(obj32, theObject);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Reanimation* theObject)
{
    Reanimation_32 obj32;
    if (!theContext.mReading)
    {
        MapReanimationToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(Reanimation_32));
    if (theContext.mReading)
    {
        MapReanimationFromDisk(obj32, theObject);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Trail* theObject)
{
    Trail_32 obj32;
    if (!theContext.mReading)
    {
        MapTrailToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(Trail_32));
    if (theContext.mReading)
    {
        MapTrailFromDisk(obj32, theObject);
    }
}

template <> void SyncObject(SaveGameContext& theContext, Attachment* theObject)
{
    Attachment_32 obj32;
    if (!theContext.mReading)
    {
        MapAttachmentToDisk(theObject, obj32);
    }
    SyncBytesRaw(theContext, &obj32, sizeof(Attachment_32));
    if (theContext.mReading)
    {
        MapAttachmentFromDisk(obj32, theObject);
    }
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

void SyncTrackInstance(SaveGameContext& theContext, ReanimatorTrackInstance& theTrack);
void SyncCursorObject(SaveGameContext& theContext, CursorObject* theCursor);
void SyncCursorPreview(SaveGameContext& theContext, CursorPreview* thePreview);
void SyncMessageWidget(SaveGameContext& theContext, MessageWidget* theWidget);
void SyncSeedBank(SaveGameContext& theContext, SeedBank* theSeedBank);
void SyncChallenge(SaveGameContext& theContext, Challenge* theChallenge);
void SyncMusic(SaveGameContext& theContext, Music* theMusic);

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
			SyncTrackInstance(theContext, aTrackInstance);
			theContext.SyncImage(aTrackInstance.mImageOverride);

			if (theContext.mReading)
			{
				aTrackInstance.mBlendTransform.mText = "";
				TOD_ASSERT(aTrackInstance.mBlendTransform.mFont == nullptr);
			}
		}
	}
}

void SyncTrackInstance(SaveGameContext& theContext, ReanimatorTrackInstance& theTrack)
{
    theContext.SyncInt(theTrack.mBlendCounter);
    theContext.SyncInt(theTrack.mBlendTime);
    
    theContext.SyncFloat(theTrack.mBlendTransform.mTransX);
    theContext.SyncFloat(theTrack.mBlendTransform.mTransY);
    theContext.SyncFloat(theTrack.mBlendTransform.mSkewX);
    theContext.SyncFloat(theTrack.mBlendTransform.mSkewY);
    theContext.SyncFloat(theTrack.mBlendTransform.mScaleX);
    theContext.SyncFloat(theTrack.mBlendTransform.mScaleY);
    theContext.SyncFloat(theTrack.mBlendTransform.mFrame);
    theContext.SyncFloat(theTrack.mBlendTransform.mAlpha);
    
    int aZero = 0;
    theContext.SyncInt(aZero); // mImage
    theContext.SyncInt(aZero); // mFont
    theContext.SyncInt(aZero); // mText
    
    theContext.SyncFloat(theTrack.mShakeOverride);
    theContext.SyncFloat(theTrack.mShakeX);
    theContext.SyncFloat(theTrack.mShakeY);
    theContext.SyncEnum(theTrack.mAttachmentID);
    
    theContext.SyncInt(aZero); // mImageOverride
    
    theContext.SyncInt(theTrack.mRenderGroup);
    
    if (theContext.mReading)
    {
        int aColorVal = 0;
        theContext.SyncInt(aColorVal);
        theTrack.mTrackColor = Color((aColorVal >> 16) & 0xFF, (aColorVal >> 8) & 0xFF, aColorVal & 0xFF, (aColorVal >> 24) & 0xFF);
    }
    else
    {
        int aColorVal = (theTrack.mTrackColor.mAlpha << 24) | (theTrack.mTrackColor.mRed << 16) | (theTrack.mTrackColor.mGreen << 8) | theTrack.mTrackColor.mBlue;
        theContext.SyncInt(aColorVal);
    }
    
    int aMiddlePadding[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) theContext.SyncInt(aMiddlePadding[i]);
    
    char aBools[4];
    if (!theContext.mReading)
    {
        aBools[0] = theTrack.mIgnoreClipRect;
        aBools[1] = theTrack.mTruncateDisappearingFrames;
        aBools[2] = theTrack.mIgnoreColorOverride;
        aBools[3] = theTrack.mIgnoreExtraAdditiveColor;
    }
    SyncBytesRaw(theContext, aBools, 4);
    if (theContext.mReading)
    {
        theTrack.mIgnoreClipRect = (aBools[0] != 0);
        theTrack.mTruncateDisappearingFrames = (aBools[1] != 0);
        theTrack.mIgnoreColorOverride = (aBools[2] != 0);
        theTrack.mIgnoreExtraAdditiveColor = (aBools[3] != 0);
    }
}

void SyncCursorObject(SaveGameContext& theContext, CursorObject* theCursor)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_CURSOROBJECT_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_CURSOROBJECT_32);
    }

    SyncGameObject(theContext, theCursor);
    theContext.SyncInt(theCursor->mSeedBankIndex);
    theContext.SyncEnum(theCursor->mType);
    theContext.SyncEnum(theCursor->mImitaterType);
    theContext.SyncEnum(theCursor->mCursorType);
    theContext.SyncEnum(theCursor->mCoinID);
    theContext.SyncEnum(theCursor->mGlovePlantID);
    theContext.SyncEnum(theCursor->mDuplicatorPlantID);
    theContext.SyncEnum(theCursor->mCobCannonPlantID);
    theContext.SyncInt(theCursor->mHammerDownCounter);
    theContext.SyncEnum(theCursor->mReanimCursorID);
}

void SyncCursorPreview(SaveGameContext& theContext, CursorPreview* thePreview)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_CURSORPREVIEW_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_CURSORPREVIEW_32);
    }

    SyncGameObject(theContext, thePreview);
    theContext.SyncInt(thePreview->mGridX);
    theContext.SyncInt(thePreview->mGridY);
}

void SyncMessageWidget(SaveGameContext& theContext, MessageWidget* theWidget)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_MESSAGEWIDGET_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_MESSAGEWIDGET_32);
    }

    int aZero = 0;
    theContext.SyncInt(aZero); // mApp placeholder
    SyncBytesRaw(theContext, theWidget->mLabel, 128);
    theContext.SyncInt(theWidget->mDisplayTime);
    theContext.SyncInt(theWidget->mDuration);
    theContext.SyncEnum(theWidget->mMessageStyle);
    for (int i = 0; i < 128; i++) theContext.SyncEnum(theWidget->mTextReanimID[i]);
    theContext.SyncEnum(theWidget->mReanimType);
    theContext.SyncInt(theWidget->mSlideOffTime);
    SyncBytesRaw(theContext, theWidget->mLabelNext, 128);
    theContext.SyncEnum(theWidget->mMessageStyleNext);
}

void SyncSeedBank(SaveGameContext& theContext, SeedBank* theSeedBank)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_SEEDBANK_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_SEEDBANK_32);
    }

    SyncGameObject(theContext, theSeedBank);
    theContext.SyncInt(theSeedBank->mNumPackets);
    for (int i = 0; i < SEEDBANK_MAX; i++)
    {
        SeedPacket* aPacket = &theSeedBank->mSeedPackets[i];
        SyncGameObject(theContext, aPacket);
        theContext.SyncInt(aPacket->mRefreshCounter);
        theContext.SyncInt(aPacket->mRefreshTime);
        theContext.SyncInt(aPacket->mIndex);
        theContext.SyncInt(aPacket->mOffsetX);
        theContext.SyncEnum(aPacket->mPacketType);
        theContext.SyncEnum(aPacket->mImitaterType);
        theContext.SyncInt(aPacket->mSlotMachineCountDown);
        theContext.SyncEnum(aPacket->mSlotMachiningNextSeed);
        theContext.SyncFloat(aPacket->mSlotMachiningPosition);
        theContext.SyncBool(aPacket->mActive);
        theContext.SyncBool(aPacket->mRefreshing);
        theContext.SyncInt(aPacket->mTimesUsed);
    }
    theContext.SyncInt(theSeedBank->mCutSceneDarken);
    theContext.SyncInt(theSeedBank->mConveyorBeltCounter);
}

void SyncChallenge(SaveGameContext& theContext, Challenge* theChallenge)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_CHALLENGE_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_CHALLENGE_32);
    }

    int aZero = 0;
    theContext.SyncInt(aZero); // mApp placeholder
    theContext.SyncInt(aZero); // mBoard placeholder
    
    theContext.SyncBool(theChallenge->mBeghouledMouseCapture);
    theContext.SyncInt(theChallenge->mBeghouledMouseDownX);
    theContext.SyncInt(theChallenge->mBeghouledMouseDownY);
    SyncBytesRaw(theContext, theChallenge->mBeghouledEated, 54);
    SyncBytesRaw(theContext, theChallenge->mBeghouledPurcasedUpgrade, 3);
    char aPadding[3] = {0, 0, 0};
    SyncBytesRaw(theContext, aPadding, 3);
    
    theContext.SyncInt(theChallenge->mBeghouledMatchesThisMove);
    theContext.SyncEnum(theChallenge->mChallengeState);
    theContext.SyncInt(theChallenge->mChallengeStateCounter);
    theContext.SyncInt(theChallenge->mConveyorBeltCounter);
    theContext.SyncInt(theChallenge->mChallengeScore);
    theContext.SyncBool(theChallenge->mShowBowlingLine);
    theContext.SyncEnum(theChallenge->mLastConveyorSeedType);
    theContext.SyncInt(theChallenge->mSurvivalStage);
    theContext.SyncInt(theChallenge->mSlotMachineRollCount);
    theContext.SyncEnum(theChallenge->mReanimChallenge);
    
    for (int i = 0; i < 6; i++) theContext.SyncEnum(theChallenge->mReanimClouds[i]);
    for (int i = 0; i < 6; i++) theContext.SyncInt(theChallenge->mCloudsCounter[i]);
    
    theContext.SyncInt(theChallenge->mChallengeGridX);
    theContext.SyncInt(theChallenge->mChallengeGridY);
    theContext.SyncInt(theChallenge->mScaryPotterPots);
    theContext.SyncInt(theChallenge->mRainCounter);
    theContext.SyncInt(theChallenge->mTreeOfWisdomTalkIndex);
    
    int aEndPadding[4] = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++) theContext.SyncInt(aEndPadding[i]);
}

void SyncMusic(SaveGameContext& theContext, Music* theMusic)
{
    if (theContext.mReading)
    {
        if ((unsigned long)theContext.ByteLeftToRead() < 4) { theContext.mFailed = true; return; }
        int aReadSize = theContext.mBuffer.ReadLong();
        if (aReadSize != SIZEOF_MUSIC_32) { theContext.mFailed = true; return; }
    }
    else
    {
        theContext.mBuffer.WriteLong(SIZEOF_MUSIC_32);
    }

    int aZero = 0;
    theContext.SyncInt(aZero); // mApp placeholder
    theContext.SyncInt(aZero); // mMusicInterface placeholder
    
    theContext.SyncEnum(theMusic->mCurMusicTune);
    theContext.SyncEnum(theMusic->mCurMusicFileMain);
    theContext.SyncEnum(theMusic->mCurMusicFileDrums);
    theContext.SyncEnum(theMusic->mCurMusicFileHihats);
    theContext.SyncInt(theMusic->mBurstOverride);
    theContext.SyncInt(theMusic->mBaseBPM);
    theContext.SyncInt(theMusic->mBaseModSpeed);
    theContext.SyncEnum(theMusic->mMusicBurstState);
    theContext.SyncInt(theMusic->mBurstStateCounter);
    theContext.SyncEnum(theMusic->mMusicDrumsState);
    theContext.SyncInt(theMusic->mQueuedDrumTrackPackedOrder);
    theContext.SyncInt(theMusic->mDrumsStateCounter);
    theContext.SyncInt(theMusic->mPauseOffset);
    theContext.SyncInt(theMusic->mPauseOffsetDrums);
    theContext.SyncBool(theMusic->mPaused);
    theContext.SyncBool(theMusic->mMusicDisabled);
    theContext.SyncInt(theMusic->mFadeOutCounter);
    theContext.SyncInt(theMusic->mFadeOutDuration);
}
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

    SyncCursorObject(theContext, theBoard->mCursorObject);
    SyncCursorPreview(theContext, theBoard->mCursorPreview);
    SyncMessageWidget(theContext, theBoard->mAdvice);
    SyncSeedBank(theContext, theBoard->mSeedBank);
    SyncChallenge(theContext, theBoard->mChallenge);
    SyncMusic(theContext, theBoard->mApp->mMusic);

    if (theContext.mReading)
    {
        theBoard->mApp->mEffectSystem->mParticleHolder->mParticleSystems.DataArrayFreeAll();
        theBoard->mApp->mEffectSystem->mParticleHolder->mEmitters.DataArrayFreeAll();
        theBoard->mApp->mEffectSystem->mParticleHolder->mParticles.DataArrayFreeAll();
        theBoard->mApp->mEffectSystem->mParticleHolder->mEmitterListNodeAllocator.FreeAll();
        theBoard->mApp->mEffectSystem->mParticleHolder->mParticleListNodeAllocator.FreeAll();
    }
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
    if (theContext.mReading)
    {
        theTrail->mTrailHolder = theBoard->mApp->mEffectSystem->mTrailHolder;
    }
}

//0x481A40
void SyncParticleEmitter(TodParticleSystem* theParticleSystem, TodParticleEmitter* theParticleEmitter, SaveGameContext& theContext)
{
	theContext.SyncEmitterDef(theParticleEmitter->mEmitterDef);
	theContext.SyncImage(theParticleEmitter->mImageOverride);
    if (theContext.mReading)
    {
        theParticleEmitter->mParticleSystem = theParticleSystem;
    }
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

