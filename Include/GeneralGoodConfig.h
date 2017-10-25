#ifndef NATHANHAPPYCONFIG
#define NATHANHAPPYCONFIG
	
	#define ANDROIDPACKAGENAME "com.mylegguy.lua.manga"
	#define VITAAPPID "LUAMANGAS"
	
	#define PLAT_VITA 1
	#define PLAT_WINDOWS 2
	#define PLAT_3DS 3
	
	#define SUB_NONE 0
	#define SUB_ANDROID 1
	#define SUB_UNIX 2
	
	#define SND_NONE 0
	#define SND_SDL 1

	#define REND_UNDEFINED 0
	#define REND_SDL 1
	#define REND_VITA2D 2
	#define REND_SF2D 3
	
	#define TEXT_UNDEFINED 0
	#define TEXT_DEBUG 1
	#define TEXT_VITA2D 2
	#define TEXT_FONTCACHE 3	

	// Avalible presets
	#define PRE_NONE 0
	#define PRE_WINDOWS 1
	#define PRE_VITA 2
	#define PRE_3DS 3
	#define PRE_ANDROID 4
	
	//===============================
	// (Incomplete) Auto platform
	//===============================
	#if _WIN32
		#define PRESET PRE_WINDOWS
	#elif __unix__
		#define PRESET PRE_WINDOWS
		#define SUBPLATFORM SUB_UNIX
		//#error no
	#endif

	//===============================
	// CHANGE THIS CODE TO CONFIGURE
	//===============================
	#ifndef PRESET
		#define PRESET PRE_VITA
	#endif
	#define USEUMA0 1
	#define DOFIXCOORDS 0
	#define USEVSYNC 0

	#if PRESET == PRE_WINDOWS
		#define PLATFORM PLAT_WINDOWS
		#define SOUNDPLAYER SND_NONE
		#define RENDERER REND_SDL
		#define TEXTRENDERER TEXT_FONTCACHE
	#elif PRESET == PRE_VITA
		#define PLATFORM PLAT_VITA
		#define SOUNDPLAYER SND_NONE
		#define RENDERER REND_VITA2D
		#define TEXTRENDERER TEXT_VITA2D
	#elif PRESET == PRE_3DS
		#define PLATFORM PLAT_3DS
		#define SOUNDPLAYER SND_NONE
		#define RENDERER REND_SF2D
		#define TEXTRENDERER TEXT_DEBUG
	#elif PRESET == PRE_ANDROID
		#define PLATFORM PLAT_WINDOWS
		#define SOUNDPLAYER SND_SDL
		#define RENDERER REND_SDL
		#define TEXTRENDERER TEXT_FONTCACHE
	#else
		// Put custom stuff here
		// #define PLATFORM a
		// #define SUBPLATFORM a
		// #define SOUNDPLAYER a
		// #define RENDERER a
		// #define TEXTRENDERER a
	#endif

	#ifndef SUBPLATFORM
		#define SUBPLATFORM SUB_NONE
	#endif

#endif