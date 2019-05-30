#include "Color.hpp"

static const Vector luminanceVector(Vector::make(0.2125f, 0.7154f, 0.0721, 0.0f));

static const Color WHITE(1.0f, 1.0f, 1.0f);
static const Color BLACK(0.0f, 0.0f, 0.0f);
static const Color RED(1.0, 0.0f, 0.0f);
static const Color GREEN(0.0f, 1.0f, 0.0f);
static const Color BLUE(0.0f, 0.0f, 1.0f);
static const Color CYAN(0.0f, 1.0f, 1.0f);
static const Color YELLOW(1.0f, 1.0f, 0.0f);
static const Color MAGENTA(1.0f, 0.0f, 1.0f);
static const Color TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);