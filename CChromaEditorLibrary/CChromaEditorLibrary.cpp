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
#define DEVICE_TYPE_1D "1D"
#define DEVICE_TYPE_2D "2D"
#define DEVICE_CHROMA_LINK "ChromaLink"
#define DEVICE_HEADSET "Headset"
#define DEVICE_KEYBOARD "Keyboard"
#define DEVICE_KEYPAD "Keypad"
#define DEVICE_MOUSE "Mouse"
#define DEVICE_MOUSEPAD "Mousepad"

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
	_mEdit1D.GetAnimation()->Stop();
	_mEdit2D.GetAnimation()->Stop();
}

void CMainViewDlg::OpenOrCreateAnimation(const std::string& path)
{
	_mPath = path;
}

void CMainViewDlg::PlayAnimationOnOpen()
{
	_mPlayOnOpen = true;
}

void CMainViewDlg::LoadFile()
{
	if (_mPath.empty())
	{
		fprintf(stderr, "LoadFile: Path cannot be empty! Using `%s` instead.\r\n", TEMP_FILE);
		_mPath = TEMP_FILE;
	}

	AnimationBase* animation = ChromaSDKPlugin::GetInstance()->OpenAnimation(_mPath);
	if (animation)
	{
		Animation1D* animation1D;
		Animation2D* animation2D;
		switch (animation->GetDeviceType())
		{
		case EChromaSDKDeviceTypeEnum::DE_1D:
			_mEdit1D.SetPath(_mPath);
			_mDeviceType = animation->GetDeviceType();
			animation1D = dynamic_cast<Animation1D*>(animation);
			_mEdit1D.SetAnimation(*animation1D);
			_mEdit1D.Reset();
			delete animation;
			break;
		case EChromaSDKDeviceTypeEnum::DE_2D:
			_mEdit2D.SetPath(_mPath);
			_mDeviceType = animation->GetDeviceType();
			animation2D = dynamic_cast<Animation2D*>(animation);
			_mEdit2D.SetAnimation(*animation2D);
			delete animation;
			break;
		default:
			fprintf(stderr, "LoadFile: Unexpected animation type!");
			return;
		}
	}
}

void CMainViewDlg::SaveFile()
{
	GetAnimation()->Save(_mPath.c_str());
}

CEdit* CMainViewDlg::GetControlOverrideTime()
{
	return (CEdit*)GetDlgItem(IDC_TEXT_OVERRIDE_TIME);
}

CComboBox* CMainViewDlg::GetControlDeviceType()
{
	return (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
}

CComboBox* CMainViewDlg::GetControlDevice()
{
	return (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);
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
	GetControlDeviceType()->SetCurSel(_mDeviceType);

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		GetControlDevice()->ResetContent();
		GetControlDevice()->AddString(_T(DEVICE_CHROMA_LINK));
		GetControlDevice()->AddString(_T(DEVICE_HEADSET));
		GetControlDevice()->AddString(_T(DEVICE_MOUSEPAD));
		GetControlDevice()->SetCurSel(_mEdit1D.GetDevice());
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		GetControlDevice()->ResetContent();
		GetControlDevice()->AddString(_T(DEVICE_KEYBOARD));
		GetControlDevice()->AddString(_T(DEVICE_KEYPAD));
		GetControlDevice()->AddString(_T(DEVICE_MOUSE));
		GetControlDevice()->SetCurSel(_mEdit2D.GetDevice());
		break;
	}

	int show = _mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Keyboard;
	GetControlSetKeyLabel()->ShowWindow(show);
	GetControlSetKeyCombo()->ShowWindow(show);
	GetControlSetKeyButton()->ShowWindow(show);

	show = _mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Mouse;
	GetControlSetLEDLabel()->ShowWindow(show);
	GetControlSetLEDCombo()->ShowWindow(show);
	GetControlSetLEDButton()->ShowWindow(show);

	show = _mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Keyboard;
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

	GetListTypes()->ResetContent();
	
	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_1D && _mEdit1D.GetDevice() == EChromaSDKDevice1DEnum::DE_ChromaLink)
	{
		GetListTypes()->AddString(_T("[x] ChromaLink"));
	}
	else
	{
		GetListTypes()->AddString(_T("ChromaLink"));
	}

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_1D && _mEdit1D.GetDevice() == EChromaSDKDevice1DEnum::DE_Headset)
	{
		GetListTypes()->AddString(_T("[x] Headset"));
	}
	else
	{
		GetListTypes()->AddString(_T("Headset"));
	}

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Keyboard)
	{
		GetListTypes()->AddString(_T("[x] Keyboard"));
	}
	else
	{
		GetListTypes()->AddString(_T("Keyboard"));
	}

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Keypad)
	{
		GetListTypes()->AddString(_T("[x] Keypad"));
	}
	else
	{
		GetListTypes()->AddString(_T("Keypad"));
	}

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D && _mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Mouse)
	{
		GetListTypes()->AddString(_T("[x] Mouse"));
	}
	else
	{
		GetListTypes()->AddString(_T("Mouse"));
	}

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_1D && _mEdit1D.GetDevice() == EChromaSDKDevice1DEnum::DE_Mousepad)
	{
		GetListTypes()->AddString(_T("[x] Mousepad"));
	}
	else
	{
		GetListTypes()->AddString(_T("Mousepad"));
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

	COLORREF black = RGB(0, 0, 0);

	// update grid label
	int width = 15;
	int height = 30;
	int y = 280;
	int id = ID_DYNAMIC_BUTTON_MIN;
	switch (_mDeviceType)
	{
		case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			int maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(_mEdit1D.GetDevice());
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
			int maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(_mEdit2D.GetDevice());
			int maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(_mEdit2D.GetDevice());

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
	// update grid label
	char buffer[20] = { 0 };
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			int maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(_mEdit1D.GetDevice());
			sprintf_s(buffer, "1 x %d", maxLeds);
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			int maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(_mEdit2D.GetDevice());
			int maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(_mEdit2D.GetDevice());
			sprintf_s(buffer, "%d x %d", maxRow, maxColumn);
		}
		break;
	}
	GetControlGridSize()->SetWindowText(CString(buffer));


	// update buttons
	vector<CColorButton*>& buttons = GetGridButtons();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			int maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(_mEdit1D.GetDevice());
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
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
			int maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(_mEdit2D.GetDevice());
			int maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(_mEdit2D.GetDevice());
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
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
	int currentFrame = 0;
	int frameCount = 0;

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		currentFrame = _mEdit1D.GetCurrentFrame() + 1;
		frameCount = _mEdit1D.GetFrames().size();
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		currentFrame = _mEdit2D.GetCurrentFrame() + 1;
		frameCount = _mEdit2D.GetFrames().size();
		break;
	}

	sprintf_s(bufferFrameInfo, "%d of %d", currentFrame, frameCount);
	GetControlFrames()->SetWindowText(CString(bufferFrameInfo));

	//update the frame duration
	char bufferDuration[16] = { 0 };
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
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
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
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

CListBox* CMainViewDlg::GetListTypes()
{
	return (CListBox*)GetDlgItem(IDC_LIST_TYPES);
}

BOOL CMainViewDlg::OnInitDialog()
{
	// Setup default
	_mDeviceType = EChromaSDKDeviceTypeEnum::DE_2D;

	LoadFile();

	// setup dialog
	UpdateOverrideTime(DEFAULT_OVERRIDE_TIME);
	GetControlDeviceType()->AddString(_T(DEVICE_TYPE_1D));
	GetControlDeviceType()->AddString(_T(DEVICE_TYPE_2D));

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

	// Set default type
	GetControlDeviceType()->SetCurSel(_mDeviceType);

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

	return TRUE;
}

void CMainViewDlg::ListTypesOnSelChange()
{
	// stop animation
	OnBnClickedButtonStop();

	EChromaSDKDeviceTypeEnum deviceType;
	EChromaSDKDevice1DEnum device1D;
	EChromaSDKDevice2DEnum device2D;

	int index = GetListTypes()->GetCurSel();
	switch (index)
	{
	case 0:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_ChromaLink;
		break;
	case 1:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_Headset;
		break;
	case 2:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Keyboard;
		break;
	case 3:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Keypad;
		break;
	case 4:
		deviceType = EChromaSDKDeviceTypeEnum::DE_2D;
		device2D = EChromaSDKDevice2DEnum::DE_Mouse;
		break;
	case 5:
		deviceType = EChromaSDKDeviceTypeEnum::DE_1D;
		device1D = EChromaSDKDevice1DEnum::DE_Mousepad;
		break;
	default:
		return;
	}

	bool changed = false;
	if (_mDeviceType != deviceType)
	{
		_mDeviceType = deviceType;
		changed = true;
	}

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		if (_mEdit1D.GetDevice() != device1D)
		{
			_mEdit1D.SetDevice(device1D);
			changed = true;
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		if (_mEdit2D.GetDevice() != device2D)
		{
			_mEdit2D.SetDevice(device2D);
			changed = true;
		}
		break;
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

	ON_BN_CLICKED(IDC_BUTTON_IMPORT_IMAGE, &CMainViewDlg::OnBnClickedButtonImportImage)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_ANIMATION, &CMainViewDlg::OnBnClickedButtonImportAnimation)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_OVERRIDE_TIME, &CMainViewDlg::OnBnClickedButtonImportOverrideTime)
	ON_BN_CLICKED(IDC_BUTTON_SET_DEVICE, &CMainViewDlg::OnBnClickedButtonSetDevice)
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
	ON_BN_CLICKED(IDC_BUTTON_SET_DEVICE_TYPE, &CMainViewDlg::OnBnClickedButtonSetDeviceType)
	ON_BN_CLICKED(IDC_BUTTON_SET_DURATION, &CMainViewDlg::OnBnClickedButtonSetDuration)
	ON_LBN_SELCHANGE(IDC_LIST_TYPES, &CMainViewDlg::ListTypesOnSelChange)
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

				switch (_mDeviceType)
				{
				case EChromaSDKDeviceTypeEnum::DE_1D:
					{
						EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
						int maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(device);
						vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
						unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
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
					{
						EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
						int maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(device);
						int maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(device);
						vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
						unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
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

void CMainViewDlg::OnBnClickedButtonSetDeviceType()
{
	// stop animation
	OnBnClickedButtonStop();

	bool changed = false;
	if (_mDeviceType != (EChromaSDKDeviceTypeEnum)GetControlDeviceType()->GetCurSel())
	{
		_mDeviceType = (EChromaSDKDeviceTypeEnum)GetControlDeviceType()->GetCurSel();
		changed = true;
	}
	if (changed)
	{
		switch (_mDeviceType)
		{
		case EChromaSDKDeviceTypeEnum::DE_1D:
			if (_mEdit1D.SetDevice(EChromaSDKDevice1DEnum::DE_ChromaLink))
			{
				changed = true;
			}
			break;
		case EChromaSDKDeviceTypeEnum::DE_2D:
			if (_mEdit2D.SetDevice(EChromaSDKDevice2DEnum::DE_Keyboard))
			{
				changed = true;
			}
			break;
		}
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


void CMainViewDlg::OnBnClickedButtonImportImage()
{
	// stop animation
	OnBnClickedButtonStop();

	EditorAnimationBase* editor = GetEditor();
	if (editor == nullptr)
	{
		return;
	}
	editor->ImportTextureImage();
	RefreshGrid();
	RefreshFrames();
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonImportAnimation()
{
	// stop animation
	OnBnClickedButtonStop();

	EditorAnimationBase* editor = GetEditor();
	if (editor == nullptr)
	{
		return;
	}
	editor->ImportTextureAnimation();
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
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		_mEdit1D.OverrideTime(time);
		RefreshFrames();
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		_mEdit2D.OverrideTime(time);
		RefreshFrames();
		break;
	}
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
	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D)
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

void CMainViewDlg::OnBnClickedButtonSetDevice()
{
	// stop animation
	OnBnClickedButtonStop();

	bool changed = false;
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		changed = _mEdit1D.SetDevice((EChromaSDKDevice1DEnum)GetControlDevice()->GetCurSel());
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		changed = _mEdit2D.SetDevice((EChromaSDKDevice2DEnum)GetControlDevice()->GetCurSel());
		break;
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


void CMainViewDlg::OnBnClickedButtonClear()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateColors1D(device);
				RefreshGrid();
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateColors2D(device);
				RefreshGrid();
			}
		}
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonFill()
{
	//stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			int maxLeds = ChromaSDKPlugin::GetInstance()->GetMaxLeds(device);
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				for (int i = 0; i < maxLeds; ++i)
				{
					frame.Colors[i] = _mColor;
				}
				RefreshGrid();
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			int maxRow = ChromaSDKPlugin::GetInstance()->GetMaxRow(device);
			int maxColumn = ChromaSDKPlugin::GetInstance()->GetMaxColumn(device);
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				for (int i = 0; i < maxRow; ++i)
				{
					FChromaSDKColors& row = frame.Colors[i];
					for (int j = 0; j < maxColumn; ++j)
					{
						row.Colors[j] = _mColor;
					}
				}
				RefreshGrid();
			}
		}
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonRandom()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateRandomColors1D(device);
				RefreshGrid();
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateRandomColors2D(device);
				RefreshGrid();
			}
		}
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonCopy()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				_mEdit1D.SetCopy(frame);
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				_mEdit2D.SetCopy(frame);
			}
		}
		break;
	}
}


void CMainViewDlg::OnBnClickedButtonPaste()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				frames[currentFrame] = _mEdit1D.GetCopy();
				RefreshGrid();
				RefreshFrames();
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				frames[currentFrame] = _mEdit2D.GetCopy();
				RefreshGrid();
				RefreshFrames();
			}
		}
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonPreview()
{
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			EChromaSDKDevice1DEnum device = _mEdit1D.GetDevice();
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				FChromaSDKEffectResult result = ChromaSDKPlugin::GetInstance()->CreateEffectCustom1D(device, frame.Colors);
				if (result.Result == 0)
				{
					ChromaSDKPlugin::GetInstance()->SetEffect(result.EffectId);
					ChromaSDKPlugin::GetInstance()->DeleteEffect(result.EffectId);
				}
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			EChromaSDKDevice2DEnum device = _mEdit2D.GetDevice();
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= frames.size())
			{
				currentFrame = 0;
			}
			if (currentFrame < frames.size())
			{
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				FChromaSDKEffectResult result;
				AnimationBase* animation = GetAnimation();
				Animation2D* animation2D = dynamic_cast<Animation2D*>(animation);
				if (device == EChromaSDKDevice2DEnum::DE_Keyboard &&
					animation2D->HasKeyboardCustomKeyType())
				{
					result = ChromaSDKPlugin::GetInstance()->CreateEffectKeyboardCustomKey(frame.Colors);
				}
				else
				{
					result = ChromaSDKPlugin::GetInstance()->CreateEffectCustom2D(device, frame.Colors);
				}
				if (result.Result == 0)
				{
					ChromaSDKPlugin::GetInstance()->SetEffect(result.EffectId);
					ChromaSDKPlugin::GetInstance()->DeleteEffect(result.EffectId);
				}
			}
		}
		break;
	}
}

EditorAnimationBase* CMainViewDlg::GetEditor()
{
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		return &_mEdit1D;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		return &_mEdit2D;
	default:
		return nullptr;
	}
}

AnimationBase* CMainViewDlg::GetAnimation()
{
	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		return _mEdit1D.GetAnimation();
	case EChromaSDKDeviceTypeEnum::DE_2D:
		return _mEdit2D.GetAnimation();
	default:
		return nullptr;
	}
}

void CMainViewDlg::OnBnClickedButtonPlay()
{
	if (GetAnimation() != nullptr)
	{
		GetAnimation()->Play(false);
	}
}

void CMainViewDlg::OnBnClickedButtonLoop()
{
	if (GetAnimation() != nullptr)
	{
		GetAnimation()->Play(true);
	}
}

void CMainViewDlg::OnBnClickedButtonStop()
{
	if (GetAnimation() != nullptr)
	{
		GetAnimation()->Stop();
	}
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

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D &&
		_mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Keyboard)
	{
		vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
		unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
		if (currentFrame < 0 ||
			currentFrame >= frames.size())
		{
			currentFrame = 0;
		}
		if (currentFrame < frames.size())
		{
			int id = GetControlSetKeyCombo()->GetCurSel();
			EChromaSDKKeyboardKey key = (EChromaSDKKeyboardKey)id;
			FChromaSDKColorFrame2D& frame = frames[currentFrame];
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

	if (_mDeviceType == EChromaSDKDeviceTypeEnum::DE_2D &&
		_mEdit2D.GetDevice() == EChromaSDKDevice2DEnum::DE_Mouse)
	{
		vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
		unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
		if (currentFrame < 0 ||
			currentFrame >= frames.size())
		{
			currentFrame = 0;
		}
		if (currentFrame < frames.size())
		{
			int id = GetControlSetLEDCombo()->GetCurSel();
			EChromaSDKMouseLED led = (EChromaSDKMouseLED)id;
			FChromaSDKColorFrame2D& frame = frames[currentFrame];
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

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 1 ||
				currentFrame >= _mEdit1D.GetFrameCount())
			{
				currentFrame = 0;
			}
			else
			{
				--currentFrame;
			}
			_mEdit1D.SetCurrentFrame(currentFrame);
			RefreshGrid();
			RefreshFrames();
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 1 ||
				currentFrame >= _mEdit2D.GetFrameCount())
			{
				currentFrame = 0;
			}
			else
			{
				--currentFrame;
			}
			_mEdit2D.SetCurrentFrame(currentFrame);			
			RefreshGrid();
			RefreshFrames();
		}
		break;
	}

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonNext()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit1D.GetFrameCount())
			{
				currentFrame = 0;
			}
			if ((currentFrame + 1) < _mEdit1D.GetFrameCount())
			{
				++currentFrame;
			}
			_mEdit1D.SetCurrentFrame(currentFrame);
			RefreshGrid();
			RefreshFrames();
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit2D.GetFrameCount())
			{
				currentFrame = 0;
			}
			if ((currentFrame + 1) < _mEdit2D.GetFrameCount())
			{
				++currentFrame;
			}
			_mEdit2D.SetCurrentFrame(currentFrame);			
			RefreshGrid();
			RefreshFrames();
		}
		break;
	}

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonAdd()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		_mEdit1D.AddFrame();
		RefreshGrid();
		RefreshFrames();
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		_mEdit2D.AddFrame();
		RefreshGrid();
		RefreshFrames();
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}


void CMainViewDlg::OnBnClickedButtonDelete()
{
	// stop animation
	OnBnClickedButtonStop();

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit1D.GetFrameCount())
			{
				currentFrame = 0;
			}			
			vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
			if (frames.size() == 1)
			{
				FChromaSDKColorFrame1D frame = FChromaSDKColorFrame1D();
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateColors1D(_mEdit1D.GetDevice());
				frames[0] = frame;
			}
			else if (frames.size() > 0)
			{
				auto it = frames.begin();
				frames.erase(it+currentFrame);
				if (currentFrame == frames.size())
				{
					currentFrame = frames.size() - 1;
					_mEdit1D.SetCurrentFrame(currentFrame);
				}
			}
			RefreshGrid();
			RefreshFrames();
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit2D.GetFrameCount())
			{
				currentFrame = 0;
			}			
			vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
			if (frames.size() == 1)
			{
				FChromaSDKColorFrame2D frame = FChromaSDKColorFrame2D();
				frame.Colors = ChromaSDKPlugin::GetInstance()->CreateColors2D(_mEdit2D.GetDevice());
				frames[0] = frame;
			}
			else if (frames.size() > 0)
			{
				auto it = frames.begin();
				frames.erase(it+currentFrame);
				if (currentFrame == frames.size())
				{
					currentFrame = frames.size() - 1;
					_mEdit2D.SetCurrentFrame(currentFrame);
				}
			}
			RefreshGrid();
			RefreshFrames();
		}
		break;
	}
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

	switch (_mDeviceType)
	{
	case EChromaSDKDeviceTypeEnum::DE_1D:
		{
			unsigned int currentFrame = _mEdit1D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit1D.GetFrameCount())
			{
				currentFrame = 0;
			}			
			if (currentFrame < _mEdit1D.GetFrameCount())
			{
				vector<FChromaSDKColorFrame1D>& frames = _mEdit1D.GetFrames();
				FChromaSDKColorFrame1D& frame = frames[currentFrame];
				frame.Duration = GetDuration();
				RefreshFrames();
			}
		}
		break;
	case EChromaSDKDeviceTypeEnum::DE_2D:
		{
			unsigned int currentFrame = _mEdit2D.GetCurrentFrame();
			if (currentFrame < 0 ||
				currentFrame >= _mEdit2D.GetFrameCount())
			{
				currentFrame = 0;
			}			
			if (currentFrame < _mEdit2D.GetFrameCount())
			{
				vector<FChromaSDKColorFrame2D>& frames = _mEdit2D.GetFrames();
				FChromaSDKColorFrame2D& frame = frames[currentFrame];
				frame.Duration = GetDuration();
				RefreshFrames();
			}
		}
		break;
	}
	SaveFile();

	//show changes
	OnBnClickedButtonPreview();
}
