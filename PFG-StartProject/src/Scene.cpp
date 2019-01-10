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

	//Set starting positions
	_dynamics[0]->SetStartPos(glm::vec3(-2.0f, 2.0f, 0.0f));
	_dynamics[1]->SetStartPos(glm::vec3(-1.0f, 2.0f, 0.0f));
	_dynamics[2]->SetStartPos(glm::vec3(0.0f, 2.0f, 0.0f));
	_dynamics[3]->SetStartPos(glm::vec3(1.0f, 2.0f, 0.0f));
	_dynamics[4]->SetStartPos(glm::vec3(2.0f, 2.0f, 0.0f));

	//Set "end positions"
	//When cradles are gonna change the direction
	_dynamics[0]->SetEndPoint(-3.0f);
	_dynamics[4]->SetEndPoint(3.0f);

	for (int i = 0; i < 5; i++)
	{
		_dynamics[i]->SetMesh(modelMesh);
	}
	_direction = true;
}

Scene::~Scene()
{
	// You should neatly clean everything up here
	delete _kinematics;
	delete _dynamics;
	delete _level;
	delete _camera;
}

void Scene::Update(float deltaTs, Input* input)
{
	bool collision[5] = { false };
	float f = _dynamics[0]->GetBoundingRadius();

	//Start the simulation
	if (input->cmd_x)
	{
		_dynamics[0]->SetPosition(glm::vec3(-3.0f, 2.0f, 0.0f));
		for (int i = 0; i < 5; i++)
		{
			_kinematics->StartSimulation(true);
			_dynamics[i]->StartSimulation(true);
		}
		_dynamics[0]->SetActive(true);
	}
	
	//right
	if (_direction == true)
	{
		for (int i = 0; i < 5; i++)
		{
			//Checking colliison of all cradles
			if (i < 4)
			{
				collision[i] = PFG::SphereToSphereCollision(_dynamics[i]->GetPosition(),
					_dynamics[i + 1]->GetPosition(), f, f, contactPosition);
			}

			if (collision[i])
			{
				if (i < 4)
				{
					//Chaning active cradle to the next one
					_dynamics[i]->SetActive(false);
					_dynamics[i + 1]->SetActive(true);

					//Impulse (passing velocity)
					float impulseFloat = glm::dot((-(1.0f + 0.5f) *
						(_dynamics[i]->GetVelocity() - _dynamics[i + 1]->GetVelocity())), _dynamics[i]->GetContactPos())
						/ ((1 / _dynamics[i]->GetMass()) + (1 / _dynamics[i + 1]->GetMass()));

					glm::vec3 impulseVec = impulseFloat * _dynamics[i]->GetContactPos();
					glm::vec3 newVelocityActive = impulseVec / _dynamics[i + 1]->GetMass();
					glm::vec3 newVelocityRest = -impulseVec / _dynamics[i + 1]->GetMass();
					_dynamics[i]->SetVelocity(newVelocityRest);
					_dynamics[i + 1]->SetVelocity(newVelocityActive);
				}
			}
			
			//If reaches "the end" -> change direction
			glm::vec3 end = _dynamics[i]->GetPosition();
			if (_dynamics[4]->GetActive() == true && end.x >= 3.0f)
			{
				std::cout << "\n**************************************************\n";
				_direction = false;
			}
		}
	}

	if (_direction == false)
	{
		for (int i = 4; i >= 0; i--)
		{
			//Checking colliison of all cradles
			if (i > 0)
			{
				collision[i] = PFG::SphereToSphereCollision(_dynamics[i - 1]->GetPosition(),
					_dynamics[i]->GetPosition(), f, f, contactPosition);
			}

			if (collision[i])
			{
				
				if (i > 0)
				{
					//Chaning active cradle to the next one
					_dynamics[i]->SetActive(false);
					_dynamics[i - 1]->SetActive(true);

					//Impulse
					float impulseFloat = glm::dot((-(1.0f + 0.5f) *
						(_dynamics[i]->GetVelocity() - _dynamics[i - 1]->GetVelocity())), _dynamics[i]->GetContactPos())
						/ ((1 / _dynamics[i]->GetMass()) + (1 / _dynamics[i - 1]->GetMass()));

					glm::vec3 impulseVec = impulseFloat * _dynamics[i]->GetContactPos();
					glm::vec3 newVelocityActive = impulseVec / _dynamics[i - 1]->GetMass();
					glm::vec3 newVelocityRest = -impulseVec / _dynamics[i - 1]->GetMass();
					_dynamics[i]->SetVelocity(newVelocityRest);
					_dynamics[i - 1]->SetVelocity(newVelocityActive);
				}
			}

			//If reaches "the end" -> change direction
			glm::vec3 end = _dynamics[i]->GetPosition();
			if (_dynamics[0]->GetActive() == true && end.x <= -3.0f)
			{
				std::cout << "\n**************************************************\n";
				_direction = true;
			}
		}
	}

	//Update for all cradles
	for (int i = 0; i < 5; i++)
	{
		if (i < 4)
		{
			_dynamics[i]->Update(deltaTs, _direction,
				_dynamics[i]->GetPosition(), _dynamics[i + 1]->GetPosition());
		}
		else
		{
			_dynamics[i]->Update(deltaTs, _direction,
				_dynamics[i]->GetPosition(), _dynamics[i - 1]->GetPosition());
		}
	}

	_kinematics->Update(deltaTs);
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


