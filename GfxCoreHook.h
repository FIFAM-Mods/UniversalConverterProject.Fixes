#pragma once
#include "plugin-std.h"

using namespace plugin;

unsigned int GfxCoreAddress(unsigned int addr);
void InstallGfxCoreHook(void (*callback)());
