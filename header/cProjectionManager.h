//
//										Benjamin Hernandez, PhD. benjamin.hernandez@bsc.es
//																	   All Rights Reserved
// 														   Barcelona Supercomputing Center
//
//															   See ReadMe.txt for details
//
//=======================================================================================
#pragma once
#include "cMacros.h"

#ifndef __PROJECTION_MANAGER
#define __PROJECTION_MANAGER

class ProjectionManager
{
public:
				ProjectionManager	(	void									);
				~ProjectionManager	(	void									);

	void		setOrthoProjection	(	unsigned int	w,
										unsigned int	h,
										bool			backup_viewport = true	);
	void		setOrthoProjection	(	unsigned int	x,
										unsigned int	y,
										unsigned int	w,
										unsigned int	h,
										unsigned int	l,
										unsigned int	r,
										unsigned int	b,
										unsigned int	t,
										bool			backup_viewport = true	);
	void		setTextProjection	(	unsigned int    w,
                                        unsigned int    h			            );
	void		restoreProjection	(	void									);
	GLint*		getViewport			(	void									);
	float		getAspectRatio		(	void									);
	static void displayText         (   int             x,
                                        int             y,
                                        char*           txt                     );
public:
	enum PROJECTION_TYPE{ ORTHOGRAPHIC, TEXT };
private:
	float		aspect_ratio;
	GLint		viewport[4];
	bool		backup_viewport;
	int			type;
};

#endif

//=======================================================================================
