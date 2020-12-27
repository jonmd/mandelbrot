import math
import colorsys


def colorize_black_and_white(value, iterations=1024):
    if value + 1 >= iterations:
        return (0, 0, 0)
    v = math.floor(value / iterations * 255)
    return (v, v, v)


def normalize(tuple, size=255):
    return (
        tuple[0] / size,
        tuple[1] / size,
        tuple[2] / size,
    )

def quantify(tuple, size=255):
    return (
        round(tuple[0] * size),
        round(tuple[1] * size),
        round(tuple[2] * size),
    )

def to_rgb(hsv):
    return quantify(colorsys.hsv_to_rgb(*normalize(hsv)))

def to_hsv(rgb):
    return quantify(colorsys.rgb_to_hsv(*normalize(rgb)))

def color_lerp(color1, color2, factor):
    return (
        color2[0] * factor + color1[0] * (1.0 - factor),
        color2[1] * factor + color1[1] * (1.0 - factor),
        color2[2] * factor + color1[2] * (1.0 - factor),
    )

def perpare_gradient(items):
    i = 1
    gradients = []
    while i < len(items):
        i1 = items[i - 1]
        i2 = items[i]
        c1 = to_hsv(i1[1])
        c2 = to_hsv(i2[1])
        gradients.append((i1[0], i2[0], c1, c2))
        i += 1
    return gradients

GRADIENTS = perpare_gradient([
    (0.00000, (0, 0, 0x1f)),
    (0.02000, (0x17, 0x35, 0x5f)),
    (0.05000, (0x20, 0x63, 0x9B)),
    (0.15000, (0x3C, 0xAE, 0xA3)),
    (0.40000, (0xF6, 0xD5, 0x5C)),
    (0.90000, (0xED, 0x55, 0x3B)),
    (0.99980, (255, 255, 255)),
    (0.99990, (255, 255, 255)),
    (0.99991, (0, 0, 0)),
    (1.00001, (0, 0, 0)),
])

def colorize_gradient(value, iterations=1024):
    gradients = GRADIENTS
    nominal = value / iterations
    for i_min, i_max, color1, color2 in gradients:
        if i_min <= nominal < i_max:
            i = (nominal - i_min) / (i_max - i_min)
            return to_rgb(color_lerp(color1, color2, i))
    return (0, 0, 0)
