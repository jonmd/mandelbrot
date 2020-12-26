"""Render the mandelbrot set

Usage:
    mandelbrot.py <filename> [--height=<h>] [--width=<w>] [--supersample=<s>] [--processes=<p>]

Options:
    -h --help          Show this screen
    --height=<h>       Output image height [default: 200]
    --width=<w>        Output image width [default: 350]
    --supersample<=s>  Supersample by factor S [deafult: 1]
    --processes=<p>    Number of processes to use [default: 1]
"""
import math
from functools import partial
from itertools import product
from multiprocessing import Pool

from docopt import docopt
from PIL import Image
from progress.bar import ChargingBar


# Constants
X_MIN, X_MAX = (-2.5, 1)
Y_MIN, Y_MAX = (-1, 1)

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


def solve(cx, cy, iterations=512):
    x = 0
    y = 0
    for i in range(iterations):
        x2 = x * x
        y2 = y * y
        if x2 + y2 > 4:
            return i
        x_temp = x2 - y2 + cx
        y = 2 * x * y + cy
        x = x_temp
    return iterations

def colorize(value, iterations=512):
    return math.floor(value / iterations * 255)


def process_pixel(pixel, width=None, height=None, iterations=None):
    px, py = pixel
    x, y = pixel_to_coordinate(px, py, width=width, height=height)
    v = solve(x, y, iterations=iterations)
    return (px, py, v)


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

    image = Image.new('P', (width, height))

    progess_size = width * height / 500
    progress = ChargingBar("Rendering Mandelbrot set", max=progess_size)

    if processes > 1:
        with Pool(processes) as pool:
            values = pool.imap(
                partial(process_pixel, width=width, height=height, iterations=iterations),
                product(range(width), range(height)),
                chunksize=526
            )
            i = 0
            for px, py, v in values:
                i += 1
                if progress and (i % 500) == 0:
                    progress.next()
                color = colorize(v, iterations=iterations)
                image.putpixel((px, py), color)

    else:
        for px, py in product(range(width), range(height)):
            if progress:
                progress.next()
            x, y = pixel_to_coordinate(px, py, width=width, height=height)
            v = solve(x, y, iterations=iterations)
            color = colorize(v, iterations=iterations)
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
    height = int(args['--height'], 10)
    supersample = int(args['--supersample'], 10)
    processes = int(args['--processes'], 10)

    with open(filename, 'wb') as f:
        image = mandelbrot(width, height, supersample=supersample, processes=processes, progress=True)
        image.save(f, format='png')
