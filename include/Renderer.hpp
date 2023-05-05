#pragma once
#include "Subsystem.hpp"
#include <glm/glm.hpp>

class GLFWwindow;

namespace RendererPBR
{
	class Mesh;
	class Shader;
	class Texture;
	class Light;
	class Camera;

	class Renderer : public Subsystem
	{
	public:
		virtual bool Init(const ApplicationSettings& settings)override;
		virtual void Shutdown()override;
		void Clear(glm::vec3 clearColor);
		void Clear();
		void Present();

		void SetWireFrame(bool wireframe);


		/* Internal Rendering Function
		* User is supposed to take care of the uniforms 
		* outside of this function. 
		*/
		void Render(Mesh* mesh, Shader* shader);

		void Render( Mesh* mesh, Shader* shader, Light* light, Camera* camera, 
			Texture* diffuse, Texture* specular, Texture* normalMap );

		void Render( Mesh* mesh, Shader* shader, Light* light, Camera* camera, 
			Texture* albedoMap, Texture* metallicMap, Texture* normalMap, Texture* roughnessMap, Texture* aoMap );
		
	private:
		GLFWwindow* m_Window = nullptr;
	};
}