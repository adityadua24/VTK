/*=========================================================================

  Program:   Visualization Library
  Module:    Filter.cc
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

This file is part of the Visualization Library. No part of this file or its 
contents may be copied, reproduced or altered in any way without the express
written consent of the authors.

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen 1993, 1994 

=========================================================================*/
#include "Filter.hh"

// Description:
// Construct new filter without start or end methods.
vlFilter::vlFilter()
{
  this->StartMethod = NULL;
  this->StartMethodArg = NULL;
  this->EndMethod = NULL;
  this->EndMethodArg = NULL;

  this->Updating = 0;
}

// Description:
// Set the filter start method. The start method is invoked before the 
// filter executes.
void vlFilter::SetStartMethod(void (*f)(void *), void *arg)
{
  if ( f != this->StartMethod || arg != this->StartMethodArg )
    {
    this->StartMethod = f;
    this->StartMethodArg = arg;
    this->Modified();
    }
}

// Description:
// Set the filter end method. The end method is invoked after the 
// filter executes.
void vlFilter::SetEndMethod(void (*f)(void *), void *arg)
{
  if ( f != this->EndMethod || arg != this->EndMethodArg )
    {
    this->EndMethod = f;
    this->EndMethodArg = arg;
    this->Modified();
    }
}

void vlFilter::PrintSelf(ostream& os, vlIndent indent)
{
  if (this->ShouldIPrint(vlFilter::GetClassName()))
    {
    vlObject::PrintSelf(os,indent);

    if ( this->StartMethod )
      {
      os << indent << "Start Method: (" << this->StartMethod << ")\n";
      }
    else
      {
      os << indent << "Start Method: (none)\n";
      }

    if ( this->EndMethod )
      {
      os << indent << "End Method: (" << this->EndMethod << ")\n";
      }
    else
      {
      os << indent << "End Method: (none)\n";
      }

    os << indent << "Execute Time: " <<this->ExecuteTime.GetMTime() << "\n";
   }
}
