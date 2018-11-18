#include <GLM/gtc/matrix_transform.hpp>
#include "KinematicsObject.h"

KinematicsObject::KinematicsObject()
{
	// Don't start simulation yet
	//StartSimulation(false);
	_position = { 0.0f, 5.0f, 0.0f };
	_scale = { 0.3f, 0.3f, 0.3f };
	_vInitial = { 0.0f };
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
		_vFinish.y = _vInitial.y + _acceleration * deltaTs;
		_distance.y = ((_vInitial.y + _vFinish.y) / 2) * deltaTs;

		_position.y -= _distance.y;

		_vInitial = _vFinish;

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

