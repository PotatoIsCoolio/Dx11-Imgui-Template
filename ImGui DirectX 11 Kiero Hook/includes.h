#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include <thread>
#include <string>
#include <iostream>
#include "Utils/Utils.h"
#include "Utils/font.h"
#include "Utils/XorStr.h"
#include "Utils/textures.h"
#include <cfg.h>
#include "imgui/imgui_elements.h"
#include <d3dx11.h>
#include <D3DX11tex.h> //How to Fix It Not Being Included https://stackoverflow.com/questions/7910636/d3dx11-h-missing


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;