#include "stdafx.h"
#include "animationData.h"

Keyframe::Keyframe()
	: TimePos(0.0f),
	Translation(0.0f, 0.0f, 0.0f),
	Scale(1.0f, 1.0f, 1.0f),
	RotationQuat(0.0f, 0.0f, 0.0f, 1.0f)
{}

Keyframe::~Keyframe()
{}

float BoneAnimation::GetStartTime() const
{
	// Keyframes are sorted by time, so first keyframe gives start time.
	return Keyframes.front().TimePos;
}

float BoneAnimation::GetEndTime() const
{
	// Keyframes are sorted by time, so last keyframe gives end time.
	float f = Keyframes.back().TimePos;

	return f;
}

void BoneAnimation::Interpolate(
	_In_ float t,
	_Out_ XMFLOAT4X4& M,
	_In_opt_ bool interpolate) const
{
	if (t <= Keyframes.front().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.front().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframes.front().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframes.front().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else if (t >= Keyframes.back().TimePos)
	{
		XMVECTOR S = XMLoadFloat3(&Keyframes.back().Scale);
		XMVECTOR P = XMLoadFloat3(&Keyframes.back().Translation);
		XMVECTOR Q = XMLoadFloat4(&Keyframes.back().RotationQuat);

		XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
	}
	else
	{
		for (UINT i = 0; i < Keyframes.size() - 1; ++i)
		{
			if (t >= Keyframes[i].TimePos && t <= Keyframes[i + 1].TimePos)
			{
				XMVECTOR zero = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

				if (interpolate)
				{
					float lerpPercent = (t - Keyframes[i].TimePos) / (Keyframes[i + 1].TimePos - Keyframes[i].TimePos);

					XMVECTOR s0 = XMLoadFloat3(&Keyframes[i].Scale);
					XMVECTOR s1 = XMLoadFloat3(&Keyframes[i + 1].Scale);

					XMVECTOR p0 = XMLoadFloat3(&Keyframes[i].Translation);
					XMVECTOR p1 = XMLoadFloat3(&Keyframes[i + 1].Translation);

					XMVECTOR q0 = XMLoadFloat4(&Keyframes[i].RotationQuat);
					XMVECTOR q1 = XMLoadFloat4(&Keyframes[i + 1].RotationQuat);

					XMVECTOR S = XMVectorLerp(s0, s1, lerpPercent);
					XMVECTOR P = XMVectorLerp(p0, p1, lerpPercent);
					XMVECTOR Q = XMQuaternionSlerp(q0, q1, lerpPercent);

					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
				}
				else
				{
					XMVECTOR S = XMLoadFloat3(&Keyframes[i].Scale);
					XMVECTOR P = XMLoadFloat3(&Keyframes[i].Translation);
					XMVECTOR Q = XMLoadFloat4(&Keyframes[i].RotationQuat);

					XMStoreFloat4x4(&M, XMMatrixAffineTransformation(S, zero, Q, P));
				}

				break;
			}
		}
	}
	AnimationData::Transpose(M);
}

//float AnimationClip::GetClipStartTime() const
//{
//	// Find smallest start time over all bones in this clip.
//	//float t = MathHelper::Infinity;
//	//for(UINT i = 0; i < BoneAnimations.size(); ++i)
//	//{
//	//	t = MathHelper::Min(t, BoneAnimations[i].GetStartTime());
//	//}
//
//	//return t;
//}
//
//float AnimationClip::GetClipEndTime() const
//{
//	// Find largest end time over all bones in this clip.
//	//float t = 0.0f;
//	//for(UINT i = 0; i < BoneAnimations.size(); ++i)
//	//{
//	//	t = MathHelper::Max(t, BoneAnimations[i].GetEndTime());
//	//}
//
//	//return t;
//}

void AnimationClip::Interpolate(
	_In_ float t,
	_Out_ std::vector<XMFLOAT4X4>& boneTransforms,
	_In_opt_ bool interpolate) const
{
	boneTransforms.resize(BoneAnimations.size());

	for (UINT i = 0; i < BoneAnimations.size(); ++i)
	{
		BoneAnimations[i].Interpolate(t, boneTransforms[i], interpolate);
	}
}

const char * AnimationClip::getBoneNameAt(int idx)
{
	if (BoneAnimations.size() > 0)
	{
		return BoneAnimations[idx].Name.c_str();
	}
	else
	{
		return nullptr;
	}
}

float AnimationClip::GetTimeStepAtFrame(_In_ UINT frame) const
{
	return ((Duration / TotalFrames) * frame);
}

UINT AnimationData::BoneCount() const
{
	UINT boneCount = 0;
	for (const auto& animation : _animations)
	{
		boneCount += static_cast<UINT>(animation.second.BoneAnimations.size());
	}
	return boneCount;
}

//float Engine::AnimationData::GetClipStartTime(_In_ const std::wstring& clipName ) const
//{
//	auto clip = _animations.find(clipName);
//	return clip->second.GetClipStartTime();
//}
//
//float Engine::AnimationData::GetClipEndTime(_In_ const std::wstring& clipName ) const
//{
//	auto clip = _animations.find(clipName);
//	return clip->second.GetClipEndTime();
//}

//void Engine::AnimationData::Set(
//	_In_ std::vector<int>& boneHierarchy,
//	_In_ std::vector<XMFLOAT4X4>& boneOffsets,
//	_In_ std::map<std::wstring, AnimationClip>& animations)
//{
//	_boneHierarchy = boneHierarchy;
//	_boneOffsets   = boneOffsets;
//	_animations    = animations;
//
//	// engine currently supports only 1 clip that contains all animations, but it
//	// retains a lot of code to allow for changing this in the future
//	//ASSERT_TRUE(_animations.size() == 1);
//	for (auto it : _animations)
//	{
//		_clipName = it.second.Name;
//	}
//}

void AnimationData::GetFinalTransforms(
	//_In_ const std::wstring& clipName,
	_In_ float timePos,
	_In_ std::vector<XMFLOAT4X4>& localTransforms,
	_In_opt_ bool interpolate) const
{
	if (_animations.size() == 0)
	{
		return;
	}

	//UINT numBones = (UINT)_boneOffsets.size();

	//std::vector<XMFLOAT4X4> toParentTransforms(numBones);

	// Interpolate all the bones of this clip at the given time instance.
	auto clip = _animations.find(_clipName);
	clip->second.Interpolate(timePos, localTransforms, interpolate);
}

float AnimationData::GetTimeStepAtFrame(
	//_In_ const std::wstring& clipName, 
	_In_ UINT frame) const
{
	if (_animations.size() == 0)
	{
		return 0.0f;
	}

	auto clip = _animations.find(_clipName);
	return clip->second.GetTimeStepAtFrame(frame);
}
