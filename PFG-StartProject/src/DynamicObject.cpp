#include <GLM/gtc/matrix_transform.hpp>
#include "DynamicObject.h"

DynamicObject::DynamicObject()
{
	_position = { 2.0f, 5.0f, -2.0f };
	_scale = { 0.3f, 0.3f, 0.3f };
	_velocity = { 0.0f };
	_force = { 0.0f };
	_mass = 1.0f;
	_start = false;
	_bRadius = 1.0f;
	UpdateModelMatrix();
}

DynamicObject::~DynamicObject()
{

}

void DynamicObject::Update(float deltaTs)
{
	UpdateModelMatrix();
}

void DynamicObject::Euler(float deltaTs)
{

}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
	_modelMatrix = glm::scale(_modelMatrix, _scale);
}