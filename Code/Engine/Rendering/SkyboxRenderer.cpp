#include "Engine/Rendering/SkyboxRenderer.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Rendering/RenderEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"

#include <Windows.h>
#include <commdlg.h>
#include <ShObjIdl.h>
#include <Engine/Debug/Debug.h>


SkyboxRenderer::SkyboxRenderer()
{   
	//componentType = ComponentType::SKYBOX_RENDERER;
	renderCamera = RenderEngine::GetInstance()->GetCamera();
	
	skyboxFBO = new TexturedCubeMapFBO();
	texture = skyboxFBO->LoadCubemap(faces);

	// Generate and bind VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	float skyboxVertices[] = {
		// positions          
		-100.0f,  100.0f, -100.0f,
		-100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f, -100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,

		-100.0f, -100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f,
		-100.0f, -100.0f,  100.0f,

		-100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f, -100.0f,
		 100.0f,  100.0f,  100.0f,
		 100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f,  100.0f,
		-100.0f,  100.0f, -100.0f,

		-100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f, -100.0f,
		 100.0f, -100.0f, -100.0f,
		-100.0f, -100.0f,  100.0f,
		 100.0f, -100.0f,  100.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	
	// Set attributes: position(0), normal(1) and texture coords(2)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (GLvoid*)0);

	
	// Unbind Buffers and vertexArray
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void SkyboxRenderer::Update()
{
	//setupShaderForDraw();
	//draw();
}

void SkyboxRenderer::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void SkyboxRenderer::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

void SkyboxRenderer::setRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
}

void SkyboxRenderer::setProgram(GLuint _program)
{
	this->program = _program;
	shader = new Shader(_program);
}

void SkyboxRenderer::setShaderProgram(GLuint _program)
{
	this->program = _program;
	shader = new Shader(program);
}

void SkyboxRenderer::setTexture(GLuint textureID)
{
	texture = textureID;
}



void SkyboxRenderer::OnComponentAdded()
{
	gameObject->SetRenderComponent((RenderableComponent*)this);
}

void SkyboxRenderer::DrawShadows()
{
}

void SkyboxRenderer::DrawCubemapShadows()
{
}

void SkyboxRenderer::DrawPickingColor()
{
}

void SkyboxRenderer::setupShaderForDraw(Camera* camera)
{
	glDepthMask(GL_FALSE);
	// Get position and scale from Transform component
	//setPosition(gameObject->transform->position);
	setPosition(camera->getCameraPosition());
	setScale(gameObject->transform->scale);
	setRotation(gameObject->transform->rotation);

	// Set the model matrix
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	glm::mat4 rotateMatrix = glm::rotate(translationMatrix, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrix = glm::rotate(rotateMatrix, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = translationMatrix * rotateMatrix * scaleMatrix;

	glm::vec3 FragPos = glm::vec3(modelMatrix * glm::vec4(position, 1.0f));

	glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();

	glUseProgram(shader->ID);

	shader->setMat4("vp", vp);
	shader->setMat4("model", modelMatrix);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	shader->setInt("skyboxTexture", 3);
}

void SkyboxRenderer::draw()
{
	glDepthFunc(GL_LEQUAL);
	// Bind the VAO and draw object
	glBindVertexArray(vao);
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Unbind VertexArray for safety
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

void SkyboxRenderer::draw(Camera* camera)
{
	setupShaderForDraw(camera);
	draw();
}

void SkyboxRenderer::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text("SKYBOX");

	if (ImGui::Button("Select Skybox folder"))
	{
		std::string selectedFolder = OpenFolderDialog();
		if (!selectedFolder.empty())
		{
			// Do something with the selected folder path
			Debug::Log("Skybox folder selected " + selectedFolder);
			reloadTexture(selectedFolder);
		}
	}

}

std::string SkyboxRenderer::OpenFolderDialog()
{
	std::string folderPath = "";
	IFileDialog* pFileDialog = nullptr;

	// Create the FileOpenDialog object.
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	if (SUCCEEDED(hr))
	{
		// Set the options on the dialog.
		DWORD dwOptions;
		hr = pFileDialog->GetOptions(&dwOptions);
		if (SUCCEEDED(hr))
		{
			pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

			// Show the dialog
			hr = pFileDialog->Show(nullptr);
			if (SUCCEEDED(hr))
			{
				// Get the folder name
				IShellItem* pItem;
				hr = pFileDialog->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath = nullptr;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						// Convert from wide string to standard string
						char buffer[MAX_PATH];
						wcstombs(buffer, pszFilePath, MAX_PATH);
						folderPath = buffer;

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
		}
		pFileDialog->Release();
	}

	return folderPath;
}

void SkyboxRenderer::reloadTexture(std::string newPath)
{
	faces[0] = newPath + "/right.jpg";
	faces[1] = newPath + "/left.jpg";
	faces[2] = newPath + "/bottom.jpg";
	faces[3] = newPath + "/top.jpg";
	faces[4] = newPath + "/front.jpg";
	faces[5] = newPath + "/back.jpg";

	texture = skyboxFBO->LoadCubemap(faces);
}
