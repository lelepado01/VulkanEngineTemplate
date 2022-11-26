
#include "engine/Engine.h"
#include "engine/camera/EngineCamera.h"

#include "engine/EngineModel.h"

#include "imgui/imgui.h"

int main() {
	
    Engine engine;
    EngineCamera camera{glm::vec3(-15.0f, 0.0f, 0.0f), engine.GetWindowAspectRatio()};

	std::shared_ptr<EngineModel> model = engine.LoadModelFromFile("models/cube.obj"); 
	EngineGameObject gameObject = EngineGameObject::createGameObject(); 
	gameObject.model = model; 
	gameObject.tag = GameObjectTag::SIMPLE; 
	gameObject.transform = TransformComponent(); 
	gameObject.transform.translation = glm::vec3(0,0,0); 
	gameObject.transform.scale = glm::vec3(10,10,10); 

	engine.AddGameObject(gameObject); 

    while (engine.IsRunning()) {
        engine.Update();

        float frameTime = EngineTime::DeltaTime();

        camera.Update(frameTime);

		ImGui::Text("Hello, world %d", 123);
		if (ImGui::Button("Save")){}

        engine.Draw(camera);
    }
    
    engine.Quit();

    return EXIT_SUCCESS;
}