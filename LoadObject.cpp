#include "LoadObject.h"

LoadObject::LoadObject(const char* filename, float scale)
{
	FILE* points;
	bool Debug = false;
	end.push_back(Co_ord());
	nor.push_back(Co_ord());
	ver.push_back(Co_ord());
	tex.push_back(Co_ord());
	points = fopen(filename, "r");
	if (points == NULL) {
		printf("Cannot find the file ...");
		exit(1);
	}
	printf("\nLOADING OBJECT : %s", filename);

	// LOADING NORMALS
	printf("\nLoading normals ... ");
	for (char chBuffer = NULL; chBuffer != EOF; chBuffer = getc(points)) {
		switch (chBuffer) {
		case 'v':
			if (getc(points) == 'n') {
				if (Debug)printf("\nreading normal ... %d", nor.size());
				nor.push_back(Co_ord());
				fscanf(points, "%f %f %f\n", &nor[nor.size() - 1].x, &nor[nor.size() - 1].y, &nor[nor.size() - 1].z);
				break;
			}
			else fscanf(points, "\n");
		default:
			fscanf(points, "\n");
			break;
		}
	}
	if (Debug)printf("\n%d normals loaded ...", nor.size() - 1);
	// NORMALS LOADED

	// LOADING TEXTURES
	printf("\nLoading textures ...");
	fseek(points, 0, SEEK_SET);
	for (char chBuffer = NULL; chBuffer != EOF; chBuffer = getc(points)) {
		switch (chBuffer) {
		case 'v':
			if (getc(points) == 't') {
				if (Debug)printf("\nreading texture ... %d", tex.size());
				tex.push_back(Co_ord());
				fscanf(points, "%f %f %f\n", &tex[tex.size() - 1].x, &tex[tex.size() - 1].y, &tex[tex.size() - 1].z);
				break;
			}
			else fscanf(points, "\n");
		default:
			fscanf(points, "\n");
			break;
		}
	}
	if (Debug)printf("\n%d textures loaded ...", tex.size() - 1);
	// TEXTURES LOADED

	// LOADING VERTICES
	printf("\nLoading vertices ...");
	fseek(points, 0, SEEK_SET);
	for (char chBuffer = NULL; chBuffer != EOF; chBuffer = getc(points)) {
		switch (chBuffer) {
		case 'v':
			if (getc(points) == ' ') {
				if (Debug)printf("\nreading vertice ... %d", ver.size());
				ver.push_back(Co_ord());
				fscanf(points, "%f %f %f\n", &ver[ver.size() - 1].x, &ver[ver.size() - 1].y, &ver[ver.size() - 1].z);
				if (ver[ver.size() - 1].x < end[0].x) end[0].x = ver[ver.size() - 1].x;
				if (ver[ver.size() - 1].y < end[0].y) end[0].y = ver[ver.size() - 1].y;
				if (ver[ver.size() - 1].z < end[0].z) end[0].z = ver[ver.size() - 1].z;
				if (ver[ver.size() - 1].x > end[0].xt) end[0].xt = ver[ver.size() - 1].x;
				if (ver[ver.size() - 1].y > end[0].yt) end[0].yt = ver[ver.size() - 1].y;
				if (ver[ver.size() - 1].z > end[0].zt) end[0].zt = ver[ver.size() - 1].z;
				break;
			}
		default:
			fscanf(points, "\n");
			break;
		}
	}
	if (Debug)printf("\n%d vertices loaded ...", ver.size() - 1);
	// VERTICES LOADED

	// LOADING FACES
	printf("\nLoading faces ...");
	fseek(points, 0, SEEK_SET);
	for (char chBuffer = NULL; chBuffer != EOF; chBuffer = getc(points)) {
		switch (chBuffer) {
		case 'f':
			face.push_back(Co_ord());
			fscanf(points, "%f", &face[face.size() - 1].x);
			if (fgetc(points) == '/')
			{
				if (fgetc(points) == '/')
				{
					if (Debug)printf("\nreading faces x//x x//x x//x ... %d", face.size());
					fscanf(points, "%f %f//%f %f//%f\n", &face[face.size() - 1].xn, &face[face.size() - 1].y, &face[face.size() - 1].yn, &face[face.size() - 1].z, &face[face.size() - 1].zn);
					fscanf(points, "%f", &face[face.size() - 1].a);
					if (face[face.size() - 1].a == '\n') {
						face[face.size() - 1].a = NULL;
					}
					else
					{
						fscanf(points, "//%f\n", &face[face.size() - 1].an);
					}
				}
				else
				{
					fseek(points, -1, SEEK_CUR);
					fscanf(points, "%f", &face[face.size() - 1].xt);
					if (fgetc(points) == '/')
					{
						if (Debug)printf("\nreading faces x/x/x x/x/x x/x/x ... %d", face.size());
						fscanf(points, "%f %f/%f/%f %f/%f/%f", &face[face.size() - 1].xn, &face[face.size() - 1].y, &face[face.size() - 1].yt, &face[face.size() - 1].yn, &face[face.size() - 1].z, &face[face.size() - 1].zt, &face[face.size() - 1].zn);
						fscanf(points, "%f", &face[face.size() - 1].a);
						if (face[face.size() - 1].a == '\n') {
							face[face.size() - 1].a = NULL;
						}
						else
						{
							fscanf(points, "/%f/%f\n", &face[face.size() - 1].at, &face[face.size() - 1].an);
						}
					}
					else
					{
						if (Debug)printf("\nreading faces x/x x/x x/x ... %d", face.size());
						fscanf(points, " %f/%f %f/%f", &face[face.size() - 1].y, &face[face.size() - 1].yt, &face[face.size() - 1].z, &face[face.size() - 1].zt);
						fscanf(points, "%f", &face[face.size() - 1].a);
						if (face[face.size() - 1].a == '\n') {
							face[face.size() - 1].a = NULL;
						}
						else
						{
							fscanf(points, "%f\n", &face[face.size() - 1].at);
						}
					}
				}
			}
			else
			{
				if (Debug)printf("\nreading faces x x x ... %d", face.size());
				fscanf(points, "%f %f", &face[face.size() - 1].y, &face[face.size() - 1].z);
				fscanf(points, "%f", &face[face.size() - 1].a);
				if (face[face.size() - 1].a == '\n') {
					face[face.size() - 1].a = NULL;
				}
			}
			break;
		default:
			fscanf(points, "\n");
			break;
		}
	}
	if (Debug)printf("\n%d faces loaded ...", face.size());
	// FACES LOADED

	// CENTERING THE OBJECT
	/*float temp = (end[0].x + end[0].xt) / 2;
	for (unsigned int i = 0; i < ver.size(); i++)ver[i].x -= temp;
	end[0].x -= temp; end[0].xt -= temp;
	temp = (end[0].y + end[0].yt) / 2;
	for (unsigned int i = 0; i < ver.size(); i++)ver[i].y -= temp;
	end[0].y -= temp; end[0].yt -= temp;
	temp = (end[0].z + end[0].zt) / 2;
	for (unsigned int i = 0; i < ver.size(); i++)ver[i].z -= temp;
	end[0].z -= temp; end[0].zt -= temp;
	if (Debug)printf("\nObject Centered ...");*/
	// OBJECT CENTERED


	if (ver.size() != 1)printf("\n%d vertices loaded ...", ver.size() - 1);
	if (tex.size() != 1)printf("\n%d textures loaded ...", tex.size() - 1);
	if (nor.size() != 1)printf("\n%d normals loaded ...", nor.size() - 1);
	if (face.size() != 1)printf("\n%d faces loaded ...", face.size());
}

LoadObject::~LoadObject()
{
	ver.clear();
}
