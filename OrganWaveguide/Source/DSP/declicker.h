//
//  declicker.h
//  OrganWaveguide
//
//  Created by Champ Darabundit on 5/31/21.
//

#ifndef declicker_h
template<class fType>
class declicker
{
private:
    float fs, lambda, target, state;
public:
    declicker()
    {target = 0; state = 0; fs = 0; lambda=0;}
    
    void initSampleRate(float sampleRate)
    {
        fs = sampleRate; lambda = 1 - std::exp(-1.f/(0.0025*sampleRate));
    }
    float declick()
    {
        state += (lambda)*(target - state);
        return state;
    }
    
    void setTarget(float newTarget)
    {
        target = newTarget;
    }
};
#define declicker_h


#endif /* declicker_h */
