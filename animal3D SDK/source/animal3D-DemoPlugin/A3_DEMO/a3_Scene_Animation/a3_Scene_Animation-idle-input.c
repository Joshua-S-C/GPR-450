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

	a3_Scene_Animation-idle-input.c/.cpp
	Demo mode implementations: animation scene.

	********************************************
	*** INPUT FOR ANIMATION SCENE MODE       ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Scene_Animation.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_scene_utilities/a3_SceneMacros.h"


//-----------------------------------------------------------------------------
// CALLBACKS

// main demo mode callback
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state)
{
	switch (asciiKey)
	{
		// toggle render program
		//a3sceneCtrlCasesLoop(scene->render, animation_render_max, 'k', 'j');

		// toggle display program
		//a3sceneCtrlCasesLoop(scene->display, animation_display_max, 'K', 'J');

		// toggle active camera
		//a3sceneCtrlCasesLoop(scene->activeCamera, animation_camera_max, 'v', 'c');

		// toggle pipeline mode
		a3sceneCtrlCasesLoop(scene->pipeline, animation_pipeline_max, ']', '[');

		// toggle target
		a3sceneCtrlCasesLoop(scene->targetIndex[scene->pass], scene->targetCount[scene->pass], '}', '{');

		// toggle pass to display
		a3sceneCtrlCasesLoop(scene->pass, animation_pass_max, ')', '(');

		// toggle control target
		a3sceneCtrlCasesLoop(scene->ctrl_target, animation_ctrlmode_max, '\'', ';');

		// toggle position input mode
		a3sceneCtrlCasesLoop(scene->ctrl_position, animation_inputmode_max, '=', '-');
		
		// toggle rotation input mode
		a3sceneCtrlCasesLoop(scene->ctrl_rotation, animation_inputmode_max, '+', '_');
	}
}

void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state)
{
//	switch (asciiKey)
//	{
//
//	}
}


//-----------------------------------------------------------------------------

void a3demo_input_controlObject(
	a3_DemoState* demoState, a3_SceneObject* object,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed);
void a3demo_input_controlProjector(
	a3_DemoState* demoState, a3_SceneProjector* projector,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed, a3real ctrlZoomSpeed);

void a3animation_input(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt)
{
	a3_SceneProjector* projector = scene->projector + scene->activeCamera;
	a3_SceneObject* sceneObject;

	// right click to ray pick
	if (a3mouseGetState(demoState->mouse, a3mouse_right) == a3input_down)
	{
		// get window coordinates
		a3i32 const x = a3mouseGetX(demoState->mouse) + demoState->frameBorder;
		a3i32 const y = a3mouseGetY(demoState->mouse) + demoState->frameBorder;

		// transform to NDC
		a3vec4 coord = a3vec4_one;
		coord.x = +((a3real)x * demoState->frameWidthInv * a3real_two - a3real_one);
		coord.y = -((a3real)y * demoState->frameHeightInv * a3real_two - a3real_one);
		coord.z = -a3real_one;

		// transform to view space
		a3real4Real4x4Mul(projector->projectionMatInv.m, coord.v);
		a3real4DivS(coord.v, coord.w);

		// transform to world space
		a3real4Real4x4Mul(projector->sceneObject->modelMat.m, coord.v);
	}
	
	// choose control target
	switch (scene->ctrl_target)
	{
	case animation_ctrl_camera:
		// move camera
		a3demo_input_controlProjector(demoState, projector,
			dt, projector->ctrlMoveSpeed, projector->ctrlRotateSpeed, projector->ctrlZoomSpeed);
		break;
	case animation_ctrl_teapot:
		sceneObject = scene->obj_teapot;
		a3demo_input_controlObject(demoState, sceneObject, dt, a3real_one, a3real_zero);
		break;

	case animation_ctrl_character:
		sceneObject = scene->obj_skeleton_ctrl + scene->ctrl_target - animation_ctrl_character;
		a3demo_input_controlObject(demoState, sceneObject, dt, a3real_one, a3real_zero);
		break;

	case animation_ctrl_character_rig:
	case animation_ctrl_neckLookat:
	case animation_ctrl_wristEffector_r:
	case animation_ctrl_wristConstraint_r:
	case animation_ctrl_wristEffector_l:
	case animation_ctrl_wristConstraint_l:
	case animation_ctrl_ankleEffector_r:
	case animation_ctrl_ankleConstraint_r:
	case animation_ctrl_ankleEffector_l:
	case animation_ctrl_ankleConstraint_l:
		sceneObject = scene->obj_skeleton_ctrl + scene->ctrl_target - animation_ctrl_character;
		a3demo_input_controlObject(demoState, sceneObject, dt, a3real_one, a3real_zero);
		break;
	}

	// capture axes
/*	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		// get directly from joysticks
		a3XboxControlGetJoysticks(demoState->xcontrol, scene->axis_l, scene->axis_r);
	}
	else
	{
		// calculate normalized vectors given keyboard state
		a3f64 lenInv;
		scene->axis_l[0] = (a3f64)a3keyboardGetDifference(demoState->keyboard, a3key_D, a3key_A);
		scene->axis_l[1] = (a3f64)a3keyboardGetDifference(demoState->keyboard, a3key_W, a3key_S);
		scene->axis_r[0] = (a3f64)a3keyboardGetDifference(demoState->keyboard, a3key_L, a3key_J);
		scene->axis_r[1] = (a3f64)a3keyboardGetDifference(demoState->keyboard, a3key_I, a3key_K);

		lenInv = scene->axis_l[0] * scene->axis_l[0] + scene->axis_l[1] * scene->axis_l[1];
		lenInv = __a3isNotNearZeroF64(lenInv) ? a3sqrtdInverse(lenInv) : __a3f64zero;
		scene->axis_l[0] *= lenInv;
		scene->axis_l[1] *= lenInv;

		lenInv = scene->axis_r[0] * scene->axis_r[0] + scene->axis_r[1] * scene->axis_r[1];
		lenInv = __a3isNotNearZeroF64(lenInv) ? a3sqrtdInverse(lenInv) : __a3f64zero;
		scene->axis_r[0] *= lenInv;
		scene->axis_r[1] *= lenInv;
	}
*/

	// allow the controller, if connected, to change control targets
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_right))
			a3sceneCtrlIncLoop(scene->ctrl_target, animation_ctrlmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_DPAD_left))
			a3sceneCtrlDecLoop(scene->ctrl_target, animation_ctrlmode_max);
		
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_B))
			a3sceneCtrlIncLoop(scene->ctrl_position, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_X))
			a3sceneCtrlDecLoop(scene->ctrl_position, animation_inputmode_max);
		
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_Y))
			a3sceneCtrlIncLoop(scene->ctrl_rotation, animation_inputmode_max);
		if (a3XboxControlIsPressed(demoState->xcontrol, a3xbox_A))
			a3sceneCtrlDecLoop(scene->ctrl_rotation, animation_inputmode_max);
	}
}


//-----------------------------------------------------------------------------
