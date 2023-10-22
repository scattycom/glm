#define GLFW_INCLUDE_VULKAN
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class demo
{
public:
	demo();
	~demo();

	void initInstance();
	void choosePhysicalDevice(VkInstance instance);
	void createLogicalDevice(VkPhysicalDevice physicalDevice);
	void createSurface(VkInstance instance, GLFWwindow* window);
	void createSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

private:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapchain;
};

demo::demo()
{
	initInstance();
	choosePhysicalDevice(instance);
	createLogicalDevice(physicalDevice);

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(800, 600, "Vulkan Window", nullptr, nullptr);

	createSurface(instance, window);
	createSwapchain(device, physicalDevice, surface);

}

demo::~demo()
{

	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);

}

void demo::initInstance()
{
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
}

void demo::choosePhysicalDevice(VkInstance instance) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		// 未找到支持Vulkan的物理设备
		return;
	}

	VkPhysicalDevice* devices = new VkPhysicalDevice[deviceCount];
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

	// 在这里，我们简单地选择第一个物理设备。
	// 在实际应用中，你可能需要根据特定需求来选择一个物理设备。
	physicalDevice = devices[0];

	delete[] devices;
}

void demo::createLogicalDevice(VkPhysicalDevice physicalDevice) {
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = 0; // 在这里，我们假设第0个队列族可用
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	// 如果需要指定使用的扩展，也可以在createInfo中设置
	// createInfo.enabledExtensionCount = ...
	// createInfo.ppEnabledExtensionNames = ...


	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		// 创建逻辑设备失败
		return ;
	}
}

void demo::createSurface(VkInstance instance, GLFWwindow* window) {

	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		return ;
	}
}

void demo::createSwapchain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
	// 获取交换链支持的详细信息
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	// 在这里，我们简化了交换链的配置。在真实应用中，你需要根据需要进行更多的配置。
	createInfo.minImageCount = capabilities.minImageCount;
	createInfo.imageFormat = VK_FORMAT_B8G8R8A8_SRGB;  // 假设这个格式可用
	createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	createInfo.imageExtent = capabilities.currentExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;  // 通常总是可用的
	createInfo.clipped = VK_TRUE;

	VkSwapchainKHR swapchain;
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapchain) != VK_SUCCESS) {
		return;
	}
}

std::vector<VkImage> getSwapchainImages(VkDevice device, VkSwapchainKHR swapchain) {
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);

	std::vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.data());

	return images;
}

VkImage createDepthBuffer(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height) {
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = VK_FORMAT_D16_UNORM;  // 假设这个格式可用
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage depthImage;
	if (vkCreateImage(device, &imageInfo, nullptr, &depthImage) != VK_SUCCESS) {
		return VK_NULL_HANDLE;
	}

	// 注意：实际应用中还需要分配和绑定显存

	return depthImage;
}

VkFramebuffer createFramebuffer(VkDevice device, VkRenderPass renderPass, VkImageView colorImageView, VkImageView depthImageView, uint32_t width, uint32_t height) {
	VkImageView attachments[] = {
		colorImageView,
		depthImageView
	};

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = renderPass;
	framebufferInfo.attachmentCount = 2;
	framebufferInfo.pAttachments = attachments;
	framebufferInfo.width = width;
	framebufferInfo.height = height;
	framebufferInfo.layers = 1;

	VkFramebuffer framebuffer;
	if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS) {
		return VK_NULL_HANDLE;
	}

	return framebuffer;
}



int main()
{
	demo de;
}

