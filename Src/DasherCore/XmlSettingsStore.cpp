#include "XmlSettingsStore.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>

#if defined(_WIN32) || defined(_WIN64) 
#include "WinUTF8.h"
#define strcasecmp _stricmp 
#define widen(a) WinUTF8::UTF8string_to_wstring((a))
#else
#define widen(a) (a)
#endif

namespace Dasher {
namespace {

template <typename T>
bool Read(const std::map<std::string, T> values, const std::string& key,
          T* value) {
  auto i = values.find(key);
  if (i == values.end()) {
    return false;
  }
  *value = i->second;
  return true;
}

}  // namespace

XmlSettingsStore::XmlSettingsStore(const std::string& filename,
                                   CMessageDisplay* pDisplay)
    : AbstractXMLParser(pDisplay), filename_(filename) {}

bool XmlSettingsStore::Load() {
  bool result = true;
  std::ifstream f(widen(filename_));
  if (f.good()) {
    f.close();
    if (!ParseFile(filename_, true /* user */)) {
      m_pMsgs->Message("Failed to load the XML settings", true /* interrupt */);
      result = false;
    }
  } else {
    m_pMsgs->FormatMessageWithString("XML File not found: ", filename_.c_str());
    result = false;
  }
  // Load all the settings or create defaults for the ones that don't exist.
  // The superclass 'ParseFile' saves default settings if not found.
  mode_ = EXPLICIT_SAVE;
  LoadPersistent();
  mode_ = SAVE_IMMEDIATELY;
  return result;
}

bool XmlSettingsStore::LoadSetting(const std::string& key, bool* value) {
  return Read(boolean_settings_, key, value);
}

bool XmlSettingsStore::LoadSetting(const std::string& key, long* value) {
  return Read(long_settings_, key, value);
}

bool XmlSettingsStore::LoadSetting(const std::string& key, std::string* value) {
  return Read(string_settings_, key, value);
}

void XmlSettingsStore::SaveSetting(const std::string& key, bool value) {
  boolean_settings_[key] = value;
  SaveIfNeeded();
}

void XmlSettingsStore::SaveSetting(const std::string& key, long value) {
  long_settings_[key] = value;
  SaveIfNeeded();
}

void XmlSettingsStore::SaveSetting(const std::string& key,
                                   const std::string& value) {
  string_settings_[key] = value;
  SaveIfNeeded();
}

void XmlSettingsStore::SaveIfNeeded() {
  modified_ = true;
  if (mode_ == SAVE_IMMEDIATELY) {
    Save();
  }
}

bool XmlSettingsStore::Save() {
  if (!modified_) {
    return true;
  }
  try {
    modified_ = false;
    std::ofstream out;
    out.open(widen(filename_), std::ios::out | std::ios::trunc);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n";
    out << "<settings>\n";
    for (const auto& p : long_settings_) {
      out << "<long name=\"" << p.first << "\" value=\"" << p.second
          << "\"/>\n";
    }
    for (const auto& p : boolean_settings_) {
      std::string value = p.second ? "True" : "False";
      out << "<bool name=\"" << p.first << "\" value=\"" << value << "\"/>\n";
    }
    for (const auto& p : string_settings_) {
      std::string attribute = p.second;
      XML_Escape(attribute, true /* attribute */);
      out << "<string name=\"" << p.first << "\" value=\"" << attribute
          << "\"/>\n";
    }
    out << "</settings>\n";
    out.close();
  } catch (...) {
    // TODO(localize).
    m_pMsgs->Message("Failed to save the settings", true /* interrupt */);
    return false;
  }
  return true;
}

bool XmlSettingsStore::GetNameAndValue(const XML_Char** attributes,
                                       std::string* name, std::string* value) {
  bool found_name = false, found_value = false;
  for (; *attributes != nullptr; attributes += 2) {
    if (strcmp(attributes[0], "value") == 0) {
      if (found_value) {
        m_pMsgs->Message(
            "XML configuration: the 'value' attribute can only be present "
            "once in a tag",
            true /* interrupt */);
        return false;
      }
      *value = attributes[1];
      found_value = true;
    } else if (strcmp(attributes[0], "name") == 0) {
      if (found_name) {
        m_pMsgs->Message(
            "XML configuration: the 'name' attribute can only be present "
            "once in a tag",
            true /* interrupt */);
        return false;
      }
      *name = attributes[1];
      if (name->empty()) {
        m_pMsgs->Message(
            "XML configuration: the 'name' attribute can not be empty.",
            true /* interrupt */);
        return false;
      }
      found_name = true;
    } else {
      m_pMsgs->FormatMessageWithString(
          "XML configuration: invalid attribute: %s", *attributes);
      return false;
    }
  }
  if (!found_name || !found_value) {
    m_pMsgs->Message("XML configuration: missing name or value in a tag.",
                     true /* interrupt */);
    return false;
  }
  return true;
}

void XmlSettingsStore::XmlStartHandler(const XML_Char* element_name,
                                       const XML_Char** attributes) {
  std::string element = element_name;
  if (element == "settings") {
    return;
  }
  std::string name, value;
  if (!GetNameAndValue(attributes, &name, &value)) {
    return;
  }
  if (element == "string") {
    string_settings_[name] = value;
  } else if (element == "long") {
    errno = 0;
    long v = std::strtol(value.c_str(), nullptr, 0 /* base */);
    if (errno != 0) {
      m_pMsgs->FormatMessageWith2Strings(
          "XML configuration: invalid numeric value '%s' for '%s'",
          value.c_str(), name.c_str());
    }
    long_settings_[name] = v;
  } else if (element == "bool") {

    if (strcasecmp(value.c_str(), "true") == 0) {
      boolean_settings_[name] = true;
    } else if (strcasecmp(value.c_str(), "false") == 0) {
      boolean_settings_[name] = false;
    } else {
      m_pMsgs->FormatMessageWith2Strings(
          "XML configuration: boolean value should be 'true' or 'false' found "
          "%s = '%s'",
          name.c_str(), value.c_str());
    }
  } else {
    m_pMsgs->FormatMessageWithString("XML configuration: unknown tag '%s'",
                                     element.c_str());
  }
}

void XmlSettingsStore::XmlEndHandler(const XML_Char* ) {}
}  // namespace Dasher
