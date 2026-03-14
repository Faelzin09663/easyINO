#include "EasyIno.h"

EasyIno::EasyIno(Stream &stream)
    : _stream(stream),
      _variableCount(0),
      _commandCount(0) {}

void EasyIno::begin(unsigned long baudRate) {
  if (&_stream == &Serial) {
    Serial.begin(baudRate);
  }
}

void EasyIno::addVariable(const char *name, int *ptr) {
  if (_variableCount < MAX_VARIABLES) {
    _variableRegistry[_variableCount++] = {name, ptr, "int"};
  }
}

void EasyIno::addVariable(const char *name, float *ptr) {
  if (_variableCount < MAX_VARIABLES) {
    _variableRegistry[_variableCount++] = {name, ptr, "float"};
  }
}

void EasyIno::addVariable(const char *name, bool *ptr) {
  if (_variableCount < MAX_VARIABLES) {
    _variableRegistry[_variableCount++] = {name, ptr, "bool"};
  }
}

void EasyIno::addCommand(const char *name, EasyInoCommandCallback callback) {
  if (_commandCount < MAX_COMMANDS) {
    _commandRegistry[_commandCount++] = {name, callback};
  }
}

void EasyIno::update() {
  if (_stream.available() > 0) {
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    DeserializationError error = deserializeJson(doc, _stream);

    if (error) {
      // Clear buffer on error
      while (_stream.available() > 0) {
        _stream.read();
      }
      return;
    }

    processMessage(doc);
  }
}

void EasyIno::processMessage(StaticJsonDocument<JSON_BUFFER_SIZE> &doc) {
  const char *action = doc["action"];
  const char *name = doc["name"];

  StaticJsonDocument<JSON_BUFFER_SIZE> response;

  if (strcmp(action, "read") == 0) {
    bool found = false;
    for (int i = 0; i < _variableCount; i++) {
      if (strcmp(_variableRegistry[i].name, name) == 0) {
        response["status"] = "ok";
        if (strcmp(_variableRegistry[i].type, "int") == 0) {
          response["value"] = *(int *)_variableRegistry[i].ptr;
        } else if (strcmp(_variableRegistry[i].type, "float") == 0) {
          response["value"] = *(float *)_variableRegistry[i].ptr;
        } else if (strcmp(_variableRegistry[i].type, "bool") == 0) {
          response["value"] = *(bool *)_variableRegistry[i].ptr;
        }
        found = true;
        break;
      }
    }
    if (!found) {
      response["status"] = "error";
      response["message"] = "Variable not found";
    }
  } else if (strcmp(action, "exec") == 0) {
    bool found = false;
    for (int i = 0; i < _commandCount; i++) {
      if (strcmp(_commandRegistry[i].name, name) == 0) {
        _commandRegistry[i].callback();
        response["status"] = "ok";
        found = true;
        break;
      }
    }
    if (!found) {
      response["status"] = "error";
      response["message"] = "Command not found";
    }
  } else {
    response["status"] = "error";
    response["message"] = "Unknown action";
  }

  serializeJson(response, _stream);
  _stream.println();
}

