#pragma once

#include "mx.h"
#include "GPU/Base/GraphicsLibrary.h"

class mx::DirectX : public mx::GraphicsLibrary
{
public:
	bool						initContext(HWND hWindow, int32 iMajorVersion, int32 iMinorVersion);

	GPUProgram*					addProgram(std::string& strVertexShaderFilePath, std::string& strFragmentShaderFilePath);
	GPUImageBuffers*			addImageBuffers(GPUProgram *pProgram);
	GPUImage*					addImage(GPUProgram *pProgram, std::string& strImageFilePath, GPUImageBuffers *pGpuImageBuffers = nullptr);
	uint32						addTexture(GPUProgram *pProgram, std::string& strTextureRasterData, ERasterDataFormat uiFormat, glm::uvec2& vecTextureImageSize, bool bResetActiveTexture = false);

	void						resizeScene(glm::uvec2& vecRenderSize);
	void						reset(void);
	void						resetProgram(void);

	void						takeScreenshot(glm::uvec2& vecSize, std::string& strBMPFilePath, bool bAlsoTakeJPEG = false);
	uint8*						getScreenPixels(glm::uvec2& vecSize);

	uint32						addBuffer(uint32 uiShaderAttributeLocation, uint32 uiComponentCount);
	uint32						addBuffer(uint32 uiShaderAttributeLocation, std::vector<glm::vec2>& vecValues);
	uint32						addBuffer(uint32 uiShaderAttributeLocation, std::vector<glm::vec3>& vecValues);
	uint32						addArrayBuffer(uint32 uiBufferByteCount, const char* pBufferData = nullptr);
	uint32						addLocationedArrayBuffer(uint32 uiShaderAttributeLocation, uint32 uiBufferByteCount, const char* pBufferData = nullptr);

	void						removeBuffer(uint32 uiBufferId);

	void						addToBuffer(uint32 uiBufferId, std::vector<glm::vec2>& vecValues);
	void						addToBuffer(uint32 uiBufferId, std::vector<glm::vec3>& vecValues);

	void						setBufferData(uint32 uiBufferId, uint32 uiBufferByteCount, const char* pNewBufferData);

private:
	uint32						prepareShader(uint32 uiShaderType, std::string& strShaderFilePath);
};