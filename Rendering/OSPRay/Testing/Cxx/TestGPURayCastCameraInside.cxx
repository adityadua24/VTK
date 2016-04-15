/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestGPURayCastCameraInside.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// Description
// This is a test for clipping of volume using the camera near plane when the
// camera is inside the volume. The test renders the ironProt dataset and
// dollies the camera inside the dataset.

#include "vtkColorTransferFunction.h"
#include "vtkGPUVolumeRayCastMapper.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkNew.h"
#include "vtkPiecewiseFunction.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkStructuredPointsReader.h"
#include "vtkTesting.h"
#include "vtkTestUtilities.h"
#include "vtkVolume.h"
#include "vtkVolumeProperty.h"
#include "vtkOSPRayPass.h"
#include "vtkSmartPointer.h"

static const char* TestGPURayCastCameraInsideLog =
"# StreamVersion 1\n"
"EnterEvent 298 27 0 0 0 0 0\n"
"MouseMoveEvent 298 27 0 0 0 0 0\n"
"MouseMoveEvent 289 39 0 0 0 0 0\n"
"MouseMoveEvent 280 52 0 0 0 0 0\n"
"MouseMoveEvent 271 64 0 0 0 0 0\n"
"MouseMoveEvent 263 77 0 0 0 0 0\n"
"MouseMoveEvent 256 88 0 0 0 0 0\n"
"MouseMoveEvent 249 99 0 0 0 0 0\n"
"MouseMoveEvent 240 110 0 0 0 0 0\n"
"MouseMoveEvent 231 121 0 0 0 0 0\n"
"MouseMoveEvent 226 128 0 0 0 0 0\n"
"MouseMoveEvent 219 135 0 0 0 0 0\n"
"MouseMoveEvent 214 142 0 0 0 0 0\n"
"MouseMoveEvent 209 147 0 0 0 0 0\n"
"MouseMoveEvent 205 151 0 0 0 0 0\n"
"MouseMoveEvent 204 152 0 0 0 0 0\n"
"MouseMoveEvent 203 153 0 0 0 0 0\n"
"MouseMoveEvent 202 153 0 0 0 0 0\n"
"MouseMoveEvent 201 152 0 0 0 0 0\n"
"MouseMoveEvent 201 151 0 0 0 0 0\n"
"MouseMoveEvent 201 149 0 0 0 0 0\n"
"MouseMoveEvent 201 148 0 0 0 0 0\n"
"MouseMoveEvent 201 147 0 0 0 0 0\n"
"MouseMoveEvent 201 146 0 0 0 0 0\n"
"MouseMoveEvent 201 145 0 0 0 0 0\n"
"MouseMoveEvent 201 144 0 0 0 0 0\n"
"MouseMoveEvent 200 143 0 0 0 0 0\n"
"MouseMoveEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelForwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 1 0\n"
"StartInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"EndInteractionEvent 200 142 0 0 0 1 0\n"
"RenderEvent 200 142 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 142 0 0 0 0 0\n"
"StartInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"EndInteractionEvent 200 142 0 0 0 0 0\n"
"RenderEvent 200 142 0 0 0 0 0\n"
"MouseMoveEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 1 0\n"
"StartInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"EndInteractionEvent 200 141 0 0 0 1 0\n"
"RenderEvent 200 141 0 0 0 1 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseWheelBackwardEvent 200 141 0 0 0 0 0\n"
"StartInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"EndInteractionEvent 200 141 0 0 0 0 0\n"
"RenderEvent 200 141 0 0 0 0 0\n"
"MouseMoveEvent 198 141 0 0 0 0 0\n"
"MouseMoveEvent 195 144 0 0 0 0 0\n"
"MouseMoveEvent 189 148 0 0 0 0 0\n"
"MouseMoveEvent 185 150 0 0 0 0 0\n"
"MouseMoveEvent 181 152 0 0 0 0 0\n"
"MouseMoveEvent 177 154 0 0 0 0 0\n"
"MouseMoveEvent 174 155 0 0 0 0 0\n"
"MouseMoveEvent 171 159 0 0 0 0 0\n"
"MouseMoveEvent 167 161 0 0 0 0 0\n"
"MouseMoveEvent 163 163 0 0 0 0 0\n"
"MouseMoveEvent 160 164 0 0 0 0 0\n"
"MouseMoveEvent 159 165 0 0 0 0 0\n"
"MouseMoveEvent 158 167 0 0 0 0 0\n"
"MouseMoveEvent 157 170 0 0 0 0 0\n"
"MouseMoveEvent 155 173 0 0 0 0 0\n"
"MouseMoveEvent 154 174 0 0 0 0 0\n"
"MouseMoveEvent 154 175 0 0 0 0 0\n"
"LeftButtonPressEvent 154 175 0 0 0 0 0\n"
"StartInteractionEvent 154 175 0 0 0 0 0\n"
"MouseMoveEvent 153 176 0 0 0 0 0\n"
"RenderEvent 153 176 0 0 0 0 0\n"
"InteractionEvent 153 176 0 0 0 0 0\n"
"MouseMoveEvent 152 175 0 0 0 0 0\n"
"RenderEvent 152 175 0 0 0 0 0\n"
"InteractionEvent 152 175 0 0 0 0 0\n"
"MouseMoveEvent 152 174 0 0 0 0 0\n"
"RenderEvent 152 174 0 0 0 0 0\n"
"InteractionEvent 152 174 0 0 0 0 0\n"
"MouseMoveEvent 152 173 0 0 0 0 0\n"
"RenderEvent 152 173 0 0 0 0 0\n"
"InteractionEvent 152 173 0 0 0 0 0\n"
"MouseMoveEvent 152 172 0 0 0 0 0\n"
"RenderEvent 152 172 0 0 0 0 0\n"
"InteractionEvent 152 172 0 0 0 0 0\n"
"MouseMoveEvent 152 169 0 0 0 0 0\n"
"RenderEvent 152 169 0 0 0 0 0\n"
"InteractionEvent 152 169 0 0 0 0 0\n"
"MouseMoveEvent 152 168 0 0 0 0 0\n"
"RenderEvent 152 168 0 0 0 0 0\n"
"InteractionEvent 152 168 0 0 0 0 0\n"
"MouseMoveEvent 152 165 0 0 0 0 0\n"
"RenderEvent 152 165 0 0 0 0 0\n"
"InteractionEvent 152 165 0 0 0 0 0\n"
"MouseMoveEvent 152 162 0 0 0 0 0\n"
"RenderEvent 152 162 0 0 0 0 0\n"
"InteractionEvent 152 162 0 0 0 0 0\n"
"MouseMoveEvent 152 160 0 0 0 0 0\n"
"RenderEvent 152 160 0 0 0 0 0\n"
"InteractionEvent 152 160 0 0 0 0 0\n"
"MouseMoveEvent 152 156 0 0 0 0 0\n"
"RenderEvent 152 156 0 0 0 0 0\n"
"InteractionEvent 152 156 0 0 0 0 0\n"
"MouseMoveEvent 152 152 0 0 0 0 0\n"
"RenderEvent 152 152 0 0 0 0 0\n"
"InteractionEvent 152 152 0 0 0 0 0\n"
"MouseMoveEvent 152 148 0 0 0 0 0\n"
"RenderEvent 152 148 0 0 0 0 0\n"
"InteractionEvent 152 148 0 0 0 0 0\n"
"MouseMoveEvent 152 144 0 0 0 0 0\n"
"RenderEvent 152 144 0 0 0 0 0\n"
"InteractionEvent 152 144 0 0 0 0 0\n"
"MouseMoveEvent 152 141 0 0 0 0 0\n"
"RenderEvent 152 141 0 0 0 0 0\n"
"InteractionEvent 152 141 0 0 0 0 0\n"
"MouseMoveEvent 152 138 0 0 0 0 0\n"
"RenderEvent 152 138 0 0 0 0 0\n"
"InteractionEvent 152 138 0 0 0 0 0\n"
"MouseMoveEvent 152 134 0 0 0 0 0\n"
"RenderEvent 152 134 0 0 0 0 0\n"
"InteractionEvent 152 134 0 0 0 0 0\n"
"MouseMoveEvent 152 130 0 0 0 0 0\n"
"RenderEvent 152 130 0 0 0 0 0\n"
"InteractionEvent 152 130 0 0 0 0 0\n"
"MouseMoveEvent 152 127 0 0 0 0 0\n"
"RenderEvent 152 127 0 0 0 0 0\n"
"InteractionEvent 152 127 0 0 0 0 0\n"
"MouseMoveEvent 152 123 0 0 0 0 0\n"
"RenderEvent 152 123 0 0 0 0 0\n"
"InteractionEvent 152 123 0 0 0 0 0\n"
"MouseMoveEvent 152 119 0 0 0 0 0\n"
"RenderEvent 152 119 0 0 0 0 0\n"
"InteractionEvent 152 119 0 0 0 0 0\n"
"MouseMoveEvent 151 115 0 0 0 0 0\n"
"RenderEvent 151 115 0 0 0 0 0\n"
"InteractionEvent 151 115 0 0 0 0 0\n"
"MouseMoveEvent 151 113 0 0 0 0 0\n"
"RenderEvent 151 113 0 0 0 0 0\n"
"InteractionEvent 151 113 0 0 0 0 0\n"
"MouseMoveEvent 151 112 0 0 0 0 0\n"
"RenderEvent 151 112 0 0 0 0 0\n"
"InteractionEvent 151 112 0 0 0 0 0\n"
"MouseMoveEvent 151 111 0 0 0 0 0\n"
"RenderEvent 151 111 0 0 0 0 0\n"
"InteractionEvent 151 111 0 0 0 0 0\n"
"MouseMoveEvent 151 110 0 0 0 0 0\n"
"RenderEvent 151 110 0 0 0 0 0\n"
"InteractionEvent 151 110 0 0 0 0 0\n"
"MouseMoveEvent 150 109 0 0 0 0 0\n"
"RenderEvent 150 109 0 0 0 0 0\n"
"InteractionEvent 150 109 0 0 0 0 0\n"
"MouseMoveEvent 150 110 0 0 0 0 0\n"
"RenderEvent 150 110 0 0 0 0 0\n"
"InteractionEvent 150 110 0 0 0 0 0\n"
"MouseMoveEvent 150 111 0 0 0 0 0\n"
"RenderEvent 150 111 0 0 0 0 0\n"
"InteractionEvent 150 111 0 0 0 0 0\n"
"MouseMoveEvent 150 112 0 0 0 0 0\n"
"RenderEvent 150 112 0 0 0 0 0\n"
"InteractionEvent 150 112 0 0 0 0 0\n"
"MouseMoveEvent 149 114 0 0 0 0 0\n"
"RenderEvent 149 114 0 0 0 0 0\n"
"InteractionEvent 149 114 0 0 0 0 0\n"
"MouseMoveEvent 148 118 0 0 0 0 0\n"
"RenderEvent 148 118 0 0 0 0 0\n"
"InteractionEvent 148 118 0 0 0 0 0\n"
"MouseMoveEvent 148 122 0 0 0 0 0\n"
"RenderEvent 148 122 0 0 0 0 0\n"
"InteractionEvent 148 122 0 0 0 0 0\n"
"MouseMoveEvent 148 125 0 0 0 0 0\n"
"RenderEvent 148 125 0 0 0 0 0\n"
"InteractionEvent 148 125 0 0 0 0 0\n"
"MouseMoveEvent 148 127 0 0 0 0 0\n"
"RenderEvent 148 127 0 0 0 0 0\n"
"InteractionEvent 148 127 0 0 0 0 0\n"
"LeftButtonReleaseEvent 148 127 0 0 0 0 0\n"
"EndInteractionEvent 148 127 0 0 0 0 0\n"
"RenderEvent 148 127 0 0 0 0 0\n"
"MouseMoveEvent 147 128 0 0 0 0 0\n"
"MouseMoveEvent 146 127 0 0 0 0 0\n"
"MouseMoveEvent 146 126 0 0 0 0 0\n"
"MouseMoveEvent 146 125 0 0 0 0 0\n"
"MouseMoveEvent 146 124 0 0 0 0 0\n"
"MouseMoveEvent 146 123 0 0 0 0 0\n"
"MouseMoveEvent 146 122 0 0 0 0 0\n"
"MouseMoveEvent 146 120 0 0 0 0 0\n"
"MouseMoveEvent 146 119 0 0 0 0 0\n"
"MouseMoveEvent 146 118 0 0 0 0 0\n"
"MouseMoveEvent 146 117 0 0 0 0 0\n"
"MouseMoveEvent 146 116 0 0 0 0 0\n"
"MouseMoveEvent 147 114 0 0 0 0 0\n"
"MouseMoveEvent 148 113 0 0 0 0 0\n"
"MouseWheelForwardEvent 148 113 0 0 0 0 0\n"
"StartInteractionEvent 148 113 0 0 0 0 0\n"
"RenderEvent 148 113 0 0 0 0 0\n"
"EndInteractionEvent 148 113 0 0 0 0 0\n"
"RenderEvent 148 113 0 0 0 0 0\n"
"MouseWheelForwardEvent 148 113 0 0 0 0 0\n"
"StartInteractionEvent 148 113 0 0 0 0 0\n"
"RenderEvent 148 113 0 0 0 0 0\n"
"EndInteractionEvent 148 113 0 0 0 0 0\n"
"RenderEvent 148 113 0 0 0 0 0\n"
"MouseMoveEvent 148 112 0 0 0 0 0\n"
"MouseMoveEvent 148 111 0 0 0 0 0\n"
"MouseMoveEvent 148 110 0 0 0 0 0\n"
"MouseMoveEvent 148 109 0 0 0 0 0\n"
"MouseMoveEvent 148 108 0 0 0 0 0\n"
"MouseMoveEvent 148 107 0 0 0 0 0\n"
"MouseMoveEvent 148 106 0 0 0 0 0\n"
"MouseMoveEvent 148 105 0 0 0 0 0\n"
"MouseMoveEvent 148 103 0 0 0 0 0\n"
"MouseMoveEvent 149 101 0 0 0 0 0\n"
"MouseMoveEvent 149 99 0 0 0 0 0\n"
"MouseMoveEvent 150 98 0 0 0 0 0\n"
"MouseMoveEvent 150 96 0 0 0 0 0\n"
"MouseMoveEvent 151 95 0 0 0 0 0\n"
"MouseMoveEvent 152 93 0 0 0 0 0\n"
"MouseMoveEvent 153 91 0 0 0 0 0\n"
"MouseMoveEvent 155 88 0 0 0 0 0\n"
"MouseMoveEvent 156 87 0 0 0 0 0\n"
"MouseMoveEvent 157 85 0 0 0 0 0\n"
"MouseMoveEvent 159 83 0 0 0 0 0\n"
"MouseMoveEvent 160 81 0 0 0 0 0\n"
"MouseMoveEvent 162 79 0 0 0 0 0\n"
"MouseMoveEvent 163 77 0 0 0 0 0\n"
"MouseMoveEvent 164 76 0 0 0 0 0\n"
"MouseMoveEvent 165 75 0 0 0 0 0\n"
"MouseMoveEvent 166 74 0 0 0 0 0\n"
"MouseMoveEvent 167 72 0 0 0 0 0\n"
"MouseMoveEvent 168 71 0 0 0 0 0\n"
"MouseMoveEvent 169 70 0 0 0 0 0\n"
"MouseMoveEvent 170 69 0 0 0 0 0\n"
"MouseMoveEvent 171 67 0 0 0 0 0\n"
"MouseMoveEvent 172 66 0 0 0 0 0\n"
"MouseMoveEvent 172 65 0 0 0 0 0\n"
"MouseMoveEvent 173 64 0 0 0 0 0\n"
"MouseMoveEvent 174 62 0 0 0 0 0\n"
"MouseMoveEvent 175 60 0 0 0 0 0\n"
"MouseMoveEvent 176 59 0 0 0 0 0\n"
"MouseMoveEvent 177 58 0 0 0 0 0\n"
"MouseMoveEvent 179 57 0 0 0 0 0\n"
"MouseMoveEvent 180 57 0 0 0 0 0\n"
"MouseMoveEvent 182 56 0 0 0 0 0\n"
"MouseMoveEvent 183 56 0 0 0 0 0\n"
"MouseMoveEvent 184 55 0 0 0 0 0\n"
"MouseMoveEvent 185 55 0 0 0 0 0\n"
"MouseMoveEvent 187 55 0 0 0 0 0\n"
"MouseMoveEvent 188 55 0 0 0 0 0\n"
"MouseMoveEvent 189 55 0 0 0 0 0\n"
"MouseMoveEvent 190 55 0 0 0 0 0\n"
"MouseMoveEvent 191 55 0 0 0 0 0\n"
"MouseMoveEvent 193 55 0 0 0 0 0\n"
"MouseMoveEvent 194 55 0 0 0 0 0\n"
"MouseMoveEvent 195 55 0 0 0 0 0\n"
"MouseMoveEvent 196 55 0 0 0 0 0\n"
"MouseMoveEvent 197 55 0 0 0 0 0\n"
"MouseMoveEvent 198 55 0 0 0 0 0\n"
"MouseMoveEvent 199 55 0 0 0 0 0\n"
"LeftButtonPressEvent 199 55 0 0 0 0 0\n"
"StartInteractionEvent 199 55 0 0 0 0 0\n"
"MouseMoveEvent 200 56 0 0 0 0 0\n"
"RenderEvent 200 56 0 0 0 0 0\n"
"InteractionEvent 200 56 0 0 0 0 0\n"
"MouseMoveEvent 200 57 0 0 0 0 0\n"
"RenderEvent 200 57 0 0 0 0 0\n"
"InteractionEvent 200 57 0 0 0 0 0\n"
"MouseMoveEvent 200 58 0 0 0 0 0\n"
"RenderEvent 200 58 0 0 0 0 0\n"
"InteractionEvent 200 58 0 0 0 0 0\n"
"MouseMoveEvent 200 59 0 0 0 0 0\n"
"RenderEvent 200 59 0 0 0 0 0\n"
"InteractionEvent 200 59 0 0 0 0 0\n"
"MouseMoveEvent 199 60 0 0 0 0 0\n"
"RenderEvent 199 60 0 0 0 0 0\n"
"InteractionEvent 199 60 0 0 0 0 0\n"
"MouseMoveEvent 199 62 0 0 0 0 0\n"
"RenderEvent 199 62 0 0 0 0 0\n"
"InteractionEvent 199 62 0 0 0 0 0\n"
"MouseMoveEvent 198 64 0 0 0 0 0\n"
"RenderEvent 198 64 0 0 0 0 0\n"
"InteractionEvent 198 64 0 0 0 0 0\n"
"MouseMoveEvent 198 66 0 0 0 0 0\n"
"RenderEvent 198 66 0 0 0 0 0\n"
"InteractionEvent 198 66 0 0 0 0 0\n"
"MouseMoveEvent 197 67 0 0 0 0 0\n"
"RenderEvent 197 67 0 0 0 0 0\n"
"InteractionEvent 197 67 0 0 0 0 0\n"
"MouseMoveEvent 197 68 0 0 0 0 0\n"
"RenderEvent 197 68 0 0 0 0 0\n"
"InteractionEvent 197 68 0 0 0 0 0\n"
"MouseMoveEvent 197 69 0 0 0 0 0\n"
"RenderEvent 197 69 0 0 0 0 0\n"
"InteractionEvent 197 69 0 0 0 0 0\n"
"LeftButtonReleaseEvent 197 69 0 0 0 0 0\n"
"EndInteractionEvent 197 69 0 0 0 0 0\n"
"RenderEvent 197 69 0 0 0 0 0\n"
"MouseMoveEvent 197 66 0 0 0 0 0\n"
"MouseMoveEvent 197 64 0 0 0 0 0\n"
"MouseMoveEvent 200 61 0 0 0 0 0\n"
"MouseMoveEvent 201 60 0 0 0 0 0\n"
"MouseMoveEvent 202 59 0 0 0 0 0\n"
"MouseMoveEvent 203 57 0 0 0 0 0\n"
"MouseMoveEvent 203 56 0 0 0 0 0\n"
"MouseMoveEvent 204 55 0 0 0 0 0\n"
"MouseMoveEvent 205 53 0 0 0 0 0\n"
"MouseMoveEvent 205 52 0 0 0 0 0\n"
"MouseWheelForwardEvent 205 52 0 0 0 0 0\n"
"StartInteractionEvent 205 52 0 0 0 0 0\n"
"RenderEvent 205 52 0 0 0 0 0\n"
"EndInteractionEvent 205 52 0 0 0 0 0\n"
"RenderEvent 205 52 0 0 0 0 0\n"
"MouseWheelForwardEvent 205 52 0 0 0 0 0\n"
"StartInteractionEvent 205 52 0 0 0 0 0\n"
"RenderEvent 205 52 0 0 0 0 0\n"
"EndInteractionEvent 205 52 0 0 0 0 0\n"
"RenderEvent 205 52 0 0 0 0 0\n"
"MouseMoveEvent 206 51 0 0 0 0 0\n"
"MouseMoveEvent 207 50 0 0 0 0 0\n"
"LeftButtonPressEvent 207 50 0 0 0 0 0\n"
"StartInteractionEvent 207 50 0 0 0 0 0\n"
"MouseMoveEvent 206 51 0 0 0 0 0\n"
"RenderEvent 206 51 0 0 0 0 0\n"
"InteractionEvent 206 51 0 0 0 0 0\n"
"MouseMoveEvent 205 51 0 0 0 0 0\n"
"RenderEvent 205 51 0 0 0 0 0\n"
"InteractionEvent 205 51 0 0 0 0 0\n"
"MouseMoveEvent 204 52 0 0 0 0 0\n"
"RenderEvent 204 52 0 0 0 0 0\n"
"InteractionEvent 204 52 0 0 0 0 0\n"
"MouseMoveEvent 203 52 0 0 0 0 0\n"
"RenderEvent 203 52 0 0 0 0 0\n"
"InteractionEvent 203 52 0 0 0 0 0\n"
"MouseMoveEvent 201 52 0 0 0 0 0\n"
"RenderEvent 201 52 0 0 0 0 0\n"
"InteractionEvent 201 52 0 0 0 0 0\n"
"MouseMoveEvent 199 54 0 0 0 0 0\n"
"RenderEvent 199 54 0 0 0 0 0\n"
"InteractionEvent 199 54 0 0 0 0 0\n"
"MouseMoveEvent 197 56 0 0 0 0 0\n"
"RenderEvent 197 56 0 0 0 0 0\n"
"InteractionEvent 197 56 0 0 0 0 0\n"
"MouseMoveEvent 196 57 0 0 0 0 0\n"
"RenderEvent 196 57 0 0 0 0 0\n"
"InteractionEvent 196 57 0 0 0 0 0\n"
"MouseMoveEvent 195 58 0 0 0 0 0\n"
"RenderEvent 195 58 0 0 0 0 0\n"
"InteractionEvent 195 58 0 0 0 0 0\n"
"MouseMoveEvent 194 59 0 0 0 0 0\n"
"RenderEvent 194 59 0 0 0 0 0\n"
"InteractionEvent 194 59 0 0 0 0 0\n"
"MouseMoveEvent 192 61 0 0 0 0 0\n"
"RenderEvent 192 61 0 0 0 0 0\n"
"InteractionEvent 192 61 0 0 0 0 0\n"
"MouseMoveEvent 191 62 0 0 0 0 0\n"
"RenderEvent 191 62 0 0 0 0 0\n"
"InteractionEvent 191 62 0 0 0 0 0\n"
"MouseMoveEvent 190 63 0 0 0 0 0\n"
"RenderEvent 190 63 0 0 0 0 0\n"
"InteractionEvent 190 63 0 0 0 0 0\n"
"MouseMoveEvent 189 64 0 0 0 0 0\n"
"RenderEvent 189 64 0 0 0 0 0\n"
"InteractionEvent 189 64 0 0 0 0 0\n"
"MouseMoveEvent 189 65 0 0 0 0 0\n"
"RenderEvent 189 65 0 0 0 0 0\n"
"InteractionEvent 189 65 0 0 0 0 0\n"
"MouseMoveEvent 188 66 0 0 0 0 0\n"
"RenderEvent 188 66 0 0 0 0 0\n"
"InteractionEvent 188 66 0 0 0 0 0\n"
"MouseMoveEvent 188 67 0 0 0 0 0\n"
"RenderEvent 188 67 0 0 0 0 0\n"
"InteractionEvent 188 67 0 0 0 0 0\n"
"MouseMoveEvent 187 68 0 0 0 0 0\n"
"RenderEvent 187 68 0 0 0 0 0\n"
"InteractionEvent 187 68 0 0 0 0 0\n"
"MouseMoveEvent 187 69 0 0 0 0 0\n"
"RenderEvent 187 69 0 0 0 0 0\n"
"InteractionEvent 187 69 0 0 0 0 0\n"
"MouseMoveEvent 186 70 0 0 0 0 0\n"
"RenderEvent 186 70 0 0 0 0 0\n"
"InteractionEvent 186 70 0 0 0 0 0\n"
"LeftButtonReleaseEvent 186 70 0 0 0 0 0\n"
"EndInteractionEvent 186 70 0 0 0 0 0\n"
"RenderEvent 186 70 0 0 0 0 0\n"
"MouseMoveEvent 187 70 0 0 0 0 0\n"
"MouseMoveEvent 188 70 0 0 0 0 0\n"
"MouseMoveEvent 189 70 0 0 0 0 0\n"
"MouseMoveEvent 190 70 0 0 0 0 0\n"
"MouseMoveEvent 192 69 0 0 0 0 0\n"
"MouseMoveEvent 194 68 0 0 0 0 0\n"
"MouseMoveEvent 196 67 0 0 0 0 0\n"
"MouseMoveEvent 198 66 0 0 0 0 0\n"
"MouseMoveEvent 202 62 0 0 0 0 0\n"
"MouseMoveEvent 204 60 0 0 0 0 0\n"
"MouseMoveEvent 206 58 0 0 0 0 0\n"
"MouseMoveEvent 208 56 0 0 0 0 0\n"
"MouseMoveEvent 209 55 0 0 0 0 0\n"
"MouseMoveEvent 210 54 0 0 0 0 0\n"
"MouseMoveEvent 212 52 0 0 0 0 0\n"
"MouseMoveEvent 213 51 0 0 0 0 0\n"
"MouseMoveEvent 214 49 0 0 0 0 0\n"
"MouseMoveEvent 216 47 0 0 0 0 0\n"
"MouseMoveEvent 219 43 0 0 0 0 0\n"
"MouseMoveEvent 222 40 0 0 0 0 0\n"
"MouseMoveEvent 227 35 0 0 0 0 0\n"
"MouseMoveEvent 232 30 0 0 0 0 0\n"
"MouseMoveEvent 240 22 0 0 0 0 0\n"
"MouseMoveEvent 247 15 0 0 0 0 0\n"
"MouseMoveEvent 255 7 0 0 0 0 0\n"
"LeaveEvent 265 -1 0 0 0 0 0\n"
"EnterEvent 273 0 0 0 0 0 0\n"
"MouseMoveEvent 273 0 0 0 0 0 0\n"
"MouseMoveEvent 271 4 0 0 0 0 0\n"
"MouseMoveEvent 270 5 0 0 0 0 0\n"
"MouseMoveEvent 269 7 0 0 0 0 0\n"
"MouseMoveEvent 268 9 0 0 0 0 0\n"
"MouseMoveEvent 267 11 0 0 0 0 0\n"
"MouseMoveEvent 266 12 0 0 0 0 0\n"
"MouseMoveEvent 265 14 0 0 0 0 0\n"
"MouseMoveEvent 264 16 0 0 0 0 0\n"
"MouseMoveEvent 262 20 0 0 0 0 0\n"
"MouseMoveEvent 261 21 0 0 0 0 0\n"
"MouseMoveEvent 260 23 0 0 0 0 0\n"
"MouseMoveEvent 258 26 0 0 0 0 0\n"
"MouseMoveEvent 258 27 0 0 0 0 0\n"
"MouseMoveEvent 257 29 0 0 0 0 0\n"
"MouseMoveEvent 256 30 0 0 0 0 0\n"
"MouseMoveEvent 255 32 0 0 0 0 0\n"
"MouseMoveEvent 254 33 0 0 0 0 0\n"
"MouseMoveEvent 253 34 0 0 0 0 0\n"
"MouseMoveEvent 253 35 0 0 0 0 0\n"
"MouseMoveEvent 252 37 0 0 0 0 0\n"
"MouseMoveEvent 251 38 0 0 0 0 0\n"
"MouseMoveEvent 250 39 0 0 0 0 0\n"
"MouseMoveEvent 249 42 0 0 0 0 0\n"
"MouseMoveEvent 249 43 0 0 0 0 0\n"
"MouseMoveEvent 248 44 0 0 0 0 0\n"
"MouseWheelForwardEvent 248 44 0 0 0 0 0\n"
"StartInteractionEvent 248 44 0 0 0 0 0\n"
"RenderEvent 248 44 0 0 0 0 0\n"
"EndInteractionEvent 248 44 0 0 0 0 0\n"
"RenderEvent 248 44 0 0 0 0 0\n"
"LeftButtonPressEvent 248 44 0 0 0 0 0\n"
"StartInteractionEvent 248 44 0 0 0 0 0\n"
"MouseMoveEvent 246 45 0 0 0 0 0\n"
"RenderEvent 246 45 0 0 0 0 0\n"
"InteractionEvent 246 45 0 0 0 0 0\n"
"MouseMoveEvent 245 46 0 0 0 0 0\n"
"RenderEvent 245 46 0 0 0 0 0\n"
"InteractionEvent 245 46 0 0 0 0 0\n"
"MouseMoveEvent 244 47 0 0 0 0 0\n"
"RenderEvent 244 47 0 0 0 0 0\n"
"InteractionEvent 244 47 0 0 0 0 0\n"
"MouseMoveEvent 243 49 0 0 0 0 0\n"
"RenderEvent 243 49 0 0 0 0 0\n"
"InteractionEvent 243 49 0 0 0 0 0\n"
"MouseMoveEvent 242 50 0 0 0 0 0\n"
"RenderEvent 242 50 0 0 0 0 0\n"
"InteractionEvent 242 50 0 0 0 0 0\n"
"MouseMoveEvent 241 52 0 0 0 0 0\n"
"RenderEvent 241 52 0 0 0 0 0\n"
"InteractionEvent 241 52 0 0 0 0 0\n"
"MouseMoveEvent 240 53 0 0 0 0 0\n"
"RenderEvent 240 53 0 0 0 0 0\n"
"InteractionEvent 240 53 0 0 0 0 0\n"
"MouseMoveEvent 240 54 0 0 0 0 0\n"
"RenderEvent 240 54 0 0 0 0 0\n"
"InteractionEvent 240 54 0 0 0 0 0\n"
"MouseMoveEvent 238 57 0 0 0 0 0\n"
"RenderEvent 238 57 0 0 0 0 0\n"
"InteractionEvent 238 57 0 0 0 0 0\n"
"MouseMoveEvent 238 58 0 0 0 0 0\n"
"RenderEvent 238 58 0 0 0 0 0\n"
"InteractionEvent 238 58 0 0 0 0 0\n"
"MouseMoveEvent 237 59 0 0 0 0 0\n"
"RenderEvent 237 59 0 0 0 0 0\n"
"InteractionEvent 237 59 0 0 0 0 0\n"
"MouseMoveEvent 237 60 0 0 0 0 0\n"
"RenderEvent 237 60 0 0 0 0 0\n"
"InteractionEvent 237 60 0 0 0 0 0\n"
"MouseMoveEvent 236 61 0 0 0 0 0\n"
"RenderEvent 236 61 0 0 0 0 0\n"
"InteractionEvent 236 61 0 0 0 0 0\n"
"MouseMoveEvent 236 62 0 0 0 0 0\n"
"RenderEvent 236 62 0 0 0 0 0\n"
"InteractionEvent 236 62 0 0 0 0 0\n"
"MouseMoveEvent 235 63 0 0 0 0 0\n"
"RenderEvent 235 63 0 0 0 0 0\n"
"InteractionEvent 235 63 0 0 0 0 0\n"
"LeftButtonReleaseEvent 235 63 0 0 0 0 0\n"
"EndInteractionEvent 235 63 0 0 0 0 0\n"
"RenderEvent 235 63 0 0 0 0 0\n"
"MouseMoveEvent 237 64 0 0 0 0 0\n"
"MouseMoveEvent 242 64 0 0 0 0 0\n"
"MouseMoveEvent 250 61 0 0 0 0 0\n"
"MouseMoveEvent 261 58 0 0 0 0 0\n"
"MouseMoveEvent 272 56 0 0 0 0 0\n"
"MouseMoveEvent 283 53 0 0 0 0 0\n"
"MouseMoveEvent 293 51 0 0 0 0 0\n"
"LeaveEvent 304 46 0 0 0 0 0\n"
"EnterEvent 251 4 0 0 0 0 0\n"
"MouseMoveEvent 251 4 0 0 0 0 0\n"
"MouseMoveEvent 244 15 0 0 0 0 0\n"
"MouseMoveEvent 237 26 0 0 0 0 0\n"
"MouseMoveEvent 230 33 0 0 0 0 0\n"
"MouseMoveEvent 223 42 0 0 0 0 0\n"
"MouseMoveEvent 218 51 0 0 0 0 0\n"
"MouseMoveEvent 213 58 0 0 0 0 0\n"
"MouseMoveEvent 208 65 0 0 0 0 0\n"
"MouseMoveEvent 203 72 0 0 0 0 0\n"
"MouseMoveEvent 199 79 0 0 0 0 0\n"
"MouseMoveEvent 194 83 0 0 0 0 0\n"
"MouseMoveEvent 189 88 0 0 0 0 0\n"
"MouseMoveEvent 185 92 0 0 0 0 0\n"
"MouseMoveEvent 180 97 0 0 0 0 0\n"
"MouseMoveEvent 174 105 0 0 0 0 0\n"
"MouseMoveEvent 172 107 0 0 0 0 0\n"
"MouseMoveEvent 170 111 0 0 0 0 0\n"
"MouseMoveEvent 168 114 0 0 0 0 0\n"
"MouseMoveEvent 167 118 0 0 0 0 0\n"
"MouseMoveEvent 165 122 0 0 0 0 0\n"
"MouseMoveEvent 165 125 0 0 0 0 0\n"
"MouseWheelBackwardEvent 165 125 0 0 0 0 0\n"
"StartInteractionEvent 165 125 0 0 0 0 0\n"
"RenderEvent 165 125 0 0 0 0 0\n"
"EndInteractionEvent 165 125 0 0 0 0 0\n"
"RenderEvent 165 125 0 0 0 0 0\n"
"MouseMoveEvent 164 126 0 0 0 0 0\n"
"MouseMoveEvent 163 126 0 0 0 0 0\n"
"MouseMoveEvent 162 126 0 0 0 0 0\n"
"MouseMoveEvent 161 125 0 0 0 0 0\n"
"LeftButtonPressEvent 161 125 0 0 0 0 0\n"
"StartInteractionEvent 161 125 0 0 0 0 0\n"
"MouseMoveEvent 163 124 0 0 0 0 0\n"
"RenderEvent 163 124 0 0 0 0 0\n"
"InteractionEvent 163 124 0 0 0 0 0\n"
"MouseMoveEvent 164 123 0 0 0 0 0\n"
"RenderEvent 164 123 0 0 0 0 0\n"
"InteractionEvent 164 123 0 0 0 0 0\n"
"MouseMoveEvent 165 122 0 0 0 0 0\n"
"RenderEvent 165 122 0 0 0 0 0\n"
"InteractionEvent 165 122 0 0 0 0 0\n"
"MouseMoveEvent 167 121 0 0 0 0 0\n"
"RenderEvent 167 121 0 0 0 0 0\n"
"InteractionEvent 167 121 0 0 0 0 0\n"
"MouseMoveEvent 169 120 0 0 0 0 0\n"
"RenderEvent 169 120 0 0 0 0 0\n"
"InteractionEvent 169 120 0 0 0 0 0\n"
"LeftButtonReleaseEvent 169 120 0 0 0 0 0\n"
"EndInteractionEvent 169 120 0 0 0 0 0\n"
"RenderEvent 169 120 0 0 0 0 0\n"
"MouseWheelBackwardEvent 169 120 0 0 0 0 0\n"
"StartInteractionEvent 169 120 0 0 0 0 0\n"
"RenderEvent 169 120 0 0 0 0 0\n"
"EndInteractionEvent 169 120 0 0 0 0 0\n"
"RenderEvent 169 120 0 0 0 0 0\n"
"MouseMoveEvent 170 119 0 0 0 0 0\n"
"MouseMoveEvent 169 118 0 0 0 0 0\n"
"MouseMoveEvent 168 117 0 0 0 0 0\n"
"MouseMoveEvent 167 116 0 0 0 0 0\n"
"MouseMoveEvent 169 114 0 0 0 0 0\n"
"MouseMoveEvent 176 113 0 0 0 0 0\n"
"MouseMoveEvent 186 110 0 0 0 0 0\n"
"MouseMoveEvent 201 105 0 0 0 0 0\n"
"MouseMoveEvent 216 100 0 0 0 0 0\n"
"MouseMoveEvent 231 95 0 0 0 0 0\n"
"MouseMoveEvent 250 90 0 0 0 0 0\n"
"MouseMoveEvent 269 85 0 0 0 0 0\n"
"MouseMoveEvent 290 78 0 0 0 0 0\n"
"LeaveEvent 311 71 0 0 0 0 0\n"
;

int TestGPURayCastCameraInside(int argc, char *argv[])
{
  cout << "CTEST_FULL_OUTPUT (Avoid ctest truncation of output)" << endl;

  char* volumeFile = vtkTestUtilities::ExpandDataFileName(
                       argc, argv, "Data/ironProt.vtk");
  vtkNew<vtkStructuredPointsReader> reader;
  reader->SetFileName(volumeFile);
  delete[] volumeFile;

  vtkNew<vtkGPUVolumeRayCastMapper> mapper;
  mapper->SetInputConnection(reader->GetOutputPort());
  mapper->SetAutoAdjustSampleDistances(0);
  mapper->SetSampleDistance(1.0);

  vtkNew<vtkColorTransferFunction> color;
  color->AddRGBPoint(0.0, 0.0, 0.0, 0.0);
  color->AddRGBPoint(64.0, 1.0, 0.0, 0.0);
  color->AddRGBPoint(128.0, 0.0, 0.0, 1.0);
  color->AddRGBPoint(192.0, 0.0, 1.0, 0.0);
  color->AddRGBPoint(255.0, 0.0, 0.2, 0.0);

  vtkNew<vtkPiecewiseFunction> opacity;
  opacity->AddPoint(0.0, 0.0);
  opacity->AddPoint(255.0, 1.0);

  vtkNew<vtkVolumeProperty> property;
  property->SetColor(color.GetPointer());
  property->SetScalarOpacity(opacity.GetPointer());
  property->SetInterpolationTypeToLinear();
  property->ShadeOff();

  vtkNew<vtkVolume> volume;
  volume->SetMapper(mapper.GetPointer());
  volume->SetProperty(property.GetPointer());

  vtkNew<vtkRenderWindow> renWin;
  renWin->SetSize(301, 300);
  renWin->SetMultiSamples(0);

  vtkNew<vtkRenderWindowInteractor> iren;
  iren->SetRenderWindow(renWin.GetPointer());
  vtkNew<vtkInteractorStyleTrackballCamera> style;
  iren->SetInteractorStyle(style.GetPointer());

  vtkNew<vtkRenderer> ren;
  renWin->AddRenderer(ren.GetPointer());

// Attach OSPRay render pass
  vtkSmartPointer<vtkOSPRayPass> osprayPass =
    vtkSmartPointer<vtkOSPRayPass>::New();
  ren->SetPass(osprayPass.GetPointer());

  ren->AddVolume(volume.GetPointer());
  ren->ResetCamera();

  renWin->Render();
  iren->Initialize();

  return vtkTesting::InteractorEventLoop(argc, argv,
                                         iren.GetPointer(),
                                         TestGPURayCastCameraInsideLog);
}
