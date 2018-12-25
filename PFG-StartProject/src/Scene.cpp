#include "Scene.h"

Scene::Scene()
{
	// Set up your scene here......
	// Set a camera
	_camera = new Camera();
	
	// Position of the light, in world-space
	_lightPosition = glm::vec3(10, 10, 0);

	//Add new kinematic and dynamic object here:
	_kinematics = new KinematicsObject();

	//5 balls
	for (int i = 0; i < 5; i++)
	{
		_dynamics[i] = new DynamicObject();
	}

	// Create a game level object
	_level = new GameObject();

	// Create the material for the game object- level
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	modelMaterial->SetDiffuseColour(glm::vec3(0.8, 0.8, 0.8));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("assets/textures/diffuse.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	modelMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_level->SetMaterial(modelMaterial);

	// The mesh is the geometry for the object
	Mesh *groundMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	groundMesh->LoadOBJ("assets/models/woodfloor.obj");
	// Tell the game object to use this mesh
	_level->SetMesh(groundMesh);
	_level->SetPosition(0.0f, 0.0f, 0.0f);
	_level->SetRotation(3.141590f, 0.0f, 0.0f);

	// Create the material for the game object- level
	Material *objectMaterial = new Material();
	// Shaders are now in files
	objectMaterial->LoadShaders("assets/shaders/VertShader.txt", "assets/shaders/FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour
	objectMaterial->SetDiffuseColour(glm::vec3(1, 1, 1));
	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	objectMaterial->SetTexture("assets/textures/metal.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	objectMaterial->SetLightPosition(_lightPosition);
	// Tell the level object to use this material
	_kinematics->SetMaterial(objectMaterial);

	for (int i = 0; i < 5; i++)
	{
		_dynamics[i]->SetMaterial(objectMaterial);
	}

	// Set the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("assets/models/sphere.obj");
	// Tell the game object to use this mesh

	_kinematics->SetMesh(modelMesh);

	_dynamics[0]->SetPosition(glm::vec3(-4.0f, 2.0f, 0.0f));
	_dynamics[1]->SetPosition(glm::vec3(-1.0f, 2.0f, 0.0f));
	_dynamics[2]->SetPosition(glm::vec3(0.0f, 2.0f, 10.0f));
	_dynamics[3]->SetPosition(glm::vec3(1.0f, 2.0f, 10.0f));
	_dynamics[4]->SetPosition(glm::vec3(2.0f, 2.0f, 10.0f));

	for (int i = 0; i < 5; i++)
	{
		_dynamics[i]->SetMesh(modelMesh);
		//_dynamics[i]->Update(deltaTs);
	}
}

Scene::~Scene()
{
	// You should neatly clean everything up here
	//delete _kinematics_object;
	delete _kinematics;
	delete _dynamics;
	delete _level;
	delete _camera;
}

void Scene::Update(float deltaTs, Input* input)
{
	//Start the simulation
	if (input->cmd_x)
	{
		for (int i = 0; i < 2; i++)
		{
			_dynamics[i]->StartSimulation(true);
		}
		_dynamics[0]->SetActive(true);
	}
	
	for (int i = 0; i < 2; i++)
	{
		_dynamics[i]->Update(deltaTs);
	}

	bool collision = PFG::SphereToSphereCollision(_dynamics[0]->GetPosition(), 
		_dynamics[1]->GetPosition(), 0.50f, 0.50f, contactPosition);

	if (collision)
	{
		std::cout << "collision\n";

		glm::vec3 temp = _dynamics[1]->GetPosition();
		temp.x += 0.1f;
		_dynamics[1]->SetPosition(temp);
		_dynamics[1]->SetVelocity(_dynamics[0]->GetVelocity());


		_dynamics[0]->SetActive(false);
		_dynamics[1]->SetActive(true);
		
	}

	_level->Update(deltaTs);
	_camera->Update(input);

	_viewMatrix = _camera->GetView();
	_projMatrix = _camera->GetProj();
														
}

void Scene::Draw()
{
	// Draw objects, giving the camera's position and projection
	_level->Draw(_viewMatrix, _projMatrix);
	_kinematics->Draw(_viewMatrix, _projMatrix);

	for (int i = 0; i < 5; i++)
	{
		_dynamics[i]->Draw(_viewMatrix, _projMatrix);
	}
}


