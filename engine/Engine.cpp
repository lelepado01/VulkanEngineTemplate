//
//  Engine.cpp
//  VulkanEngine
//
//  Created by Gabriele Padovani on 14/01/22.
//

#include "Engine.h"

Engine::Engine(){    
    EngineInput::SetWindow(window.GetGLFWwindow());
    
    globalPool = EngineDescriptorPool::Builder(device)
        .setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT*2)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT*2)
        .build();
    
    for (int i = 0; i < globalUniformBuffers.size(); i++) {
        globalUniformBuffers[i] = std::make_unique<EngineBuffer>(device,
                                                                 sizeof(GlobalUniformBuffer),
                                                                 1,
                                                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        globalUniformBuffers[i]->map();
    }
    
    auto globalSetLayout = EngineDescriptorSetLayout::Builder(device)
        .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL)
        .build();
    
    for (int i = 0; i < globalDescriptorSets.size(); i++) {
        auto bufferInfo = globalUniformBuffers[i]->descriptorInfo();

        EngineDescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }
    
    simpleRenderSystem = std::make_unique<SimpleRenderSystem>(device, renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

	initImGui();

}

Engine::~Engine(){}

void Engine::Update(){
    glfwPollEvents();
    
    EngineInput::Update();
    EngineTime::Update();
}

void Engine::Draw(EngineCamera& camera){
    
    if (auto commandBuffer = renderer.BeginFrame()){
        int frameIndex = renderer.GetFrameIndex();

        GlobalUniformBuffer ubo{};
		ubo.projectionMatrix  = camera.GetProjectionMatrix();  
		ubo.viewMatrix  = camera.GetViewMatrix();  
        ubo.light = EngineSettings::LightParams;
        
		globalUniformBuffers[frameIndex]->writeToBuffer(&ubo);
		globalUniformBuffers[frameIndex]->flush();
        
        renderer.BeginSwapChainRenderPass(commandBuffer);
        
        FrameInfo frameInfo{
            frameIndex,
            EngineTime::DeltaTime(),
            commandBuffer,
            camera,
            globalDescriptorSets[frameIndex],
            GetGameObjects(GameObjectTag::SIMPLE)};
        simpleRenderSystem->RenderGameObjects(frameInfo);

		imguiRender(commandBuffer);
        
        renderer.EndSwapChainRenderPass(commandBuffer);
        renderer.EndFrame();
    }
}

void Engine::Quit(){
    vkDeviceWaitIdle(device.device());

	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Engine::AddGameObject(EngineGameObject &gameObject){
    gameObjects[gameObject.tag].emplace(gameObject.getId(), std::move(gameObject));
}

EngineGameObject::Map& Engine::GetGameObjects(GameObjectTag searchedTag){
    return gameObjects[searchedTag];
};

std::unique_ptr<EngineModel> Engine::LoadModelFromFile(const std::string &path){
    return EngineModel::createModelFromFile(device, path);
}

void Engine::initImGui(){
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
	// io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
	
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	
	ImGui_ImplGlfw_InitForVulkan(window.GetGLFWwindow(), true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = device.getInstance();  
	init_info.PhysicalDevice = device.phDevice();
	init_info.Device = device.device();
	init_info.Queue = device.graphicsQueue();
	init_info.DescriptorPool = globalPool->getDescriptorPool();
	init_info.MinImageCount = EngineSwapChain::MAX_FRAMES_IN_FLIGHT;
	init_info.ImageCount = renderer.GetSwapChainImageCount();

	ImGui_ImplVulkan_Init(&init_info, renderer.GetSwapChainRenderPass());
	
	VkCommandBuffer commandBuffer = device.beginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	device.endSingleTimeCommands(commandBuffer);
	
	ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void Engine::imguiRender(VkCommandBuffer commandBuffer){
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::ShowDemoWindow();
	
	ImGui::Render();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
}