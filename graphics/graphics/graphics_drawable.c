//
//  graphics_drawable.c
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void graphicsDrawableReturn(GRAPHICS_DRAWABLE* graphicsDrawable)
{
	assert(graphicsDrawable);
	
	if (graphicsDrawable->texture)
	{
		graphicsDrawable->graphicsView->drawableReturnTexture(graphicsDrawable, graphicsDrawable->texture);
		debug_memset(graphicsDrawable->texture, 0, sizeof(TEXTURE2D));
		magicArrayRemoveItem(&graphicsDrawable->graphicsView->device->textures, graphicsDrawable->texture);
	}
	
	graphicsDrawable->graphicsView->returnDrawableFunc(graphicsDrawable);
	magicArrayRemoveItem(&graphicsDrawable->graphicsView->drawables, graphicsDrawable);
	//debug_memset(graphicsDrawable, 0, sizeof(GRAPHICS_DRAWABLE));
}

TEXTURE2D* graphicsDrawableGetTexture(GRAPHICS_DRAWABLE* graphicsDrawable)
{
	assert(graphicsDrawable);
	
	void* textureId = graphicsDrawable->graphicsView->drawableGetTextureFunc(graphicsDrawable);
	assert(textureId);
	
	GR_DEVICE* device = graphicsDrawable->graphicsView->device;
	TEXTURE2D* texture = magicArrayAddItem(&device->textures);
	texture->device = device;
	texture->width = 0;		// TODO: get texture size and usage
	texture->height = 0;
	texture->usage = TEXTURE_USAGE_UNKNOWN;
	texture->isReceivedOutside = 1;
	texture->textureId = textureId;
	
	graphicsDrawable->texture = texture;
	
	return texture;
}
