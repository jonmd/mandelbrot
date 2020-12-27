#!/usr/bin/env bash

date +'%Y-%m-%d %H:%M'

#python mandelbrot.py mandelbrot.png --width=350 --processes=8 --supersample=2 --iterations=512 \
#    --xmin -0.55 --xmax -0.525 --ymin 0.475 --ymax 0.5

# Pretty cut
# python mandelbrot.py mandelbrot.png --width=512 --processes=8 --supersample=8 --iterations=512 \
#     --xmin -0.55 --xmax -0.5375 --ymin 0.4875 --ymax 0.5

# Large!
#python mandelbrot.py mandelbrot_8192.png --width=8192 --processes=8 --supersample=2 --iterations=2048 \
#    --xmin -0.550 --xmax -0.539 --ymin 0.490 --ymax 0.501

# Quick version of 8Kx8K
#python mandelbrot.py mandelbrot.png --width=512 --processes=8 --supersample=2 --iterations=1024 \
#    --xmin -0.550 --xmax -0.539 --ymin 0.490 --ymax 0.501

# Zoomed v1
python mandelbrot.py mandelbrot.png --width=4096 --processes=8 --supersample=2 --iterations=768 \
      --xmin -0.545 --xmax -0.543 --ymin 0.493 --ymax 0.495

# Closing in on point
#python mandelbrot.py mandelbrot.png --width=256 --processes=8 --supersample=2 --iterations=2048 \
#     --xmin -0.5437 --xmax -0.5435 --ymin 0.495 --ymax 0.4952

# Zoomed again
#python mandelbrot.py mandelbrot.png --width=256 --processes=8 --supersample=2 --iterations=2048 \
#     --xmin -0.54361 --xmax -0.54359 --ymin 0.49509 --ymax 0.49511

# Woho!
#python mandelbrot.py mandelbrot.png --width=1024 --processes=8 --supersample=2 --iterations=2048 \
#     --xmin -0.543592 --xmax -0.543590 --ymin 0.495109 --ymax 0.495111

# Woho!
#python mandelbrot.py mandelbrot.png --width=1200 --processes=8 --supersample=2 --iterations=2048 \
#     --xmin 0.2500 --xmax 0.2504 --ymin -0.00001 --ymax 0.00001 #--zx 0.8 --zy -0.082

date +'%Y-%m-%d %H:%M'
