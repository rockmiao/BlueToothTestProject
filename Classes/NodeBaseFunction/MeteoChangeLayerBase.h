//
//  MeteoChangeLayerBase.h
//  BlueToothTestProject-mobile
//
//  擁有出現/消失動畫的Layer
//
//  Created by Ching-Miao Lin on 2018/9/2.
//

#ifndef MeteoChangeLayerBase_h
#define MeteoChangeLayerBase_h

class MeteoChangeLayerBase {
public:
    //畫面出現/消失
    virtual void layerWillAppear() = 0;
    virtual void layerDidAppear() = 0;
    virtual void layerWillDisappear() = 0;
    virtual void layerDidDisappear() = 0;
};

#endif /* MeteoChangeLayerBase_h */
