#include "../include/Deserializer.h"
#include "../include/WaveformType.h"

void Deserializer::deserializeInstrument(JSON instJson, Instrument *instrument) {
    deserializeEnvelope(instJson["envelope"], &instrument->env);
    instrument->index = instJson["index"];
    instrument->oscillators.pop_back();
    for (auto oscJson : instJson["oscillators"]){
        auto* newOsc = new Oscillator(44100);
        deserializeOscillator(oscJson, newOsc);
        instrument->oscillators.push_back(newOsc);
    }
}

void Deserializer::deserializeEnvelope(JSON envJson, Envelope *envelope) {
    envelope->initialAmplitude = envJson["initial"];
    envelope->sustainAmplitude = envJson["sustain"];
    envelope->attackDuration = envJson["attack"];
    envelope->decayDuration = envJson["decay"];
    envelope->releaseDuration = envJson["release"];
}

void Deserializer::deserializeOscillator(JSON oscJson, Oscillator* osc){
    osc->freqModifier = oscJson["freq_mod"];
    osc->ampModifier = oscJson["amp_mod"];
    osc->setWaveformType(stringToWavetype(oscJson["m_type"]));
    if (oscJson["lfo"] != nullptr){
        auto* lfo = new Oscillator(44100);
        deserializeLFO(oscJson["lfo"], lfo);
        osc->lfo = lfo;
    }
}

void Deserializer::deserializeLFO(JSON lfoJson, Oscillator* lfo){
    lfo->freqModifier = lfoJson["freq_mod"];
    lfo->ampModifier = lfoJson["amp_mod"];
    lfo->setWaveformType(stringToWavetype(lfoJson["m_type"]));
}