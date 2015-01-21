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
using std::exception;
using std::string;
vtkRenderer* getRendererLoadFile(string filePath,bool isSmooth);
void getNumOfFile(char* pathFirst,char* desPath,int pos,int num);
int main()
{
	char filePathFirst[] = 
		"D:/liver segmentation/data/ssmseg/1.mhd";
	char filePathFirst2[] = 
		"D:/liver segmentation/data/ssmseg/2.mhd";
	auto renderer1= getRendererLoadFile(filePathFirst,false);
	auto renderer2=getRendererLoadFile(filePathFirst2,true);

	renderer1->SetViewport(0,0,0.5,1);
	renderer2->SetViewport(0.5,0,1,1);
	vtkRenderWindow *renWin = vtkRenderWindow::New(); 
	renWin->AddRenderer(renderer1);
	renWin->AddRenderer(renderer2);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();  
	iren->SetRenderWindow(renWin);  
	renWin->SetSize(1200,600);  
	renWin->Render();  
	iren->Start();   
	renWin->Delete();  
	iren->Delete();  
	return 0;
}

vtkRenderer* getRendererLoadFile(string filePath ,bool isSmooth)
{
	vtkSmartPointer<vtkMetaImageReader> reader = 
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.c_str());
	reader->Update();
	auto imageData = reader->GetOutput();
	vtkSmartPointer<vtkImageCast> castFilter = 
		vtkSmartPointer<vtkImageCast>::New();
	/*用filter处理把有符号的short转换为无符号的short*/
	castFilter->SetOutputScalarTypeToUnsignedInt();
	castFilter->SetInputData(imageData);
	castFilter->Update();
	imageData= castFilter->GetOutput();
	auto image = imageData;
	vtkMarchingCubes *iso = vtkMarchingCubes::New();  
	iso->SetInputData(image);  
	iso->SetNumberOfContours(1);  
	iso->SetValue(0,50);  
	iso->ComputeGradientsOn();  
	iso->ComputeNormalsOff();
	//iso->ComputeNormalsOn();
	//iso->ComputeScalarsOff();  
	iso->ComputeScalarsOn();
	iso->Update();

	auto polyData = iso->GetOutput();


	//auto num11= polyData->GetNumberOfPolys();
	//cout<<endl;
	//cout<<"poly:GetNumberOfPolys(): "<<num11;
	//auto num00= polyData->GetNumberOfPoints();
	//cout<<endl;
	//cout<<"poly:>GetNumberOfPoints "<<num00;
	///*auto cleanPoly = vtkCleanPolyData::New();
	//cleanPoly->SetInputData(polyData);
	//cleanPoly->SetTolerance(0.5);
	//cleanPoly->Update();*/
	//num11= polyData->GetNumberOfPolys();

	////ofstream ouput((filePath+".txt").c_str());
	////for(int i = 0;i < polyData->GetNumberOfPoints();++i)
	////{
	////	double* p =polyData->GetPoint(i);
	////	//if(!(i%10))
	////	ouput<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
	////}
	//cout<<endl;
	//cout<<"poly:GetNumberOfPolys()-post: "<<num11;
	//num00= polyData->GetNumberOfPoints();
	//cout<<endl;
	//cout<<"poly:>GetNumberOfPoints-post "<<num00;
	//cout<<endl;
	//vtkSmoothPolyDataFilter *smooth=vtkSmoothPolyDataFilter::New();
	//smooth->SetInputData(polyData);
	//smooth->Update();
	//vtkVectorNorm *gradiant = vtkVectorNorm::New();  
	vtkPolyDataMapper *cubeMapper = vtkPolyDataMapper ::New();  

	cubeMapper->SetInputData(polyData); 
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
	return renderer;
}
//默认文件名从1开始，num从0开始
void getNumOfFile(char* pathFirst,char* desPath,int pos,int num)
{
	if(pathFirst == nullptr || desPath ==nullptr)
	{
		throw new std::runtime_error("Input is invalid,@getNumOfFile");
	}
	++num;
	char* tempPath = new char[strlen(pathFirst)+1];
	strcpy(tempPath,pathFirst);
	char numString[10];
	itoa(num,numString,10);
	int len=strlen(numString);
	for(int i = 0;i < len;i++)
	{
		char* curLetter = &tempPath[pos-i];
		if(curLetter < tempPath)
			throw new std::runtime_error("Num is too big,@getNumOfFile");
		*curLetter = numString[len-i-1];
	}
	strcpy(desPath,tempPath);
	delete []tempPath;

}
void simpliedPolyData(vtkPolyData* polydata)
{

}