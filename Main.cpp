
#include<conio.h>
#include "Core.h"
#include "OBJ.h"
#include "Simplify.h"

extern float fps;
int main(int argc, char **argv) 
{ 
	char filename[20];
	int parameter;
	printf("\n\t Enter object filename : ");		// get object filename from user
	scanf("%s",filename);

	OBJ obj(filename);
	// Setup Mesh for Simplification
	
	// Simplify::vertices[].p : vertex position
	// Simplify::triangles[].v[0..2] : vertex indices
	
	loopi(0,obj.objects[0].points.size())
	{
		Simplify::Vertex v;
		v.p=obj.objects[0].points[i];
		Simplify::vertices.push_back(v);
	}
	loopi(0,obj.objects[0].triangles.size())
	{
		Simplify::Triangle t;
		loopj(0,3)t.v[j]=obj.objects[0].triangles[i].points[j];
		Simplify::triangles.push_back(t);
	}
	printf("Input: %d triangles %d vertices\n",Simplify::triangles.size(),Simplify::vertices.size());
	
	// Simplification start
	int target,choice;
	printf("\nReduce By : \n [1] 50% \n [2] 30% \n [3] 15% \n Enter your choice :");		// get choice from user
	scanf("%d",&choice);
	target = Simplify::triangles.size();

	switch(choice)
	{
	case 1 : target = 0.50 * target;			// 50%
			 Simplify::simplify_mesh(target);
			 break;
	case 2 : target = 0.70 * target;			//30%
			 Simplify::simplify_mesh(target);
			 break;

	case 3 : target = 0.85 * target;		
			 Simplify::simplify_mesh(target);	//15%
			 break;

	default : printf("\n\t Invalid input ! ");
	}
			 
	Simplify::write_obj("../reduced_bunny.obj");
	
	// Simplification done;	
	// Visualize the Result
	
	printf("Output: %d triangles %d vertices\n",Simplify::triangles.size(),Simplify::vertices.size());
	
	loopi(0,Simplify::vertices.size())
	{
		obj.objects[0].points[i]=Simplify::vertices[i].p;
	}
	int ofs=0;
	loopi(0,Simplify::triangles.size())
	{
		if(!Simplify::triangles[i].deleted)
		{
			obj.objects[0].triangles[ofs] = obj.objects[0].triangles[i];
			obj.objects[0].triangles[ofs].material=0;
			loopj(0,3) obj.objects[0].triangles[ofs].points[j]=Simplify::triangles[i].v[j];
			ofs++;
		}
	}
	obj.objects[0].triangles.resize(ofs);

	printf("\n\n MESH REDUCED. File saved ! \n\n Press ENTER to exit");
	getch();
	
	
}

