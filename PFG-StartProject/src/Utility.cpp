#include "Utility.h"
#include <iostream>
namespace PFG
{
	/*	Variables:
	*	d - distance
	*	n - norm vector
	*	q - point on the plane
	*	p - point in 3D		*/
	float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q)
	{
		float d = glm::dot((p - q), n);
		return d;
	}

	/*	c0 - the centre of the sphere at the current time step
	*	c1 - the centre of the sphere at the end of the time step
	*	ci - the centre of the sphere when it collides with the plane
	*	r - radius of the sphere	*/
	bool MovingSpehereToPlaneCollision2(glm::vec3 n, glm::vec3 c0, glm::vec3 c1, glm::vec3 q,
		glm::vec3& ci, float r)
	{
		float t;

		float d0 = DistanceToPlane(n, c0, q);
		float d1 = DistanceToPlane(n, c1, q);

		//Sphere is already overlapping with the plane
		if (glm::abs(d0) <= r)
		{
			//Set the time of collision to zero and
			//set the position of the contact phere as the phere position
			ci = c0;
			t = 0.0f;
			return true;
		}
		if (d0 > r && d1 < r)
		{
			//Normalised time
			t = (d0 - r) / (d0 - d1);

			ci = (1 - t) * c0 + t * c1;
			return true;
		}

		//If sphere is moving away from and parallel to plane
		return false;
	}

	/*	c0 - the centre of the first sphere
	*	c1 - the centre of the second sphere
	*	r1 - the radius of the first sphere
	*	r2 - the radius of the second sphere
	*	ci - contact point when spheres collide		*/
	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1,
		float r1, float r2, glm::vec3& cp)
	{
		float d = glm::length(c1 - c0);
		glm::vec3 n;

		if (d <= (r1 + r2))
		{
			n = glm::normalize(c0 - c1 + 0.01f);
			cp = r1 * n;
			return true;
		}

		return false;
	}
}