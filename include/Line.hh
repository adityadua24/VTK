/*=========================================================================

  Program:   Visualization Library
  Module:    Line.hh
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

This file is part of the Visualization Library. No part of this file
or its contents may be copied, reproduced or altered in any way
without the express written consent of the authors.

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 1993, 1994 

=========================================================================*/
// .NAME vlLine - cell represents a 1D line
// .SECTION Description
// vlLine is a concrete implementation of vlCell to represent a 1D line.

#ifndef __vlLine_h
#define __vlLine_h

#include "Cell.hh"

class vlLine : public vlCell
{
public:
  vlLine() {};
  vlLine(const vlLine& l);
  char *GetClassName() {return "vlLine";};

  vlCell *MakeObject() {return new vlLine(*this);};
  int GetCellType() {return vlLINE;};
  int GetCellDimension() {return 1;};
  int GetNumberOfEdges() {return 0;};
  int GetNumberOfFaces() {return 0;};
  vlCell *GetEdge(int edgeId) {return 0;};
  vlCell *GetFace(int faceId) {return 0;};

  void Contour(float value, vlFloatScalars *cellScalars, 
               vlFloatPoints *points, vlCellArray *verts, 
               vlCellArray *lines, vlCellArray *polys, 
               vlFloatScalars *s);
  int EvaluatePosition(float x[3], float closestPoint[3],
                       int& subId, float pcoords[3], 
                       float& dist2, float weights[MAX_CELL_SIZE]);
  void EvaluateLocation(int& subId, float pcoords[3], float x[3],
                        float weights[MAX_CELL_SIZE]);

  int Intersection(float x[3], float xray[3], float x1[3], float x2[3],
               float& u, float& v);

  float DistanceToLine (float x[3], float p1[3], float p2[3]);
  
};

#endif


