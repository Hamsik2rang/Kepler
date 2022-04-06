#pragma once

#include <iostream>
#include <algorithm>
#include <utility>
#include <functional>
#include <memory>

#include <string>
#include <vector>
#include <sstream>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

// for Windows
#ifdef KEPLER_PLATFORM_WINDOWS
#include <Windows.h>

#include "framework.h"
#include "resource.h"

#endif
#ifdef KEPLER_GRAPHICS_API_DIRECTX
#include <d3d11.h>
#endif