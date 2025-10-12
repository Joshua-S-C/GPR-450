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
	
	a3_SpatialPose.c
	Implementation of spatial pose.
*/

#include "../a3_SpatialPose.h"


//-----------------------------------------------------------------------------

// convert single node pose to matrix
a3i32 a3spatialPoseConvert(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		// Build transformation by channel (to-do: check channels): 
		//	-> final result: 
		//		v' = t + R * S * v
		//		T = { R * S (upper-left 3x3); t (upper-right 1x3) }
		//	-> build and concatenate rotations
		//		-> e.g. R(z,y,x) = R(z) * R(y) * R(z))
		//			(or use existing function if all rotation channels enabled)
		//	-> apply scale by multiplying rotation columns by respective scale component
		//		-> e.g. R * S = { R[0] * S[0,0]; R[1] * S[1,1]; R[2] * S[2,2] }
		//			(more efficient than full matrix multiplication)
		//	-> add translation in fourth column
		//test:
		//a3real4x4SetRotateZYX(spatialPose->transformMat.m, spatialPose->rotate.x, spatialPose->rotate.y, spatialPose->rotate.z);

		// compute rotation
		a3mat4 Rx, Ry, Rz, R;
		a3real4x4SetRotateX(Rx.m, a3trigValid_sind(spatialPose->rotate.x));
		a3real4x4SetRotateY(Ry.m, a3trigValid_sind(spatialPose->rotate.y));
		a3real4x4SetRotateZ(Rz.m, a3trigValid_sind(spatialPose->rotate.z));
		switch (order)
		{
		case a3poseEulerOrder_xyz:
			a3real4x4Product(R.m, Rx.m, Ry.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Rz.m);
			break;
		case a3poseEulerOrder_yzx:
			a3real4x4Product(R.m, Ry.m, Rz.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Rx.m);
			break;
		case a3poseEulerOrder_zxy:
			a3real4x4Product(R.m, Rz.m, Rx.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Ry.m);
			break;
		case a3poseEulerOrder_yxz:
			a3real4x4Product(R.m, Ry.m, Rx.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Rz.m);
			break;
		case a3poseEulerOrder_xzy:
			a3real4x4Product(R.m, Rx.m, Rz.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Ry.m);
			break;
		case a3poseEulerOrder_zyx:
			a3real4x4Product(R.m, Rz.m, Ry.m);
			a3real4x4Product(spatialPose->transformMat.m, R.m, Rx.m);
			break;
		}

		// multiply columns by respective scale
		a3real3MulS(spatialPose->transformMat.v0.v, spatialPose->scale.x);
		a3real3MulS(spatialPose->transformMat.v1.v, spatialPose->scale.y);
		a3real3MulS(spatialPose->transformMat.v2.v, spatialPose->scale.z);

		// add translation in fourth column
		spatialPose->transformMat.v3 = spatialPose->translate;
		
		// Done
		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// restore single node pose from matrix
extern double asin(double y_r);
extern double atan2(double y, double x);
a3i32 a3spatialPoseRestore(a3_SpatialPose* spatialPose, const a3_SpatialPoseChannel channel, const a3_SpatialPoseEulerOrder order)
{
	if (spatialPose)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-3: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		// ****HINT: this is the opposite of the above function!
		// If you understand how the transformation was assembled, 
		// then the reverse process is used to pull break it down.
		// ****HINT: for rotation, first focus on ZYX order: 
		// 
		// R(z,y,x) = R(z) * R(y) * R(x)
		// 
		//   {  cz  -sz  0 } {  cy  0   sy } { 1   0    0  }
		// = {  sz   cz  0 } {  0   1   0  } { 0   cx  -sx }
		//   {  0    0   1 } { -sy  0   cy } { 0   sx   cx }
		// 
		//   { cz cy  cz sy sx - sz cx  cz sy cx + sz sx }
		// = { sz cy  sz sy sx + cz cx  sz sy cx - cz sx }
		//   {   -sy     cy sx             cy cx         }
		//

		a3mat3 R = a3mat3_identity;

		// Copy translation from fourth column
		spatialPose->translate = spatialPose->transformMat.v3;

		// Get scale from columns
		spatialPose->scale.x = a3real3Length(spatialPose->transformMat.v0.v);
		spatialPose->scale.y = a3real3Length(spatialPose->transformMat.v1.v);
		spatialPose->scale.z = a3real3Length(spatialPose->transformMat.v2.v);

		// Get pure rotation
		a3real3QuotientS(R.v0.v, spatialPose->transformMat.v0.v, spatialPose->scale.x);
		a3real3QuotientS(R.v1.v, spatialPose->transformMat.v1.v, spatialPose->scale.y);
		a3real3QuotientS(R.v2.v, spatialPose->transformMat.v2.v, spatialPose->scale.z);

		// Extract angles
		spatialPose->rotate.x = a3real_rad2deg * (a3f32)atan2(R.v1.z, R.v2.z);
		spatialPose->rotate.y = a3real_rad2deg * (a3f32)asin(-R.v0.z);
		spatialPose->rotate.z = a3real_rad2deg * (a3f32)atan2(R.v0.y, R.v0.x);

		// Done
		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-3
//-----------------------------------------------------------------------------
	}
	return -1;
}

// copy operation for single node pose
a3i32 a3spatialPoseCopy(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_in)
{
	if (spatialPose_out && spatialPose_in)
	{
		*spatialPose_out = *spatialPose_in;
		return 1;
	}
	return -1;
}

// concat
a3i32 a3spatialPoseConcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
 
		// angles: addition is ok for the purposes of what we're doing
		// to-do: check channels
		spatialPose_out->rotate.x = spatialPose_lhs->rotate.x + spatialPose_rhs->rotate.x;
		spatialPose_out->rotate.y = spatialPose_lhs->rotate.y + spatialPose_rhs->rotate.y;
		spatialPose_out->rotate.z = spatialPose_lhs->rotate.z + spatialPose_rhs->rotate.z;

		// scale: multiplication
		// to-do: check channels
		spatialPose_out->scale.x = spatialPose_lhs->scale.x * spatialPose_rhs->scale.x;
		spatialPose_out->scale.y = spatialPose_lhs->scale.y * spatialPose_rhs->scale.y;
		spatialPose_out->scale.z = spatialPose_lhs->scale.z * spatialPose_rhs->scale.z;

		// translation: addition
		// to-so: check channels
		spatialPose_out->translate.x = spatialPose_lhs->translate.x + spatialPose_rhs->translate.x;
		spatialPose_out->translate.y = spatialPose_lhs->translate.y + spatialPose_rhs->translate.y;
		spatialPose_out->translate.z = spatialPose_lhs->translate.z + spatialPose_rhs->translate.z;

		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// deconcat
a3i32 a3spatialPoseDeconcat(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_lhs, const a3_SpatialPose* spatialPose_rhs)
{
	if (spatialPose_out && spatialPose_lhs && spatialPose_rhs)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------
		
		// angles: subtraction is ok for the purposes of what we're doing
		// to-do: check channels
		spatialPose_out->rotate.x = spatialPose_lhs->rotate.x - spatialPose_rhs->rotate.x;
		spatialPose_out->rotate.y = spatialPose_lhs->rotate.y - spatialPose_rhs->rotate.y;
		spatialPose_out->rotate.z = spatialPose_lhs->rotate.z - spatialPose_rhs->rotate.z;

		// scale: division
		// to-do: check channels
		spatialPose_out->scale.x = spatialPose_lhs->scale.x / spatialPose_rhs->scale.x;
		spatialPose_out->scale.y = spatialPose_lhs->scale.y / spatialPose_rhs->scale.y;
		spatialPose_out->scale.z = spatialPose_lhs->scale.z / spatialPose_rhs->scale.z;

		// translation: subtraction
		// to-so: check channels
		spatialPose_out->translate.x = spatialPose_lhs->translate.x - spatialPose_rhs->translate.x;
		spatialPose_out->translate.y = spatialPose_lhs->translate.y - spatialPose_rhs->translate.y;
		spatialPose_out->translate.z = spatialPose_lhs->translate.z - spatialPose_rhs->translate.z;

		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}

// lerp
extern float powf(float b, float e);//#include <math.h>
a3i32 a3spatialPoseLerp(a3_SpatialPose* spatialPose_out, const a3_SpatialPose* spatialPose_0, const a3_SpatialPose* spatialPose_1, const a3real u)
{
	if (spatialPose_out && spatialPose_0 && spatialPose_1)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-2: IMPLEMENT ME
//-----------------------------------------------------------------------------

		// additive (addition and multiplication): 
		//	lerp[x0,x1](u) = (x1 - x0) * u + x0
		// multiplicative (multiplication and exponentiation): 
		//	loglerp[x0,x1](u) = (x1 / x0) ^ u * x0

		// angles: lerp is ok for the purposes of what we're doing
		// to-do: check channels
		spatialPose_out->rotate.v[0] = (spatialPose_1->rotate.v[0] - spatialPose_0->rotate.v[0]) * u + spatialPose_0->rotate.v[0];
		spatialPose_out->rotate.v[1] = (spatialPose_1->rotate.v[1] - spatialPose_0->rotate.v[1]) * u + spatialPose_0->rotate.v[1];
		spatialPose_out->rotate.v[2] = (spatialPose_1->rotate.v[2] - spatialPose_0->rotate.v[2]) * u + spatialPose_0->rotate.v[2];

		// scale: log-lerp
		// to-do: check channels
		spatialPose_out->scale.v[0] = powf(spatialPose_1->scale.v[0] / spatialPose_0->scale.v[0], u) * spatialPose_0->scale.v[0];
		spatialPose_out->scale.v[1] = powf(spatialPose_1->scale.v[1] / spatialPose_0->scale.v[1], u) * spatialPose_0->scale.v[1];
		spatialPose_out->scale.v[2] = powf(spatialPose_1->scale.v[2] / spatialPose_0->scale.v[2], u) * spatialPose_0->scale.v[2];

		// translate: lerp
		// to-do: check channels
		spatialPose_out->translate.v[0] = (spatialPose_1->translate.v[0] - spatialPose_0->translate.v[0]) * u + spatialPose_0->translate.v[0];
		spatialPose_out->translate.v[1] = (spatialPose_1->translate.v[1] - spatialPose_0->translate.v[1]) * u + spatialPose_0->translate.v[1];
		spatialPose_out->translate.v[2] = (spatialPose_1->translate.v[2] - spatialPose_0->translate.v[2]) * u + spatialPose_0->translate.v[2];

		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-2
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------

a3ui8 a3basisAxisIndex(a3_BasisAxis const axis)
{
	return (axis & 0x0F);
}

a3ui8 a3basisAxisSign(a3_BasisAxis const axis)
{
	return (axis & 0xF0) >> 4;
}

a3boolean a3basisAxisValid(a3_BasisAxis const axis)
{
	return (a3basisAxisIndex(axis) < 3) && (a3basisAxisSign(axis) < 2);
}

a3_Basis a3basisInit(a3_BasisAxis const fwd, a3_BasisAxis const up)
{
	if (!a3basisAxisValid(fwd) || !a3basisAxisValid(up))
		return basis_invalid;
	if (a3basisAxisIndex(fwd) == a3basisAxisIndex(up))
		return basis_invalid;
	return ((a3_Basis)(fwd & 0xFF)) | ((a3_Basis)(up & 0xFF) << 8);
}

a3boolean a3basisExtract(a3_BasisAxis* fwd_out, a3_BasisAxis* up_out, a3_Basis const basis)
{
	a3_BasisAxis fwd, up;
	if (!fwd_out || !up_out)
		return false;
	*fwd_out = basis_invalid;
	*up_out = basis_invalid;
	fwd = (basis) & 0xFF;
	up = (basis >> 8) & 0xFF;
	if (a3basisInit(fwd, up) != basis)
		return false;
	*fwd_out = fwd;
	*up_out = up;
	return true;
}

a3real3x3r a3basisToMat3(a3real3x3 m_out, a3_Basis const basis)
{
	a3ui32 i_fwd, i_up;
	a3_BasisAxis fwd, up;
	if (!m_out)
		return 0;
	if (!a3basisExtract(&fwd, &up, basis))
		return 0;
	a3real3x3SetNonUnif(m_out, 0, 0, 0);
	i_fwd = fwd & 0x0F;
	i_up = up & 0x0F;
	m_out[1][(i_fwd) % 3] = (fwd & 0xF0) ? -a3real_one : +a3real_one;
	m_out[2][(i_up) % 3] = (up & 0xF0) ? -a3real_one : +a3real_one;
	a3real3Cross(m_out[0], m_out[1], m_out[2]);
	return m_out;
}

a3real4x4r a3basisToMat4(a3real4x4 m_out, a3_Basis const basis)
{
	a3ui32 i_fwd, i_up;
	a3_BasisAxis fwd, up;
	if (!m_out)
		return 0;
	if (!a3basisExtract(&fwd, &up, basis))
		return 0;
	a3real4x4SetNonUnif(m_out, 0, 0, 0);
	i_fwd = fwd & 0x0F;
	i_up = up & 0x0F;
	m_out[1][(i_fwd) % 3] = (fwd & 0xF0) ? -a3real_one : +a3real_one;
	m_out[2][(i_up) % 3] = (up & 0xF0) ? -a3real_one : +a3real_one;
	a3real3Cross(m_out[0], m_out[1], m_out[2]);
	return m_out;
}


//-----------------------------------------------------------------------------
