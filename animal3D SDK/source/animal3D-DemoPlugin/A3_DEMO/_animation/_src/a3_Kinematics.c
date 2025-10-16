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

	a3real4x4 invParentTransform;
	a3real4x4GetInverse(invParentTransform, hierarchyState->objectSpace->hpose_base[parentIndex].transformMat.m);

	a3real4x4Product
	(
		hierarchyState->localSpace->hpose_base[index].transformMat.m,       // Result: this node local-space
		invParentTransform,                                                 // Left-hand: parent node inverse object-space
		hierarchyState->objectSpace->hpose_base[index].transformMat.m       // Right-hand: this node object-space
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

// Should be done || in theory done now
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

		a3hierarchyPoseRestore
		(
			activeHS->localSpace,
			activeHS->hierarchy->numNodes,
			poseGroup->channel,
			poseGroup->order
		);

		a3hierarchyPoseDeconcat
		(
			activeHS->localSpace,
			activeHS->animPose,						
			baseHS->localSpace,					
			activeHS->hierarchy->numNodes
		);

		a3hierarchyPoseDeconcat(activeHS->localSpace,
			activeHS->animPose,						// holds current sample pose
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

	const a3_HierarchyNode* node = activeHS->hierarchy->nodes + nodeIndex;

	// 1. Reassign the new desired object-space transform to the single affected joint
	a3real4x4SetReal4x4(activeHS->objectSpace->hpose_base[nodeIndex].transformMat.m, j2obj);

	// 2. Compute the object-space inverse matrix for this single node
	a3real4x4GetInverse(activeHS->objectSpaceInv->hpose_base[nodeIndex].transformMat.m, j2obj);

	// 3. Compute the new local-space matrix for this single node
	if (node->parentIndex >= 0)
		a3kinematicsSolveInverseSingle(activeHS, nodeIndex, node->parentIndex);
	else
		a3kinematicsSolveInverseRoot(activeHS, nodeIndex);

	
	// 4. Restore the local-space matrices back into poses
	a3hierarchyPoseRestore(activeHS->localSpace,
		activeHS->hierarchy->numNodes,
		poseGroup->channel,
		poseGroup->order);

	// 5. Deconcatenate the base pose
	a3hierarchyPoseDeconcat(activeHS->localSpace,
		activeHS->animPose,
		baseHS->localSpace,
		activeHS->hierarchy->numNodes);

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
	// The bases in params tells which axes map to which axes (a3basisToMat3)

	// First Step
	//	Trnsform everything into the space of the skeleton
	//	-> look-at target

	// Main Step
	//	Solver: Build an orthonormal basis (joint-to-object basis)
	//	1. direction basis = target (m_hierachyObj) - joint position (m_affected)
	//	2. side basis (X-Axis) = up vector Xcross direction basis
	//	3. up basis = direction basis Xcross side basis
	//	4. normalize all (or do this first to the first and second bases)
	//	Contingency (dont need to implement): If look direction is Up or Down (in line with up vector), then their cross would be 0. So we need to pick a new up by choosing a diff basis (like vector.back)

	// Last Step
	//	Resolve every affected jointP: a3KinematicsResolvePostIK

	
	//a3vec3 target = m_hierarchyObj.v2;
	//a3vec3 jointPos = m_affected.v2;

	a3vec4 target, jointPos;

	target = sceneGraphState->localSpace->hpose_base[sceneGraphIndex_effector].transformMat.v3;
	jointPos = sceneGraphState->localSpace->hpose_base[hierarchyObjIndex_affected].transformMat.v3;

	a3vec4 upVector = { 0,1,0,0 };

	a3mat4 sightPosition;
	a3mat4 invSightPosition;

	a3real4x4MakeLookAt
	(
		&sightPosition.mm,
		&invSightPosition.mm,
		jointPos.v,
		target.v,
		upVector.v
	);

	// Last Step
	//	Resolve every affected jointP: a3KinematicsResolvePostIK
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected, sightPosition.m);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}

void a3kinematicsUpdateLimbIK
(
	//Hierarchy State
	a3_HierarchyState const* sceneGraphState,
	a3_HierarchyState* activeHS,
	a3_HierarchyState const* baseHS, 
	a3_HierarchyPoseGroup const* poseGroup,

	//Scene thingies (move things around)
	a3ui32 const sceneGraphIndex_hierarchyObj, 
	a3ui32 const sceneGraphIndex_effector_end,
	a3ui32 const sceneGraphIndex_constraint,

	//joint positions (things we modify)
	a3ui32 const hierarchyObjIndex_affected_end,
	a3ui32 const hierarchyObjIndex_affected_hinge,
	a3ui32 const hierarchyObjIndex_affected_base,

	//joint basis positions (how we get things)
	a3_Basis const basis_hierarchyObj, 
	a3_Basis const basis_affected_end, 
	a3_Basis const basis_affected_hinge, 
	a3_Basis const basis_affected_base
)

{
	a3mat3 m_hierarchyObj, m_affected_end, m_affected_hinge, m_affected_base;

	//! Failstates
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
	// First Step
		// Transform everything into the space of the skeleton
		// -> wrist effector
		// -> pole vector constraint	

	a3vec4 endPosVec4, hingePosVec4, basePosVec4, targetPosVec4, polePosVec4;

	endPosVec4 = sceneGraphState->objectSpace->hpose_base[hierarchyObjIndex_affected_end].transformMat.v3;
	hingePosVec4 = sceneGraphState->objectSpace->hpose_base[hierarchyObjIndex_affected_hinge].transformMat.v3;
	basePosVec4 = sceneGraphState->objectSpace->hpose_base[hierarchyObjIndex_affected_base].transformMat.v3;


	targetPosVec4 = sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_constraint].transformMat.v3;
	polePosVec4 = sceneGraphState->objectSpace->hpose_base[sceneGraphIndex_effector_end].transformMat.v3;



	// Main Step
	// solve joint-to-object for end, hinge, base
	//	-> end*
	//	-> hinge position*
	//	1. base joint to end effector vector (and distance)
	//	2. base joint to pole vector constraint
	//	3. plane normal = step 1 (base to pole) Xcross step 2 (base to end)
	//	4. geometric (Heron's formular) or algebrais (law of cosines)
	//	-> solves elbow position
	//	5. "look at" solves the shoulder and elbow rotations

	// Last Step
	// Resolve every affected jointP: a3KinematicsResolvePostIK
	// -> work from root to leaf
	// a3KinematicsResolvePostIK
	// a3KinematicsResolvePostIK
	// a3KinematicsResolvePostIK

	//Extracting x, y, and z for future use
	a3real3 basePos, hingePos, endPos, targetPos, polePos;
	a3real3Set(basePos, basePosVec4.x, basePosVec4.y, basePosVec4.z);
	a3real3Set(hingePos, hingePosVec4.x, hingePosVec4.y, hingePosVec4.z);
	a3real3Set(endPos, endPosVec4.x, endPosVec4.y, endPosVec4.z);
	a3real3Set(targetPos, targetPosVec4.x, targetPosVec4.y, targetPosVec4.z);
	a3real3Set(polePos, polePosVec4.x, polePosVec4.y, polePosVec4.z);

	//Get Segment Lengths
	a3real3 baseToHinge, hingeToEnd, baseToTarget;
	a3real3Diff(baseToHinge, hingePos, basePos);
	a3real3Diff(hingeToEnd, endPos, hingePos);
	a3real3Diff(baseToTarget, targetPos, basePos);

	a3real lengthUpperLimb;
	a3real lengthLowerLimb;
	a3real distanceToTarget;

	lengthUpperLimb = a3real3Length(baseToHinge);
	lengthLowerLimb = a3real3Length(hingeToEnd);
	distanceToTarget = a3real3Length(baseToTarget);

	//Clamp to triangle Maths
	a3real epsilon, minimumReach, maximumReach;

	//epsilon is to prevent divide by zero; Maths Explanation by @ZuchiniByDay//
	epsilon = (a3real)1e-6;

	minimumReach = a3maximum(lengthUpperLimb - lengthLowerLimb, epsilon);
	maximumReach = lengthUpperLimb + lengthLowerLimb - epsilon;
	distanceToTarget = a3clamp(distanceToTarget, minimumReach, maximumReach);

	//Calculate Direction from Base to Target
	a3real3 directionBaseToTarget;
	if (a3real3GetUnit(directionBaseToTarget, baseToTarget))
		return;

	//Pole Direction
	a3real3 baseToPole, polePlaneNormalized;

	a3real3Diff(baseToPole, polePos, basePos);
	a3real3Cross(polePlaneNormalized, directionBaseToTarget, polePlaneNormalized);

	if (a3real3GetUnit(polePlaneNormalized, polePlaneNormalized) == 0)
	{
		a3real3Set(polePlaneNormalized, 0, 0, 1);
		a3real3Cross(polePlaneNormalized, directionBaseToTarget, polePlaneNormalized);
		a3real3GetUnit(polePlaneNormalized, polePlaneNormalized);
	}

	a3real3 bendDirection;
	a3real3Cross(bendDirection, polePlaneNormalized, directionBaseToTarget);
	a3real3GetUnit(polePlaneNormalized, polePlaneNormalized);

	//Law of Cosines (Pythagorean Theorem)
	a3real a, b, c;
	a = lengthUpperLimb;
	b = lengthLowerLimb;
	c = distanceToTarget;

	//Maths from @ZuchiniByDay; Referenced as well in the original commented below; variables changed to match the Pythagorean Theorem rather than PseudoCode
	a3real cosA = (a * a + c * c - b * b) / (2.0f * a * c);
	cosA = a3clamp(cosA, -1.0f, 1.0f);
	const a3real sinA = a3sqrt(a3maximum(0.0f, 1.0f - cosA * cosA));

	a3real3 newHinge;
	a3real3 tan1, tan2;

	a3real3ProductS(tan1, directionBaseToTarget, a * cosA);
	a3real3ProductS(tan2, bendDirection, a * sinA);
	a3real3Sum(newHinge, basePos, tan1);
	a3real3Sum(newHinge, newHinge, tan2);

	//Look at stuff
	a3vec4 upVec4;
	upVec4.x = bendDirection[0];
	upVec4.y = bendDirection[1];
	upVec4.z = bendDirection[2];
	upVec4.w = 0.00f;

	a3real4x4 j2obj_base, inverseBase;
	a3real4x4 j2obj_hinge, inverseHinge;
	a3real4x4 j2obj_end, inverseEnd;

	//Base to Hinge
	{
		a3vec4 baseVec4, newHingeVec4;

		baseVec4.x = basePos[0];
		baseVec4.y = basePos[1];
		baseVec4.z = basePos[2];
		baseVec4.w = 1.0f;

		newHingeVec4.x = newHinge[0];
		newHingeVec4.y = newHinge[1];
		newHingeVec4.z = newHinge[2];
		newHingeVec4.w = 1.0f;

		a3real4x4MakeLookAt
		(
			j2obj_base, 
			inverseBase, 
			baseVec4.v, 
			newHingeVec4.v, 
			upVec4.v
		);
	}

	//Hinge to Target
	{
		a3vec4 targetVec4, newHingeVec4;
		
		targetVec4.x = targetPos[0];
		targetVec4.y = targetPos[1];
		targetVec4.z = targetPos[2];
		targetVec4.w = 1.0f;

		newHingeVec4.x = newHinge[0];
		newHingeVec4.y = newHinge[1];
		newHingeVec4.z = newHinge[2];
		newHingeVec4.w = 1.0f;

		a3real4x4MakeLookAt
		(
			j2obj_hinge,
			inverseHinge,
			targetVec4.v,
			newHingeVec4.v,
			upVec4.v
		);
	}

	//End looks the same way
	{
		a3vec4 endVec4, targetVec4;

		endVec4.x = targetPos[0];
		endVec4.y = targetPos[1];
		endVec4.z = targetPos[2];
		endVec4.w = 1.0f;

		targetVec4 = endVec4;
		targetVec4.z += 1.0f;

		a3real4x4MakeLookAt
		(
			j2obj_end,
			inverseEnd,
			targetVec4.v,
			targetVec4.v,
			upVec4.v
		);
	}

	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_base, j2obj_base);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_hinge, j2obj_hinge);
	a3kinematicsResolvePostIK(activeHS, baseHS, poseGroup, hierarchyObjIndex_affected_end, j2obj_end);


	/*// Constrained Displacement
	// Distance of bottom of right triangle
	a3real4x4 constrainDisplacement;
	a3real3Diff(*constrainDisplacement, endPosVec4.v, basePosVec4.v);

	// Effector Displacement
	a3real4x4 effectorDisplacement;
	a3real3Diff(*effectorDisplacement, basePosVec4.v, hingePosVec4.v);

	// Normal Displacement
	a3real4x4 planeNormal;
	a3real3CrossUnit(*planeNormal, *effectorDisplacement, *constrainDisplacement);

	// Height of triangle
	a3real3 height;
	a3real3Cross(height, *planeNormal, *effectorDisplacement);


	//Math by @ZuchiniByDay (Seriously, thank you)
	a3real Line1 = a3real2Length(*effectorDisplacement);
	a3real Line2 = a3real2Length(*constrainDisplacement);
	a3real Line3 = a3real2Length(height);

	a3real cosineTheta =
		(Line1 * Line1 + Line2 * Line2 + Line3 * Line3) / (2 * Line1 * Line2);

	a3real theta = a3cosd(cosineTheta);*/


	/*a3kinematicsUpdateLookAtIK
	(
		sceneGraphState,
		activeHS,
		baseHS,
		poseGroup,
		sceneGraphIndex_hierarchyObj,
		sceneGraphIndex_effector_end,
		&m_affected_base.m,
		basis_hierarchyObj,
		basis_affected_base
	);

	a3kinematicsUpdateLookAtIK
	(
		sceneGraphState,
		activeHS,
		baseHS,
		poseGroup,
		sceneGraphIndex_hierarchyObj,
		sceneGraphIndex_effector_end,
		&m_affected_hinge.m,
		basis_hierarchyObj,
		basis_affected_hinge
	);

	a3kinematicsUpdateLookAtIK
	(
		sceneGraphState,
		activeHS,
		baseHS,
		poseGroup,
		sceneGraphIndex_hierarchyObj,
		sceneGraphIndex_effector_end,
		&m_affected_end.m,
		basis_hierarchyObj,
		basis_affected_end
	);*/

//----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
}
																					

//-----------------------------------------------------------------------------
