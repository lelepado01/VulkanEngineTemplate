//
//  EngineSettings.h
//  VulkanEngine
//
//  Created by Gabriele Padovani on 29/01/22.
//

#ifndef EngineSettings_h
#define EngineSettings_h

#include <glm/glm.hpp>

struct Light {
    alignas(16) glm::vec3 lightPosition;
    alignas(16) glm::vec4 ambientColor;
    alignas(16) glm::vec4 lightColor;
};

class EngineSettings {
private:
    
public:
    inline static bool DrawWireframe = false;
    inline static float CameraSpeed = 4.0f;
    
    inline static Light LightParams {
        glm::vec3(0.0f, 11.0f, 0.0f),
        glm::vec4(0.5f, 0.5f, 1.0f, 0.5f),
        glm::vec4(0.5f, 0.5f, 1.0f, 1.0f),
    };
};

#endif /* EngineSettings_h */
