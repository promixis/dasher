#ifndef __gnomesettingsstore_h__
#define __gnomesettingsstore_h__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

#ifdef WITH_GCONF
#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <gconf/gconf-enum-types.h>
#endif

#ifdef WITH_GSETTINGS
#include <gio/gio.h>
#endif

#include "SettingsStore.h"

class CGnomeSettingsStore:public Dasher::CSettingsStore {
public:
  CGnomeSettingsStore();
  ~CGnomeSettingsStore();

private:
  bool LoadSetting(const std::string & Key, bool * Value) override;
  bool LoadSetting(const std::string & Key, long * Value) override;
  bool LoadSetting(const std::string & Key, std::string * Value) override;

  void SaveSetting(const std::string & Key, bool Value) override;
  void SaveSetting(const std::string & Key, long Value) override;
  void SaveSetting(const std::string & Key, const std::string & Value) override;

#ifdef WITH_GCONF
  GConfClient *the_gconf_client;
  GConfEngine *gconfengine;
#endif
#ifdef WITH_GSETTINGS
  GSettings *settings;
#endif
};

#endif
