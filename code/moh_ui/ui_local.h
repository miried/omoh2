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
#ifndef __UI_LOCAL_H__
#define __UI_LOCAL_H__

#include "../qcommon/q_shared.h"
#include "../renderercommon/tr_types.h"
//NOTE: include the ui_public.h from the new UI
#include "../ui/ui_public.h"
//redefine to old API version
#undef UI_API_VERSION
#define UI_API_VERSION	4
#include "../client/keycodes.h"
#include "../game/bg_public.h"

typedef void (*voidfunc_f)(void);


extern vmCvar_t	ui_wombat;

//
// ui_main.c
//
extern void UI_RegisterCvars( void );
extern void UI_UpdateCvars( void );

//
// ui_atoms.c
//
typedef struct {
	int			frametime;
	int			realtime;
	int			cursorx;
	int			cursory;
	glconfig_t		glconfig;
	float			xscale;
	float			yscale;
	float			bias;
	int			activemenu;
	fontInfo_t		font;
	qhandle_t		whiteShader;
	qhandle_t		cursor;
	qhandle_t		back_a;
	qhandle_t		back_b;
	sfxHandle_t		main_theme;
} uiStatic_t;

extern void			UI_Init( void );
extern void			UI_Shutdown( void );
extern void			UI_KeyEvent( int key, int down );
extern void			UI_MouseEvent( int dx, int dy );
extern void			UI_Refresh( int realtime );
extern qboolean			UI_ConsoleCommand( int realTime );
extern float			UI_ClampCvar( float min, float max, float value );
extern void			UI_DrawNamedPic( float x, float y, float width, float height, const char *picname );
extern void			UI_DrawHandlePic( float x, float y, float w, float h, qhandle_t hShader ); 
extern void			UI_FillRect( float x, float y, float width, float height, const float *color );
extern void			UI_DrawRect( float x, float y, float width, float height, const float *color );
extern void			UI_UpdateScreen( void );
extern void			UI_SetColor( const float *rgba );
extern void			UI_LerpColor(vec4_t a, vec4_t b, vec4_t c, float t);
extern void			UI_DrawString( int x, int y, const char* str, int style, vec4_t color );
extern qboolean 		UI_CursorInRect (int x, int y, int width, int height);
extern void			UI_AdjustFrom640( float *x, float *y, float *w, float *h );
extern void			UI_DrawTextBox (int x, int y, int width, int lines);
extern qboolean			UI_IsFullscreen( void );
extern void			UI_SetActiveMenu( uiMenuCommand_t menu );
extern char			*UI_Argv( int arg );
extern char			*UI_Cvar_VariableString( const char *var_name );
extern void			UI_Refresh( int time );
extern uiStatic_t		uis;

//
// ui_connect.c
//
extern void UI_DrawConnectScreen( qboolean overlay );

//
// ui_ingame.c
//
void UI_InGameMenu( void );

//
// ui_menu.c
//
void UI_MainMenu( void );

//
// ui_syscalls.c
//
void			trap_Print( const char *string );
void			trap_Error( const char *string ) __attribute__((noreturn));
int				trap_Milliseconds( void );
void			trap_Cvar_Register( vmCvar_t *vmCvar, const char *varName, const char *defaultValue, int flags );
void			trap_Cvar_Update( vmCvar_t *vmCvar );
void			trap_Cvar_Set( const char *var_name, const char *value );
float			trap_Cvar_VariableValue( const char *var_name );
void			trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
void			trap_Cvar_SetValue( const char *var_name, float value );
void			trap_Cvar_Reset( const char *name );
void			trap_Cvar_Create( const char *var_name, const char *var_value, int flags );
void			trap_Cvar_InfoStringBuffer( int bit, char *buffer, int bufsize );
int				trap_Argc( void );
void			trap_Argv( int n, char *buffer, int bufferLength );
void			trap_Cmd_ExecuteText( int exec_when, const char *text );	// don't use EXEC_NOW!
int				trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void			trap_FS_Read( void *buffer, int len, fileHandle_t f );
void			trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void			trap_FS_FCloseFile( fileHandle_t f );
int				trap_FS_GetFileList(  const char *path, const char *extension, char *listbuf, int bufsize );
int				trap_FS_Seek( fileHandle_t f, long offset, int origin ); // fsOrigin_t
qhandle_t		trap_R_RegisterModel( const char *name );
qhandle_t		trap_R_RegisterSkin( const char *name );
qhandle_t		trap_R_RegisterShaderNoMip( const char *name );
void			trap_R_ClearScene( void );
void			trap_R_AddRefEntityToScene( const refEntity_t *re );
void			trap_R_AddPolyToScene( qhandle_t hShader , int numVerts, const polyVert_t *verts );
void			trap_R_AddLightToScene( const vec3_t org, float intensity, float r, float g, float b );
void			trap_R_RenderScene( const refdef_t *fd );
void			trap_R_SetColor( const float *rgba );
void			trap_R_DrawStretchPic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t hShader );
void			trap_UpdateScreen( void );
int				trap_CM_LerpTag( orientation_t *tag, clipHandle_t mod, int startFrame, int endFrame, float frac, const char *tagName );
void			trap_S_StartLocalSound( sfxHandle_t sfx, int channelNum );
sfxHandle_t	trap_S_RegisterSound( const char *sample, qboolean compressed );
void			trap_S_StopBackgroundTrack( void );
void			trap_S_StartBackgroundTrack( const char *intro, const char *loop);
void			trap_Key_KeynumToStringBuf( int keynum, char *buf, int buflen );
void			trap_Key_GetBindingBuf( int keynum, char *buf, int buflen );
void			trap_Key_SetBinding( int keynum, const char *binding );
qboolean		trap_Key_IsDown( int keynum );
qboolean		trap_Key_GetOverstrikeMode( void );
void			trap_Key_SetOverstrikeMode( qboolean state );
void			trap_Key_ClearStates( void );
int				trap_Key_GetCatcher( void );
void			trap_Key_SetCatcher( int catcher );
void			trap_GetClipboardData( char *buf, int bufsize );
void			trap_GetClientState( uiClientState_t *state );
void			trap_GetGlconfig( glconfig_t *glconfig );
int				trap_GetConfigString( int index, char* buff, int buffsize );
int				trap_LAN_GetServerCount( int source );
void			trap_LAN_GetServerAddressString( int source, int n, char *buf, int buflen );
void			trap_LAN_GetServerInfo( int source, int n, char *buf, int buflen );
int				trap_LAN_GetPingQueueCount( void );
int				trap_LAN_ServerStatus( const char *serverAddress, char *serverStatus, int maxLen );
void			trap_LAN_ClearPing( int n );
void			trap_LAN_GetPing( int n, char *buf, int buflen, int *pingtime );
void			trap_LAN_GetPingInfo( int n, char *buf, int buflen );
int				trap_MemoryRemaining( void );
void			trap_GetCDKey( char *buf, int buflen );
void			trap_SetCDKey( char *buf );

void			trap_R_RegisterFont(const char *pFontname, int pointSize, fontInfo_t *font);

qboolean               trap_VerifyCDKey( const char *key, const char *chksum);

void			trap_SetPbClStatus( int status );

#endif
