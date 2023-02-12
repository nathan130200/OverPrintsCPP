#pragma once

#ifndef HAVE_APPLICATION_H
#define HAVE_APPLICATION_H

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <imgui.h>
#include <string>
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#endif // !HAVE_APPLICATION_H

#include <memory>

#ifndef SAFE_RELEASE

#define SAFE_RELEASE(Ptr) do {\
	if (Ptr) {\
		Ptr->Release();\
		Ptr = NULL;\
	}\
} while (false);

#endif // !SAFE_RELEASE

static ID3D11Device* g_pRenderDevice = nullptr;
static ID3D11DeviceContext* g_pDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_pRenderView = nullptr;
static int32_t g_ExitCode = 0;

bool CreateDeviceD3D(HWND);
void CleanupDeviceD3D();
void CreateRenderTarget();
void Quit(int32_t exit_code);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


extern const char* s_AppClass;
extern const char* s_AppTitle;
extern void WINAPI Application_OnInit(int argc, char** argv);
extern void WINAPI Application_OnRender();