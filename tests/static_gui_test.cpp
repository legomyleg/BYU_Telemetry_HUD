#include <hud/run_hud.hpp>
#include <chrono>

int main(){
    RunHud(nullptr, std::chrono::microseconds(0), true);
    return 0;
}
