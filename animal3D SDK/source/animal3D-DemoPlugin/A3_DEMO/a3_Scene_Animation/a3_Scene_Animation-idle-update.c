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
//****TO-DO-ANIM-PREP-3: ADD UPDATE LOGIC
//-----------------------------------------------------------------------------

void a3animation_update_animation_skeletal(
	a3_Scene_Animation* scene, a3f64 const dt,
	a3boolean const updateIK)
{
	a3ui32 h = 0;
	a3ui32 n = sizeof(scene->clipCtrl) / sizeof(*scene->clipCtrl);

	a3_ClipController const* active_ctrl = scene->clipCtrl_idle_f;//change controller for different effect

	a3_HierarchyState* activeHS_fk = scene->hierarchyState_skel_fk;
	a3_HierarchyState* activeHS_ik = scene->hierarchyState_skel_ik;
	a3_HierarchyState* activeHS = scene->hierarchyState_skel_final;
	a3_HierarchyState const* baseHS = scene->hierarchyState_skel_base;
	a3_HierarchyPoseGroup const* poseGroup = scene->hierarchyPoseGroup_skel;

	// bail
	if (!activeHS->hierarchy || !activeHS->hierarchy->nodes)
		return;

	// update all clip poses
	for (h = 0; h < n; ++h)
	{
		a3_ClipController* clipCtrl_fk = &scene->clipCtrl[h];

		// update clip controller, keyframe lerp
		a3clipControllerUpdate(clipCtrl_fk, dt);
	}

	// resolve final FK state:
	// interpolate to frame and run FK pipeline
	{
		a3ui32 sampleIndex0, sampleIndex1;
		sampleIndex0 = scene->clipPool->keyframe[active_ctrl->keyframeIndex].sampleIndex0;
		sampleIndex1 = scene->clipPool->keyframe[active_ctrl->keyframeIndex].sampleIndex1;
		a3hierarchyPoseLerp(activeHS_fk->animPose,
			poseGroup->hpose + sampleIndex0, poseGroup->hpose + sampleIndex1,
			(a3real)active_ctrl->keyframeParam, activeHS_fk->hierarchy->numNodes);
		a3kinematicsUpdateHierarchyStateFK(activeHS_fk, baseHS, poseGroup);
	}

	// resolve final state
	// copy FK result
	a3hierarchyPoseCopy(activeHS->animPose,	// dst: IK anim
		activeHS_fk->animPose,				// src: FK anim
	//	baseHS->animPose,					// src test: base anim (identity)
		activeHS_fk->hierarchy->numNodes);

	// finally, rerun FK pipeline (skinning optional)
	a3kinematicsUpdateHierarchyStateFK(activeHS, baseHS, poseGroup);
	a3kinematicsUpdateHierarchyStateSkin(activeHS, baseHS);
}

//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-3
//-----------------------------------------------------------------------------


void a3animation_update_animation_other(
	a3_Scene_Animation* scene, a3f64 const dt)
{
	a3ui32 sampleIndex0;
	a3f64 keyframeParam;
	a3f64 clipParam;

	a3clipControllerUpdate(scene->clipCtrl_morph, dt);
	sampleIndex0 = scene->clipPool->keyframe[scene->clipCtrl_morph->keyframeIndex].sampleIndex0;
	keyframeParam = scene->clipCtrl_morph->keyframeParam;
	clipParam = scene->clipCtrl_morph->clipParam;

	scene->morph_time = (a3f64)sampleIndex0 + keyframeParam;
	scene->obj_teapot->euler.z = a3trigValid_sind((a3real)keyframeParam * a3real_threesixty);
}

void a3animation_update_animation(
	a3_Scene_Animation* scene, a3f64 const dt,
	a3boolean const updateIK)
{
	a3animation_update_animation_skeletal(scene, dt, updateIK);

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
	a3kinematicsSolveForward(scene->sceneGraphState);
	a3hierarchyStateUpdateLocalInverse(scene->sceneGraphState);
	a3hierarchyStateUpdateObjectInverse(scene->sceneGraphState);
//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: REMOVE ME WHEN FK IMPLEMENTED
//-----------------------------------------------------------------------------
	//for (i = 0; i < animationMaxCount_sceneObject; ++i)
	//{
	//	//****HINT: consider this for one part of the core FK function - what does this do?
	//	scene->sceneGraphState->objectSpace->hpose_base[i].transformMat = scene->sceneGraphState->localSpace->hpose_base[i].transformMat;

	//	//****HINT: consider this for updating object-space inverses
	//	a3real4x4TransformInverse(
	//		scene->sceneGraphState->objectSpaceInv->hpose_base[i].transformMat.m,
	//		scene->sceneGraphState->objectSpace->hpose_base[i].transformMat.m);
	//}
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
//****TO-DO-ANIM-PREP-4: ADD SKELETAL GRAPHICS UPDATE
//-----------------------------------------------------------------------------

	// prepare and graphics data
	{
		a3ui32 const skeletonIndex = (a3ui32)(scene->obj_skeleton - scene->object_scene);
		a3ui32 const max_mats = sizeof(scene->display_main.mvp_joint) / sizeof(a3mat4);
		a3mat4 const mvp_obj = scene->matrixStack[skeletonIndex].modelViewProjectionMat;

		a3demo_updateHierarchyGraphics(
			scene->display_main.mvp_joint, scene->display_main.mvp_bone, scene->display_main.t_skin, scene->display_main.dq_skin,
			max_mats, mvp_obj, scene->hierarchyState_skel_final);
	}

//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: CONTROL
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
