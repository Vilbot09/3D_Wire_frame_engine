#include "relay/rle.hpp"

class GameClass : public rle::engine3d {
private:
    void OnUserCreate() override {

    }

    void OnUserUpdate() override {

    }
};

int main() {
    GameClass game;
    game.Start();
    return 0;
}