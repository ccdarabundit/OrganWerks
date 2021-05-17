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
    // insert code here...
    tiir<float, 58> tiir_test;
    tiir_test.updateCoefs(-0.00000237415232038436, -1.21463260814617735051, -0.18763544004887952599);
    // Create an impulse
    float delta[68];
    float out[68];
    for (int i = 0; i < 68; ++i)
    {
        delta[i] = 0;
        out[i] = 0;
    }
    delta[0] = 1.f;
    float* inputs = {&delta[0]};
    float* outputs = {&out[0]};
    tiir_test.compute(68, &inputs, &outputs);
    
    std::ofstream myfile("tiir.txt");
    if (myfile.is_open())
    {
        std::cout << "Printing to file" << std::endl;
    for (int i = 0; i < 68; ++i)
    {
        myfile << outputs[i] ;
        if (i < 67)
            myfile << ", ";
        else
            myfile << "\n";
    }
        
    myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl;
    
    tiirpp<float, 58> tiirpp_test;
    float tiirCoefs[3] = {-0.00000237415232038436, -1.21463260814617735051, -0.18763544004887952599};
    float iirCoefs[3] = {-0.103841983703594, 0.0595728030857674, 0.672771204016737};
    
    tiirpp_test.updateCoefs(tiirCoefs, iirCoefs);
    tiirpp_test.compute(68, &inputs, &outputs);
    
    std::ofstream myfile2("tiirpp.txt");
    if (myfile2.is_open())
    {
        std::cout << "Printing to file" << std::endl;
    for (int i = 0; i < 68; ++i)
    {
        myfile2 << outputs[i] ;
        if (i < 67)
            myfile2 << ", ";
        else
            myfile2 << "\n";
    }
        
    myfile2.close();
    }
    else std::cout << "Unable to open file" << std::endl;
    return 0;
}
