#include <GLM/gtc/matrix_transform.hpp>
#include "DynamicObject.h"
#include <iostream>

DynamicObject::DynamicObject()
{
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_scale = glm::vec3(0.5f, 0.5f, 0.5f);
	_vFinish = glm::vec3(0.0f, 0.0f, 0.0f);
	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 2.0f;
	_start = false;
	_active = false;
	_bRadius = 0.4999f;
	_stringLength = 2.0f;
	UpdateModelMatrix();

	_frequency = 2 * 3.14159 * sqrt(_stringLength / 9.8);
	std::cout << _frequency << "\n";
}

DynamicObject::~DynamicObject()
{
	
}

void DynamicObject::Update(float deltaTs, bool dir)
{
	glm::vec3 f;

	if (_start == true)
	{
		if (_active == true)
		{
			ClearForces();
			if (dir == true)
			{
				f = glm::vec3(10.0f, 0.0f, 0.0f);
			}
			else if (dir == false)
			{
				f = glm::vec3(-10.0f, 0.0f, 0.0f);
			}
			
			//glm::vec3 f = glm::vec3(0.0f, -9.8f * _mass, 0.0f);
			AddForce(f);
			//StringTransform(deltaTs);
			CollisionResponses(deltaTs);
			Euler(deltaTs);
			UpdateModelMatrix();
		}
		else
		{
			//pos = start pos
			_position = _startPos;
		}
	}
	else
	{
		UpdateModelMatrix();
	}
}

void DynamicObject::Euler(float deltaTs)
{
	_vFinish = _vFinish + (_force / _mass) * deltaTs;
	_position += _vFinish * deltaTs;
}

//NOT USED
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
		//ClearForces();
	}
}

void DynamicObject::StringTransform(float deltaTs)
{
	if (_position.y > 2.0f)
	{
		glm::vec3 f = glm::vec3(0.0f, (-9.8f * _mass), 0.0f);
		AddForce(f);
	}
	if (_position.y <= 2.0f)
	{
		ClearForces();
		_position.y = 2.0f;
	}
}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}

void DynamicObject::HorizontalPositionCalc()
{
	int a = 1;
	int q = 0;
	_position.y = a / _position.x + q;
}