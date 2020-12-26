import colorsys
import math


COLORS = [
    (0x17, 0x35, 0x5f),
    (0x20, 0x63, 0x9B),
    (0x3C, 0xAE, 0xA3),
    (0xF6, 0xD5, 0x5C),
    (0xED, 0x55, 0x3B),
    (0xBF, 0xA0, 0xFF),
]


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


def create_gradient(iterations):
    gradient = [
        (0, (0, 0, 0x1f)),
    ]
    i, c = 0, 0
    while i + 51 < iterations:
        i0 = i + 35
        i1 = i + 50
        color = COLORS[c]
        c = (c + 1) % len(COLORS)
        gradient.append((i0, color))
        gradient.append((i1, color))
        i += 50
    gradient.append((iterations - 1, (0, 0, 0)))
    gradient.append((iterations + 2, (0, 0, 0)))
    # Make it searchable
    gradient = perpare_gradient(gradient)
    return gradient


def create_colorize_gradient(iterations):
    gradient = create_gradient(iterations)
    def colorize(value, iterations=iterations):
        imin, imax = 0, len(gradient)
        while True:
            i = math.floor((imin + imax) / 2)
            v_min, v_max, color1, color2 = gradient[i]
            if v_min <= value < v_max:
                factor = (value - v_min) / (v_max - v_min)
                rgb = to_rgb(color_lerp(color1, color2, factor))
                return rgb
            elif value < v_min:
                imax = i
            elif value >= v_max:
                imin = i
    return colorize


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