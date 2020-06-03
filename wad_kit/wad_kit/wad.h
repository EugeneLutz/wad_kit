//
//  wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//
// Achtung! These structures do not reflect .wad file

#ifndef wad_kit_wad_h
#define wad_kit_wad_h

#include "wk_vertex.h"
#include "wk_polygon.h"
#include "animation.h"
#include "texture.h"
#include "movable.h"

typedef struct MESH MESH;
typedef struct STATIC STATIC;

typedef struct WAD
{
	unsigned int version;
	
	// DONE
	unsigned int numTextureSamples;
	TEXTURE_SAMPLE* textureSamples;
	
	// DONE
	unsigned int numTexturePages;
	TEXTURE_PAGE* texturePages;
	
	// DONE
	/* In theory, movables can refer to the same skeleton. Let's check it in future.
	   If not, then move each skeleton instance to related mesh. */
	unsigned int numSkeletons;
	SKELETON* skeletons;
	
	// DONE
	unsigned int numMeshes;
	MESH* meshes;
	
	// DONE
	unsigned int numMovables;
	MOVABLE* movables;
	
	// DONE
	unsigned int numStatics;
	STATIC* statics;
}
WAD;

#endif /* wad_kit_wad_h */