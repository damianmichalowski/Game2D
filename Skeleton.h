#ifndef SKELETON_H
#define SKELETON_H

#include "Enemy.h"

class Skeleton : public Enemy {
private:

public:
    Skeleton(float x, float y) : Enemy(x, y) {
    }

    void Load() override;
};

#endif // SKELETON_H
