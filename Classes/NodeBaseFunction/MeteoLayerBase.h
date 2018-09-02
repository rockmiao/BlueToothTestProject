//
//  MeteoLayerBase.h
//  BlueToothTestProject-mobile
//
//  擁有出現/消失動畫的Layer
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef MeteoLayerBase_h
#define MeteoLayerBase_h

class MeteoLayerBase {
public:
    //出現/消失動畫
    virtual void appearAnimation() = 0;
    virtual void disAppearAnimation() = 0;
    
    //畫面出現/消失
    virtual void layerWillAppear() = 0;
    virtual void layerDidAppear() = 0;
    virtual void layerWillDisappear() = 0;
    virtual void layerDidDisappear() = 0;
};

#endif /* MeteoLayerBase_h */
