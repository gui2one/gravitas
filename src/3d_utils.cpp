#include "3d_utils.h"
namespace Orbiter{
namespace Utils{
	
	glm::vec3 
	polar_to_euclidian(double u, double v, double radius)
	{
		glm::vec3 temp;
		
		temp.x = radius * sin(u) * cos(v);
		temp.y = radius * sin(u) * sin(v);
		temp.z = radius * cos(u);
		
		return temp;
	}
	
	void printMeshData(Mesh& mesh)
	{
		printf("num vertices : %d\n", mesh.vertices.size());
		int counter = 0;
		for(auto vtx : mesh.vertices)
		{
			printf("\t%d-x: %.3f, y: %.3f, z: %.3f\n", counter, vtx.position.x, vtx.position.y, vtx.position.z);
			
			counter ++;
		}
		
		printf("num indices : %d\n", mesh.indices.size());
		counter = 0;
		for(auto index : mesh.indices)
		{
			if(counter%3 == 0) printf("\t");
			printf("%d ", index);
			if(counter%3 == 2) printf("\n");
			counter ++;
		}
	}
	
	void computeNormals(Mesh& mesh)
	{
			std::vector<int> checked_indices;
			
			std::vector<int> num_normals(mesh.vertices.size());
			for (size_t i = 0; i < num_normals.size(); i++)
			{
				num_normals[i] = 0;
			}
			
			std::vector<glm::vec3> temp_normals(mesh.vertices.size());
			for (size_t i = 0; i < temp_normals.size(); i++)
			{
				temp_normals[i] = glm::vec3(0.0f,0.0f,0.0f);
			}			
			// compute normals

			for (size_t i = 0; i < mesh.indices.size(); i++)
			{
				glm::vec3 AB;
				glm::vec3 AC;
				
				if((i % 3) == 0){				
					AB = mesh.vertices[mesh.indices[i+1]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i+2]].position - mesh.vertices[mesh.indices[i]].position;					
				}else if((i % 3) == 1){				
					AB = mesh.vertices[mesh.indices[i+1]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i-1]].position - mesh.vertices[mesh.indices[i]].position;							
				}else if((i % 3) == 2){				
					AB = mesh.vertices[mesh.indices[i-2]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i-1]].position - mesh.vertices[mesh.indices[i]].position;							
				}
					
				num_normals[mesh.indices[i]] += 1;
				
				glm::vec3 n_normal = glm::cross(glm::normalize(AB),glm::normalize(AC));
				
				temp_normals[mesh.indices[i]] += n_normal;

			}
			
			for (size_t i = 0; i < mesh.vertices.size(); i++)
			{
			
				mesh.vertices[i].normal = glm::normalize(temp_normals[i]);

			}

	}
	
	void translateMesh(Mesh& mesh, glm::vec3 translation)
	{
		for(auto& vtx : mesh.vertices)
		{
			glm::mat4 matrix = glm::mat4(1.0f);
			matrix = glm::translate(matrix, translation);
			vtx.position = matrix * glm::vec4(vtx.position,1.0f);
		}
	}
	
	void 
	rotateMesh(Mesh& mesh,  float angle, glm::vec3 axis)
	{
		for(auto& vtx : mesh.vertices)
		{
			glm::mat4 matrix = glm::mat4(1.0f);
			matrix = glm::rotate(matrix, glm::radians( angle), axis);
			vtx.position = matrix * glm::vec4(vtx.position,1.0f);
		}
	}
	
	void
	scaleMesh(Mesh& mesh, glm::vec3 scale_vec)
	{
		for(auto& vtx : mesh.vertices)
		{
			vtx.position = vtx.position * scale_vec;
		}
	}
	
	Mesh 
	mergeMesh(Mesh& mesh1, Mesh& mesh2)
	{
		Mesh result_mesh;
		result_mesh.vertices.insert(result_mesh.vertices.end(), mesh1.vertices.begin(), mesh1.vertices.end());
		result_mesh.indices.insert(result_mesh.indices.end(), mesh1.indices.begin(), mesh1.indices.end());
		
		
		result_mesh.vertices.insert(result_mesh.vertices.end(), mesh2.vertices.begin(), mesh2.vertices.end());
		int n_vertices = mesh1.vertices.size();
		for(size_t i = 0; i < mesh2.indices.size(); i++)
		{
			mesh2.indices[i] += n_vertices;
		}
		
		result_mesh.indices.insert(result_mesh.indices.end(), mesh2.indices.begin(), mesh2.indices.end());
		
		return result_mesh;
	}
	
	Mesh 
	makeQuadSphere(unsigned int num)
	{

		Mesh mesh = makeGrid( num, num);	
		translateMesh(mesh, glm::vec3(-0.5f, -0.5f, 0.5f));
		


		for(auto& vtx : mesh.vertices)
		{	
			vtx.position = glm::normalize(vtx.position);			
		}
		
		Mesh bottom_plane = mesh;
		Mesh left_plane = mesh;
		Mesh right_plane = mesh;
		Mesh back_plane = mesh;
		Mesh front_plane = mesh;
		
		rotateMesh(bottom_plane, 180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mesh = mergeMesh(mesh, bottom_plane);
		
		rotateMesh(left_plane, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mesh = mergeMesh(mesh, left_plane);				

		rotateMesh(right_plane, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		mesh = mergeMesh(mesh, right_plane);	
		
		rotateMesh(back_plane, -90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		mesh = mergeMesh(mesh, back_plane);	
		
		rotateMesh(front_plane, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		mesh = mergeMesh(mesh, front_plane);	
				
		computeNormals(mesh);
		
		return mesh;
	}
	
	Mesh
	makeSimpleSphere(unsigned int rows, unsigned int cols, float radius)
	{
		
		//// begin make simple grid
		if( rows < 1) rows = 1;
		if( cols < 1) cols = 1;
		Mesh mesh;
		std::vector<Vertex> vertices;
		vertices.clear();
		
		vertices.reserve((cols+1) * (rows + 1));
		for(size_t y = 0; y < rows+1; y++)
		{
			for(size_t x = 0; x < cols+1; x++)
			{	
				Vertex vtx;
				float pos_x = (1.0/(float)cols) * (float)x;
				float pos_y = (1.0/(float)rows) * (float)y;
				vtx.position = glm::vec3(pos_x, pos_y ,0.0f);
					
				vtx.normal = glm::vec3(0.0f, 0.0f, 1.0f);
				vtx.texture_coords = glm::vec2(pos_y, 1.0-pos_x);
				vertices.emplace_back(vtx);
			}
		}
		
		std::vector<unsigned int> indices;
		indices.clear();
		indices.reserve(cols * rows * 6);
		for(size_t y = 0; y < rows; y++)
		{
			for(size_t x = 0; x < cols; x++)
			{		
				unsigned int index = x + (y * (cols+1));
				/*
				 * if() tests are for removing every other triangle at the 'poles', because they get squashed with the deformation
				 * and become invalid ( because 0 area)
				 * */
				if(x != 0)
				{
					indices.emplace_back(index);
					indices.emplace_back(index + cols + 2);
					indices.emplace_back(index + cols + 1);
				}

				if(x != cols-1)
				{				
					indices.emplace_back(index);
					indices.emplace_back(index + 1);
					indices.emplace_back(index + cols + 2);
				}
			}
		}
		
		mesh.vertices = vertices;
		mesh.indices = indices;
		
		///// end make simple grid
		
		for(auto& vtx : mesh.vertices)
		{
			vtx.position = polar_to_euclidian(vtx.position.x * PI, vtx.position.y * PI * 2.f, radius);
			vtx.normal = glm::normalize(vtx.position);
		}
		

		//computeNormals(mesh);
		
		
		//// try and fix poles normals
		for( size_t i = cols; i <= (rows+1) * (cols+1); i+= (cols+1))
		{
			
			//~ mesh.vertices[i].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			//~ mesh.vertices[i - cols].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
			mesh.vertices[i].normal = glm::vec3(0.0f, 0.0f, -1.0f);
			mesh.vertices[i - cols].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		}
		
		return mesh;
	}
	
	Mesh
	makeGrid(int rows, int cols)
	{
		if( rows < 1) rows = 1;
		if( cols < 1) cols = 1;
		Mesh mesh;
		std::vector<Vertex> vertices;
		vertices.clear();
		
		vertices.reserve((cols+1) * (rows + 1));
		for(int y = 0; y < rows+1; y++)
		{
			for(int x = 0; x < cols+1; x++)
			{	
				Vertex vtx;
				float pos_x = (1.0/(float)cols) * (float)x;
				float pos_y = (1.0/(float)rows) * (float)y;
				vtx.position = glm::vec3(pos_x, pos_y ,0.0f);
					
				vtx.normal = glm::vec3(0.0f, 0.0f, 1.0f);
				vtx.texture_coords = glm::vec2(pos_x, pos_y);
				vertices.emplace_back(vtx);
			}
		}
		
		std::vector<unsigned int> indices;
		indices.clear();
		indices.reserve(cols * rows * 6);
		for(int y = 0; y < rows; y++)
		{
			for(int x = 0; x < cols; x++)
			{		
				unsigned int index = x + (y * (cols+1));
				
				indices.emplace_back(index);
				indices.emplace_back(index + cols + 2);
				indices.emplace_back(index + cols + 1);
				
				indices.emplace_back(index);
				indices.emplace_back(index + 1);
				indices.emplace_back(index + cols + 2);
			}
		}
		
		mesh.vertices = vertices;
		mesh.indices = indices;
		
		return mesh;
		
	}


	Mesh
	makeSimpleQuad(float width, float height)
	{
		Mesh mesh;
		
		Vertex vtx;
		vtx.normal = glm::vec3(0.0f, 0.0f, 1.0f); 
		
		std::vector<Vertex> vertices;
		
		vertices.reserve(4);
		
		vtx.position = glm::vec3(0.0f, 0.0f, 0.0f);
		vtx.texture_coords = glm::vec2(0.0f, 1.0f);
		vertices.emplace_back(vtx);

		vtx.position = glm::vec3(width, 0.0f, 0.0f);
		vtx.texture_coords = glm::vec2(1.0f, 1.0f);
		vertices.emplace_back(vtx);		
		
		vtx.position = glm::vec3(width, height, 0.0f);
		vtx.texture_coords = glm::vec2(1.0f, 0.0f);
		vertices.emplace_back(vtx);		
		
		vtx.position = glm::vec3(0.0f, height, 0.0f);
		vtx.texture_coords = glm::vec2(0.0f, 0.0f);
		vertices.emplace_back(vtx);		
		
		
		std::vector<unsigned int> indices = {
			0,1,2,
			0,2,3
		};
		
		mesh.vertices = vertices;
		mesh.indices = indices;
		
		return mesh;
	}
}
}
