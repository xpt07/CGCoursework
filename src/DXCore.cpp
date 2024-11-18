#include "../inc/DXCore.h"

void DXCore::init(int width, int height, HWND hwnd, bool window_fullscreen)
{
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = window_fullscreen ? false : true;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL fl;
	fl = D3D_FEATURE_LEVEL_11_0;

	Adapter adapter;
	adapter.findAdapter();

	D3D11CreateDeviceAndSwapChain(adapter.adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&fl,
		1,
		D3D11_SDK_VERSION,
		&sd,
		&swapchain,
		&device,
		NULL,
		&devicecontext);

	swapchain->SetFullscreenState(window_fullscreen, NULL);

}
