#include "hud/hud_app.hpp"
#include "../src/hud/run_hud.hpp"

int main(){
    auto dummyApp = SetupHudApp();
    RunHud(nullptr);
    return 0;
}
