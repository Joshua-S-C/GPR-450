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
	
	a3_Kinematics.c
	Implementation of kinematics solvers.
*/

#include "../a3_Kinematics.h"


//-----------------------------------------------------------------------------

// single FK helpers
static inline void a3kinematicsSolveForwardSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_object] = T[parent_object] * T[this_local]
	a3real4x4Product(
		hierarchyState->objectSpace->hpose_base[index].transformMat.m,		// Result: this node object-space.
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,// Left-hand: parent node object-space.
		hierarchyState->localSpace->hpose_base[index].transformMat.m		// Right-hand: this node local space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_object] = T[root_local]
	hierarchyState->objectSpace->hpose_base[index] = hierarchyState->localSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}

// partial FK solver
a3i32 a3kinematicsSolveForwardPartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement forward kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- object matrix = parent object matrix * local matrix
		//		- else
		//			- copy local matrix to object matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveForwardSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveForwardRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

// single IK helpers
static inline void a3kinematicsSolveInverseSingle(const a3_HierarchyState* hierarchyState, const a3ui32 index, const a3ui32 parentIndex)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[this_local] = T[parent_object]^-1 * T[this_object]
	a3real4x4Product(
		hierarchyState->localSpace->hpose_base[index].transformMat.m,			// Result: this node local-space.
		hierarchyState->objectSpaceInv->hpose_base[parentIndex].transformMat.m,	// Left-hand: parent node object-space inverse.
		hierarchyState->objectSpace->hpose_base[index].transformMat.m			// Right-hand: this node object-space.
	);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// T[root_local] = T[root_object]
	hierarchyState->localSpace->hpose_base[index] = hierarchyState->objectSpace->hpose_base[index];

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

// partial IK solver
a3i32 a3kinematicsSolveInversePartial(const a3_HierarchyState* hierarchyState, const a3ui32 firstIndex, const a3ui32 nodeCount)
{
	if (hierarchyState && hierarchyState->hierarchy &&
		firstIndex < hierarchyState->hierarchy->numNodes && nodeCount)
	{
		// implement inverse kinematics algorithm: 
		//	- for all nodes starting at first index
		//		- if node is not root (has parent node)
		//			- local matrix = inverse parent object matrix * object matrix
		//		- else
		//			- copy object matrix to local matrix
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

		const a3_HierarchyNode* itr = hierarchyState->hierarchy->nodes + firstIndex;
		const a3_HierarchyNode* const end = itr + nodeCount;
		for (; itr < end; ++itr)
		{
			if (itr->parentIndex >= 0)
				a3kinematicsSolveInverseSingle(hierarchyState, itr->index, itr->parentIndex);
			else
				a3kinematicsSolveInverseRoot(hierarchyState, itr->index);
		}
		return (a3i32)(end - itr);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

void a3kinematicsUpdateHierarchyStateFK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// FK pipeline
		//	-> concatenate base pose
		//	-> convert poses to local-space matrices
		//	-> perform recursive FK
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyPoseConcat(activeHS->localSpace,	// local: goal to calculate
			activeHS->animPose,						// holds current sample pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);
		a3hierarchyPoseConvert(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3kinematicsSolveForward(activeHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup)
{
	if (activeHS->hierarchy == baseHS->hierarchy &&
		activeHS->hierarchy == poseGroup->hierarchy)
	{
		// IK pipeline
		//	-> perform recursive IK
		//	-> restore local-space matrices to poses
		//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3kinematicsSolveInverse(activeHS);
		a3hierarchyPoseRestore(activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order);
		a3hierarchyPoseDeconcat(activeHS->animPose,	// current sample pose: goal to calculate
			activeHS->localSpace,					// holds local pose
			baseHS->localSpace,						// holds base pose (animPose is all identity poses)
			activeHS->hierarchy->numNodes);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
}

void a3kinematicsUpdateHierarchyStateSkin(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS)
{
	if (activeHS->hierarchy == baseHS->hierarchy)
	{
		// FK pipeline extended for skinning and other applications
		//	-> update local-space inverse matrices
		//	-> update object-space inverse matrices
		//	-> update transform from base to current
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		a3hierarchyStateUpdateLocalInverse(activeHS);
		a3hierarchyStateUpdateObjectInverse(activeHS);
		a3hierarchyStateUpdateObjectBindToCurrent(activeHS, baseHS);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
}


//-----------------------------------------------------------------------------

// helper to resolve single-joint IK after solver
static void a3kinematicsResolvePostIK(a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const nodeIndex, a3real4x4 const j2obj)
{
	// post-IK resolution for single affected joint
	//	-> reassign resolved transform to object-space
	//	-> compute object-space inverse matrix
	//	-> compute local-space matrix
	//	-> restore local-space matrix to pose
	//	-> deconcatenate base pose
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// reassign resolved transform to OBJECT-SPACE matrix
	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);
	// compute OBJECT-SPACE matrix inverse
	a3real4x4TransformInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, j2obj);
	// solve LOCAL-SPACE matrix
	a3kinematicsSolveInverseSingle(activeHS,
		activeHS->hierarchy->nodes[nodeIndex].index,
		activeHS->hierarchy->nodes[nodeIndex].parentIndex);
	// restore pose
	a3spatialPoseRestore(&activeHS->localSpace->hpose_base[nodeIndex],
		poseGroup->channel[nodeIndex], poseGroup->order[nodeIndex]);
	// deconcat
	a3spatialPoseDeconcat(&activeHS->animPose->hpose_base[nodeIndex],
		&activeHS->localSpace->hpose_base[nodeIndex],
		&baseHS->localSpace->hpose_base[nodeIndex]);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLookAtIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector,
	a3ui32 const hierarchyObjIndex_affected, a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected)
{
	a3mat3 m_hierarchyObj, m_affected;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected.m, basis_affected))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// need to properly transform joints to their parent frame and vice-versa
	// get the hierarchy root object transform relative to the rig
	a3mat4 const obj2rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;
	a3mat4 const rig2obj = sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;

	// affected joint relative to hierarchy
	a3mat4 j2obj_affected = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected].transformMat;

	// SOLVER
	{
		// affected joint relative to rig
		a3mat4 j2rig_affected;
		a3real4x4ProductTransform(j2rig_affected.m, obj2rig.m, j2obj_affected.m);

		// affected joint position in rig
		a3vec3 const affectedPos_rig = j2rig_affected.v3.xyz;

		// effector locator position in rig
		a3vec3 const effectorPos_rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.v3.xyz;

		// bases to form
		a3vec3 right = m_hierarchyObj.v0, fwd = m_hierarchyObj.v1, up = m_hierarchyObj.v2;
		a3mat3 r_affected;

		// compute look-at basis
		{
			// compute bases
			a3real3Diff(fwd.v, effectorPos_rig.v, affectedPos_rig.v);
			a3real3Normalize(fwd.v);
			a3real3CrossUnit(right.v, fwd.v, up.v);
			a3real3Cross(up.v, right.v, fwd.v);

			// convert to matrix
			a3real3SetReal3(r_affected.v0.v, right.v);
			a3real3SetReal3(r_affected.v1.v, fwd.v);
			a3real3SetReal3(r_affected.v2.v, up.v);

			// map to joint orientation
			a3real3x3Transpose(m_affected.m);
			a3real3x3ConcatL(r_affected.m, m_affected.m);

			// put it back in hierarchy object space
			j2rig_affected.v0.xyz = r_affected.v0;
			j2rig_affected.v1.xyz = r_affected.v1;
			j2rig_affected.v2.xyz = r_affected.v2;
		}

		a3real4x4ProductTransform(j2obj_affected.m, rig2obj.m, j2rig_affected.m);
	}

	// RESOLVE IK
	// (single-chain)
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, j2obj_affected.m);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLimbIK(a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS, a3_HierarchyState const* baseHS, a3_HierarchyPoseGroup const* poseGroup,
	a3ui32 const sceneGraphIndex_hierarchyObj, a3ui32 const sceneGraphIndex_effector_end, a3ui32 const sceneGraphIndex_constraint,
	a3ui32 const hierarchyObjIndex_affected_end, a3ui32 const hierarchyObjIndex_affected_hinge, a3ui32 const hierarchyObjIndex_affected_base,
	a3_Basis const basis_hierarchyObj, a3_Basis const basis_affected_end, a3_Basis const basis_affected_hinge, a3_Basis const basis_affected_base)
{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;
	if (!a3basisToMat3(m_hierarchyObj.m, basis_hierarchyObj))
		return;
	if (!a3basisToMat3(m_affected_end.m, basis_affected_end))
		return;
	if (!a3basisToMat3(m_affected_hinge.m, basis_affected_hinge))
		return;
	if (!a3basisToMat3(m_affected_base.m, basis_affected_base))
		return;

	if ((!sceneGraphState || !activeHS || !baseHS || !poseGroup) ||
		(activeHS->hierarchy != baseHS->hierarchy) ||
		(activeHS->hierarchy != poseGroup->hierarchy))
		return;
	
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------

	// need to properly transform joints to their parent frame and vice-versa
	// get the hierarchy root object transform relative to the rig
	a3mat4 const obj2rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;
	a3mat4 const rig2obj = sceneGraphState->localSpaceInv->hpose_base[sceneGraphIndex_hierarchyObj].transformMat;

	// affected joints relative to hierarchy
	a3mat4 j2obj_affected_end = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat;
	a3mat4 j2obj_affected_hinge = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat;
	a3mat4 j2obj_affected_base = activeHS->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat;

	// SOLVER
	{
		// affected joints relative to rig
		a3mat4 j2rig_affected_end, j2rig_affected_hinge, j2rig_affected_base;
		a3real4x4ProductTransform(j2rig_affected_end.m, obj2rig.m, j2obj_affected_end.m);
		a3real4x4ProductTransform(j2rig_affected_hinge.m, obj2rig.m, j2obj_affected_hinge.m);
		a3real4x4ProductTransform(j2rig_affected_base.m, obj2rig.m, j2obj_affected_base.m);

		// affected joint positions in rig
		a3vec3 affectedPos_end_rig = j2rig_affected_end.v3.xyz;
		a3vec3 affectedPos_hinge_rig = j2rig_affected_hinge.v3.xyz;
		a3vec3 const affectedPos_base_rig = j2rig_affected_base.v3.xyz;

		// effector and constraint positions in rig
		a3vec3 const effectorPos_end_rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.v3.xyz;
		a3vec3 const constraintPos_rig = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_constraint].transformMat.v3.xyz;

		// determine if solution exists
		a3vec3 upperDiff, lowerDiff, effectorDiff, constraintDiff, normal, offset;
		a3real upperDist, lowerDist, effectorDist, maxDist;
		a3real3Diff(upperDiff.v, affectedPos_base_rig.v, affectedPos_hinge_rig.v);
		a3real3Diff(lowerDiff.v, affectedPos_hinge_rig.v, affectedPos_end_rig.v);
		a3real3Diff(effectorDiff.v, effectorPos_end_rig.v, affectedPos_base_rig.v);
		a3real3Diff(constraintDiff.v, constraintPos_rig.v, affectedPos_base_rig.v);
		a3real3CrossUnit(normal.v, constraintDiff.v, effectorDiff.v);
		upperDist = a3real3Length(upperDiff.v);
		lowerDist = a3real3Length(lowerDiff.v);
		effectorDist = a3real3Length(effectorDiff.v);
		a3real3MulS(effectorDiff.v, a3recipsafe(effectorDist));
		maxDist = upperDist + lowerDist;
		if (effectorDist >= maxDist)
		{
			// simple solution: end goes to farthest possible point, hinge also easy to solve
			a3real3Add(a3real3ProductS(affectedPos_end_rig.v, effectorDiff.v, maxDist), affectedPos_base_rig.v);
			a3real3Add(a3real3ProductS(affectedPos_hinge_rig.v, effectorDiff.v, upperDist), affectedPos_base_rig.v);
		}
		else
		{
			// not-so-simple solution: while wrist position is solved, need elbow
			// use properties of triangles to get location
			// area of triangle using Heron's formula
			a3real const s = a3real_half * (effectorDist + maxDist),
				area = a3sqrt(s * (s - effectorDist) * (s - upperDist) * (s - lowerDist)),
				height = a3real_two * area / effectorDist,
				base = a3sqrt(upperDist * upperDist - height * height);

			a3real3MulS(a3real3Cross(offset.v, effectorDiff.v, normal.v), height);
			a3real3ProductS(affectedPos_hinge_rig.v, effectorDiff.v, base);
			a3real3Add(a3real3Add(affectedPos_hinge_rig.v, offset.v), affectedPos_base_rig.v);
			affectedPos_end_rig = effectorPos_end_rig;
		}

		// bases to form
		a3vec3 right = m_hierarchyObj.v0, fwd = m_hierarchyObj.v1;
		a3mat3 r_affected;

		// compute base node basis
		{
			// compute bases
			a3real3Diff(fwd.v, affectedPos_hinge_rig.v, affectedPos_base_rig.v);
			a3real3Normalize(fwd.v);
			a3real3Cross(right.v, fwd.v, normal.v);

			// convert to matrix
			a3real3SetReal3(r_affected.v0.v, right.v);
			a3real3SetReal3(r_affected.v1.v, fwd.v);
			a3real3SetReal3(r_affected.v2.v, normal.v);

			// map to joint orientation
			a3real3x3Transpose(m_affected_base.m);
			a3real3x3ConcatL(r_affected.m, m_affected_base.m);

			// put it back in hierarchy object space
			j2rig_affected_base.v0.xyz = r_affected.v0;
			j2rig_affected_base.v1.xyz = r_affected.v1;
			j2rig_affected_base.v2.xyz = r_affected.v2;
		}

		// compute hinge node basis
		{
			// compute bases
			a3real3Diff(fwd.v, affectedPos_end_rig.v, affectedPos_hinge_rig.v);
			a3real3Normalize(fwd.v);
			a3real3Cross(right.v, fwd.v, normal.v);

			// convert to matrix
			a3real3SetReal3(r_affected.v0.v, right.v);
			a3real3SetReal3(r_affected.v1.v, fwd.v);
			a3real3SetReal3(r_affected.v2.v, normal.v);

			// map to joint orientation
			a3real3x3Transpose(m_affected_hinge.m);
			a3real3x3ConcatL(r_affected.m, m_affected_hinge.m);

			// put it back in hierarchy object space
			j2rig_affected_hinge.v0.xyz = r_affected.v0;
			j2rig_affected_hinge.v1.xyz = r_affected.v1;
			j2rig_affected_hinge.v2.xyz = r_affected.v2;

			// position
			j2rig_affected_hinge.v3.xyz = affectedPos_hinge_rig;
		}

		// update end node basis
		{
			// position
			j2rig_affected_end.v3.xyz = affectedPos_end_rig;
		}

		a3real4x4ProductTransform(j2obj_affected_end.m, rig2obj.m, j2rig_affected_end.m);
		a3real4x4ProductTransform(j2obj_affected_hinge.m, rig2obj.m, j2rig_affected_hinge.m);
		a3real4x4ProductTransform(j2obj_affected_base.m, rig2obj.m, j2rig_affected_base.m);
	}

	//RESOLVE IK
	// (multi-chain: work from root to leaf too get correct transformations)
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_base, j2obj_affected_base.m);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_hinge, j2obj_affected_hinge.m);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_end, j2obj_affected_end.m);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
