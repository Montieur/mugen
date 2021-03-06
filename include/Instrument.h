#ifndef MUGEN_INSTRUMENT_H
#define MUGEN_INSTRUMENT_H

#include <vector>
#include <string>
#include "Oscillator.h"
#include "Envelope.h"

using std::vector;

class Instrument {
public:
    Instrument(int blockSize);
    ~Instrument();
    int blockSize;
    vector<Oscillator*> oscillators;
    Envelope env;
    int index = 0;

    void addOscillator();
    void removeOscillator();
    float generateSample(Note* note, double dTime);
    void addToBufferBlock(float *block, Note* note, double timePoint);
};

#endif //MUGEN_INSTRUMENT_H
