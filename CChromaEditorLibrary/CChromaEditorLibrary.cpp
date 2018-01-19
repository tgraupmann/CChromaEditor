// CChromaEditorLibrary.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CChromaEditorLibrary.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TEMP_FILE "temp.chroma"
#define ANIMATION_VERSION 1
#define ID_DYNAMIC_BUTTON_MIN 2000
#define ID_DYNAMIC_COLOR_MIN 2200
#define ID_DYNAMIC_BUTTON_MAX 2256

#define DEFAULT_OVERRIDE_TIME 0.1f
#define DEFAULT_DURATION 1.0f

using namespace std;

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCChromaEditorLibraryApp

BEGIN_MESSAGE_MAP(CCChromaEditorLibraryApp, CWinApp)
END_MESSAGE_MAP()


// CCChromaEditorLibraryApp construction

CCChromaEditorLibraryApp::CCChromaEditorLibraryApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCChromaEditorLibraryApp object

CCChromaEditorLibraryApp theApp;


// CCChromaEditorLibraryApp initialization

BOOL CCChromaEditorLibraryApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CMainViewDlg::CMainViewDlg() : CDialogEx(IDD_MAIN_VIEW)
{
	_mPath = "";
	_mPlayOnOpen = false;
}

CMainViewDlg::~CMainViewDlg()
{
	OnBnClickedMenuExit();
}

void CMainViewDlg::OpenOrCreateAnimation(const std::string& path)
{
	_mPath = path;
}

void CMainViewDlg::PlayAnimationOnOpen()
{
	_mPlayOnOpen = true;
}

void CMainViewDlg::SetDeviceType(AnimationBase* animation)
{
	switch (animation->GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		switch (((Animation1D*)animation)->GetDevice())
		{
		case EChromaSDKDevice1DEnum::DE_ChromaLink:
			_mDevice = EChromaSDKDeviceEnum::EDIT_ChromaLink;
			break;
		case EChromaSDKDevice1DEnum::DE_Headset:
			_mDevice = EChromaSDKDeviceEnum::EDIT_Headset;
			break;
		case EChromaSDKDevice1DEnum::DE_Mousepad:
			_mDevice = EChromaSDKDeviceEnum::EDIT_Mousepad;
			break;
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		switch (((Animation2D*)animation)->GetDevice())
		{
		case EChromaSDKDevice2DEnum::DE_Keyboard:
			_mDevice = EChromaSDKDeviceEnum::EDIT_Keyboard;
			break;
		case EChromaSDKDevice2DEnum::DE_Keypad:
			_mDevice = EChromaSDKDeviceEnum::EDIT_Keypad;
			break;
		case EChromaSDKDevice2DEnum::DE_Mouse:
			_mDevice = EChromaSDKDeviceEnum::EDIT_Mouse;
			break;
		}
		break;
	}
}

void CMainViewDlg::LoadFile()
{
	AnimationBase* selectedAnimation = nullptr;
	AnimationBase* animation = nullptr;
	int animationId;
	if (_mPath.empty())
	{
		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_ChromaLink);
		animation = GetAnimationInstance(animationId);
		_mEditChromaLink.SetAnimation(animation);
		_mEditChromaLink.Reset();
		selectedAnimation = animation;

		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_Headset);
		animation = GetAnimationInstance(animationId);
		_mEditHeadset.SetAnimation(animation);
		_mEditHeadset.Reset();

		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Keyboard);
		animation = GetAnimationInstance(animationId);
		_mEditKeyboard.SetAnimation(animation);
		_mEditKeyboard.Reset();

		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Keypad);
		animation = GetAnimationInstance(animationId);
		_mEditKeypad.SetAnimation(animation);
		_mEditKeypad.Reset();

		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Mouse);
		animation = GetAnimationInstance(animationId);
		_mEditMouse.SetAnimation(animation);
		_mEditMouse.Reset();

		animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_Mousepad);
		animation = GetAnimationInstance(animationId);
		_mEditMousepad.SetAnimation(animation);
		_mEditMousepad.Reset();
	}
	else
	{
		FILE* stream = nullptr;
		if (0 != fopen_s(&stream, _mPath.c_str(), "rb") ||
			stream == nullptr)
		{
			LogError("OpenAnimation: Failed to open animation! %s\r\n", _mPath.c_str());
		}
		else
		{
			_mEditChromaLink.SetAnimation(nullptr);
			_mEditHeadset.SetAnimation(nullptr);
			_mEditKeyboard.SetAnimation(nullptr);
			_mEditKeypad.SetAnimation(nullptr);
			_mEditMouse.SetAnimation(nullptr);
			_mEditMousepad.SetAnimation(nullptr);

			do
			{
				animation = ChromaSDKPlugin::GetInstance()->OpenAnimationStream(stream);

				if (animation != nullptr)
				{
					if (selectedAnimation == nullptr)
					{
						selectedAnimation = animation;
					}

					string strPath = _mPath;

					switch (animation->GetDeviceType())
					{
						case EChromaSDKDeviceTypeEnum::DE_1D:
							switch (((Animation1D*)animation)->GetDevice())
							{
							case EChromaSDKDevice1DEnum::DE_ChromaLink:
								strPath += "_ChromaLink";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditChromaLink.SetAnimation(animation);
								_mEditChromaLink.Reset();
								break;
							case EChromaSDKDevice1DEnum::DE_Headset:
								strPath += "_Headset";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditHeadset.SetAnimation(animation);
								_mEditHeadset.Reset();
								break;
							case EChromaSDKDevice1DEnum::DE_Mousepad:
								strPath += "_Mousepad";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditMousepad.SetAnimation(animation);
								_mEditMousepad.Reset();
								break;
							}
							break;
						case EChromaSDKDeviceTypeEnum::DE_2D:
							switch (((Animation2D*)animation)->GetDevice())
							{
							case EChromaSDKDevice2DEnum::DE_Keyboard:
								strPath += "_Keyboard";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditKeyboard.SetAnimation(animation);
								_mEditKeyboard.Reset();
								break;
							case EChromaSDKDevice2DEnum::DE_Keypad:
								strPath += "_Keypad";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditKeypad.SetAnimation(animation);
								_mEditKeypad.Reset();
								break;
							case EChromaSDKDevice2DEnum::DE_Mouse:
								strPath += "_Mouse";
								RegisterAnimation(strPath.c_str(), animation);
								_mEditMouse.SetAnimation(animation);
								_mEditMouse.Reset();
								break;
							}
							break;
					}
				}
				
				//LogDebug("OpenAnimation: Loaded %s\r\n", _mPath.c_str());
			} while (animation != nullptr);

			std::fclose(stream);
		}

		if (selectedAnimation != nullptr)
		{
			SetDeviceType(selectedAnimation);
			SetPath(_mPath);
			SetAnimation(selectedAnimation);
			Reset();
		}
	}
}

void CMainViewDlg::SaveFile()
{
	if (!_mPath.empty())
	{
		const char* path = _mPath.c_str();

		FILE* stream;
		int result = fopen_s(&stream, path, "wb");
		if (result == 13)
		{
			fprintf(stderr, "Save: Permission denied! %s\r\n", path);
		}
		else if (0 == result &&
			stream)
		{
			_mEditChromaLink.SaveStream(stream);
			_mEditHeadset.SaveStream(stream);
			_mEditKeyboard.SaveStream(stream);
			_mEditKeypad.SaveStream(stream);
			_mEditMouse.SaveStream(stream);
			_mEditMousepad.SaveStream(stream);

			fflush(stream);
			std::fclose(stream);
		}
	}
}

CEdit* CMainViewDlg::GetControlOverrideTime()
{
	return (CEdit*)GetDlgItem(IDC_TEXT_OVERRIDE_TIME);
}

CStatic* CMainViewDlg::GetControlGridSize()
{
	return (CStatic*)GetDlgItem(IDC_STATIC_GRID_SIZE);
}

CStatic* CMainViewDlg::GetControlSetKeyLabel()
{
	return (CStatic*)GetDlgItem(IDC_STATIC_SELECT_KEY);
}

CComboBox* CMainViewDlg::GetControlSetKeyCombo()
{
	return (CComboBox*)GetDlgItem(IDC_COMBO_KEYS);
}

CButton* CMainViewDlg::GetControlSetKeyButton()
{
	return (CButton*)GetDlgItem(IDC_BUTTON_SET_KEY);
}

CStatic* CMainViewDlg::GetControlSetLEDLabel()
{
	return (CStatic*)GetDlgItem(IDC_STATIC_SELECT_LED);
}

CComboBox* CMainViewDlg::GetControlSetLEDCombo()
{
	return (CComboBox*)GetDlgItem(IDC_COMBO_LEDS);
}

CButton* CMainViewDlg::GetControlSetLEDButton()
{
	return (CButton*)GetDlgItem(IDC_BUTTON_SET_LED);
}

CStatic* CMainViewDlg::GetControlFrames()
{
	return (CStatic*)GetDlgItem(IDC_STATIC_FRAMES);
}

CEdit* CMainViewDlg::GetControlDuration()
{
	return (CEdit*)GetDlgItem(IDC_EDIT_DURATION);
}

CButton* CMainViewDlg::GetControlUseKeyboardCustomKeys()
{
	return (CButton*)GetDlgItem(IDC_CHECK_USE_KEYBOARD_CUSTOM_KEY_TYPE);
}

void CMainViewDlg::UpdateOverrideTime(float time)
{
	char buffer[10] = { 0 };
	sprintf_s(buffer, "%f", time);
	GetControlOverrideTime()->SetWindowText(CString(buffer));
	GetControlOverrideTime()->Invalidate();
}

float CMainViewDlg::GetOverrideTime()
{
	CString text;
	GetControlOverrideTime()->GetWindowText(text);
	float time = (float)_ttof(text);
	if (time <= 0.0f)
	{
		UpdateOverrideTime(DEFAULT_OVERRIDE_TIME);
		return DEFAULT_OVERRIDE_TIME;
	}
	return time;
}

void CMainViewDlg::UpdateDuration(float time)
{
	char buffer[10] = { 0 };
	sprintf_s(buffer, "%f", time);
	GetControlDuration()->SetWindowText(CString(buffer));
	GetControlDuration()->Invalidate();
}

float CMainViewDlg::GetDuration()
{
	CString text;
	GetControlDuration()->GetWindowText(text);
	float time = (float)_ttof(text);
	if (time <= 0.0f)
	{
		UpdateDuration(DEFAULT_DURATION);
		return DEFAULT_DURATION;
	}
	return time;
}

void CMainViewDlg::RefreshDevice()
{
	int show = _mDevice == EChromaSDKDeviceEnum::EDIT_Keyboard;
	GetControlSetKeyLabel()->ShowWindow(show);
	GetControlSetKeyCombo()->ShowWindow(show);
	GetControlSetKeyButton()->ShowWindow(show);

	show = _mDevice == EChromaSDKDeviceEnum::EDIT_Mouse;
	GetControlSetLEDLabel()->ShowWindow(show);
	GetControlSetLEDCombo()->ShowWindow(show);
	GetControlSetLEDButton()->ShowWindow(show);

	show = _mDevice == EChromaSDKDeviceEnum::EDIT_Keyboard && GetAnimation() != nullptr;
	GetControlUseKeyboardCustomKeys()->ShowWindow(show);
	if (show)
	{
		Animation2D* animation2D = (Animation2D*)GetAnimation();
		if (animation2D->HasKeyboardCustomKeyType())
		{
			GetControlUseKeyboardCustomKeys()->SetCheck(BST_CHECKED);
		}
		else
		{
			GetControlUseKeyboardCustomKeys()->SetCheck(BST_UNCHECKED);
		}
	}

	GetControlListTypes()->ResetContent();

	if (GetAnimation() == nullptr)
	{
		//disabled
		GetEnabledButton()->SetWindowTextW(_T("Enable"));
	}
	else
	{
		//enabled
		GetEnabledButton()->SetWindowTextW(_T("Disable"));
	}
	
	if (_mEditChromaLink.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("ChromaLink"));
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] ChromaLink"));
	}

	if (_mEditHeadset.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("Headset")); 
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] Headset"));
	}

	if (_mEditKeyboard.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("Keyboard")); 
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] Keyboard"));
	}

	if (_mEditKeypad.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("Keypad")); 
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] Keypad"));
	}

	if (_mEditMouse.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("Mouse"));
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] Mouse")); 
	}

	if (_mEditMousepad.GetAnimation() == nullptr)
	{
		GetControlListTypes()->AddString(_T("Mousepad")); 
	}
	else
	{
		GetControlListTypes()->AddString(_T("[x] Mousepad"));
	}
}

void CMainViewDlg::RecreateGrid()
{
	// clear old grid
	vector<CColorButton*>& buttons = GetGridButtons();
	for (unsigned int i = 0; i < buttons.size(); ++i)
	{
		CColorButton* button = buttons[i];
		if (button)
		{
			button->DestroyWindow();
			delete(button);
		}
	}
	buttons.clear();

	if (GetAnimation() == nullptr)
	{
		return;
	}

	COLORREF black = RGB(0, 0, 0);

	// update grid label
	int width = 15;
	int height = 30;
	int y = 280;
	int id = ID_DYNAMIC_BUTTON_MIN;
	switch (GetDeviceType())
	{
		case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			int maxLeds;
			GetDimensions1D(maxLeds);
			int x = 25;
			for (int i = 0; i < maxLeds; ++i)
			{
				CColorButton* button = new CColorButton(black, black);
				const int flags = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |
					BS_OWNERDRAW | BS_MULTILINE;
				button->Create(_T(""), flags, CRect(x, y, x + width, y + height), this, id);
				++id;
				buttons.push_back(button);
				x += width + 2;
			}
		}
		break;
		case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			int maxRow;
			int maxColumn;
			GetDimensions2D(maxRow, maxColumn);

			// create grid buttons
			for (int j = 0; j < maxRow; ++j)
			{
				int x = 25;
				for (int i = 0; i < maxColumn; ++i)
				{
					CColorButton* button = new CColorButton(black, black);
					const int flags = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |
						BS_OWNERDRAW | BS_MULTILINE;
					button->Create(_T(""), flags, CRect(x, y, x + width, y + height), this, id);
					++id;
					buttons.push_back(button);
					x += width + 2;
				}

				y += height + 2;
			}
		}
		break;
	}
}

void CMainViewDlg::RefreshGrid()
{
	if (GetAnimation() == nullptr)
	{
		return;
	}

	// update grid label
	char buffer[20] = { 0 };
	int maxLeds;
	int maxRow;
	int maxColumn;
	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (GetDimensions1D(maxLeds))
		{
			sprintf_s(buffer, "1 x %d", maxLeds);
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (GetDimensions2D(maxRow, maxColumn))
		{
			sprintf_s(buffer, "%d x %d", maxRow, maxColumn);
		}
		break;
	}
	GetControlGridSize()->SetWindowText(CString(buffer));


	// update buttons
	vector<CColorButton*>& buttons = GetGridButtons();

	int currentFrame = GetCurrentFrame();

	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				int id = 0;
				for (int i = 0; i < maxLeds; ++i)
				{
					CColorButton* button = buttons[id];
					if (button)
					{
						COLORREF color = frame.Colors[i];
						button->SetColor(color, color);
						button->Invalidate();
					}
					++id;
				}
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				int id = 0;
				for (int i = 0; i < maxRow; ++i)
				{
					FChromaSDKColors& row = frame.Colors[i];
					for (int j = 0; j < maxColumn; ++j)
					{
						CColorButton* button = buttons[id];
						if (button)
						{
							COLORREF color = row.Colors[j];
							button->SetColor(color, color);
							button->Invalidate();
						}
						++id;
					}
				}
			}
		}
		break;
	}
}

void CMainViewDlg::RefreshFrames()
{
	//update frames label
	char bufferFrameInfo[48] = { 0 };
	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();

	sprintf_s(bufferFrameInfo, "%d of %d", currentFrame + 1, frameCount);
	GetControlFrames()->SetWindowText(CString(bufferFrameInfo));

	//update the frame duration
	char bufferDuration[16] = { 0 };
	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = GetEditor1D()->GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				sprintf_s(bufferDuration, "%f", frame.Duration);
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = GetEditor2D()->GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				sprintf_s(bufferDuration, "%f", frame.Duration);
			}
		}
		break;
	}
	GetControlDuration()->SetWindowText(CString(bufferDuration));
}

CMenu* CMainViewDlg::GetControlMenu()
{
	return (CMenu*)GetDlgItem(IDR_MENU1);
}

CListBox* CMainViewDlg::GetControlListTypes()
{
	return (CListBox*)GetDlgItem(IDC_LIST_TYPES);
}

CButton* CMainViewDlg::GetEnabledButton()
{
	return (CButton*)GetDlgItem(IDC_BUTTON_ENABLE);
}

BOOL CMainViewDlg::OnInitDialog()
{
	LoadFile();

	// setup dialog
	UpdateOverrideTime(DEFAULT_OVERRIDE_TIME);

	// setup keyboard chars
	for (int key = EChromaSDKKeyboardKey::KK_ESC; key < EChromaSDKKeyboardKey::KK_INVALID; ++key)
	{
		const char* strKey = ChromaSDKPlugin::GetInstance()->GetKeyboardChar((EChromaSDKKeyboardKey)key);
		GetControlSetKeyCombo()->AddString(CString(strKey));
	}
	GetControlSetKeyCombo()->SetCurSel(0);

	// setup mouse chars
	for (int led = EChromaSDKMouseLED::ML_SCROLLWHEEL; led <= EChromaSDKMouseLED::ML_RIGHT_SIDE7; ++led)
	{
		const char* strLed = ChromaSDKPlugin::GetInstance()->GetMouseChar((EChromaSDKMouseLED)led);
		GetControlSetLEDCombo()->AddString(CString(strLed));
	}
	GetControlSetLEDCombo()->SetCurSel(0);

	// Create the grid buttons
	RecreateGrid();

	// Display enums
	RefreshDevice();

	// Display grid
	RefreshGrid();

	// DIsplay frames
	RefreshFrames();

	COLORREF black = RGB(0, 0, 0);
	COLORREF red = RGB(255, 0, 0);

	//create color picker
	int id = ID_DYNAMIC_COLOR_MIN;
	int y = 600;
	int x = 85;
	int width = 50;
	int height = 50;
	if (true)
	{
		SetColor(red);
		CColorButton* button = new CColorButton(red, red);
		const int flags = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |
			BS_OWNERDRAW | BS_MULTILINE;
		button->Create(_T(""), flags, CRect(x, y, x + width, y + height), this, id);
		++id;
		GetColorButtons().push_back(button);
		x += width + 2;
	}

	// create palette
	const float full = 255;
	const float half = 127;
	const float quater = 63;
	int colors[] =
	{
		RGB(full, 0, 0), RGB(half, 0, 0),
		RGB(full, half, 0), RGB(half, quater, 0),
		RGB(full, full, 0), RGB(half, half, 0),
		RGB(0, full, 0), RGB(0, half, 0),
		RGB(0, 0, full), RGB(0, 0, half),
		RGB(0, full, full), RGB(0, half, half),
		RGB(full, 0, full), RGB(half, 0, half),
		RGB(full, full, full), RGB(half, half, half), RGB(0,0,0)
	};
	width = 15;
	height = 30;
	for (unsigned int i = 0; i < size(colors); ++i)
	{
		CColorButton* button = new CColorButton(colors[i], colors[i]);
		const int flags = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |
			BS_OWNERDRAW | BS_MULTILINE;
		button->Create(_T(""), flags, CRect(x, y, x + width, y + height), this, id);
		++id;
		GetColorButtons().push_back(button);
		x += width + 2;
	}

	if (_mPlayOnOpen)
	{
		OnBnClickedButtonPlay();
	}

	int index = (int)_mDevice;
	GetControlListTypes()->SetCurSel(index);

	return TRUE;
}

void CMainViewDlg::OnSelChangeListTypes()
{
	// stop animation
	OnBnClickedButtonStop();

	EChromaSDKDeviceEnum device;
	EChromaSDKDeviceTypeEnum deviceType;
	EChromaSDKDevice1DEnum device1D;
	EChromaSDKDevice2DEnum device2D;

	int index = GetControlListTypes()->GetCurSel();
	switch (index)
	{
	case 0:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_ChromaLink;
		device = EChromaSDKDeviceEnum::EDIT_ChromaLink;
		break;
	case 1:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_Headset;
		device = EChromaSDKDeviceEnum::EDIT_Headset;
		break;
	case 2:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Keyboard;
		device = EChromaSDKDeviceEnum::EDIT_Keyboard;
		break;
	case 3:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Keypad;
		device = EChromaSDKDeviceEnum::EDIT_Keypad;
		break;
	case 4:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Mouse;
		device = EChromaSDKDeviceEnum::EDIT_Mouse;
		break;
	case 5:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_Mousepad;
		device = EChromaSDKDeviceEnum::EDIT_Mousepad;
		break;
	default:
		return;
	}

	bool changed = false;
	if (_mDevice != device)
	{
		_mDevice = device;
		changed = true;
	}

	if (changed)
	{
		// Create the grid buttons
		RecreateGrid();

		// Display enums
		RefreshDevice();

		// Display grid
		RefreshGrid();
	}

	GetControlListTypes()->SetCurSel(index);

	//show changes
	OnBnClickedButtonPreview();
}

BOOL CMainViewDlg::PreTranslateMessage(MSG* pMsg)
{
	// check focus first
	bool textFieldHasFocus = false;
	CWnd* control = GetFocus();
	if (control)
	{
		switch (control->GetDlgCtrlID())
		{
		case IDC_TEXT_OVERRIDE_TIME:
		case IDC_EDIT_DURATION:
			textFieldHasFocus = true;
			break;
		}
	}

	if (!textFieldHasFocus)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_CONTROL:
				_mControlModifier = true;
				break;
			}
		}
		else if (pMsg->message == WM_KEYUP)
		{
			switch (pMsg->wParam)
			{
			case VK_OEM_MINUS:
				OnBnClickedButtonDelete();
				return true;
			case VK_OEM_PLUS:
				OnBnClickedButtonAdd();
				return true;
			case VK_LEFT:
				OnBnClickedButtonPrevious();
				return true;
			case VK_RIGHT:
				OnBnClickedButtonNext();
				return true;
			case VK_CONTROL:
				_mControlModifier = false;
				break;
			case 'C':
				if (_mControlModifier)
				{
					OnBnClickedButtonCopy();
					return true;
				}
				break;
			case 'V':
				if (_mControlModifier)
				{
					OnBnClickedButtonPaste();
					return true;
				}
				break;
			default:
				fprintf(stdout, "Pressed: %d\r\n", pMsg->wParam);
				break;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMainViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainViewDlg, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_IMPORT_OVERRIDE_TIME, &CMainViewDlg::OnBnClickedButtonImportOverrideTime)
	ON_BN_CLICKED(IDC_CHECK_USE_KEYBOARD_CUSTOM_KEY_TYPE, &CMainViewDlg::OnBnClickedCheckUseKeyboardKeyType)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CMainViewDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_FILL, &CMainViewDlg::OnBnClickedButtonFill)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CMainViewDlg::OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CMainViewDlg::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_PASTE, &CMainViewDlg::OnBnClickedButtonPaste)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CMainViewDlg::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CMainViewDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_LOOP, &CMainViewDlg::OnBnClickedButtonLoop)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMainViewDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMainViewDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_UNLOAD, &CMainViewDlg::OnBnClickedButtonUnload)
	ON_BN_CLICKED(IDC_BUTTON_SET_KEY, &CMainViewDlg::OnBnClickedButtonSetKey)
	ON_BN_CLICKED(IDC_BUTTON_SET_LED, &CMainViewDlg::OnBnClickedButtonSetLed)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, &CMainViewDlg::OnBnClickedButtonPrevious)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CMainViewDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CMainViewDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CMainViewDlg::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMainViewDlg::OnBnClickedButtonReset)
	ON_COMMAND_RANGE(ID_DYNAMIC_BUTTON_MIN, ID_DYNAMIC_BUTTON_MAX, &CMainViewDlg::OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_SET_DURATION, &CMainViewDlg::OnBnClickedButtonSetDuration)
	ON_LBN_SELCHANGE(IDC_LIST_TYPES, &CMainViewDlg::OnSelChangeListTypes)
	ON_BN_CLICKED(IDC_BUTTON_ENABLE, &CMainViewDlg::OnBnClickedButtonEnable)
	ON_BN_CLICKED(ID_MENU_NEW, &CMainViewDlg::OnBnClickedMenuNew)
	ON_BN_CLICKED(ID_MENU_OPEN, &CMainViewDlg::OnBnClickedMenuOpen)
	ON_BN_CLICKED(ID_MENU_SAVE, &CMainViewDlg::OnBnClickedMenuSave)
	ON_BN_CLICKED(ID_MENU_SAVE_AS, &CMainViewDlg::OnBnClickedMenuSaveAs)
	ON_BN_CLICKED(ID_MENU_EXIT, &CMainViewDlg::OnBnClickedMenuExit)
	ON_BN_CLICKED(ID_MENU_IMPORT_IMAGE, &CMainViewDlg::OnBnClickedMenuImportImage)
	ON_BN_CLICKED(ID_MENU_IMPORT_ANIMATION, &CMainViewDlg::OnBnClickedMenuImportAnimation)
END_MESSAGE_MAP()

vector<CColorButton*>& CMainViewDlg::GetGridButtons()
{
	return _mGridButtons;
}

vector<CColorButton*>& CMainViewDlg::GetColorButtons()
{
	return _mColorButtons;
}

COLORREF CMainViewDlg::GetColor()
{
	return _mColor;
}

void CMainViewDlg::SetColor(COLORREF color)
{
	_mColor = color;
}

void CMainViewDlg::OnBnClickedButtonColor(UINT nID)
{
	// stop animation
	OnBnClickedButtonStop();

	if (nID >= ID_DYNAMIC_BUTTON_MIN)
	{
		if (nID < ID_DYNAMIC_COLOR_MIN)
		{
			unsigned int index = nID - ID_DYNAMIC_BUTTON_MIN;
			vector<CColorButton*>& buttons = GetGridButtons();
			if (index < buttons.size())
			{
				CColorButton* button = buttons[index];
				COLORREF color = GetColor();
				button->SetColor(color, color);
				button->Invalidate();

				int currentFrame = GetCurrentFrame();
				int maxLeds;
				int maxRow;
				int maxColumn;

				switch (GetDeviceType())
				{
				case EChromaSDKDeviceTypeEnum::DE_1D:
					if (GetDimensions1D(maxLeds))
					{
						vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
						if (currentFrame < 0 ||
							currentFrame >= frames.size())
						{
							currentFrame = 0;
						}
						if (currentFrame < frames.size())
						{
							FChromaSDKColorFrame1D& frame = frames[currentFrame];
							int i = index;
							frame.Colors[i] = color;
							RefreshGrid();
						}
					}
					break;
				case EChromaSDKDeviceTypeEnum::DE_2D:
					if (GetDimensions2D(maxRow, maxColumn))
					{
						vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
						if (currentFrame < 0 ||
							currentFrame >= frames.size())
						{
							currentFrame = 0;
						}
						if (currentFrame < frames.size())
						{
							FChromaSDKColorFrame2D& frame = frames[currentFrame];
							int i = index / maxColumn;
							FChromaSDKColors& row = frame.Colors[i];
							int j = index - i * maxColumn;
							row.Colors[j] = color;
							RefreshGrid();
						}
					}
					break;
				}
			}
		}
		else
		{
			int index = nID - ID_DYNAMIC_COLOR_MIN;
			if (index == 0)
			{
				// Get the selected color from the CColorDialog. 
				CColorDialog dlg(GetColor());
				if (dlg.DoModal() == IDOK)
				{
					COLORREF color = dlg.GetColor();
					SetColor(color);
					GetColorButtons()[0]->SetColor(color, color);
					GetColorButtons()[0]->Invalidate();
				}
			}
			else
			{
				vector<CColorButton*> buttons = GetColorButtons();
				CColorButton* button = buttons[index];
				COLORREF color = button->GetBackgroundColor();
				SetColor(color);
				GetColorButtons()[0]->SetColor(color, color);
				GetColorButtons()[0]->Invalidate();
			}
		}
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}

void CMainViewDlg::OnBnClickedMenuImportImage()
{
	// stop animation
	OnBnClickedButtonStop();

	GetEditor()->ImportTextureImage();
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedMenuImportAnimation()
{
	// stop animation
	OnBnClickedButtonStop();

	GetEditor()->ImportTextureAnimation();
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonImportOverrideTime()
{
	// stop animation
	OnBnClickedButtonStop();

	float time = GetOverrideTime();
	_mOverrideTime = time;
	OverrideTime(time);
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}

void CMainViewDlg::OnBnClickedCheckUseKeyboardKeyType()
{
	// stop animation
	OnBnClickedButtonStop();

	// unload animation
	OnBnClickedButtonUnload();

	bool changed = false;
	AnimationBase* animation = CMainViewDlg::GetAnimation();
	if (GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D)
	{
		Animation2D* animation2D = (Animation2D*)animation;
		CButton* checkControl = GetControlUseKeyboardCustomKeys();
		bool isChecked = checkControl->GetCheck() == BST_CHECKED;
		animation2D->SetUseKeyboardCustomKeyType(isChecked);
		changed = true;
	}

	if (changed)
	{
		// Save the file
		SaveFile();

		// Create the grid buttons
		RecreateGrid();

		// Display enums
		RefreshDevice();

		// Display grid
		RefreshGrid();
	}

	//show changes
	OnBnClickedButtonPreview();
}

bool CMainViewDlg::GetDimensions1D(int& maxLeds)
{
	if (GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_1D)
	{
		EChromaSDKDevice1DEnum device = GetEditor1D()->GetDevice();
		maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(device);
		return true;
	}
	else
	{
		return false;
	}
}

bool CMainViewDlg::GetDimensions2D(int& maxRow, int& maxColumn)
{
	if (GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D)
	{
		EChromaSDKDevice2DEnum device = GetEditor2D()->GetDevice();
		maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(device);
		maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(device);
		return true;
	}
	else
	{
		return false;
	}
}

int CMainViewDlg::GetCurrentFrame()
{
	return GetEditor()->GetCurrentFrame();
}

void CMainViewDlg::SetCurrentFrame(unsigned int index)
{
	GetEditor()->SetCurrentFrame(index);
}

int CMainViewDlg::GetFrameCount()
{
	return GetEditor()->GetFrameCount();
}

void CMainViewDlg::OverrideTime(float time)
{
	GetEditor()->OverrideTime(time);
}

void CMainViewDlg::AddFrame()
{
	GetEditor()->AddFrame();
}

void CMainViewDlg::SetPath(const std::string& path)
{
	GetEditor()->SetPath(path);
}

void CMainViewDlg::SetAnimation(AnimationBase* animation)
{
	GetEditor()->SetAnimation(animation);
}

void CMainViewDlg::CloseAnimations()
{
	_mEditChromaLink.CloseAnimation();
	_mEditHeadset.CloseAnimation();
	_mEditKeyboard.CloseAnimation();
	_mEditKeypad.CloseAnimation();
	_mEditMouse.CloseAnimation();
	_mEditMousepad.CloseAnimation();
}

void CMainViewDlg::Reset()
{
	GetEditor()->Reset();
}

vector<COLORREF> CMainViewDlg::CreateColors1D()
{
	EChromaSDKDevice1DEnum device = GetEditor1D()->GetDevice();
	return ChromaSDKPlugin::GetInstance()->CreateColors1D(device);
}

vector<FChromaSDKColors> CMainViewDlg::CreateColors2D()
{
	EChromaSDKDevice2DEnum device = GetEditor2D()->GetDevice();
	return ChromaSDKPlugin::GetInstance()->CreateColors2D(device);
}

bool CMainViewDlg::GetCurrentFrame1D(FChromaSDKColorFrame1D& frame)
{
	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();
	if (currentFrame < 0 ||
		currentFrame >= frameCount)
	{
		currentFrame = 0;
	}
	if (currentFrame < frameCount &&
		GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_1D)
	{
		vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
		frame = frames[currentFrame];
		return true;
	}
	return false;
}

bool CMainViewDlg::SetCurrentFrame1D(FChromaSDKColorFrame1D& frame)
{
	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();
	if (currentFrame < 0 ||
		currentFrame >= frameCount)
	{
		currentFrame = 0;
	}
	if (currentFrame < frameCount &&
		GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_1D)
	{
		vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
		frames[currentFrame] = frame;
		return true;
	}
	return false;
}

bool CMainViewDlg::GetCurrentFrame2D(FChromaSDKColorFrame2D& frame)
{
	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();
	if (currentFrame < 0 ||
		currentFrame >= frameCount)
	{
		currentFrame = 0;
	}
	if (currentFrame < frameCount &&
		GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D)
	{
		vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
		frame = frames[currentFrame];
		return true;
	}
	return false;
}

bool CMainViewDlg::SetCurrentFrame2D(FChromaSDKColorFrame2D& frame)
{
	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();
	if (currentFrame < 0 ||
		currentFrame >= frameCount)
	{
		currentFrame = 0;
	}
	if (currentFrame < frameCount &&
		GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D)
	{
		vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
		frames[currentFrame] = frame;
		return true;
	}
	return false;
}

void CMainViewDlg::OnBnClickedButtonClear()
{
	// stop animation
	OnBnClickedButtonStop();

	FChromaSDKColorFrame1D frame1D;
	FChromaSDKColorFrame2D frame2D;

	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (GetCurrentFrame1D(frame1D))
		{
			frame1D.Colors = CreateColors1D();
			SetCurrentFrame1D(frame1D);
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (GetCurrentFrame2D(frame2D))
		{
			frame2D.Colors = CreateColors2D();
			SetCurrentFrame2D(frame2D);
		}
		break;
	}
	RefreshGrid();
	SaveFile();	

	//show changes
	OnBnClickedButtonPreview();
}

void CMainViewDlg::OnBnClickedButtonFill()
{
	//stop animation
	OnBnClickedButtonStop();

	int maxLeds;
	FChromaSDKColorFrame1D frame1D;

	int maxRow;
	int maxColumn;
	FChromaSDKColorFrame2D frame2D;

	switch (GetDeviceType())
	{
		case EChromaSDKDeviceTypeEnum::DE_1D:
			if (GetCurrentFrame1D(frame1D) &&
				GetDimensions1D(maxLeds))
			{
				for (int i = 0; i < maxLeds; ++i)
				{
					frame1D.Colors[i] = _mColor;
				}
				SetCurrentFrame1D(frame1D);
			}
			break;
		case EChromaSDKDeviceTypeEnum::DE_2D:
			if (GetCurrentFrame2D(frame2D) &&
				GetDimensions2D(maxRow, maxColumn))
			{
				for (int i = 0; i < maxRow; ++i)
				{
					FChromaSDKColors& row = frame2D.Colors[i];
					for (int j = 0; j < maxColumn; ++j)
					{
						row.Colors[j] = _mColor;
					}
				}
				SetCurrentFrame2D(frame2D);
			}
			break;
	}
	RefreshGrid();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonRandom()
{
	// stop animation
	OnBnClickedButtonStop();

	FChromaSDKColorFrame1D frame1D;
	FChromaSDKColorFrame2D frame2D;

	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (GetCurrentFrame1D(frame1D))
		{
			EChromaSDKDevice1DEnum device = GetEditor1D()->GetDevice();
			frame1D.Colors = ChromaSDKPlugin::GetInstance()->CreateRandomColors1D(device);
			SetCurrentFrame1D(frame1D);
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (GetCurrentFrame2D(frame2D))
		{
			EChromaSDKDevice2DEnum device = GetEditor2D()->GetDevice();
			frame2D.Colors = ChromaSDKPlugin::GetInstance()->CreateRandomColors2D(device);
			SetCurrentFrame2D(frame2D);
		}
		break;
	}
	RefreshGrid();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonCopy()
{
	// stop animation
	OnBnClickedButtonStop();

	FChromaSDKColorFrame1D frame1D;
	FChromaSDKColorFrame2D frame2D;

	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (GetCurrentFrame1D(frame1D))
		{
			GetEditor1D()->SetCopy(frame1D);
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (GetCurrentFrame2D(frame2D))
		{
			GetEditor2D()->SetCopy(frame2D);
		}
		break;
	}
}


void CMainViewDlg::OnBnClickedButtonPaste()
{
	// stop animation
	OnBnClickedButtonStop();

	int currentFrame = GetCurrentFrame();
	int frameCount = GetFrameCount();
	if (currentFrame < 0 ||
		currentFrame >= frameCount)
	{
		currentFrame = 0;
	}
	if (currentFrame < frameCount)
	{
		switch (GetDeviceType())
		{
		case EChromaSDKDeviceTypeEnum::DE_1D:
			{
				vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();				
				frames[currentFrame] = GetEditor1D()->GetCopy();
			}
			break;
		case EChromaSDKDeviceTypeEnum::DE_2D:
			{
				vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
				frames[currentFrame] = GetEditor2D()->GetCopy();
			}
			break;
		}
	}
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonPreview()
{
	if (GetAnimation() == nullptr)
	{
		return;
	}

	FChromaSDKColorFrame1D frame1D;
	FChromaSDKColorFrame2D frame2D;

	switch (GetDeviceType())
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (GetCurrentFrame1D(frame1D))
		{
			EChromaSDKDevice1DEnum device = GetEditor1D()->GetDevice();
			FChromaSDKEffectResult result = ChromaSDKPlugin::GetInstance()->CreateEffectCustom1D(device, frame1D.Colors);
			if (result.Result == 0)
			{
				ChromaSDKPlugin::GetInstance()->SetEffect(result.EffectId);
				ChromaSDKPlugin::GetInstance()->DeleteEffect(result.EffectId);
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (GetCurrentFrame2D(frame2D))
		{
			EChromaSDKDevice2DEnum device = GetEditor2D()->GetDevice();
			FChromaSDKEffectResult result;
			if (device == EChromaSDKDevice2DEnum::DE_Keyboard &&
				GetAnimation2D()->HasKeyboardCustomKeyType())
			{
				result = ChromaSDKPlugin::GetInstance()->CreateEffectKeyboardCustomKey(frame2D.Colors);
			}
			else
			{
				result = ChromaSDKPlugin::GetInstance()->CreateEffectCustom2D(device, frame2D.Colors);
			}
			if (result.Result == 0)
			{
				ChromaSDKPlugin::GetInstance()->SetEffect(result.EffectId);
				ChromaSDKPlugin::GetInstance()->DeleteEffect(result.EffectId);
			}
		}
		break;
	}
}

EChromaSDKDeviceTypeEnum CMainViewDlg::GetDeviceType()
{
	switch (_mDevice)
	{
	case EChromaSDKDeviceEnum::EDIT_ChromaLink:
		return EChromaSDKDeviceTypeEnum::DE_1D;
	case EChromaSDKDeviceEnum::EDIT_Headset:
		return EChromaSDKDeviceTypeEnum::DE_1D;
	case EChromaSDKDeviceEnum::EDIT_Keyboard:
		return EChromaSDKDeviceTypeEnum::DE_2D;
	case EChromaSDKDeviceEnum::EDIT_Keypad:
		return EChromaSDKDeviceTypeEnum::DE_2D;
	case EChromaSDKDeviceEnum::EDIT_Mouse:
		return EChromaSDKDeviceTypeEnum::DE_2D;
	case EChromaSDKDeviceEnum::EDIT_Mousepad:
		return EChromaSDKDeviceTypeEnum::DE_1D;
	default:
		return EChromaSDKDeviceTypeEnum::DE_1D;
	}
}

EditorAnimationBase* CMainViewDlg::GetEditor()
{
	switch (_mDevice)
	{
	case EChromaSDKDeviceEnum::EDIT_ChromaLink:
		return &_mEditChromaLink;
	case EChromaSDKDeviceEnum::EDIT_Headset:
		return &_mEditHeadset;
	case EChromaSDKDeviceEnum::EDIT_Keyboard:
		return &_mEditKeyboard;
	case EChromaSDKDeviceEnum::EDIT_Keypad:
		return &_mEditKeypad;
	case EChromaSDKDeviceEnum::EDIT_Mouse:
		return &_mEditMouse;
	case EChromaSDKDeviceEnum::EDIT_Mousepad:
		return &_mEditMousepad;
	default:
		return nullptr;
	}
}

EditorAnimation1D* CMainViewDlg::GetEditor1D()
{
	return (EditorAnimation1D*)GetEditor();
}

EditorAnimation2D* CMainViewDlg::GetEditor2D()
{
	return (EditorAnimation2D*)GetEditor();
}

AnimationBase* CMainViewDlg::GetAnimation()
{
	return GetEditor()->GetAnimation();
}

Animation1D* CMainViewDlg::GetAnimation1D()
{
	return (Animation1D*)GetAnimation();
}

Animation2D* CMainViewDlg::GetAnimation2D()
{
	return (Animation2D*)GetAnimation();
}

void CMainViewDlg::OnBnClickedButtonPlay()
{
	if (GetAnimation() != nullptr)
	{
		int animationId = PluginGetAnimationIdFromInstance(GetAnimation());
		if (animationId < 0)
		{
			return;
		}
		PluginPlayAnimationLoop(animationId, false);
	}
}

void CMainViewDlg::OnBnClickedButtonEnable()
{
	int index = GetControlListTypes()->GetCurSel();

	AnimationBase* animation = GetAnimation();
	if (animation == nullptr)
	{
		int animationId;

		switch (_mDevice)
		{
		case EChromaSDKDeviceEnum::EDIT_ChromaLink:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_ChromaLink);
			animation = GetAnimationInstance(animationId);
			_mEditChromaLink.SetAnimation(animation);
			_mEditChromaLink.Reset();
			break;
		case EChromaSDKDeviceEnum::EDIT_Headset:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_Headset);
			animation = GetAnimationInstance(animationId);
			_mEditHeadset.SetAnimation(animation);
			_mEditHeadset.Reset();
			break;
		case EChromaSDKDeviceEnum::EDIT_Keyboard:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Keyboard);
			animation = GetAnimationInstance(animationId);
			_mEditKeyboard.SetAnimation(animation);
			_mEditKeyboard.Reset();
			break;
		case EChromaSDKDeviceEnum::EDIT_Keypad:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Keypad);
			animation = GetAnimationInstance(animationId);
			_mEditKeypad.SetAnimation(animation);
			_mEditKeypad.Reset();
			break;
		case EChromaSDKDeviceEnum::EDIT_Mouse:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_2D, (int)EChromaSDKDevice2DEnum::DE_Mouse);
			animation = GetAnimationInstance(animationId);
			_mEditMouse.SetAnimation(animation);
			_mEditMouse.Reset();
			break;
		case EChromaSDKDeviceEnum::EDIT_Mousepad:
			animationId = PluginCreateAnimationInMemory((int)EChromaSDKDeviceTypeEnum::DE_1D, (int)EChromaSDKDevice1DEnum::DE_Mousepad);
			animation = GetAnimationInstance(animationId);
			_mEditMousepad.SetAnimation(animation);
			_mEditMousepad.Reset();
			break;
		}
		GetEditor()->SetAnimation(animation);
	}
	else
	{
		int animationId = PluginGetAnimationIdFromInstance(animation);
		if (animationId >= 0)
		{
			PluginStopAnimation(animationId);
			PluginUnloadAnimation(animationId);
			PluginCloseAnimation(animationId);
		}
		GetEditor()->SetAnimation(nullptr);
	}

	// Create the grid buttons
	RecreateGrid();

	// Display enums
	RefreshDevice();

	// Display grid
	RefreshGrid();

	GetControlListTypes()->SetCurSel(index);
	GetControlListTypes()->SetFocus();
}

void CMainViewDlg::OnBnClickedButtonLoop()
{
	if (GetAnimation() != nullptr)
	{
		int animationId = PluginGetAnimationIdFromInstance(GetAnimation());
		if (animationId < 0)
		{
			return;
		}
		PluginPlayAnimationLoop(animationId, true);
	}
}

void CMainViewDlg::OnBnClickedButtonStop()
{
	GetEditor()->Stop();
}

void CMainViewDlg::OnBnClickedButtonLoad()
{
	// stop animation
	OnBnClickedButtonStop();

	if (GetAnimation() != nullptr)
	{
		GetAnimation()->Load();
	}
}


void CMainViewDlg::OnBnClickedButtonUnload()
{
	// stop animation
	OnBnClickedButtonStop();

	if (GetAnimation() != nullptr)
	{
		GetAnimation()->Unload();
	}
}


void CMainViewDlg::OnBnClickedButtonSetKey()
{
	// stop animation
	OnBnClickedButtonStop();

	if (GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D &&
		GetEditor2D()->GetDevice() == EChromaSDKDevice2DEnum::DE_Keyboard)
	{
		FChromaSDKColorFrame2D frame;
		if (GetCurrentFrame2D(frame))
		{
			int id = GetControlSetKeyCombo()->GetCurSel();
			EChromaSDKKeyboardKey key = (EChromaSDKKeyboardKey)id;
			std::vector<FChromaSDKColors>& colors = frame.Colors;
			ChromaSDKPlugin::GetInstance()->SetKeyboardKeyColor(key, _mColor, colors);
			RefreshGrid();
		}
	}

	//show changes
	OnBnClickedButtonPreview();
}

void CMainViewDlg::OnBnClickedButtonSetLed()
{
	// stop animation
	OnBnClickedButtonStop();

	if (GetDeviceType() == EChromaSDKDeviceTypeEnum::DE_2D &&
		GetEditor2D()->GetDevice() == EChromaSDKDevice2DEnum::DE_Mouse)
	{
		FChromaSDKColorFrame2D frame;
		if (GetCurrentFrame2D(frame))
		{
			int id = GetControlSetLEDCombo()->GetCurSel();
			EChromaSDKMouseLED led = (EChromaSDKMouseLED)id;
			std::vector<FChromaSDKColors>& colors = frame.Colors;
			ChromaSDKPlugin::GetInstance()->SetMouseLEDColor(led, _mColor, colors);
			RefreshGrid();
		}
	}

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonPrevious()
{
	// stop animation
	OnBnClickedButtonStop();

	int currentFrame = GetCurrentFrame();
	if (currentFrame < 1 ||
		currentFrame >= GetFrameCount())
	{
		currentFrame = 0;
	}
	else
	{
		--currentFrame;
	}
	SetCurrentFrame(currentFrame);
	RefreshGrid();
	RefreshFrames();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonNext()
{
	// stop animation
	OnBnClickedButtonStop();

	int currentFrame = GetCurrentFrame();
	if (currentFrame < 0 ||
		currentFrame >= GetFrameCount())
	{
		currentFrame = 0;
	}
	if ((currentFrame + 1) < GetFrameCount())
	{
		++currentFrame;
	}
	SetCurrentFrame(currentFrame);
	RefreshGrid();
	RefreshFrames();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonAdd()
{
	// stop animation
	OnBnClickedButtonStop();

	AddFrame();
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonDelete()
{
	// stop animation
	OnBnClickedButtonStop();

	int currentFrame = GetCurrentFrame();
	if (currentFrame < 0 ||
		currentFrame >= GetFrameCount())
	{
		currentFrame = 0;
	}

	switch (GetDeviceType())
	{
		case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
			if (frames.size() == 1)
			{
				FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
				frame.Colors = CreateColors1D();
				frames[0] = frame;
			}
			else if (frames.size() > 0)
			{
				auto it = frames.begin();
				frames.erase(it+currentFrame);
				if (currentFrame == frames.size())
				{
					currentFrame = frames.size() - 1;
					SetCurrentFrame(currentFrame);
				}
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
			if (frames.size() == 1)
			{
				FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
				frame.Colors = CreateColors2D();
				frames[0] = frame;
			}
			else if (frames.size() > 0)
			{
				auto it = frames.begin();
				frames.erase(it+currentFrame);
				if (currentFrame == frames.size())
				{
					currentFrame = frames.size() - 1;
					SetCurrentFrame(currentFrame);
				}
			}
		}
		break;
	}
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonReset()
{
	// stop animation
	OnBnClickedButtonStop();

	AnimationBase* animation = GetAnimation();
	if (animation != nullptr)
	{
		animation->ResetFrames();
		RefreshGrid();
		RefreshFrames();
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}

void CMainViewDlg::OnBnClickedButtonSetDuration()
{
	// stop animation
	OnBnClickedButtonStop();

	unsigned int currentFrame = GetCurrentFrame();
	if (currentFrame < 0 ||
		currentFrame >= GetFrameCount())
	{
		currentFrame = 0;
	}
	if (currentFrame < GetFrameCount())
	{
		switch (GetDeviceType())
		{
			case EChromaSDKDeviceTypeEnum::DE_1D:
			{
				vector<FChromaSDKColorFrame1D>& frames = GetEditor1D()->GetFrames();
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				frame.Duration = GetDuration();
			}
			break;
			case EChromaSDKDeviceTypeEnum::DE_2D:
			{
				vector<FChromaSDKColorFrame2D>& frames = GetEditor2D()->GetFrames();
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				frame.Duration = GetDuration();

			}
			break;
		}

		RefreshFrames();
		SaveFile();

		//show changes
		OnBnClickedButtonPreview();
	}
}

void CMainViewDlg::OnBnClickedMenuNew()
{
	// stop animation
	OnBnClickedButtonStop();

	OnBnClickedButtonReset();
}

void CMainViewDlg::OnBnClickedMenuOpen()
{
	// stop animation
	OnBnClickedButtonStop();

	// get path from loaded filename
	CString szDir;
	size_t lastSlash = _mPath.find_last_of("/\\");
	if (lastSlash < 0)
	{
		return;
	}
	string path = _mPath.substr(0, lastSlash);
	//LogDebug("ImportTextureAnimation path=%s", path.c_str());
	szDir += path.c_str();

	const int MAX_CFileDialog_FILE_COUNT = 99;
	const int FILE_LIST_BUFFER_SIZE = ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1);

	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _TEXT("Animation\0*.chroma\0");
	ofn.lpstrInitialDir = szDir;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	if (dlgFile.DoModal() == IDOK)
	{
		_mPath = string(CT2CA(fileName));
		if (_mPath.size() <= 2 ||
			_mPath.substr(_mPath.find_last_of(".") + 1) != "chroma")
		{
			_mPath += ".chroma";
		}
		LoadFile();

		// Create the grid buttons
		RecreateGrid();

		// Display enums
		RefreshDevice();

		// Display grid
		RefreshGrid();
	}
	fileName.ReleaseBuffer();
}

void CMainViewDlg::OnBnClickedMenuSave()
{
	// stop animation
	OnBnClickedButtonStop();

	SaveFile();
}

void CMainViewDlg::OnBnClickedMenuSaveAs()
{
	// stop animation
	OnBnClickedButtonStop();

	// get path from loaded filename
	CString szDir;
	size_t lastSlash = _mPath.find_last_of("/\\");
	if (lastSlash < 0)
	{
		return;
	}
	string path = _mPath.substr(0, lastSlash);
	//LogDebug("ImportTextureAnimation path=%s", path.c_str());
	szDir += path.c_str();

	const int MAX_CFileDialog_FILE_COUNT = 99;
	const int FILE_LIST_BUFFER_SIZE = ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1);

	CString fileName;
	wchar_t* p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);
	CFileDialog dlgFile(TRUE);
	OPENFILENAME& ofn = dlgFile.GetOFN();
	ofn.lpstrFilter = _TEXT("Animation\0*.chroma\0");
	ofn.lpstrInitialDir = szDir;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	if (dlgFile.DoModal() == IDOK)
	{
		_mPath = string(CT2CA(fileName));
		if (_mPath.size() <= 2 ||
			_mPath.substr(_mPath.find_last_of(".") + 1) != "chroma")
		{
			_mPath += ".chroma";
		}
		SaveFile();
	}
	fileName.ReleaseBuffer();
}

void CMainViewDlg::OnBnClickedMenuExit()
{
	_mEditChromaLink.Stop();
	_mEditHeadset.Stop();
	_mEditKeyboard.Stop();
	_mEditKeypad.Stop();
	_mEditMouse.Stop();
	_mEditMousepad.Stop();

	PostQuitMessage(0);
}
