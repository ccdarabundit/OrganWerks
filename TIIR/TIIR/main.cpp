//
//  main.cpp
//  TIIR
//
//  Created by Champ Darabundit on 5/14/21.
//  Copyright © 2021 Champ Darabundit. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "TIIR.h"

int main(int argc, const char * argv[]) {
    int nTaps = 23;
    tiirModal<float, double, 23> tiirModalTest;
    float poles[2] = {3.594138106591555, -1.141572963219655};
    float res[2] = {-2.602794606723534e-13, -0.003045487708501};
    float tGain[2] = {6.007302545589120e+12, -21.017667305919375};
    tiirModalTest.setCoefs(poles, res, tGain);
    // Create an impulse
    float delta[nTaps+50];
    float out[nTaps+50];
    for (int i = 0; i < nTaps+50; ++i)
    {
        delta[i] = 0;
        out[i] = 0;
    }
    delta[0] = 1.f;
    float* inputs = {&delta[0]};
    float* outputs = {&out[0]};
    tiirModalTest.compute(nTaps+50, &inputs, &outputs);
    
    std::ofstream myfile("tiirModal.csv");
    if (myfile.is_open())
    {
    for (int i = 0; i < nTaps+50; ++i)
    {
        myfile << outputs[i] ;
        if (i < nTaps+50-1)
            myfile << ", ";
        else
            myfile << "\n";
    }
        
    myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl;
    return 0;
}
