#include "OpenGL.h"
#include "Static/String.h"
#include "Static/File.h"
#include "Static/Image.h"
#include "Static/Math.h"
#include "Static/Components/ImageData.h"
#include "Static/Components/EImageFormat.h"
#include "Format/BMP/BMPFormat.h"
#include "Stream/Stream.h"
#include "GLProgram.h"
#include "GLImage.h"

using namespace std;
using namespace glm;
using namespace mx;

// open GL initialization
bool					OpenGL::initContext(HWND hWindow, int32 iMajorVersion, int32 iMinorVersion)
{
	WNDCLASS wndClassFake;
	wndClassFake.style = CS_OWNDC;
	wndClassFake.lpfnWndProc = [](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT { return DefWindowProc(hwnd, uMsg, wParam, lParam); };
	wndClassFake.cbClsExtra = 0;
	wndClassFake.cbWndExtra = 0;
	wndClassFake.hInstance = GetModuleHandle(NULL);
	wndClassFake.hIcon = NULL;
	wndClassFake.hCursor = NULL;
	wndClassFake.hbrBackground = NULL;
	wndClassFake.lpszMenuName = L"";
	wndClassFake.lpszClassName = L"bxFakeWindow";
	if (RegisterClass(&wndClassFake) == 0)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 0.1)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	HWND hWndFake = CreateWindow(L"bxFakeWindow", L"FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN, 0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!hWndFake)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 0.2)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	HDC hDC = GetDC(hWndFake);
	if (hDC == NULL)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 0.3)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd2;
	memset(&pfd2, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd2.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd2.nVersion = 1;
	pfd2.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd2.iPixelType = PFD_TYPE_RGBA;
	pfd2.cColorBits = 32;
	pfd2.cDepthBits = 24;
	pfd2.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(hDC, &pfd2);
	if (iPixelFormat == 0)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 1)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	if (!SetPixelFormat(hDC, iPixelFormat, &pfd2))
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	// Create the false, old style context (OpenGL 2.1 and before)

	HGLRC hRCFake = wglCreateContext(hDC);
	if (hRCFake == NULL)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.1)", L"Fatal Error", MB_ICONERROR);
		return false;
	}
	if (wglMakeCurrent(hDC, hRCFake) == FALSE)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.2)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		MessageBox(NULL, L"Couldn't initialize GLEW!", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	if (wglMakeCurrent(NULL, NULL) == FALSE)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.3.1)", L"Fatal Error", MB_ICONERROR);
		return false;
	}
	if (wglDeleteContext(hRCFake) == FALSE)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.3.2)", L"Fatal Error", MB_ICONERROR);
		return false;
	}
	if (DestroyWindow(hWndFake) == 0)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.3.3)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	if (UnregisterClass(L"bxFakeWindow", GetModuleHandle(NULL)) == 0)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.3.4)", L"Fatal Error", MB_ICONERROR);
		return false;
	}













	HDC hWindowDC = GetWindowDC(hWindow);
	if (hWindowDC == NULL)
	{
		MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 2.5)", L"Fatal Error", MB_ICONERROR);
		return false;
	}

	HGLRC hContextRC;

	PIXELFORMATDESCRIPTOR pfd3;
	memset(&pfd3, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd3.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd3.nVersion = 1;
	pfd3.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfd3.iPixelType = PFD_TYPE_RGBA;
	pfd3.cColorBits = 24;
	pfd3.cAlphaBits = 8;
	pfd3.cDepthBits = 24;
	pfd3.iLayerType = PFD_MAIN_PLANE;

	if (iMajorVersion <= 2)
	{
		int iPixelFormat = ChoosePixelFormat(hWindowDC, &pfd3);
		if (iPixelFormat == 0)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 3)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		if (!SetPixelFormat(hWindowDC, iPixelFormat, &pfd3))
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 4)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		// Create the old style context (OpenGL 2.1 and before)
		hContextRC = wglCreateContext(hWindowDC); // local?
		if (!hContextRC)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 4.1)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		if (wglMakeCurrent(hWindowDC, hContextRC) == FALSE)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 4.2)", L"Fatal Error", MB_ICONERROR);
			return false;
		}
	}
	else if (WGLEW_ARB_create_context && WGLEW_ARB_pixel_format)
	{
		/*
		// Note: This comment code seems to not find a pixel format.

		const int iPixelFormatAttribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_SUPPORT_GDI_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			//WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			//WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 24,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_ALPHA_BITS_ARB, 8,
			//WGL_STENCIL_BITS_ARB, 8,
			//GL_ARB_framebuffer_object, GL_TRUE,
			0
		};

		int iPixelFormat2, iNumFormats;
		if (wglChoosePixelFormatARB(hWindowDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat2, (UINT*)&iNumFormats) == FALSE)
		{
			MessageBox(NULL, String::atow(string("Couldn't initialize OpenGL! (Part 4.5) Windows Error Code: ") + String::toString((::uint32)GetLastError())).c_str(), L"Fatal Error", MB_ICONERROR);
			return false;
		}

		if (iNumFormats == 0)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 4.6)", L"Fatal Error", MB_ICONERROR);
			return false;
		}
		*/

		int iPixelFormat2 = ChoosePixelFormat(hWindowDC, &pfd3);
		if (iPixelFormat2 == 0)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 3)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		// PFD seems to be only redundant parameter now
		PIXELFORMATDESCRIPTOR pfd3;
		if (DescribePixelFormat(hWindowDC, iPixelFormat2, sizeof(pfd3), &pfd3) == 0)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 4.8)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		if (!SetPixelFormat(hWindowDC, iPixelFormat2, &pfd3))
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 5)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		int iContextAttribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, iMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB, iMinorVersion,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0
		};
		hContextRC = wglCreateContextAttribsARB(hWindowDC, 0, iContextAttribs);
		if (!hContextRC)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 6)", L"Fatal Error", MB_ICONERROR);
			return false;
		}

		if (wglMakeCurrent(hWindowDC, hContextRC) == FALSE)
		{
			MessageBox(NULL, L"Couldn't initialize OpenGL! (Part 7)", L"Fatal Error", MB_ICONERROR);
			return false;
		}
		return true;
	}

	return false;
}

// programs/shaders
GPUProgram* OpenGL::addProgram(string& strVertexShaderFilePath, string& strFragmentShaderFilePath)
{
	GLProgram* pProgram = new GLProgram;

	pProgram->m_uiId = glCreateProgram();
	pProgram->m_uiVertexShader = prepareShader(GL_VERTEX_SHADER, strVertexShaderFilePath);
	pProgram->m_uiFragmentShader = prepareShader(GL_FRAGMENT_SHADER, strFragmentShaderFilePath);

	glAttachShader(pProgram->m_uiId, pProgram->m_uiVertexShader);
	glAttachShader(pProgram->m_uiId, pProgram->m_uiFragmentShader);

	string strError;

	glLinkProgram(pProgram->m_uiId);
	GLint uiError = glGetError();
	if (uiError != GL_NO_ERROR)
	{
		strError = "Failed to link program (" + String::toString(uiError) + ") (" + strVertexShaderFilePath + ", " + strFragmentShaderFilePath + ")";
		MessageBox(NULL, String::atow(strError).c_str(), L"Error", MB_OK);
	}

	GLint uiIsLinked = 0;
	glGetProgramiv(pProgram->m_uiId, GL_LINK_STATUS, &uiIsLinked);
	if (uiIsLinked == 0)
	{
		GLint uiMaxLength = 0;
		glGetProgramiv(pProgram->m_uiId, GL_INFO_LOG_LENGTH, &uiMaxLength); // The uiMaxLength includes the NULL character
		vector<GLchar> vecInfoLog(uiMaxLength);
		glGetProgramInfoLog(pProgram->m_uiId, uiMaxLength, &uiMaxLength, &vecInfoLog[0]);
		glDeleteProgram(pProgram->m_uiId);
		string strInfoLog = vecInfoLog.data();
		strError = "Failed to link program (GL_LINK_STATUS)\n\n" + String::toString(pProgram->m_uiId) + "\n\n" + strInfoLog;
		MessageBox(NULL, String::atow(strError).c_str(), L"Error", MB_OK);
	}

	pProgram->use();

	return pProgram;
}

uint32					OpenGL::prepareShader(uint32 uiShaderType, string& strShaderFilePath)
{
	while (glGetError() != GL_NO_ERROR) {}

	string strShaderCode = File::getText(strShaderFilePath);
	const char *pShaderCode = strShaderCode.c_str();
	string str;

	GLuint uiShader = glCreateShader(uiShaderType);
	if (glGetError() != GL_NO_ERROR)
	{
		str = "glCreateShader Failed\n\n" + strShaderFilePath;
		MessageBox(NULL, String::atow(str).c_str(), L"Error", MB_OK);
	}

	glShaderSource(uiShader, 1, &pShaderCode, NULL);
	if (glGetError() != GL_NO_ERROR)
	{
		str = "glShaderSource Failed\n\n" + strShaderFilePath;
		MessageBox(NULL, String::atow(str).c_str(), L"Error", MB_OK);
	}

	glCompileShader(uiShader);
	if (glGetError() != GL_NO_ERROR)
	{
		str = "glCompileShader Failed\n\n" + strShaderFilePath;
		MessageBox(NULL, String::atow(str).c_str(), L"Error", MB_OK);
	}

	GLint uiIsCompiled = 0;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &uiIsCompiled);
	if (uiIsCompiled == 0)
	{
		GLint uiMaxLength = 0;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &uiMaxLength); // The uiMaxLength includes the NULL character
		vector<GLchar> vecInfoLog(uiMaxLength);
		glGetShaderInfoLog(uiShader, uiMaxLength, &uiMaxLength, &vecInfoLog[0]);
		glDeleteShader(uiShader);
		string strInfoLog = vecInfoLog.data();
		str = "Failed to compile shader (GL_COMPILE_STATUS)\n\n" + strShaderFilePath + "\n\n" + strInfoLog;
		MessageBox(NULL, String::atow(str).c_str(), L"Error", MB_OK);
	}

	return uiShader;
}

void					OpenGL::resetProgram(void)
{
	glUseProgram(0);
}

// errors
void					OpenGL::checkForGLError(string strMarkerText)
{
	GLenum uiError = glGetError();
	if (uiError != GL_NO_ERROR)
	{
		printf("ERROR = %u [%s]\n", uiError, strMarkerText.c_str());
		////////////MessageBoxA(NULL, String::toString(uiError).c_str(), strMarkerText.c_str(), 0);
	}
}

// textures
uint32					OpenGL::addTexture(GPUProgram *pProgram, uint8 *pImageData, EImageFormat uiFormat, uvec2& vecTextureImageSize, bool bResetActiveTexture)
{
	checkForGLError("C0");

	pProgram->use();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	uint32 uiTextureBuffer;
	glGenTextures(1, &uiTextureBuffer);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	checkForGLError("C1");

	//glActiveTexture(GL_TEXTURE0);
	glActiveTexture(GL_TEXTURE3);
	checkForGLError("C1a");
	glBindTexture(GL_TEXTURE_2D, uiTextureBuffer);
	checkForGLError("C1b");
	glGenerateMipmap(GL_TEXTURE_2D);
	//glUniform1i(glGetUniformLocation(pProgram->getId(), "tex"), 0);

	checkForGLError("C2");

	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	checkForGLError("C3");

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	*/

	checkForGLError("C4");

	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	int level = 0;
	if (uiFormat == UNCOMPRESSED_RGB)
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGB8, vecTextureImageSize.x, vecTextureImageSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageData);
	else if (uiFormat == UNCOMPRESSED_RGBA)
		glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA8, vecTextureImageSize.x, vecTextureImageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImageData);
	else if(uiFormat == COMPRESSED_RGB_DXT1)
		glCompressedTexImage2D(GL_TEXTURE_2D, level, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, vecTextureImageSize.x, vecTextureImageSize.y, 0, (vecTextureImageSize.x * vecTextureImageSize.y) / 2, pImageData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	checkForGLError("C5");

	if (bResetActiveTexture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	checkForGLError("C6");

	return uiTextureBuffer;
}

// images
GPUImageBuffers*			OpenGL::addImageBuffers(GPUProgram *pProgram)
{
	GPUImageBuffers* pGpuImageBuffers = new GPUImageBuffers;

	// use the program
	pProgram->use();

	// generate VP and TP
	vector<GLfloat> vecVertexPositions = {
		-1,1,
		1,1,
		-1,-1,

		-1,-1,
		1,1,
		1,-1
	};
	vector<GLfloat> vecTexturePositions = {
		0,1,
		1,1,
		0,0,

		0,0,
		1,1,
		1,0
	};

	OpenGL::checkForGLError("H1");

	// send VP and TP to GPU
	glGenVertexArrays(1, &pGpuImageBuffers->m_vao);
	glBindVertexArray(pGpuImageBuffers->m_vao);

	OpenGL::checkForGLError("H2");

	glGenBuffers(1, &pGpuImageBuffers->m_vboVP);
	glBindBuffer(GL_ARRAY_BUFFER, pGpuImageBuffers->m_vboVP);
	glBufferData(GL_ARRAY_BUFFER, vecVertexPositions.size() * 2 * sizeof(GLfloat), vecVertexPositions.data(), GL_STATIC_DRAW);
	GLint iAttributeLocation1 = glGetAttribLocation(pProgram->getId(), "in_position");
	glEnableVertexAttribArray(iAttributeLocation1);
	glVertexAttribPointer(iAttributeLocation1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	OpenGL::checkForGLError("H3");

	glGenBuffers(1, &pGpuImageBuffers->m_vboTP);
	glBindBuffer(GL_ARRAY_BUFFER, pGpuImageBuffers->m_vboTP);
	glBufferData(GL_ARRAY_BUFFER, vecTexturePositions.size() * 2 * sizeof(GLfloat), vecTexturePositions.data(), GL_STATIC_DRAW);
	GLint iAttributeLocation2 = glGetAttribLocation(pProgram->getId(), "in_texcoord");
	glEnableVertexAttribArray(iAttributeLocation2);
	glVertexAttribPointer(iAttributeLocation2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	OpenGL::checkForGLError("H4");

	// reset active data
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	OpenGL::checkForGLError("H5");

	return pGpuImageBuffers;
}

GPUImage*				OpenGL::addImage(GPUProgram *pProgram, string& strImageFilePath, GPUImageBuffers *pGpuImageBuffers)
{
	// load image file
	ImageData image;
	Image::load(strImageFilePath, image);
	//ImageManager::swapRows(pImage->m_strRasterData, pImage->m_uiImageWidth, pImage->m_uiImageHeight); // todo

	// create object to return
	GLImage *pGLImage = new GLImage;
	pGLImage->m_pProgram = pProgram;
	pGLImage->m_pImageBuffers = pGpuImageBuffers ? pGpuImageBuffers : addImageBuffers(pProgram);

	// send texture to GPU
	pGLImage->m_uiTextureBuffer = addTexture(pProgram, image.m_pData, image.m_uiFormat, image.m_vecSize, true);

	return pGLImage;
}

void					OpenGL::resizeScene(uvec2& vecRenderSize)
{
	glViewport(0, 0, vecRenderSize.x, vecRenderSize.y);
}

// reset
void					OpenGL::reset(void)
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// screenshot
void					OpenGL::takeScreenshot(uvec2& vecSize, string& strBMPFilePath, bool bAlsoTakeJPEG)
{
	GLubyte *pPixelsBGRA32 = getScreenPixels(vecSize);
	string strNextFilePathOutBMP = File::getNextIncrementingFileName(strBMPFilePath);

	{
		ImageData image(UNCOMPRESSED_RGBA, vecSize, pPixelsBGRA32);

		Stream stream(strNextFilePathOutBMP);
		BMPFormat bmp(stream, image);

		bmp.serialize();

		delete[] image.m_pData;
		image.m_pData = nullptr;
	}

	if (bAlsoTakeJPEG)
	{
		string strNextFilePathOutJPG = Path::setExtWithCase(strNextFilePathOutBMP, "JPG");
		Image::convertToJPG(strNextFilePathOutBMP, strNextFilePathOutJPG);
	}
}

uint8*					OpenGL::getScreenPixels(uvec2& vecSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glPixelStorei(GL_PACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_PACK_LSB_FIRST, GL_TRUE);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
	glPixelStorei(GL_PACK_IMAGE_HEIGHT, 0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	const unsigned long uiImageDataSize = vecSize.x * vecSize.y * 4;
	GLubyte *pPixelsBGRA32 = new GLubyte[uiImageDataSize];

	glReadPixels(0, 0, vecSize.x, vecSize.y, GL_RGBA, GL_UNSIGNED_BYTE, pPixelsBGRA32);
	
	return pPixelsBGRA32;
}

/*
// axis
void					OpenGL::prepareAxis(void)
{
	float32 fMin = -5000.0f;
	float32 fMax = 5000.0f;

	vector<vec3> vecAxisVertexPositions;
	vector<vec3> vecAxisVertexColours;

	vecAxisVertexPositions.resize(6);
	vecAxisVertexColours.resize(6);

	// axis vertex positions

	// X
	vecAxisVertexPositions[0].x = fMin;
	vecAxisVertexPositions[0].y = 0.0f;
	vecAxisVertexPositions[0].z = 0.0f;

	vecAxisVertexPositions[1].x = fMax;
	vecAxisVertexPositions[1].y = 0.0f;
	vecAxisVertexPositions[1].z = 0.0f;

	// Y
	vecAxisVertexPositions[2].x = 0.0f;
	vecAxisVertexPositions[2].y = 0.0f;
	vecAxisVertexPositions[2].z = fMin;

	vecAxisVertexPositions[3].x = 0.0f;
	vecAxisVertexPositions[3].y = 0.0f;
	vecAxisVertexPositions[3].z = fMax;

	// Z
	vecAxisVertexPositions[4].x = 0.0f;
	vecAxisVertexPositions[4].y = fMin;
	vecAxisVertexPositions[4].z = 0.0f;

	vecAxisVertexPositions[5].x = 0.0f;
	vecAxisVertexPositions[5].y = fMax;
	vecAxisVertexPositions[5].z = 0.0f;

	// axis colours

	// X
	vecAxisVertexColours[0].x = 1.0f;
	vecAxisVertexColours[0].y = 0.0f;
	vecAxisVertexColours[0].z = 0.0f;

	vecAxisVertexColours[1].x = 1.0f;
	vecAxisVertexColours[1].y = 0.0f;
	vecAxisVertexColours[1].z = 0.0f;

	// Y
	vecAxisVertexColours[2].x = 0.0f;
	vecAxisVertexColours[2].y = 1.0f;
	vecAxisVertexColours[2].z = 0.0f;

	vecAxisVertexColours[3].x = 0.0f;
	vecAxisVertexColours[3].y = 1.0f;
	vecAxisVertexColours[3].z = 0.0f;

	// Z
	vecAxisVertexColours[4].x = 0.0f;
	vecAxisVertexColours[4].y = 0.0f;
	vecAxisVertexColours[4].z = 1.0f;

	vecAxisVertexColours[5].x = 0.0f;
	vecAxisVertexColours[5].y = 0.0f;
	vecAxisVertexColours[5].z = 1.0f;

	// add mesh
	GLEntity *pEntity = addEntity();
	vector<vec2> vecTexturePositions;
	m_pAxisMesh = pEntity->addMesh(vecAxisVertexPositions, vecTexturePositions, vecAxisVertexColours, vector<vec3>(), GL_LINES);
}
*/

uint32					OpenGL::addBuffer(uint32 uiShaderAttributeLocation, uint32 uiComponentCount)
{
	return addBufferRaw(uiShaderAttributeLocation, nullptr, 0, uiComponentCount);
}

uint32					OpenGL::addBuffer(uint32 uiShaderAttributeLocation, vector<vec2>& vecValues)
{
	return addBufferRaw(uiShaderAttributeLocation, (const char*)vecValues.data(), sizeof(float) * 2 * vecValues.size(), 2);
}

uint32					OpenGL::addBuffer(uint32 uiShaderAttributeLocation, vector<vec3>& vecValues)
{
	return addBufferRaw(uiShaderAttributeLocation, (const char*)vecValues.data(), sizeof(float) * 3 * vecValues.size(), 3);
}

uint32					OpenGL::addBufferRaw(uint32 uiShaderAttributeLocation, const char *pBufferData, uint32 uiBufferByteCount, uint32 uiComponentCount)
{
	uint32 uiBufferId = addArrayBuffer(uiBufferByteCount, pBufferData);

	glEnableVertexAttribArray(uiShaderAttributeLocation);
	glVertexAttribPointer(uiShaderAttributeLocation, uiComponentCount, GL_FLOAT, GL_FALSE, 0, 0);

	return uiBufferId;
}

void					OpenGL::removeBuffer(uint32 uiBufferId)
{
	glDeleteBuffers(1, &uiBufferId);
}

uint32					OpenGL::addArrayBuffer(uint32 uiBufferByteCount, const char *pBufferData)
{
	uint32 uiBufferId;
	glGenBuffers(1, &uiBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferId);

	if (pBufferData)
		glBufferData(GL_ARRAY_BUFFER, uiBufferByteCount, pBufferData, GL_STATIC_DRAW);

	return uiBufferId;
}

uint32					OpenGL::addLocationedArrayBuffer(uint32 uiShaderAttributeLocation, uint32 uiBufferByteCount, const char* pBufferData)
{
	return addBufferRaw(uiShaderAttributeLocation, pBufferData, uiBufferByteCount, sizeof(vec3) / sizeof(float32));
}

void					OpenGL::addToBuffer(uint32 uiBufferId, vector<vec2>& vecValues)
{
	// todo
}

void					OpenGL::addToBuffer(uint32 uiBufferId, vector<vec3>& vecValues)
{
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferId);

	GLint iBufferByteCount = 0;
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &iBufferByteCount);

	uint32 uiAddedDataByteCount = sizeof(float) * 3 * vecValues.size();
	uint32 uiNewDataByteCount = iBufferByteCount + uiAddedDataByteCount;
	char *pBufferData = new char[uiNewDataByteCount];
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, iBufferByteCount, pBufferData);
	memcpy(pBufferData + iBufferByteCount, vecValues.data(), uiAddedDataByteCount);

	glBufferData(GL_ARRAY_BUFFER, uiNewDataByteCount, pBufferData, GL_STATIC_DRAW);

	delete[] pBufferData;
}

void					OpenGL::setBufferData(uint32 uiBufferId, uint32 uiBufferByteCount, const char* pNewBufferData)
{
	glBindBuffer(GL_ARRAY_BUFFER, uiBufferId);
	glBufferData(GL_ARRAY_BUFFER, uiBufferByteCount, pNewBufferData, GL_STATIC_DRAW);
}