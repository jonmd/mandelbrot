"""Render the mandelbrot set for a

Usage:
    mandelbrot.py <filename> [--width=<w>] [--supersample=<s>] [--processes=<p>] [--iterations=<i>] [--xmin=<n>] [--xmax=<n>] [--ymin=<n>] [--ymax=<n>] [--colorize] [--zx=<N>] [--zy=<N>]

Options:
    -h --help          Show this screen
    --width=<w>        Output image width [default: 350]
    --iterations=<i>   Number of iterations per sample [default: 1024]
    --supersample<=s>  Supersample by factor S [deafult: 1]
    --processes=<p>    Number of processes to use [default: 1]
    --xmin=<n>         Minimum X [default: -2.5]
    --xmax=<n>         Maximum X [default:  1.0]
    --ymin=<n>         Minimum Y [default: -1.0]
    --ymax=<n>         Maximum Y [default:  1.0]
    --zx=<n>           Starting point for zx in the equation [default: 0.0]
    --zy=<n>           Starting point for zy in the equation [default: 0.0]
"""
import math
from functools import partial
from itertools import product
from multiprocessing import Pool

from docopt import docopt
from PIL import Image
from progress.bar import ChargingBar

from colors import COLORIZE_FUNCTIONS


# Constants
X_MIN = -2.5
X_MAX =  1.0
Y_MIN = -1.0
Y_MAX =  1.0

X_RANGE = (X_MAX - X_MIN)
Y_RANGE = (Y_MAX - Y_MIN)


def pixel_to_coordinate(px, py, width=None, height=None):
    # Center in pixel
    px, py = px + 0.5, py + 0.5
    # Transform to X in coordinate system
    x = X_MIN + (px / width) * X_RANGE
    # Transform to Y in coordinate system (Y is inverse in image)
    y = Y_MIN + ((height - py) / height) * Y_RANGE
    return (x, y)


colorize = None
init_x = 0
init_y = 0


def solve(cx, cy, iterations=512):
    x = init_x
    y = init_y
    for i in range(iterations):
        x2 = x * x
        y2 = y * y
        if x2 + y2 > 4:
            return i
        x_temp = x2 - y2 + cx
        y = 2 * x * y + cy
        x = x_temp
    return iterations


def process_pixel(pixel, width=None, height=None, iterations=None):
    px, py = pixel
    x, y = pixel_to_coordinate(px, py, width=width, height=height)
    v = solve(x, y, iterations=iterations)
    color = colorize(v, iterations=iterations)
    return (px, py, color)


def mandelbrot(
        width,
        height,
        iterations=1024,
        processes=1,
        supersample=1,
        progress=False):

    smooth = supersample > 1

    if smooth:
        width *= supersample
        height *= supersample

    image = Image.new('RGB', (width, height))

    step = round(iterations / 200)
    progess_size = width * height / step
    progress = ChargingBar("Rendering Mandelbrot set", max=progess_size)

    with Pool(processes) as pool:
        values = pool.imap(
            partial(process_pixel, width=width, height=height, iterations=iterations),
            product(range(width), range(height)),
            chunksize=512
        )
        i = 0
        for px, py, color in values:
            if progress:
                i += 1
                if (i % step) == 0:
                    progress.next()
            image.putpixel((px, py), color)

    if progress:
        progress.finish()

    if smooth:
        image = image.resize((width // supersample, height // supersample))

    return image




if __name__ == '__main__':
    args = docopt(__doc__)
    print(args)

    filename = args['<filename>']
    width = int(args['--width'], 10)
    supersample = int(args['--supersample'], 10)
    processes = int(args['--processes'], 10)
    iterations = int(args['--iterations'], 10)

    init_x = float(args['--zx'])
    init_y = float(args['--zy'])

    X_MIN = float(args['--xmin'])
    X_MAX = float(args['--xmax'])
    Y_MIN = float(args['--ymin'])
    Y_MAX = float(args['--ymax'])

    X_RANGE = (X_MAX - X_MIN)
    Y_RANGE = (Y_MAX - Y_MIN)

    height = round(width * (Y_RANGE / X_RANGE))

    colorize = COLORIZE_FUNCTIONS['gradient'](iterations=iterations)

    print(f"width={width}, height={height}")
    print(f"xmin={X_MIN}, xmax={X_MAX}")
    print(f"xmin={Y_MIN}, xmax={Y_MAX}")

    with open(filename, 'wb') as f:
        image = mandelbrot(
            width,
            height,
            iterations=iterations,
            supersample=supersample,
            processes=processes,
            progress=True
        )
        image.save(f, format='png')
