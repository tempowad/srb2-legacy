// SONIC ROBO BLAST 2
//-----------------------------------------------------------------------------
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2000 by DooM Legacy Team.
// Copyright (C) 1999-2018 by Sonic Team Junior.
//
// This program is free software distributed under the
// terms of the GNU General Public License, version 2.
// See the 'LICENSE' file for more details.
//-----------------------------------------------------------------------------
/// \file  r_main.h
/// \brief Rendering variables, consvars, defines

#ifndef __R_MAIN__
#define __R_MAIN__

#include "d_player.h"
#include "r_data.h"
#include "m_perfstats.h"

//
// POV related.
//
extern fixed_t viewcos, viewsin;
extern INT32 viewheight;
extern INT32 centerx, centery;

extern fixed_t centerxfrac, centeryfrac;
extern fixed_t projection, projectiony;
extern fixed_t fovtan;

// WARNING: a should be unsigned but to add with 2048, it isn't!
#define AIMINGTODY(a) FixedDiv((FINETANGENT((2048+(((INT32)a)>>ANGLETOFINESHIFT)) & FINEMASK)*160), fovtan)


#define MINFOV 60
#define MAXFOV 179

extern size_t validcount, linecount, loopcount, framecount;

// The fraction of a tic being drawn (for interpolation between two tics)
extern fixed_t rendertimefrac;
// Same as rendertimefrac but not suspended when the game is paused
extern fixed_t rendertimefrac_unpaused;
// Evaluated delta tics for this frame (how many tics since the last frame)
extern fixed_t renderdeltatics;
// The current render is a new logical tic
extern boolean renderisnewtic;

INT32 R_GetHudUncap(boolean menu);



//
// Lighting LUT.
// Used for z-depth cuing per column/row,
//  and other lighting effects (sector ambient, flash).
//

// Lighting constants.
// Now with 32 levels.
#define LIGHTLEVELS 32
#define LIGHTSEGSHIFT 3
#include "r_fps.h" // Uncapped framerate -- Fury
#include "i_system.h"
#define MAXLIGHTSCALE 48
#define LIGHTSCALESHIFT 12
#define MAXLIGHTZ 128
#define LIGHTZSHIFT 20

#define LIGHTRESOLUTIONFIX (640*fovtan/vid.width)

extern lighttable_t *scalelight[LIGHTLEVELS][MAXLIGHTSCALE];
extern lighttable_t *scalelightfixed[MAXLIGHTSCALE];
extern lighttable_t *zlight[LIGHTLEVELS][MAXLIGHTZ];

// Number of diminishing brightness levels.
// There a 0-31, i.e. 32 LUT in the COLORMAP lump.
#define NUMCOLORMAPS 32

// Utility functions.
INT32 R_OldPointOnSide(fixed_t x, fixed_t y, const node_t *node);

ATTRINLINE FUNCINLINE INT32 PUREFUNC R_PointOnSide(fixed_t x, fixed_t y, const node_t *node)
{
	// use cross product to determine side quickly
	return ((INT64)y - node->y) * node->dx - ((INT64)x - node->x) * node->dy > 0;
}

ATTRINLINE FUNCINLINE INT32 PUREFUNC R_PointOnSegSide(fixed_t x, fixed_t y, const seg_t *line)
{
	fixed_t lx = line->v1->x;
	fixed_t ly = line->v1->y;
	fixed_t ldx = line->v2->x - lx;
	fixed_t ldy = line->v2->y - ly;

	// use cross product to determine side quickly
	return ((INT64)y - ly) * ldx - ((INT64)x - lx) * ldy > 0;
}

angle_t R_PointToAngle(fixed_t x, fixed_t y);
angle_t R_PointToAngle64(INT64 x, INT64 y);
angle_t R_PointToAngle2(fixed_t px2, fixed_t py2, fixed_t px1, fixed_t py1);
angle_t R_PointToAngleEx(INT32 x2, INT32 y2, INT32 x1, INT32 y1);
fixed_t R_PointToDist(fixed_t x, fixed_t y);
fixed_t R_PointToDist2(fixed_t px2, fixed_t py2, fixed_t px1, fixed_t py1);

fixed_t R_ScaleFromGlobalAngle(angle_t visangle);
boolean R_IsPointInSector(sector_t *sector, fixed_t x, fixed_t y);
subsector_t *R_PointInSubsector(fixed_t x, fixed_t y);
subsector_t *R_OldPointInSubsector(fixed_t x, fixed_t y);
subsector_t *R_IsPointInSubsector(fixed_t x, fixed_t y);

boolean R_DoCulling(line_t *cullheight, line_t *viewcullheight, fixed_t vz, fixed_t bottomh, fixed_t toph);

// Render stats

extern precise_t ps_prevframetime;// time when previous frame was rendered
extern ps_metric_t ps_rendercalltime;
extern ps_metric_t ps_otherrendertime;
extern ps_metric_t ps_uitime;
extern ps_metric_t ps_swaptime;

extern ps_metric_t ps_skyboxtime;
extern ps_metric_t ps_bsptime;

extern ps_metric_t ps_sw_spritecliptime;
extern ps_metric_t ps_sw_portaltime;
extern ps_metric_t ps_sw_planetime;
extern ps_metric_t ps_sw_maskedtime;

extern ps_metric_t ps_numbspcalls;
extern ps_metric_t ps_numsprites;
extern ps_metric_t ps_numdrawnodes;
extern ps_metric_t ps_numpolyobjects;


//
// REFRESH - the actual rendering functions.
//

extern consvar_t cv_showhud, cv_translucenthud, cv_uncappedhud;
extern consvar_t cv_homremoval;
extern consvar_t cv_chasecam, cv_chasecam2;
extern consvar_t cv_flipcam, cv_flipcam2;
extern consvar_t cv_shadow, cv_shadowoffs;
extern consvar_t cv_skydome;
extern consvar_t cv_ffloorclip;
extern consvar_t cv_translucency;
extern consvar_t cv_precipdensity, cv_drawdist, cv_drawdist_nights, cv_drawdist_precip;
extern consvar_t cv_fov, cv_fovchange;
extern consvar_t cv_skybox;
extern consvar_t cv_tailspickup;


// Called by startup code.
void R_Init(void);

// just sets setsizeneeded true
extern boolean setsizeneeded;
void R_SetViewSize(void);

void R_CheckViewMorph(void);
void R_ApplyViewMorph(void);


// do it (sometimes explicitly called)
void R_ExecuteSetViewSize(void);

fixed_t R_GetPlayerFov(player_t *player);

void R_SkyboxFrame(player_t *player);

void R_SetupFrame(player_t *player, boolean skybox);
boolean R_IsViewpointThirdPerson(player_t *player, boolean skybox);
// Called by G_Drawer.
void R_RenderPlayerView(player_t *player);

// add commands related to engine, at game startup
void R_RegisterEngineStuff(void);
#endif
