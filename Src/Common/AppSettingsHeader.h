#ifndef __appsettingsheader_h__
#define __appsettingsheader_h__

#include "../DasherCore/Parameters.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

enum { 
  APP_BP_TIME_STAMP = END_OF_SPS, APP_BP_CONFIRM_UNSAVED, APP_BP_SHOW_TOOLBAR,
  APP_BP_SHOW_STATUSBAR, END_OF_APP_BPS
};

enum { 
  APP_LP_EDIT_FONT_SIZE = END_OF_APP_BPS, // TODO Extract font size from APP_SP_EDIT_FONT as linux
  APP_LP_EDIT_HEIGHT,
  APP_LP_EDIT_WIDTH, 
  APP_LP_SCREEN_WIDTH, APP_LP_SCREEN_HEIGHT, APP_LP_SCREEN_WIDTH_H, APP_LP_SCREEN_HEIGHT_H, 
  APP_LP_STYLE, APP_LP_X, APP_LP_Y,
#ifdef WITH_MAEMO
  APP_LP_MAEMO_SIZE,
#endif
  END_OF_APP_LPS
};

enum {
  APP_SP_EDIT_FONT = END_OF_APP_LPS, 
  APP_SP_WINDOW_STATE,  // TODO Use the same settings as linux.
  END_OF_APP_SPS
};

enum {
  APP_STYLE_TRAD = 0,
  APP_STYLE_COMPOSE,
  APP_STYLE_DIRECT,
  APP_STYLE_FULLSCREEN };
#endif
