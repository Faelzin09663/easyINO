/*
 * EasyIno.h - Core library for EasyIno firmware
 * This library exposes a small API to register variables and commands
 * that can be controlled over a JSON-based serial protocol.
 */

#ifndef EASYINO_H
#define EASYINO_H

#include <Arduino.h>
#include <ArduinoJson.h>

typedef void (*EasyInoCommandCallback)();

class EasyIno {
public:
  explicit EasyIno(Stream &stream = Serial);

  void begin(unsigned long baudRate = 115200);
  void update();

  void addVariable(const char *name, int *ptr);
  void addVariable(const char *name, float *ptr);
  void addVariable(const char *name, bool *ptr);

  void addCommand(const char *name, EasyInoCommandCallback callback);

private:
  struct Variable {
    const char *name;
    void *ptr;
    const char *type; // "int", "float", "bool"
  };

  struct Command {
    const char *name;
    EasyInoCommandCallback callback;
  };

  static const uint8_t MAX_COMMANDS = 20;
  static const uint8_t MAX_VARIABLES = 20;
  static const size_t JSON_BUFFER_SIZE = 256;

  Stream &_stream;

  Variable _variableRegistry[MAX_VARIABLES];
  int _variableCount;

  Command _commandRegistry[MAX_COMMANDS];
  int _commandCount;

  void processMessage(StaticJsonDocument<JSON_BUFFER_SIZE> &doc);
};

#endif // EASYINO_H

