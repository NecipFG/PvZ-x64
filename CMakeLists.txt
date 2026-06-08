cmake_minimum_required(VERSION 3.16)
project(PvZ-x64 CXX C)
if(WIN32)
    enable_language(RC)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Source root (project root, one level above SexyAppFramework/)
# ---------------------------------------------------------------------------
set(ROOT ${CMAKE_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Game sources  (mirrors SexyAppFramework/SexyAppBase.vcxproj ClCompile list)
# ---------------------------------------------------------------------------
set(GAME_SOURCES
    ${ROOT}/main.cpp
    ${ROOT}/LawnApp.cpp
    ${ROOT}/Resources.cpp

    # Lawn – core objects
    ${ROOT}/Lawn/Board.cpp
    ${ROOT}/Lawn/Challenge.cpp
    ${ROOT}/Lawn/Coin.cpp
    ${ROOT}/Lawn/CursorObject.cpp
    ${ROOT}/Lawn/CutScene.cpp
    ${ROOT}/Lawn/GameObject.cpp
    ${ROOT}/Lawn/GridItem.cpp
    ${ROOT}/Lawn/LawnCommon.cpp
    ${ROOT}/Lawn/LawnMower.cpp
    ${ROOT}/Lawn/MessageWidget.cpp
    ${ROOT}/Lawn/Plant.cpp
    ${ROOT}/Lawn/Projectile.cpp
    ${ROOT}/Lawn/SeedPacket.cpp
    ${ROOT}/Lawn/Hotkey.cpp
    ${ROOT}/Lawn/ToolTipWidget.cpp
    ${ROOT}/Lawn/ZenGarden.cpp
    ${ROOT}/Lawn/Zombie.cpp

    # Lawn – subsystems
    ${ROOT}/Lawn/System/DataSync.cpp
    ${ROOT}/Lawn/System/PopDRMComm.cpp
    ${ROOT}/Lawn/System/Music.cpp
    ${ROOT}/Lawn/System/PlayerInfo.cpp
    ${ROOT}/Lawn/System/PoolEffect.cpp
    ${ROOT}/Lawn/System/ProfileMgr.cpp
    ${ROOT}/Lawn/System/ReanimationLawn.cpp
    ${ROOT}/Lawn/System/SaveGame.cpp
    ${ROOT}/Lawn/System/TypingCheck.cpp

    # Lawn – UI widgets
    ${ROOT}/Lawn/Widget/LawnDialog.cpp
    ${ROOT}/Lawn/Widget/AlmanacDialog.cpp
    ${ROOT}/Lawn/Widget/AwardScreen.cpp
    ${ROOT}/Lawn/Widget/ChallengeScreen.cpp
    ${ROOT}/Lawn/Widget/ContinueDialog.cpp
    ${ROOT}/Lawn/Widget/CreditScreen.cpp
    ${ROOT}/Lawn/Widget/GameButton.cpp
    ${ROOT}/Lawn/Widget/HotkeyHelpDialog.cpp
    ${ROOT}/Lawn/Widget/GameSelector.cpp
    ${ROOT}/Lawn/Widget/ImitaterDialog.cpp
    ${ROOT}/Lawn/Widget/NewOptionsDialog.cpp
    ${ROOT}/Lawn/Widget/NewUserDialog.cpp
    ${ROOT}/Lawn/Widget/SeedChooserScreen.cpp
    ${ROOT}/Lawn/Widget/StoreScreen.cpp
    ${ROOT}/Lawn/Widget/TitleScreen.cpp
    ${ROOT}/Lawn/Widget/UserDialog.cpp
)

# ---------------------------------------------------------------------------
# SexyAppFramework sources
# ---------------------------------------------------------------------------
set(SEXYAPP_SOURCES
    ${ROOT}/SexyAppFramework/BassLoader.cpp
    ${ROOT}/SexyAppFramework/BassMusicInterface.cpp
    ${ROOT}/SexyAppFramework/Buffer.cpp
    ${ROOT}/SexyAppFramework/ButtonWidget.cpp
    ${ROOT}/SexyAppFramework/Checkbox.cpp
    ${ROOT}/SexyAppFramework/Color.cpp
    ${ROOT}/SexyAppFramework/Common.cpp
    ${ROOT}/SexyAppFramework/CritSect.cpp
    ${ROOT}/SexyAppFramework/CursorWidget.cpp
    ${ROOT}/SexyAppFramework/D3D8Helper.cpp
    ${ROOT}/SexyAppFramework/D3DInterface.cpp
    ${ROOT}/SexyAppFramework/D3DTester.cpp
    ${ROOT}/SexyAppFramework/DDImage.cpp
    ${ROOT}/SexyAppFramework/DDInterface.cpp
    ${ROOT}/SexyAppFramework/DSoundInstance.cpp
    ${ROOT}/SexyAppFramework/DSoundManager.cpp
    ${ROOT}/SexyAppFramework/Debug.cpp
    ${ROOT}/SexyAppFramework/DescParser.cpp
    ${ROOT}/SexyAppFramework/Dialog.cpp
    ${ROOT}/SexyAppFramework/DialogButton.cpp
    ${ROOT}/SexyAppFramework/DirectXErrorString.cpp
    ${ROOT}/SexyAppFramework/EditWidget.cpp
    ${ROOT}/SexyAppFramework/FModLoader.cpp
    ${ROOT}/SexyAppFramework/FModMusicInterface.cpp
    ${ROOT}/SexyAppFramework/FModSoundInstance.cpp
    ${ROOT}/SexyAppFramework/FModSoundManager.cpp
    ${ROOT}/SexyAppFramework/Flags.cpp
    ${ROOT}/SexyAppFramework/Font.cpp
    ${ROOT}/SexyAppFramework/Graphics.cpp
    ${ROOT}/SexyAppFramework/HTTPTransfer.cpp
    ${ROOT}/SexyAppFramework/HyperlinkWidget.cpp
    ${ROOT}/SexyAppFramework/Image.cpp
    ${ROOT}/SexyAppFramework/ImageFont.cpp
    ${ROOT}/SexyAppFramework/Insets.cpp
    ${ROOT}/SexyAppFramework/KeyCodes.cpp
    ${ROOT}/SexyAppFramework/ListWidget.cpp
    ${ROOT}/SexyAppFramework/MTRand.cpp
    ${ROOT}/SexyAppFramework/MemoryImage.cpp
    ${ROOT}/SexyAppFramework/ModVal.cpp
    ${ROOT}/SexyAppFramework/MusicInterface.cpp
    ${ROOT}/SexyAppFramework/NativeDisplay.cpp
    ${ROOT}/SexyAppFramework/PerfTimer.cpp
    ${ROOT}/SexyAppFramework/PropertiesParser.cpp
    ${ROOT}/SexyAppFramework/Quantize.cpp
    ${ROOT}/SexyAppFramework/Ratio.cpp
    ${ROOT}/SexyAppFramework/ResourceManager.cpp
    ${ROOT}/SexyAppFramework/SEHCatcher.cpp
    ${ROOT}/SexyAppFramework/SWTri.cpp
    ${ROOT}/SexyAppFramework/ScrollbarWidget.cpp
    ${ROOT}/SexyAppFramework/ScrollbuttonWidget.cpp
    ${ROOT}/SexyAppFramework/SDL2RendererWrapper.cpp
    ${ROOT}/SexyAppFramework/SexyApp.cpp
    ${ROOT}/SexyAppFramework/SexyAppBase.cpp
    ${ROOT}/SexyAppFramework/SexyMatrix.cpp
    ${ROOT}/SexyAppFramework/SharedImage.cpp
    ${ROOT}/SexyAppFramework/Slider.cpp
    ${ROOT}/SexyAppFramework/SysFont.cpp
    ${ROOT}/SexyAppFramework/TextWidget.cpp
    ${ROOT}/SexyAppFramework/Widget.cpp
    ${ROOT}/SexyAppFramework/WidgetContainer.cpp
    ${ROOT}/SexyAppFramework/WidgetManager.cpp
    ${ROOT}/SexyAppFramework/XMLParser.cpp

    # Ogg Vorbis (Tremor) decoder
    ${ROOT}/SexyAppFramework/ogg/bitwise.c
    ${ROOT}/SexyAppFramework/ogg/block.c
    ${ROOT}/SexyAppFramework/ogg/codebook.c
    ${ROOT}/SexyAppFramework/ogg/floor0.c
    ${ROOT}/SexyAppFramework/ogg/floor1.c
    ${ROOT}/SexyAppFramework/ogg/framing.c
    ${ROOT}/SexyAppFramework/ogg/info.c
    ${ROOT}/SexyAppFramework/ogg/mapping0.c
    ${ROOT}/SexyAppFramework/ogg/mdct.c
    ${ROOT}/SexyAppFramework/ogg/registry.c
    ${ROOT}/SexyAppFramework/ogg/res012.c
    ${ROOT}/SexyAppFramework/ogg/sharedbook.c
    ${ROOT}/SexyAppFramework/ogg/synthesis.c
    ${ROOT}/SexyAppFramework/ogg/vorbisfile.c
    ${ROOT}/SexyAppFramework/ogg/window.c
)

# ---------------------------------------------------------------------------
# Sexy.TodLib sources
# ---------------------------------------------------------------------------
set(TODLIB_SOURCES
    ${ROOT}/Sexy.TodLib/Attachment.cpp
    ${ROOT}/Sexy.TodLib/Definition.cpp
    ${ROOT}/Sexy.TodLib/EffectSystem.cpp
    ${ROOT}/Sexy.TodLib/FilterEffect.cpp
    ${ROOT}/Sexy.TodLib/ReanimAtlas.cpp
    ${ROOT}/Sexy.TodLib/Reanimator.cpp
    ${ROOT}/Sexy.TodLib/TodCommon.cpp
    ${ROOT}/Sexy.TodLib/TodDebug.cpp
    ${ROOT}/Sexy.TodLib/TodFoley.cpp
    ${ROOT}/Sexy.TodLib/TodList.cpp
    ${ROOT}/Sexy.TodLib/TodParticle.cpp
    ${ROOT}/Sexy.TodLib/TodStringFile.cpp
    ${ROOT}/Sexy.TodLib/Trail.cpp
)

# ---------------------------------------------------------------------------
# PakLib sources
# ---------------------------------------------------------------------------
set(PAKLIB_SOURCES
    ${ROOT}/PakLib/PakInterface.cpp
)

# ---------------------------------------------------------------------------
# ImageLib sources
# Note: excludes test/utility files (example.c, pngtest.c, minigzip.c, etc.)
# ---------------------------------------------------------------------------
set(IMAGELIB_SOURCES
    ${ROOT}/ImageLib/ImageLib.cpp

    # zlib
    ${ROOT}/ImageLib/zlib/adler32.c
    ${ROOT}/ImageLib/zlib/compress.c
    ${ROOT}/ImageLib/zlib/crc32.c
    ${ROOT}/ImageLib/zlib/deflate.c
    ${ROOT}/ImageLib/zlib/gzio.c
    ${ROOT}/ImageLib/zlib/infblock.c
    ${ROOT}/ImageLib/zlib/infcodes.c
    ${ROOT}/ImageLib/zlib/inffast.c
    ${ROOT}/ImageLib/zlib/inflate.c
    ${ROOT}/ImageLib/zlib/inftrees.c
    ${ROOT}/ImageLib/zlib/infutil.c
    ${ROOT}/ImageLib/zlib/trees.c
    ${ROOT}/ImageLib/zlib/uncompr.c
    ${ROOT}/ImageLib/zlib/zutil.c

    # libpng (exclude pngtest.c, pngvcrd.c — asm/test only)
    ${ROOT}/ImageLib/png/png.c
    ${ROOT}/ImageLib/png/pngerror.c
    ${ROOT}/ImageLib/png/pngget.c
    ${ROOT}/ImageLib/png/pngmem.c
    ${ROOT}/ImageLib/png/pngpread.c
    ${ROOT}/ImageLib/png/pngread.c
    ${ROOT}/ImageLib/png/pngrio.c
    ${ROOT}/ImageLib/png/pngrtran.c
    ${ROOT}/ImageLib/png/pngrutil.c
    ${ROOT}/ImageLib/png/pngset.c
    ${ROOT}/ImageLib/png/pngtrans.c
    ${ROOT}/ImageLib/png/pngwio.c
    ${ROOT}/ImageLib/png/pngwrite.c
    ${ROOT}/ImageLib/png/pngwtran.c
    ${ROOT}/ImageLib/png/pngwutil.c

    # libjpeg (exclude cdjpeg.c, jpegtran.c, rdjpgcom.c, wrjpgcom.c — CLI tools)
    ${ROOT}/ImageLib/jpeg/jcapimin.c
    ${ROOT}/ImageLib/jpeg/jcapistd.c
    ${ROOT}/ImageLib/jpeg/jccoefct.c
    ${ROOT}/ImageLib/jpeg/jccolor.c
    ${ROOT}/ImageLib/jpeg/jcdctmgr.c
    ${ROOT}/ImageLib/jpeg/jchuff.c
    ${ROOT}/ImageLib/jpeg/jcinit.c
    ${ROOT}/ImageLib/jpeg/jcmainct.c
    ${ROOT}/ImageLib/jpeg/jcmarker.c
    ${ROOT}/ImageLib/jpeg/jcmaster.c
    ${ROOT}/ImageLib/jpeg/jcomapi.c
    ${ROOT}/ImageLib/jpeg/jcparam.c
    ${ROOT}/ImageLib/jpeg/jcphuff.c
    ${ROOT}/ImageLib/jpeg/jcprepct.c
    ${ROOT}/ImageLib/jpeg/jcsample.c
    ${ROOT}/ImageLib/jpeg/jctrans.c
    ${ROOT}/ImageLib/jpeg/jdapimin.c
    ${ROOT}/ImageLib/jpeg/jdapistd.c
    ${ROOT}/ImageLib/jpeg/jdatadst.c
    ${ROOT}/ImageLib/jpeg/jdatasrc.c
    ${ROOT}/ImageLib/jpeg/jdcoefct.c
    ${ROOT}/ImageLib/jpeg/jdcolor.c
    ${ROOT}/ImageLib/jpeg/jddctmgr.c
    ${ROOT}/ImageLib/jpeg/jdhuff.c
    ${ROOT}/ImageLib/jpeg/jdinput.c
    ${ROOT}/ImageLib/jpeg/jdmainct.c
    ${ROOT}/ImageLib/jpeg/jdmarker.c
    ${ROOT}/ImageLib/jpeg/jdmaster.c
    ${ROOT}/ImageLib/jpeg/jdmerge.c
    ${ROOT}/ImageLib/jpeg/jdphuff.c
    ${ROOT}/ImageLib/jpeg/jdpostct.c
    ${ROOT}/ImageLib/jpeg/jdsample.c
    ${ROOT}/ImageLib/jpeg/jdtrans.c
    ${ROOT}/ImageLib/jpeg/jerror.c
    ${ROOT}/ImageLib/jpeg/jfdctflt.c
    ${ROOT}/ImageLib/jpeg/jfdctfst.c
    ${ROOT}/ImageLib/jpeg/jfdctint.c
    ${ROOT}/ImageLib/jpeg/jidctflt.c
    ${ROOT}/ImageLib/jpeg/jidctfst.c
    ${ROOT}/ImageLib/jpeg/jidctint.c
    ${ROOT}/ImageLib/jpeg/jidctred.c
    ${ROOT}/ImageLib/jpeg/jmemmgr.c
    ${ROOT}/ImageLib/jpeg/jmemnobs.c
    ${ROOT}/ImageLib/jpeg/jquant1.c
    ${ROOT}/ImageLib/jpeg/jquant2.c
    ${ROOT}/ImageLib/jpeg/jutils.c

    # JasPer JPEG2000 (replaces prebuilt j2k-codec.lib)
    ${ROOT}/ImageLib/jpeg2000/jas_cm.c
    ${ROOT}/ImageLib/jpeg2000/jas_debug.c
    ${ROOT}/ImageLib/jpeg2000/jas_getopt.c
    ${ROOT}/ImageLib/jpeg2000/jas_icc.c
    ${ROOT}/ImageLib/jpeg2000/jas_iccdata.c
    ${ROOT}/ImageLib/jpeg2000/jas_image.c
    ${ROOT}/ImageLib/jpeg2000/jas_init.c
    ${ROOT}/ImageLib/jpeg2000/jas_malloc.c
    ${ROOT}/ImageLib/jpeg2000/jas_seq.c
    ${ROOT}/ImageLib/jpeg2000/jas_stream.c
    ${ROOT}/ImageLib/jpeg2000/jas_string.c
    ${ROOT}/ImageLib/jpeg2000/jas_tvp.c
    ${ROOT}/ImageLib/jpeg2000/jas_version.c
    ${ROOT}/ImageLib/jpeg2000/jp2_cod.c
    ${ROOT}/ImageLib/jpeg2000/jp2_dec.c
    ${ROOT}/ImageLib/jpeg2000/jp2_enc.c
    ${ROOT}/ImageLib/jpeg2000/jpc_bs.c
    ${ROOT}/ImageLib/jpeg2000/jpc_cs.c
    ${ROOT}/ImageLib/jpeg2000/jpc_dec.c
    ${ROOT}/ImageLib/jpeg2000/jpc_enc.c
    ${ROOT}/ImageLib/jpeg2000/jpc_math.c
    ${ROOT}/ImageLib/jpeg2000/jpc_mct.c
    ${ROOT}/ImageLib/jpeg2000/jpc_mqcod.c
    ${ROOT}/ImageLib/jpeg2000/jpc_mqdec.c
    ${ROOT}/ImageLib/jpeg2000/jpc_mqenc.c
    ${ROOT}/ImageLib/jpeg2000/jpc_qmfb.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t1cod.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t1dec.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t1enc.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t2cod.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t2dec.c
    ${ROOT}/ImageLib/jpeg2000/jpc_t2enc.c
    ${ROOT}/ImageLib/jpeg2000/jpc_tagtree.c
    ${ROOT}/ImageLib/jpeg2000/jpc_tsfb.c
    ${ROOT}/ImageLib/jpeg2000/jpc_util.c
)

# ---------------------------------------------------------------------------
# Create the executable
# ---------------------------------------------------------------------------
if(WIN32)
# ---------------------------------------------------------------------------
# Create the executable
# ---------------------------------------------------------------------------
add_executable(PvZ-x64 WIN32
    ${GAME_SOURCES}
    ${SEXYAPP_SOURCES}
    ${TODLIB_SOURCES}
    ${PAKLIB_SOURCES}
    ${IMAGELIB_SOURCES}
    ${ROOT}/PlantsVsZombies.rc
)

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(PvZ-x64 PRIVATE
    ${ROOT}/compat                   # MUST be first: case-sensitivity shims (Windows.h → windows.h)
    ${ROOT}                          # project root (for LawnApp.h, Resources.h, etc.)
    ${ROOT}/sdl2/x86_64-w64-mingw32/include # local SDL2 include headers
    ${ROOT}/dx8sdk/include           # DirectX 8 SDK headers (d3d8.h, ddraw.h, dsound.h)
    ${ROOT}/SexyAppFramework          # framework headers
    ${ROOT}/Sexy.TodLib
    ${ROOT}/ImageLib
    ${ROOT}/ImageLib/jpeg
    ${ROOT}/ImageLib/jpeg2000
    ${ROOT}/ImageLib/png
    ${ROOT}/ImageLib/zlib
    ${ROOT}/PakLib
)

# ---------------------------------------------------------------------------
# Compile definitions
# ---------------------------------------------------------------------------
target_compile_definitions(PvZ-x64 PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _VISUALC_
    _JPEGLIB_
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:_CRT_SECURE_NO_WARNINGS>
    $<$<CONFIG:Release>:NDEBUG>
)

# ---------------------------------------------------------------------------
# Compile flags (MinGW)
# ---------------------------------------------------------------------------
target_compile_options(PvZ-x64 PRIVATE
    -mwindows                   # Windows subsystem
    -w                          # suppress all warnings during initial build
    # Force-include compat header FIRST: pre-pulls common headers then redefines
    # min/max as type-permissive macros (matching MSVC Windows.h behavior)
    -include ${ROOT}/compat/pvz_compat.h
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>   # relax for decompiled code
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Debug>:-g -O0>
)

# ---------------------------------------------------------------------------
# Link flags
# ---------------------------------------------------------------------------
target_link_options(PvZ-x64 PRIVATE
    -mwindows
    -static-libgcc
    -static-libstdc++
)

# ---------------------------------------------------------------------------
# Windows system libraries (MinGW provides these automatically)
# ---------------------------------------------------------------------------
target_link_libraries(PvZ-x64 PRIVATE
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    winmm
    ws2_32
    # DirectX 8 — manually generated for 64-bit
    "${ROOT}/libd3d8.a"
    dsound
    ddraw
    dxguid
    dinput8
    # SDL2 local development libraries
    "${ROOT}/sdl2/x86_64-w64-mingw32/lib/libSDL2.dll.a"
    "${ROOT}/sdl2/x86_64-w64-mingw32/lib/libSDL2main.a"
)

# ---------------------------------------------------------------------------
# BASS audio import library
# MinGW cannot use the MSVC bass.lib directly.
# build.sh generates libbass.dll.a via dlltool before cmake runs.
# ---------------------------------------------------------------------------
set(BASS_IMPLIB "${ROOT}/libbass.dll.a")
if(EXISTS "${BASS_IMPLIB}")
    target_link_libraries(PvZ-x64 PRIVATE "${BASS_IMPLIB}")
else()
    message(WARNING
        "libbass.dll.a not found — run build.sh which auto-generates it.\n"
        "Or manually: x86_64-w64-mingw32-dlltool -D bass.dll -d bass.def -l libbass.dll.a")
endif()

# ---------------------------------------------------------------------------
# Post-build: copy bass.dll next to the exe
# ---------------------------------------------------------------------------
add_custom_command(TARGET PvZ-x64 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT}/bass.dll"
        "$<TARGET_FILE_DIR:PvZ-x64>/bass.dll"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT}/sdl2/x86_64-w64-mingw32/bin/SDL2.dll"
        "$<TARGET_FILE_DIR:PvZ-x64>/SDL2.dll"
    COMMENT "Copying required DLLs (bass.dll, SDL2.dll) to output directory"
)
else()
# ---------------------------------------------------------------------------
# Find SDL2 on Linux
# ---------------------------------------------------------------------------
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

# ---------------------------------------------------------------------------
# Create the Linux executable (no RC file)
# ---------------------------------------------------------------------------
add_executable(PvZ-Linux
    ${GAME_SOURCES}
    ${SEXYAPP_SOURCES}
    ${TODLIB_SOURCES}
    ${PAKLIB_SOURCES}
    ${IMAGELIB_SOURCES}
)

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
target_include_directories(PvZ-Linux PRIVATE
    ${ROOT}                          # project root
    ${ROOT}/SexyAppFramework          # framework headers
    ${ROOT}/Sexy.TodLib
    ${ROOT}/ImageLib
    ${ROOT}/ImageLib/jpeg
    ${ROOT}/ImageLib/jpeg2000
    ${ROOT}/ImageLib/png
    ${ROOT}/ImageLib/zlib
    ${ROOT}/PakLib
    ${SDL2_INCLUDE_DIRS}
)

# ---------------------------------------------------------------------------
# Compile definitions
# ---------------------------------------------------------------------------
target_compile_definitions(PvZ-Linux PRIVATE
    _LIB
    _VISUALC_
    _JPEGLIB_
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# ---------------------------------------------------------------------------
# Compile flags
# ---------------------------------------------------------------------------
target_compile_options(PvZ-Linux PRIVATE
    -w                          # suppress warnings
    -fpermissive                # relax for decompiled code
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Debug>:-g -O0>
)

# ---------------------------------------------------------------------------
# Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(PvZ-Linux PRIVATE
    ${SDL2_LIBRARIES}
    dl                          # for dlopen (BASS loader)
    pthread                     # standard threading
)
endif()
