#pragma once

#include "Types.h"

#include <string>
#include <vector>
#include <unordered_map>

#include "ThirdParty/GLM/glm.hpp"
#include "ThirdParty/GLM/gtc/matrix_transform.hpp"
#include "ThirdParty/GLM/gtc/type_ptr.hpp"
#include "ThirdParty/GLM/gtx/matrix_decompose.hpp"

// Visual Studio
#if _WIN32 || _WIN64
	#if _WIN64
		#define MX_ARCHITECTURE_64
	#else
		#define MX_ARCHITECTURE_32
	#endif
#endif
// GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define MX_ARCHITECTURE_64
	#else
		#define MX_ARCHITECTURE_32
	#endif
#endif

#ifndef MX_SKIP_THE_NO_DEFINES
	#define NOGDICAPMASKS
	//#define NOVIRTUALKEYCODES
	//#define NOWINMESSAGES
	//#define NOWINSTYLES
	#define NOSYSMETRICS
	#define NOMENUS
	#define NOICONS
	#define NOKEYSTATES
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	//#define NOSHOWWINDOW
	#define OEMRESOURCE
	#define NOATOM
	//#define NOCLIPBOARD
	#define NOCOLOR
	//#define NOCTLMGR
	#define NODRAWTEXT
	//#define NOGDI // Needed by GDI+
	#define NOKERNEL
	//#define NOUSER
	//#define NONLS
	//#define NOMB
	#define NOMEMMGR
	//#define NOMETAFILE // Needed by GDI+
	#define NOMINMAX
	//#define NOMSG
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	//#define NOTEXTMETRIC
	#define NOWH
	#define NOWINOFFSETS
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
#endif
#include <Windows.h>

#include "ThirdParty/GLEW/GL/glew.h"
#include "ThirdParty/GLEW/GL/wglew.h"

#include "ThirdParty/STB/stb_image.h"

#define min(x,y) ((x) < (y) ? (x) : (y)) // for GDI+
#define max(x,y) ((x) > (y) ? (x) : (y)) // for GDI+
#include <Gdiplus.h>
#undef min
#undef max

namespace mx
{
	// reference parameters
	extern bool					g_bDefaultBool;
	extern glm::vec2			g_vecDefaultVec2;
	extern glm::uvec2			g_vecDefaultUvec2;
	extern glm::vec3			g_vecDefaultVec3;
	extern std::string			g_strBlankString;
	
	// misc
	const uint32				SECTOR_BYTE_COUNT = 2048;
	
	// configuration
	enum EGraphicsLibrary;
	enum EOperatingSystem;
	enum EPlatform;
	enum EVersionedGraphicsLibrary;
	enum EVersionedOperatingSystem;
	enum EVersionedPlatform;
	
	// data
	class Reader;
	class Writer;
	class ReaderOrWriter;
	
	enum EEndian;
	
	class Stream;
	class StreamException;
	enum EStreamDeviceType;
	enum EStreamError;
	
	class StreamFlow;
	class FileStreamFlow;
	class MemoryStreamFlow;
	class DirectoryStreamFlow;
	
	class FileStream;
	class MemoryStream;
	class DirectoryStream;
	
	// format
	class Format;

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
	
	// GPU
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
	
	// object
	template<class Class> class IndexedInstance;
	template<class SingletonClass> class Singleton;
	
	// pool
	template <class T> class VectorPool;
	template <class Key, class Value> class UMapPool;
	
	// static
	class Crashes;
	class DataPath;
	class Debug;
	class Dir;
	class File;
	class GUI;
	class Image;
	class Input;
	class Math;
	class Memory;
	class Number;
	class Path;
	class Process;
	class Registry;
	class String;
	class StdVector;
	class Timing;
	
	// static components
	struct Plane;
	struct Frustum;
	struct OBB;
	class ImageData;
	enum EImageFormat;
	
	// third party
	uint64_t pack754(float80 f, unsigned bits, unsigned expbits);
	float80 unpack754(uint64_t i, unsigned bits, unsigned expbits);
};