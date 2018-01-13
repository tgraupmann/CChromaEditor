#pragma once

#include "Animation1D.h"
#include "EditorAnimationBase.h"

namespace ChromaSDK
{
	class EditorAnimation1D : public EditorAnimationBase
	{
	public:
		EditorAnimation1D();
		Animation1D* GetAnimation1D();
		EChromaSDKDevice1DEnum GetDevice();
		bool SetDevice(EChromaSDKDevice1DEnum device);
		std::vector<FChromaSDKColorFrame1D>& GetFrames();
		void SetCopy(FChromaSDKColorFrame1D& copy);
		FChromaSDKColorFrame1D& GetCopy();
		void OverrideTime(float time);
		float GetDuration(unsigned int index);
		void Reset();
		void CopyPixels(COLORREF* pColor, UINT width, UINT height);
		void AddFrame();
	private:
		FChromaSDKColorFrame1D _mFrameCopy;
	};
}
