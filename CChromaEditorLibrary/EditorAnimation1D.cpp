#include "stdafx.h"
#include "EditorAnimation1D.h"
#include "ChromaSDKPlugin.h"

using namespace ChromaSDK;
using namespace std;

EditorAnimation1D::EditorAnimation1D()
{
}

Animation1D* EditorAnimation1D::GetAnimation1D()
{
	return (Animation1D*)GetAnimation();
}

EChromaSDKDevice1DEnum EditorAnimation1D::GetDevice()
{
	return GetAnimation1D()->GetDevice();
}

bool EditorAnimation1D::SetDevice(EChromaSDKDevice1DEnum device)
{
	if (GetAnimation1D()->SetDevice(device))
	{
		Reset();
		return true;
	}
	else
	{
		return false;
	}
}

vector<FChromaSDKColorFrame1D>& EditorAnimation1D::GetFrames()
{
	return GetAnimation1D()->GetFrames();
}

void EditorAnimation1D::Reset()
{
	EChromaSDKDevice1DEnum device = GetDevice();
	_mFrameCopy.Colors = ChromaSDKPlugin::GetInstance()->CreateColors1D(device);
}

void EditorAnimation1D::SetCopy(FChromaSDKColorFrame1D& copy)
{
	_mFrameCopy = copy;
}

FChromaSDKColorFrame1D& EditorAnimation1D::GetCopy()
{
	return _mFrameCopy;
}

void EditorAnimation1D::OverrideTime(float time)
{
	vector<FChromaSDKColorFrame1D>& frames = GetFrames();
	for (unsigned int i = 0; i < frames.size(); ++i)
	{
		FChromaSDKColorFrame1D& frame = frames[i];
		frame.Duration = time;
	}
}

float EditorAnimation1D::GetDuration(unsigned int index)
{
	vector<FChromaSDKColorFrame1D>& frames = GetFrames();
	if (index < frames.size())
	{
		FChromaSDKColorFrame1D& frame = frames[index];
		return frame.Duration;
	}
	return 0.0f;
}

void EditorAnimation1D::CopyPixels(COLORREF* pColor, UINT width, UINT height)
{
	std::vector<COLORREF>& colors = _mFrameCopy.Colors;
	for (int i = 0; i < (int)height; i++)
	{
		COLORREF* nextRow = pColor + width;
		vector<int> row = vector<int>();
		for (int j = 0; j < (int)width; j++)
		{
			int red = GetBValue(*pColor);
			int green = GetGValue(*pColor) << 8;
			int blue = GetRValue(*pColor) << 16;

			int color = red | green | blue;
			colors[j] = color;
			pColor++;
		}
		pColor = nextRow;
	}

	vector<FChromaSDKColorFrame1D>& frames = GetFrames();
	unsigned int currentFrame = GetCurrentFrame();
	if (currentFrame < 0 ||
		currentFrame >= frames.size())
	{
		currentFrame = 0;
	}
	if (currentFrame < frames.size())
	{
		frames[currentFrame] = GetCopy();
	}
}

void EditorAnimation1D::AddFrame()
{
	unsigned int currentFrame = GetCurrentFrame();
	if (currentFrame < 0 ||
		currentFrame >= GetFrameCount())
	{
		currentFrame = 0;
	}

	vector<FChromaSDKColorFrame1D>& frames = GetFrames();
	FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
	frame.Colors = ChromaSDKPlugin::GetInstance()->CreateColors1D(GetDevice());

	if (currentFrame == GetFrameCount())
	{
		frames.push_back(frame);
		currentFrame = GetFrameCount() - 1;
	}
	else
	{
		auto it = frames.begin();
		++currentFrame;
		frames.insert(it + currentFrame, frame);
	}
	SetCurrentFrame(currentFrame);
}
