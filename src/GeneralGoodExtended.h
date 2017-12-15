#ifndef GENERALGOODSTUFFEXTENDED
#define GENERALGOODSTUFFEXTENDED
	#define ISUSINGEXTENDED 1
	
	char tempPathFixBuffer[256];
	char* DATAFOLDER=NULL;
	
	// For FixPath argument
	#define TYPE_UNDEFINED 0
	#define TYPE_DATA 1
	#define TYPE_EMBEDDED 2

	//#if DOFIXCOORDS == 1
	//	int FixX(int x);
	//	int FixY(int y);
	//#endif

	//#if DOFIXCOORDS == 1
	//	void FixCoords(int* _x, int* _y){
	//		*_x = FixX(*_x);
	//		*_y = FixY(*_y);
	//	}
	//	#define EASYFIXCOORDS(x, y) FixCoords(x,y)
	//#else
	//	#define EASYFIXCOORDS(x,y)
	//#endif

	unsigned char isSkipping=0;
	signed char InputValidity = 1;

	int screenHeight = 544;
	int screenWidth = 960;

	#include <stdio.h>
	#include <math.h>
	#include <string.h>
	#include <stdlib.h>
	#include <unistd.h>

	int SCE_TOUCH = 19;
	int SCE_ANDROID_BACK = 20;
	int touchX=-1;
	int touchY=-1;

	// Platform stuff
	#if PLATFORM == PLAT_VITA
		#include <psp2/ctrl.h>
		#include <psp2/kernel/processmgr.h>
		#include <psp2/rtc.h>
		#include <psp2/types.h>
		#include <psp2/display.h>
		#include <psp2/touch.h>
		#include <psp2/io/fcntl.h>
		#include <psp2/io/dirent.h>
		#include <psp2/power.h>
		// Controls at start of frame.
		SceCtrlData pad;
		// Controls from start of last frame.
		SceCtrlData lastPad;
	#endif
	#if PLATFORM == PLAT_COMPUTER
		// Header for directory functions
		#include <dirent.h>
	#endif

	#if PLATFORM == PLAT_3DS
		#include <3ds/types.h>
		signed int cacheIsCiaBuild=-1;
		char getIsCiaBuild();
	#endif

	#if PLATFORM != PLAT_VITA
		enum SceCtrlPadButtons {
			SCE_CTRL_SELECT      = 0,	//!< Select button.
			SCE_CTRL_L3          = 1,	//!< L3 button.
			SCE_CTRL_R3          = 2,	//!< R3 button.
			SCE_CTRL_START       = 3,	//!< Start button.
			SCE_CTRL_UP          = 4,	//!< Up D-Pad button.
			SCE_CTRL_RIGHT       = 5,	//!< Right D-Pad button.
			SCE_CTRL_DOWN        = 6,	//!< Down D-Pad button.
			SCE_CTRL_LEFT        = 7,	//!< Left D-Pad button.
			SCE_CTRL_LTRIGGER    = 8,	//!< Left trigger.
			SCE_CTRL_RTRIGGER    = 9,	//!< Right trigger.
			SCE_CTRL_L1          = 10,	//!< L1 button.
			SCE_CTRL_R1          = 11,	//!< R1 button.
			SCE_CTRL_TRIANGLE    = 12,	//!< Triangle button.
			SCE_CTRL_CIRCLE      = 13,	//!< Circle button.
			SCE_CTRL_CROSS       = 14,	//!< Cross button.
			SCE_CTRL_SQUARE      = 15,	//!< Square button.
			SCE_CTRL_INTERCEPTED = 16,  //!< Input not available because intercepted by another application
			SCE_CTRL_VOLUP       = 17,	//!< Volume up button.
			SCE_CTRL_VOLDOWN     = 18	//!< Volume down button.
			//int SCE_TOUCH = 19;
		};
	#endif
	#if PLATFORM == PLAT_3DS
		u32 pad;
		u32 lastPad;
	#endif

	// Subplatform Stuff
	#if SUBPLATFORM == SUB_ANDROID
		// For mkdir
		#include <sys/stat.h>
		// So we can see console output with adb logcat
		#define printf SDL_Log

		//#include <android/asset_manager.h>
		//#include <android/asset_manager_jni.h>
	#endif

	// Renderer stuff
	#if RENDERER == REND_SDL && PLATFORM != PLAT_VITA
		// Stores control data
		char pad[21]={0};
		char lastPad[21]={0};
	#endif

	//////////////////////////////////////////////////////////
	// Need dis
	#include "GeneralGood.h"
	//////////////////////////////////////////////////////////

	// itoa replacement for android which only supports base 10
	#if SUBPLATFORM == SUB_ANDROID
		void itoa(int _num, char* _buffer, int _uselessBase){
			sprintf(_buffer, "%d", _num);
		}
	#endif

	signed char wasJustReleased(int value){
		if (InputValidity==1 || isSkipping==1){
			#if PLATFORM == PLAT_VITA
				if (lastPad.buttons & value && !(pad.buttons & value)){
					return 1;
				}
			#elif PLATFORM == PLAT_COMPUTER
				if (lastPad[value]==1 && pad[value]==0){
					return 1;
				}
			#endif
		}
		return 0;
	}

	signed char wasJustPressedRegardless(int value){
		
		#if PLATFORM == PLAT_VITA
			if (pad.buttons & value && !(lastPad.buttons & value)){
				return 1;
			}
		#elif PLATFORM == PLAT_COMPUTER
			if (pad[value]==1 && lastPad[value]==0){
				return 1;
			}
		#elif PLATFORM==PLAT_3DS
			if (lastPad & value){
				return 1;
			}
		#endif
		
		return 0;
	}

	signed char wasJustPressed(int value){
		if (InputValidity==1 || isSkipping==1){
			#if PLATFORM == PLAT_VITA
				if (pad.buttons & value && !(lastPad.buttons & value)){
					return 1;
				}
			#elif PLATFORM == PLAT_COMPUTER
				if (pad[value]==1 && lastPad[value]==0){
					return 1;
				}
			#elif PLATFORM==PLAT_3DS
				if (lastPad & value){
					return 1;
				}
			#endif
		}
		return 0;
	}

	void controlsStart(){
		#if PLATFORM == PLAT_VITA
			sceCtrlPeekBufferPositive(0, &pad, 1);
			//sceTouchPeek(SCE_TOUCH_PORT_FRONT, &currentTouch, 1);
		#elif RENDERER == REND_SDL
			SDL_Event e;
			while( SDL_PollEvent( &e ) != 0 ){
				if( e.type == SDL_QUIT ){
					//XOutFunction();
				}
				#if PLATFORM == PLAT_COMPUTER
					if( e.type == SDL_KEYDOWN ){
						if (e.key.keysym.sym==SDLK_z){ /* X */
							pad[SCE_CTRL_CROSS]=1;
						}else if (e.key.keysym.sym==SDLK_x){/* O */
							pad[SCE_CTRL_CIRCLE]=1;
						}else if (e.key.keysym.sym==SDLK_LEFT){/* Left */
							pad[SCE_CTRL_LEFT]=1;
						}else if (e.key.keysym.sym==SDLK_RIGHT){ /* Right */
							pad[SCE_CTRL_RIGHT]=1;
						}else if (e.key.keysym.sym==SDLK_DOWN){ /* Down */
							pad[SCE_CTRL_DOWN]=1;
						}else if (e.key.keysym.sym==SDLK_UP){ /* Up */
							pad[SCE_CTRL_UP]=1;
						}else if (e.key.keysym.sym==SDLK_a){ /* Square */
							pad[SCE_CTRL_SQUARE]=1;
						}else if (e.key.keysym.sym==SDLK_s){ /* Triangle */
							pad[SCE_CTRL_TRIANGLE]=1;
						}else if (e.key.keysym.sym==SDLK_ESCAPE || e.key.keysym.sym==SDLK_RETURN){ /* Start */
							pad[SCE_CTRL_START]=1;
						}else if (e.key.keysym.sym==SDLK_e){ /* Select */
							pad[SCE_CTRL_SELECT]=1;
						}else if (e.key.keysym.sym==SDLK_b || e.key.keysym.sym==SDLK_AC_BACK){ /* Back button on android */
							pad[SCE_ANDROID_BACK]=1;
						}
					}else if (e.type == SDL_KEYUP){
						if (e.key.keysym.sym==SDLK_z){ /* X */
							pad[SCE_CTRL_CROSS]=0;
						}else if (e.key.keysym.sym==SDLK_x){/* O */
							pad[SCE_CTRL_CIRCLE]=0;
						}else if (e.key.keysym.sym==SDLK_LEFT){/* Left */
							pad[SCE_CTRL_LEFT]=0;
						}else if (e.key.keysym.sym==SDLK_RIGHT){ /* Right */
							pad[SCE_CTRL_RIGHT]=0;
						}else if (e.key.keysym.sym==SDLK_DOWN){ /* Down */
							pad[SCE_CTRL_DOWN]=0;
						}else if (e.key.keysym.sym==SDLK_UP){ /* Up */
							pad[SCE_CTRL_UP]=0;
						}else if (e.key.keysym.sym==SDLK_a){ /* Square */
							pad[SCE_CTRL_SQUARE]=0;
						}else if (e.key.keysym.sym==SDLK_s){ /* Triangle */
							pad[SCE_CTRL_TRIANGLE]=0;
						}else if (e.key.keysym.sym==SDLK_ESCAPE || e.key.keysym.sym==SDLK_RETURN){ /* Start */
							pad[SCE_CTRL_START]=0;
						}else if (e.key.keysym.sym==SDLK_e){ /* Select */
							pad[SCE_CTRL_SELECT]=0;
						}else if (e.key.keysym.sym==SDLK_b || e.key.keysym.sym==SDLK_AC_BACK){ /* Back button on android */
							pad[SCE_ANDROID_BACK]=0;
						}
					}
				#endif
				
				#if PLATFORM == PLAT_COMPUTER
					if( e.type == SDL_FINGERDOWN || (pad[SCE_TOUCH]==1 && e.type == SDL_FINGERMOTION)){
						touchX = e.tfinger.x * screenWidth;
						touchY = e.tfinger.y * screenHeight;
						pad[SCE_TOUCH]=1;
					}else if (e.type == SDL_MOUSEBUTTONDOWN || (pad[SCE_TOUCH]==1 && e.type == SDL_MOUSEMOTION) ){
						SDL_GetMouseState(&touchX,&touchY);
						pad[SCE_TOUCH] = 1;
					}
					if (e.type == SDL_FINGERUP){
						pad[SCE_TOUCH] = 0;
					}else if (e.type == SDL_MOUSEBUTTONUP){
						pad[SCE_TOUCH] = 0;
					}
				#endif

			}
		#elif PLATFORM == PLAT_3DS
			hidScanInput();
			lastPad = hidKeysDown();
			pad = hidKeysHeld();
		#endif
	}

	void controlsEnd(){
		#if PLATFORM == PLAT_VITA
			lastPad=pad;
		#elif PLATFORM == PLAT_COMPUTER
			memcpy(lastPad,pad,sizeof(pad));
		#elif PLATFORM == PLAT_3DS
			// I guess I don't need this.
		#endif
	}

	#define controlsReset() \
		controlsStart(); \
		controlsEnd();

	void controlsResetEmpty(){
		#if PLATFORM != PLAT_VITA
			memset(&pad,0,sizeof(pad));
			memset(&lastPad,0,sizeof(lastPad));
		#elif PLATFORM == PLAT_VITA
			memset(&pad.buttons,0,sizeof(pad.buttons));
			memset(&lastPad.buttons,0,sizeof(pad.buttons));
		#endif
	}

	signed char isDown(int value){
		if (InputValidity==1 || isSkipping==1){
			#if PLATFORM == PLAT_VITA
				if (pad.buttons & value){
					return 1;
				}
			#elif PLATFORM == PLAT_COMPUTER
		
				if (pad[value]==1){
					return 1;
				}
			#elif PLATFORM == PLAT_3DS
				if (pad & value){
					return 1;
				}
			#endif
		}
		return 0;
	}

	// Passed string should be freed already
	void generateDefaultDataDirectory(char** _dataDirPointer, char _useUma0){
		#if SUBPLATFORM == SUB_ANDROID
			*_dataDirPointer = malloc(strlen("/data/data//")+strlen(ANDROIDPACKAGENAME)+1);
			strcpy(*_dataDirPointer,"/data/data/");
			strcat(*_dataDirPointer,ANDROIDPACKAGENAME);
			strcat(*_dataDirPointer,"/");
		#elif PLATFORM == PLAT_COMPUTER
			*_dataDirPointer = malloc(strlen("./")+1);
			strcpy(*_dataDirPointer,"./");
		#elif PLATFORM == PLAT_VITA
			if (_useUma0){
				*_dataDirPointer = malloc(strlen("uma0:data//")+strlen(VITAAPPID)+1);
				strcpy(*_dataDirPointer,"uma0:data/");
				strcat(*_dataDirPointer,VITAAPPID);
				strcat(*_dataDirPointer,"/");
			}else{
				*_dataDirPointer = malloc(strlen("ux0:data//")+strlen(VITAAPPID)+1);
				strcpy(*_dataDirPointer,"ux0:data/");
				strcat(*_dataDirPointer,VITAAPPID);
				strcat(*_dataDirPointer,"/");
			}
		#elif PLATFORM == PLAT_3DS
			*_dataDirPointer = malloc(strlen("/3ds/data/")+strlen(VITAAPPID)+2);
			strcpy(*_dataDirPointer,"/3ds/data/");
			strcat(*_dataDirPointer,VITAAPPID);
			strcat(*_dataDirPointer,"/");
		#endif
	}

	void fixPath(char* filename,char _buffer[], char type){
		if (DATAFOLDER==NULL){
			generateDefaultDataDirectory(&DATAFOLDER,USEUMA0);
		}
		if (type==TYPE_DATA){
			strcpy((char*)_buffer,DATAFOLDER);
		}else if (type==TYPE_EMBEDDED){
			#if SUBPLATFORM == SUB_ANDROID
				strcpy((char*)_buffer,"");
			#elif PLATFORM == PLAT_COMPUTER
				strcpy((char*)_buffer,"./");
			#elif PLATFORM == PLAT_VITA
				strcpy((char*)_buffer,"app0:");
			#elif PLATFORM == PLAT_3DS
				if (cacheIsCiaBuild==-1){
					cacheIsCiaBuild = getIsCiaBuild();
				}
				if (cacheIsCiaBuild){
					strcpy((char*)_buffer,"romfs:/");
				}else{
					strcpy((char*)_buffer,DATAFOLDER);
				}
			#endif
		}else{
			printf("Unknown type.\n");
		}
		strcat((char*)_buffer,filename);
	}
	void makeDataDirectory(){
		char tempPathFixBuffer[256];
		fixPath("",tempPathFixBuffer,TYPE_DATA);
		if (directoryExists((const char*)tempPathFixBuffer)==0){
			createDirectory((const char*)tempPathFixBuffer);
		}
	}
#endif
