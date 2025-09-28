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

void a3animation_update_applyEffectors(a3_Scene_Animation* scene,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// hierarchy object
		a3_SceneObject const* sceneObjectRoot = scene->obj_skeleton;
		a3_Basis const basis_obj = a3basisInit(basis_yp, basis_zp);

		// NECK LOOK-AT
		{
			// look-at effector object
			a3_SceneObject const* sceneObject_effector = scene->obj_skeleton_neckLookat_ctrl;

			// affected node
			a3ui32 const j_neck = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:Neck");
			a3_Basis const basis_neck = a3basisInit(basis_zp, basis_yp);

			// invoke IK
			a3kinematicsUpdateLookAtIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
				sceneObjectRoot->sceneGraphIndex, sceneObject_effector->sceneGraphIndex,
				j_neck, basis_obj, basis_neck);
		}

		// RIGHT ARM REACH
		{
			// right wrist effector object
			a3_SceneObject const* sceneObject_wristEffector = scene->obj_skeleton_wristEffector_r_ctrl;

			// write wrist constraint object
			a3_SceneObject const* sceneObject_wristConstraint = scene->obj_skeleton_wristConstraint_r_ctrl;

			// affected end node
			a3ui32 const j_wrist = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightHand");
			a3_Basis const basis_wrist = a3basisInit(basis_zp, basis_yp);

			// affected hinge node
			a3ui32 const j_elbow = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightForeArm");
			a3_Basis const basis_elbow = a3basisInit(basis_xn, basis_yp);

			// affected base node
			a3ui32 const j_shoulder = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightArm");
			a3_Basis const basis_shoulder = basis_elbow;

			// invoke IK
			a3kinematicsUpdateLimbIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
				sceneObjectRoot->sceneGraphIndex, sceneObject_wristEffector->sceneGraphIndex, sceneObject_wristConstraint->sceneGraphIndex,
				j_wrist, j_elbow, j_shoulder, basis_obj, basis_wrist, basis_elbow, basis_shoulder);
		}

		// LEFT ARM REACH
		{
			// right wrist effector object
			a3_SceneObject const* sceneObject_wristEffector = scene->obj_skeleton_wristEffector_l_ctrl;

			// write wrist constraint object
			a3_SceneObject const* sceneObject_wristConstraint = scene->obj_skeleton_wristConstraint_l_ctrl;

			// affected end node
			a3ui32 const j_wrist = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftHand");
			a3_Basis const basis_wrist = a3basisInit(basis_zp, basis_yp);

			// affected hinge node
			a3ui32 const j_elbow = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftForeArm");
			a3_Basis const basis_elbow = a3basisInit(basis_xp, basis_yn);

			// affected base node
			a3ui32 const j_shoulder = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftArm");
			a3_Basis const basis_shoulder = basis_elbow;

			// invoke IK
			a3kinematicsUpdateLimbIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
				sceneObjectRoot->sceneGraphIndex, sceneObject_wristEffector->sceneGraphIndex, sceneObject_wristConstraint->sceneGraphIndex,
				j_wrist, j_elbow, j_shoulder, basis_obj, basis_wrist, basis_elbow, basis_shoulder);
		}

		// RIGHT LEG REACH
		{
			// right ankle effector object
			a3_SceneObject const* sceneObject_ankleEffector = scene->obj_skeleton_ankleEffector_r_ctrl;

			// write ankle constraint object
			a3_SceneObject const* sceneObject_ankleConstraint = scene->obj_skeleton_ankleConstraint_r_ctrl;

			// affected end node
			a3ui32 const j_ankle = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightFoot");
			a3_Basis const basis_ankle = a3basisInit(basis_zp, basis_yp);

			// affected hinge node
			a3ui32 const j_knee = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightLeg");
			a3_Basis const basis_knee = a3basisInit(basis_yn, basis_xp);

			// affected base node
			a3ui32 const j_hip = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:RightUpLeg");
			a3_Basis const basis_hip = basis_knee;

			// invoke IK
			a3kinematicsUpdateLimbIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
				sceneObjectRoot->sceneGraphIndex, sceneObject_ankleEffector->sceneGraphIndex, sceneObject_ankleConstraint->sceneGraphIndex,
				j_ankle, j_knee, j_hip, basis_obj, basis_ankle, basis_knee, basis_hip);
		}

		// LEFT LEG REACH
		{
			// right ankle effector object
			a3_SceneObject const* sceneObject_ankleEffector = scene->obj_skeleton_ankleEffector_l_ctrl;

			// write ankle constraint object
			a3_SceneObject const* sceneObject_ankleConstraint = scene->obj_skeleton_ankleConstraint_l_ctrl;

			// affected end node
			a3ui32 const j_ankle = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftFoot");
			a3_Basis const basis_ankle = a3basisInit(basis_zp, basis_yp);

			// affected hinge node
			a3ui32 const j_knee = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftLeg");
			a3_Basis const basis_knee = a3basisInit(basis_yn, basis_xp);

			// affected base node
			a3ui32 const j_hip = a3hierarchyGetNodeIndex(activeHS->hierarchy, "mixamorig:LeftUpLeg");
			a3_Basis const basis_hip = basis_knee;

			// invoke IK
			a3kinematicsUpdateLimbIK(scene->sceneGraphState, activeHS, baseHS, poseGroup,
				sceneObjectRoot->sceneGraphIndex, sceneObject_ankleEffector->sceneGraphIndex, sceneObject_ankleConstraint->sceneGraphIndex,
				j_ankle, j_knee, j_hip, basis_obj, basis_ankle, basis_knee, basis_hip);
		}
	}
}

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: ADD UPDATE LOGIC
//-----------------------------------------------------------------------------

void a3animation_update_animation_skeletal(
	a3_Scene_Animation* scene, a3f64 const dt,
	a3boolean const updateIK)
{
	a3ui32 h = 0;
	a3ui32 n = sizeof(scene->clipCtrl) / sizeof(*scene->clipCtrl);
	a3ui32 n_anim = 3;//controlled only
	a3ui32 sampleIndex0, sampleIndex1;

	// FK/IK blend
	//	-> both states are simultaneously available; select either or in-between
	a3real blend_fk_ik = a3real_one;// 0 = fk, 1 = ik

	a3_HierarchyState* activeHS_fk = scene->hierarchyState_skel_fk;
	a3_HierarchyState* activeHS_ik = scene->hierarchyState_skel_ik;
	a3_HierarchyState* activeHS = scene->hierarchyState_skel_final;
	a3_HierarchyState const* baseHS = scene->hierarchyState_skel_base;
	a3_HierarchyPoseGroup const* poseGroup = scene->hierarchyPoseGroup_skel;

	// bail
	if (!activeHS->hierarchy || !activeHS->hierarchy->nodes)
		return;

	// update all clip poses
	for (h = 0; h < n_anim; ++h)
	{
		a3_ClipController* clipCtrl_fk = &scene->clipCtrl[h];

		// update clip controller, keyframe lerp
		a3clipControllerUpdate(clipCtrl_fk, dt);
	}

	// select single clip
	{
		a3_ClipController const* clipCtrl = scene->clipCtrl_idle_f;
		sampleIndex0 = scene->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex0;
		sampleIndex1 = scene->clipPool->keyframe[clipCtrl->keyframeIndex].sampleIndex1;

		a3hierarchyPoseLerp(activeHS_fk->animPose,
			poseGroup->hpose + sampleIndex0, poseGroup->hpose + sampleIndex1,
			(a3real)clipCtrl->keyframeParam, activeHS_fk->hierarchy->numNodes);
		a3kinematicsUpdateHierarchyStateFK(activeHS_fk, baseHS, poseGroup);
	}

	// resolve final IK state
	// copy FK result to IK to begin IK pipeline
	// all joints not affected by IK will match FK state
	a3hierarchyPoseCopy(activeHS_ik->animPose,	// dst: IK anim
		activeHS_fk->animPose,					// src: FK anim
	//	baseHS->animPose,						// src test: base anim (identity)
		activeHS_ik->hierarchy->numNodes);
	a3kinematicsUpdateHierarchyStateFK(activeHS_ik, baseHS, poseGroup);

	// do full IK update
	if (updateIK)
	{
		// invert object-space
		a3hierarchyStateUpdateObjectInverse(activeHS_ik);

		// run solvers
		a3animation_update_applyEffectors(scene, activeHS_ik, baseHS, poseGroup);

		//// run full IK pipeline (if not resolving with effectors)
		//a3kinematicsUpdateHierarchyStateIK(activeHS_ik, baseHS, poseGroup);
	}

	// blend FK/IK to final
	// since IK was based on FK, this would only affect IK joints
	a3hierarchyPoseLerp(activeHS->animPose,	// dst: final anim
		activeHS_fk->animPose,				// src(0): FK anim
		activeHS_ik->animPose,				// src(1): IK anim
	//	baseHS->animPose,									// src test(1): base anim (identity)
		blend_fk_ik, activeHS->hierarchy->numNodes);

	// finally, rerun FK pipeline (skinning optional)
	a3kinematicsUpdateHierarchyStateFK(activeHS, baseHS, poseGroup);
	a3kinematicsUpdateHierarchyStateSkin(activeHS, baseHS);
}

//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
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
	//
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
