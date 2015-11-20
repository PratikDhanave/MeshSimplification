#pragma once
#include <string>
#include <vector>
//#include "vec_math.h"

#ifndef uint
#define uint unsigned int
#endif


char int_to_str_out[100];
std::string int_to_str(const int x)
{
	sprintf(int_to_str_out,"%d",x);

	return std::string(int_to_str_out);
}


struct Tri3
{
	int points[3];
	int normals[3];
	int tex_coords[3];
	int material;
};


class Texture
{
	public:
	Texture():gl_handle(-1){}

	std::string filename;
	int gl_handle;
};

class Material
{
	public:

	Material()
	{
		ambient = vec3f ( 0.6 , 0.3 ,0 );
		diffuse = vec3f ( 0.3 , 0.3 ,0.3 );
		specular= vec3f ( 0,0,0 );
		alpha = 1;
		reflect = 0;
		name = "";
	}

	std::string name;
	vec3f diffuse;
	vec3f specular;
	vec3f ambient;
	float alpha,reflect;
	Texture diffuse_map;
	Texture diffuse_map2;
	Texture diffuse_map3;
	Texture diffuse_map4;
	Texture diffuse_map5;
	Texture ambient_map;
	Texture bump_map;
};

struct SubGroup{ int listid,material;};

class Geometry
{
	public:
	std::string name;
	std::string filename;
	std::vector<vec3f> points;
	std::vector<vec3f> normals;
	std::vector<vec3f> tex_coords;
	std::vector<Tri3> triangles;
	vec3f bb_min;
	vec3f bb_max;

	vec3f rotation;
	vec3f translation;

	vec3f initial_rotation;
	vec3f initial_translation;

	std::vector<SubGroup> subgroups;

	Geometry(){clear() ;}
		
	void clear() 
	{
		points.clear();
		normals.clear();
		tex_coords.clear();
		triangles.clear();
		subgroups.clear();
		rotation = vec3f(0,0,0);
		translation = vec3f(0,0,0);
	};
	void init_bbox()
	{
		if (points.size ()<2) return;
		if (triangles.size ()<2) return;

		vec3f min,max;
		min = max = points[triangles[0].points[0]];

		for ( std::size_t i = 0 ; i < triangles.size () ; i++ )
		loopj(0,3)
		{
			vec3f p=points[triangles[i].points[j]];
			if ( p.x < min.x ) min.x = p.x;
			if ( p.y < min.y ) min.y = p.y;
			if ( p.z < min.z ) min.z = p.z;
			if ( p.x > max.x ) max.x = p.x;
			if ( p.y > max.y ) max.y = p.y;
			if ( p.z > max.z ) max.z = p.z;
		}
		bb_min = min;
		bb_max = max;

		translation=(bb_max+bb_min)*0.5;

		initial_rotation=vec3f(0,0,0);
		initial_translation=translation;

		bb_min = bb_min - translation;
		bb_max = bb_max - translation;
		loopi(0,points.size())points[i]=points[i]-translation;
	}	
	void normalize_()
	{
		init_bbox();

		float sx=bb_max.x-bb_min.x;
		float sy=bb_max.y-bb_min.y;
		float sz=bb_max.z-bb_min.z;
		float maxsize=sx;
		if(sy>maxsize)maxsize=sy;
		if(sz>maxsize)maxsize=sz;
		
		float mx=(bb_max.x+bb_min.x)/2;
		float my=(bb_max.y+bb_min.y)/2;
		float mz=(bb_max.z+bb_min.z)/2;
		
		for ( std::size_t i = 0 ; i < points.size () ; i++ )
		{
			float x=points[i].x;
			float y=points[i].y;
			float z=points[i].z;
						
			x=(x-mx)/maxsize;
			y=(y-my)/maxsize;
			z=(z-mz)/maxsize;
			
			points[i].x=x; 
			points[i].y=y; 
			points[i].z=z; 
		}
	}

	
};
//class geometry ends//

class OBJ { 
public:
 
	std::vector<Material>	 materials;
	std::vector<Geometry>	 objects;

	OBJ(){};
	OBJ(char* filename)
	{
		materials.clear();
		objects.clear();
		load_obj(filename);
	}
	~OBJ(){

		//materials.clear();
		//objects.clear();
	}

	int push_back( Material& mat )
	{
		materials.push_back(mat);
		return materials.size();
	}

	// objects //
	void print_objects ()
	{
		for ( uint i = 0 ; i<objects.size(); i++ )
		{
			printf("Object[%d] = %s\n", i,objects[i].name.c_str() );
			printf("  - Tri3s: %d\n", objects[i].triangles.size() );
			printf("  - Vertices : %d\n", objects[i].points.size() );
			printf("  - Normals  : %d\n", objects[i].normals.size() );
			printf("  - TexCoords: %d\n", objects[i].tex_coords.size() );
			printf("  - Center   : %2.3lf %2.3lf %2.3lf\n",objects[i].translation.x,objects[i].translation.y,objects[i].translation.z);
			printf("  - BB max   : %2.3lf %2.3lf %2.3lf\n",objects[i].bb_max.x,objects[i].bb_max.y,objects[i].bb_max.z);
			printf("  - BB min   : %2.3lf %2.3lf %2.3lf\n",objects[i].bb_min.x,objects[i].bb_min.y,objects[i].bb_min.z);
		}
	}
	int get_object_index ( std::string& name )
	{
		for ( uint i = 0 ; i<objects.size(); i++ )
			if ( name.compare( objects[i].name ) == 0 ) return i;

		return -1;
	}
	//  String  //
	std::string get_path ( std::string filename )
	{
		uint pos1 = filename.find_last_of( "/" );
		uint pos2 = filename.find_last_of( "\\" );

		if ( pos1 ==  std::string::npos ) pos1 = pos2;
		if ( pos1 ==  std::string::npos ) return "./";

		if (pos1 < filename.size())
		if (pos1 != 0)
			pos1++;

		//printf( "substr = %s\n" , filename.substr(0,pos1).c_str());

		return (filename.substr(0,pos1));
	}

	std::string get_pure_filename ( std::string filename )
	{
		uint pos1 = filename.find_last_of( "/" );
		uint pos2 = filename.find_last_of( "\\" );
		uint pos3 = filename.find_last_of( "." );

		if ( pos1 ==  std::string::npos ) pos1 = pos2;
		if ( pos1 ==  std::string::npos ) pos1 = 0;
		if ( pos1 < filename.size())
		if ( pos1 != 0 )pos1++;

		if ( pos3 == std::string::npos ) 
		{
			pos3 = filename.size();
		}		
		//printf( "input %s substr = %s\n" ,filename.c_str(), filename.substr(pos1,pos3-pos1).c_str());
		return (filename.substr(pos1,pos3-pos1));
	}

	// ---------- Load Mtl ------------ //

	void load_mtl(const char* filename , std::string prefix){

		printf ( "Loading Materials %s ... \n",filename);

		std::string path = get_path( filename );

		FILE* fn;
		if(filename==NULL)		return ;
		if((char)filename[0]==0)	return ;
		if ((fn = fopen(filename, "rb")) == NULL)
		{
			printf ( "File %s not found!\n" ,filename );
			while(1);
		}
		char line[1000];
		memset ( line,0,1000 );

		Material mat,empty_mat;
		char str[200];
		vec3f color;
		float alpha=1,reflect=0;
		bool first = true;

		while(fgets( line, 1000, fn ) != NULL)
		{
			switch (line[0] )
			{
				case 'n':
					char material_str[200];
					if(sscanf(line,"newmtl %s",material_str)==1) 
					{
						if (!first)
							materials.push_back ( mat );

						mat = empty_mat;
						mat.name = prefix + std::string(material_str);
						first = false;
					}
				break;
				
				case 'K':
					if(sscanf(line,"Kd %lf %lf %lf",
						&color.x,&color.y,&color.z)==3) 
					{
						mat.diffuse = color;
					}
					if(sscanf(line,"Ka %lf %lf %lf",
						&color.x,&color.y,&color.z)==3) 
					{
						mat.ambient = color;
					}
				//break;
				//case 'K':
					if(sscanf(line,"Ks %lf %lf %lf",
					   &color.x,&color.y,&color.z)==3) 
					{
						mat.specular = color;
					}
				case 'R':
					if(sscanf(line,"Rf %f",
					   &reflect)==1) 
					{
						mat.reflect = reflect;
					}
				case 'O':
					if(sscanf(line,"Op %f",&alpha)==1) 
					{
						mat.alpha = alpha;
					}
				break;
			}
		}
		if (!first)
			materials.push_back ( mat );

		fclose(fn);
	}

	// ---------- Load Obj ------------ //

	void load_obj(const char* filename){

		printf ( "\nLoading Objects %s ... \n",filename);

		get_pure_filename ( std::string(filename) );

		std::string path = get_path( filename );
		FILE* fn;
		if(filename==NULL)		return ;
		if((char)filename[0]==0)	return ;
		if ((fn = fopen(filename, "rb")) == NULL)
		{
			printf ( "File %s not found!\n" ,filename );
			while(1);
		}
		char line[1000];
		memset ( line,0,1000 );

		int material= -1;
		int vertex_cnt=0;
		int tex_coords_cnt=0;
		int normals_cnt=0;

		char state='-',state_before='-';

		Geometry geo;
		
		geo.name = "default";

		static int counter=0;counter++;

		std::string mtl_prefix = int_to_str(counter) + "-";

		while(fgets( line, 1000, fn ) != NULL)
		{
			/*
			if ( line[0] == 'v' )
			if ( line[1] == ' ' )
			{
				state_before = state;state ='v';
			}*/

			vec3f val; 

			if ( line[0] == 'v' )
			{
				if ( line[1] == ' ' )
				if(sscanf(line,"v %lf %lf %lf",
					&val.x,	&val.y,	&val.z)==3) 
				{
					val.x*=1;
					val.y*=1;
					val.z*=1;
					geo.points.push_back ( val );
				}

				if ( line[1] == 't' )
					if(sscanf(line,"vt %lf %lf",
						&val.x,	&val.y)==2) 
							geo.tex_coords.push_back ( val );

				if ( line[1] == 'n' )
					if(sscanf(line,"vn %lf %lf %lf",
						&val.x,	&val.y,	&val.z)==3) 
							geo.normals.push_back ( val );
			}

			int integers[9];
			
			if ( line[0] == 'f' )
			{
				Tri3 tri;
				bool tri_ok = false;

				if(sscanf(line,"f %d %d %d",
					&integers[0],&integers[1],&integers[2])==3) 
				{
					tri_ok = true;
				}else
				if(sscanf(line,"f %d// %d// %d//",
					&integers[0],&integers[1],&integers[2])==3) 
				{
					tri_ok = true;
				}else
				if(sscanf(line,"f %d//%d %d//%d %d//%d",
					&integers[0],&integers[3],
					&integers[1],&integers[4],
					&integers[2],&integers[5])==6) 
				{
					tri_ok = true;
				}else
				if(sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",
					&integers[0],&integers[6],&integers[3],
					&integers[1],&integers[7],&integers[4],
					&integers[2],&integers[8],&integers[5])==9) 
				{
					tri_ok = true;
				}
				else
				{
					printf("unrecognized sequence\n");
					printf("%s\n",line);
					while(1);
				}

				if ( tri_ok )
				{
					tri.points[0] = integers[0]-1-vertex_cnt;
					tri.points[1] = integers[1]-1-vertex_cnt;
					tri.points[2] = integers[2]-1-vertex_cnt;

					tri.normals[0] = integers[3]-1-normals_cnt;
					tri.normals[1] = integers[4]-1-normals_cnt;
					tri.normals[2] = integers[5]-1-normals_cnt;

					tri.tex_coords[0] = integers[6]-1-tex_coords_cnt;
					tri.tex_coords[1] = integers[7]-1-tex_coords_cnt;
					tri.tex_coords[2] = integers[8]-1-tex_coords_cnt;

					for(int t=0;t<3;t++)
						if (tri.tex_coords[t]<0) tri.tex_coords[t]=0;
					
					if ( (uint)tri.points[1] >= geo.points.size())
					{
						int a = integers[0]-vertex_cnt;
						int b = geo.points.size();
						printf("Error at f%d: %d-%d > %d!\n",
							geo.triangles.size(),
							tri.points[1],vertex_cnt , 
							geo.points.size());
					}

					tri.material = material;
					geo.triangles.push_back ( tri );

					//state_before = state;
					//state ='f';
				}
			}
			if ( line[0] == 'm' )
			{
				char mtl_file_str[200];
				if(sscanf(line,"mtllib %s",mtl_file_str)==1)
				{
					std::string s = path + std::string(mtl_file_str);
					this->load_mtl ( s.c_str() , mtl_prefix );
				}
			}
			
			if ( line[0] == 'g' || line[0] == 'o' )
			{
				geo.init_bbox();

				if(geo.triangles.size()>0) objects.push_back( geo );
				/*
				vertex_cnt    += geo.points.size();
				tex_coords_cnt+= geo.tex_coords.size();
				normals_cnt   += geo.normals.size();
				geo.clear();
				*/
				geo.triangles.clear();

				char name_str[300];
				if(sscanf(line,"g %s",name_str)==1)
				{
					geo.name = name_str;
					//printf("group:%s\n",name_str );
				}
				if(sscanf(line,"o %s",name_str)==1)
				{
					geo.name = name_str;
					//printf("group:%s\n",name_str );
				}
			}
		}
		fclose(fn);

		geo.init_bbox();
		if(geo.triangles.size()>0) objects.push_back( geo );

		//print_materials();
		//print_objects  ();

		printf ( "Read %s - %d points , %d faces\n" , 
			filename , geo.points.size() , geo.triangles.size() );
	}
	

};
