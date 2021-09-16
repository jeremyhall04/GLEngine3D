#ifndef MATH_FUNCS_H
#define MATH_FUNCS_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"


#define PI glm::pi<float>()
#define TAU 2.0f * PI

inline glm::vec3 operator*(const glm::mat4& matrix, const glm::vec3& vec3)
{
	return glm::vec3(
		matrix[0].x * vec3.x + matrix[1].x * vec3.y + matrix[2].x * vec3.z + matrix[3].x,
		matrix[0].y * vec3.x + matrix[1].y * vec3.y + matrix[2].y * vec3.z + matrix[3].y,
		matrix[0].z * vec3.x + matrix[1].z * vec3.y + matrix[2].z * vec3.z + matrix[3].z
	);
}

inline glm::vec4 operator*(const glm::mat4& matrix, const glm::vec4& vec3)
{
	return glm::vec4(
		matrix[0].x * vec3.x + matrix[1].x * vec3.y + matrix[2].x * vec3.z + matrix[3].x * vec3.w,
		matrix[0].y * vec3.x + matrix[1].y * vec3.y + matrix[2].y * vec3.z + matrix[3].y * vec3.w,
		matrix[0].z * vec3.x + matrix[1].z * vec3.y + matrix[2].z * vec3.z + matrix[3].z * vec3.w,
		matrix[0].w * vec3.x + matrix[1].w * vec3.y + matrix[2].w * vec3.z + matrix[3].w * vec3.w
	);
}


#endif // !MATH_FUNCS_H
