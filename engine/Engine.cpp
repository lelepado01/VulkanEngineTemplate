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
        .setMaxSets(EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
        .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, EngineSwapChain::MAX_FRAMES_IN_FLIGHT)
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
        
        renderer.EndSwapChainRenderPass(commandBuffer);
        renderer.EndFrame();
    }
}

void Engine::Quit(){
    vkDeviceWaitIdle(device.device());
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