//
//  EngineCamera.h
//  VulkanEngine
//
//  Created by Gabriele Padovani on 18/01/22.
//

#ifndef EngineCamera_h
#define EngineCamera_h

#include <stdio.h>
#include <cassert>
#include <limits>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "../input/EngineInput.h"
#include "../settings/EngineSettings.h"
#include "Frustum.h"

class EngineCamera {
private:
    const float movementSpeed = EngineSettings::CameraSpeed;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 direction;
    
    float pitch;
    float yaw;
    float aspectRatio;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    
	Frustum cameraFrustum; 

public:
    
private:
    void recalculateCameraView();
    void recalculateCameraDirection();
    void updatePosition(float deltaTime);
    void updateDirection();

public:
    EngineCamera(glm::vec3 initialPosition, float aspectRatio);
    
    const glm::mat4& GetProjectionMatrix() const { return projectionMatrix; };
    const glm::mat4& GetViewMatrix() const { return viewMatrix; };
    const glm::vec3& GetPosition() const { return position; };
    const glm::vec3& GetDirection() const { return direction; };

    void Update(float deltaTime); 
    void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up);

	bool BoxIsVisible(glm::vec3 minVertex, glm::vec3 maxVertex) const; 

};

#endif /* EngineCamera_h */
