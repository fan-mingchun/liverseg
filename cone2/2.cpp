// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// headers needed for this example
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
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL);
int main(int, char *[])
{
	// adapt path !
	std::string filePath = 
		"D:/liver segmentation/data/training/liver-seg002.mhd";
	std::string filePathRaw = 
		"D:/liver segmentation/data/training/liver-seg002.raw";
	//vtkSmartPointer<vtkMetaImageWriter> writer =
	//   vtkSmartPointer<vtkMetaImageWriter>::New();
	//writer->SetInputConnection(castFilter->GetOutputPort());
	//writer->SetFileName(filePath.c_str());
	//writer->SetRAWFileName(filePathRaw.c_str());
	//writer->Write();

	// Read and display file for verification that it was written correctly
	vtkSmartPointer<vtkMetaImageReader> reader = 
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(filePath.c_str());
	reader->Update();

	auto imageData = reader->GetOutput();
	vtkSmartPointer<vtkImageCast> castFilter = 
		vtkSmartPointer<vtkImageCast>::New();
	/*用filter处理把有符号的short转换为无符号的short*/
	castFilter->SetOutputScalarTypeToUnsignedChar();
	castFilter->SetInputData(imageData);
	auto a =imageData->GetScalarPointer();
	auto b = imageData->GetPointData();
	auto c1 = (unsigned char*)b;
	auto d = c1[0];

	castFilter->Update();
	imageData= castFilter->GetOutput();
	//datamapper
	vtkVolumeRayCastCompositeFunction* compositeFunction = vtkVolumeRayCastCompositeFunction::New();
	vtkVolumeRayCastMapper* volumeMapper = vtkVolumeRayCastMapper::New();
	volumeMapper->SetInputData(imageData);
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);

	//volume,the same pos of actor
	vtkVolume* volume = vtkVolume::New();
	volume->SetMapper(volumeMapper);
	//设置volume的属性
	//设定体数据中不同标量值的透明度
	vtkPiecewiseFunction *opacityTransferFunction = 
		vtkPiecewiseFunction::New();


	opacityTransferFunction->AddPoint(0,0);
	opacityTransferFunction->AddPoint(1,.1);

	vtkColorTransferFunction  *colorTransferFunction=
		vtkColorTransferFunction::New(); 
	colorTransferFunction->AddRGBPoint(0,0.5,0,0); 
	/*colorTransferFunction->AddRGBPoint(64,1.0,0,0); 
	co  lorTransferFunction->AddRGBPoint(128,0.0,0,1.0);
	colorTransferFunction->AddRGBPoint(192,0.0,1.0,0.0); 
	colorTransferFunction->AddRGBPoint(255,0.0,0.2,0.0);*/
	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New(); 

	volumeProperty->SetScalarOpacity(opacityTransferFunction); 
	volumeProperty->SetColor(colorTransferFunction);
	volume->SetProperty(volumeProperty);

	/*	 vtkSmartPointer<vtkImageActor> actor =
	vtkSmartPointer<vtkImageActor>::New();*/

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetSize(500,500);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);



	renderer->AddViewProp(volume);
	renderer->SetBackground(.2, .3, .4);
	vtkCamera *camera = renderer->GetActiveCamera(); 
	double *c = volume->GetCenter(); 
	camera->SetFocalPoint(c[0], c[1], c[2]); 
	camera->SetPosition(c[0] + 400, c[1], c[2]); 
	camera->SetViewUp(0, 0, -1); 

	renderer->SetActiveCamera(camera); 
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}