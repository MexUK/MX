#include "DirectX.h"

using namespace std;
using namespace glm;
using namespace mx;

bool							DirectX::initContext(HWND hWindow, int32 iMajorVersion, int32 iMinorVersion)
{
	return false;
}

GPUImageBuffers*				DirectX::addImageBuffers(GPUProgram *pProgram)
{
	return nullptr;
}

GPUImage*						DirectX::addImage(GPUProgram *pProgram, string& strImageFilePath, GPUImageBuffers *pGpuImageBuffers)
{
	return nullptr;
}

GPUProgram*						DirectX::addProgram(string& strVertexShaderFilePath, string& strFragmentShaderFilePath)
{
	return nullptr;
}

uint32							DirectX::addTexture(GPUProgram *pProgram, uint8* pImageData, EImageFormat uiFormat, uvec2& vecTextureImageSize, bool bResetActiveTexture)
{
	return -1;
}

void							DirectX::resizeScene(uvec2& vecRenderSize)
{
}

void							DirectX::reset(void)
{
}

void							DirectX::resetProgram(void)
{
}

void							DirectX::takeScreenshot(uvec2& vecSize, string& strBMPFilePath, bool bAlsoTakeJPEG)
{
}

uint8*							DirectX::getScreenPixels(uvec2& vecSize)
{
	return nullptr;
}

uint32							DirectX::prepareShader(uint32 uiShaderType, string& strShaderFilePath)
{
	return -1;
}

uint32							DirectX::addBuffer(uint32 uiShaderAttributeLocation, uint32 uiComponentCount)
{
	return -1;
}

uint32							DirectX::addBuffer(uint32 uiShaderAttributeLocation, vector<vec2>& vecValues)
{
	return -1;
}

uint32							DirectX::addBuffer(uint32 uiShaderAttributeLocation, vector<vec3>& vecValues)
{
	return -1;
}

uint32							DirectX::addArrayBuffer(uint32 uiBufferByteCount, const char* pBufferData)
{
	return -1;
}

uint32							DirectX::addLocationedArrayBuffer(uint32 uiShaderAttributeLocation, uint32 uiBufferByteCount, const char* pBufferData)
{
	return -1;
}

void							DirectX::removeBuffer(uint32 uiBufferId)
{
}

void							DirectX::addToBuffer(uint32 uiBufferId, vector<vec2>& vecValues)
{
}

void							DirectX::addToBuffer(uint32 uiBufferId, vector<vec3>& vecValues)
{
}

void							DirectX::setBufferData(uint32 uiBufferId, uint32 uiBufferByteCount, const char* pNewBufferData)
{
}