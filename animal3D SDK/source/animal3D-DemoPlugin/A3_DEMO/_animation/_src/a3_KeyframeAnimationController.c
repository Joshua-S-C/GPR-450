/*
	Copyright 2011-2025 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

/*
	Modifications organized by Author
	Team:		Initial clip controller update functionality. Updating clip and keyframe time, keyframe index, keyframe param, and resetting at end of clip
	Jupi:		Finishing update base functionality. Resolving keyframe and clip times with while loops, accounting for overshoot and direction, clip param, transition action setup and finished loop and stop
	Brandon:	
	Ben:		
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_sec, const a3f64 playback_secPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_sec, playback_secPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (!clipCtrl || !clipCtrl->clipPool)
		return -1;

	if (dt == 0)
		return -1;

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// Test commit - Jupi
	// test commit brandon :3

	// algorithm
	// 1. time step
	// 2. resolve keyframe
	//		a. paused: dt = 0
	//		b. forward dt > 0
	//			i. stop
	//			ii. step(s) taken
	//			iii. clip exited
	//		c. reverse: dt < 0
	//			i. stop
	//			ii. step(s) taken
	//			iii. clip exited
	// 3. normalized keyframe / clip time: relative time / duration

	// This is the new forward backwards
	clipCtrl->playback_sec = (clipCtrl->playback_sec > 0) - (clipCtrl->playback_sec < 0);

	// Time Step
	clipCtrl->clipTime_sec += dt * clipCtrl->clip->keyframeDirection * clipCtrl->playback_sec;
	clipCtrl->keyframeTime_sec += dt * clipCtrl->clip->keyframeDirection * clipCtrl->playback_sec;

	// Resolve Clip
	a3f64 clipTimeOvershoot = clipCtrl->playback_sec == 1
		? clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec
		: -clipCtrl->clipTime_sec;

	while (clipCtrl->playback_sec == 1 
		? clipTimeOvershoot >= 0 
		: clipCtrl->clipTime_sec < 0)
	{
		// Determine transition action going backwards and forwards
		a3_ClipTransitionFlag transitionFlag = (clipCtrl->playback_sec == 1
											   ? clipCtrl->clip->transitionForward->flag 
				                               : clipCtrl->clip->transitionReverse->flag);

		switch (transitionFlag)
		{
			// Stop
			case a3clip_stopFlag:
				// Set keyframe to last
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe;

				// Max out times
				clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec;
				clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec;

				// Max out params
				clipCtrl->keyframeParam = 1;
				clipCtrl->clipParam = 1;

				return -1;

			// Loop
			case a3clip_playFlag:
				// Set time
				clipCtrl->clipTime_sec = clipCtrl->playback_sec == 1
					? clipTimeOvershoot
					: clipCtrl->clip->duration_sec + (clipTimeOvershoot + clipCtrl->clip->duration_sec);
				
				// Set keyframe
				clipCtrl->keyframeTime_sec = clipCtrl->clipTime_sec;
				clipCtrl->keyframeIndex = clipCtrl->playback_sec == 1 
					? 0
					: clipCtrl->clip->keyframeIndex_final;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				break;

			// Ping Pong
			case a3clip_reverseFlag:
				if (clipCtrl->playback_sec == 1) {
					clipCtrl->playback_sec *= -1; // Temp

					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;

					clipCtrl->clipTime_sec = clipCtrl->clip->duration_sec - clipTimeOvershoot;
					clipCtrl->keyframeTime_sec = clipCtrl->keyframe->duration_sec - clipTimeOvershoot;


				} else {
					clipCtrl->playback_sec *= -1; // Temp

					clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;

					clipCtrl->clipTime_sec = clipTimeOvershoot;
					clipCtrl->keyframeTime_sec = clipTimeOvershoot;
				}

				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				break;

			default:
				// Achivement Unlocked: How did we get here?
				break;
		}

		// Keep checking if past clip
		clipTimeOvershoot = clipCtrl->playback_sec == 1
			? clipCtrl->clipTime_sec - clipCtrl->clip->duration_sec
			: -clipCtrl->clipTime_sec;
	}
		
	// Resolve Keyframes
	a3f64 frameTimeOvershoot = clipCtrl->keyframeTime_sec - clipCtrl->keyframe->duration_sec;
	
	// Going forward
	while (clipCtrl->playback_sec == 1 
		? frameTimeOvershoot >= 0 
		: frameTimeOvershoot < -clipCtrl->keyframe->duration_sec)
	{
		if (clipCtrl->playback_sec == 1) 
		{
			// Go to next keyframe. 
			// If it's the last keyframe, then the clip was already looped anyway
			if (clipCtrl->keyframeIndex < (a3ui32)clipCtrl->clip->keyframeIndex_final)
				clipCtrl->keyframeIndex++;

			clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
			clipCtrl->keyframeTime_sec = frameTimeOvershoot;

			// Keep checking if past next frame
			frameTimeOvershoot = clipCtrl->keyframeTime_sec - clipCtrl->keyframe->duration_sec;
		} else if (clipCtrl->playback_sec == -1) {
			// Go to prev keyframe
			if (clipCtrl->keyframeIndex > 0)
				clipCtrl->keyframeIndex--;

			clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

			// Keep checking if past next frame
			frameTimeOvershoot = clipCtrl->keyframe->duration_sec + clipCtrl->keyframeTime_sec;

			clipCtrl->keyframeTime_sec = frameTimeOvershoot;
		}
	}

	// Yupdate relative time
	clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec / clipCtrl->keyframe->duration_sec;
	clipCtrl->clipParam = clipCtrl->clipTime_sec / clipCtrl->clip->duration_sec;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	return -1;
}


//-----------------------------------------------------------------------------
