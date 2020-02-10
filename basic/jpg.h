#pragma once

#include "env.h"

#include "jpeglib.h"
#include "jerror.h"

#include "png.h"

int jpeg2rgb(unsigned char* buff, unsigned size, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);
int jpeg2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h, unsigned scale_denom = 1);

void rgb2jpeg(const char* filename, unsigned char* img, unsigned w, unsigned h);

bool png2rgb(const char* filename, unsigned char*& img, unsigned& w, unsigned& h);
void rgb2png(const char* filename, unsigned char* img, unsigned w, unsigned h);
