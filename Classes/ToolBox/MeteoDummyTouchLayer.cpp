//
//  MeteoDummyTouchLayer.cpp
//  BlueToothTestProject-mobile
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#include "MeteoDummyTouchLayer.h"

MeteoDummyTouchLayer*  MeteoDummyTouchLayer::create(const Size& touchSize, bool debug /* false */)
{
    MeteoDummyTouchLayer *ret = new MeteoDummyTouchLayer();
    if(ret && ret->init(touchSize, debug))
        ret->autorelease();
    else
        CC_SAFE_DELETE(ret);
    return ret;
}

bool MeteoDummyTouchLayer::init(const Size& touchSize, bool debug /* false */)
{
    if( !Layer::init() )
        return false;
    
    _touchSize = touchSize;
    
    if(debug)
    {
        LayerColor *test = LayerColor::create(Color4B(255, 0, 0, 100), touchSize.width, touchSize.height);
        this->addChild(test);
    }
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(MeteoDummyTouchLayer::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

bool MeteoDummyTouchLayer::onTouchBegan(Touch *touch, Event *event)
{
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    if (!this->isVisible())
    {
        return false;
    }
    
    Rect boundingBox = Rect(0, 0, _touchSize.width, _touchSize.height);
    Vec2 touchPosition = this->convertToNodeSpace(touch->getLocation());
    
    if(boundingBox.containsPoint(touchPosition))
        return true;
    else
        return false;
}
