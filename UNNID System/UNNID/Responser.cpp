/***************************************************************************
                          Responser.cpp  -  description
                             -------------------
    begin                : Thu May 15 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#include "Responser.h"

Responser::Responser()
{
}
Responser::~Responser()
{
}

void Responser::SetWorkSpaceParameters(char *ParamString)
{
    if ( !strncasecmp(ParamString, "null", 4)) return;
    
}//End of function Responser::SetWorkSpaceParameters

void Responser::GetWorkSpaceParameters(char *ParamString)
{
    strcpy(ParamString, "null");
}//End of function Responser::GetWorkSpaceParameters