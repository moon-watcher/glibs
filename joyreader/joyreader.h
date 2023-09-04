#pragma once

typedef struct
{
    unsigned int port : 2;
    unsigned int (*reader_f)(unsigned int);
    unsigned int changed;
    unsigned int active;
} joyreader;

void joyreader_init(joyreader *const, unsigned int, unsigned int (*)(unsigned int));
void joyreader_update(joyreader *const);
void joyreader_reset(joyreader *const);
void joyreader_press(joyreader *const, unsigned int);

enum {
    JOYREADER_UP    = 0,
    JOYREADER_U     = 0,
    JOYREADER_DOWN  = 1,
    JOYREADER_D     = 1,
    JOYREADER_LEFT  = 2,
    JOYREADER_L     = 2,
    JOYREADER_RIGHT = 3,
    JOYREADER_R     = 3,
    JOYREADER_B     = 4,
    JOYREADER_C     = 5,
    JOYREADER_A     = 6,
    JOYREADER_STAR  = 7,
    JOYREADER_S     = 7,
    JOYREADER_Z     = 8,
    JOYREADER_Y     = 9,
    JOYREADER_X     = 10,
    JOYREADER_MODE  = 11,
    JOYREADER_M     = 11,
};


#define joy_active(J)                 ( J->active )
#define joy_active_up(J)            ( ( joy_active(J)   >> JOYREADER_U ) & 1 )
#define joy_active_u(J)             ( ( joy_active(J)   >> JOYREADER_U ) & 1 )
#define joy_active_down(J)          ( ( joy_active(J)   >> JOYREADER_D ) & 1 )
#define joy_active_d(J)             ( ( joy_active(J)   >> JOYREADER_D ) & 1 )
#define joy_active_left(J)          ( ( joy_active(J)   >> JOYREADER_L ) & 1 )
#define joy_active_l(J)             ( ( joy_active(J)   >> JOYREADER_L ) & 1 )
#define joy_active_right(J)         ( ( joy_active(J)   >> JOYREADER_R ) & 1 )
#define joy_active_r(J)             ( ( joy_active(J)   >> JOYREADER_R ) & 1 )
#define joy_active_a(J)             ( ( joy_active(J)   >> JOYREADER_A ) & 1 )
#define joy_active_b(J)             ( ( joy_active(J)   >> JOYREADER_B ) & 1 )
#define joy_active_c(J)             ( ( joy_active(J)   >> JOYREADER_C ) & 1 )
#define joy_active_x(J)             ( ( joy_active(J)   >> JOYREADER_X ) & 1 )
#define joy_active_y(J)             ( ( joy_active(J)   >> JOYREADER_Y ) & 1 )
#define joy_active_z(J)             ( ( joy_active(J)   >> JOYREADER_Z ) & 1 )
#define joy_active_start(J)         ( ( joy_active(J)   >> JOYREADER_S ) & 1 )
#define joy_active_s(J)             ( ( joy_active(J)   >> JOYREADER_S ) & 1 )
#define joy_active_mode(J)          ( ( joy_active(J)   >> JOYREADER_M ) & 1 )
#define joy_active_m(J)             ( ( joy_active(J)   >> JOYREADER_M ) & 1 )
#define joy_active_vertical(J)        ( joy_active_u(J)     | joy_active_d(J) )
#define joy_active_v(J)               ( joy_active_u(J)     | joy_active_d(J) )
#define joy_active_horizontal(J)      ( joy_active_l(J)     | joy_active_r(J) )
#define joy_active_h(J)               ( joy_active_l(J)     | joy_active_r(J) )
#define joy_active_dir(J)             ( joy_active_v(J)     | joy_active_h(J) )
#define joy_active_abc(J)             ( joy_active_a(J)     | joy_active_b(J)   | joy_active_c(J) )
#define joy_active_xyz(J)             ( joy_active_x(J)     | joy_active_y(J)   | joy_active_z(J) )
#define joy_active_btn(J)             ( joy_active_abc(J)   | joy_active_xyz(J) )

#define joy_changed(J)                ( J->changed )
#define joy_changed_up(J)           ( ( joy_changed(J)  >> JOYREADER_U ) & 1 )
#define joy_changed_u(J)            ( ( joy_changed(J)  >> JOYREADER_U ) & 1 )
#define joy_changed_down(J)         ( ( joy_changed(J)  >> JOYREADER_D ) & 1 )
#define joy_changed_d(J)            ( ( joy_changed(J)  >> JOYREADER_D ) & 1 )
#define joy_changed_left(J)         ( ( joy_changed(J)  >> JOYREADER_L ) & 1 )
#define joy_changed_l(J)            ( ( joy_changed(J)  >> JOYREADER_L ) & 1 )
#define joy_changed_right(J)        ( ( joy_changed(J)  >> JOYREADER_R ) & 1 )
#define joy_changed_r(J)            ( ( joy_changed(J)  >> JOYREADER_R ) & 1 )
#define joy_changed_a(J)            ( ( joy_changed(J)  >> JOYREADER_A ) & 1 )
#define joy_changed_b(J)            ( ( joy_changed(J)  >> JOYREADER_B ) & 1 )
#define joy_changed_c(J)            ( ( joy_changed(J)  >> JOYREADER_C ) & 1 )
#define joy_changed_x(J)            ( ( joy_changed(J)  >> JOYREADER_X ) & 1 )
#define joy_changed_y(J)            ( ( joy_changed(J)  >> JOYREADER_Y ) & 1 )
#define joy_changed_z(J)            ( ( joy_changed(J)  >> JOYREADER_Z ) & 1 )
#define joy_changed_start(J)        ( ( joy_changed(J)  >> JOYREADER_S ) & 1 )
#define joy_changed_s(J)            ( ( joy_changed(J)  >> JOYREADER_S ) & 1 )
#define joy_changed_mode(J)         ( ( joy_changed(J)  >> JOYREADER_M ) & 1 )
#define joy_changed_m(J)            ( ( joy_changed(J)  >> JOYREADER_M ) & 1 )
#define joy_changed_vertical(J)       ( joy_changed_u(J)    | joy_changed_d(J) )
#define joy_changed_v(J)              ( joy_changed_u(J)    | joy_changed_d(J) )
#define joy_changed_horizontal(J)     ( joy_changed_l(J)    | joy_changed_r(J) )
#define joy_changed_h(J)              ( joy_changed_l(J)    | joy_changed_r(J) )
#define joy_changed_dir(J)            ( joy_changed_v(J)    | joy_changed_h(J) )
#define joy_changed_abc(J)            ( joy_changed_a(J)    | joy_changed_b(J)  | joy_changed_c(J) )
#define joy_changed_xyz(J)            ( joy_changed_x(J)    | joy_changed_y(J)  | joy_changed_z(J) )
#define joy_changed_btn(J)            ( joy_changed_abc(J)  | joy_changed_xyz(J) )

#define joy_inactive(J)               ( ~joy_active(J) )
#define joy_inactive_up(J)          ( ( joy_inactive(J) >> JOYREADER_U ) & 1 )
#define joy_inactive_u(J)           ( ( joy_inactive(J) >> JOYREADER_U ) & 1 )
#define joy_inactive_down(J)        ( ( joy_inactive(J) >> JOYREADER_D ) & 1 )
#define joy_inactive_d(J)           ( ( joy_inactive(J) >> JOYREADER_D ) & 1 )
#define joy_inactive_left(J)        ( ( joy_inactive(J) >> JOYREADER_L ) & 1 )
#define joy_inactive_l(J)           ( ( joy_inactive(J) >> JOYREADER_L ) & 1 )
#define joy_inactive_right(J)       ( ( joy_inactive(J) >> JOYREADER_R ) & 1 )
#define joy_inactive_r(J)           ( ( joy_inactive(J) >> JOYREADER_R ) & 1 )
#define joy_inactive_a(J)           ( ( joy_inactive(J) >> JOYREADER_A ) & 1 )
#define joy_inactive_b(J)           ( ( joy_inactive(J) >> JOYREADER_B ) & 1 )
#define joy_inactive_c(J)           ( ( joy_inactive(J) >> JOYREADER_C ) & 1 )
#define joy_inactive_x(J)           ( ( joy_inactive(J) >> JOYREADER_X ) & 1 )
#define joy_inactive_y(J)           ( ( joy_inactive(J) >> JOYREADER_Y ) & 1 )
#define joy_inactive_z(J)           ( ( joy_inactive(J) >> JOYREADER_Z ) & 1 )
#define joy_inactive_start(J)       ( ( joy_inactive(J) >> JOYREADER_S ) & 1 )
#define joy_inactive_s(J)           ( ( joy_inactive(J) >> JOYREADER_S ) & 1 )
#define joy_inactive_mode(J)        ( ( joy_inactive(J) >> JOYREADER_M ) & 1 )
#define joy_inactive_m(J)           ( ( joy_inactive(J) >> JOYREADER_M ) & 1 )
#define joy_inactive_vertical(J)      ( joy_inactive_u(J)   | joy_inactive_d(J) )
#define joy_inactive_v(J)             ( joy_inactive_u(J)   | joy_inactive_d(J) )
#define joy_inactive_horizontal(J)    ( joy_inactive_l(J)   | joy_inactive_r(J) )
#define joy_inactive_h(J)             ( joy_inactive_l(J)   | joy_inactive_r(J) )
#define joy_inactive_dir(J)           ( joy_inactive_v(J)   | joy_inactive_h(J) )
#define joy_inactive_abc(J)           ( joy_inactive_a(J)   | joy_inactive_b(J) | joy_inactive_c(J) )
#define joy_inactive_xyz(J)           ( joy_inactive_x(J)   | joy_inactive_y(J) | joy_inactive_z(J) )
#define joy_inactive_btn(J)           ( joy_inactive_abc(J) | joy_inactive_xyz(J) )

#define joy_pressed(J)                ( joy_active(J)  & joy_changed(J) )
#define joy_pressed_up(J)           ( ( joy_pressed(J)  >> JOYREADER_U ) & 1 )
#define joy_pressed_u(J)            ( ( joy_pressed(J)  >> JOYREADER_U ) & 1 )
#define joy_pressed_down(J)         ( ( joy_pressed(J)  >> JOYREADER_D ) & 1 )
#define joy_pressed_d(J)            ( ( joy_pressed(J)  >> JOYREADER_D ) & 1 )
#define joy_pressed_left(J)         ( ( joy_pressed(J)  >> JOYREADER_L ) & 1 )
#define joy_pressed_l(J)            ( ( joy_pressed(J)  >> JOYREADER_L ) & 1 )
#define joy_pressed_right(J)        ( ( joy_pressed(J)  >> JOYREADER_R ) & 1 )
#define joy_pressed_r(J)            ( ( joy_pressed(J)  >> JOYREADER_R ) & 1 )
#define joy_pressed_a(J)            ( ( joy_pressed(J)  >> JOYREADER_A ) & 1 )
#define joy_pressed_b(J)            ( ( joy_pressed(J)  >> JOYREADER_B ) & 1 )
#define joy_pressed_c(J)            ( ( joy_pressed(J)  >> JOYREADER_C ) & 1 )
#define joy_pressed_x(J)            ( ( joy_pressed(J)  >> JOYREADER_X ) & 1 )
#define joy_pressed_y(J)            ( ( joy_pressed(J)  >> JOYREADER_Y ) & 1 )
#define joy_pressed_z(J)            ( ( joy_pressed(J)  >> JOYREADER_Z ) & 1 )
#define joy_pressed_start(J)        ( ( joy_pressed(J)  >> JOYREADER_S ) & 1 )
#define joy_pressed_s(J)            ( ( joy_pressed(J)  >> JOYREADER_S ) & 1 )
#define joy_pressed_mode(J)         ( ( joy_pressed(J)  >> JOYREADER_M ) & 1 )
#define joy_pressed_m(J)            ( ( joy_pressed(J)  >> JOYREADER_M ) & 1 )
#define joy_pressed_vertical(J)       ( joy_pressed_u(J)    | joy_pressed_d(J) )
#define joy_pressed_v(J)              ( joy_pressed_u(J)    | joy_pressed_d(J) )
#define joy_pressed_horizontal(J)     ( joy_pressed_l(J)    | joy_pressed_r(J) )
#define joy_pressed_h(J)              ( joy_pressed_l(J)    | joy_pressed_r(J) )
#define joy_pressed_dir(J)            ( joy_pressed_v(J)    | joy_pressed_h(J) )
#define joy_pressed_abc(J)            ( joy_pressed_a(J)    | joy_pressed_b(J)  | joy_pressed_c(J) )
#define joy_pressed_xyz(J)            ( joy_pressed_x(J)    | joy_pressed_y(J)  | joy_pressed_z(J) )
#define joy_pressed_btn(J)            ( joy_pressed_abc(J)  | joy_pressed_xyz(J) )

#define joy_released(J)               ( ~joy_active(J) & joy_changed(J) )
#define joy_released_up(J)          ( ( joy_released(J) >> JOYREADER_U ) & 1 )
#define joy_released_u(J)           ( ( joy_released(J) >> JOYREADER_U ) & 1 )
#define joy_released_down(J)        ( ( joy_released(J) >> JOYREADER_D ) & 1 )
#define joy_released_d(J)           ( ( joy_released(J) >> JOYREADER_D ) & 1 )
#define joy_released_left(J)        ( ( joy_released(J) >> JOYREADER_L ) & 1 )
#define joy_released_l(J)           ( ( joy_released(J) >> JOYREADER_L ) & 1 )
#define joy_released_right(J)       ( ( joy_released(J) >> JOYREADER_R ) & 1 )
#define joy_released_r(J)           ( ( joy_released(J) >> JOYREADER_R ) & 1 )
#define joy_released_a(J)           ( ( joy_released(J) >> JOYREADER_A ) & 1 )
#define joy_released_b(J)           ( ( joy_released(J) >> JOYREADER_B ) & 1 )
#define joy_released_c(J)           ( ( joy_released(J) >> JOYREADER_C ) & 1 )
#define joy_released_x(J)           ( ( joy_released(J) >> JOYREADER_X ) & 1 )
#define joy_released_y(J)           ( ( joy_released(J) >> JOYREADER_Y ) & 1 )
#define joy_released_z(J)           ( ( joy_released(J) >> JOYREADER_Z ) & 1 )
#define joy_released_start(J)       ( ( joy_released(J) >> JOYREADER_S ) & 1 )
#define joy_released_s(J)           ( ( joy_released(J) >> JOYREADER_S ) & 1 )
#define joy_released_mode(J)        ( ( joy_released(J) >> JOYREADER_M ) & 1 )
#define joy_released_m(J)           ( ( joy_released(J) >> JOYREADER_M ) & 1 )
#define joy_released_vertical(J)      ( joy_released_u(J)   | joy_released_d(J) )
#define joy_released_v(J)             ( joy_released_u(J)   | joy_released_d(J) )
#define joy_released_horizontal(J)    ( joy_released_l(J)   | joy_released_r(J) )
#define joy_released_h(J)             ( joy_released_l(J)   | joy_released_r(J) )
#define joy_released_dir(J)           ( joy_released_v(J)   | joy_released_h(J) )
#define joy_released_abc(J)           ( joy_released_a(J)   | joy_released_b(J) | joy_released_c(J) )
#define joy_released_xyz(J)           ( joy_released_x(J)   | joy_released_y(J) | joy_released_z(J) )
#define joy_released_btn(J)           ( joy_released_abc(J) | joy_released_xyz(J) )