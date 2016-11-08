/***************************************************************************
                          Responser.h  -  description
                             -------------------
    begin                : Thu May 15 2003
    copyright            : (C) 2003 by Morteza Amini
    email                : MortezaAmini@yahoo.com
 ***************************************************************************/


#ifndef RESPONSER_H
#define RESPONSER_H

#include <string.h>


class Responser
{
    public:
        Responser();
        ~Responser();
        void SetWorkSpaceParameters(char *);
        void GetWorkSpaceParameters(char *);
};

#endif
