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
			_mAnimationChromaLink->Play(loop);
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Play(loop);
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Play(loop);
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Play(loop);
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Play(loop);
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Play(loop);
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
			_mAnimationChromaLink->Stop();
		}
		if (_mAnimationHeadset != nullptr)
		{
			_mAnimationHeadset->Stop();
		}
		if (_mAnimationKeyboard != nullptr)
		{
			_mAnimationKeyboard->Stop();
		}
		if (_mAnimationKeypad != nullptr)
		{
			_mAnimationKeypad->Stop();
		}
		if (_mAnimationMouse != nullptr)
		{
			_mAnimationMouse->Stop();
		}
		if (_mAnimationMousepad != nullptr)
		{
			_mAnimationMousepad->Stop();
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
}
