#include "tgl/tgl.h"
#include "widgets.h"

sui_cmd sui_rect(sui_aabb aabb, unsigned char col[4])
{
    return (sui_cmd) {
        .type = SUI_RECT,
        .aabb = aabb,
        .col = {col[0], col[1], col[2], col[3]},
    };
}
