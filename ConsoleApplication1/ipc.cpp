#include <vtkSmartPointer.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkLandmarkTransform.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <fstream>

//#pragma comment(lib, "vtkPolyData.lib")
int main(int argc, char *argv[])
{
	ifstream input("D:/liver segmentation/data/ssmseg/1.txt");
	vtkPoints *points = vtkPoints::New();
	vtkCellArray *polys = vtkCellArray::New();
	vtkPolyData *polyData = vtkPolyData::New();

	vtkPoints *points1 = vtkPoints::New();
	vtkCellArray *polys1 = vtkCellArray::New();
	vtkPolyData *polyData1 = vtkPolyData::New();
	int i = 0;
	float p[4][3] = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
	float p1[4][3] = {{1,0,0},{2,0,0},{1,1,0},{1,0,1}};
	float x,y,z;
	for(i = 0 ;i<4;i++)
	{
		points->InsertPoint(i,p[i]);
		points1->InsertPoint(i,p1[i]);
		
	}  
	int pts[4][3] = {{0,1,3},{0,1,2},{0,2,3},{1,2,3}};
	int pts1[4][3] = {{0,0,0},{0,0,0},{1,0,0},{1,2,3}};
	for(i = 0; i<4;i++)
	{
		polys->InsertNextCell(3,pts[i]);
		polys1->InsertNextCell(3,pts1[i]);
	}  
	polyData->SetPoints(points1);
	polyData1->SetPoints(points);
	polyData->SetPolys(polys);
	polyData1->SetPolys(polys1);
	cout<<polyData->GetNumberOfPoints()<<endl;
	cout<<polyData->GetNumberOfPolys()<<endl;
	vtkSmartPointer<vtkIterativeClosestPointTransform> icp = 
		vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	icp->SetSource(polyData);
	icp->SetTarget(polyData1);
	icp->GetLandmarkTransform()->SetModeToRigidBody();
	//icp->DebugOn();
	icp->SetMaximumNumberOfIterations(20);
	icp->StartByMatchingCentroidsOn();
	icp->Modified();
	icp->Update();

	//get the resulting transformation matrix (this matrix takes the source points to the target points)
	vtkSmartPointer<vtkMatrix4x4> M = icp->GetMatrix();
	std::cout << "The resulting matrix is: " << *M << std::cout;
	return EXIT_SUCCESS;
}