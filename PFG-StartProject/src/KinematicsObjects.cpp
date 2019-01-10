#include <GLM/gtc/matrix_transform.hpp>
#include "KinematicsObject.h"

KinematicsObject::KinematicsObject()
{
	_position = glm::vec3(-5.0f, 5.0f, -5.0f);
	_scale = glm::vec3(0.3f, 0.3f, 0.3f);
	_vInitial = glm::vec3(0.0f, 0.0f, 0.0f) ;
	_acceleration = 9.8f;
	UpdateModelMatrix();
}

KinematicsObject::~KinematicsObject()
{
}

void KinematicsObject::Update(float deltaTs)
{
	if (_start == true)
	{
		_velocity.y = _vInitial.y + _acceleration * deltaTs;
		_distance.y = ((_vInitial.y + _velocity.y) / 2) * deltaTs;

		_position.y -= _distance.y;

		_vInitial = _velocity;

		UpdateModelMatrix();
	}
	//Simple Collision Detection
	if (_position.y <=  0.35f)
	{
		_vInitial.y = -_vInitial.y * 3.0f / 4.0f;
	}
}

void KinematicsObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}

