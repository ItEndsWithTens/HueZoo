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



#include <cctype>
#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "lodepng/lodepng.h"



void PlotHsl(
  std::vector<unsigned char>* image, int width, int height, double saturation)
{

  // Naive approach to HSL->RGB derived from Wikipedia description:
  // http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSL

  double lightness = 1.0;

  double stepHue = 360.0 / static_cast<double>(width),
         stepLightness = 1.0 / static_cast<double>(height);

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

      image->push_back(static_cast<unsigned char>(((r + m) * 255.0) + 0.5));
      image->push_back(static_cast<unsigned char>(((g + m) * 255.0) + 0.5));
      image->push_back(static_cast<unsigned char>(((b + m) * 255.0) + 0.5));
      image->push_back(255);

      hue += stepHue;

    }

    lightness -= stepLightness;

  }

}



void PlotHsv(
  std::vector<unsigned char>* image, int width, int height, double saturation)
{

  // Naive approach to HSV->RGB derived from Wikipedia description:
  // http://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV

  double value = 1.0;

  double stepHue = 360.0 / static_cast<double>(width),
         stepValue = 1.0 / static_cast<double>(height);

  for (int i = 0; i < height; ++i) {
    
    double hue = 0.0,
           chroma = value * saturation,
           m = value - chroma;

    for (int j = 0; j < width; ++j) {

      double hPrime = hue / 60.0,
             x = chroma * (1.0 - std::abs(std::fmod(hPrime, 2.0) - 1.0));

      double r = 0.0, g = 0.0, b = 0.0;
      if (hPrime >= 0.0 && hPrime < 1.0) {
        r = chroma;
        g = x;
        b = 0.0;
      } else if (hPrime >= 1.0 && hPrime < 2.0) {
        r = x;
        g = chroma;
        b = 0.0;
      } else if (hPrime >= 2.0 && hPrime < 3.0) {
        r = 0.0;
        g = chroma;
        b = x;
      } else if (hPrime >= 3.0 && hPrime < 4.0) {
        r = 0.0;
        g = x;
        b = chroma;
      } else if (hPrime >= 4.0 && hPrime < 5.0) {
        r = x;
        g = 0.0;
        b = chroma;
      } else if (hPrime >= 5.0 && hPrime < 6.0) {
        r = chroma;
        g = 0.0;
        b = x;
      }

      image->push_back(static_cast<unsigned char>(((r + m) * 255.0) + 0.5));
      image->push_back(static_cast<unsigned char>(((g + m) * 255.0) + 0.5));
      image->push_back(static_cast<unsigned char>(((b + m) * 255.0) + 0.5));
      image->push_back(255);

      hue += stepHue;

    }

    value -= stepValue;

  }

}



int main(int argc, char* argv[])
{

  int width = 512,
      height = 512;

  double saturation = 1.0;

  std::string mode = "HSL",
              outfile = "";

  for (int i = 0; i < argc; ++i) {

    std::string arg = argv[i];

    if (arg == "--help" || argc % 2 == 0) {

      std::cout <<
        "Usage: huezoo [--help] [-w <width>] [-h <height>] [-m <mode>] "
        "[-s <saturation>] [-o <outfile>]" << std::endl;
      return -1;

    } else if (arg == "-w" || arg == "--width") {

      width = std::atoi(argv[i+1]);

    } else if (arg == "-h" || arg == "--height") {

      height = std::atoi(argv[i+1]);

    } else if (arg == "-m" || arg == "--mode") {

      mode = argv[i+1];
      std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper);

    } else if (arg == "-s" || arg == "--saturation") {

      saturation = std::atof(argv[i+1]);

    } else if (arg == "-o" || arg == "--outfile") {

      outfile = argv[i+1];

    }

  }

  if (outfile == "") {
    if (mode == "HSV")
      outfile = "hsv.png";
    else
      outfile = "hsl.png";
  }

  std::vector<unsigned char> image;
  image.reserve(width * height * 4);

  if (mode == "HSV")
    PlotHsv(&image, width, height, saturation);
  else
    PlotHsl(&image, width, height, saturation);

  unsigned int err = lodepng::encode(outfile, image.data(), width, height);

  if (err) {
    std::cout << "LodePNG error " << err << ": " <<
    lodepng_error_text(err) << std::endl;
    return -1;
  }

  return 0;

}
