#include "GfxCoreHook.h"

unsigned int gOriginalLoadGfxCore = 0;
unsigned int hLibrary = 0;
void (*gCallback)() = nullptr;

unsigned int GfxCoreAddress(unsigned int addr) {
    return hLibrary + addr;
}

bool METHOD OnLoadGfxCore(void **info, DUMMY_ARG, const wchar_t *libFileName, const char *procName) {
    bool result = false;
    if (gOriginalLoadGfxCore) {
        result = CallMethodAndReturnDynGlobal<bool>(gOriginalLoadGfxCore, info, libFileName, procName);
        if (result) {
            void *gfxCore = info[0];
            hLibrary = (unsigned int)info[1];
            gCallback();
        }
    }
    return result;
}

void InstallGfxCoreHook(void (*callback)()) {
    gOriginalLoadGfxCore = patch::RedirectCall(0x45BBEF, OnLoadGfxCore);
    gCallback = callback;
}
