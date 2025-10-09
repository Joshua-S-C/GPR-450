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
	
	a3_HierarchyState.c
	Implementation of transform hierarchy state.
*/

#include "../a3_HierarchyState.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//-----------------------------------------------------------------------------

// initialize pose set given an initialized hierarchy and key pose count
a3i32 a3hierarchyPoseGroupCreate(a3_HierarchyPoseGroup *poseGroup_out, const a3_Hierarchy *hierarchy, const a3ui32 poseCount)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (poseGroup_out && hierarchy && !poseGroup_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = poseCount, hposeSpace = sizeof(a3_HierarchyPose) * hposeCount;
		a3ui32 const sposeCount = hposeCount * nodeCount, sposeSpace = sizeof(a3_SpatialPose) * sposeCount;
		a3ui32 const channelSpace = sizeof(a3_SpatialPoseChannel) * nodeCount;
		a3ui32 const orderSpace = sizeof(a3_SpatialPoseEulerOrder) * nodeCount;
		a3ui32 const memreq = hposeSpace + sposeSpace + channelSpace + orderSpace;
		a3index i;

		// allocate everything (one malloc)
		poseGroup_out->hpose = (a3_HierarchyPose*)malloc(memreq);
		poseGroup_out->hpose->hpose_base = poseGroup_out->pose = (a3_SpatialPose*)(poseGroup_out->hpose + hposeCount);
		poseGroup_out->channel = (a3_SpatialPoseChannel*)(poseGroup_out->pose + sposeCount);
		poseGroup_out->order = (a3_SpatialPoseEulerOrder*)(poseGroup_out->channel + nodeCount);

		// set pointers
		for (i = 1; i < hposeCount; ++i)
		{
			poseGroup_out->hpose[i].hpose_base = poseGroup_out->hpose[i - 1].hpose_base + nodeCount;
			poseGroup_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(poseGroup_out->hpose, sposeCount);
		memset(poseGroup_out->channel, a3poseChannel_none, channelSpace);
		memset(poseGroup_out->order, a3poseEulerOrder_xyz, orderSpace);
		poseGroup_out->hierarchy = hierarchy;
		poseGroup_out->hposeCount = hposeCount;
		poseGroup_out->poseCount = sposeCount;

		// done
		return 1;
	}
	return -1;
}

// release pose set
a3i32 a3hierarchyPoseGroupRelease(a3_HierarchyPoseGroup *poseGroup)
{
	// validate param exists and is initialized
	if (poseGroup && poseGroup->hierarchy)
	{
		// release everything (one free)
		free(poseGroup->hpose);

		// reset pointers
		poseGroup->hierarchy = 0;
		poseGroup->hpose = 0;
		poseGroup->pose = 0;
		poseGroup->channel = 0;
		poseGroup->order = 0;

		// done
		return 1;
	}
	return -1;
}

// load binary
a3i32 a3hierarchyPoseGroupLoadBinary(a3_HierarchyPoseGroup* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	a3ui32 dataSize = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && !poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// fread
				// -> create

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}

// save binary
a3i32 a3hierarchyPoseGroupSaveBinary(a3_HierarchyPoseGroup const* poseGroup, a3_FileStream const* fileStream)
{
	FILE* fp;
	a3ui32 ret = 0;
	if (poseGroup && fileStream)
	{
		if (poseGroup->hierarchy && poseGroup->pose)
		{
			fp = fileStream->stream;
			if (fp)
			{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------
				
				// fwrite
				// -> 

//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
			}
			return ret;
		}
	}
	return -1;
}


//-----------------------------------------------------------------------------

// initialize hierarchy state given an initialized hierarchy
a3i32 a3hierarchyStateCreate(a3_HierarchyState *state_out, const a3_Hierarchy *hierarchy)
{
	// validate params and initialization states
	//	(output is not yet initialized, hierarchy is initialized)
	if (state_out && hierarchy && !state_out->hierarchy && hierarchy->nodes)
	{
		// determine memory requirements
		a3ui32 const nodeCount = hierarchy->numNodes;
		a3ui32 const hposeCount = sizeof(state_out->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 const sposeCount = hposeCount * nodeCount;
		a3ui32 const memreq = sizeof(a3_SpatialPose) * sposeCount;
		a3index i;

		// allocate everything (one malloc)
		state_out->hpose->hpose_base = (a3_SpatialPose*)malloc(memreq);

		// set pointers
		state_out->hierarchy = hierarchy;
		for (i = 1; i < hposeCount; ++i)
		{
			state_out->hpose[i].hpose_base = state_out->hpose[i - 1].hpose_base + nodeCount;
			state_out->hpose[i].hpose_index = i * nodeCount;
		}

		// reset all data
		a3hierarchyPoseReset(state_out->hpose, sposeCount);

		// done
		return 1;
	}
	return -1;
}

// release hierarchy state
a3i32 a3hierarchyStateRelease(a3_HierarchyState *state)
{
	// validate param exists and is initialized
	if (state && state->hierarchy)
	{
		a3ui32 const hposeCount = sizeof(state->hpose) / sizeof(a3_HierarchyPose);
		a3ui32 i;

		// release everything (one free)
		free(state->hpose->hpose_base);

		// reset pointers
		state->hierarchy = 0;
		for (i = 0; i < hposeCount; ++i)
		{
			state->hpose[i].hpose_base = 0;
			state->hpose[i].hpose_index = 0;
		}

		// done
		return 1;
	}
	return -1;
}


//-----------------------------------------------------------------------------

// update inverse local-space matrices
a3i32 a3hierarchyStateUpdateLocalInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4TransformInverse(
				state->localSpaceInv->hpose_base[i].transformMat.m,
				state->localSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space matrices
a3i32 a3hierarchyStateUpdateObjectInverse(const a3_HierarchyState* state)
{
	if (state && state->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4TransformInverse(
				state->objectSpaceInv->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

// update inverse object-space bind-to-current matrices
a3i32 a3hierarchyStateUpdateObjectBindToCurrent(const a3_HierarchyState* state, const a3_HierarchyState* state_bind)
{
	if (state && state->hierarchy && state_bind && state_bind->hierarchy)
	{
		a3index i = 0;
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		for (i = 0; i < state->hierarchy->numNodes; ++i)
		{
			a3real4x4Product(
				state->objectSpaceBindToCurrent->hpose_base[i].transformMat.m,
				state->objectSpace->hpose_base[i].transformMat.m,
				state_bind->objectSpaceInv->hpose_base[i].transformMat.m);
		}

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
		return i;
	}
	return -1;
}

a3byte calculateValue(FILE* file, char* value, char* line, char* currLine)
{
	long pos = ftell(file);

	if (!fgets(currLine, 512, file))
	{
		return false;
	}

	if (currLine[0] == '[' || currLine[0] == '#')
	{
		fseek(file, pos, SEEK_SET);
		return false;
	}

	return sscanf(currLine, "%s %s", line, value) == 2;
}

a3byte handleHeaders(a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, FILE* file, char* currLine, a3f32* scale)
{
	char line[512];
	char value[64];

	a3f32 unitScale = 1;
	a3f32 scaleFactor = 1;

	while (calculateValue(file, value, line, currLine))
	{
		if (strstr(line, "FileType"))
		{
			printf("%s\n", value);

			if (!strstr(value, "HTR"))
			{
				return false;
			}
		}
		else if (strstr(line, "DataType"))
		{
			printf("%s\n", value);

			if (!strstr(value, "HTRS"))
			{
				return false;
			}
		}
		else if (strstr(line, "FileVersion"))
		{
			printf("%s\n", value);

			if (atoi(value) != 1)
			{
				return false;
			}
		}
		else if (strstr(line, "NumSegments"))
		{
			printf("%s\n", value);

			// Not implemented yet
			//a3hierarchyCreate(hierarchy_out, (a3ui32)atoi(value), NULL);
		}
		else if (strstr(line, "NumFrames"))
		{
			printf("%s\n", value);

			// Not implemented yet
			//a3hierarchyPoseGroupCreate(poseGroup_out, hierarchy_out, (a3ui32)atoi(value));
		}
		else if (strstr(line, "EulerRotationOrder"))
		{
			printf("%s\n", value);

			if (strstr(line, "ZYX"))
			{
				// Not implemented yet
				//*poseGroup_out->order = a3poseEulerOrder_zyx;
			}
		}
		else if (strstr(line, "CalibrationUnits"))
		{
			printf("%s\n", value);

			if (strstr(value, "mm"))
			{
				unitScale = 1.0f / 10.0f;
			}
		}
		else if (strstr(line, "RotationUnits"))
		{
			printf("%s\n", value);

			if (strstr(value, "Degrees"))
			{
				printf("%s\nRotationUnits read but not assigned", value);
			}
		}
		else if (strstr(line, "GlobalAxisofGravity"))
		{
			printf("%s\n", value);

			if (strstr(value, "Y"))
			{
				printf("%s\nGlobalAxisofGravity read but not assigned", value);
			}
		}
		else if (strstr(line, "BoneLengthAxis"))
		{
			printf("%s\n", value);

			if (strstr(value, "Y"))
			{
				// Not implemented yet
				//*poseGroup_out->channel = a3poseChannel_scale_y;
			}
		}
		else if (strstr(line, "ScaleFactor"))
		{
			printf("%s\n", value);

			scaleFactor = (a3f32)atof(value);

			return true;
		}
	}

	*scale = scaleFactor * unitScale;

	return true;
}

a3byte handleSegmentsAndHierarchy(a3_Hierarchy* hierarchy_out, FILE* file, char* currLine)
{
	//char line[512];
	char child[a3node_nameSize];
	char parent[a3node_nameSize];

	a3i32 childIndex = 0;
	a3i32 parentIndex = 0;

	printf("\nSegments and Hierarchy\n");

	while (true)
	{
		long pos = ftell(file);
	
		if (!fgets(currLine, 512, file))
		{
			break;
		}
	
		if (currLine[0] == '[' || currLine[0] == '#')
		{
			fseek(file, pos, SEEK_SET);
			break;
		}
	
		if (sscanf(currLine, "%s %s", &child, &parent) == 2)
		{
			printf("%s %s\n", child, parent);
	
			parentIndex = a3hierarchyGetNodeIndex(hierarchy_out, parent);
	
			a3hierarchySetNode(hierarchy_out, childIndex, parentIndex, child);
		}		
	
		childIndex++;
	}

	return true;
}

a3byte handleBasePosition(a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, FILE* file, char* currLine, a3f32 scalePass)
{
	char value[a3node_nameSize];

	a3vec3 posVec;
	a3vec3 rotVec;
	a3f32 scale;

	printf("\nBase Position\n");

	while (true)
	{
		long pos = ftell(file);

		if (!fgets(currLine, 512, file))
		{
			break;
		}

		if (currLine[0] == '[' || currLine[0] == '#')
		{
			fseek(file, pos, SEEK_SET);
			break;
		}

		if (sscanf(currLine, "%s %f %f %f %f %f %f %f", value, &posVec.x, &posVec.y, &posVec.z, &rotVec.x, &rotVec.y, &rotVec.z, &scale) == 8)
		{
			printf("%s %f %f %f %f %f %f %f\n", value, posVec.x, posVec.y, posVec.z, rotVec.x, rotVec.y, rotVec.z, scale);
			a3i32 index = a3hierarchyGetNodeIndex(hierarchy_out, value);

			// Not implemented yet
			//a3spatialPoseSetTranslation(&poseGroup_out->hpose[0].hpose_base[index], posVec.x * scalePass, posVec.y * scalePass, posVec.z * scalePass);
			//a3spatialPoseSetRotation(&poseGroup_out->hpose[0].hpose_base[index], rotVec.x, rotVec.y, rotVec.z);
			//a3spatialPoseSetScale(&poseGroup_out->hpose[0].hpose_base[index], scale, scale, scale);
		}
	}

	return true;
}

a3byte handlePoses(a3_Hierarchy* hierarchy_out, a3_HierarchyPoseGroup* poseGroup_out, FILE* file, char* currLine, a3f32 scalePass)
{

}


//-----------------------------------------------------------------------------

// load HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadHTR(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		FILE* file = fopen(resourceFilePath, "r");

		char line[512];

		a3f32 scale = 1;

		if (!file)
		{
			printf("Cannot find file!");
			return -1;			
		}

		while (fgets(line, sizeof(line), file) != NULL)
		{
			if (strstr(line, "[Header]"))
			{
				if (!handleHeaders(hierarchy_out, poseGroup_out, file, line, &scale))
				{
					return -1;
				}
			}		
			else if (strstr(line, "[SegmentNames&Hierarchy]"))
			{
				if (!handleSegmentsAndHierarchy(hierarchy_out, file, line))
				{					
					return -1;
				}
			}		
			else if (strstr(line, "[BasePosition]"))
			{
				if (!handleBasePosition(hierarchy_out, poseGroup_out, file, line, scale))
				{
					return -1;
				}
			}
			else if (line[0] == '#')
			{
				if (!true)
				{
					return -1;
				}
			}
		}

		fclose(file);

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return 1;
}

// load BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupLoadBVH(a3_HierarchyPoseGroup* poseGroup_out, a3_Hierarchy* hierarchy_out, const a3byte* resourceFilePath)
{
	if (poseGroup_out && !poseGroup_out->hierarchy && hierarchy_out && !hierarchy_out->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save HTR file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveHTR(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}

// save BVH file, read and store complete pose group and hierarchy
a3i32 a3hierarchyPoseGroupSaveBVH(const a3_HierarchyPoseGroup* poseGroup_in, const a3_Hierarchy* hierarchy_in, const a3byte* resourceFilePath)
{
	if (poseGroup_in && poseGroup_in->hierarchy && hierarchy_in && hierarchy_in->nodes && resourceFilePath && *resourceFilePath)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-OPTIONAL: IMPLEMENT ME
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//****END-TO-DO-OPTIONAL
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
