/*
 *  GLErrStream.h
 *  VBO TEST
 *
 *  Created by Patrick  Wingo on 3/8/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __GL_ERR_STREAM_PW__
#define __GL_ERR_STREAM_PW__

#ifdef __APPLE__
#include<OpenGL/gl3.h>
#else
#ifdef _WIN32
#include<Windows.h>
#endif
#include <GL/glew.h>
#include<GL/gl.h>
#include<cstdio>
#endif

inline void GLPrintErr(const char* name, const char* file, const int line)
{
    unsigned int err=glGetError();
    const char* outStr;
    
//    static int hits = 0;
//    printf("[%s,%i]  %s, hit: %i\n",file,line,name,++hits);
    
    int print=1;
    switch(err)
    {
        case GL_INVALID_ENUM:
            outStr="Invalid Enumerated Argument";
            break;
        case GL_INVALID_VALUE:
            outStr="Value Out of Range";
            break;
        case GL_INVALID_OPERATION:
            outStr="Operation Not Allowed";
            break;
        //case GL_STACK_OVERFLOW:
        //    outStr="Stack Overflow";
        case GL_OUT_OF_MEMORY:
            outStr="Out of Memory";
            break;
       // case GL_TABLE_TOO_LARGE:
        //    outStr="Table Exceeds Maximum Size";
            break;
        default:
            print=0;
            outStr="No Error";
    };
    
    if(print)
    {
        printf("GLError[%s,%i]: %s: %s",file,line,name,outStr);
        //cerr<<"GLError["<<file<<':'<<line<<"]: "<<name<<": "<<outStr<<endl;
#ifdef ASSERT_GL_GO_NUCLEAR
        exit(1);
#endif
    }
}

#if defined(DEBUG) || defined(_DEBUG) || defined(ASSERT_GL_ERR)
    #define ASSERT_GL( x ) GLPrintErr( x, __FILE__,__LINE__)
    #define ASSERT_GL_WRAP( x ) x; GLPrintErr("\""#x"\"",__FILE__,__LINE__)
#else
#define ASSERT_GL( x )
#define ASSERT_GL_WRAP( x ) x
#endif

#endif
