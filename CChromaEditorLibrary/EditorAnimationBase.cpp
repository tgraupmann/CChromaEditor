#include "stdafx.h"
#include "EditorAnimationBase.h"
#include "direct.h"

using namespace ChromaSDK;
using namespace std;

AnimationBase* EditorAnimationBase::GetAnimation()
{
	return _mAnimation;
}

void EditorAnimationBase::SetAnimation(AnimationBase* animation)
{
	_mAnimation = animation;
}

int EditorAnimationBase::GetCurrentFrame()
{
	return GetAnimation()->GetCurrentFrame();
}

void EditorAnimationBase::SetCurrentFrame(unsigned int index)
{
	GetAnimation()->SetCurrentFrame(index);
}

unsigned int EditorAnimationBase::GetFrameCount()
{
	return GetAnimation()->GetFrameCount();
}

void EditorAnimationBase::ImportTextureImage()
{
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
	ofn.lpstrFilter = _TEXT("Image Files\0*.bmp;*.jpg;*.png\0");
	ofn.lpstrInitialDir = szDir;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	if (dlgFile.DoModal() == IDOK)
	{
		string texturePath = string(CT2CA(fileName));
		//LogDebug("ImportTextureImage: %s\r\n", texturePath.c_str());
		ReadImage(texturePath, false);
	}
	fileName.ReleaseBuffer();
}

void EditorAnimationBase::ImportTextureAnimation()
{
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
	ofn.lpstrFilter = _TEXT("Image Files\0*.gif\0");
	ofn.lpstrInitialDir = szDir;
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;

	if (dlgFile.DoModal() == IDOK)
	{
		string texturePath = string(CT2CA(fileName));
		//LogDebug("ImportTextureAnimation: %s\r\n", texturePath.c_str());
		ReadImage(texturePath, true);
	}
	fileName.ReleaseBuffer();
}

void EditorAnimationBase::ReadImage(const std::string& texturePath, bool isAnimation)
{
	HRESULT hr = CoInitialize(NULL);

	// The factory pointer
	IWICImagingFactory* pFactory = nullptr;

	// Create the COM imaging factory
	hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
	);

	// Decode the source image to IWICBitmapSource
	IWICBitmapDecoder *pDecoder = nullptr;

	CString strPath = CString(texturePath.c_str());

	// Create a decoder for the given image file
	hr = pFactory->CreateDecoderFromFilename(
		strPath, // Image to be decoded
		NULL,                            // Do not prefer a particular vendor
		GENERIC_READ,                    // Desired read access to the file
		WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
		&pDecoder                        // Pointer to the decoder
	);

	// the number of frames
	UINT frameCount = 0;

	if (SUCCEEDED(hr))
	{
		//Debug("Loaded image.");
		hr = pDecoder->GetFrameCount(&frameCount);
	}
	else
	{
		//Debug("Failed to load image!");
		frameCount = 0;
	}

	for (UINT frameIndex = 0; frameIndex < frameCount; ++frameIndex)
	{
		// holds the frame data
		IWICBitmapFrameDecode* pFrame = NULL;

		// Retrieve the first frame of the image from the decoder
		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(frameIndex, &pFrame);
		}

		// Retrieve IWICBitmapSource from the frame
		IWICBitmapSource* pOriginalBitmapSource = NULL;
		if (SUCCEEDED(hr))
		{
			hr = pFrame->QueryInterface(IID_IWICBitmapSource, reinterpret_cast<void **>(&pOriginalBitmapSource));
		}

		IWICBitmapSource* pToRenderBitmapSource = NULL;

		// convert the pixel format
		if (SUCCEEDED(hr))
		{
			IWICFormatConverter* pConverter = NULL;

			hr = pFactory->CreateFormatConverter(&pConverter);

			// Format convert to 32bppBGR
			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					pFrame,                          // Input bitmap to convert
					GUID_WICPixelFormat32bppBGR,     // Destination pixel format
					WICBitmapDitherTypeNone,         // Specified dither patterm
					NULL,                            // Specify a particular palette 
					0.f,                             // Alpha threshold
					WICBitmapPaletteTypeCustom       // Palette translation type
				);

				// Store the converted bitmap if successful
				if (SUCCEEDED(hr))
				{
					hr = pConverter->QueryInterface(IID_PPV_ARGS(&pToRenderBitmapSource));
				}
			}

			if (pConverter)
			{
				pConverter->Release();
			}
		}

		// Check BitmapSource format
		WICPixelFormatGUID pixelFormat;
		if (SUCCEEDED(hr))
		{
			hr = pToRenderBitmapSource->GetPixelFormat(&pixelFormat);
		}

		if (SUCCEEDED(hr))
		{
			hr = (pixelFormat == GUID_WICPixelFormat32bppBGR) ? S_OK : E_FAIL;
		}

		// the width of the frame
		UINT nWidth = 0;

		// the height of the frame
		UINT nHeight = 0;

		// Retrieve the image dimensions
		if (SUCCEEDED(hr))
		{
			hr = pFrame->GetSize(&nWidth, &nHeight);
		}

		//_gImageData.push_back();

		// Create a DIB from the converted IWICBitmapSource
		HBITMAP hDIBBitmap = 0;
		if (SUCCEEDED(hr))
		{
			// Get image attributes and check for valid image
			UINT width = 0;
			UINT height = 0;

			void *ImageBits = NULL;

			// Check BitmapSource format
			WICPixelFormatGUID pixelFormat;
			hr = pToRenderBitmapSource->GetPixelFormat(&pixelFormat);

			if (SUCCEEDED(hr))
			{
				hr = (pixelFormat == GUID_WICPixelFormat32bppBGR) ? S_OK : E_FAIL;
			}

			if (SUCCEEDED(hr))
			{
				hr = pToRenderBitmapSource->GetSize(&width, &height);
			}

			// Holds the image pixels
			void* pImageBits = NULL;

			// Create a DIB section based on Bitmap Info
			// BITMAPINFO Struct must first be setup before a DIB can be created.
			// Note that the height is negative for top-down bitmaps
			if (SUCCEEDED(hr))
			{
				BITMAPINFO bminfo;
				ZeroMemory(&bminfo, sizeof(bminfo));
				bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				bminfo.bmiHeader.biWidth = width;
				bminfo.bmiHeader.biHeight = -(LONG)height;
				bminfo.bmiHeader.biPlanes = 1;
				bminfo.bmiHeader.biBitCount = 32;
				bminfo.bmiHeader.biCompression = BI_RGB;

				// Get a DC for the full screen
				HDC hdcScreen = GetDC(NULL);

				hr = hdcScreen ? S_OK : E_FAIL;

				// Release the previously allocated bitmap 
				if (SUCCEEDED(hr))
				{
					if (hDIBBitmap)
					{
						DeleteObject(hDIBBitmap);
					}

					hDIBBitmap = CreateDIBSection(hdcScreen, &bminfo, DIB_RGB_COLORS, &pImageBits, NULL, 0);

					ReleaseDC(NULL, hdcScreen);

					hr = hDIBBitmap ? S_OK : E_FAIL;
				}
			}

			UINT cbStride = 0;
			if (SUCCEEDED(hr))
			{
				// Size of a scan line represented in bytes: 4 bytes each pixel
				hr = UIntMult(width, sizeof(DWORD), &cbStride);
			}

			UINT cbImage = 0;
			if (SUCCEEDED(hr))
			{
				// Size of the image, represented in bytes
				hr = UIntMult(cbStride, height, &cbImage);
			}

			// Extract the image into the HBITMAP    
			if (SUCCEEDED(hr) && pToRenderBitmapSource)
			{
				hr = pToRenderBitmapSource->CopyPixels(
					NULL,
					cbStride,
					cbImage,
					reinterpret_cast<BYTE *> (pImageBits));

				if (frameIndex != 0)
				{
					AddFrame();
				}

				COLORREF* pColor = (COLORREF*)pImageBits;
				CopyPixels(pColor, width, height);
			}

			// done with bitmap
			if (hDIBBitmap)
			{
				DeleteObject(hDIBBitmap);
				hDIBBitmap = NULL;
			}
		}

		if (pOriginalBitmapSource)
		{
			pOriginalBitmapSource->Release();
		}

		if (pToRenderBitmapSource)
		{
			pToRenderBitmapSource->Release();
		}

		if (pFrame)
		{
			pFrame->Release();
		}
	}

	if (pDecoder)
	{
		pDecoder->Release();
	}

	if (pFactory)
	{
		pFactory->Release();
	}
}

void EditorAnimationBase::SetPath(const std::string& path)
{
	_mPath = path;
}
