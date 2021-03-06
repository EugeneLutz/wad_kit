//
//  movable.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef movable_h
#define movable_h

#include "wad_kit.h"

typedef struct WK_MOVABLE
{
	WK_WAD* wad;
	unsigned long numReferences;
	
	MOVABLE_ID movableId;
	
	WK_MESH* rootMesh;
	
	/*!
	 Collection of @b WK_JOINT objects, where other meshes are located.
	 */
	MAGIC_ARRAY joints;
	
	/*!
	 Collection of @b WK_ANIMATION objects.
	 */
	MAGIC_ARRAY animations;
}
WK_MOVABLE;

#endif /* movable_h */
