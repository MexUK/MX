#pragma once

#include <string>
#include <vector>
#include "mx.h"
#include "Image/ERasterDataFormat.h"

class mx::GraphicsLibrary
{
public:
	virtual bool						initContext(HWND hWindow, int32 iMajorVersion, int32 iMinorVersion) = 0;

	virtual mx::GPUProgram*				addProgram(std::string& strVertexShaderFilePath, std::string& strFragmentShaderFilePath) = 0;
	virtual mx::GPUImageBuffers*		addImageBuffers(GPUProgram *pProgram) = 0;
	virtual mx::GPUImage*				addImage(GPUProgram *pProgram, std::string& strImageFilePath, GPUImageBuffers *pGpuImageBuffers = nullptr) = 0;
	virtual uint32						addTexture(GPUProgram *pProgram, std::string& strTextureRasterData, ERasterDataFormat uiFormat, glm::uvec2& vecTextureImageSize, bool bResetActiveTexture = false) = 0;

	virtual void						resizeScene(glm::uvec2& vecRenderSize) = 0;
	virtual void						reset(void) = 0;
	virtual void						resetProgram(void) = 0;

	virtual void						takeScreenshot(glm::uvec2& vecSize, std::string& strBMPFilePath, bool bAlsoTakeJPEG = false) = 0;
	virtual uint8*						getScreenPixels(glm::uvec2& vecSize) = 0;

	virtual uint32						addBuffer(uint32 uiShaderAttributeLocation, uint32 uiComponentCount) = 0;
	virtual uint32						addBuffer(uint32 uiShaderAttributeLocation, std::vector<glm::vec2>& vecValues) = 0;
	virtual uint32						addBuffer(uint32 uiShaderAttributeLocation, std::vector<glm::vec3>& vecValues) = 0;
	virtual uint32						addArrayBuffer(uint32 uiBufferByteCount, const char* pBufferData = nullptr) = 0;
	virtual uint32						addLocationedArrayBuffer(uint32 uiShaderAttributeLocation, uint32 uiBufferByteCount, const char* pBufferData = nullptr) = 0;

	virtual void						removeBuffer(uint32 uiBufferId) = 0;

	virtual void						addToBuffer(uint32 uiBufferId, std::vector<glm::vec2>& vecValues) = 0; // slow
	virtual void						addToBuffer(uint32 uiBufferId, std::vector<glm::vec3>& vecValues) = 0; // slow

	virtual void						setBufferData(uint32 uiBufferId, uint32 uiBufferByteCount, const char* pNewBufferData) = 0;

private:
	virtual uint32						prepareShader(uint32 uiShaderType, std::string& strShaderFilePath) = 0;
};