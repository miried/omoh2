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
#include "ui_local.h"

/*
===============================================================================

CONNECTION SCREEN

===============================================================================
*/


/*
========================
UI_DrawConnectScreen

This will also be overlaid on the cgame info screen during loading
to prevent it from blinking away too rapidly on local or lan games.
========================
*/
void UI_DrawConnectScreen( qboolean overlay ) {
	vec4_t col_blue = { 0.0, 0.0, 1.0, 1.0 };

	Com_Printf( "connect screen \n" );
	UI_FillRect( 0, 0, 640, 480, col_blue );
}


/*
===================
UI_KeyConnect
===================
*/
void UI_KeyConnect( int key ) {
	if ( key == K_ESCAPE ) {
		trap_Cmd_ExecuteText( EXEC_APPEND, "disconnect\n" );
		return;
	}
}
