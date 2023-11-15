#pragma once
#include<memory>
#include<vector>
#include<vulkan/vulkan.h>

namespace thunderdemo
{
	class VulkanInstance;
	class VulkanDevice;

	class context
	{
	public:
		static void init();
		static void quit();
		static std::shared_ptr<context> GetInstance() { return _instance; }

		void create();

	private:
		static std::shared_ptr<context> _instance;
		std::shared_ptr<VulkanInstance> _vulkanInstance;
		std::shared_ptr<VulkanDevice> _vulkanDevice;
	};

	class VulkanInstance {
	public:
		VulkanInstance() {
			createInstance();
		}

		~VulkanInstance() {
			vkDestroyInstance(instance, nullptr);
		}

		VkInstance getInstance() const {
			return instance;
		}

	private:
		void createInstance();
		bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);

		VkInstance instance;
	};

	class VulkanDevice {
	public:
		VulkanDevice(VkInstance instance) : vkInstance(instance) {
			pickPhysicalDevice();
			createLogicalDevice();
		}

		~VulkanDevice() {
			vkDestroyDevice(device, nullptr);
		}

		VkDevice getDevice() const {
			return device;
		}

		VkPhysicalDevice getPhysicalDevice() const {
			return physicalDevice;
		}

	private:
		void pickPhysicalDevice();
		void createLogicalDevice();

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		void findQueueFamilies(VkPhysicalDevice device);

		VkInstance vkInstance;
		VkDevice device;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		int graphicsQueueFamily = -1;
		int presentQueueFamily = -1;
	};

}

