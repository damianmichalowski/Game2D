#ifndef GLOBALS_H
#define GLOBALS_H



class Globals {
private:
    static bool isDebugMode;

public:
    static bool IsDebugMode() {
        return isDebugMode;
    }
};


#endif //GLOBALS_H
