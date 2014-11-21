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
using std::exception;
using std::string;
vtkRenderer* getRendererLoadFile(string filePath);
void getNumOfFile(char* pathFirst,char* desPath,int pos,int num);
int main()
{
	// adapt path !
	char filePathFirst[] = 
		"D:/liver segmentation/data/training/liver-seg001.mhd";
	int lenOfPath = strlen(filePathFirst);
	const int numOfFile = 6000;
	vtkRenderer* rendererArray[numOfFile];
	for (int i = 0; i < numOfFile; i++)
	{
		char* filePathTemp  = new char[lenOfPath+1];
		getNumOfFile(filePathFirst,filePathTemp,lenOfPath-5,i);
		
		
		rendererArray[i] = getRendererLoadFile(filePathTemp);
		rendererArray[i]->SetViewport((i%4)/4.0,(i/4)/4.0,((i%4)/4.0+0.25),(i/4)/4.0+0.5);
		
	}

	vtkRenderWindow *renWin = vtkRenderWindow::New(); 
	for (int i =0;i < numOfFile; i++)
	{
		renWin->AddRenderer(rendererArray[i]);
	}
	 
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();  
	iren->SetRenderWindow(renWin);  

	

	renWin->SetSize(1200,600);  
	renWin->Render();  
	iren->Start();  

	/*image->Delete();  
	cubeMapper->Delete();  
	cubeActor->Delete();  
	camera->Delete();  */
	//renderer->Delete();  
	renWin->Delete();  
	iren->Delete();  
	return 0;
}

vtkRenderer* getRendererLoadFile(string filePath)
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

	//auto image = vtkImageData::New();
	//image->SetDimensions(30, 30, 30);  
	//image->SetSpacing(5, 5, 5);  
	//image->SetOrigin(0, 0, 0);  
	//
	////image->SetNumberOfScalarComponents(1);  
	//image->AllocateScalars(VTK_INT,1);
	//int *ptr = (int *)image->GetScalarPointer();  

	//for(int i=0; i<30*30*30; i++)  
	//{  
	//	if(i%9 == 0)  
	//		*ptr++ = 0;  
	//	else  
	//		*ptr++ = 1;  
	//}  

	vtkMarchingCubes *iso = vtkMarchingCubes::New();  
	iso->SetInputData(image);  
	iso->SetNumberOfContours(1);  
	iso->SetValue(0,1);  
	iso->ComputeGradientsOn();  
	iso->ComputeNormalsOff();  
	iso->ComputeScalarsOff();  

	vtkVectorNorm *gradiant = vtkVectorNorm::New();  
	gradiant->SetInputConnection(iso->GetOutputPort());  

	vtkDataSetMapper *cubeMapper = vtkDataSetMapper::New();  
	cubeMapper->SetInputConnection(gradiant->GetOutputPort());  

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