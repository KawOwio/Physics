#include <GLM/gtc/matrix_transform.hpp>
#include "DynamicObject.h"
#include <iostream>

DynamicObject::DynamicObject()
{
	_position = { 2.0f, 5.0f, -2.0f };
	_scale = { 0.3f, 0.3f, 0.3f };
	_vInitial = { 0.0f };
	_vFinish = { 0.0f };
	_force = { 0.0f };
	_mass = 2.0f;
	_start = false;
	_bRadius = 1.0f;
	_displacement = { 0.0f };
	UpdateModelMatrix();
}

DynamicObject::~DynamicObject()
{
	
}

void DynamicObject::Update(float deltaTs)
{
	if (_start == true)
	{
		ClearForces();
		glm::vec3 f = glm::vec3(0.0f, -9.8f * _mass, 0.0f);
		AddForce(f);

		if (_position.y <= 0.35f)
		{
			glm::vec3 bounceForce = glm::vec3(0.0f, 150.0f, 0.0f);
			AddForce(bounceForce);
			std::cout << "Force: " << _force.y << std::endl;
		}
		Euler(deltaTs);
		UpdateModelMatrix();
	}
}

void DynamicObject::Euler(float deltaTs)
{
	_vFinish = _vInitial + (_force / _mass) * deltaTs;
	_position  += _vFinish * deltaTs;

	_vInitial = _vFinish;
}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}