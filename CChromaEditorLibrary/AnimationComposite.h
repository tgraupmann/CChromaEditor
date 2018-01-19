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
		~AnimationComposite();
		Animation1D* GetChromaLink();
		Animation1D* GetHeadset();
		Animation2D* GetKeyboard();
		Animation2D* GetKeypad();
		Animation2D* GetMouse();
		Animation1D* GetMousepad();
		AnimationBase* GetAnimation(EChromaSDKDeviceTypeEnum device);
		void SetChromaLink(Animation1D* animation);
		void SetHeadset(Animation1D* animation);
		void SetKeyboard(Animation2D* animation);
		void SetKeypad(Animation2D* animation);
		void SetMouse(Animation2D* animation);
		void SetMousepad(Animation1D* animation);
		void Reset();
		void Load();
		void Unload();
		void Play(bool loop);
		virtual bool IsPlaying();
		void Pause();
		void Resume(bool loop);
		void Stop();
		void Update(float deltaTime);
		int Save(const char* path);
		// ???
		virtual EChromaSDKDeviceTypeEnum GetDeviceType();
		virtual int GetDeviceId();
		virtual void ResetFrames();
		virtual int GetFrameCount();
		virtual void SaveStream(FILE* stream);
	private:
		Animation1D* _mAnimationChromaLink;
		Animation1D* _mAnimationHeadset;
		Animation2D* _mAnimationKeyboard;
		Animation2D* _mAnimationKeypad;
		Animation2D* _mAnimationMouse;
		Animation1D* _mAnimationMousepad;
	};
}
