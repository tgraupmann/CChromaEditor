#pragma once

#include "ChromaSDKPluginTypes.h"
#include "AnimationBase.h"
#include "Animation1D.h"
#include "Animation2D.h"

namespace ChromaSDK
{
	class AnimationComposite : public AnimationBase
	{
	public:
		AnimationComposite();
		void Reset();
		void Load();
		void Unload();
		void Play(bool loop);
		void Pause();
		void Resume(bool loop);
		void Stop();
		void Update(float deltaTime);
		int Save(const char* path);
	private:
		Animation1D* _mAnimationChromaLink;
		Animation1D* _mAnimationHeadset;
		Animation2D* _mAnimationKeyboard;
		Animation2D* _mAnimationKeypad;
		Animation2D* _mAnimationMouse;
		Animation1D* _mAnimationMousepad;
	};
}
