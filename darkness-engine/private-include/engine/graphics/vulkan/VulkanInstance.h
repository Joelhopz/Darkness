#pragma once

#include <vector>
#include "engine/graphics/vulkan/VulkanHeaders.h"

VkResult CreateDebugReportCallbackEXT(
    VkInstance instance, 
    const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, 
    const VkAllocationCallbacks* pAllocator, 
    VkDebugReportCallbackEXT* pCallback);

void DestroyDebugReportCallbackEXT(
    VkInstance instance, 
    VkDebugReportCallbackEXT callback, 
    const VkAllocationCallbacks* pAllocator);

class VulkanInstance
{
public:
    VulkanInstance();
    ~VulkanInstance();
    VkInstance& instance();
    const VkInstance& instance() const;
    std::vector<const char*>& validationLayers();
private:
    VkInstance m_instance;
    VkDebugReportCallbackEXT m_callback;

    std::vector<const char*> m_validationLayers;
    std::vector<const char*> m_requiredExtensions;

    std::vector<VkExtensionProperties> extensionProperties();
    
    bool checkValidationLayerSupport();
    void bindDebugInfo();
};
