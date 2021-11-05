/*
  ==============================================================================

    GFDN.cpp
    Created: 17 Sep 2021 12:31:02pm
    Author:  Orchisama Das

  ==============================================================================
*/

#include "GFDN.h"

GFDN::GFDN(){}
GFDN::~GFDN(){
    delete [] fdns;
    delete [] nDelayLines;
    delete [] mixingAngles;
    delete [] b;
    delete [] c;
}

void GFDN::initialize(int nGrp, float sR, int* nDel, int* LR, int* UR, int numChannels){
    
    nGroups = nGrp;
    fdns = new FDN[nGroups];
    nDelayLines = new int[nGroups];
    mixingAngles = new float[nGroups];
  
    totalDelayLines = 0;
    for(int i = 0; i < nGroups; i++){
        
        nDelayLines[i] = nDel[i];
        fdns[i].initialize(sR, nDelayLines[i], LR[i], UR[i]);
        totalDelayLines += nDelayLines[i];
        mixingAngles[i] = 0.0f;
    }
    
    M_block.resize(totalDelayLines, totalDelayLines);
    couplingMatrix.initialize(nGroups, totalDelayLines);
    
    delayLineOutput.resize(totalDelayLines);
    delayLineOutput.setZero();
    delayLineInput.resize(totalDelayLines);
    delayLineInput.setZero();
    
    //initialize driver and receiver coefficients
    b = new float[totalDelayLines];
    c = new float[totalDelayLines];
    int j = 0;
    for(int i = 0; i < nGroups; i++){
        for(int k = 0; k < nDelayLines[i]; k++){
            b[j] = fdns[i].b[k];
            c[j] = fdns[i].c[k];
            j++;
        }
    }

    output.resize (numChannels, 0.0f);
}


void GFDN::updateMixingMatrix(float mixingFrac, int whichRoom){
    fdns[whichRoom].updateMixingMatrix(mixingFrac);
    mixingAngles[whichRoom] = mixingFrac;
    updateBlockMixingMatrix();
}


void GFDN::updateT60Filter(float t60low, float t60high, float fT, int whichRoom){
    fdns[whichRoom].updateT60Filter(t60low, t60high, fT);
}


void GFDN::updateDryMix(float dry){
    dryMix = dry;
}

void GFDN::updateListenerRoom(int whichRoom){
    //update c coefficients
    
    //set all receiver coefficients to 0
    int j = 0;
    for(int i = 0; i < nGroups; i++){
        for(int k = 0; k < nDelayLines[i]; k++)
            c[j++] = fdns[i].c[k];
    }
    
    //update only the coefficiens in the room where listener is
    int delayLineStart = 0;
    for(int i = 0; i < whichRoom; i++)
        delayLineStart += nDelayLines[i];
    
    for(int k = 0; k < nDelayLines[whichRoom]; k++)
        c[delayLineStart + k] = fdns[whichRoom].c[k];
    
}

void GFDN::updateSourceRoom(int whichRoom){
    //update b coefficients
    
    //set all driver coefficients to 0
    int j = 0;
    for(int i = 0; i < nGroups; i++){
        for(int k = 0; k < nDelayLines[i]; k++)
            b[j++] = fdns[i].b[k];
    }
    
    //update only the coefficients in the room where source is
    int delayLineStart = 0;
    for(int i = 0; i < whichRoom; i++)
        delayLineStart += nDelayLines[i];
    
    for(int k = 0; k < nDelayLines[whichRoom]; k++)
        b[delayLineStart + k] = fdns[whichRoom].b[k];
}



void GFDN::updateCouplingCoeff(float alpha){
    
    couplingMatrix.updateCouplingCoeff(alpha);
    
}


void GFDN::updateBlockMixingMatrix(){
    
    int row = 0, col = 0;

    for(int i = 0; i < nGroups; i++)
    {
        MixingMatrix M = fdns[i].getMixingMatrix();

        for (int j = 0 ; j < nGroups; j++)
        {
            MixingMatrix N = fdns[j].getMixingMatrix();
            
            //diagonal block elements
            if (i == j){
                M_block.block(row, col, nDelayLines[i], nDelayLines[j])  = M.updateMixingMatrix(mixingAngles[i]);
            }
            else{
                M_block.block(row, col, nDelayLines[i], nDelayLines[j]) =
                                                        (N.updateMixingMatrix(mixingAngles[j]/2.0) * M.updateMixingMatrix(mixingAngles[i]/2.0));
            }
            col += nDelayLines[j];
            
        }
        row += nDelayLines[i];
        col = 0;
        
    }
    couplingMatrix.updateBlockMatrix(M_block);
    
}



float* GFDN::processSample(float input[], int numChannels){
    
    // output should be stereo
    for (int chan = 0; chan < numChannels; chan++){
        output[chan] = 0.0;
    }
    
    //variable to keep track of delay line number
    int j = 0;
    int chan = 0;
    
    //loop through groups
    for(int i = 0; i < nGroups; i++){
        
        if(j < totalDelayLines/numChannels)
            chan = 0;
        else
            chan = 1;
        
        //loop through all delay lines in groups
         for(int k = 0; k < nDelayLines[i]; k++){
             fdns[i].buffers[k].update();
             fdns[i].buffers[k].write(b[j]*input[chan] + delayLineInput(j));
             delayLineOutput(j) = fdns[i].buffers[k].read();
             output[chan] += c[j] * delayLineOutput(j);
             j++;
         }
        
    }
    
    for (int chan = 0; chan < numChannels; chan++){
        output[chan] += dryMix * input[chan];
        //std::cout << "Input: " << input[chan] << " Output: " << output[chan] << std::endl;
    }

    
    delayLineInput = couplingMatrix.process(delayLineOutput);
    //std::cout << "Input : " << input << ", Output : " <<  output << std::endl;
    return output.data();

}
