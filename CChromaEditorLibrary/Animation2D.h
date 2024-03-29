#pragma once

#include "ChromaSDKPluginTypes.h"
#include "AnimationBase.h"

namespace ChromaSDK
{
	class Animation2D : public AnimationBase
	{
	public:
		Animation2D();
		void Reset();
		virtual EChromaSDKDeviceEnum GetDeviceEnum();
		EChromaSDKDeviceTypeEnum GetDeviceType();
		EChromaSDKDevice2DEnum GetDevice();
		bool SetDevice(EChromaSDKDevice2DEnum device);
		int GetDeviceId();
		std::vector<FChromaSDKColorFrame2D>& GetFrames();
		int GetFrameCount();
		float GetDuration(unsigned int index);
		void Load();
		void Unload();
		void Play(bool loop);
		void Pause();
		void Resume(bool loop);
		void Stop();
		void Update(float deltaTime);
		void ResetFrames();
		void SaveStream(FILE* stream);
		int Save(const char* path);
		void SetUseKeyboardCustomKeyType(bool enabled);
		bool HasKeyboardCustomKeyType();
	private:
		EChromaSDKDevice2DEnum _mDevice;
		std::vector<FChromaSDKColorFrame2D> _mFrames;
		bool _mUseKeyboardCustomKeyType;
	};
}
