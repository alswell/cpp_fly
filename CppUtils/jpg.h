#pragma once

#include "env.h"

#include "jpeglib.h"
#include "jerror.h"

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& rdata, unsigned& w, unsigned& h);
int jpeg2rgb(const char* file_name, unsigned char*& rdata, unsigned& w, unsigned& h);
