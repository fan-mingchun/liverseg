			#include <vtkSmartPointer.h>  
#include <vtkRenderer.h>  
#include <vtkRenderWindow.h>  
#include <vtkRenderWindowInteractor.h>  
#include <vtkVolume16Reader.h>  
#include <vtkVolume.h>  
#include <vtkVolumeRayCastMapper.h>  
#include <vtkVolumeRayCastCompositeFunction.h>  
#include <vtkVolumeProperty.h>  
#include <vtkColorTransferFunction.h>  
#include <vtkPiecewiseFunction.h>  
#include <vtkCamera.h>  
#include<vtkJPEGReader.h>  
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkPlane.h>  
#include <vtkCutter.h>  
#include <vtkProperty.h>  
#include <vtkActor.h>  
#include <vtkPolyDataMapper.h>  
#include <vtkImageData.h>  
#include <vtkImageMapToColors.h>  
#include <vtkLookupTable.h>  
#include <vtkImageActor.h>  
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
 #include "vtkMarchingCubes.h"  
#include "vtkStripper.h"  
#include "vtkPolyDataMapper.h" 
#include "vtkCamera.h"  
#include"vtkImageSlice.h"
#include "vtkImageSliceMapper.h"
int main (int argc, char *argv[]) 
{ 
	 // adapt path !
   std::string filePath = "D:/liver segmentation/data/training/liver-seg002.mhd";
   std::string filePathRaw = "D:/liver segmentation/data/training/liver-seg002.raw";
   // Create an image
   vtkSmartPointer<vtkImageMandelbrotSource> source =
      vtkSmartPointer<vtkImageMandelbrotSource>::New();
 
   /*vtkSmartPointer<vtkImageCast> castFilter = 
      vtkSmartPointer<vtkImageCast>::New();
   castFilter->SetOutputScalarTypeToUnsignedChar();
   castFilter->SetInputConnection(source->GetOutputPort());
   castFilter->Update();
 
   vtkSmartPointer<vtkMetaImageWriter> writer =
      vtkSmartPointer<vtkMetaImageWriter>::New();
   writer->SetInputConnection(castFilter->GetOutputPort());
   writer->SetFileName(filePath.c_str());
   writer->SetRAWFileName(filePathRaw.c_str());
   writer->Write();
 */
   // Read and display file for verification that it was written correctly
   vtkSmartPointer<vtkMetaImageReader> v16 = 
      vtkSmartPointer<vtkMetaImageReader>::New();
   v16->SetFileName(filePath.c_str());
   v16->Update();
   auto imageData = v16->GetOutput();
    //用filter处理把有符号的short转换为无符号的short
    vtkSmartPointer<vtkImageCast> castFilter = 
      vtkSmartPointer<vtkImageCast>::New();
   castFilter->SetOutputScalarTypeToUnsignedShort();
   castFilter->SetInputData(imageData);
   castFilter->Update();
   imageData= castFilter->GetOutput();
   v16->SetOutput(imageData);
   //v16->Update();

 
  vtkSmartPointer<vtkRenderer> ren =     vtkSmartPointer<vtkRenderer>::New(); 
  vtkSmartPointer<vtkRenderWindow> renWin =    vtkSmartPointer<vtkRenderWindow>::New(); 
  renWin->AddRenderer(ren); 
  vtkSmartPointer<vtkRenderWindowInteractor> iren =    vtkSmartPointer<vtkRenderWindowInteractor>::New(); 
  iren->SetRenderWindow(renWin); 



  vtkSmartPointer<vtkVolumeRayCastCompositeFunction> rayCastFunction =     vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New(); 

  vtkSmartPointer<vtkVolumeRayCastMapper> volumeMapper =     vtkSmartPointer<vtkVolumeRayCastMapper>::New(); 
  volumeMapper->SetInputData(v16->GetOutput()); 
  volumeMapper->SetVolumeRayCastFunction(rayCastFunction); 
  vtkSmartPointer<vtkColorTransferFunction>volumeColor = 
    vtkSmartPointer<vtkColorTransferFunction>::New(); 
  volumeColor->AddRGBPoint(0,    0.0, 0.0, 0.0); 
  volumeColor->AddRGBPoint(100,  1.0, 0.5, 0.3); 
  volumeColor->AddRGBPoint(200, 1.0, 1.0, 0.9); 
  vtkSmartPointer<vtkPiecewiseFunction> volumeScalarOpacity = 
    vtkSmartPointer<vtkPiecewiseFunction>::New(); 
  volumeScalarOpacity->AddPoint(0,    0.00); 
  volumeScalarOpacity->AddPoint(100,  0.15); 
  volumeScalarOpacity->AddPoint(200, 0.85); 

  vtkSmartPointer<vtkPiecewiseFunction> volumeGradientOpacity = 
    vtkSmartPointer<vtkPiecewiseFunction>::New(); 
  volumeGradientOpacity->AddPoint(0,   0.0); 
  volumeGradientOpacity->AddPoint(90,  0.5); 
  volumeGradientOpacity->AddPoint(100, 1.0); 

  vtkSmartPointer<vtkVolumeProperty> volumeProperty = 
    vtkSmartPointer<vtkVolumeProperty>::New(); 
  volumeProperty->SetColor(volumeColor); 
  volumeProperty->SetScalarOpacity(volumeScalarOpacity); 
  volumeProperty->SetGradientOpacity(volumeGradientOpacity); 
  volumeProperty->SetInterpolationTypeToLinear(); 
  volumeProperty->ShadeOn(); 
  volumeProperty->SetAmbient(0.4); 
  volumeProperty->SetDiffuse(0.6); 
  volumeProperty->SetSpecular(0.2); 

  vtkSmartPointer<vtkVolume> volume = 
    vtkSmartPointer<vtkVolume>::New(); 
  volume->SetMapper(volumeMapper); 
  volume->SetProperty(volumeProperty); 


  vtkSmartPointer<vtkLookupTable> bwLut = 
    vtkSmartPointer<vtkLookupTable>::New(); 
  bwLut->SetTableRange (0, 2000); 
  bwLut->SetSaturationRange (0, 0); 
  bwLut->SetHueRange (0, 0); 
  bwLut->SetValueRange (0, 1); 
  bwLut->Build(); //effective built 

  vtkSmartPointer<vtkLookupTable> hueLut = 
    vtkSmartPointer<vtkLookupTable>::New(); 
  hueLut->SetTableRange (0, 2000); 
  hueLut->SetHueRange (0, 1); 
  hueLut->SetSaturationRange (1, 1); 
  hueLut->SetValueRange (1, 1); 
  hueLut->Build(); //effective built 

  vtkSmartPointer<vtkLookupTable> satLut = 
    vtkSmartPointer<vtkLookupTable>::New(); 
  satLut->SetTableRange (0, 2000); 
  satLut->SetHueRange (.6, .6); 
  satLut->SetSaturationRange (0, 1); 
  satLut->SetValueRange (1, 1); 
  satLut->Build(); //effective built 

  vtkSmartPointer<vtkImageMapToColors> sagittalColors =     vtkSmartPointer<vtkImageMapToColors>::New(); 
  sagittalColors->SetInputConnection(v16->GetOutputPort()); 
  sagittalColors->SetLookupTable(bwLut); 
  sagittalColors->Update(); 

  vtkSmartPointer<vtkImageActor> sagittal =     vtkSmartPointer<vtkImageActor>::New(); 
  sagittal->SetInputData(sagittalColors->GetOutput()); 
  sagittal->SetDisplayExtent(117,117,0,173,1,180); 
  vtkSmartPointer<vtkImageMapToColors> axialColors = 
    vtkSmartPointer<vtkImageMapToColors>::New(); 
  axialColors->SetInputConnection(v16->GetOutputPort()); 
  axialColors->SetLookupTable(hueLut); 
  axialColors->Update(); 

  vtkSmartPointer<vtkImageActor> axial = 
    vtkSmartPointer<vtkImageActor>::New(); 
  axial->SetInputData(axialColors->GetOutput()); 
  axial->SetDisplayExtent(0,236,0,173,90,90); 

  // Create the third (coronal) plane of the three planes. We use 
  // the same approach as before except that the extent differs. 
  vtkSmartPointer<vtkImageMapToColors> coronalColors = 
    vtkSmartPointer<vtkImageMapToColors>::New(); 
  coronalColors->SetInputConnection(v16->GetOutputPort()); 
  coronalColors->SetLookupTable(satLut); 
  coronalColors->Update(); 

  vtkSmartPointer<vtkImageActor> coronal = 
    vtkSmartPointer<vtkImageActor>::New(); 
  coronal->SetInputData(coronalColors->GetOutput()); 
  coronal->SetDisplayExtent(0,236,87,87,1,180); 

  ren->AddViewProp(volume); 

  ren->AddActor(sagittal); 
  ren->AddActor(axial); 
  ren->AddActor(coronal); 

  vtkCamera *camera = ren->GetActiveCamera(); 
  double *c = volume->GetCenter(); 
  camera->SetFocalPoint(c[0], c[1], c[2]); 
  camera->SetPosition(c[0] + 400, c[1], c[2]); 
  camera->SetViewUp(0, 0, -1); 

  ren->SetActiveCamera(camera); 
  
  renWin->Render(); 
  
 ren->ResetCamera (); 
  camera->Dolly(1.5); 
  
 ren->ResetCameraClippingRange (); 

  renWin->SetSize(640, 480); 
  iren->Initialize(); 
  iren->Start(); 

  return EXIT_SUCCESS; 
} 