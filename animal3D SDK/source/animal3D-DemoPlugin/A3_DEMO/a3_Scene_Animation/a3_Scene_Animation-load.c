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

	a3_Scene_Animation-load.c/.cpp
	Demo mode implementations: animation scene.

	********************************************
	*** LOADING FOR ANIMATION SCENE MODE     ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Scene_Animation.h"

#include "../a3_DemoState.h"


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: ADD BLENDING
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

void a3animation_load_resetEffectors(a3_Scene_Animation* scene,
	a3_HierarchyState* hierarchyState, a3_HierarchyPoseGroup const* poseGroup)
{
	a3_SceneObject* sceneObject = scene->obj_skeleton;
	a3ui32 j = sceneObject->sceneGraphIndex;

	// need to properly transform joints to their parent frame
	a3mat4 const skeletonToControl = scene->sceneGraphState->localSpace->hpose_base[j].transformMat;
	a3vec4 controlLocator;

	// bail
	if (!hierarchyState->hierarchy)
		return;

	// look-at effector
	// position in front of neck
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:Neck");
	sceneObject = scene->obj_skeleton_neckLookat_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x;
	sceneObject->position.y = controlLocator.y + a3real_four;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right wrist effector
	// position above wrist
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:RightHand");
	sceneObject = scene->obj_skeleton_wristEffector_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x + a3real_one;
	sceneObject->position.y = controlLocator.y + a3real_one;
	sceneObject->position.z = controlLocator.z + a3real_one;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right wrist constraint
	// position behind elbow
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:RightForeArm");
	sceneObject = scene->obj_skeleton_wristConstraint_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x + a3real_half;
	sceneObject->position.y = controlLocator.y - a3real_half;
	sceneObject->position.z = controlLocator.z + a3real_half;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// left wrist effector
	// position above wrist
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:LeftHand");
	sceneObject = scene->obj_skeleton_wristEffector_l_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x - a3real_one;
	sceneObject->position.y = controlLocator.y + a3real_one;
	sceneObject->position.z = controlLocator.z + a3real_one;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// left wrist constraint
	// position behind elbow
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:LeftForeArm");
	sceneObject = scene->obj_skeleton_wristConstraint_l_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x - a3real_half;
	sceneObject->position.y = controlLocator.y - a3real_half;
	sceneObject->position.z = controlLocator.z + a3real_half;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right ankle effector
	// position on ankle
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:RightFoot");
	sceneObject = scene->obj_skeleton_ankleEffector_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x + a3real_half;
	sceneObject->position.y = controlLocator.y;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// right ankle constraint
	// position in front of knee
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:RightLeg");
	sceneObject = scene->obj_skeleton_ankleConstraint_r_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x + a3real_quarter;
	sceneObject->position.y = controlLocator.y + a3real_half;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// left ankle effector
	// position on ankle
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:LeftFoot");
	sceneObject = scene->obj_skeleton_ankleEffector_l_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x - a3real_half;
	sceneObject->position.y = controlLocator.y;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;

	// left ankle constraint
	// position in front of knee
	j = a3hierarchyGetNodeIndex(scene->hierarchy_skel, "mixamorig:LeftLeg");
	sceneObject = scene->obj_skeleton_ankleConstraint_l_ctrl;
	a3real4Real4x4Product(controlLocator.v, skeletonToControl.m,
		hierarchyState->objectSpace->hpose_base[j].transformMat.v3.v);
	sceneObject->position.x = controlLocator.x - a3real_quarter;
	sceneObject->position.y = controlLocator.y + a3real_half;
	sceneObject->position.z = controlLocator.z;
	sceneObject->scale.x = a3real_third;
	sceneObject->scaleMode = 1;
}

// utility to load animation
void a3animation_init_animation(a3_DemoState const* demoState, a3_Scene_Animation* scene)
{
	// general counters
	a3ui32 j, p;


	a3_FileStream fileStream[1] = { 0 };
	const a3byte* const animationStream = "./data/gpro25_base_anim_active_3.dat";
	const a3boolean force_disable_streaming = false;

	
	// object pointers
	a3_Hierarchy* hierarchy = 0;
	a3_HierarchyState* hierarchyState = 0;
	a3_HierarchyPoseGroup* hierarchyPoseGroup = 0;
	a3_SpatialPose* spatialPose = 0;

	a3boolean const using_character = true;//using xbot character; otherwise use dummy
	a3boolean hierarchy_loaded = false;

	// next set up hierarchy poses
	hierarchy = scene->hierarchy_skel;
	hierarchyPoseGroup = scene->hierarchyPoseGroup_skel;

	// clip info for character
	a3byte const* clipName[] = {
		"xbot",
		"xbot_base",
		"xbot_ymca",
		"xbot_idle_pistol",
		"xbot_skintest",
		"xbot_gangnam",
		"xbot_samba",
		"xbot_backflip",
		"xbot_idle_f",
		"xbot_jump_f",
		"xbot_walk_f",
		"xbot_run_f",
		"xbot_walk_s_l_f",
		"xbot_strafe_l_f",
		"xbot_turn_l_f",
		"xbot_walk_s_r_f",
		"xbot_strafe_r_f",
		"xbot_turn_r_f",
		"xbot_idle_m",
		"xbot_jump_m",
		"xbot_walk_m",
		"xbot_run_m",
		"xbot_walk_s_l_m",
		"xbot_strafe_l_m",
		"xbot_turn_l_m",
		"xbot_walk_s_r_m",
		"xbot_strafe_r_m",
		"xbot_turn_r_m",
	};
	a3ui32 const hierarchyClipCount = sizeof(clipName) / sizeof(*clipName);


	// stream animation assets
	const a3boolean streaming = demoState->streaming & !force_disable_streaming;
	if (streaming && a3fileStreamOpenRead(fileStream, animationStream))
	{
		// load hierarchy assets
		a3hierarchyLoadBinary(hierarchy, fileStream);
		hierarchyPoseGroup->hierarchy = hierarchy;
		if (a3hierarchyPoseGroupLoadBinary(hierarchyPoseGroup, fileStream) >= 0)
		{
			hierarchy_loaded = hierarchyPoseGroup->hposeCount > 0;
		}

		// load scene graph
		a3hierarchyLoadBinary(scene->sceneGraph, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}
	// not streaming or stream doesn't exist
	else if (a3fileStreamOpenWrite(fileStream, animationStream))
	{
		// set up scenegraph
		a3hierarchyCreate(scene->sceneGraph, animationMaxCount_sceneObject, 0);
		a3hierarchySetNode(scene->sceneGraph,  0, -1, "scene_world_root");
		a3hierarchySetNode(scene->sceneGraph,  1, 0, "scene_camera_main");
		a3hierarchySetNode(scene->sceneGraph,  2, 0, "scene_light_main");
		a3hierarchySetNode(scene->sceneGraph,  3, 0, "scene_skybox");
		a3hierarchySetNode(scene->sceneGraph,  4, 0, "scene_teapot");

		a3hierarchySetNode(scene->sceneGraph,  5, 0, "scene_skeleton_ctrl");
		a3hierarchySetNode(scene->sceneGraph,  6, 5, "scene_skeleton_rig");
		a3hierarchySetNode(scene->sceneGraph,  7, 6, "scene_skeleton_neckLookat_ctrl");
		a3hierarchySetNode(scene->sceneGraph,  8, 6, "scene_skeleton_wristEff_r_ctrl");
		a3hierarchySetNode(scene->sceneGraph,  9, 6, "scene_skeleton_wristCon_r_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 10, 6, "scene_skeleton_wristEff_l_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 11, 6, "scene_skeleton_wristCon_l_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 12, 6, "scene_skeleton_ankleEff_r_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 13, 6, "scene_skeleton_ankleCon_r_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 14, 6, "scene_skeleton_ankleEff_l_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 15, 6, "scene_skeleton_ankleCon_l_ctrl");
		a3hierarchySetNode(scene->sceneGraph, 16, 5, "scene_skeleton");
		
		// manually set up a skeleton
		if (!using_character)
		{
			// first is the hierarchy: the general non-spatial relationship between bones
			const a3ui32 jointCount = 32;

			// indices of joints, their parents and branching joints
			a3ui32 jointIndex = 0;
			a3i32 jointParentIndex = -1;
			a3i32 rootJointIndex, upperSpineJointIndex, clavicleJointIndex, pelvisJointIndex;

			// initialize hierarchy
			hierarchy = scene->hierarchy_skel;
			a3hierarchyCreate(hierarchy, jointCount, 0);

			// set up joint relationships
			jointParentIndex = rootJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:root");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_lower");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_mid");
			jointParentIndex = upperSpineJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:spine_upper");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:neck");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:head");
			jointParentIndex = upperSpineJointIndex;
			jointParentIndex = clavicleJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:clavicle");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_r");
			jointParentIndex = clavicleJointIndex;
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulderblade_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shoulder_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:elbow_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:forearm_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:wrist_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hand_l");
			jointParentIndex = rootJointIndex;
			jointParentIndex = pelvisJointIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:pelvis");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_r");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_r");
			jointParentIndex = pelvisJointIndex;
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:hip_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:knee_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:shin_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:ankle_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:foot_l");
			jointParentIndex = a3hierarchySetNode(hierarchy, jointIndex++, jointParentIndex, "skel:toe_l");


			// allocate poses
			a3hierarchyPoseGroupCreate(hierarchyPoseGroup, hierarchy, 5);

			// define "bind pose" or "base pose" or the initial transformation
			//	description for each joint (not a literal transform)
			p = 0;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, +3.6f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz | a3poseChannel_scale_xyz | a3poseChannel_translate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_lower");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -5.0f);
			a3spatialPoseSetTranslation(spatialPose, 0.0f, -0.1f, +0.1f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_mid");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:spine_upper");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -5.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:neck");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:head");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:clavicle");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, 0.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.1f, +0.1f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, -0.1f, -0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +30.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulderblade_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, -0.5f, -0.1f, -0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_translate_yz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shoulder_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -30.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:elbow_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
			a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:forearm_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:wrist_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hand_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:pelvis");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, 0.0f, 0.0f, -0.1f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, +90.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, -0.1f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -20.0f);
			a3spatialPoseSetTranslation(spatialPose, +2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +90.0f);
			a3spatialPoseSetTranslation(spatialPose, +1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +10.0f);
			a3spatialPoseSetTranslation(spatialPose, +0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_r");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +0.2f, 0.0f, 0.0f);

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:hip_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, -90.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, -0.1f, +0.5f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:knee_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, +20.0f);
			a3spatialPoseSetTranslation(spatialPose, -2.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:shin_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_x;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:ankle_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -90.0f);
			a3spatialPoseSetTranslation(spatialPose, -1.0f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_xyz;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:foot_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, 0.0f, 0.0f, -10.0f);
			a3spatialPoseSetTranslation(spatialPose, -0.5f, 0.0f, 0.0f);
			hierarchyPoseGroup->channel[j] = a3poseChannel_rotate_z;

			j = a3hierarchyGetNodeIndex(hierarchy, "skel:toe_l");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, -0.2f, 0.0f, 0.0f);


			// each remaining pose represents a "delta" from the base
			// initialize the changes where applicable
			// (note: the channels describe which pose components can change)
			p = 2;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetRotation(spatialPose, +45.0f, +60.0f, +90.0f);	// rotate whole figure about all axes


			p = 3;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetScale(spatialPose, 2.0f, 2.0f, 2.0f);	// uniformly scale whole figure up to 200%


			p = 4;
			j = a3hierarchyGetNodeIndex(hierarchy, "skel:root");
			spatialPose = hierarchyPoseGroup->hpose[p].hpose_base + j;
			a3spatialPoseSetTranslation(spatialPose, +3.0f, +4.0f, +5.0f);	// shift whole figure by some vector

			hierarchy_loaded = true;
		}
		// load skeletal data from file
		else
		{
			hierarchyPoseGroup->hierarchy = 0;
			if (a3hierarchyPoseGroupLoadHTR(hierarchyPoseGroup, hierarchy,
				//"../../../../resource/animdata/egnaro/egnaro_skel_anim.htr");
				"../../../../resource/animdata/xbot/xbot_loco.htr") >= 0)
			{
				hierarchy_loaded = hierarchyPoseGroup->hposeCount > 0;
			}

			// edit assets as needed
			// mixamo assets have the wrong base pose; use first key as base and subtract from all
			p = 1;
			a3hierarchyPoseCopy(hierarchyPoseGroup->hpose, hierarchyPoseGroup->hpose + p, hierarchy->numNodes);
			for (; p < hierarchyPoseGroup->hposeCount; ++p)
				a3hierarchyPoseDeconcat(hierarchyPoseGroup->hpose + p, hierarchyPoseGroup->hpose + p,
					hierarchyPoseGroup->hpose, hierarchy->numNodes);

			// furthermore, end joints were removed, so they have no animation data; initialize it as identity
			for (j = a3hierarchyGetNodeIndex(hierarchy, "HeadTop_End"), p = 1;
				p < hierarchyPoseGroup->hposeCount; ++p)
				a3spatialPoseReset(hierarchyPoseGroup->hpose[p].hpose_base + j);
			for (j = a3hierarchyGetNodeIndex(hierarchy, "LeftToe_End"), p = 1;
				p < hierarchyPoseGroup->hposeCount; ++p)
				a3spatialPoseReset(hierarchyPoseGroup->hpose[p].hpose_base + j);
			for (j = a3hierarchyGetNodeIndex(hierarchy, "RightToe_End"), p = 1;
				p < hierarchyPoseGroup->hposeCount; ++p)
				a3spatialPoseReset(hierarchyPoseGroup->hpose[p].hpose_base + j);

			// finally, append prefix names to match what is expected for skinning
			a3hierarchyPrefixNodeNames(scene->hierarchy_skel, "mixamorig:");
		}

		// save hierarchy assets
		a3hierarchySaveBinary(hierarchy, fileStream);
		a3hierarchyPoseGroupSaveBinary(hierarchyPoseGroup, fileStream);

		// save scene graph
		a3hierarchySaveBinary(scene->sceneGraph, fileStream);

		// done
		a3fileStreamClose(fileStream);
	}


	// map relevant objects to scene graph
	scene->obj_world_root->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_world_root");
	scene->obj_camera_main->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_camera_main");
	scene->obj_light_main->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_light_main");
	scene->obj_skybox->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skybox");
	scene->obj_teapot->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_teapot");
	
	scene->obj_skeleton_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_ctrl");
	scene->obj_skeleton_rig->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_rig");
	scene->obj_skeleton_neckLookat_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_neckLookat_ctrl");
	scene->obj_skeleton_wristEffector_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_wristEff_r_ctrl");
	scene->obj_skeleton_wristConstraint_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_wristCon_r_ctrl");
	scene->obj_skeleton_wristEffector_l_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_wristEff_l_ctrl");
	scene->obj_skeleton_wristConstraint_l_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_wristCon_l_ctrl");
	scene->obj_skeleton_ankleEffector_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_ankleEff_r_ctrl");
	scene->obj_skeleton_ankleConstraint_r_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_ankleCon_r_ctrl");
	scene->obj_skeleton_ankleEffector_l_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_ankleEff_l_ctrl");
	scene->obj_skeleton_ankleConstraint_l_ctrl->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton_ankleCon_l_ctrl");
	scene->obj_skeleton->sceneGraphIndex = a3hierarchyGetNodeIndex(scene->sceneGraph, "scene_skeleton");

	// scene graph state
	scene->sceneGraphState->hierarchy = 0;
	a3hierarchyStateCreate(scene->sceneGraphState, scene->sceneGraph);

	// initialize clip pool
	a3ui32 const hierarchySampleCount = hierarchyPoseGroup->hposeCount ? hierarchyPoseGroup->hposeCount : 1;
	a3ui32 const hierarchyKeyframeCount = hierarchySampleCount - 1;
	a3ui32 const additionalSampleCount = 6;
	a3ui32 const additionalKeyframeCount = additionalSampleCount - 1;
	a3ui32 const additionalClipCount = 1;
	a3clipPoolCreate(scene->clipPool,
		hierarchyClipCount + additionalClipCount,
		hierarchyKeyframeCount + additionalKeyframeCount,
		hierarchySampleCount + additionalSampleCount);
	
	// clips and controllers
	if (using_character && hierarchy_loaded)
	{
		a3ui32 const sampleIndexFirst[] = {
			1,1,25,134,167,222,519,1092,1233,1434,1475,1499,1517,1540,1557,1586,1609,1626,1655,1856,1909,1935,1953,1979,1996,2019,2045,2062,
		};
		a3ui32 const sampleIndexFinal[] = {
			2084,24,133,166,221,518,1091,1232,1433,1474,1498,1516,1539,1556,1585,1608,1625,1654,1855,1908,1934,1952,1978,1995,2018,2044,2061,2084,
		};
		a3ui32 const rate_hierarchy = 24;

		// initialize hierarchy clips and controllers
		a3f64 const fps_hierarchy = (a3f64)rate_hierarchy;
		for (j = 0; j < hierarchySampleCount; ++j)
			a3sampleInit(scene->clipPool->sample + j, j, fps_hierarchy);
		for (j = 0; j < hierarchyKeyframeCount; ++j)
			a3keyframeInit(scene->clipPool->keyframe + j,
				scene->clipPool->sample + j, scene->clipPool->sample + j + 1, fps_hierarchy);
		for (j = 0; j < hierarchyClipCount; ++j)
		{
			a3clipInit(scene->clipPool->clip + j, clipName[j],
				scene->clipPool->keyframe + sampleIndexFirst[j],
				scene->clipPool->keyframe + sampleIndexFinal[j] - 1);
			a3clipCalculateDuration(scene->clipPool, j, fps_hierarchy);
		}

		j = a3clipGetIndexInPool(scene->clipPool, "xbot_idle_f");
		a3clipControllerInit(scene->clipCtrl_idle_f, "xbot_ctrl_idle_f", scene->clipPool, j, rate_hierarchy, fps_hierarchy);
		j = a3clipGetIndexInPool(scene->clipPool, "xbot_idle_m");
		a3clipControllerInit(scene->clipCtrl_idle_m, "xbot_ctrl_idle_m", scene->clipPool, j, rate_hierarchy, fps_hierarchy);
		j = a3clipGetIndexInPool(scene->clipPool, "xbot_idle_pistol");
		a3clipControllerInit(scene->clipCtrl_idle_p, "xbot_ctrl_idle_p", scene->clipPool, j, rate_hierarchy, fps_hierarchy);
	}

	// initialize additional clips and controllers
	{
		a3ui32 const rate_additional = 30;
		a3f64 const fps_additional = (a3f64)rate_additional;
		a3ui32 const additionalSampleStart = hierarchySampleCount;
		a3ui32 const additionalKeyframeStart = hierarchyKeyframeCount;
		a3ui32 const additionalClipStart = hierarchyClipCount;
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 0],   0, fps_additional);//0.0s
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 1],  15, fps_additional);//0.5s
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 2],  75, fps_additional);//2.5s
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 3], 105, fps_additional);//3.5s
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 4], 120, fps_additional);//4.0s
		a3sampleInit(&scene->clipPool->sample[additionalSampleStart + 5], 150, fps_additional);//5.0s
		for (j = 0; j < additionalKeyframeCount; ++j)
			a3keyframeInit(&scene->clipPool->keyframe[additionalKeyframeStart + j],
				&scene->clipPool->sample[additionalSampleStart + j], &scene->clipPool->sample[additionalSampleStart + j + 1], fps_additional);

		j = additionalClipStart;
		a3clipInit(&scene->clipPool->clip[j], "teapot_morph",
			&scene->clipPool->keyframe[additionalKeyframeStart + 0],
			&scene->clipPool->keyframe[additionalKeyframeStart + 4]);
		a3clipCalculateDuration(scene->clipPool, j, fps_additional);

		j = a3clipGetIndexInPool(scene->clipPool, "teapot_morph");
		a3clipControllerInit(scene->clipCtrl_morph, "teapot_ctrl_morph", scene->clipPool, j, rate_additional, fps_additional);
		scene->morph_time = 0.0;
	}
	
	// finally set up hierarchy states
	// base state for skeleton
	hierarchyState = scene->hierarchyState_skel_base;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);
	a3hierarchyPoseCopy(hierarchyState->localSpace, hierarchyPoseGroup->hpose, hierarchy->numNodes);
	a3hierarchyPoseConvert(hierarchyState->localSpace, hierarchy->numNodes, hierarchyPoseGroup->channel, hierarchyPoseGroup->order);
	a3kinematicsSolveForward(hierarchyState);
	a3hierarchyStateUpdateLocalInverse(hierarchyState);
	a3hierarchyStateUpdateObjectInverse(hierarchyState);

	// FK state
	hierarchyState = scene->hierarchyState_skel_fk;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);

	// IK state
	hierarchyState = scene->hierarchyState_skel_ik;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);

	// final blend state
	hierarchyState = scene->hierarchyState_skel_final;
	hierarchyState->hierarchy = 0;
	a3hierarchyStateCreate(hierarchyState, hierarchy);

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: BLENDING
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------
	
	// control node
	scene->obj_skeleton_ctrl->euler.z = a3real_oneeighty;
	scene->obj_skeleton_ctrl->position.y = +a3real_four;
	
	// xbot
	scene->obj_skeleton->euler.x = a3real_ninety;
	scene->obj_skeleton->euler.y = a3real_oneeighty;

	// teapot
	scene->obj_teapot->position.y = -a3real_six;
	scene->obj_teapot->scale.x = a3real_half;
	scene->obj_teapot->scaleMode = 1;


	// effectors
	// do one update to get first pose for target IK frame
	{
		void a3animation_update_animation(a3_Scene_Animation* scene, a3f64 const dt, a3boolean const updateIK);
		void a3animation_update_sceneGraph(a3_Scene_Animation* scene, a3f64 const dt);
		for (p = 0; p < 3; ++p)
		{
			a3animation_update_animation(scene, 0.0, false);
			a3animation_update_sceneGraph(scene, 0.0);

			a3animation_load_resetEffectors(scene, scene->hierarchyState_skel_fk, hierarchyPoseGroup);
		}
	}
}


//-----------------------------------------------------------------------------

void a3animation_input(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt);
void a3animation_update(a3_DemoState* demoState, a3_Scene_Animation* scene, a3f64 const dt);
void a3animation_render(a3_DemoState const* demoState, a3_Scene_Animation const* scene, a3f64 const dt);
void a3animation_input_keyCharPress(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state);
void a3animation_input_keyCharHold(a3_DemoState const* demoState, a3_Scene_Animation* scene, a3i32 const asciiKey, a3i32 const state);

void a3animation_loadValidate(a3_DemoState* demoState, a3_Scene_Animation* scene)
{
	// initialize callbacks
	a3_SceneCallbacks* const callbacks = demoState->sceneCallbacks + demoState_modeAnimation;
	callbacks->scene = scene;
	callbacks->handleInput =	(a3_Scene_EventCallback)		a3animation_input;
	callbacks->handleUpdate =	(a3_Scene_EventCallback)		a3animation_update;
	callbacks->handleRender =	(a3_Scene_EventCallbackConst)	a3animation_render;
	callbacks->handleKeyPress = (a3_Scene_InputCallback)		a3animation_input_keyCharPress;
	callbacks->handleKeyHold =	(a3_Scene_InputCallback)		a3animation_input_keyCharHold;

	// initialize cameras dependent on viewport
	scene->proj_camera_main->aspect = demoState->frameAspect;
	a3scene_updateProjectorProjectionMat(scene->proj_camera_main, true);
	a3scene_setProjectorSceneObject(scene->proj_camera_main, scene->obj_camera_main);
	// initialize cameras not dependent on viewport

	a3clipControllerRefresh(scene->clipCtrl_morph, scene->clipPool);

	a3ui32 i, j;
	a3ui32 const n_hierarchy = sizeof(scene->hierarchyState_skel) / sizeof(a3_HierarchyState);
	
	// animation
	scene->sceneGraphState->hierarchy = scene->sceneGraph;
	scene->hierarchyPoseGroup_skel->hierarchy = scene->hierarchy_skel;
	for (i = 0, j = sizeof(scene->clipCtrl) / sizeof(*scene->clipCtrl); i < j; ++i)
		a3clipControllerRefresh(&scene->clipCtrl[i], scene->clipPool);
	for (i = 0, j = n_hierarchy; i < j; ++i)
		scene->hierarchyState_skel[i].hierarchy = scene->hierarchy_skel;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: INIT BLEND
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------
}


void a3animation_load(a3_DemoState const* demoState, a3_Scene_Animation* scene)
{
	a3ui32 i;

	a3_SceneObject* currentSceneObject;
	a3_SceneProjector* projector;


	// camera's starting orientation depends on "vertical" axis
	// we want the exact same view in either case
	const a3real sceneCameraAxisPos = 20.0f;
	const a3vec3 sceneCameraStartPos = {
		+sceneCameraAxisPos,
		-sceneCameraAxisPos,
		+sceneCameraAxisPos + 5.0f,
	};
	const a3vec3 sceneCameraStartEuler = {
		-35.0f,
		  0.0f,
		 45.0f,
	};
	const a3f32 sceneObjectDistance = 8.0f;
	const a3f32 sceneObjectHeight = 2.0f;


	// all objects
	for (i = 0; i < animationMaxCount_sceneObject; ++i)
		a3scene_initSceneObject(scene->object_scene + i);
	for (i = 0; i < animationMaxCount_projector; ++i)
		a3scene_initProjector(scene->projector + i);

	currentSceneObject = scene->obj_skybox;
	currentSceneObject->scaleMode = 1;
	currentSceneObject->scale.x = 256.0f;

	currentSceneObject = scene->obj_skeleton;
	currentSceneObject->scaleMode = 0;

	// set up cameras
	projector = scene->proj_camera_main;
	projector->perspective = a3true;
	projector->fovy = a3real_fortyfive;
	projector->znear = 1.0f;
	projector->zfar = 1024.0f;
	projector->ctrlMoveSpeed = 10.0f;
	projector->ctrlRotateSpeed = 5.0f;
	projector->ctrlZoomSpeed = 5.0f;
	projector->sceneObject->position = sceneCameraStartPos;
	projector->sceneObject->euler = sceneCameraStartEuler;


	// set flags
	scene->render = animation_renderPhong;
	scene->display = animation_displayTexture;
	scene->activeCamera = animation_cameraSceneViewer;

	scene->pipeline = animation_forward;
	scene->pass = animation_passComposite;

	scene->targetIndex[animation_passScene] = animation_scene_finalcolor;
	scene->targetIndex[animation_passComposite] = animation_scene_finalcolor;

	scene->targetCount[animation_passScene] = animation_target_scene_max;
	scene->targetCount[animation_passComposite] = 1;


	// setup
	a3animation_init_animation(demoState, scene);
}


//-----------------------------------------------------------------------------
