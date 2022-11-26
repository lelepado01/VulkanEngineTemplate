//
//  EnginePipeline.h
//  VulkanEngine
//
//  Created by Gabriele Padovani on 14/01/22.
//

#ifndef EnginePipeline_h
#define EnginePipeline_h

#include <stdio.h>
#include <string>
#include <vector>
#include <ios>
#include <fstream>
#include <stdexcept>
#include <cassert>

#include "../EngineDevice.h"
#include "../EngineModel.h"
#include "../settings/EngineSettings.h"
#include "../vertex/EngineVertex.h"

struct PipelineConfigInfo {
    PipelineConfigInfo(const PipelineConfigInfo&) = delete;
    PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
    
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo; 
    
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;    
};

class EnginePipeline {
public:
    EngineDevice& device;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    
public:
    void createGraphicsPipeline(const std::string& vertSource, const std::string& fragSource, const PipelineConfigInfo& configInfo);    
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    
    static std::vector<char> readFile(const std::string& filename);
    
public:
    EnginePipeline(EngineDevice& device,
                   const std::string& vertSource,
                   const std::string& fragSource,
                   const PipelineConfigInfo& configInfo);
    ~EnginePipeline();
    
    EnginePipeline(const EnginePipeline&) = delete;
    EnginePipeline& operator=(const EnginePipeline& ) = delete;
    
    void Bind(VkCommandBuffer commandBuffer); 
    
    static void GetPipelineConfigInfo(PipelineConfigInfo& configInfo);
};

#endif /* EnginePipeline_h */
