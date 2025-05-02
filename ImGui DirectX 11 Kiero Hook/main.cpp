#define IMGUI_DEFINE_MATH_OPERATORS
#include "includes.h"
#include "MinHook.h"


#pragma comment(lib, "d3dx11.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef HRESULT(__stdcall* Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

static bool CheckBox1 = false; 
static float slider = 10.0f;




void CreateConsole()
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    SetConsoleTitleA("Developer Console");
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

    ImFontConfig cfg;
    font::segue_semibold = io.Fonts->AddFontFromMemoryTTF(segue_semibold, sizeof(segue_semibold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::segue_semibold_tabs = io.Fonts->AddFontFromMemoryTTF(segue_semibold, sizeof(segue_semibold), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic()); //font Loading For Buttons And Menu, & Allat Shit 
    font::segue_bold = io.Fonts->AddFontFromMemoryTTF(segue_bold, sizeof(segue_bold), 18.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    io.MouseDrawCursor = true;
    io.WantCaptureMouse = true;


    io.FontGlobalScale = 1.0f;
    // ImGui Rounnding Style shit
    style.WindowPadding = ImVec2(15, 15);
    style.WindowRounding = 5.0f;
    style.FramePadding = ImVec2(5, 5);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.IndentSpacing = 25.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 3.0f;

    // colors for the style yap yap yap
    style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);

    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.30f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.70f, 0.30f, 0.80f, 0.50f);

    // Other colors
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    //style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };

    if (texture::ragebot == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, ragebot, sizeof(ragebot), &info, pump, &texture::ragebot, 0);
    if (texture::legitbot == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, legitbot, sizeof(legitbot), &info, pump, &texture::legitbot, 0);

    if (texture::visuals == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, visuals, sizeof(visuals), &info, pump, &texture::visuals, 0);
    if (texture::misc == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, misc, sizeof(misc), &info, pump, &texture::misc, 0);
    if (texture::world == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, world, sizeof(world), &info, pump, &texture::world, 0);
    if (texture::settings == nullptr) D3DX11CreateShaderResourceViewFromMemory(pDevice, settings, sizeof(settings), &info, pump, &texture::settings, 0);

}

LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_KEYUP && wParam == VK_INSERT) // CHange this to the key you want to have set to open the menu 
        UI::show = !UI::show;                    // Default one is Insert
                                                                 //@Zynco1337 - Im Gonna Keep It Like This Because im Not Gonna Try Coding a Mouse Fix Due To it Being Aids Asf
    if (UI::show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if (!init)
    {   
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


    //To Be Clear I Spent Around 10 Minutes on This ImGui So ITS NOT GOOD
    // But Good To Keep WOrking on It May Coming Back To It - Zynco
    if (UI::show)
    {
        ImGui::SetNextWindowSize(ImVec2(550, 600), ImGuiCond_Once);
        ImGui::Begin("ImGui DX11 Base  BY PotatoIsCoolio & Zynco1337", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::BeginChild("##sidebar", ImVec2(200, 0), true);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.4f, 1.0f));
        const char* labels[] = { "Tab 0", "Tab 1", "Tab 2", "Tab 3", "Tab 4", "Tab 5" };
        void* textures[] = {
            texture::ragebot,
            texture::legitbot,
            texture::visuals,
            texture::misc,
            texture::world,
            texture::settings
        };

        for (int i = 0; i < IM_ARRAYSIZE(labels); ++i)
        {
            if (UI::tab == i)
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.4f, 0.8f, 1.0f));
            else
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));


            if (textures[i])
            {
                ImGui::PushID(i);
                if (ImGui::ImageButton(textures[i], ImVec2(20, 20))) UI::tab = i;
                ImGui::SameLine();
                if (ImGui::Button(labels[i], ImVec2(120, 25))) UI::tab = i;
                ImGui::PopID();
            }
            else
            {
                if (ImGui::Button(labels[i], ImVec2(150, 25))) UI::tab = i;
            }

            ImGui::PopStyleColor();
            ImGui::Spacing();
        }
        ImGui::PopStyleColor(3); // restore button style
        ImGui::PopStyleVar();
        ImGui::EndChild();

        ImGui::SameLine();

        ImGui::BeginChild("##content", ImVec2(0, 0), true);
        {
            ImGui::Spacing();

            if (UI::tab == 0)
            {
                ImGui::Text("Tab 0");
                ImGui::Separator();
                // Add your Ragebot content here





                //Example Stuff
                static bool CheckboxAlwaysOn = true; // static bool Is NOT NEEDED
                ImGui::Checkbox("Checkbox On", &CheckboxAlwaysOn);

                static bool CheckboxOff = false; // static bool Is NOT NEEDED
                ImGui::Checkbox("Checkbox Off", &CheckboxOff);

                static float knobfloat = 0.350f; // static float Is NOT NEEDED
                ImGui::KnobFloat("Knob Float", &knobfloat, 0.000f, 1.000f, "%.3fF", NULL);  // Not a Normal Slider THis Is An Knob Its Mostly Style Just Looks better

                static int Keybind; // static int Is NOT NEEDED
                ImGui::Keybind("Keybind", &Keybind);


            }






            else if (UI::tab == 1)
            {
                ImGui::Text("Tab 1");
                ImGui::Separator();
                // Add your Legitbot content here
            }






            else if (UI::tab == 2)
            {
                ImGui::Text("Tab 2");
                ImGui::Separator();
                // Add your Visuals content here
            }





            else if (UI::tab == 3)
            {
                ImGui::Text("Tab 3");
                ImGui::Separator();
                // Add your MISC content here
            }






            else if (UI::tab == 4)
            {
                ImGui::Text("Tab 4");
                ImGui::Separator();
                // Add your World content here
            }

            else if (UI::tab == 5)
            {
                ImGui::Text("Tab 5");
                ImGui::Separator();
                // Add your Settings content here

                if (ImGui::Button("Uninject", ImVec2(200, 30))) //Un-Injects From The Procces
                    kiero::shutdown();

                if (ImGui::Button("Crash Game", ImVec2(200, 30))) //Completly Crashes Your Game / Potatos Code
                    exit(0);
            }
        }
        ImGui::EndChild();
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
