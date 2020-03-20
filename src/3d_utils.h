#pragma once

#include "pch.h"

namespace Orbiter {
	
	struct Vertex{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec2 texture_coords;
	};

	struct Mesh {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
	};
	
	struct Polyline{
			std::vector<Vertex> vertices;
	};
	
	namespace Utils {	
		
		glm::vec3 polar_to_euclidian(double u, double v, double radius);		
		void printMeshData(Mesh& mesh);		
		void computeNormals(Mesh& mesh);
		void translateMesh(Mesh& mesh,  glm::vec3 translation);
		void rotateMesh(Mesh& mesh,  float angle, glm::vec3 axis);
		void scaleMesh(Mesh& mesh, glm::vec3 scale_vec);
		
		Mesh mergeMesh(Mesh& mesh1, Mesh& mesh2);
		Mesh makeQuadSphere(unsigned int num = 3);
		Mesh makeSimpleSphere(unsigned int rows = 10, unsigned int cols = 10, float radius = 0.5f);
		Mesh makeGrid(int rows, int cols);
		Mesh makeSimpleQuad(float width = 1.0f, float height = 1.0f);
	}
}

