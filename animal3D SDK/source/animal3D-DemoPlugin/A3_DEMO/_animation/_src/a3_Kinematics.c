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

	a3real4x4Product(hierarchyState->objectSpace->hpose_base[index].transformMat.m,
		hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m,
		hierarchyState->localSpace->hpose_base[index].transformMat.m);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveForwardRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

	hierarchyState->objectSpace->hpose_base[index].transformMat = hierarchyState->localSpace->hpose_base[index].transformMat;

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
		a3ui32 i;
		for (i = firstIndex; i < nodeCount; ++i)
		{
			// Check if there is no parent, therefore we're root
			if (hierarchyState->hierarchy->nodes[i].parentIndex < 0) {
				a3kinematicsSolveForwardRoot(hierarchyState, hierarchyState->hierarchy->nodes[i].index);
			} else {
				a3kinematicsSolveForwardSingle(hierarchyState, hierarchyState->hierarchy->nodes[i].index, hierarchyState->hierarchy->nodes[i].parentIndex);
			}
		}

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



//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
static inline void a3kinematicsSolveInverseRoot(const a3_HierarchyState* hierarchyState, const a3ui32 index)
{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------



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



//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}


//-----------------------------------------------------------------------------
