

#include "EnginePipeline.h"

class TesselationShaderPipeline : public EnginePipeline {
private: 
	VkShaderModule tessControlShaderModule;
    VkShaderModule tessEvalShaderModule;

public:
	TesselationShaderPipeline(EngineDevice& device,
                   const std::string& vertSource,
                   const std::string& tessControlSource,
                   const std::string& tessEvalSource,
                   const std::string& fragSource,
                   const PipelineConfigInfo& configInfo);
	
	~TesselationShaderPipeline(); 


    void createGraphicsPipeline(const std::string &vertSource,
                                const std::string &tessControlSource,
                                const std::string &tessEvalSource,
                                const std::string &fragSource,
                                const PipelineConfigInfo& configInfo);

	static void GetPipelineConfigInfo(PipelineConfigInfo& configInfo);

};
