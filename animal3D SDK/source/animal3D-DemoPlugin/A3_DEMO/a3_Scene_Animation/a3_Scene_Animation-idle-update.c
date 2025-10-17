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

void a3animation_update_animation_skeletal(
	a3_Scene_Animation* scene, a3f64 const dt,
	a3boolean const updateIK)
{
	a3ui32 h = 0;
	a3ui32 n = sizeof(scene->clipCtrl) / sizeof(*scene->clipCtrl);
	a3ui32 n_anim = 3;//controlled only
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
		a3_HierarchyState* activeHS_tree = &scene->hierarchyState_skel_blend[h];
		a3ui32 sampleIndex0, sampleIndex1;

		// update clip controller, keyframe lerp
		a3clipControllerUpdate(clipCtrl_fk, dt);
		sampleIndex0 = scene->clipPool->keyframe[clipCtrl_fk->keyframeIndex].sampleIndex0;
		sampleIndex1 = scene->clipPool->keyframe[clipCtrl_fk->keyframeIndex].sampleIndex1;
		a3hierarchyPoseLerp(activeHS_tree->animPose,
			poseGroup->hpose + sampleIndex0, poseGroup->hpose + sampleIndex1,
			(a3real)clipCtrl_fk->keyframeParam, activeHS_tree->hierarchy->numNodes);
	}

	// do blending here
	//	-> interpolate idle_f/idle_m -> idle_fm
	//	-> interpolate idle_fm/idle_p -> result
	a3hierarchyPoseLerp(scene->hierarchyState_skel_blend_idle_fm_blend->animPose,	// dst: idle_fm
		scene->hierarchyState_skel_blend_idle_f->animPose,							// src(0): idle_f
		scene->hierarchyState_skel_blend_idle_m->animPose,							// src(1): idle_m
		a3real_half, activeHS->hierarchy->numNodes);
	a3hierarchyPoseLerp(scene->hierarchyState_skel_blend_result->animPose,			// dst: blend tree result
		scene->hierarchyState_skel_blend_idle_fm_blend->animPose,					// src(0): idle_fm
		scene->hierarchyState_skel_blend_idle_p->animPose,							// src(1): idle_p
		a3real_half, activeHS->hierarchy->numNodes);

	// resolve final FK state:
	// copy result to fk and run FK pipeline
	a3hierarchyPoseCopy(activeHS_fk->animPose,				// dst: FK anim
		scene->hierarchyState_skel_blend_result->animPose,	// src: blend tree result
		activeHS_fk->hierarchy->numNodes);
	a3kinematicsUpdateHierarchyStateFK(activeHS_fk, baseHS, poseGroup);

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
	//	scene->hierarchyState_skel_blend_result->animPose,	// src test(1): blend target
		blend_fk_ik, activeHS->hierarchy->numNodes);

	// finally, rerun FK pipeline (skinning optional)
	a3kinematicsUpdateHierarchyStateFK(activeHS, baseHS, poseGroup);
	a3kinematicsUpdateHierarchyStateSkin(activeHS, baseHS);
}


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
	
	// prepare and graphics data
	{
		a3ui32 const skeletonIndex = (a3ui32)(scene->obj_skeleton - scene->object_scene);
		a3ui32 const max_mats = sizeof(scene->display_main.mvp_joint) / sizeof(a3mat4);
		a3mat4 const mvp_obj = scene->matrixStack[skeletonIndex].modelViewProjectionMat;
		a3ui32 const n_nodes = sizeof(scene->hierarchyState_skel_blend) / sizeof(*scene->hierarchyState_skel_blend);

		a3demo_updateHierarchyGraphics(
			scene->display_main.mvp_joint, scene->display_main.mvp_bone, scene->display_main.t_skin, scene->display_main.dq_skin,
			max_mats, mvp_obj, scene->hierarchyState_skel_final);

		// optionally update intermediate states FK for rendering
		if (demoState->displayHiddenVolumes && updateBlendTreeFK)
		{
			a3mat4 mvp_sub, local_sub = a3mat4_identity;

			// idle_p, idle_f, idle_m, idle_fm, result
			a3real3 const offset_debug[] = { { +5, +10, 0 }, { 0, +15, 0 }, { -10, +15, 0 }, { -5, +10, 0 }, { 0, +5, 0 } };
			a3_HierarchyState const* baseHS = scene->hierarchyState_skel_base;
			a3_HierarchyPoseGroup const* poseGroup = scene->hierarchyPoseGroup_skel;

			for (i = 0; i < n_nodes; ++i)
			{
				a3_HierarchyState* activeHS_tree = &scene->hierarchyState_skel_blend[i];

				a3kinematicsUpdateHierarchyStateFK(activeHS_tree, baseHS, poseGroup);
				a3kinematicsUpdateHierarchyStateSkin(activeHS_tree, baseHS);

				a3real3SetReal3(local_sub.v3.v, offset_debug[i]);
				a3real4x4Product(mvp_sub.m, mvp_obj.m, local_sub.m);
				if (activeHS_tree->hierarchy && activeHS_tree->hierarchy->nodes)
				{
					a3demo_updateHierarchyGraphics(
						scene->display_tree[i].mvp_joint, scene->display_tree[i].mvp_bone, scene->display_tree[i].t_skin, scene->display_tree[i].dq_skin,
						max_mats, mvp_sub, activeHS_tree);
				}
			}
		}
	}

	// testing: reset IK effectors to lock them to FK result
	{
		//void a3animation_load_resetEffectors(a3_Scene_Animation * scene,
		//	a3_HierarchyState * hierarchyState, a3_HierarchyPoseGroup const* poseGroup);
		//a3animation_load_resetEffectors(scene,
		//	scene->hierarchyState_skel_final, scene->hierarchyPoseGroup_skel);
	}

	// process input
/*	switch (scene->ctrl_target)
	{
	case animation_ctrl_camera:
		// do nothing
		break;
	case animation_ctrl_character:
		// apply integration method
		switch (scene->ctrl_position)
		{
		case animation_input_direct:
			scene->vel = scene->acc = a3vec2_zero;
			a3real2Set(scene->pos.v,
				a3real_four * (a3real)scene->axis_l[0],
				a3real_four * (a3real)scene->axis_l[1]);
			break;
		case animation_input_euler:
			scene->acc = a3vec2_zero;
			a3real2Set(scene->vel.v,
				a3real_four * (a3real)scene->axis_l[0],
				a3real_four * (a3real)scene->axis_l[1]);
			a3demo_integrateEuler2(scene->pos.v, scene->pos.v, scene->vel.v, dtr);
			break;
		case animation_input_kinematic:
			a3real2Set(scene->acc.v,
				a3real_four * (a3real)scene->axis_l[0],
				a3real_four * (a3real)scene->axis_l[1]);
			a3demo_integrateKinematic2(scene->pos.v, scene->pos.v, scene->vel.v, scene->acc.v, dtr);
			a3demo_integrateEuler2(scene->vel.v, scene->vel.v, scene->acc.v, dtr);
			break;
		case animation_input_interpolate1:
			scene->acc = a3vec2_zero;
			a3real2Set(scene->vel.v,
				a3real_four * (a3real)scene->axis_l[0],
				a3real_four* (a3real)scene->axis_l[1]);
			a3real2Lerp(scene->pos.v, scene->pos.v, scene->vel.v, a3real_half);
			break;
		case animation_input_interpolate2:
			a3real2Set(scene->acc.v,
				a3real_four * (a3real)scene->axis_l[0],
				a3real_four * (a3real)scene->axis_l[1]);
			a3real2Lerp(scene->vel.v, scene->vel.v, scene->acc.v, a3real_half);
			a3demo_integrateEuler2(scene->pos.v, scene->pos.v, scene->vel.v, dtr);
			break;
		}
		// process rotation
		switch (scene->ctrl_rotation)
		{
		case animation_input_direct:
			scene->velr = scene->accr = a3real_zero;
			scene->rot = a3real_oneeighty * (a3real)scene->axis_r[0];
			break;
		case animation_input_euler:
			scene->accr = a3real_zero;
			scene->velr = a3real_oneeighty * (a3real)scene->axis_r[0];
			scene->rot = a3demo_integrateEuler(scene->rot, scene->velr, dtr);
			break;
		case animation_input_kinematic:
			scene->accr = a3real_oneeighty * (a3real)scene->axis_r[0];
			scene->rot = a3demo_integrateKinematic(scene->rot, scene->velr, scene->accr, dtr);
			scene->velr = a3demo_integrateEuler(scene->velr, scene->accr, dtr);
			break;
		case animation_input_interpolate1:
			scene->accr = a3real_zero;
			scene->velr = a3real_oneeighty * (a3real)scene->axis_r[0];
			scene->rot = a3lerpFunc(scene->rot, scene->velr, a3real_half);
			break;
		case animation_input_interpolate2:
			scene->accr = a3real_oneeighty * (a3real)scene->axis_r[0];
			scene->velr = a3lerpFunc(scene->velr, scene->accr, a3real_half);
			scene->rot = a3demo_integrateEuler(scene->rot, scene->velr, dtr);
			break;
		}
		break;
	}

	// apply input
	scene->obj_skeleton_ctrl->position.x = +(scene->pos.x);
	scene->obj_skeleton_ctrl->position.y = +(scene->pos.y);
	scene->obj_skeleton_ctrl->euler.z = -a3trigValid_sind(scene->rot);
*/
}


//-----------------------------------------------------------------------------
