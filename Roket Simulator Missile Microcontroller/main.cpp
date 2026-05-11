#include <iostream>
#include <chrono>
#include <thread>
#include "../../rocket_v2/Layers/HAL/Producers/missile_data.cpp"
#include "../../rocket_v2/Layers/HAL/Producers/target_data.cpp"
#include "../../rocket_v2/Layers/GNC/master.cpp"


int main() {
    std::cout << "=== Guided Missile Simulation ===\n\n";

    MissileData missile_data;
    TargetData  target_data;
    GNCMaster   gnc;

    const float delta_time = 0.020f;
    const int   ticks = 500; // 10 seconds at 50Hz

    for (int i = 0; i < ticks; i++) {

        // update sensors
        missile_data.update();
        target_data.update();

        // feed sensor data into GNC (sensor mode)
        gnc.setMissileState(
            missile_data.getPosition(),
            missile_data.getVelocity(),
            missile_data.getOrientation()
        );
        gnc.setLOSBody(target_data.getLineOfSight());

        // run guidance pipeline
        gnc.update();

        // output - in Unity this drives the missile Rigidbody
        // Vector3f steer = gnc.getOptimalDirection();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<int>(delta_time * 1000)));
    }

    std::cout << "\nDone.\n";
    return 0;
}
