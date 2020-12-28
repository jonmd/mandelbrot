from .black_and_white import create_colorize_black_and_white
from .gradient import create_colorize_gradient


COLORIZE_FUNCTIONS = {
    'bw': create_colorize_black_and_white,
    'gradient': create_colorize_gradient,
}

__all__ = [
    COLORIZE_FUNCTIONS
]