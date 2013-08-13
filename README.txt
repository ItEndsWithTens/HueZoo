
  --HueZoo 0.1.0--

    Plot the HSL/HSV color spectrum as a custom size PNG.
    
    http://www.gyroshot.com
    robert.martens@gmail.com
    @ItEndsWithTens


  --Usage--

    huezoo [--help] [-w <width>] [-h <height>] [-m <mode>] [-s <saturation>]
           [-o <outfile>]

      --help
        Display usage information.

      -w, --width
      -h, --height
        Width and height, limited only by available memory. Default 512x512.

      -m, --mode
        Whether to use the HSL or HSV color model. Default HSL.

      -s, --saturation
        The constant dimension of both the HSL and HSV color models. I've left
      this value uncapped, so feel free to experiment. Default 1.0.

      -o, --outfile
        The filename to use when saving output. Default "hsl.png" for HSL,
      "hsv.png" for HSV.


  --Changes--

    0.1.0 - August 13th, 2013
      Initial release
