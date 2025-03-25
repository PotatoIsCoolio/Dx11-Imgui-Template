#include "includes.h"
#include "MinHook.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

static bool CheckBox1 = false; 
static float slider = 10.0f; // mostly used for FOV and scale stuff

bool menuOpen = true;
bool init = false;

void CreateConsole()
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    SetConsoleTitleA("Potatos Console");
    std::cout << "[+] Console injected yip yap\n";
}

void InitImGui()
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange | ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(pDevice, pContext);
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 2.0f;
    style.ScrollbarRounding = 3.0f;
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(8, 6);
    style.FramePadding = ImVec2(10, 6);
    style.WindowPadding = ImVec2(10, 10);
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.05f, 0.10f, 1.00f); // Pluh this is like the background
    colors[ImGuiCol_TitleBg] = ImVec4(0.18f, 0.00f, 0.30f, 1.00f);  // Title bar (not the active one)
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.30f, 0.00f, 0.50f, 1.00f); // Title bar (Sexually active)
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.00f, 0.35f, 1.00f);// Buttons
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.45f, 0.00f, 0.65f, 1.00f);  // When you hover over a button
    colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.00f, 0.80f, 1.00f);  // wen button pressed
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.00f, 0.20f, 1.00f);  // another background color pluh
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.00f, 0.30f, 1.00f); // another background color pluh
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.00f, 0.35f, 1.00f); // another background color pluh
    colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.40f, 1.00f, 1.00f); // Checkboxes & other shit you can have
    colors[ImGuiCol_SliderGrab] = ImVec4(0.75f, 0.20f, 0.95f, 1.00f); // This is for sliders for like fov
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.30f, 1.00f, 1.00f);// This is for sliders for like fov
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 15.0f); // The font. Go to C:\Windows\Fonts\ for more fonts
}

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYUP && wParam == VK_INSERT) // CHange this to the key you want to have set to open the menu
        menuOpen = !menuOpen;                    // Default one is Insert

    if (menuOpen && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {   
        // Tbh idk what this stuff does I didnt make this
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
        {
            pDevice->GetImmediateContext(&pContext);
            DXGI_SWAP_CHAIN_DESC sd;
            pSwapChain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();
            oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
            InitImGui();
            init = true;
        }
        else
            return oPresent(pSwapChain, SyncInterval, Flags);
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (menuOpen)
    {
        ImGui::Begin("Imgui | PotatoIsCool"); // Title of the imgui

        if (ImGui::BeginTabBar("Tabs")) // Makes the tabs for the imgui so you can have Main and Settings add more if you want
        {
            if (ImGui::BeginTabItem("Main")) // adds the main tab
            {
                ImGui::Text("Thx for using this template <33");
                ImGui::Checkbox("CheckBox", &CheckBox1);
                ImGui::SliderFloat("Slider", &slider, 1.0f, 100.0f, "%.1f");

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Settings")) // Adds the setting tab
            {
                if (ImGui::Button("Close game"))
                    exit(0);

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    ImGui::Render();
    pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    CreateConsole(); // Remove this line if you want to remove the console
    if (MH_Initialize() != MH_OK)
    {
        MessageBox(NULL, "MinHook failed :c", "Error!!!", MB_OK);
        return 0;
    }

    bool init_hook = false;
    do
    {
        if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
        {
            kiero::bind(8, (void**)&oPresent, hkPresent);
            init_hook = true;
        }
    } while (!init_hook);
    return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hMod);
        std::thread(MainThread, hMod).detach(); // Using this instead of create threads make a type of "eac bypass"
        break;                                  // As eac checks for create thread
    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        MH_Uninitialize();
        break;
    }
    return TRUE;
}