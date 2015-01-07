/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/**********************************************************************
	UI_ATOMS.C

	User interface building blocks and support functions.
**********************************************************************/
#include "ui_local.h"

uiStatic_t		uis;

void QDECL Com_Error( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	Q_vsnprintf (text, sizeof(text), error, argptr);
	va_end (argptr);

	trap_Error( text );
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

	trap_Print( text );
}

/*
=================
UI_ClampCvar
=================
*/
float UI_ClampCvar( float min, float max, float value )
{
	if ( value < min ) return min;
	if ( value > max ) return max;
	return value;
}

/*
=================
UI_LerpColor
=================
*/
void UI_LerpColor(vec4_t a, vec4_t b, vec4_t c, float t)
{
	int i;

	// lerp and clamp each component
	for (i=0; i<4; i++)
	{
		c[i] = a[i] + t*(b[i]-a[i]);
		if (c[i] < 0)
			c[i] = 0;
		else if (c[i] > 1.0)
			c[i] = 1.0;
	}
}

int UI_StringWidth( const char* str ) {
	const char *	s;
	int		ch;
	int		charWidth;
	int		width;

	s = str;
	width = 0;
	while ( *s ) {
		ch = *s & 127;
		charWidth = 0;
		if ( charWidth != -1 ) {
			width += charWidth;
			width += 0; // gap width
		}
		s++;
	}

	width -= 0; // remove last gap width
	return width;
}

/*
=================
UI_DrawString
=================
*/
void UI_DrawString( int x, int y, const char* str, int style, vec4_t color )
{
	int		len;
	int		charw;
	int		charh;
	vec4_t		newcolor;
	vec4_t		lowlight;
	float		*drawcolor;
	vec4_t		dropcolor;

	if( !str ) {
		return;
	}

	if ((style & UI_BLINK) && ((uis.realtime/BLINK_DIVISOR) & 1))
		return;

	if (style & UI_PULSE)
	{
		lowlight[0] = 0.8*color[0]; 
		lowlight[1] = 0.8*color[1];
		lowlight[2] = 0.8*color[2];
		lowlight[3] = 0.8*color[3];
		UI_LerpColor(color,lowlight,newcolor,0.5+0.5*sin(uis.realtime/PULSE_DIVISOR));
		drawcolor = newcolor;
	}	
	else
		drawcolor = color;

	charw = 14;
	charh = 14;
	switch (style & UI_FORMATMASK)
	{
		case UI_CENTER:
			// center justify at x
			len = strlen(str);
			x   = x - len*charw/2;
			break;

		case UI_RIGHT:
			// right justify at x
			len = strlen(str);
			x   = x - len*charw;
			break;

		default:
			// left justify at x
			break;
	}

	if ( style & UI_DROPSHADOW )
	{
		dropcolor[0] = dropcolor[1] = dropcolor[2] = 0;
		dropcolor[3] = drawcolor[3];
		//UI_DrawString2(x+2,y+2,str,dropcolor,charw,charh);
	}

	Com_Printf( "DrawString called: %s\n", str );
	//UI_DrawString2(x,y,str,drawcolor,charw,charh);
}

#ifdef BUILD_FREETYPE
void UI_Text_PaintChar ( float x, float y, float width, float height, float scale, float s, float t, float s2, float t2, qhandle_t hShader ) {
	float w, h;

	w = width * scale;
	h = height * scale;
	UI_AdjustFrom640 ( &x, &y, &w, &h );

	trap_R_DrawStretchPic( x, y, w, h, s, t, s2, t2, hShader );
}

void UI_Text_Paint ( float x, float y, float scale, const vec4_t color, const char *text, float adjust, int limit, int style, const fontInfo_t *font ) {
	int len, count;
	vec4_t newColor;
	const glyphInfo_t *glyph;
	float useScale;

	useScale = scale * font->glyphScale;
	if ( text ) {
// TTimo: FIXME
//      const unsigned char *s = text;
		const char *s = text;

		UI_SetColor ( color );
		memcpy ( &newColor[ 0 ], &color[ 0 ], sizeof ( vec4_t ) );
		len = strlen ( text );
		if ( limit > 0 && len > limit ) {
			len = limit;
		}
		count = 0;
		while ( s && *s && count < len ) {
			glyph = &font->glyphs[ ( int ) *s ];

			if ( Q_IsColorString ( s ) ) {
				memcpy ( newColor, ( float * ) g_color_table[ ColorIndex ( *( s + 1 ) ) ], sizeof ( newColor ) );
				newColor[ 3 ] = color[ 3 ];
				UI_SetColor ( newColor );
				s += 2;
				continue;
			} else {
				float yadj = useScale * glyph->top;

				if ( style & UI_DROPSHADOW ) { // || style == ITEM_TEXTSTYLE_SHADOWEDMORE)
					int ofs = 1;               //style == ITEM_TEXTSTYLE_SHADOWED ? 1 : 2;

					colorBlack[ 3 ] = newColor[ 3 ];
					UI_SetColor ( colorBlack );
					UI_Text_PaintChar ( x + ofs, y - yadj + ofs,
					                     glyph->imageWidth,
					                     glyph->imageHeight, useScale, glyph->s, glyph->t, glyph->s2, glyph->t2, glyph->glyph );
					colorBlack[ 3 ] = 1.0;
					UI_SetColor ( newColor );
				}
				UI_Text_PaintChar ( x, y - yadj,
				                     glyph->imageWidth,
				                     glyph->imageHeight, useScale, glyph->s, glyph->t, glyph->s2, glyph->t2, glyph->glyph );

				x += ( glyph->xSkip * useScale ) + adjust;
				s++;
				count++;
			}
		}
		UI_SetColor ( NULL );
	}
}
#endif // BUILD_FREETYPE

qboolean UI_IsFullscreen( void ) {
	return qtrue;
}

void UI_ForceMenuOff (void)
{
	uis.activemenu = 0;

	trap_S_StopBackgroundTrack();
	trap_Key_SetCatcher( trap_Key_GetCatcher() & ~KEYCATCH_UI );
	trap_Key_ClearStates();
	trap_Cvar_Set( "cl_paused", "0" );
}

void UI_SetActiveMenu( uiMenuCommand_t menu ) {

	switch ( menu ) {
	case UIMENU_NONE:
		UI_ForceMenuOff();
		return;
	case UIMENU_MAIN:
		UI_MainMenu();
		return;
	case UIMENU_NEED_CD:
		Com_Error( ERR_FATAL, "Need CD Key menu called.\n" );
		return;
	case UIMENU_BAD_CD_KEY:
		Com_Error( ERR_FATAL, "Bad CD Key menu called.\n" );
		return;
	case UIMENU_INGAME:
		trap_Cvar_Set( "cl_paused", "1" );
		UI_InGameMenu();
		return;
		
	case UIMENU_TEAM:
	case UIMENU_POSTGAME:
	default:
#ifndef NDEBUG
	  Com_Printf("UI_SetActiveMenu: bad enum %d\n", menu );
#endif
	  break;
	}
}

/*
=================
UI_KeyEvent
=================
*/
void UI_KeyEvent( int key, int down ) {

	return;
}

/*
=================
UI_MouseEvent
=================
*/
void UI_MouseEvent( int dx, int dy )
{
	if (!uis.activemenu)
		return;

	// update mouse screen position
	uis.cursorx += dx;
	if (uis.cursorx < -uis.bias)
		uis.cursorx = -uis.bias;
	else if (uis.cursorx > SCREEN_WIDTH+uis.bias)
		uis.cursorx = SCREEN_WIDTH+uis.bias;

	uis.cursory += dy;
	if (uis.cursory < 0)
		uis.cursory = 0;
	else if (uis.cursory > SCREEN_HEIGHT)
		uis.cursory = SCREEN_HEIGHT;
}

char *UI_Argv( int arg ) {
	static char	buffer[MAX_STRING_CHARS];

	trap_Argv( arg, buffer, sizeof( buffer ) );

	return buffer;
}


char *UI_Cvar_VariableString( const char *var_name ) {
	static char	buffer[MAX_STRING_CHARS];

	trap_Cvar_VariableStringBuffer( var_name, buffer, sizeof( buffer ) );

	return buffer;
}


/*
=================
UI_Cache
=================
*/
void UI_Cache( void ) {

}


/*
=================
UI_ConsoleCommand
=================
*/
qboolean UI_ConsoleCommand( int realTime ) {
	char	*cmd;

	uis.frametime = realTime - uis.realtime;
	uis.realtime = realTime;

	cmd = UI_Argv( 0 );

	if ( Q_stricmp (cmd, "iamamonkey") == 0 ) {
		UI_Cache();
		return qtrue;
	}

	return qfalse;
}

/*
=================
UI_Shutdown
=================
*/
void UI_Shutdown( void ) {
}

/*
=================
UI_Init
=================
*/
void UI_Init( void ) {
	UI_RegisterCvars();

	// cache redundant calulations
	trap_GetGlconfig( &uis.glconfig );

	// for 640x480 virtualized screen
	uis.xscale = uis.glconfig.vidWidth * (1.0/640.0);
	uis.yscale = uis.glconfig.vidHeight * (1.0/480.0);
	if ( uis.glconfig.vidWidth * 480 > uis.glconfig.vidHeight * 640 ) {
		// wide screen
		uis.bias = 0.5 * ( uis.glconfig.vidWidth - ( uis.glconfig.vidHeight * (640.0/480.0) ) );
		uis.xscale = uis.yscale;
	}
	else {
		// no wide screen
		uis.bias = 0;
	}

	uis.activemenu = 0;

	uis.whiteShader = trap_R_RegisterShaderNoMip( "ColorBlock" );
	uis.cursor = trap_R_RegisterShaderNoMip( "mouse" );

	uis.back_a = trap_R_RegisterShaderNoMip( "sepiaload_a" );
	uis.back_b = trap_R_RegisterShaderNoMip( "sepiaload_b" );
}

/*
================
UI_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void UI_AdjustFrom640( float *x, float *y, float *w, float *h ) {
	// expect valid pointers
	*x = *x * uis.xscale + uis.bias;
	*y *= uis.yscale;
	*w *= uis.xscale;
	*h *= uis.yscale;
}

void UI_DrawNamedPic( float x, float y, float width, float height, const char *picname ) {
	qhandle_t	hShader;

	hShader = trap_R_RegisterShaderNoMip( picname );
	UI_AdjustFrom640( &x, &y, &width, &height );
	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}

void UI_DrawHandlePic( float x, float y, float w, float h, qhandle_t hShader ) {
	float	s0;
	float	s1;
	float	t0;
	float	t1;

	if( w < 0 ) {	// flip about vertical
		w  = -w;
		s0 = 1;
		s1 = 0;
	}
	else {
		s0 = 0;
		s1 = 1;
	}

	if( h < 0 ) {	// flip about horizontal
		h  = -h;
		t0 = 1;
		t1 = 0;
	}
	else {
		t0 = 0;
		t1 = 1;
	}
	
	UI_AdjustFrom640( &x, &y, &w, &h );
	trap_R_DrawStretchPic( x, y, w, h, s0, t0, s1, t1, hShader );
}

/*
================
UI_FillRect

Coordinates are 640*480 virtual values
=================
*/
void UI_FillRect( float x, float y, float width, float height, const float *color ) {
	trap_R_SetColor( color );

	UI_AdjustFrom640( &x, &y, &width, &height );
	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 0, 0, uis.whiteShader );

	trap_R_SetColor( NULL );
}

/*
================
UI_DrawRect

Coordinates are 640*480 virtual values
=================
*/
void UI_DrawRect( float x, float y, float width, float height, const float *color ) {
	trap_R_SetColor( color );

	UI_AdjustFrom640( &x, &y, &width, &height );

	trap_R_DrawStretchPic( x, y, width, 1, 0, 0, 0, 0, uis.whiteShader );
	trap_R_DrawStretchPic( x, y, 1, height, 0, 0, 0, 0, uis.whiteShader );
	trap_R_DrawStretchPic( x, y + height - 1, width, 1, 0, 0, 0, 0, uis.whiteShader );
	trap_R_DrawStretchPic( x + width - 1, y, 1, height, 0, 0, 0, 0, uis.whiteShader );

	trap_R_SetColor( NULL );
}

void UI_SetColor( const float *rgba ) {
	trap_R_SetColor( rgba );
}

void UI_UpdateScreen( void ) {
	trap_UpdateScreen();
}

/*
=================
UI_Refresh
=================
*/
void UI_Refresh( int realtime )
{

	uis.frametime = realtime - uis.realtime;
	uis.realtime  = realtime;

	if ( !( trap_Key_GetCatcher() & KEYCATCH_UI ) ) {
		return;
	}

	UI_UpdateCvars();

	if ( uis.activemenu )
	{
		// draw background image for now
		UI_DrawHandlePic( 0, 0, 512, 512, uis.back_a );
		UI_DrawHandlePic( 384, 0, 256, 512, uis.back_b );
	}

	// draw cursor
	UI_SetColor( NULL );
	UI_DrawHandlePic( uis.cursorx-16, uis.cursory-16, 32, 32, uis.cursor);
}

#define TEXTCHAR_WIDTH 14
#define TEXTCHAR_HEIGHT 14
void UI_DrawTextBox (int x, int y, int width, int lines)
{
	UI_FillRect( x + TEXTCHAR_WIDTH/2, y + TEXTCHAR_HEIGHT/2, ( width + 1 ) * TEXTCHAR_WIDTH, ( lines + 1 ) * TEXTCHAR_HEIGHT, colorBlack );
	UI_DrawRect( x + TEXTCHAR_WIDTH/2, y + TEXTCHAR_HEIGHT/2, ( width + 1 ) * TEXTCHAR_WIDTH, ( lines + 1 ) * TEXTCHAR_HEIGHT, colorWhite );
}

qboolean UI_CursorInRect (int x, int y, int width, int height)
{
	if (uis.cursorx < x ||
		uis.cursory < y ||
		uis.cursorx > x+width ||
		uis.cursory > y+height)
		return qfalse;

	return qtrue;
}
