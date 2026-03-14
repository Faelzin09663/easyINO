#ifndef EASYINO_H
#define EASYINO_H

#include <Arduino.h>
#include <ArduinoJson.h>

typedef void (*CommandCallback)();

class EasyIno {
public:
    EasyIno(Stream& stream);
    void begin(unsigned long baud);
    void addVariable(const char* name, int* ptr);
    void addVariable(const char* name, float* ptr);
    void addVariable(const char* name, bool* ptr);
    void addCommand(const char* name, CommandCallback callback);
    void update();
    void notify(const char* eventName, const char* message);
    void notify(const char* eventName, int value);
    void notify(const char* eventName, float value);

private:
    struct Variable {
        const char* name;
        void* ptr;
        const char* type;
    };

    struct Command {
        const char* name;
        CommandCallback callback;
    };

    Stream& _stream;
    static const int MAX_ITEMS = 20;
    static const int JSON_SIZE = 256;

    Variable _vars[MAX_ITEMS];
    int _varCount = 0;

    Command _cmds[MAX_ITEMS];
    int _cmdCount = 0;

    void _process(JsonDocument& doc);
};

#endif
