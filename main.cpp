
#include "engine/Engine.h"
#include "engine/camera/EngineCamera.h"

#include "engine/EngineModel.h"

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

		// ImGui::Text("Hello, world %d", 123);  
		// if (ImGui::Button("Save"))  
		// {  
		// 	// do stuff  
		// }  
		// ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));  
		// ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        
        float frameTime = EngineTime::DeltaTime();

        camera.Update(frameTime);

        engine.Draw(camera);
    }
    
    engine.Quit();

    return EXIT_SUCCESS;
}