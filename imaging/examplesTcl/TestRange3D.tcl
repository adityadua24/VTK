catch {load vtktcl}
source vtkImageInclude.tcl

# Image pipeline

vtkImageReader reader
reader ReleaseDataFlagOff
reader SetDataByteOrderToLittleEndian
reader SetDataExtent 0 255 0 255 1 93
reader SetFilePrefix "../../../vtkdata/fullHead/headsq"
reader SetDataMask 0x7fff
#reader DebugOn

vtkImageRange3D range
range SetInput [reader GetOutput]
range SetKernelSize 5 5 5

vtkImageViewer viewer
viewer SetInput [range GetOutput]
viewer SetZSlice 22
viewer SetColorWindow 1000
viewer SetColorLevel 500
#viewer DebugOn


source WindowLevelInterface.tcl


