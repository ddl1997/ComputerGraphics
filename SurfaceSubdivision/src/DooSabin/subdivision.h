#ifndef SUBDIVISION_H
#define SUBDIVISION_H

#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <time.h>
#include <Windows.h>
using namespace std;

struct Point
{
	double x,y,z;
};

struct Vertex;
struct Face;

struct HalfEdge
{
	Vertex *vert;
	HalfEdge *pair;
	Face *face;
	HalfEdge *next;
	Point midEdgePoint;
	int index;
};

struct Vertex
{
	Point point;
	HalfEdge *edge;
};

struct Face
{
	HalfEdge *edge;
	Point midFacePoint;
	int index;
};

#endif