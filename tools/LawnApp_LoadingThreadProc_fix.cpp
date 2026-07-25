//0x4528E0
void LawnApp::LoadingThreadProc()
{
	TodTraceAndLog("LawnApp::LoadingThreadProc: Started");
	if (!TodLoadResources("LoaderBar"))
	{
		TodTraceAndLog("LawnApp::LoadingThreadProc: Failed to load LoaderBar");
		return;
	}

	TodTraceAndLog("LawnApp::LoadingThreadProc: Loading LawnStrings.txt");
	TodStringListLoad("Properties\\LawnStrings.txt");

	if (mTitleScreen)
	{
		mTitleScreen->mLoaderScreenIsLoaded = true;
	}

	TodTraceAndLog("LawnApp::LoadingThreadProc: Precalculating tasks");
	const char* groups[] = { "LoadingFonts", "LoadingImages", "LoadingSounds" };
	int group_ave_ms_to_load[] = { 54, 9, 54 };
	for (int i = 0; i < 3; i++)
	{
		mNumLoadingThreadTasks += mResourceManager->GetNumResources(groups[i]) * group_ave_ms_to_load[i];
	}
	mNumLoadingThreadTasks += 636;
	mNumLoadingThreadTasks += GetNumPreloadingTasks();
	mNumLoadingThreadTasks += mMusic->GetNumLoadingTasks();

	PerfTimer aTimer;
	aTimer.Start();

	TodHesitationTrace("start loading");
	TodHesitationBracket aHesitationResources("Resources");
	TodHesitationTrace("loading thread start");

	TodTraceAndLog("LawnApp::LoadingThreadProc: Loading resource groups");
	LoadGroup("LoadingImages", 9);
	LoadGroup("LoadingFonts", 54);
	if (mLoadingFailed || mShutdown || mCloseRequest)
		return;

	aHesitationResources.EndBracket();
	TodTrace("loading '%s' %d ms", "resources", (int)aTimer.GetDuration());

	TodTraceAndLog("LawnApp::LoadingThreadProc: Initializing Music");
	mMusic->MusicInit();
	int aDuration = max(aTimer.GetDuration(), 0);
	aTimer.Start();

	TodTraceAndLog("LawnApp::LoadingThreadProc: Creating Subsystems");
	mPoolEffect = new PoolEffect();
	mPoolEffect->PoolEffectInitialize();
	mZenGarden = new ZenGarden();
	mReanimatorCache = new ReanimatorCache();
	mReanimatorCache->ReanimatorCacheInitialize();
	TodFoleyInitialize(gLawnFoleyParamArray, LENGTH(gLawnFoleyParamArray));

	TodTrace("loading '%s' %d ms", "stuff", (int)aTimer.GetDuration());
	aTimer.Start();

	TodTraceAndLog("LawnApp::LoadingThreadProc: Loading Trail definitions");
	TrailLoadDefinitions(gLawnTrailArray, LENGTH(gLawnTrailArray));
	TodTrace("loading '%s' %d ms", "trail", (int)aTimer.GetDuration());
	aTimer.Start();
	TodHesitationTrace("trail");
	
	TodTraceAndLog("LawnApp::LoadingThreadProc: Loading Particle definitions");
	TodParticleLoadDefinitions(gLawnParticleArray, LENGTH(gLawnParticleArray));
	aDuration = max(aTimer.GetDuration(), 0);
	aTimer.Start();

	TodTraceAndLog("LawnApp::LoadingThreadProc: Preloading for user");
	PreloadForUser();
	if (mLoadingFailed || mShutdown || mCloseRequest)
		return;

	aDuration = max(aTimer.GetDuration(), 0);
	aTimer.Start();

	TodTraceAndLog("LawnApp::LoadingThreadProc: Loading Sound group");
	GetNumPreloadingTasks();
	LoadGroup("LoadingSounds", 54);
	TodHesitationTrace("finished loading");
	TodTraceAndLog("LawnApp::LoadingThreadProc: Done");
}
