#include <GLM/gtc/matrix_transform.hpp>
#include "DynamicObject.h"
#include <iostream>

DynamicObject::DynamicObject()
{
	_position = glm::vec3(0.0f, 0.0f, 0.0f);
	_scale = glm::vec3(0.5f, 0.5f, 0.5f);
	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 2.0f;
	_start = false;
	_active = false;
	_bRadius = 0.499999f;
	UpdateModelMatrix();
}

DynamicObject::~DynamicObject()
{
	
}

void DynamicObject::Update(float deltaTs, bool dir, const glm::vec3& c0, const glm::vec3& c1)
{
	glm::vec3 force;

	if (_start == true)
	{
		if (_active == true)
		{
			ClearForces();
			if (dir == true)
			{
				force = glm::vec3(10.0f, 0.0f, 0.0f);
			}
			else if (dir == false)
			{
				force = glm::vec3(-10.0f, 0.0f, 0.0f);
			}
			
			AddForce(force);
			RungeKutta2(deltaTs);
			UpdateModelMatrix();
		}
		else
		{
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
	_velocity = _velocity + (_force / _mass) * deltaTs;
	_position += _velocity * deltaTs;
} 

void DynamicObject::RungeKutta2(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k[2];

	//Evaluate once at t0
	force = _force;
	acceleration = force / _mass;
	k[0] = deltaTs * acceleration;

	//Evaluate once at t0 + deltaTs/2.0 using half of k0
	force = _force + k[0] / 2.0f;
	acceleration = force / _mass;
	k[1] = deltaTs * acceleration;

	//Evalueate once at t0 + deltaTs using k1
	_velocity += k[1];
	_position += _velocity * deltaTs;
}

void DynamicObject::RungeKutta4(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k[4];

	//Evaluate once at t0
	force = _force;
	acceleration = force / _mass;
	k[0] = deltaTs * acceleration;

	//Evaluate once at t0 + deltaTs/2.0 using half of k0
	force = _force + k[0] / 2.0f;
	acceleration = force / _mass;
	k[1] = deltaTs * acceleration;

	force = _force + k[1] / 2.0f;
	acceleration = force / _mass;
	k[2] = deltaTs * acceleration;

	//Evaluate once at t0 + deltaTs using k2
	force = _force + k[2];
	acceleration = force / _mass;
	k[3] = deltaTs * acceleration;

	//evaluate at t0 + deltaTs using weighted sum of k0, k1, k2 and k3
	_velocity += (k[0] + 2.0f * k[1] + 2.0f * k[2] + k[3]) / 6.0f;
	_position += _velocity * deltaTs;
}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}