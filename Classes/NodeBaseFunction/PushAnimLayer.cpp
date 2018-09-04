//
//  PushAnimLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "PushAnimLayer.h"

#define SCALE_TIME 0.25
#define ANIM_GAP 0.08

inline void runAnimation(Node *node, float &time, float scale) {
//    float from = scale;
//    float to = !scale;
    node->setScale(scale);
    node->runAction(Sequence::create(DelayTime::create(time), ScaleTo::create(SCALE_TIME, !scale), NULL));
    time += ANIM_GAP;
}

bool PushAnimLayer::init() {
    if (!Layer::init())
        return false;
    
    return true;
}

float PushAnimLayer::appearAnimation(bool animation) {
    Vector<Node *> children = this->getChildren();
    float appearTime = 0;
    for (auto node : this->getChildren()) {
        Menu *menu = dynamic_cast<Menu *>(node);
        if (menu) {
            for (auto menuitem : menu->getChildren()) {
                runAnimation(menuitem, appearTime, 0);
            }
        }
        else {
            runAnimation(node, appearTime, 0);
        }
    }
    return appearTime + SCALE_TIME - ANIM_GAP;
}

float PushAnimLayer::disappearAnimation(bool animation) {
    Vector<Node *> children = this->getChildren();
    float disappearTime = 0;
    for (auto node : this->getChildren())
    {
        Menu *menu = dynamic_cast<Menu *>(node);
        if (menu) {
            for (auto menuitem : menu->getChildren()) {
                runAnimation(menuitem, disappearTime, 1);
            }
        }
        else {
            runAnimation(node, disappearTime, 1);
        }
    }
    return disappearTime + SCALE_TIME - ANIM_GAP;
}
