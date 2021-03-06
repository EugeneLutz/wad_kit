//
//  animation.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

void animationInitialize(WK_ANIMATION* animation, WK_MOVABLE* movable, RAW_MOVABLE* rawMovable, RAW_ANIMATION* rawAnimation, RAW_ANIMATION* nextRawAnimation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(animation);
	assert(movable);
	assert(rawAnimation);
	// nextRawAnimation is nullable
	assert(loadInfo);
	
	WK_WAD* wad = loadInfo->wad;
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	animation->movable = movable;
	animation->stateId = rawAnimation->stateId;
	//magicArrayInitialize(&animation->keyframes, sizeof(WK_KEYFRAME), 64);
	magicArrayInitializeWithAllocator(&animation->keyframes, wad->keyframeAllocator);
	animation->frameDuration = rawAnimation->frameDuration;
	animation->moveSpeed = rawAnimation->speed;
	animation->moveAcceleration = rawAnimation->acceleration;
	animation->unknown1 = rawAnimation->unknown1;
	animation->unknown2 = rawAnimation->unknown2;
	animation->nextAnimation = rawAnimation->nextAnimation - rawMovable->animsIndex;
	assert(rawAnimation->nextAnimation >= rawMovable->animsIndex);
	
	// TODO: understand this shit
	animation->nextAnimationFrame = rawAnimation->frameIn;
	/*if (animation->nextAnimation < loadInfo->numAnimations)
	{
		RAW_ANIMATION* nextDefaultAnimation = &loadInfo->rawAnimations[rawAnimation->nextAnimation];
		if (rawAnimation->frameIn < nextDefaultAnimation->frameStart)
		{
			if (nextDefaultAnimation->keyframeSize == 0)
			{
				assert(0);
			}
			assert(0);
		}
		animation->nextAnimationFrame = rawAnimation->frameIn - nextDefaultAnimation->frameStart;
	}
	else
	{
		// WAD is just a little corrupted
		animation->nextAnimationFrame = 0;
		assert(0);
	}*/
	
	magicArrayInitialize(&animation->stateChanges, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(WK_STATE_CHANGE), 8);
	magicArrayInitialize(&animation->commands, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(WK_COMMAND), 8);
	
	// MARK: Read keyframes
	if (rawAnimation->keyframeSize > 0)
	{
		// Calculate number of keyframes. It's also tricky as reading movable's number of animations.
		unsigned int numKeyframes = 0;
		if (nextRawAnimation) {
			numKeyframes = (nextRawAnimation->keyframeOffset - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
		}
		else {
			numKeyframes = (loadInfo->numKeyframesWords * 2 - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
		}
		
		bufferReaderSetEditorPosition(buffer, loadInfo->keyframesDataLocation + rawAnimation->keyframeOffset);
		for (unsigned int keyframeIndex = 0; keyframeIndex < numKeyframes; keyframeIndex++)
		{
			bufferReaderSetEditorPosition(buffer, loadInfo->keyframesDataLocation + rawAnimation->keyframeOffset + keyframeIndex * rawAnimation->keyframeSize * 2);
			
			WK_KEYFRAME* keyframe = magicArrayAddItem(&animation->keyframes);
			keyframeInitialize(keyframe, animation, rawAnimation, loadInfo);
			if (executeResultIsFailed(executeResult)) { return; }
		}
	}
	
	// MARK: Read commands
	bufferReaderSetEditorPosition(buffer, loadInfo->commandsDataLocation + rawAnimation->commandOffsets * 2);
	for (unsigned int i = 0; i < rawAnimation->numCommands; i++)
	{
		WK_COMMAND* command = magicArrayAddItem(&animation->commands);
		commandInitialize(command, animation, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	// MARK: Read state changes
	for (unsigned int i = 0; i < rawAnimation->numStateChanges; i++)
	{
		RAW_STATE_CHANGE* rawStateChange = &loadInfo->rawStateChanges[rawAnimation->changesIndex + i];
		WK_STATE_CHANGE* stateChange = magicArrayAddItem(&animation->stateChanges);
		stateChangeInitialize(stateChange, animation, rawStateChange, loadInfo);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void animationDeinitialize(WK_ANIMATION* animation)
{
	assert(animation);
	
	for (unsigned int i = 0; i < animation->stateChanges.length; i++)
	{
		WK_STATE_CHANGE* stateChange = magicArrayGetItem(&animation->stateChanges, i);
		stateChangeDeinitialize(stateChange);
	}
	magicArrayDeinitialize(&animation->stateChanges);
	
	for (unsigned int i = 0; i < animation->commands.length; i++)
	{
		WK_COMMAND* command = magicArrayGetItem(&animation->commands, i);
		commandDeinitialize(command);
	}
	magicArrayDeinitialize(&animation->commands);
	
	for (unsigned int i = 0; i < animation->keyframes.length; i++)
	{
		WK_KEYFRAME* keyframe = magicArrayGetItem(&animation->keyframes, i);
		keyframeDeinitialize(keyframe);
	}
	magicArrayDeinitialize(&animation->keyframes);
	
	memset(animation, 0, sizeof(WK_ANIMATION));
}

// MARK: - Public interface

unsigned int animationGetFrameDuration(WK_ANIMATION* animation)
{
	assert(animation);
	return animation->frameDuration;
}

unsigned int animationGetNumKeyframes(WK_ANIMATION* animation)
{
	assert(animation);
	return animation->keyframes.length;
}

WK_KEYFRAME* animationGetKeyframe(WK_ANIMATION* animation, unsigned int keyframeIndex)
{
	assert(animation);
	return magicArrayGetItem(&animation->keyframes, keyframeIndex);
}
