#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace PFG
{
	/*	A signed distance 'd' from a point in space 'p' to a plance is 
	*	calculated by a dot product of the plane's normal vector 'n' 
	*	with the vector '(p-q)', where 'q' is any point on the plane.	*/
	float DistanceToPlane(const glm::vec3& n, const glm::vec3& p, const glm::vec3& q);

	/*	A sphere to a plane collision detection is calculated by finiding
	*	the paramenter between the centre of the sphere c0 at time step t0
	*	and predict its movement using 'v'. Then find  position of the centre
	*	of the phere using the parameter 't'. This also return a contact point.	*/
	bool MovingSphereToPlaneCollision(const glm::vec3& n, const glm::vec3& c0, const glm::vec3& c1,
										const glm::vec3& q, const glm::vec3& ci, float r);

	bool MovingSpehereToPlaneCollision2(glm::vec3 n, glm::vec3 c0, glm::vec3 c1, glm::vec3 q,
										glm::vec3& ci, float r);

	/*	Sphere to sphere collision detection is calculated by finding 
	*	the distance between the centre of the sphere c0 and the centre 
	*	of the sphere c1. This function also finds the contact point cp.	*/
	bool SphereToSphereCollision(const glm::vec3& c0, const glm::vec3& c1,
									float r1, float r2, glm::vec3& cp);
}