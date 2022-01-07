#include "subdivision.h"

vector<Point> objVertices;
vector<vector<int>> objFaces;
vector<HalfEdge*> edges;
vector<Face*> faces;
vector<Vertex*> vertices;

void split(const string& s, vector<string>& tokens, const string& delimiters = " ")
{
    string::size_type lastPos = s.find_first_not_of(delimiters, 0);
    string::size_type pos = s.find_first_of(delimiters, lastPos);
    while (string::npos != pos || string::npos != lastPos)
    {
        tokens.push_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delimiters, pos);
        pos = s.find_first_of(delimiters, lastPos);
    }
}


bool importFile(string filename)
{
	int strLen = filename.length();
	if (filename.substr(strLen - 3, 3) != "obj")
		return false;
	ifstream obj(filename);
	if (!obj.is_open())
		return false;
	string str;
	while (getline(obj, str))
	{
		string type = str.substr(0, 2);
		if (type == "v " || type == "f ")
		{
			vector<std::string> parts;
			split(str, parts, " ");
			if (parts[0] == "v")
			{
				objVertices.push_back(Point{ atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()) });
			}
			else if (parts[0] == "f")
			{
				int pSize = parts.size();
				vector<int> temp;
				for (int i = 1; i < pSize; i++)
				{
					vector<string> v;
					split(parts[i], v, "/");
					temp.push_back(atoi(parts[0].c_str()) - 1);
				}
				objFaces.push_back(temp);
			}
		}
	}
	return true;
}

void initHalfEdge() 
{
	for(auto v = objVertices.begin(); v != objVertices.end(); v++)
	{
		Vertex* tempV = new Vertex();
		tempV->point.x = v->x;
		tempV->point.y = v->y;
		tempV->point.z = v->z;
		tempV->edge = NULL;
		vertices.push_back(tempV);
	}
	int tempFaceIndex = 0;

	int fSize = objFaces.size();
	for (int i = 0; i < fSize; i++)
	{
		auto objF = objFaces[i];
		int halfEdgeIndex = 0;
		Face* face = new Face();

		vector<int>::iterator ptr2 = objF.begin();
		ptr2->back()
		HalfEdge* firstE = new HalfEdge();

		firstE->vert = vertices[objF[0]];
		firstE->face = face;
		firstE->pair = NULL;
		for (auto f = faces.begin(); f != faces.end(); f++)
		{
			HalfEdge* findPair = (*f)->edge;
			do
			{
				if ((findPair->vert == vertices[objF[0]]) && (findPair->next->vert == vertices[ptr1->back() - 1]))
				{
					findPair->next->pair = firstE;
					firstE->pair = findPair->next;
					break;
				}
				findPair = findPair->next;
			} while ((findPair != (*f)->edge) && findPair != NULL);
			if (firstE->pair != NULL)
				break;
		}

		vertices[(*(ptr1->end() - 1)) - 1]->edge = firstEdge;
		//计算边的中点
		Point tempMidEdgePoint;
		tempMidEdgePoint.x = (vertices[(*ptr2) - 1]->point.x + vertices[ptr1->back() - 1]->point.x) / 2.0f;
		tempMidEdgePoint.y = (vertices[(*ptr2) - 1]->point.y + vertices[ptr1->back() - 1]->point.y) / 2.0f;
		tempMidEdgePoint.z = (vertices[(*ptr2) - 1]->point.z + vertices[ptr1->back() - 1]->point.z) / 2.0f;
		firstEdge->midEdgePoint = tempMidEdgePoint;

		firstEdge->index = tempHalfEdgeIndex;
		tempHalfEdgeIndex++;
		edges.push_back(firstEdge);
		HalfEdge* prevEdge = firstEdge;

		for (; (ptr2 + 1) != ptr1->end(); ++ptr2)
		{
			HalfEdge* tempHalfEdge = new HalfEdge();
			tempHalfEdge->vert = vertices[(*(ptr2 + 1)) - 1];
			prevEdge->next = tempHalfEdge;
			vertices[(*ptr2) - 1]->edge = tempHalfEdge;
			tempHalfEdge->face = tempFace;

			if ((ptr2 + 2) == ptr1->end())  //如果是最后一个，就设置next为第一个
				tempHalfEdge->next = firstEdge;

			tempHalfEdge->pair = NULL;
			for (vector<Face*>::iterator facePtr = faces.begin(); facePtr != faces.end(); ++facePtr)
			{
				HalfEdge* findPair = (*facePtr)->edge;
				do
				{
					if ((findPair->vert == vertices[(*(ptr2 + 1)) - 1]) && (findPair->next->vert == vertices[(*ptr2) - 1]))
					{
						findPair->next->pair = tempHalfEdge;
						tempHalfEdge->pair = findPair->next;
						break;
					}
					findPair = findPair->next;
				} while ((findPair != (*facePtr)->edge) && findPair != NULL);
				if (tempHalfEdge->pair != NULL)
					break;
			}
			prevEdge = tempHalfEdge;

			//计算中点
			tempMidEdgePoint.x = (vertices[(*(ptr2 + 1)) - 1]->point.x + vertices[(*ptr2) - 1]->point.x) / 2.0f;
			tempMidEdgePoint.y = (vertices[(*(ptr2 + 1)) - 1]->point.y + vertices[(*ptr2) - 1]->point.y) / 2.0f;
			tempMidEdgePoint.z = (vertices[(*(ptr2 + 1)) - 1]->point.z + vertices[(*ptr2) - 1]->point.z) / 2.0f;
			tempHalfEdge->midEdgePoint = tempMidEdgePoint;
			tempHalfEdge->index = halfEdgeIndex;
			halfEdgeIndex++;
			edges.push_back(tempHalfEdge);
		}
		face->edge = firstEdge;

		//计算平面重心点
		HalfEdge* caculMid = face->edge;
		Point tempMidFacePoint;
		tempMidFacePoint.x = 0;
		tempMidFacePoint.y = 0;
		tempMidFacePoint.z = 0;
		int tempCount = 0;
		do
		{
			tempMidFacePoint.x += caculMid->vert->point.x;
			tempMidFacePoint.y += caculMid->vert->point.y;
			tempMidFacePoint.z += caculMid->vert->point.z;
			tempCount++;
			caculMid = caculMid->next;
		} while (caculMid != face->edge);
		tempMidFacePoint.x /= tempCount;
		tempMidFacePoint.y /= tempCount;
		tempMidFacePoint.z /= tempCount;
		face->midFacePoint = tempMidFacePoint;
		face->index = tempFaceIndex;
		tempFaceIndex++;
		faces.push_back(face);
	}
	//for(vector<vector<int>>::iterator ptr1=objVertixIndex.begin(); ptr1!=objVertixIndex.end(); ++ptr1)
	//{
	//	int tempHalfEdgeIndex = 0;
	//	Face *tempFace = new Face();

	//	vector<int>::iterator ptr2 = ptr1->begin();
	//	HalfEdge *firstEdge = new HalfEdge();

	//	firstEdge->vert = vertices[(*ptr2)-1];
	//	firstEdge->face = tempFace;
	//	firstEdge->pair = NULL;
	//	for(vector<Face*>::iterator facePtr=faces.begin(); facePtr!=faces.end(); ++facePtr)
	//	{
	//		HalfEdge *findPair = (*facePtr)->edge;
	//		do 
	//		{
	//			if((findPair->vert==vertices[(*ptr2)-1])&&(findPair->next->vert==vertices[ptr1->back()-1]))
	//			{
	//				findPair->next->pair= firstEdge;
	//				firstEdge->pair = findPair->next;
	//				break;
	//			}
	//			findPair = findPair->next;
	//		}while((findPair!=(*facePtr)->edge)&& findPair!=NULL);
	//		if(firstEdge->pair!=NULL)
	//			break;
	//	}

	//	vertices[(*(ptr1->end()-1))-1]->edge = firstEdge;
	//	//计算边的中点
	//	Point tempMidEdgePoint;
	//	tempMidEdgePoint.x = (vertices[(*ptr2)-1]->point.x + vertices[ptr1->back()-1]->point.x) / 2.0f;
	//	tempMidEdgePoint.y = (vertices[(*ptr2)-1]->point.y + vertices[ptr1->back()-1]->point.y) / 2.0f;
	//	tempMidEdgePoint.z = (vertices[(*ptr2)-1]->point.z + vertices[ptr1->back()-1]->point.z) / 2.0f;
	//	firstEdge->midEdgePoint = tempMidEdgePoint;

	//	firstEdge->index = tempHalfEdgeIndex;
	//	tempHalfEdgeIndex++;
	//	edges.push_back(firstEdge);
	//	HalfEdge *prevEdge = firstEdge;

	//	for(;(ptr2+1)!=ptr1->end(); ++ptr2)
	//	{
	//		HalfEdge *tempHalfEdge = new HalfEdge();
	//		tempHalfEdge->vert = vertices[(*(ptr2+1))-1];	
	//		prevEdge->next = tempHalfEdge;
	//		vertices[(*ptr2)-1]->edge = tempHalfEdge;
	//		tempHalfEdge->face = tempFace;

	//		if((ptr2+2)==ptr1->end())  //如果是最后一个，就设置next为第一个
	//			tempHalfEdge->next = firstEdge;

	//		tempHalfEdge->pair = NULL;
	//		for(vector<Face*>::iterator facePtr=faces.begin(); facePtr!=faces.end(); ++facePtr)
	//		{
	//			HalfEdge *findPair = (*facePtr)->edge;
	//			do 
	//			{
	//				if((findPair->vert==vertices[(*(ptr2+1))-1])&&(findPair->next->vert==vertices[(*ptr2)-1]))
	//				{
	//					findPair->next->pair = tempHalfEdge;
	//					tempHalfEdge->pair = findPair->next;
	//					break;
	//				}
	//				findPair = findPair->next;
	//			} while ((findPair!=(*facePtr)->edge)&&findPair!=NULL);
	//			if(tempHalfEdge->pair!=NULL)
	//					break;
	//		}
	//		prevEdge = tempHalfEdge;

	//		//计算中点
	//		tempMidEdgePoint.x = (vertices[(*(ptr2+1))-1]->point.x + vertices[(*ptr2)-1]->point.x) / 2.0f;
	//		tempMidEdgePoint.y = (vertices[(*(ptr2+1))-1]->point.y + vertices[(*ptr2)-1]->point.y) / 2.0f;
	//		tempMidEdgePoint.z = (vertices[(*(ptr2+1))-1]->point.z + vertices[(*ptr2)-1]->point.z) / 2.0f;
	//		tempHalfEdge->midEdgePoint = tempMidEdgePoint;
	//		tempHalfEdge->index = tempHalfEdgeIndex;
	//		tempHalfEdgeIndex++;
	//		edges.push_back(tempHalfEdge);
	//	}
	//	tempFace->edge = firstEdge;
	//	
	//	//计算平面重心点
	//	HalfEdge *caculMid = tempFace->edge;
	//	Point tempMidFacePoint;
	//	tempMidFacePoint.x = 0;
	//	tempMidFacePoint.y = 0;
	//	tempMidFacePoint.z = 0;
	//	int tempCount = 0;
	//	do 
	//	{
	//		tempMidFacePoint.x += caculMid->vert->point.x;
	//		tempMidFacePoint.y += caculMid->vert->point.y;
	//		tempMidFacePoint.z += caculMid->vert->point.z;
	//		tempCount++;
	//		caculMid = caculMid->next;	
	//	}while(caculMid!=tempFace->edge);
	//	tempMidFacePoint.x /= tempCount;
	//	tempMidFacePoint.y /= tempCount;
	//	tempMidFacePoint.z /= tempCount;
	//	tempFace->midFacePoint = tempMidFacePoint;
	//	tempFace->index = tempFaceIndex;
	//	tempFaceIndex++;
	//	faces.push_back(tempFace);
	//}
}

void dooSabin()
{
	int **table = (int**)malloc(sizeof(int*)*faces.size());
	bool **isEdgePair = (bool**)malloc(sizeof(bool*)*faces.size());
	for(int i=0; i<faces.size(); ++i)
	{
		table[i] = (int*)malloc(sizeof(int)*20);  //I cann't know how much edges in a face , so I malloc 20; it is enough!
		isEdgePair[i] = (bool*)malloc(sizeof(bool)*20);
	}
	for(int i=0; i<faces.size(); ++i)
	{
		for(int j=0; j<20; ++j)
		{
			table[i][j] = -1;
			isEdgePair[i][j] = false;
		}
	}
	objVertix.clear();
	objVertixIndex.clear();

	//for face-faces and initialize table
	int indexPoint = 1;
	for(vector<Face*>::iterator ptrF=faces.begin(); ptrF!=faces.end(); ++ptrF)
	{
		HalfEdge* tempHalfEdge = (*ptrF)->edge;
		vector<int> subVerticesIndex;
		do 
		{	
			Point subPoint;
			subPoint.x = (tempHalfEdge->midEdgePoint.x + tempHalfEdge->next->midEdgePoint.x + (*ptrF)->midFacePoint.x + tempHalfEdge->vert->point.x) / 4;
			subPoint.y = (tempHalfEdge->midEdgePoint.y + tempHalfEdge->next->midEdgePoint.y + (*ptrF)->midFacePoint.y + tempHalfEdge->vert->point.y) / 4;
			subPoint.z = (tempHalfEdge->midEdgePoint.z + tempHalfEdge->next->midEdgePoint.z + (*ptrF)->midFacePoint.z + tempHalfEdge->vert->point.z) / 4;
			objVertix.push_back(subPoint);
			table[(*ptrF)->index][tempHalfEdge->next->index] = indexPoint;
			subVerticesIndex.push_back(indexPoint);
			indexPoint++;
			tempHalfEdge = tempHalfEdge->next;
		}while(tempHalfEdge!=(*ptrF)->edge);
		objVertixIndex.push_back(subVerticesIndex);
	}
	//for vertices-face
	for(vector<Vertex*>::iterator ptrV=vertices.begin(); ptrV!=vertices.end(); ++ptrV)
	{
		HalfEdge *tempHalfEdge = (*ptrV)->edge;
		vector<int> subVerticesIndex1;
		do 
		{
			int tempIndexPoint;
			tempIndexPoint = table[tempHalfEdge->face->index][tempHalfEdge->index];
			subVerticesIndex1.push_back(tempIndexPoint);
			tempHalfEdge = tempHalfEdge->pair->next;
		}while(tempHalfEdge!=(*ptrV)->edge);
		//reverse
		vector<int> subVerticesIndex2;
		for(vector<int>::reverse_iterator ptrSubV=subVerticesIndex1.rbegin(); ptrSubV!=subVerticesIndex1.rend(); ++ptrSubV)
			subVerticesIndex2.push_back((*ptrSubV));
		objVertixIndex.push_back(subVerticesIndex2);
	}
	//for edge-faces
	for(vector<HalfEdge*>::iterator ptrE=edges.begin(); ptrE!=edges.end(); ++ptrE)
	{
		HalfEdge *tempHalfEdge = (*ptrE);
		if(!isEdgePair[tempHalfEdge->face->index][tempHalfEdge->index])
		{
			isEdgePair[tempHalfEdge->face->index][tempHalfEdge->index] = true;
			vector<int> subVerticesIndex;
			int tempIndexPoint1;
			int tempIndexPoint2;
			int tempIndexPoint3;
			int tempIndexPoint4;
			tempIndexPoint1 = table[tempHalfEdge->face->index][tempHalfEdge->index];
			tempIndexPoint2 = table[tempHalfEdge->next->face->index][tempHalfEdge->next->index];
			HalfEdge *tempHalfEdgePair = tempHalfEdge->pair;
			isEdgePair[tempHalfEdgePair->face->index][tempHalfEdgePair->index] = true;
			tempIndexPoint3 = table[tempHalfEdgePair->face->index][tempHalfEdgePair->index];
			tempIndexPoint4 = table[tempHalfEdgePair->next->face->index][tempHalfEdgePair->next->index];
			subVerticesIndex.push_back(tempIndexPoint1);
			subVerticesIndex.push_back(tempIndexPoint4);
			subVerticesIndex.push_back(tempIndexPoint3);
			subVerticesIndex.push_back(tempIndexPoint2);

			objVertixIndex.push_back(subVerticesIndex);
		}
	}
	for(int i=0; i<faces.size(); ++i)
	{
		free(table[i]);
		free(isEdgePair[i]);
	}
	free(table);
	free(isEdgePair);

	edges.clear();
	faces.clear();
	vertices.clear();
}