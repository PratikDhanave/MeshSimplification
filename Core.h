#pragma once
////////////////////////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <iostream> 
#include <vector> 
#include <string> 
#include <stdio.h>
#include <glew.h>
#include <wglew.h>
#include <GL/glut.h>// Header File For The GLUT Library 
#include <time.h>
#include <mmsystem.h>
#include <windows.h>
using namespace std;
#include "VecMath.h"
#include "glsl.h"
#define _USE_MATH_DEFINES
#define loopi(start_l,end_l,step_l) for ( int i=start_l;i<end_l;i+=step_l )
#define loopi(start_l,end_l) for ( int i=start_l;i<end_l;++i )
#define loopj(start_l,end_l,step_l) for ( int j=start_l;j<end_l;j+=step_l )
#define loopj(start_l,end_l) for ( int j=start_l;j<end_l;++j )
#define loopk(start_l,end_l,step_l) for ( int k=start_l;k<end_l;k+=step_l )
#define loopk(start_l,end_l) for ( int k=start_l;k<end_l;++k )
#define loopm(start_l,end_l) for ( int m=start_l;m<end_l;++m )
#define loopl(start_l,end_l) for ( int l=start_l;l<end_l;++l )
#define loopn(start_l,end_l) for ( int n=start_l;n<end_l;++n )
#define loop(var_l,start_l,end_l) for ( int var_l=start_l;var_l<end_l;++var_l )
#define loops(a_l,start_l,end_l,step_l) for ( a_l = start_l;a_l<end_l;a_l+=step_l )

/*
#ifndef byte
#define byte unsigned char
#endif

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

*/
