//
// DeviceResources.h - A wrapper for the Direct3D 12.X device and swapchain
//
// Explicit HDR10+GameDVR rendering variant.
//

#pragma once

namespace DX
{
    // Controls all the DirectX device resources.
    class DeviceResources
    {
    public:
        static constexpr unsigned int c_Enable4K_UHD         = 0x1;
        static constexpr unsigned int c_EnableQHD            = 0x2;
        static constexpr unsigned int c_EnableHDR            = 0x4;
        static constexpr unsigned int c_ReverseDepth         = 0x8;
        static constexpr unsigned int c_GeometryShaders      = 0x10;
        static constexpr unsigned int c_TessellationShaders  = 0x20;
        static constexpr unsigned int c_AmplificationShaders = 0x40;
        static constexpr unsigned int c_EnableDXR            = 0x80;

        DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM,
                        DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT,
                        UINT backBufferCount = 2,
                        unsigned int flags = 0) noexcept(false);
        ~DeviceResources();

        DeviceResources(DeviceResources&&) = default;
        DeviceResources& operator= (DeviceResources&&) = default;

        DeviceResources(DeviceResources const&) = delete;
        DeviceResources& operator= (DeviceResources const&) = delete;

#ifdef _GAMING_XBOX_SCARLETT
        void CreateDeviceResources(D3D12XBOX_CREATE_DEVICE_FLAGS createDeviceFlags = D3D12XBOX_CREATE_DEVICE_FLAG_NONE);
#else
        void CreateDeviceResources();
#endif
        void CreateWindowSizeDependentResources();
        void SetWindow(HWND window) noexcept { m_window = window; }
        void Prepare(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_PRESENT,
                     D3D12_RESOURCE_STATES afterState = D3D12_RESOURCE_STATE_RENDER_TARGET);
        void Present(D3D12_RESOURCE_STATES beforeState = D3D12_RESOURCE_STATE_RENDER_TARGET,
                     _In_opt_ const D3D12XBOX_PRESENT_PARAMETERS* params = nullptr);
        void Suspend();
        void Resume();
        void WaitForGpu() noexcept;
        void WaitForOrigin();

        // Direct3D Properties.
        void SetClearColor(_In_reads_(4) const float* rgba) noexcept { memcpy(m_clearColor, rgba, sizeof(m_clearColor)); }

        // Device Accessors.
        RECT GetOutputSize() const noexcept { return m_outputSize; }

        // Direct3D Accessors.
        auto                        GetD3DDevice() const noexcept          { return m_d3dDevice.Get(); }
        HWND                        GetWindow() const noexcept             { return m_window; }
        D3D_FEATURE_LEVEL           GetDeviceFeatureLevel() const noexcept { return m_d3dFeatureLevel; }
        ID3D12Resource*             GetRenderTarget() const noexcept       { return m_renderTargets[m_backBufferIndex].Get(); }
        ID3D12Resource*             GetDepthStencil() const noexcept       { return m_depthStencil.Get(); }
        ID3D12CommandQueue*         GetCommandQueue() const noexcept       { return m_commandQueue.Get(); }
        ID3D12CommandAllocator*     GetCommandAllocator() const noexcept   { return m_commandAllocators[m_backBufferIndex].Get(); }
        auto                        GetCommandList() const noexcept        { return m_commandList.Get(); }
        DXGI_FORMAT                 GetBackBufferFormat() const noexcept   { return m_backBufferFormat; }
        DXGI_FORMAT                 GetDepthBufferFormat() const noexcept  { return m_depthBufferFormat; }
        D3D12_VIEWPORT              GetScreenViewport() const noexcept     { return m_screenViewport; }
        D3D12_RECT                  GetScissorRect() const noexcept        { return m_scissorRect; }
        UINT                        GetCurrentFrameIndex() const noexcept  { return m_backBufferIndex; }
        UINT                        GetBackBufferCount() const noexcept    { return m_backBufferCount; }
        unsigned int                GetDeviceOptions() const noexcept      { return m_options; }

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const noexcept
        {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(
                m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                static_cast<INT>(m_backBufferIndex), m_rtvDescriptorSize);
        }
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const noexcept
        {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
        }

        // Direct3D HDR Game DVR support for Xbox One.
        ID3D12Resource* GetGameDVRRenderTarget() const noexcept { return m_renderTargetsGameDVR[m_backBufferIndex].Get(); }
        DXGI_FORMAT GetGameDVRFormat() const noexcept { return m_gameDVRFormat; }

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetGameDVRRenderTargetView() const noexcept
        {
            return CD3DX12_CPU_DESCRIPTOR_HANDLE(
                m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
                static_cast<INT>(m_backBufferCount + m_backBufferIndex), m_rtvDescriptorSize);
        }

    private:
        void RegisterFrameEvents();

        static constexpr size_t MAX_BACK_BUFFER_COUNT = 3;

        UINT                                                m_backBufferIndex;

        // Direct3D objects.
#ifdef _GAMING_XBOX_SCARLETT
        Microsoft::WRL::ComPtr<ID3D12Device8>               m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>  m_commandList;
#else
        Microsoft::WRL::ComPtr<ID3D12Device2>               m_d3dDevice;
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>   m_commandList;
#endif
        Microsoft::WRL::ComPtr<ID3D12CommandQueue>          m_commandQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator>      m_commandAllocators[MAX_BACK_BUFFER_COUNT];

        // Swap chain objects.
        Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[MAX_BACK_BUFFER_COUNT];
        Microsoft::WRL::ComPtr<ID3D12Resource>              m_depthStencil;

        // Presentation fence objects.
        Microsoft::WRL::ComPtr<ID3D12Fence>                 m_fence;
        UINT64                                              m_fenceValue;
        Microsoft::WRL::Wrappers::Event                     m_fenceEvent;
        D3D12XBOX_FRAME_PIPELINE_TOKEN                      m_framePipelineToken;

        // Direct3D rendering objects.
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_rtvDescriptorHeap;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>        m_dsvDescriptorHeap;
        UINT                                                m_rtvDescriptorSize;
        D3D12_VIEWPORT                                      m_screenViewport;
        D3D12_RECT                                          m_scissorRect;

        // Direct3D properties.
        DXGI_FORMAT                                         m_backBufferFormat;
        DXGI_FORMAT                                         m_depthBufferFormat;
        UINT                                                m_backBufferCount;
        float                                               m_clearColor[4];

        // Cached device properties.
        HWND                                                m_window;
        D3D_FEATURE_LEVEL                                   m_d3dFeatureLevel;
        RECT                                                m_outputSize;

        // DeviceResources options (see flags above)
        unsigned int                                        m_options;

        // Direct3D HDR Game DVR support for Xbox One.
        Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargetsGameDVR[MAX_BACK_BUFFER_COUNT];
        DXGI_FORMAT                                         m_gameDVRFormat;
    };
}
