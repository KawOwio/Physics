#include <GLM/gtc/matrix_transform.hpp>
#include "DynamicObject.h"
#include <iostream>

DynamicObject::DynamicObject()
{
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_scale = glm::vec3(0.5f, 0.5f, 0.5f);
	_vInitial = glm::vec3(0.0f, 0.0f, 0.0f);
	_vFinish = glm::vec3(0.0f, 0.0f, 0.0f);
	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_displacement = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 2.0f;
	_start = false;
	_active = false;
	_bRadius = 1.0f;
	_stringLength = 2.0f;
	UpdateModelMatrix();
}

DynamicObject::~DynamicObject()
{
	
}

void DynamicObject::Update(float deltaTs)
{
	if (_start == true)
	{
		if (_active == true)
		{
			ClearForces();
			glm::vec3 f = glm::vec3(1.0f, 0.0f, 0.0f);
			//glm::vec3 f = glm::vec3(0.0f, -9.8f * _mass, 0.0f);
			AddForce(f);
			//StringTransform(deltaTs);
			CollisionResponses(deltaTs);
			Euler(deltaTs);
			UpdateModelMatrix();
		}
	}
	else
	{
		UpdateModelMatrix();
	}
}

void DynamicObject::Euler(float deltaTs)
{
	_vFinish = _vInitial + (_force / _mass) * deltaTs;
	_position  += _vFinish * deltaTs;

	_vInitial = _vFinish;
}

void DynamicObject::CollisionResponses(float deltaTs)
{
	//Set plane parametrs
	glm::vec3 n(0.0f, 1.0f, 0.0f);	//the normal of the plane pointing up
	glm::vec3 q(0.0f, 0.0f, 0.0f);

	float d = PFG::DistanceToPlane(n, _position, q);

	glm::vec3 contactPosition;
	glm::vec3 c1 = _position + _vFinish * deltaTs;
	glm::vec3 c0 = _position;
	bool collision = PFG::MovingSpehereToPlaneCollision2(n, c0, c1, q, contactPosition, _bRadius);
	
	if (collision)
	{
		std::cout << "*";
		ClearForces();
	}
}

void DynamicObject::StringTransform(float deltaTs)
{
	if (_position.y > 2.0f)
	{

	}
	if (_position.y <= 2.0f)
	{
		glm::vec3 f = glm::vec3(0.0f, -(-9.8f * _mass), 0.0f);
		AddForce(f);
	}
}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}