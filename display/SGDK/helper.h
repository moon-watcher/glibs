#pragma once

#define DISPLAY_IMAGE(X) \
    ((Image *)X)->palette->data

#define DISPLAY_SD(X) \
    ((SpriteDefinition *)X)->palette->data

#define DISPLAY_PALETTE(X) \
    ((Palette *)X)->data

#define DISPLAY_SPRITE(X) \
    ((Sprite *)X)->definition->palette->data
