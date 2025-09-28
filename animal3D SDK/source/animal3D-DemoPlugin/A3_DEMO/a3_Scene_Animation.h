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

	a3_Scene_Animation.h
	Demo mode interface: animation scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_SCENE_ANIMATION_H
#define __ANIMAL3D_SCENE_ANIMATION_H


//-----------------------------------------------------------------------------

#include "_a3_scene_utilities/a3_SceneObject.h"

#include "_animation/a3_KeyframeAnimationController.h"
#include "_animation/a3_HierarchyStateBlend.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// maximum unique objects
	enum
	{
		animationMaxCount_sceneObject = 24,
		animationMaxCount_projector = 1,
	};

	// scene object rendering program names
	typedef enum a3_Scene_Animation_RenderProgramName
	{
		animation_renderSolid,			// solid color
		animation_renderTexture,		// textured
		animation_renderLambert,		// Lambert shading model
		animation_renderPhong,			// Phong shading model

		animation_render_max
	} a3_Scene_Animation_RenderProgramName;

	// final display modes
	typedef enum a3_Scene_Animation_DisplayProgramName
	{
		animation_displayTexture,			// display simple texture

		animation_display_max
	} a3_Scene_Animation_DisplayProgramName;

	// active camera names
	typedef enum a3_Scene_Animation_ActiveCameraName
	{
		animation_cameraSceneViewer,		// scene viewing camera

		animation_camera_max
	} a3_Scene_Animation_ActiveCameraName;

	// pipeline names
	typedef enum a3_Scene_Animation_PipelineName
	{
		animation_forward,				// forward lighting pipeline

		animation_pipeline_max
	} a3_Scene_Animation_PipelineName;

	// render passes
	typedef enum a3_Scene_Animation_PassName
	{
		animation_passScene,				// render scene objects
		animation_passComposite,			// composite layers

		animation_pass_max
	} a3_Scene_Animation_PassName;

	// render target names
	typedef enum a3_Scene_Animation_TargetName
	{
		animation_scene_finalcolor = 0,	// final display color
		animation_scene_fragdepth,		// fragment depth
	
		animation_target_scene_max
	} a3_Scene_Animation_TargetName;

	// control targets
	typedef enum a3_Scene_Animation_ControlTarget
	{
		animation_ctrl_camera,
		animation_ctrl_teapot,
	
		animation_ctrl_character,

		animation_ctrl_character_rig,
		animation_ctrl_neckLookat,
		animation_ctrl_wristEffector_r,
		animation_ctrl_wristConstraint_r,
		animation_ctrl_wristEffector_l,
		animation_ctrl_wristConstraint_l,
		animation_ctrl_ankleEffector_r,
		animation_ctrl_ankleConstraint_r,
		animation_ctrl_ankleEffector_l,
		animation_ctrl_ankleConstraint_l,

		animation_ctrlmode_max
	} a3_Scene_Animation_ControlTarget;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: ADD MODES
//-----------------------------------------------------------------------------
	


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

	// display data for hierarchy drawing
	typedef struct a3_HierarchyDisplayData
	{
		a3mat4 mvp_joint[128], mvp_bone[128], t_skin[128];
		a3dualquat dq_skin[128];
	} a3_HierarchyDisplayData;


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	typedef struct a3_Scene_Animation
	{
		a3_Scene_Animation_RenderProgramName render;
		a3_Scene_Animation_DisplayProgramName display;
		a3_Scene_Animation_ActiveCameraName activeCamera;

		a3_Scene_Animation_PipelineName pipeline;
		a3_Scene_Animation_PassName pass;
		a3_Scene_Animation_TargetName targetIndex[animation_pass_max], targetCount[animation_pass_max];

		// scene graph
		a3_Hierarchy sceneGraph[1];
		a3_HierarchyState sceneGraphState[1];
		a3_SceneModelMatrixStack matrixStack[animationMaxCount_sceneObject];
		
		// skeletal animation controllers
		union {
			a3_ClipController clipCtrl[3];
			struct {
				a3_ClipController
					clipCtrl_idle_p[1],
					clipCtrl_idle_m[1],
					clipCtrl_idle_f[1];
			};
		};

		// other animation controllers
		a3_ClipController clipCtrl_morph[1];
		a3f64 morph_time;

		// complete clip/keyframe/sample collection
		a3_ClipPool clipPool[1];
		
		// skeletal animation
		union {
			a3_HierarchyState hierarchyState_skel[4];
			struct {
				a3_HierarchyState
					hierarchyState_skel_ik[1],
					hierarchyState_skel_fk[1],
					hierarchyState_skel_final[1],
					hierarchyState_skel_base[1];
			};
		};
		a3_Hierarchy hierarchy_skel[1];
		a3_HierarchyPoseGroup hierarchyPoseGroup_skel[1];
		a3_HierarchyDisplayData display_main;

//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PREP-4: ADD BLENDING
//-----------------------------------------------------------------------------
		


//-----------------------------------------------------------------------------
//****END-TO-DO-PREP-4
//-----------------------------------------------------------------------------

		// control modes
		a3_Scene_Animation_ControlTarget ctrl_target;

		// objects
		union {
			a3_SceneObject object_scene[animationMaxCount_sceneObject];
			struct {
				a3_SceneObject
					obj_world_root[1];
				a3_SceneObject
					obj_camera_main[1];
				a3_SceneObject
					obj_light_main[1];
				a3_SceneObject
					obj_skybox[1];
				a3_SceneObject
					obj_teapot[1];
				
				a3_SceneObject
					obj_skeleton_ctrl[1],
					obj_skeleton_rig[1];
				a3_SceneObject
					obj_skeleton_neckLookat_ctrl[1],
					obj_skeleton_wristEffector_r_ctrl[1],
					obj_skeleton_wristConstraint_r_ctrl[1],
					obj_skeleton_wristEffector_l_ctrl[1],
					obj_skeleton_wristConstraint_l_ctrl[1],
					obj_skeleton_ankleEffector_r_ctrl[1],
					obj_skeleton_ankleConstraint_r_ctrl[1],
					obj_skeleton_ankleEffector_l_ctrl[1],
					obj_skeleton_ankleConstraint_l_ctrl[1],
					obj_skeleton[1];
			};
		};
		union {
			a3_SceneProjector projector[animationMaxCount_projector];
			struct {
				a3_SceneProjector
					proj_camera_main[1];
			};
		};
	} a3_Scene_Animation;


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_SCENE_ANIMATION_H