#include "EasyIno.h"

EasyIno::EasyIno(Stream& stream) : _stream(stream) {}

void EasyIno::begin(unsigned long baud) {
    if (&_stream == &Serial) {
        Serial.begin(baud);
    }
}

void EasyIno::addVariable(const char* name, int* ptr) {
    if (_varCount < MAX_ITEMS) _vars[_varCount++] = {name, ptr, "int"};
}

void EasyIno::addVariable(const char* name, float* ptr) {
    if (_varCount < MAX_ITEMS) _vars[_varCount++] = {name, ptr, "float"};
}

void EasyIno::addVariable(const char* name, bool* ptr) {
    if (_varCount < MAX_ITEMS) _vars[_varCount++] = {name, ptr, "bool"};
}

void EasyIno::addCommand(const char* name, CommandCallback callback) {
    if (_cmdCount < MAX_ITEMS) _cmds[_cmdCount++] = {name, callback};
}

void EasyIno::notify(const char* eventName, const char* message) {
    JsonDocument res;
    res["event"] = eventName;
    res["value"] = message;
    serializeJson(res, _stream);
    _stream.println();
}

void EasyIno::notify(const char* eventName, int value) {
    JsonDocument res;
    res["event"] = eventName;
    res["value"] = value;
    serializeJson(res, _stream);
    _stream.println();
}

void EasyIno::notify(const char* eventName, float value) {
    JsonDocument res;
    res["event"] = eventName;
    res["value"] = value;
    serializeJson(res, _stream);
    _stream.println();
}

void EasyIno::update() {
    if (_stream.available() > 0) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, _stream);
        if (!error) {
            _process(doc);
        } else {
            while(_stream.available() > 0) _stream.read();
        }
    }
}

void EasyIno::_process(JsonDocument& doc) {
    const char* action = doc["action"];
    const char* name = doc["name"];
    JsonDocument res;

    if (strcmp(action, "read") == 0) {
        for (int i = 0; i < _varCount; i++) {
            if (strcmp(_vars[i].name, name) == 0) {
                res["status"] = "ok";
                if (strcmp(_vars[i].type, "int") == 0) res["value"] = *(int*)_vars[i].ptr;
                else if (strcmp(_vars[i].type, "float") == 0) res["value"] = *(float*)_vars[i].ptr;
                else if (strcmp(_vars[i].type, "bool") == 0) res["value"] = *(bool*)_vars[i].ptr;
                serializeJson(res, _stream);
                _stream.println();
                return;
            }
        }
    } else if (strcmp(action, "exec") == 0) {
        for (int i = 0; i < _cmdCount; i++) {
            if (strcmp(_cmds[i].name, name) == 0) {
                _cmds[i].callback();
                res["status"] = "ok";
                serializeJson(res, _stream);
                _stream.println();
                return;
            }
        }
    } else if (strcmp(action, "ping") == 0) {
        res["status"] = "ok";
        res["value"] = "pong";
        serializeJson(res, _stream);
        _stream.println();
        return;
    } else if (strcmp(action, "schema") == 0) {
        res["status"] = "ok";
        JsonArray vars = res.createNestedArray("variables");
        for (int i = 0; i < _varCount; i++) {
            vars.add(_vars[i].name);
        }
        JsonArray cmds = res.createNestedArray("commands");
        for (int i = 0; i < _cmdCount; i++) {
            cmds.add(_cmds[i].name);
        }
        serializeJson(res, _stream);
        _stream.println();
        return;
    }
    
    res["status"] = "error";
    res["message"] = "Not found";
    serializeJson(res, _stream);
    _stream.println();
}
