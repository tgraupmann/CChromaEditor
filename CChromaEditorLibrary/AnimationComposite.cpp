#include "stdafx.h"
#include "AnimationComposite.h"

namespace ChromaSDK
{
	AnimationComposite::AnimationComposite()
	{
		_mAnimationChromaLink = nullptr;
		_mAnimationHeadset = nullptr;
		_mAnimationKeyboard = nullptr;
		_mAnimationKeypad = nullptr;
		_mAnimationMouse = nullptr;
		_mAnimationMousepad = nullptr;
	}
	AnimationComposite::~AnimationComposite()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationChromaLink));
		}
		if (_mAnimationHeadset != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationHeadset));
		}
		if (_mAnimationKeyboard != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationKeyboard));
		}
		if (_mAnimationKeypad != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationKeypad));
		}
		if (_mAnimationMouse != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationMouse));
		}
		if (_mAnimationMousepad != nullptr)
		{
			PluginCloseAnimation(PluginGetAnimationIdFromInstance(_mAnimationMousepad));
		}
	}
	Animation1D* AnimationComposite::GetChromaLink()
	{
		return _mAnimationChromaLink;
	}
	Animation1D* AnimationComposite::GetHeadset()
	{
		return _mAnimationHeadset;
	}
	Animation2D* AnimationComposite::GetKeyboard()
	{
		return _mAnimationKeyboard;
	}
	Animation2D* AnimationComposite::GetKeypad()
	{
		return _mAnimationKeypad;
	}
	Animation2D* AnimationComposite::GetMouse()
	{
		return _mAnimationMouse;
	}
	Animation1D* AnimationComposite::GetMousepad()
	{
		return _mAnimationMousepad;
	}
	AnimationBase* AnimationComposite::GetAnimation(EChromaSDKDeviceTypeEnum device)
	{
		switch (device)
		{
		case EChromaSDKDeviceEnum::EDIT_ChromaLink:
			return GetChromaLink();
		case EChromaSDKDeviceEnum::EDIT_Headset:
			return GetHeadset();
		case EChromaSDKDeviceEnum::EDIT_Keyboard:
			return GetKeyboard();
		case EChromaSDKDeviceEnum::EDIT_Keypad:
			return GetKeypad();
		case EChromaSDKDeviceEnum::EDIT_Mouse:
			return GetMouse();
		case EChromaSDKDeviceEnum::EDIT_Mousepad:
			return GetMousepad();
		}
	}
	void AnimationComposite::SetChromaLink(Animation1D* animation)
	{
		_mAnimationChromaLink = animation;
	}
	void AnimationComposite::SetHeadset(Animation1D* animation)
	{
		_mAnimationHeadset = animation;
	}
	void AnimationComposite::SetKeyboard(Animation2D* animation)
	{
		_mAnimationKeyboard = animation;
	}
	void AnimationComposite::SetKeypad(Animation2D* animation)
	{
		_mAnimationKeypad = animation;
	}
	void AnimationComposite::SetMouse(Animation2D* animation)
	{
		_mAnimationMouse = animation;
	}
	void AnimationComposite::SetMousepad(Animation1D* animation)
	{
		_mAnimationMousepad = animation;
	}

	void AnimationComposite::Reset()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Reset();
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Reset();
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Reset();
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Reset();
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Reset();
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Reset();
		}
	}

	void AnimationComposite::Load()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Load();
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Load();
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Load();
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Load();
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Load();
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Load();
		}
	}

	void AnimationComposite::Unload()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Unload();
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Unload();
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Unload();
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Unload();
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Unload();
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Unload();
		}
	}

	void AnimationComposite::Play(bool loop)
	{
		if (_mAnimationChromaLink != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationChromaLink), loop);
		}
		if (_mAnimationHeadset != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationHeadset), loop);
		}
		if (_mAnimationKeyboard != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationKeyboard), loop);
		}
		if (_mAnimationKeypad != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationKeypad), loop);
		}
		if (_mAnimationMouse != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationMouse), loop);
		}
		if (_mAnimationMousepad != nullptr)
		{
			PluginPlayAnimationLoop(PluginGetAnimationIdFromInstance(_mAnimationMousepad), loop);
		}
	}

	bool AnimationComposite::IsPlaying()
	{
		if (_mAnimationChromaLink != nullptr &&
			_mAnimationChromaLink->IsPlaying())
		{
			return true;
		}
		if (_mAnimationHeadset != nullptr &&
			_mAnimationHeadset->IsPlaying())
		{
			return true;
		}
		if (_mAnimationKeyboard != nullptr &&
			_mAnimationKeyboard->IsPlaying())
		{
			return true;
		}
		if (_mAnimationKeypad != nullptr &&
			_mAnimationKeypad->IsPlaying())
		{
			return true;
		}
		if (_mAnimationMouse != nullptr &&
			_mAnimationMouse->IsPlaying())
		{
			return true;
		}
		if (_mAnimationMousepad != nullptr &&
			_mAnimationMousepad->IsPlaying())
		{
			return true;
		}
	}

	void AnimationComposite::Pause()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Pause();
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Pause();
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Pause();
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Pause();
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Pause();
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Pause();
		}
	}

	void AnimationComposite::Resume(bool loop)
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Resume(loop);
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Resume(loop);
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Resume(loop);
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Resume(loop);
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Resume(loop);
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Resume(loop);
		}
	}

	void AnimationComposite::Stop()
	{
		if (_mAnimationChromaLink != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationChromaLink));
		}
		if (_mAnimationHeadset != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationHeadset));
		}
		if (_mAnimationKeyboard != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationKeyboard));
		}
		if (_mAnimationKeypad != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationKeypad));
		}
		if (_mAnimationMouse != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationMouse));
		}
		if (_mAnimationMousepad != nullptr)
		{
			PluginStopAnimation(PluginGetAnimationIdFromInstance(_mAnimationMousepad));
		}
	}

	void AnimationComposite::Update(float deltaTime)
	{
		if (_mAnimationChromaLink != nullptr)
		{
			_mAnimationChromaLink->Update(deltaTime);
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Update(deltaTime);
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Update(deltaTime);
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Update(deltaTime);
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Update(deltaTime);
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Update(deltaTime);
		}
	}

	int AnimationComposite::Save(const char* path)
	{
		FILE* stream;
		int result = fopen_s(&stream, path, "wb");
		if (result == 13)
		{
			fprintf(stderr, "Save: Permission denied! %s\r\n", path);
		}
		else if (0 == result &&
			stream)
		{
			if (_mAnimationChromaLink != nullptr)
			{
				_mAnimationChromaLink->SaveStream(stream);
			}
			if (_mAnimationHeadset != nullptr)
			{
				_mAnimationHeadset->SaveStream(stream);
			}
			if (_mAnimationKeyboard != nullptr)
			{
				_mAnimationKeyboard->SaveStream(stream);
			}
			if (_mAnimationKeypad != nullptr)
			{
				_mAnimationKeypad->SaveStream(stream);
			}
			if (_mAnimationMouse != nullptr)
			{
				_mAnimationMouse->SaveStream(stream);
			}
			if (_mAnimationMousepad != nullptr)
			{
				_mAnimationMousepad->SaveStream(stream);
			}

			fflush(stream);
			std::fclose(stream);
		}
		return -1;
	}
	//???
	EChromaSDKDeviceTypeEnum AnimationComposite::GetDeviceType()
	{
		return EChromaSDKDeviceTypeEnum::DE_Composite;
	}
	int AnimationComposite::GetDeviceId()
	{
		return -1;
	}
	void AnimationComposite::ResetFrames()
	{

	}
	int AnimationComposite::GetFrameCount()
	{
		return 0;
	}
	void AnimationComposite::SaveStream(FILE* stream)
	{

	}
}
