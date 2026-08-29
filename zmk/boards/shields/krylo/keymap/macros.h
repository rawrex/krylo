#pragma once

#include "config.h"

/* --------------------------------------------------------------------------
 * Layer numbers
 * -------------------------------------------------------------------------- */

#define U_BASE   0
#define U_SYM    1
#define U_NUM    2
#define U_NAV    3
#define U_MEDIA  4
#define U_FUN    5
#define U_EXTRA  6

/* --------------------------------------------------------------------------
 * Cyrillic key aliases (QWERTY keycodes, as in miryoku keys_ru.h)
 * -------------------------------------------------------------------------- */

#define RU_J  Q      // Й
#define RU_TS W      // Ц
#define RU_U  E      // У
#define RU_K  R      // К
#define RU_YE T      // Е
#define RU_N  Y      // Н
#define RU_G  U      // Г
#define RU_SH I      // Ш
#define RU_SHCH O    // Щ
#define RU_Z  P      // З
#define RU_KH LBKT   // Х
#define RU_HARD RBKT // Ъ

#define RU_F  A      // Ф
#define RU_Y  S      // Ы
#define RU_V  D      // В
#define RU_A  F      // А
#define RU_P  G      // П
#define RU_R  H      // Р
#define RU_O  J      // О
#define RU_L  K      // Л
#define RU_D  L      // Д
#define RU_ZH SEMI   // Ж
#define RU_E  SQT    // Э

#define RU_YA Z      // Я
#define RU_CH X      // Ч
#define RU_S  C      // С
#define RU_M  V      // М
#define RU_I  B      // И
#define RU_T  N      // Т
#define RU_SOFT M    // Ь
#define RU_B  COMMA  // Б
#define RU_YU DOT    // Ю

// "ё" symbol is not used

/* --------------------------------------------------------------------------
 * Shared macros
 * -------------------------------------------------------------------------- */

#define U_TAPPING_TERM 200

#define U_NA &none // present but not available for use
#define U_NP &none // key is not present
#define U_NU &none // available but not used

// Clipboard (miryoku_clipboard.h)
#if defined(KRYLO_CLIPBOARD_FUN)
  #define U_RDO &kp K_AGAIN
  #define U_PST &kp K_PASTE
  #define U_CPY &kp K_COPY
  #define U_CUT &kp K_CUT
  #define U_UND &kp K_UNDO
#elif defined(KRYLO_CLIPBOARD_MAC)
  #define U_RDO &kp LS(LG(Z))
  #define U_PST &kp LG(V)
  #define U_CPY &kp LG(C)
  #define U_CUT &kp LG(X)
  #define U_UND &kp LG(Z)
#elif defined(KRYLO_CLIPBOARD_WIN)
  #define U_RDO &kp LC(Y)
  #define U_PST &kp LC(V)
  #define U_CPY &kp LC(C)
  #define U_CUT &kp LC(X)
  #define U_UND &kp LC(Z)
#else
  #define U_RDO &kp K_AGAIN
  #define U_PST &kp LS(INS)
  #define U_CPY &kp LC(INS)
  #define U_CUT &kp LS(DEL)
  #define U_UND &kp K_UNDO
#endif
