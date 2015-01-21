#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>


#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>
#include <vtkCamera.h>  
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>
#include<vtkPiecewiseFunction.h> 
#include<vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkMarchingCubes.h>
#include <vtkVectorNorm.h>
#include <vtkDataSetMapper.h>
#include <exception>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkCleanPolyData.h>

#include <vtkDelaunay3D.h>
#include <vtkDelaunay2D.h>
#include <vtkGeometryFilter.h>
#include <vtkDataSetSurfaceFilter.h>
//#pragma comment(lib, "vtkPolyData.lib")
int main(int argc, char *argv[])
{
	ifstream input("D:/liver segmentation/data/ssmseg/7.txt");
	auto points = vtkPoints::New();
	auto pointset = vtkPolyData::New();
	int i = 0;

	float array[3];
	while (input>>array[0]>>array[1]>>array[2])

	{
		points->InsertPoint(i++,array);


	}  
	pointset->SetPoints(points);
	auto delaunay = vtkDelaunay3D::New();
	delaunay->SetInputData(pointset);
	delaunay->Update();


	auto *polyData = delaunay->GetOutput();
	auto geometryFilter = vtkDataSetSurfaceFilter::New();
	geometryFilter->SetInputData((vtkDataObject*)polyData);
	geometryFilter->Update();


	 auto *cubeMapper =  vtkPolyDataMapper::New();  
	cubeMapper->SetInputData((vtkPolyData*)geometryFilter->GetInput());
	vtkActor *cubeActor = vtkActor::New();  
	cubeActor->SetMapper(cubeMapper);  
	vtkCamera *camera = vtkCamera::New();  
	camera->SetPosition(1,1,1);  
	camera->SetFocalPoint(0,0,0);  
	vtkRenderer *renderer = vtkRenderer::New();  
	renderer->AddActor(cubeActor);  
	renderer->SetActiveCamera(camera);  
	renderer->ResetCamera();  
	renderer->SetBackground(1,1,1);
	vtkRenderWindow *renWin = vtkRenderWindow::New(); 
	renWin->AddRenderer(renderer);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();  
	iren->SetRenderWindow(renWin);  
	renWin->SetSize(1200,600);  
	renWin->Render();  
	iren->Start();  
	return EXIT_SUCCESS;
}