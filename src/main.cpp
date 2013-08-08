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

#include <iostream>
#include <string>
#include <vector>

#include "lodepng/lodepng.h"



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

  // Naive approach to HSL->RGB derived from Wikipedia description:
  // http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL

  double saturation = 1.0,
         lightness = 1.0;

  double stepHue = 360.0 / static_cast<double>(width),
         stepLightness = 1.0 / static_cast<double>(height);

  std::vector<unsigned char> image;
  image.reserve(width * height * 4);
  for (int i = 0; i < height; ++i) {
    
    double hue = 0.0,
           chroma = (1.0 - std::abs((2.0 * lightness) - 1.0)) * saturation,
           m = lightness - (chroma * 0.5);

    for (int j = 0; j < width; ++j) {

      double col = hue / 60.0,
             x = chroma * (1.0 - std::abs(std::fmod(col, 2.0) - 1.0));

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

      image.push_back(static_cast<unsigned char>(((r + m) * 255.0) + 0.5));
      image.push_back(static_cast<unsigned char>(((g + m) * 255.0) + 0.5));
      image.push_back(static_cast<unsigned char>(((b + m) * 255.0) + 0.5));
      image.push_back(255);

      hue += stepHue;

    }

    lightness -= stepLightness;

  }

  unsigned int err = lodepng::encode(outfile, image.data(), width, height);

  if (err) {
    std::cout << "LodePNG error " << err << ": " <<
    lodepng_error_text(err) << std::endl;
    return -1;
  }

  return 0;

}
