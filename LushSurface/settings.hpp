#ifndef SETTINGS_H
#define SETTINGS_H

class Settings {
public:
    uint16_t MINRESX = 1280, MINRESY = 720;

    double ORTHORATIO = 15.0; //Amount of blocks fitting in greatest screen dimension

    uint8_t VIEWUPDATETICK = 20; //Minimum amount of milliseconds a view waits to update

    uint16_t WINDOWWIDTH = MINRESX, WINDOWHEIGHT = MINRESY;
};

#endif
