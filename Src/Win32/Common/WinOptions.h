// WinOptions.h
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002 Iain Murray, Inference Group, Cavendish, Cambridge.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __WinOptions_h__
#define __WinOptions_h__

#include <string>
#include <map>
#include <windows.h>

// Look for documenting comments here:
#include "../../DasherCore/SettingsStore.h"
#include "../../Common/NoClones.h"

class CWinOptions:public Dasher::CSettingsStore, private NoClones {
public:
  CWinOptions(const std::string & Group, const std::string & Product);
   ~CWinOptions();

private:
  // Platform Specific settings file management
    bool LoadSetting(const std::string & Key, bool * Value);
  bool LoadSetting(const std::string & Key, long *Value);
  bool LoadSetting(const std::string & Key, std::string * Value);
  bool LoadSettingT(const std::string & Key, Tstring * Value);

  void SaveSetting(const std::string & Key, bool Value);
  void SaveSetting(const std::string & Key, long Value);
  void SaveSetting(const std::string & Key, const std::string & Value);

  void SaveSettingT(const std::string & Key, const Tstring & TValue);

  // Platform Specific helpers
  HKEY ProductKey;
  int GetOrCreate(HKEY hKey, LPCTSTR lpSubKey, REGSAM samDesired, HKEY * lpNewKey);
  // CARE! Users of GetlpByte must call delete[] on *Data after use.
  bool GetlpByte(const Tstring & key, BYTE ** Data) const;
};

#endif  /* #ifndef __WinOptions_h__ */
