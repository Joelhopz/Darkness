#include "engine/graphics/dx12/DX12Debug.h"
#include "engine/graphics/dx12/DX12Device.h"
#include <D3D12SDKLayers.h>
#include <map>
#include "tools/ComPtr.h"

using namespace std;
using namespace tools;

namespace engine
{
    namespace implementation
    {
        static map<ID3D12Device*, ComPtr<ID3D12DebugDevice>> debugDevices;
        static GraphicsDebug debugInitializer;

        GraphicsDebug::GraphicsDebug()
        {
            auto res = D3D12GetDebugInterface(DARKNESS_IID_PPV_ARGS(m_dbgInterface.GetAddressOf()));
            ASSERT(SUCCEEDED(res));
            m_dbgInterface->EnableDebugLayer();

            /*m_debug1 = m_dbgInterface.As<ID3D12Debug1>();
            m_debug1->SetEnableSynchronizedCommandQueueValidation(TRUE);
            m_debug1->SetEnableGPUBasedValidation(TRUE);*/
        }

        GraphicsDebug::~GraphicsDebug()
        {
            for (auto& pair : debugDevices)
            {
                pair.second->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
            }
        }

        void GraphicsDebug::addDevice(ID3D12Device* device)
        {
            if (debugDevices.find(device) == debugDevices.end())
            {
                ComPtr<ID3D12DebugDevice> debugDevice;
                auto res = device->QueryInterface(debugDevice.GetAddressOf());
                ASSERT(SUCCEEDED(res));
                debugDevices[device].Swap(debugDevice);
            }
        }
    }
}
