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
	
	a3_Scene_Animation-idle-update.c/.cpp
	Demo mode implementations: animation scene.

	********************************************
	*** UPDATE FOR ANIMATION SCENE MODE      ***
	********************************************
*/

/*
	Modifications organized by Author
	Ben:		Creating new animation for teapot
*/

//-----------------------------------------------------------------------------

#include "../a3_Scene_Animation.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_scene_utilities/a3_SceneMacros.h"


//-----------------------------------------------------------------------------
// UTILS

void a3demo_update_objects(a3f64 const dt, a3_SceneObject* sceneObjectBase,
	a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_SceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_SceneObject* obj_camera, a3_SceneObject* obj_skybox);
void a3demo_update_pointLight(a3_SceneObject* obj_camera, a3_ScenePointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_SceneObject* sceneObject, a3real4x4p s);

void a3demo_updateHierarchyGraphics(
	a3mat4* transform_mvp_joints, a3mat4* transform_mvp_bones, a3mat4* transform_t_skin, a3dualquat* transform_dq_skin, a3ui32 const max_transforms,
	a3mat4 const mvp_obj, a3_HierarchyState const* activeHS);


//-----------------------------------------------------------------------------
// UPDATE

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-3: ADD IK LOGIC
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD UPDATE LOGIC
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------


void a3animation_update_animation_other(
	a3_Scene_Animation* scene, a3f64 const dt)
{
	a3ui32 sampleIndex0;
	a3f64 keyframeParam;
	a3f64 clipParam;

	a3ui32 sampleIndex1;
	a3f64 keyframeParam2;
	a3f64 clipParam2;

	a3clipControllerUpdate(scene->clipCtrl_morph, dt);
	a3clipControllerUpdate(scene->clipCtrl_morph2, dt);
	sampleIndex0 = scene->clipPool->keyframe[scene->clipCtrl_morph->keyframeIndex].sampleIndex0;
	sampleIndex1 = scene->clipPool2->keyframe[scene->clipCtrl_morph2->keyframeIndex].sampleIndex0;
	keyframeParam = scene->clipCtrl_morph->keyframeParam;
	keyframeParam2 = scene->clipCtrl_morph2->keyframeParam;
	clipParam = scene->clipCtrl_morph->clipParam;
	clipParam2 = scene->clipCtrl_morph->clipParam;

	scene->morph_time = (a3f64)sampleIndex0 + keyframeParam;
	scene->obj_teapot->euler.z = a3trigValid_sind((a3real)keyframeParam * a3real_threesixty);

	//Animation for the 2nd teapot
	scene->obj_teapot2->euler.x = a3trigValid_sind((a3real)keyframeParam2 * 360);
	scene->obj_teapot2->euler.y = a3trigValid_sind((a3real)keyframeParam2 * 360);
	scene->obj_teapot2->euler.z = a3trigValid_sind((a3real)keyframeParam2 * 180);
	scene->obj_teapot2->scale.x = a3trigValid_sind((a3real)keyframeParam2 * (a3real)2.5);
	scene->obj_teapot2->scale.y = a3trigValid_sind((a3real)keyframeParam2 * (a3real)0.5);
	scene->obj_teapot2->scale.z = a3trigValid_sind((a3real)keyframeParam2 * (a3real)1.5);

	scene->obj_teapot2->position.y = a3trigValid_sind((a3real)keyframeParam2 * 5);
	scene->obj_teapot2->position.x = -a3trigValid_sind((a3real)keyframeParam2 * 10);
}

void a3animation_update_animation(
	a3_Scene_Animation* scene, a3f64 const dt,
	a3boolean const updateIK)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD UPDATE LOGIC
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

	a3animation_update_animation_other(scene, dt);
}

void a3animation_update_sceneGraph(a3_Scene_Animation* scene, a3f64 const dt)
{
	a3ui32 i;
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(dt, scene->object_scene, animationMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(dt, scene->obj_camera_main, 1, 1, 0);

	a3scene_updateProjectorViewProjectionMat(scene->proj_camera_main);

	// apply scales to objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3demo_applyScale_internal(scene->object_scene + i, scaleMat.m);
	}

	// update skybox
	a3demo_update_bindSkybox(scene->obj_camera_main, scene->obj_skybox);

	// the scene graph is also a hierarchy - update FK
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		scene->sceneGraphState->localSpace->hpose_base[i].transformMat = scene->object_scene[i].modelMat;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: UNCOMMENT ME WHEN FK IMPLEMENTED
//-----------------------------------------------------------------------------
	//a3kinematicsSolveForward(scene->sceneGraphState);
	//a3hierarchyStateUpdateLocalInverse(scene->sceneGraphState);
	//a3hierarchyStateUpdateObjectInverse(scene->sceneGraphState);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: REMOVE ME WHEN FK IMPLEMENTED
//-----------------------------------------------------------------------------
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		//****HINT: consider this for one part of the core FK function - what does this do?
		scene->sceneGraphState->objectSpace->hpose_base[i].transformMat = scene->sceneGraphState->localSpace->hpose_base[i].transformMat;

		//****HINT: consider this for updating object-space inverses
		a3real4x4TransformInverse(
			scene->sceneGraphState->objectSpaceInv->hpose_base[i].transformMat.m,
			scene->sceneGraphState->objectSpace->hpose_base[i].transformMat.m);
	}
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

void a3animation_update(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt)
{
	a3ui32 i;
	a3boolean const updateIK = true;
	a3boolean const updateBlendTreeFK = true;

	// active camera
	a3_SceneProjector const* activeCamera = scene->projector + scene->activeCamera;
	a3_SceneObject const* activeCameraObject = activeCamera->sceneObject;

	// skeletal
	if (demoState->updateAnimation)
	{
		a3animation_update_animation(scene, dt, updateIK);
	}

	// update scene graph local transforms
	a3animation_update_sceneGraph(scene, dt);

	// update matrix stack data using scene graph
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
	{
		a3scene_updateModelMatrixStack(scene->matrixStack + i,
			activeCamera->projectionMat.m,
			scene->sceneGraphState->objectSpace->hpose_base[scene->obj_camera_main->sceneGraphIndex].transformMat.m,
			scene->sceneGraphState->objectSpaceInv->hpose_base[scene->obj_camera_main->sceneGraphIndex].transformMat.m,
			scene->sceneGraphState->objectSpace->hpose_base[scene->object_scene[i].sceneGraphIndex].transformMat.m,
			a3mat4_identity.m);
	}
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-2: ADD SKELETAL GRAPHICS UPDATE
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: CONTROL
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
