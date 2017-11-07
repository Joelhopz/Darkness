#include "engine/graphics/vulkan/VulkanInstance.h"
#include "tools/Debug.h"
#include "engine/graphics/ShaderStorage.h"
#include "engine/Engine.h"
#include <Windows.h>

using namespace std;

VkResult CreateDebugReportCallbackEXT(
    VkInstance instance, 
    const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugReportCallbackEXT* pCallback)
{
    auto func = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
                reinterpret_cast<void*>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT")));

    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugReportCallbackEXT(
    VkInstance instance, 
    VkDebugReportCallbackEXT callback,
    const VkAllocationCallbacks* pAllocator)
{
    auto func = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
                reinterpret_cast<void*>(vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT")));

    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

static VkBool32 debugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT /*objType*/,
    uint64_t /*obj*/,
    size_t /*location*/,
    int32_t /*code*/,
    const char* /*layerPrefix*/,
    const char* msg,
    void* /*userData*/)
{
    if ((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) ||
        (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) ||
        (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) ||
        (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT))
    {
        OutputDebugStringA(msg);
        OutputDebugStringA("\n");
        return VK_TRUE;
    }
    else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
    {
        OutputDebugStringA(msg);
        OutputDebugStringA("\n");
        ASSERT(false);
        return VK_TRUE;
    }
    else
    {
        OutputDebugStringA(msg);
        OutputDebugStringA("\n");
        return VK_TRUE;
    }
    return VK_FALSE;
}

VulkanInstance::VulkanInstance()
    : //m_instance{ vkDestroyInstance }
    //, m_callback( m_instance, DestroyDebugReportCallbackEXT )
     m_validationLayers{ 
        "VK_LAYER_LUNARG_core_validation"
        /*"VK_LAYER_LUNARG_standard_validation",
        "VK_LAYER_LUNARG_core_validation",
        "VK_LAYER_LUNARG_image",
        "VK_LAYER_LUNARG_object_tracker",
        "VK_LAYER_LUNARG_parameter_validation",
        "VK_LAYER_LUNARG_swapchain",
        "VK_LAYER_NV_optimus",
        "VK_LAYER_RENDERDOC_Capture"*/
    }
    , m_requiredExtensions{
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME
#ifndef NDEBUG
        ,VK_EXT_DEBUG_REPORT_EXTENSION_NAME
#endif
    }
{
#ifndef NDEBUG
    if (!checkValidationLayerSupport())
    {
        ASSERT(false);
    }
#endif

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Darkness";
    appInfo.apiVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Darkness Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledLayerCount = 0;
    
    // validation layers
#ifndef NDEBUG
    createInfo.enabledLayerCount = static_cast<uint32_t>(m_validationLayers.size());
    createInfo.ppEnabledLayerNames = m_validationLayers.data();
#endif

    // extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(m_requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = m_requiredExtensions.data();

    auto result = vkCreateInstance(&createInfo, nullptr, &m_instance);
    ASSERT(result == VK_SUCCESS);

    std::vector<VkExtensionProperties> properties = extensionProperties();

#ifndef NDEBUG
    bindDebugInfo();
#endif
}

VulkanInstance::~VulkanInstance()
{
    
}

void VulkanInstance::bindDebugInfo()
{
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugCallback;

    if (CreateDebugReportCallbackEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug callback!");
    }
}

std::vector<VkExtensionProperties> VulkanInstance::extensionProperties()
{
    uint32_t extensionCount{0};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> properties(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, properties.data());

    for (auto& ext : properties)
    {
        OutputDebugStringA(ext.extensionName);
        OutputDebugStringA("\n");
    }

    return properties;
}

bool VulkanInstance::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : m_validationLayers)
    {
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                return true;
            }
        }
    }

    return false;
}

VkInstance& VulkanInstance::instance()
{
    return m_instance;
}

const VkInstance& VulkanInstance::instance() const
{
    return m_instance;
}

std::vector<const char*>& VulkanInstance::validationLayers()
{
    return m_validationLayers;
}

