#ifndef COLLISION_H
#define COLLISION_H

#define collides(x, y, rx, ry, rw, rh) \
            x >= rx && x <= rx + rw && \
            y >= ry && y <= ry + rh

#endif
