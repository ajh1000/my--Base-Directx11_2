#pragma once

	struct Keyframe
	{
		Keyframe();
		~Keyframe();

		float TimePos;
		XMFLOAT3 Translation;
		XMFLOAT3 Scale;
		XMFLOAT4 RotationQuat;
	};
	struct BoneAnimation
	{
		float GetStartTime() const;
		float GetEndTime() const;

		void Interpolate(
			_In_ float t,
			_Out_ XMFLOAT4X4& M,
			_In_opt_ bool interpolate = true) const;

		std::vector<Keyframe> Keyframes;
		std::string Name;
	};

	struct AnimationClip
	{
		float GetTimeStepAtFrame(_In_ UINT frame) const;

		void Interpolate(
			_In_ float t,
			_Out_ std::vector<XMFLOAT4X4>& boneTransforms,
			_In_opt_ bool interpolate = true) const;

		const char* getBoneNameAt(int idx);

		std::vector<BoneAnimation> BoneAnimations;
		float TicksPerSecond;
		float Duration;
		std::string Name;
		UINT TotalFrames;
	};

	class AnimationData
	{
	public:

		UINT BoneCount() const;

		inline static void Transpose(_In_ XMFLOAT4X4& m)
		{
			XMMATRIX M = XMLoadFloat4x4(&m);

			M = XMMatrixTranspose(M);

			XMStoreFloat4x4(&m, M);
		}

		bool ContainsData() const
		{
			return _animations.size() > 0;
		}

		//float GetClipStartTime(_In_ const std::string& clipName) const;
		//float GetClipEndTime(_In_ const std::string& clipName) const;
		float GetTimeStepAtFrame(
			//_In_ const std::string& clipName, 
			_In_ UINT frame) const;

		//void Set(
		//	_In_ std::vector<int>& boneHierarchy, 
		//	_In_ std::vector<XMFLOAT4X4>& boneOffsets,
		//	_In_ std::map<std::string, AnimationClip>& animations);

		void AddAnimationClip(_In_ AnimationClip animationClip)
		{
			animationClip.TotalFrames = static_cast<UINT>(animationClip.Duration * 30.0f);
			_animations[animationClip.Name] = animationClip;
			_clipName = animationClip.Name;
		}

		void SetCurrentClip(_In_ std::string clipName)
		{
			_clipName = clipName;
		}

		// TODO: cache the result if there was a chance
		// that you were calling this several times with the same clipName at 
		// the same timePos.
		void GetFinalTransforms(
			//_In_ const std::string& clipName, 
			_In_ float timePos,
			_In_ std::vector<XMFLOAT4X4>& localTransforms,
			_In_opt_ bool interpolate = true) const;

	private:

		std::map<std::string, AnimationClip> _animations;

		// The name for the combined clip (engine currently supports only 1 clip that contains all animations, but it
		// retains a lot of code to allow for changing this in the future)
		std::string _clipName;
	};
