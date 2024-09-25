#include "include/Logger.hpp"

int LoggerSettings::logLevel = 3; // Default to info log-level
bool LoggerSettings::useLineNumbers = false; // Default to not printing from which file and line the logger call came from
vector<string> LoggerSettings::enabledOrigins = {"ALL"}; // Default to showing all log areas
bool LoggerSettings::useLightmodeColors = false; // Default to dark-mode colors

bool LoggerSettings::begin(YAML::Node loggerSettingsYAML) {
  try {
    logLevel = loggerSettingsYAML[LOGGER_LOG_LEVEL_KEY_NAME].as<int>();
    useLineNumbers = loggerSettingsYAML[LOGGER_USE_LINE_NUMBERS_KEY_NAME].as<bool>();
    enabledOrigins = loggerSettingsYAML[LOGGER_LOG_AEREAS_KEY_NAME].as<vector<string>>();
    useLightmodeColors = loggerSettingsYAML[LOGGER_USE_LIGHT_MODE_COLORS_KEY_NAME].as<bool>();
    return true;
  } catch (exception &error) {
    cerr << "Standard error while parsing logger config YAML: " << error.what() << endl;
    exit(1);
    return false;
  } catch (...) {
    cerr << "Unknown error while parsing logger config YAML: " << __cxxabiv1::__cxa_current_exception_type()->name() << endl;
    exit(1);
    return false;
  }
}

int LoggerSettings::getLogLevel() {
  return logLevel;
}

bool LoggerSettings::getUseLineNumbers() {
  return useLineNumbers;
}

bool LoggerSettings::getAllAereasAreEnabled() {
  return checkIfOriginIsEnabled(LOGGER_ALLOW_ALL_LOG_AEREAS_AEREA);
}

bool LoggerSettings::getUseLightmodeColors() {
  return useLightmodeColors;
}

bool LoggerSettings::checkIfOriginIsEnabled(string origin) {
  return find(enabledOrigins.begin(), enabledOrigins.end(), origin) != enabledOrigins.end();
}
