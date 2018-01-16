#pragma once

#include "AnimationBase.h"

class EditorAnimationBase
{
public:
	virtual ChromaSDK::AnimationBase* GetAnimation();
	virtual void SetAnimation(ChromaSDK::AnimationBase* animation);
	int GetCurrentFrame();
	void SetCurrentFrame(unsigned int index);
	unsigned int GetFrameCount();
	virtual float GetDuration(unsigned int index) = 0;
	virtual void OverrideTime(float time) = 0;
	void ImportTextureImage();
	void ImportTextureAnimation();
	void ReadImage(const std::string& texturePath, bool isAnimation);
	virtual void CopyPixels(COLORREF* pColor, UINT width, UINT height) = 0;
	virtual void AddFrame() = 0;
	void SetPath(const std::string& path);
	virtual void Reset() = 0;
	virtual void Stop();
	virtual void SaveStream(FILE* stream);
private:
	std::string _mPath;
	ChromaSDK::AnimationBase* _mAnimation;
};
