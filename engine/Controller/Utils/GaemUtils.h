#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gaemutils {

	static void PrintVec3(std::string name, glm::vec3 vec) {
		std::cout << name << ": " << vec.x << " | " << vec.y << " | " << vec.z << "\n";
	}


	static void PrintMat3(glm::mat3 mat) {

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				std::cout << std::fixed << std::setprecision(4) << mat[i][j] << " | ";
			}
			std::cout << "\n";
		}
	}

}
