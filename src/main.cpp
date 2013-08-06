// HueZoo 0.1.0
//
// Copyright (c) 2013 Robert Martens
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the
// use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software in a
//   product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source distribution.



#include <cmath>
#include <cstdlib>

#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <vector>



typedef struct
{

  short int magic; // IRIS image file magic number, 474
  char storage; // Storage format, 0=verbatim, 1=RLE
  char bpc; // Bytes per component
  unsigned short int dimension; // Number of dimensions (3 for RGB, RGBA)
  unsigned short int xsize; // Width, in pixels
  unsigned short int ysize; // Height, in pixels
  unsigned short int zsize; // Number of channels (3 for RGB, 4 for RGBA)
  unsigned int pixmin; // Minimum pixel value
  unsigned int pixmax; // Maximum pixel value
  unsigned char dummy4[4]; // Ignored
  char* imagename[80];
  unsigned int colormap; // Colormap ID
  unsigned char dummy404[404]; // Ignored

} SGI_HDR;



void WriteChar(std::ofstream* outfile, unsigned char val)
{

  unsigned char buf[1];

  buf[0] = static_cast<unsigned char>(val);

  outfile->write((const char*)&buf[0], 1);

}



void WriteShort(std::ofstream* outfile, unsigned short val)
{

  unsigned char buf[2];

  buf[0] = static_cast<unsigned char>(val >> 8);
  buf[1] = static_cast<unsigned char>(val);

  outfile->write((const char*)&buf[0], 2);

}



void WriteInt(std::ofstream* outfile, unsigned int val)
{

  unsigned char buf[4];

  buf[0] = static_cast<unsigned char>(val >> 24);
  buf[1] = static_cast<unsigned char>(val >> 16);
  buf[2] = static_cast<unsigned char>(val >> 8);
  buf[3] = static_cast<unsigned char>(val);

  outfile->write((const char*)&buf[0], 4);

}



int main(int argc, char* argv[])
{

  int width = 512,
      height = 512;

  std::string outfile;

  if (argc == 4) {
    width = std::atoi(argv[1]);
    height = std::atoi(argv[2]);
    outfile = argv[3];
  } else if (argc == 2) {
    outfile = argv[1];
  } else {
    std::cout << "Usage: huezoo [width height] filename" << std::endl;
    return -1;
  }

  SGI_HDR hdr;
  hdr.magic = 474;
  hdr.storage = 0;
  hdr.bpc = 1;
  hdr.dimension = 3;
  hdr.xsize = width;
  hdr.ysize = height;
  hdr.zsize = 3;
  hdr.pixmin = 0;
  hdr.pixmax = 255;
  hdr.colormap = 0;


  std::vector<unsigned char> R, G, B;

  // Naive approach to HSL->RGB derived from Wikipedia description:
  // http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL

  double saturation = 1.0,
         lightness = 0.0;

  double stepHue = 360.0 / static_cast<double>(hdr.xsize),
         stepLightness = 1.0 / static_cast<double>(hdr.ysize);

  for (int i = 0; i < hdr.ysize; ++i) {
    
    double hue = 0.0;

    for (int j = 0; j < hdr.xsize; ++j) {

      double chroma = (1.0 - std::abs((2.0 * lightness) - 1.0)) * saturation;

      double col = hue / 60.0;

      double x = chroma * (1.0 - std::abs(std::fmod(col, 2.0) - 1.0));

      double r = 0.0, g = 0.0, b = 0.0;
      if (col >= 0.0 && col < 1.0) {
        r = chroma;
        g = x;
        b = 0.0;
      } else if (col >= 1.0 && col < 2.0) {
        r = x;
        g = chroma;
        b = 0.0;
      } else if (col >= 2.0 && col < 3.0) {
        r = 0.0;
        g = chroma;
        b = x;
      } else if (col >= 3.0 && col < 4.0) {
        r = 0.0;
        g = x;
        b = chroma;
      } else if (col >= 4.0 && col < 5.0) {
        r = x;
        g = 0.0;
        b = chroma;
      } else if (col >= 5.0 && col < 6.0) {
        r = chroma;
        g = 0.0;
        b = x;
      }

      double m = lightness - (chroma * 0.5);

      R.push_back(static_cast<unsigned char>(((r + m) * 255.0) + 0.5));
      G.push_back(static_cast<unsigned char>(((g + m) * 255.0) + 0.5));
      B.push_back(static_cast<unsigned char>(((b + m) * 255.0) + 0.5));

      hue += stepHue;

    }

    lightness += stepLightness;

  }


  std::ofstream out(outfile.c_str(), std::ios::binary | std::ios::out);
  if (!out) {
    std::cout << "Couldn't create " << outfile << "!" << std::endl;
    return -1;
  }

  WriteShort(&out, hdr.magic);
  WriteChar(&out, hdr.storage);
  WriteChar(&out, hdr.bpc);
  WriteShort(&out, hdr.dimension);
  WriteShort(&out, hdr.xsize);
  WriteShort(&out, hdr.ysize);
  WriteShort(&out, hdr.zsize);
  WriteInt(&out, hdr.pixmin);
  WriteInt(&out, hdr.pixmax);
  for (int i = 0; i < 4; ++i) // dummy4
    WriteChar(&out, 0);
  for (int i = 0; i < 80; ++i) // imagename
    WriteChar(&out, 0);
  WriteInt(&out, hdr.colormap);
  for (int i = 0; i < 404; ++i) // dummy404
    WriteChar(&out, 0);


  std::vector<unsigned char> image;
  image.reserve(hdr.xsize * hdr.ysize * hdr.dimension);

  for (std::vector<unsigned char>::iterator i = R.begin(); i != R.end(); ++i)
    image.push_back(*i);
  for (std::vector<unsigned char>::iterator i = G.begin(); i != G.end(); ++i)
    image.push_back(*i);
  for (std::vector<unsigned char>::iterator i = B.begin(); i != B.end(); ++i)
    image.push_back(*i);

  out.write((const char*)image.data(), hdr.xsize * hdr.ysize * hdr.dimension);
  out.close();

  return 0;

}
