#include "transformation.h"

namespace Transformation
{
	/*
	Précisions concernant GLM :

	glm::mat4 m4( 1.0f ); // construct identity matrix
    m4[ 0 ]     // column zero
    m4[ 0 ].x   // same as m4[ 0 ][ 0 ]
    m4[ 0 ].y   // same as m4[ 0 ][ 1 ]
    m4[ 0 ].z   // same as m4[ 0 ][ 2 ]
    m4[ 0 ].w   // same as m4[ 0 ][ 3 ]

	Each column of matrix glm::mat4 is a vec4.
	*/

	glm::mat4 translate(const glm::vec3 &t)
	{
		return glm::transpose( glm::mat4(1, 0, 0, t.x,
										0, 1, 0, t.y,
										0, 0, 1, t.z,
										0, 0, 0, 1) );
	}
   
	glm::mat4 rotate(GLfloat angle, glm::vec3 axis)
	{
		GLfloat angleRadian = angle * M_PI / 180.0f;

		if(axis == glm::vec3(1, 0, 0))
			return glm::transpose( glm::mat4(1, 0, 0, 0,
											0, cos(angleRadian), -sin(angleRadian), 0,
											0, sin(angleRadian), cos(angleRadian), 0,
											0, 0, 0, 1) );
		else if(axis == glm::vec3(0, 1, 0))
			return glm::transpose( glm::mat4(cos(angleRadian), 0, sin(angleRadian), 0,
											0, 1, 0, 0,
											-sin(angleRadian), 0, cos(angleRadian), 0,
											0, 0, 0, 1) );
		else if(axis == glm::vec3(0, 0, 1))
			return glm::transpose( glm::mat4(cos(angleRadian), -sin(angleRadian), 0, 0,
											sin(angleRadian), cos(angleRadian), 0, 0,
											0, 0, 1, 0,
											0, 0, 0, 1) );
		else return glm::mat4(1);
	}

	glm::mat4 scale(glm::vec3 s)
	{
		return glm::mat4(s.x, 0, 0, 0,
						0, s.y, 0, 0,
						0, 0, s.z, 0,
						0, 0, 0, 1);
	}
}