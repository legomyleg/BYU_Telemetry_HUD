#include "hud/draw/hud_draw.hpp"
#include "hud/hud_app.hpp"
#include "raylib.h"


int main(){
    
    auto dummyApp = SetupHudApp();

    while (!WindowShouldClose()) {
        DrawHud(dummyApp);
    }

    dummyApp.unload();

    return 0;
}
