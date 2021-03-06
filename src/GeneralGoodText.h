
#ifndef GENERALGOODTEXTHEADER
#define GENERALGOODTEXTHEADER
	// Text Stuff
	#if TEXTRENDERER == TEXT_FONTCACHE
		#include <SDL_FontCache.h>
		#define CrossFont FC_Font
	#elif TEXTRENDERER == TEXT_DEBUG
		#define CrossFont CrossTexture
	#elif TEXTRENDERER == TEXT_VITA2D
		#define CrossFont vita2d_font
	#elif TEXTRENDERER == TEXT_UNDEFINED
		#define CrossFont int
	#endif

	CrossFont* fontImage=NULL;

	#if TEXTRENDERER == TEXT_DEBUG
		float fontSize = 1;
	#endif
	#if TEXTRENDERER == TEXT_FONTCACHE
		//int fontSize = 20;
		int fontSize=50;
	#endif
	#if TEXTRENDERER == TEXT_VITA2D
		int fontSize=32;
	#endif
	#if TEXTRENDERER == TEXT_UNDEFINED
		int fontSize = 32;
	#endif
	#if TEXTRENDERER == TEXT_DEBUG
		typedef struct{
			int x;
			int y;
			int imageWidth;
			int imageHeight;
			int imageDisplayWidth;
		}BitmapFontLetter;
		
		BitmapFontLetter bitmapFontLetterInfo[95];
		unsigned short maxBitmapCharacterHeight = 12;

		void drawLetter(int letterId, int _x, int _y, float size){
			letterId-=32;
			drawTexturePartScale(fontImage,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size);
		}
		void drawLetterColorAlpha(int letterId, int _x, int _y, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
			letterId-=32;
			drawTexturePartScaleTintAlpha(fontImage,_x,_y+bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].x,bitmapFontLetterInfo[letterId].y,bitmapFontLetterInfo[letterId].imageWidth,bitmapFontLetterInfo[letterId].imageHeight,size,size,r,g,b,a);
		}
		void drawLetterColor(int letterId, int _x, int _y, float size, unsigned char r, unsigned char g, unsigned char b){
			drawLetterColorAlpha(letterId,_x,_y,size,r,g,b,255);
		}

	#endif

	void loadFont(char* filename){
		#if TEXTRENDERER == TEXT_DEBUG
			// Load bitmap font image
			char _specificFontImageBuffer[strlen(filename)+5+1]; // filepath + extention + null
			sprintf(_specificFontImageBuffer, "%s%s", filename, ".png");
			fontImage=loadPNG(_specificFontImageBuffer);
			
			// Load font info
			sprintf(_specificFontImageBuffer, "%s%s", filename, ".info");
			FILE* fp = fopen(_specificFontImageBuffer,"r");
			int i;
			for (i=0;i<95;i++){
				fscanf(fp,"%d %d %d %d %d\n",&(bitmapFontLetterInfo[i].x),&(bitmapFontLetterInfo[i].y),&(bitmapFontLetterInfo[i].imageWidth),&(bitmapFontLetterInfo[i].imageHeight),&(bitmapFontLetterInfo[i].imageDisplayWidth));
				if (bitmapFontLetterInfo[i].imageHeight>maxBitmapCharacterHeight){
					maxBitmapCharacterHeight=bitmapFontLetterInfo[i].imageHeight;
				}
			}
			fclose(fp);
		#elif TEXTRENDERER == TEXT_FONTCACHE
			//fontSize = (SCREENHEIGHT-TEXTBOXY)/3.5;
			FC_FreeFont(fontImage);
			fontImage = NULL;
			fontImage = FC_CreateFont();
			FC_LoadFont(fontImage, mainWindowRenderer, filename, fontSize, FC_MakeColor(0,0,0,255), TTF_STYLE_NORMAL);
		#elif TEXTRENDERER == TEXT_VITA2D
			if (fontImage!=NULL){
				vita2d_free_font(fontImage);
				fontImage=NULL;
			}
			fontImage = vita2d_load_font_file(filename);
		#elif TEXTRENDERER == TEXT_UNDEFINED
		#endif
	}

	int textHeight(float scale){
		#if TEXTRENDERER == TEXT_DEBUG
			return (maxBitmapCharacterHeight*scale);
		#elif TEXTRENDERER == TEXT_VITA2D
			return vita2d_font_text_height(fontImage,scale,"a");
		#elif TEXTRENDERER == TEXT_FONTCACHE
			return floor(FC_GetRealHeight(fontImage));
		#elif TEXTRENDERER == TEXT_UNDEFINED
			return fontSize;
		#endif
	}

	// Please always use the same font size
	int textWidth(float scale, const char* message){
		#if TEXTRENDERER == TEXT_DEBUG
			int _currentWidth=0;
			int i;
			for (i=0;i<strlen(message);i++){
				if (message[i]-32<95 && message[i]-32>=0){
					_currentWidth+=(bitmapFontLetterInfo[message[i]-32].imageDisplayWidth)*scale;
				}
			}
			return _currentWidth;
		#elif TEXTRENDERER == TEXT_VITA2D
			return vita2d_font_text_width(fontImage,scale,message);
		#elif TEXTRENDERER == TEXT_FONTCACHE
			return FC_GetWidth(fontImage,"%s",message);
		#elif TEXTRENDERER == TEXT_UNDEFINED
			return fontSize*strlen(message);
		#endif
	}
	void goodDrawTextColoredAlpha(int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b, unsigned char a){
		EASYFIXCOORDS(&x,&y);
		#if TEXTRENDERER == TEXT_VITA2D
			vita2d_font_draw_text(fontImage,x,y+textHeight(size), RGBA8(r,g,b,a),floor(size),text);
		#elif TEXTRENDERER == TEXT_DEBUG
			int i=0;
			int _currentDrawTextX=x;
			for (i = 0; i < strlen(text); i++){
				if (text[i]-32<95){
					drawLetterColorAlpha(text[i],_currentDrawTextX,y,size,r,g,b,a);
					_currentDrawTextX+=bitmapFontLetterInfo[text[i]-32].imageDisplayWidth*size;
				}
			}
		#elif TEXTRENDERER == TEXT_FONTCACHE
			SDL_Color _tempcolor;
			_tempcolor.r = r;
			_tempcolor.g = g;
			_tempcolor.b = b;
			_tempcolor.a = a;
			FC_DrawColor(fontImage, mainWindowRenderer, x, y, _tempcolor ,"%s", text);
		#endif
	}
	void goodDrawTextColored(int x, int y, const char* text, float size, unsigned char r, unsigned char g, unsigned char b){
		goodDrawTextColoredAlpha(x,y,text,size,r,g,b,255);
	}
	void goodDrawText(int x, int y, const char* text, float size){
		#if TEXTRENDERER == TEXT_VITA2D
			EASYFIXCOORDS(&x,&y);
			vita2d_font_draw_text(fontImage,x,y+textHeight(size), RGBA8(255,255,255,255),floor(size),text);
		#elif TEXTRENDERER == TEXT_DEBUG
			EASYFIXCOORDS(&x,&y);
			// TODO - Make this just call goodDrawTextColored
			int i=0;
			int _currentDrawTextX=x;
			for (i = 0; i < strlen(text); i++){
				if (text[i]-32<95){
					drawLetter(text[i],_currentDrawTextX,y,size);
					_currentDrawTextX+=bitmapFontLetterInfo[text[i]-32].imageDisplayWidth*size;
				}
			}
		#elif TEXTRENDERER == TEXT_FONTCACHE
			goodDrawTextColoredAlpha(x,y,text,size,255,255,255,255);
		#endif
	}

#endif
