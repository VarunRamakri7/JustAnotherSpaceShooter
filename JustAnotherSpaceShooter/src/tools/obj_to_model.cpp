#include <iostream>
#include <iomanip>
#include <fstream>
#include "OBJ_Loader.h"
#include <cfloat>

int main(int argc, char* argv[])
{
	std::string filename = "enemy_1";
	std::string in_filename = filename + ".obj";
	std::string out_filename = filename + ".model";

	objl::Loader Loader;
	bool loadout = Loader.LoadFile(in_filename);

	if (loadout)
	{
		std::ofstream file(out_filename);

		unsigned int total_floats = 0;
		float extreme_min_x = FLT_MAX;
		float extreme_min_y = FLT_MAX;
		float extreme_min_z = FLT_MAX;
		float extreme_max_x = FLT_MIN;
		float extreme_max_y = FLT_MIN;
		float extreme_max_z = FLT_MIN;

		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			for (int j = 0; j < curMesh.Indices.size(); j++)
			{
				int index_1 = curMesh.Indices[j];
				file << std::fixed << std::setprecision(3) << std::showpos << curMesh.Vertices[index_1].Position.X << " " << std::showpos << curMesh.Vertices[index_1].Position.Y << " " << std::showpos << curMesh.Vertices[index_1].Position.Z << " " << std::showpos << curMesh.Vertices[index_1].Normal.X << " " << std::showpos << curMesh.Vertices[index_1].Normal.Y << " " << std::showpos << curMesh.Vertices[index_1].Normal.Z << " " << std::showpos << curMesh.Vertices[index_1].TextureCoordinate.X << " " << std::showpos << curMesh.Vertices[index_1].TextureCoordinate.Y << "\n";

				if(curMesh.Vertices[index_1].Position.X < extreme_min_x) extreme_min_x = curMesh.Vertices[index_1].Position.X;
				if(curMesh.Vertices[index_1].Position.Y < extreme_min_y) extreme_min_y = curMesh.Vertices[index_1].Position.Y;
				if(curMesh.Vertices[index_1].Position.Z < extreme_min_z) extreme_min_z = curMesh.Vertices[index_1].Position.Z;
				if(curMesh.Vertices[index_1].Position.X > extreme_max_x) extreme_max_x = curMesh.Vertices[index_1].Position.X;
				if(curMesh.Vertices[index_1].Position.Y > extreme_max_y) extreme_max_y = curMesh.Vertices[index_1].Position.Y;
				if(curMesh.Vertices[index_1].Position.Z > extreme_max_z) extreme_max_z = curMesh.Vertices[index_1].Position.Z;

				total_floats += 8;
			}
		}

		file << "num_sub_models 1 " << "\n";
		file << "num_vertex_floats: " << total_floats << "\n";
		file << "extremes" << " " << extreme_min_x << " " << extreme_max_x << " " << extreme_min_y << " " << extreme_max_y << " " << extreme_min_z << " " << extreme_max_z << "\n";

		file.close();
		std::cout << "Done." << std::endl;
	}
	else
	{
		std::ofstream file("ERROR.log");
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";
		file.close();
	}

	return 0;
}
