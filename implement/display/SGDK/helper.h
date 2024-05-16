#pragma once

#define DISPLAY_PREPARE_IMAGE(X, PAL) \
    display_prepare(((Image *)X)->palette->data, PAL)

#define DISPLAY_PREPARE_MULTIFONT(X) \
    display_prepare(((multifont *)X)->palette_data, ((multifont *)X)->pal)

#define DISPLAY_PREPARE_SD(X, PAL) \
    display_prepare(((SpriteDefinition *)X)->palette->data, PAL)

#define DISPLAY_PREPARE_PALETTE(X, PAL) \
    display_prepare(((Palette *)X)->data, PAL)

#define DISPLAY_PREPARE_SPRITE(X, PAL) \
    display_prepare(((Sprite *)X)->definition->palette->data, PAL)
