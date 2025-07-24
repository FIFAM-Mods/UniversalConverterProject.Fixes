#include "plugin-std.h"
#include "license_check/license_check.h"
#include "GfxCoreHook.h"

AUTHOR_INFO("UniversalConverterProject.Fixes 1.0.0 by Dmitri");

using namespace plugin;

void __declspec(naked) Fix_ACA1A0_FBFF80() {
    __asm {
        test ecx, ecx
        je SKIP_ACA1A0_FBFF80
        mov eax, 0xECA1A0
        call eax
        mov ecx, 0xFBFF80
        jmp ecx
    SKIP_ACA1A0_FBFF80:
        mov ecx, 0xFBFFA6
        jmp ecx
    }
}

class UniversalConverterProjectFixes {
public:
    static bool UCPPluginInstalled() {
        auto pluginPath = FM::GameDirPath(L"plugins\\UniversalConverterProject.Main.asi");
        auto attrs = GetFileAttributesW(pluginPath.c_str());
        return (attrs != INVALID_FILE_ATTRIBUTES) && !(attrs & FILE_ATTRIBUTE_DIRECTORY);
    }

    static void InstallGfxCorePatches() {
        // set 32-bit render mode for EAGL PC Rertrival (Hardware Acceleration Fix)
        patch::SetUInt(GfxCoreAddress(0x2D63C5 + 1), 32);
        // old fonts glyph buffer fix
        patch::SetUInt(GfxCoreAddress(0x3DAE47 + 2), 6'000);
    }

    UniversalConverterProjectFixes() {
        
        if (!CheckLicense(L"UniversalConverterProject.Fixes.asi"))
            return;

        if (UCPPluginInstalled())
            Warning(L"UniversalConverterProject.Main plugin is already installed. You don't need UniversalConverterProject.Fixes plugin. Please remove UniversalConverterProject.Fixes.asi from \"plugins\" folder.");

        auto v = FM::GetAppVersion();
        auto version = v.id();

        if (version == VERSION_FM_13) {
            InstallGfxCoreHook(InstallGfxCorePatches);

            // season transition crash (returning player from loan)
            patch::RedirectJump(0xFBFF7B, Fix_ACA1A0_FBFF80);
        }
    }
};

UniversalConverterProjectFixes universalConverterProjectFixes;
