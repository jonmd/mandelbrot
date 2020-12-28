import math

def colorize_black_and_white(value, iterations=1024):
    if value + 1 >= iterations:
        return (0, 0, 0)
    v = math.floor(value / iterations * 255)
    return (v, v, v)


def create_colorize_black_and_white(iterations):
    def colorize(value):
        return colorize_black_and_white(value, iterations=iterations)
    return colorize