Please note, this todo list is incomplete.



add DirectX GPU code.

fix all compiler warnings.

add code for directory stream.

support x64 too.

research better way to load/unload gdi plus. only load if not loaded. only unload if loaded by MX.

support other platforms too.

use size_t where applicable instead of uint64/uint32/unsigned long/etc.

try to support as much of the BMP format that I can.

add an Image::save function, to go with the Image::load function.

potentially replace gdi plus usage with stb usage.

add signed integers to streams.

add float80 for streams, and String::pack/String::unpack.

don't use tellg for fetching stream position. e.g. in Stream::size.

