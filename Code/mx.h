#pragma once

#include "Types.h"
#include <string>
#include <vector>
#include <unordered_map>

#include "ThirdParty/GLM/glm.hpp"
#include "ThirdParty/GLM/gtc/matrix_transform.hpp"
#include "ThirdParty/GLM/gtc/type_ptr.hpp"
#include "ThirdParty/GLM/gtx/matrix_decompose.hpp"

#include <Windows.h>

#include "ThirdParty/GLEW/GL/glew.h"
#include "ThirdParty/GLEW/GL/wglew.h"

#define min(x,y) ((x) < (y) ? (x) : (y)) // for GDI+
#define max(x,y) ((x) > (y) ? (x) : (y)) // for GDI+
#include <Gdiplus.h>
#undef min
#undef max

namespace mx
{
	extern bool					g_bDefaultBool;
	extern glm::vec2			g_vecDefaultVec2;
	extern glm::vec3			g_vecDefaultVec3;
	extern std::string			g_strBlankString;

	const uint32f				SECTOR_BYTE_COUNT = 2048;

	class GPU;
	class GraphicsLibrary;

	class OpenGL;
	class DirectX;
	
	class GPUFont;
	class GPUImage;
	class GPUImageBuffers;
	class GPUProgram;
	class GPUTexture;

	class GLTexture;
	class GLFont;
	class GLProgram;
	class GLImage;

	enum EGraphicsLibrary;
	enum EOperatingSystem;
	enum EPlatform;
	enum EVersionedGraphicsLibrary;
	enum EVersionedOperatingSystem;
	enum EVersionedPlatform;

	class DataPath;


	template<class T> class FormatManager;
	class Format;
	class ContainerFormat;
	enum EEndian;

	class BMPFormat;
	struct BMPFormat_Header1;
	struct BMPFormat_Header2_Version2;
	struct BMPFormat_Header2_Version3;
	struct BMPFormat_Header2_Version4;

	class CURFormat;

	class DDSFormat;
	struct DDSFormat_Header_DDS_HEADER;
	struct DDSFormat_Header_DDS_HEADER_DXT10;
	struct DDSFormat_Header_DDS_PIXELFORMAT_Part1;
	struct DDSFormat_Header_DDS_PIXELFORMAT_Part2;

	class ICOFormat;

	class JSON;
	enum EJSONType;
	class JSONString;
	class JSONValue;
	class JSONNumber;
	class JSONBoolean;
	class JSONNull;
	class JSONContainer;
	class JSONArray;
	class JSONObject;

	class URL;

	class ImageFile;
	class ImageManager;
	class RasterDataFormat;
	class RasterDataFormatManager;

	template<class Class> class IndexedInstance;
	template<class SingletonClass> class Singleton;

	template <class T> class VectorPool;
	template <class Key, class Value> class UMapContainer;

	class DataReader;
	class DataStream;
	enum EDataStreamType;
	class DataWriter;

	enum EStreamDeviceType;
	enum EStreamError;
	class Stream;
	class StreamException;
	class StreamPoint;
	class StreamFlow;
	class FileStreamFlow;
	class MemoryStreamFlow;
	class DirectoryStreamFlow;
	class FileStream;
	class MemoryStream;
	class DirectoryStream;
	class ReaderOrWriter;
	class Reader;
	class Writer;

	class Timing;

	class CompressionManager;
	class Crashes;
	class Debug;
	class EncryptionManager;
	enum EEncryptionAlgorithm;
	class File;
	class Dir;
	class ImageManager;
	class Image;
	class Input;
	enum EDXTCompressionType;
	enum ERasterDataFormat;
	class Math;
	class Memory;
	class Number;
	class Path;
	class Process;
	class Registry;
	class String;
	class StdVector;
	class Thread;

	struct Plane;
	struct Frustum;
	struct OBB;

	uint64_t pack754(float80 f, unsigned bits, unsigned expbits);
	float80 unpack754(uint64_t i, unsigned bits, unsigned expbits);
};