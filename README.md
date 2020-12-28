# Mandelbrot

A hobby project during Christmas 2020. Draw a region of the Mandelbrot
set with a repeating gradient.

## How to build and run

Required libraries for building:

 * `libpthread`
 * `libm`
 * `libpng`

Build:

```
$ make
```

Options:

```
$ ./mandelbrot --help
Usage: mandelbrot [OPTION...] IMAGE.png
Draw the Mandelbrot set a selected region.

  -?, --help                 Give this help list
  -i, --iterations=N         Number of iterations per pixel [default: 100]
  -p, --progress             Show progress [default: no]
  -s, --supersampling        Sample with a factor 2x2 [default: no]
  -t, --threads=NTHREADS     Set number of threads
      --usage                Give a short usage message
  -v, --verbose              Print more
  -V, --version              Print program version
  -w, --width=WIDTH          Set output image width in pixels [default: 300]
      --xmax=F               Maximum X [default:  1.0]
      --xmin=F               Minimum X [default: -2.5]
      --ymax=F               Maximum Y [default:  1.0]
      --ymin=F               Minimum Y [default: -1.0]

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

## Images


### Full view

```
$ ./mandelbrot --width 512 --supersampling --ymin -1.75 --ymax 1.75 image.png
```

![Full](https://github.com/jonmd/mandelbrot/blob/master/images/mandelbrot_full_512px.png?raw=true)


### Zoomed in on -0.544 + 0.494i ±0.001

```
$ ./mandelbrot --width 512 --iterations 768 --supersampling \
    --xmin -0.545 --xmax -0.543 --ymin 0.493 --ymax 0.495 \
    image.png
```

![Full](https://github.com/jonmd/mandelbrot/blob/master/images/mandelbrot_0.544_0.494.png?raw=true)