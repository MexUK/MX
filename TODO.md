add DirectX GPU code.

fix all compiler warnings.

add code for memory stream.

add code for directory stream.

support x64 too.

rename UMapContainer to UMapPool.

ensure both DLL injection techniques are in the same file. (currently one is in Static/Memory and the other one is in Static/Process).

tidy Static/Memory.

split Static/Input into 2 places. (currently it contains both GUI input based code and keyboard input based code).

research better way to load/unload gdi plus. only load if not loaded. only unload if not loaded by MX.

support other platforms too.

(This todo list is incomplete).