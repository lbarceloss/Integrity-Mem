#include <windows.h>
#include <cmath>
#include <cstdint>

const int BASE_POINTER_OFFSET = 0x00B006E8;
const int offsets[] = { 0x1C, 0xC, 0xC, 0x14, 0x18, 0x0, 0x0 };
const int numOffsets = sizeof(offsets) / sizeof(offsets[0]);

DWORD WINAPI AntiCheatMonitor(LPVOID lpParam) {
    uintptr_t baseAddr = (uintptr_t)GetModuleHandleA("ProjectG.exe");
    if (!baseAddr) {
        baseAddr = (uintptr_t)GetModuleHandleA(NULL); 

    bool warnedSemprePangya = false;
    bool warnedSemprePangya2 = false;
    bool warnedAlpha = false;
    bool warnedDiametro = false;
    bool warnedRaio = false;
    bool warnedDeslocX = false;
    bool warnedDeslocY = false;
    bool warnedShotType = false;

    while (true) {
        Sleep(500); 

        __try {
            uint8_t* spBytes = (uint8_t*)(baseAddr + 0x29B192);
            if (spBytes[0] != 0xD8 || spBytes[1] != 0x80 || spBytes[2] != 0xE8) {
                if (!warnedSemprePangya) {
                    MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Sempre Pangya alterada (Fora do padrao D8 80 E8)!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                    warnedSemprePangya = true;
                }
            } else {
                warnedSemprePangya = false;
            }

            uint8_t* spBytes2 = (uint8_t*)(baseAddr + 0x275198);
            if (spBytes2[0] != 0xD9 || spBytes2[1] != 0x98 || spBytes2[2] != 0xE4) {
                if (!warnedSemprePangya2) {
                    MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Sempre Pangya alterada (Fora do padrao D9 98 E4)!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                    warnedSemprePangya2 = true;
                }
            } else {
                warnedSemprePangya2 = false;
            }

            uintptr_t current = *(uintptr_t*)(baseAddr + BASE_POINTER_OFFSET);
            if (current != 0) {
                for (int i = 0; i < numOffsets; i++) {
                    uintptr_t nextPtr = current + offsets[i];
                    if (i < numOffsets - 1) {
                        current = *(uintptr_t*)nextPtr;
                        if (current == 0) break;
                    } else {
                        current = nextPtr;
                    }
                }

                if (current != 0) {
                    uintptr_t finalAddress = current;
                    
                    // Alpha Padrão = 90.0
                    float alpha = *(float*)(finalAddress + 0x8C);
                    if (std::abs(alpha - 90.0f) > 0.01f) {
                        if (!warnedAlpha) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Alpha alterada!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedAlpha = true;
                        }
                    } else warnedAlpha = false;

                    // Diametro Padrão = 100.0
                    float diametro = *(float*)(finalAddress + 0x88);
                    if (std::abs(diametro - 100.0f) > 0.01f) {
                        if (!warnedDiametro) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Diametro alterada!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedDiametro = true;
                        }
                    } else warnedDiametro = false;

                    // Circulo Raio Padrão = 20.0
                    float raio = *(float*)(finalAddress + 0x84);
                    if (std::abs(raio - 20.0f) > 0.01f) {
                        if (!warnedRaio) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Circulo Raio alterada!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedRaio = true;
                        }
                    } else warnedRaio = false;

                    // Deslocamento X, o valor nunca é 0.00
                    float deslocX = *(float*)(finalAddress + 0x78);
                    if (std::abs(deslocX) < 0.001f) { 
                        if (!warnedDeslocX) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Deslocamento X alterada (Esta como 0.00)!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedDeslocX = true;
                        }
                    } else warnedDeslocX = false;

                    // Deslocamento Y, o valor nunca é 0.00
                    float deslocY = *(float*)(finalAddress + 0x70);
                    if (std::abs(deslocY) < 0.001f) {
                        if (!warnedDeslocY) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do Deslocamento Y alterada (Esta como 0.00)!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedDeslocY = true;
                        }
                    } else warnedDeslocY = false;

                    // ShotType Padrão = 0
                    uint8_t shotType = *(uint8_t*)(finalAddress + 0x4C);
                    if (shotType != 0) {
                        if (!warnedShotType) {
                            MessageBoxA(NULL, "Alerta Anti-Cheat: Memoria do ShotType alterada!", "Anti-Cheat (ProjectG)", MB_ICONWARNING | MB_OK | MB_TOPMOST);
                            warnedShotType = true;
                        }
                    } else warnedShotType = false;
                }
            }
        } __except(EXCEPTION_EXECUTE_HANDLER) {
            
        }
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, AntiCheatMonitor, NULL, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
