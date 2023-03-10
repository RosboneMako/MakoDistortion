/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


//R1.00 Structure to hold our FILTER CALC VARS.
struct tp_coeffs {
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    float c0;
    float d0;
};

//==============================================================================
/**
*/
class MakoDist01AudioProcessor : public juce::AudioProcessor
#if JucePlugin_Enable_ARA
    , public juce::AudioProcessorARAExtension
#endif
{



public:
    //==============================================================================
    MakoDist01AudioProcessor();
    ~MakoDist01AudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void Mako_SettingsUpdate();
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;


    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================

    //R1.00 DAW SAVE/LOAD PARAMETERS
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState parameters;

    //****************************************************************************************************************
    //R1.00 IR CABS left and Right.
    //****************************************************************************************************************
    float TIRL[1025] = {};  //R1.00 LEFT speaker.
    float TIRR[1025] = {};  //R1.00 RIGHT speaker.
    float TIRT[1025] = {};  //R1.00 Temp array for building edited IRs. Faster than allocating in function???

    //R1.00 MAKO vars.
    float makoSampleRate = 48000.0f;
    int makoSettingsChanged = 0;       //R1.00 FLAG that editor has changed vars that need updated in processor.
    int makoSettingsClearbuffers = 0;  //R1.00 FLAG that editor has changed vars that need updated in processor.

    //R1.00 Amp/Gain
    float makoVol = .25f;       //R1.00 Input Volume. 
    int makoChan = 0;           //R1.00 Amp Channel - 0-Clean,1-Edge,2-Rock,3-Metal
    float makoBreak =  .7f;     //R1.00 Angle of our KNEE OVER distortion.             
    float makoSlope = 0.0f;     //R1.00 Roundness of the KNEE OVER curve. 
    float makoAsym1 = 0.0f;     //R1.00 Do additional gain on negative half to create asymmetry seen in some tube setups. 

    //R1.00 Mako OPTIONs
    float makoOD = .0f;         //R1.00 OverDrive Input FX.
    float makoEnvFade = 0.0f;   //R1.00 Envelope Fade In / Attack.       
    float makoBiteGain = 0.0f;  //R1.00 Additional high freq signal added back in. 
    float makoFrenzy = 0.0f;    //R1.00 Create a volume envelope based on the incoming clean signal. Add dynamics to high gain. 
    float makoGate = .01f;      //R1.00 Crude and meh noise gate.
    float makoparmTube = 0;     //R1.00 Envelope filter at input to add some WAH. Helps cut noise also. 

    //R1.00 MUD - High Pass pre gain filter. 
    float makoTone = 190;
    float makoTone_Last = 190;
    float makoTone_Min = 50;
    float makoTone_Max = 550;

    //R1.00 MID BOOST before gain. 
    float makoTone5 = 1000;
    float makoTone5_Last = 1000;
    float makoTone5_Min = 400.0f;
    float makoTone5_Max = 1200.0f;
    float makoTone5_Q = .707f;
    float makoTone5_Q_Last = .707f;
    float makoTone5_Q_Min = .1f;
    float makoTone5_Q_Max = 2.0f;

    //R1.00 Mid EQ Final
    float makoTone6 = 700.0f;
    float makoTone6_Last = 700.0f;
    float makoTone6_Min = 200.0f;
    float makoTone6_Max = 1600.0f;
    float makoTone6_Q = .707f;
    float makoTone6_Q_Last = .707f;
    float makoTone6_Q_Min = .1f;
    float makoTone6_Q_Max = 2.0f;

    //R1.00 Mix High Pass.
    float makoMixHP = 50.0f;
    float makoMixHP_Last = 50.0f;
    float makoMixHP_Min = 50.0f;
    float makoMixHP_Max = 550.0f;

    //R1.00 FIZZ - Low Pass post gain filter. 
    float makoTone2 = 5200;
    float makoTone2_Last = 5200;
    float makoTone2_Min = 500;
    float makoTone2_Max = 5500;

    //R1.00 Stage 2 Gain settings.
    //float makoBreak2 = 1;
    //float makoSlope2 = 0;
    float makoPowerAmp = 0.0f;          //R1.00 Final EQ before/after Power Amp.
    int makoEQPreCut = 0;               //R1.00 Gain High Cut Pre Power/Post Power selector. 

    //R1.00 Clean Mix settings.
    float makoMix = 1.0f;

    //R1.00 Tuner A
    int makoTune = 0;
    int makoTuneNote = 1;   //R1.00       E,      A,        D,       G,        B,         E,        C,       Eb,       Db,        B,       EB,     AE,       DA,
    float makoTuneF1[20] = { 0.0f, 164.814f, 220.0f, 293.664f, 195.998f, 246.942f, 329.628f, 261.626f, 155.563f, 138.591f, 123.471f,  82.407f, 110.0f,  73.416f, };
    float makoTuneH1[20] = { 0.0f,     0.0f,    .0f,     0.0f,     0.0f,     0.0f,     0.0f,     0.0f,     0.0f,     0.0f,     0.0f,     3.0f,   3.0f,     3.0f, };

    float makoTuneA1[2] = { };
    float makoTuneStp1[2] = { };
     
    //R1.00 Frequency Test
    int makoFreqTest = 0;
    int makoFreqTestStart = 0;
    int makoFreqTest_F[2] = { };
    float makoFreqTest_A[2] = { };
    float makoFreqTest_Stp[2] = { };
    float makoFreqTestMax = 0.0f;
    int makoFreqGfxStart = 0;
    int makoFreqGfxStop = 0;
    float makoFreqGfxAmpl[20002] = { };

    //R1.00 Final EQ Settings.
    int makoInvMid = 0;                       //R1.00 invert the phase of the MID filter.
    float makoTreble = .6;
    float makoTrebleCut = 1500;
    float makoMid = .2;
    float makoBass = .5;
    float makoBassCut = 150;
    int makoFinalMid = 0;
    float makoFinalBal[2] = { 1.0f, 1.0f };
    float makoFinalWidth = 0.0f;              //R1.00 Stereo Delay effect. Should rework to go left or right.  
    float makoVol2 = .35;
    float makoVol2_Setting = .35;
    
    //R1.00 Noise gate operational variables.. 
    float makoparmNGate = 0;
    float makoparmNGate_Adj = 0;
    float makoparmNGate_Last = 1;
    float makoparmNGate_Min = 0;
    float makoparmNGate_Max = 1;
    int makoNGate_Trig[5] = { 0 };
    float makoNGate_Max[5] = { 0.0f };
    float makoNGate_Fac[5] = { 0.0f };
        
    //R1.00 Compressor operational variables.
    float makoComp[5] = { 0.0f };
    float makoComp_Max[5] = { 0.0f };           //R1.00 The loudest sample we have seen. 
    float makoComp2_Max[5] = { 0.0f };          //R1.00 The loudest sample we have seen. 
    float makoComp3_Max[5] = { 0.0f };          //R1.00 The loudest sample we have seen. 
    float makoComp_Gain[5] = { .0f };
    float makoComp2_Gain[5] = { .0f };
    float makoComp3_Gain[5] = { .0f };
    float makoparmComp_Release = .0f;           //R1.00 We slowly decrease volume from our compressor gain.
    float makoparmComp_Gain = .0f;
    float makoparmComp2_Gain = .0f;
    float makoparmComp3_Gain = .0f;
    float makoparmComp_Gain_Last = .0f;
    float makoComp_Gain_Max = 1.0f;
    float makoComp2_Gain_Max = 1.0f;
    float makoComp3_Gain_Max = 1.0f;
    //float makoparmComp_Gain_Min = .01f;
    //float makoparmComp_Gain_Max = 1.0f;

    //R1.00 Modulation Effect #1 Settings.
    int makoModType = 1;
    int makoModType_Last = 1;
    int makoModLoc = 5;
    int makoModLoc_Last = 5;
    float makoModRate = .34f;
    float makoModRate_Last = .5f;
    float makoModRate_Min = .01f;
    float makoModRate_Max = 1.0f;
    float makoModMix = .25f;
    float makoModMix_Last = .25f;
    float makoModMix_Min = 0.0f;
    float makoModMix_Max = 1.0f;
    float makoModDepth = .05f;
    float makoModDepth_Last = .05f;
    float makoModDepth_Min = 0.0f;
    float makoModDepth_Max = 1.0f;

    //R1.00 Modulation Effect #2 Settings.
    int makoMod2Type = 1;
    int makoMod2Type_Last = 1;
    int makoMod2Loc = 1;
    int makoMod2Loc_Last = 1;
    float makoMod2Rate = .34f;
    float makoMod2Rate_Last = .5f;
    float makoMod2Rate_Min = .01f;
    float makoMod2Rate_Max = 1.0f;
    float makoMod2Mix = .5f;
    float makoMod2Mix_Last = .5f;
    float makoMod2Mix_Min = 0.0f;
    float makoMod2Mix_Max = 1.0f;
    float makoMod2Depth = .5f;
    float makoMod2Depth_Last = .5f;
    float makoMod2Depth_Min = 0.0f;
    float makoMod2Depth_Max = 1.0f;

    //R1.00 Delay Settings.
    float makoDelayOffset = 1.0f;         //R1.00 Mono - Stereo 0-Mono, 1-Stereo
    float makoDelayOffset_Last = 1.0f;    //R1.00 Mono - Stereo 0-Mono, 1-Stereo
    float makoDelayTime = .5f;
    float makoDelayTime_Last = .5f;
    float makoDelayTime_Min = .01f;
    float makoDelayTime_Max = 1.0f;
    float makoDelayRepeat = .25f;
    float makoDelayMix = .3f;
    float makoDelayDry = 1.0f;
    float makoDelayWet = .6f;
    float makoDelayMix_Last = .3f;
    float makoDelayMix_Min = .0f;
    float makoDelayMix_Max = 1.0f;
    float makoDelayBal = .5f;
    float makoDelayBalLR[2] = { 1.0f, 1.0f };
    
    //R1.00 Reverb Settings.
    float makoReverbMix = .3f;
    float makoReverbDry = 1.0f;
    float makoReverbWet = .6f;
    float makoReverbTime = .75f;
    float makoReverbTimeInv = .25f;
    float makoReverbBal = .5f;
    float makoReverbBalLR[2] = { 1.0f, 1.0f };
    float makoReverbMix_Last = .3f;
    float makoReverbMix_Min = .0f;
    float makoReverbMix_Max = 1.0f;

    //R1.00 Impulse Response Variables.
    int makoIRL = 2;                  //R1.00 IR Cabinet selector.
    int makoIRL_Last = 2;         
    int makoIRR = 7;                  
    int makoIRR_Last = 7;         
    float makoIRLMix = .5f;           //R1.00 IR Voice adjustment.
    float makoIRLMix_Last = .5f;
    float makoIRRMix = .5f;           
    float makoIRRMix_Last = .5f;
    float makoIRLSize = .5f;           //R1.00 IR Speaker Size.
    float makoIRLSize_Last = .5f;
    float makoIRRSize = .5f;
    float makoIRRSize_Last = .5f;
    int makoIR_DoingMax = 0;

    //R1.00 Temporary IR vars used while doing IR calcs.
    //R1.00 Dont want editor changing values while we are calculating.
    //R1.00 Probably not needed, but want to be safe.
    int makoTempIRL = makoIRL;
    int makoTempIRR = makoIRR;
    float makoTempIRLMix = makoIRLMix;
    float makoTempIRRMix = makoIRRMix;
    float makoTempIRLSize = makoIRLSize;
    float makoTempIRRSize = makoIRRSize;

    //R1.00 Next version will have different final EQ types. 
    int makoEQType = 1;
    int makoEQType_Last = 0;

    //R1.00 VU meter and debugging vars.
    int makoClipStage = 0;
    int makoClip_Display = 0;
    int makoClip_Display_Last = 0;
    int makoClip_Display2 = 0;
    float makoClip_Peak = 0.0f;
    float makoClip_Peak2 = 0.0f;
        
    //R1.00 Vars used to display Debug info in Editor.
    int makoDBG_Int = 0;
    float makoDBG_Float = 0;

    //***************************************************************************************************
    //R1.00 DSP Filter calculations - Poorly hacked up by me. 
    //R1.00 Based on Will Pirkle book "Designing Audio Effect Plugins in C++" Chap 11.3 BiQuad Filters
    //R1.00 y(n) = a0*x(n) + a1*x(n-1) + a2*x(n-2) - b*y(n-1) + b2*y(n-2)
    //R1.00 https://en.wikipedia.org/wiki/Digital_biquad_filter
    //R1.00 Many types of filters can be made by calculating coefficients used in BiQuad. 
    //R1.00 Coeff routines calc the coeffs for each type of filter.  
    //R1.00 Based on https://github.com/dimtass/dsp-c-filters     
    //***************************************************************************************************
    void F1_Input_High_coeffs(float fc);            //R1.00 Input High Pass Filter (Mud).
    void F2_PostGain_Low_coeffs(float fc);          //R1.00 Post Gain Low Pass Filter (Fizz).
    void F12_Final_LowCut_coeffs(float fc);         //R1.00 Final Low Cut Filter.
    void F3_Final_Treble_coeffs(float fc);          //R1.00 Final EQ Treble.
    void F4_Final_Bass_coeffs(float fc);            //R1.00 Final EQ Bass.
    void F5_Input_Mid_coeffs(float q, int fc);      //R1.00 Input Band Pass (Amps 0-2).
    void F6_Final_Mid_coeffs(float q, int fc);      //R1.00 Final EQ Mid.
    void F6_Final_Mid_coeffs_BW(float bw, int fc);  //R1.00 Final EQ Mid using Bandwidth.
    void F7_Clean_High_coeffs(float fc);            //R1.00 Clean Path input High Pass.
        
    //R1.00 Phaser All Pass Filters
    //R1.00 Pirkle design uses 6, we used 4.
    void F10_Phaser_Init();                                                                        
    void F10a_Phaser_coeffs(float fc);
    void F10b_Phaser_coeffs(float fc);
    void F10c_Phaser_coeffs(float fc);
    void F10d_Phaser_coeffs(float fc);
    //void F10ePhaser_coeffs(float fc);
    //void F10f_Phaser_coeffs(float fc);

    //R1.00 Phaser 2 All Pass Filters
    void F10_Phaser2_Init();
    void F10a_Phaser2_coeffs(float fc);
    void F10b_Phaser2_coeffs(float fc);
    void F10c_Phaser2_coeffs(float fc);
    void F10d_Phaser2_coeffs(float fc);
    //void F10e_Phaser2_coeffs(float fc);
    //void F10f_Phaser2_coeffs(float fc);

    //R1.00 MOD FILTER WAH and AUTOWAH use the same variables. 
    void F11_AutoWah_coeffs(float Q, float fc);               
    void F11_AutoWah2_coeffs(float Q, float fc);
    
    float Mako_EQ_Input_Filters(float tSample, int channel);
    float Mako_EQ_Fizz_Filter(float tSample, int channel);
    float Mako_EQ_Dry_HighPass(float tSample,int channel);

    float Mako_Clip(float tSample);
    float Mako_FX_Dist_Stage1(float tSample);   
    float Mako_FX_Chan3_Dist_Dry(float tSample);

    //R1.00 Various Effects routines. 
    float Mako_FX_OD(float tSample);        //R1.00 Mako calculation wave shaper. 
    float Mako_FX_ODHT(float tSample);      //R1.00 Hyperbolic Tangent wave shape.
    
    float Mako_FX_Compress_1(float tSample, int channel);
    float Mako_FX_Compress_2(float tSample, int channel);    
    float Mako_FX_Compress_Clean(float tSample, int channel);

    float Mako_FX_NoiseGate(float tSample, int channel);
    float Mako_FX_Attack(float tSample, int channel);
    float Mako_FX_Tube(float tSample, int channel);
    float Mako_FX_Bite(float tSample, int channel);

    float Mako_EQ_Final(float tSample, int channel);

    float Mako_FX_Reverb(float tSample, int channel);
    float Mako_FX_ReverbChorus(float tSample, int channel);
    float Mako_FX_Reverb_ModFilter(float tSample, int channel);  //R1.00 not used.
    float Mako_FX_Delay(float tSample, int channel);

    float Mako_FX_Width(float tSample);

    float Mako_Modulate(float tSample, int channel);
    float Mako_FX_Chorus(float tSample, int channel);
    float Mako_FX_Tremolo(float tSample, int channel);
    float Mako_FX_ModFilter(float tSample, int channel);
    float Mako_FX_Phaser(float tSample, int channel);
    float Mako_FX_AutoWah(float tSample, int channel);
    float Mako_FX_ModWah(float tSample, int channel);
    
    float Mako_Modulate2(float tSample, int channel);
    float Mako_FX_Chorus2(float tSample, int channel);
    float Mako_FX_Tremolo2(float tSample, int channel);
    float Mako_FX_ModWah2(float tSample, int channel);
    float Mako_FX_Phaser2(float tSample, int channel);
    float Mako_FX_AutoWah2(float tSample, int channel);

    //R1.00 Impulse Response routines. 
    float Mako_CabSim(float tSample, int channel);
    void Mako_SetIR_Left();
    void Mako_SetIR_Right();

    float Mako_Volume_Final(float tSample, int channel);

    float Mako_FreqTest(int channel);
    float Mako_TuneA(int channel);
    

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MakoDist01AudioProcessor)


    //*****************************************************************
    // MAKO VARIABLES
    // Vars that process audio must be at least an array [2] since they
    //  handle two channels (chan 0-left and 1-right) of audio.
    //*****************************************************************
    float pi = 3.14159265;
    float pi2 = 3.14159265 * 2.0;
    float sqrt2 = (2.0 * 0.70710678);
    float sqrt2over2 = 0.70710678;

    float TuneTime = 0;
    long TuneCross0 = 0;
    long TuneCross1 = 0;
    long TuneCross2 = 0;
    long TuneCross3 = 0;
    float TuneLast = -1.0f;
    float TuneLPLast = -1.0f;
    long TuneCount = 0;
    float TuneMax = 0.0f;

    float f1a_xn[2] = {}, f1a_xnz1[2] = {}, f1a_xnz2[2] = {}, f1a_ynz1[2] = {}, f1a_ynz2[2] = {}, f1a_offset[2] = {};
    float f1b_xn[2] = {}, f1b_xnz1[2] = {}, f1b_xnz2[2] = {}, f1b_ynz1[2] = {}, f1b_ynz2[2] = {}, f1b_offset[2] = {};
    float f2_xn[2] = {}, f2_xnz1[2] = {}, f2_xnz2[2] = {}, f2_ynz1[2] = {}, f2_ynz2[2] = {}, f2_offset[2] = {};
    float f3_xn[2] = {}, f3_xnz1[2] = {}, f3_xnz2[2] = {}, f3_ynz1[2] = {}, f3_ynz2[2] = {}, f3_offset[2] = {};
    float f4_xn[2] = {}, f4_xnz1[2] = {}, f4_xnz2[2] = {}, f4_ynz1[2] = {}, f4_ynz2[2] = {}, f4_offset[2] = {};
    float f5_xn[2] = {}, f5_xnz1[2] = {}, f5_xnz2[2] = {}, f5_ynz1[2] = {}, f5_ynz2[2] = {}, f5_offset[2] = {};
    float f6_xn[2] = {}, f6_xnz1[2] = {}, f6_xnz2[2] = {}, f6_ynz1[2] = {}, f6_ynz2[2] = {}, f6_offset[2] = {};
    float f7_xn[2] = {}, f7_xnz1[2] = {}, f7_xnz2[2] = {}, f7_ynz1[2] = {}, f7_ynz2[2] = {}, f7_offset[2] = {};

    float f10a_xn[2] = {}, f10a_xnz1[2] = {}, f10a_xnz2[2] = {}, f10a_ynz1[2] = {}, f10a_ynz2[2] = {}, f10a_offset[2] = {};
    float f10b_xn[2] = {}, f10b_xnz1[2] = {}, f10b_xnz2[2] = {}, f10b_ynz1[2] = {}, f10b_ynz2[2] = {}, f10b_offset[2] = {};
    float f10c_xn[2] = {}, f10c_xnz1[2] = {}, f10c_xnz2[2] = {}, f10c_ynz1[2] = {}, f10c_ynz2[2] = {}, f10c_offset[2] = {};
    float f10d_xn[2] = {}, f10d_xnz1[2] = {}, f10d_xnz2[2] = {}, f10d_ynz1[2] = {}, f10d_ynz2[2] = {}, f10d_offset[2] = {};
    float f10e_xn[2] = {}, f10e_xnz1[2] = {}, f10e_xnz2[2] = {}, f10e_ynz1[2] = {}, f10e_ynz2[2] = {}, f10e_offset[2] = {};
    float f10f_xn[2] = {}, f10f_xnz1[2] = {}, f10f_xnz2[2] = {}, f10f_ynz1[2] = {}, f10f_ynz2[2] = {}, f10f_offset[2] = {};

    float fM2_10a_xn[2] = {}, fM2_10a_xnz1[2] = {}, fM2_10a_xnz2[2] = {}, fM2_10a_ynz1[2] = {}, fM2_10a_ynz2[2] = {}, fM2_10a_offset[2] = {};
    float fM2_10b_xn[2] = {}, fM2_10b_xnz1[2] = {}, fM2_10b_xnz2[2] = {}, fM2_10b_ynz1[2] = {}, fM2_10b_ynz2[2] = {}, fM2_10b_offset[2] = {};
    float fM2_10c_xn[2] = {}, fM2_10c_xnz1[2] = {}, fM2_10c_xnz2[2] = {}, fM2_10c_ynz1[2] = {}, fM2_10c_ynz2[2] = {}, fM2_10c_offset[2] = {};
    float fM2_10d_xn[2] = {}, fM2_10d_xnz1[2] = {}, fM2_10d_xnz2[2] = {}, fM2_10d_ynz1[2] = {}, fM2_10d_ynz2[2] = {}, fM2_10d_offset[2] = {};

    float f11_xn[2] = {}, f11_xnz1[2] = {}, f11_xnz2[2] = {}, f11_ynz1[2] = {}, f11_ynz2[2] = {}, f11_offset[2] = {};
    float fM2_11_xn[2] = {}, fM2_11_xnz1[2] = {}, fM2_11_xnz2[2] = {}, fM2_11_ynz1[2] = {}, fM2_11_ynz2[2] = {}, fM2_11_offset[2] = {};       //R1.00 Mod 2

    float f12_xn[2] = {}, f12_xnz1[2] = {}, f12_xnz2[2] = {}, f12_ynz1[2] = {}, f12_ynz2[2] = {}, f12_offset[2] = {};

    //R1.00 BiQuad Filter coefficients.
    tp_coeffs f1a_coeffs = {};    //R1.00 Mud HPF 1
    tp_coeffs f1b_coeffs = {};    //R1.00 Mud HPF 2
    tp_coeffs f5_coeffs = {};     //R1.00 Pre Gain Mid boost.
    tp_coeffs f2_coeffs = {};     //R1.00 Fizz LPF
    tp_coeffs f3_coeffs = {};     //R1.00 Treble HPF
    tp_coeffs f32_coeffs = {};    //R1.00 Treble HPF Shelf
    tp_coeffs f4_coeffs = {};     //R1.00 Bass HPF
    tp_coeffs f42_coeffs = {};    //R1.00 Bass HPF Shelf
    tp_coeffs f6_coeffs = {};     //R1.00 Final Mid EQ.
    tp_coeffs f7_coeffs = {};     //R1.00 Clean Mix High Pass.
    
    tp_coeffs f10a_coeffs = {};   //R1.00 PHASER
    tp_coeffs f10b_coeffs = {};
    tp_coeffs f10c_coeffs = {};
    tp_coeffs f10d_coeffs = {};
    tp_coeffs f10e_coeffs = {};
    tp_coeffs f10f_coeffs = {};

    tp_coeffs fM2_10a_coeffs = {};   //R1.00 PHASER
    tp_coeffs fM2_10b_coeffs = {};
    tp_coeffs fM2_10c_coeffs = {};
    tp_coeffs fM2_10d_coeffs = {};

    tp_coeffs f11_coeffs = {};       //R1.00 AutoWah
    tp_coeffs fM2_f11_coeffs = {};   //R1.00 AutoWah

    tp_coeffs f12_coeffs = {};      //R1.00 Final Low EQ CUT.

    //R1.00 Pre Calc some Linear amplitude calcs.
    float LinearGain_3dB = pow(10.0, 3.0 / 20.0);
    float LinearGain_6dB = pow(10.0, 6.0 / 20.0);
    float LinearGain_12dB = pow(10.0, 12.0 / 20.0);
    float LinearGain_20dB = pow(10.0, 20.0 / 20.0);
    float LinearGain_3dB_Inv = 1.0f / pow(10.0, 3.0 / 20.0);
    float LinearGain_6dB_Inv = 1.0f / pow(10.0, 6.0 / 20.0);
    float LinearGain_12dB_Inv = 1.0f / pow(10.0, 12.0 / 20.0);
    float LinearGain_20dB_Inv = 1.0f / pow(10.0, 20.0 / 20.0);

    float Release_100mS = 0.0f;
    float Release_200mS = 0.0f;
    float Release_300mS = 0.0f;
    float Release_400mS = 0.0f;
    float Release_500mS = 0.0f;

    float Signal_Last[2] = {};
    float Signal_AVG[2] = {};
    float Signal_AVG_Acc[2] = {};
    int Signal_AVG_Cnt[2] = {};
    float Signal_Max[2] = {};
    float Signal_VolFade[2] = {};
    bool Signal_VolFadeOn[2] = { false, false };
    bool Signal_VolFadeReset[2] = { false, false };

    float DFG = 0.0f;
    float makoTreble_Last = .5f;
    float makoBass_Last = .5f;
    float makoTrebleCut_Last = 1500;
    float makoBassCut_Last = 150;
    float Chan0_Sample = 0;
    float FrenzyVol[2] = { .0f };

    float MixHP_Sample[2] = {};
    float MixHP_SampleRaw[2] = {};

    float Reverb_B[2][192010] = {};
    float Reverb_B_Last[2] = {};
    int   Reverb_B_Ring1[2][20] = {};
    float Reverb_B_Ring1_Gain[20] = {};
    float Reverb_B_Ring1_GainInv[20] = {};
    int   Reverb_B_Ring1_Max[2] = {};
    float RevMod_B[2][8010] = {};
    int   RevMod_B_Ring1[2] = {};
    int   RevMod_B_Ring1_Max[2] = {};
    float RevMod_B_Offset[2] = {};
    float RevMod_B_Offset_Min[2] = {};
    float RevMod_B_Offset_Max[2] = {};
    float RevMod_B_Rate = 0;
    float RevMod_B_Rate_Up = 0;
    float RevMod_B_Rate_Down = 0;

    float FinalWidth_B[4001] = { };       //R1.00 This could be larger for higher sample rates. 
    int FinalWidth_B_Ring =  0;
    int FinalWidth_B_Ring_Max = 0;

    float Delay_B[2][192010] = { };   //R1.00 T192000 is used to clear buffers in Settings Update. 
    int Delay_B_Ring1[2] = { };
    int Delay_B_Ring1_Max[2] = { };

    float Modulate_B[2][8010] = { .0f };
    float Modulate_B_Last[2] = { .0f };
    int Modulate_B_Ring1[2] = { 0 };
    int Modulate_B_Ring1_Max[2] = { 0 };
    float Modulate_B_Offset[2] = { 0 };
    float Modulate_B_Offset_Min[2] = { 0 };
    float Modulate_B_Offset_Max[2] = { 0 };
    float Modulate_B_Rate = 0;
    float Modulate_B_Rate_Up = 0;
    float Modulate_B_Rate_Down = 0;
    float Modulate_Wave[2] = { 0 };
    float Modulate_Wave_Ramp[2] = { 1.0f, 1.0f };
    float Modulate_Wave_RampVal[2] = { 0 };
    float Mod_Peak[2] = { 0 };
    float Mod_Peak_Last[2] = { 0 };
    float Mod_Phase_Last_Sample[2] = { 0 };

    float Modulate2_B[2][8010] = { .0f };
    float Modulate2_B_Last[2] = { .0f };
    int Modulate2_B_Ring1[2] = { 0 };
    int Modulate2_B_Ring1_Max[2] = { 0 };
    float Modulate2_B_Offset[2] = { 0 };
    float Modulate2_B_Offset_Min[2] = { 0 };
    float Modulate2_B_Offset_Max[2] = { 0 };
    float Modulate2_B_Rate = 0;
    float Modulate2_B_Rate_Up = 0;
    float Modulate2_B_Rate_Down = 0;
    float Modulate2_Wave[2] = { 0 };
    float Modulate2_Wave_Ramp[2] = { 1.0f, 1.0f };
    float Modulate2_Wave_RampVal[2] = { 0 };
    float Mod2_Peak[2] = { 0 };
    float Mod2_Peak_Last[2] = { 0 };
    float Mod2_Phase_Last_Sample[2] = { 0 };

    float Reverb_Mod_Wave[2] = { 0 };
    float Reverb_Mod_Wave_Ramp[2] = { 0 };
    float Reverb_Mod_B_Last[2] = { .0f };

    float IRB[2][2025] = { .0f };
    int IRB_Ring1[2] = { 0 };
    //**************************************************


    //***************************************************************
    // IR CABS FROM HERE ON OUT
    //***************************************************************
    
    //R1.00 Rock LOW
    float TIR1[1024] = {
-0.000057, 0.000321, 0.000368, 0.000648, 0.0007, 0.000774, 0.000466, -0.000428, -0.004882, -0.011134,
-0.019523, -0.030568, -0.027607, -0.031208, -0.025571, -0.015658, 0.046193, 0.265396, 0.352565, 0.387969,
0.487578, 0.329189, 0.319235, 0.260449, 0.124512, 0.214379, 0.107548, 0.177804, 0.186619, 0.156143,
0.22669, 0.133537, 0.180786, 0.097383, 0.091148, 0.062257, 0.012805, 0.031608, -0.032919, 0.002574,
-0.049516, -0.027447, -0.053257, -0.060693, -0.060873, -0.094288, -0.077201, -0.118737, -0.096218, -0.128391,
-0.112308, -0.124943, -0.123076, -0.116373, -0.129138, -0.109525, -0.130132, -0.1064, -0.124814, -0.105537,
-0.114065, -0.104939, -0.100606, -0.103423, -0.088846, -0.10091, -0.081597, -0.096135, -0.078613, -0.089513,
-0.078826, -0.081948, -0.081033, -0.075431, -0.083903, -0.072321, -0.086065, -0.072858, -0.086763, -0.076471,
-0.086003, -0.082121, -0.084106, -0.087654, -0.082089, -0.091296, -0.080634, -0.092138, -0.080479, -0.090378,
-0.081331, -0.086093, -0.08217, -0.080381, -0.081639, -0.073844, -0.078832, -0.066927, -0.073622, -0.061167,
-0.066249, -0.056579, -0.057726, -0.052689, -0.049124, -0.048839, -0.040874, -0.044506, -0.034206, -0.039314,
-0.029627, -0.033541, -0.026797, -0.028035, -0.025214, -0.022914, -0.024362, -0.019144, -0.023614, -0.017093,
-0.022557, -0.016903, -0.021376, -0.018038, -0.020159, -0.020005, -0.019268, -0.022264, -0.01876, -0.023794,
-0.019355, -0.02445, -0.020489, -0.024114, -0.022026, -0.023204, -0.023385, -0.02154, -0.023946, -0.019908,
-0.023489, -0.018345, -0.021862, -0.01733, -0.019118, -0.01626, -0.015603, -0.015026, -0.01184, -0.013153,
-0.00786, -0.010112, -0.004066, -0.006108, -0.000644, -0.001274, 0.002358, 0.004184, 0.005366, 0.009168,
0.008428, 0.013722, 0.011831, 0.017651, 0.015587, 0.020294, 0.019324, 0.022272, 0.022715, 0.023181,
0.025426, 0.02368, 0.027241, 0.024016, 0.027689, 0.023972, 0.027189, 0.024288, 0.025532, 0.02421,
0.023616, 0.023983, 0.021192, 0.023218, 0.019475, 0.021812, 0.017784, 0.020184, 0.016898, 0.018191,
0.016406, 0.016175, 0.016526, 0.014757, 0.016513, 0.013902, 0.01654, 0.013668, 0.01645, 0.014101,
0.016224, 0.015063, 0.016015, 0.016476, 0.015938, 0.017843, 0.016295, 0.018933, 0.017015, 0.019862,
0.018109, 0.020332, 0.019437, 0.020643, 0.020959, 0.020863, 0.022233, 0.021186, 0.023176, 0.021501,
0.023666, 0.022198, 0.023795, 0.022846, 0.023417, 0.023197, 0.022997, 0.023632, 0.022432, 0.023444,
0.021971, 0.023067, 0.021487, 0.022319, 0.021224, 0.021376, 0.020927, 0.020299, 0.020555, 0.019371,
0.020108, 0.018867, 0.019491, 0.018341, 0.018952, 0.018199, 0.018424, 0.018189, 0.017865, 0.01851,
0.017837, 0.018778, 0.017981, 0.019187, 0.018514, 0.019592, 0.019426, 0.020029, 0.020454, 0.020543,
0.021622, 0.021309, 0.022753, 0.022134, 0.023565, 0.023241, 0.02438, 0.024262, 0.024858, 0.025179,
0.025115, 0.025851, 0.025359, 0.026124, 0.025334, 0.026359, 0.025338, 0.025978, 0.025016, 0.025086,
0.024803, 0.024182, 0.024019, 0.023008, 0.023228, 0.021814, 0.022157, 0.020525, 0.020601, 0.019503,
0.019217, 0.018303, 0.017644, 0.017431, 0.016237, 0.016266, 0.015178, 0.015443, 0.014598, 0.014868,
0.014002, 0.014088, 0.013618, 0.013574, 0.01349, 0.013028, 0.013181, 0.012651, 0.013076, 0.012383,
0.012923, 0.012324, 0.012705, 0.01221, 0.012425, 0.012316, 0.012046, 0.012219, 0.011802, 0.011969,
0.011269, 0.011795, 0.011159, 0.011506, 0.010773, 0.010949, 0.010791, 0.010497, 0.010304, 0.00988,
0.009877, 0.009346, 0.009814, 0.008816, 0.009047, 0.008433, 0.008675, 0.008248, 0.008072, 0.008001,
0.007678, 0.007716, 0.007228, 0.007394, 0.007002, 0.007074, 0.006654, 0.00691, 0.006491, 0.006586,
0.006244, 0.006252, 0.006349, 0.006041, 0.006181, 0.006102, 0.006058, 0.005718, 0.005909, 0.005806,
0.005792, 0.005427, 0.005698, 0.005478, 0.00558, 0.005219, 0.0051, 0.005296, 0.004886, 0.005084,
0.004788, 0.00465, 0.004566, 0.004394, 0.00417, 0.004126, 0.003883, 0.003814, 0.003617, 0.003429,
0.003377, 0.003008, 0.002845, 0.002834, 0.002611, 0.002422, 0.002365, 0.002162, 0.002025, 0.001859,
0.00179, 0.001619, 0.001568, 0.001431, 0.001368, 0.001229, 0.001141, 0.001149, 0.000925, 0.001013,
0.000892, 0.000848, 0.000719, 0.000807, 0.000762, 0.000571, 0.000757, 0.000499, 0.000634, 0.000431,
0.000543, 0.000431, 0.000388, 0.000333, 0.000221, 0.000295, 0.000078, 0.000186, -0.000049, 0.000023,
-0.000165, -0.000118, -0.000322, -0.000305, -0.000474, -0.000528, -0.000582, -0.000773, -0.000726, -0.001011,
-0.000905, -0.001232, -0.001113, -0.00145, -0.001356, -0.001627, -0.001633, -0.001803, -0.001926, -0.001965,
-0.002212, -0.002144, -0.00247, -0.002338, -0.002704, -0.002558, -0.002886, -0.002812, -0.003031, -0.003081,
-0.00315, -0.003342, -0.003274, -0.00358, -0.003414, -0.003769, -0.003588, -0.003917, -0.003796, -0.004022,
-0.004015, -0.004104, -0.004247, -0.004185, -0.004458, -0.004283, -0.004629, -0.004424, -0.004748, -0.004615,
-0.004813, -0.004824, -0.004874, -0.005036, -0.004927, -0.005232, -0.005009, -0.005385, -0.005124, -0.005484,
-0.005295, -0.005527, -0.005502, -0.005536, -0.005705, -0.005557, -0.005887, -0.005595, -0.006024, -0.005681,
-0.006094, -0.005815, -0.00611, -0.005995, -0.006076, -0.006195, -0.006033, -0.006369, -0.006017, -0.006492,
-0.006053, -0.006542, -0.006151, -0.006526, -0.006295, -0.006465, -0.006467, -0.006376, -0.006636, -0.006307,
-0.006761, -0.006276, -0.00682, -0.006334, -0.006787, -0.006449, -0.006701, -0.006608, -0.006579, -0.00676,
-0.006482, -0.006874, -0.006424, -0.006932, -0.006433, -0.006898, -0.006524, -0.006801, -0.006648, -0.006667,
-0.006789, -0.006541, -0.006887, -0.006463, -0.00693, -0.006449, -0.00688, -0.006511, -0.006766, -0.006619,
-0.006613, -0.006733, -0.006471, -0.006823, -0.006362, -0.006845, -0.006331, -0.006775, -0.006373, -0.006642,
-0.006474, -0.006452, -0.006592, -0.006279, -0.006662, -0.006153, -0.006671, -0.006102, -0.006588, -0.006134,
-0.006424, -0.006232, -0.00622, -0.006338, -0.006014, -0.006416, -0.005855, -0.006429, -0.005778, -0.006338,
-0.005796, -0.006167, -0.005885, -0.005932, -0.005999, -0.005695, -0.006095, -0.005502, -0.006113, -0.005401,
-0.00602, -0.005403, -0.005844, -0.005476, -0.005596, -0.00559, -0.005346, -0.00568, -0.005129, -0.005704,
-0.005002, -0.005625, -0.004979, -0.005448, -0.005032, -0.005208, -0.005132, -0.004941, -0.005221, -0.00471,
-0.005244, -0.004573, -0.005168, -0.004533, -0.004981, -0.004581, -0.00474, -0.004667, -0.004474, -0.004745,
-0.004244, -0.004758, -0.004104, -0.004674, -0.004057, -0.004491, -0.004103, -0.004233, -0.004199, -0.00396,
-0.004262, -0.00374, -0.004274, -0.003589, -0.004182, -0.003555, -0.003982, -0.003613, -0.003709, -0.003707,
-0.003427, -0.003789, -0.003194, -0.003803, -0.00304, -0.003711, -0.003005, -0.0035, -0.003067, -0.003225,
-0.003184, -0.002918, -0.003278, -0.002667, -0.003306, -0.002508, -0.003219, -0.00246, -0.003022, -0.002525,
-0.002741, -0.002638, -0.002431, -0.002759, -0.002159, -0.002793, -0.001994, -0.002719, -0.001945, -0.002531,
-0.002, -0.002249, -0.002125, -0.001943, -0.002233, -0.001678, -0.002282, -0.001505, -0.00221, -0.001464,
-0.002023, -0.001516, -0.001758, -0.00163, -0.001464, -0.001736, -0.001216, -0.001776, -0.001052, -0.001711,
-0.001011, -0.001524, -0.001076, -0.001263, -0.001185, -0.000984, -0.00129, -0.00075, -0.001316, -0.000606,
-0.00124, -0.000585, -0.001052, -0.000659, -0.000789, -0.00078, -0.000512, -0.000888, -0.000286, -0.000912,
-0.000157, -0.000834, -0.000161, -0.000623, -0.000258, -0.000366, -0.000384, -0.000086, -0.000506, 0.00013,
-0.000529, 0.000243, -0.000446, 0.000236, -0.000252, 0.000128, 0.000015, -0.000019, 0.000293, -0.000143,
0.000512, -0.000186, 0.000609, -0.0001, 0.000595, 0.000093, 0.000482, 0.000348, 0.000332, 0.000614,
0.000203, 0.000815, 0.000166, 0.000917, 0.00023, 0.000892, 0.000413, 0.000783, 0.000654, 0.000637,
0.0009, 0.000516, 0.001083, 0.000481, 0.001156, 0.000563, 0.001124, 0.000727, 0.001011, 0.000958,
0.000869, 0.00118, 0.00076, 0.001331, 0.000743, 0.001377, 0.000833, 0.001319, 0.001009, 0.001194,
0.001225, 0.001044, 0.001436, 0.000952, 0.001556, 0.000946, 0.001571, 0.001044, 0.001491, 0.001239,
0.001344, 0.001455, 0.00119, 0.001634, 0.001102, 0.001746, 0.001106, 0.001742, 0.001212, 0.001631,
0.00141, 0.001472, 0.001622, 0.001311, 0.001805, 0.001215, 0.001889, 0.001231, 0.001867, 0.001342,
0.001746, 0.001522, 0.001591, 0.001724, 0.001427, 0.001896, 0.001327, 0.00197, 0.00134, 0.001934,
0.001449, 0.001818, 0.001616, 0.00165, 0.0018, 0.001506, 0.001945, 0.001421, 0.001991, 0.001436,
0.001945, 0.001543, 0.001821, 0.001698, 0.00167, 0.001854, 0.001539, 0.001963, 0.00148, 0.001981,
0.001509, 0.001917, 0.001616, 0.001787, 0.001758, 0.001637, 0.001897, 0.001527, 0.001968, 0.001493,
0.001955, 0.001546, 0.001858, 0.001657, 0.001719, 0.001794, 0.001579, 0.001908, 0.001482, 0.00195,
0.001469, 0.001909, 0.001531, 0.001792, 0.001662, 0.001637, 0.001795, 0.001493, 0.001896, 0.00141,
0.001908, 0.00141, 0.001849, 0.001489, 0.001713, 0.001615, 0.001553, 0.001741, 0.001413, 0.00183,
0.001335, 0.001827, 0.001344, 0.001753, 0.001424, 0.001616, 0.001543, 0.001463, 0.001647, 0.001346,
0.001705, 0.001285, 0.001689, 0.001297, 0.001612, 0.001363, 0.00148, 0.001464, 0.001349, 0.001545,
0.001247, 0.001571, 0.001211, 0.00153, 0.001241, 0.001437, 0.001311, 0.00131, 0.001386, 0.001197,
0.001441, 0.001122, 0.001438, 0.001114, 0.001363, 0.001167, 0.001246, 0.001242, 0.001126, 0.00131,
0.001026, 0.001329, 0.000985, 0.001293, 0.001002, 0.00119, 0.001075, 0.001064, 0.00115, 0.000939,
0.001202, 0.000858, 0.001206, 0.000837, 0.001142, 0.000873, 0.001035, 0.000942, 0.000891, 0.00103,
0.000771, 0.001072, 0.000698, 0.001067, 0.000679, 0.000993, 0.000726, 0.000877, 0.000798, 0.000744,
0.000865, 0.000638, 0.000896, 0.000575, 0.000879, 0.000568, 0.0008, 0.000608, 0.000691, 0.000671,
0.000578, 0.000717, 0.000492, 0.000724, 0.000455, 0.000682, 0.000463, 0.000602, 0.000507, 0.000486,
0.000558, 0.000402, 0.00057, 0.000345, 0.00056, 0.000329, 0.000487, 0.00036, 0.000395, 0.000398,
0.000302, 0.000436, 0.000228, 0.000436, 0.000193, 0.000389, 0.000209, 0.000306, 0.000251, 0.000205,
0.000291, 0.000122, 0.000313, 0.000062, 0.000296, 0.00005, 0.000232, 0.000088, 0.000132, 0.000135,
0.000032, 0.000182, -0.000053, 0.000195, -0.000098, 0.000171, -0.000095, 0.000092, -0.000051, -0.000009,
0.000005, -0.000105, 0.00004, -0.000178, 0.000052, -0.000217, 0.000013, -0.000202, -0.00006, -0.000165,
-0.000148, -0.000125, -0.000224, -0.000104, -0.000278, -0.000116, -0.000289, -0.000162, -0.000272, -0.000234,
-0.000248, -0.000292, -0.000225, -0.000352, -0.000229, -0.000371, -0.000264, -0.000357, -0.000327, -0.000335,
-0.000395, -0.000301, -0.000449, 0.000
};

//R1.00 Rock HIGH
float TIR1B[1024] = {
-0.000071, 0.000378, 0.000565, 0.000978, 0.001125, 0.001189, 0.000695, -0.000721, -0.00684, -0.016495,
-0.029685, -0.046501, -0.045718, -0.046186, -0.032939, -0.009727, 0.080987, 0.384719, 0.583052, 0.678044,
0.785582, 0.540348, 0.356062, 0.108308, -0.229792, -0.280831, -0.451582, -0.379224, -0.286608, -0.207229,
-0.005115, 0.016108, 0.144185, 0.108653, 0.10671, 0.064319, -0.019348, -0.03129, -0.121518, -0.097885,
-0.144667, -0.106614, -0.102287, -0.079165, -0.044838, -0.051053, -0.01053, -0.035929, -0.002802, -0.027051,
-0.007334, -0.014829, -0.010901, 0.000199, -0.01087, 0.01132, -0.008666, 0.014934, -0.003966, 0.01313,
0.003516, 0.009465, 0.012245, 0.005701, 0.017498, 0.000825, 0.015496, -0.004219, 0.007536, -0.008302,
-0.003375, -0.010473, -0.014467, -0.011868, -0.024163, -0.015173, -0.031769, -0.020934, -0.036697, -0.028441,
-0.038828, -0.036473, -0.038403, -0.042655, -0.036388, -0.04545, -0.033852, -0.04466, -0.032436, -0.041695,
-0.032855, -0.037317, -0.034262, -0.032655, -0.035129, -0.027899, -0.034148, -0.02313, -0.031113, -0.020291,
-0.027026, -0.019894, -0.023294, -0.021557, -0.020811, -0.024186, -0.019251, -0.026387, -0.019196, -0.0274,
-0.02082, -0.027406, -0.023644, -0.027238, -0.027188, -0.026872, -0.030695, -0.027207, -0.033575, -0.028567,
-0.035374, -0.03101, -0.036331, -0.033981, -0.036448, -0.036892, -0.036089, -0.039279, -0.035368, -0.040063,
-0.035071, -0.039481, -0.034884, -0.037566, -0.034856, -0.035068, -0.034696, -0.031886, -0.033708, -0.028888,
-0.031961, -0.026325, -0.029451, -0.024857, -0.026455, -0.023829, -0.023145, -0.023014, -0.020015, -0.02188,
-0.016908, -0.01963, -0.014041, -0.01656, -0.011816, -0.013051, -0.010478, -0.009445, -0.00954, -0.006857,
-0.00909, -0.005132, -0.008498, -0.004054, -0.007408, -0.004144, -0.006298, -0.004713, -0.005295, -0.006147,
-0.004834, -0.007681, -0.004858, -0.008814, -0.005698, -0.009838, -0.006928, -0.009722, -0.008481, -0.009375,
-0.009626, -0.008477, -0.010689, -0.007752, -0.010568, -0.007199, -0.010188, -0.006727, -0.00876, -0.006392,
-0.006874, -0.006078, -0.004392, -0.005077, -0.002122, -0.003749, -0.000127, -0.002156, 0.00134, -0.000315,
0.002265, 0.00165, 0.00287, 0.00378, 0.003398, 0.005594, 0.004142, 0.006905, 0.005017, 0.007869,
0.006132, 0.008213, 0.007289, 0.008286, 0.008589, 0.008288, 0.009602, 0.008371, 0.010265, 0.008413,
0.01042, 0.008887, 0.010366, 0.00942, 0.009845, 0.009602, 0.009308, 0.010059, 0.008898, 0.010027,
0.008689, 0.009987, 0.008655, 0.009725, 0.009008, 0.009475, 0.009473, 0.009191, 0.00991, 0.009131,
0.010356, 0.009634, 0.010753, 0.010087, 0.011151, 0.01091, 0.011558, 0.011791, 0.011797, 0.012874,
0.012577, 0.013889, 0.013389, 0.014864, 0.01443, 0.015655, 0.015659, 0.01629, 0.016763, 0.016752,
0.017775, 0.017314, 0.018622, 0.017784, 0.018931, 0.018357, 0.019198, 0.01881, 0.019033, 0.019042,
0.018571, 0.01898, 0.018149, 0.018619, 0.017541, 0.018396, 0.017272, 0.017798, 0.016773, 0.016721,
0.016504, 0.015947, 0.015924, 0.015049, 0.015546, 0.014436, 0.015161, 0.013934, 0.014384, 0.0138,
0.014041, 0.013714, 0.013588, 0.014043, 0.013456, 0.014096, 0.013627, 0.014547, 0.014402, 0.015347,
0.015074, 0.015597, 0.015513, 0.015755, 0.015957, 0.01565, 0.015914, 0.015446, 0.015964, 0.015343,
0.015965, 0.015472, 0.015923, 0.015494, 0.015721, 0.01568, 0.015389, 0.015551, 0.015094, 0.015209,
0.01439, 0.014862, 0.01425, 0.014633, 0.013925, 0.014081, 0.014026, 0.013798, 0.013636, 0.013196,
0.013205, 0.012694, 0.013314, 0.012434, 0.012702, 0.01217, 0.012533, 0.012294, 0.012233, 0.012308,
0.012102, 0.012254, 0.011824, 0.01204, 0.011712, 0.011827, 0.011428, 0.011733, 0.011376, 0.011494,
0.011163, 0.011151, 0.011315, 0.011045, 0.011202, 0.011174, 0.011142, 0.010714, 0.010811, 0.010676,
0.010618, 0.010137, 0.010326, 0.010088, 0.010173, 0.009758, 0.00951, 0.009678, 0.009242, 0.009417,
0.009131, 0.008939, 0.008818, 0.008618, 0.008344, 0.008279, 0.008037, 0.007982, 0.007822, 0.007659,
0.00766, 0.007315, 0.007132, 0.007169, 0.007025, 0.006885, 0.006896, 0.006764, 0.006672, 0.006539,
0.006508, 0.006372, 0.006355, 0.006254, 0.006216, 0.006092, 0.005998, 0.00602, 0.005784, 0.00586,
0.005747, 0.005685, 0.005516, 0.005578, 0.005534, 0.005287, 0.005431, 0.005123, 0.005189, 0.004919,
0.00496, 0.004801, 0.004686, 0.004562, 0.004367, 0.004388, 0.004115, 0.004175, 0.003897, 0.003922,
0.003699, 0.003715, 0.00348, 0.00346, 0.003261, 0.003169, 0.003101, 0.002883, 0.00292, 0.002615,
0.002712, 0.002376, 0.002489, 0.002149, 0.002236, 0.001975, 0.001966, 0.001812, 0.001687, 0.00167,
0.001429, 0.001518, 0.001204, 0.001358, 0.001009, 0.00117, 0.000865, 0.000947, 0.00075, 0.000702,
0.000655, 0.000462, 0.000546, 0.000239, 0.000412, 0.000059, 0.000239, -0.000088, 0.000021, -0.0002,
-0.000208, -0.000292, -0.000456, -0.000395, -0.000689, -0.000521, -0.000883, -0.000692, -0.001026, -0.000918,
-0.001119, -0.001157, -0.001211, -0.001403, -0.001299, -0.001631, -0.00142, -0.001817, -0.001575, -0.001946,
-0.001784, -0.002022, -0.002031, -0.002068, -0.00227, -0.002128, -0.002491, -0.002211, -0.002666, -0.002345,
-0.002775, -0.002527, -0.002833, -0.002758, -0.002846, -0.003008, -0.002856, -0.003233, -0.002897, -0.003406,
-0.002994, -0.003507, -0.003154, -0.003547, -0.003361, -0.003546, -0.0036, -0.003522, -0.003834, -0.003524,
-0.004024, -0.003565, -0.004143, -0.0037, -0.004176, -0.003887, -0.004156, -0.00412, -0.004105, -0.00434,
-0.00408, -0.004521, -0.004096, -0.004645, -0.004185, -0.004675, -0.004351, -0.004647, -0.004548, -0.00458,
-0.004758, -0.004527, -0.004921, -0.004522, -0.00503, -0.004586, -0.005043, -0.004723, -0.004995, -0.004902,
-0.004911, -0.005085, -0.004841, -0.005243, -0.004807, -0.00533, -0.004853, -0.005323, -0.004968, -0.005254,
-0.005144, -0.005132, -0.00533, -0.005033, -0.005466, -0.00498, -0.005539, -0.005006, -0.005518, -0.005113,
-0.005417, -0.005283, -0.005283, -0.005458, -0.005145, -0.005599, -0.005056, -0.005672, -0.005053, -0.005638,
-0.005143, -0.005528, -0.005304, -0.005356, -0.005482, -0.005183, -0.005637, -0.005058, -0.00571, -0.005026,
-0.005667, -0.005094, -0.005547, -0.005233, -0.005355, -0.005406, -0.005165, -0.005553, -0.005011, -0.005626,
-0.004948, -0.005594, -0.004987, -0.005466, -0.0051, -0.005277, -0.005256, -0.005064, -0.005395, -0.004889,
-0.005463, -0.004812, -0.005432, -0.004833, -0.005287, -0.004934, -0.005092, -0.00507, -0.004874, -0.005192,
-0.004696, -0.005245, -0.004609, -0.0052, -0.004616, -0.005055, -0.004712, -0.004837, -0.004853, -0.004607,
-0.004951, -0.004431, -0.005, -0.004329, -0.004939, -0.004342, -0.004769, -0.004444, -0.00453, -0.004574,
-0.004283, -0.004688, -0.004092, -0.004733, -0.003982, -0.004665, -0.003988, -0.004477, -0.004084, -0.004229,
-0.004237, -0.003954, -0.004355, -0.003736, -0.004405, -0.003615, -0.004337, -0.003601, -0.004156, -0.003699,
-0.003897, -0.003839, -0.003611, -0.003983, -0.00337, -0.004031, -0.003235, -0.003969, -0.003215, -0.003794,
-0.003297, -0.003525, -0.003443, -0.003239, -0.003567, -0.002995, -0.003626, -0.002849, -0.003561, -0.002834,
-0.003384, -0.002907, -0.003127, -0.003037, -0.002847, -0.003152, -0.002615, -0.003198, -0.002471, -0.003135,
-0.00245, -0.002949, -0.00253, -0.002693, -0.002649, -0.002422, -0.002758, -0.002201, -0.002784, -0.00207,
-0.002703, -0.002064, -0.002514, -0.002149, -0.002252, -0.002275, -0.001979, -0.002383, -0.001762, -0.002401,
-0.001641, -0.002314, -0.001659, -0.002097, -0.001758, -0.001838, -0.001886, -0.001556, -0.002002, -0.001345,
-0.002014, -0.001237, -0.001919, -0.00125, -0.001714, -0.001362, -0.001442, -0.001506, -0.001158, -0.001617,
-0.000937, -0.001645, -0.000846, -0.001546, -0.000862, -0.001337, -0.000971, -0.001071, -0.001115, -0.000798,
-0.00123, -0.000595, -0.00125, -0.00049, -0.001166, -0.000516, -0.000967, -0.000618, -0.000711, -0.000752,
-0.000454, -0.000857, -0.000265, -0.000871, -0.00019, -0.000768, -0.000216, -0.000586, -0.000323, -0.00034,
-0.00045, -0.000105, -0.000539, 0.000053, -0.000533, 0.000106, -0.000421, 0.000055, -0.000224, -0.000056,
0.00001, -0.000192, 0.000234, -0.000258, 0.000366, -0.000239, 0.000388, -0.00012, 0.000316, 0.000101,
0.000188, 0.000336, 0.000053, 0.000526, -0.000014, 0.00065, 0.000021, 0.000658, 0.000154, 0.000558,
0.000373, 0.000417, 0.000605, 0.000276, 0.000802, 0.000206, 0.000896, 0.00025, 0.000888, 0.00039,
0.00078, 0.000591, 0.000643, 0.000816, 0.000502, 0.001004, 0.00043, 0.001089, 0.000471, 0.001065,
0.000608, 0.000967, 0.000799, 0.000816, 0.001001, 0.000695, 0.001163, 0.00064, 0.001222, 0.000681,
0.00119, 0.000817, 0.001085, 0.000996, 0.000954, 0.001171, 0.000848, 0.001296, 0.000816, 0.001327,
0.000871, 0.001278, 0.001006, 0.001166, 0.001169, 0.001035, 0.001326, 0.000952, 0.001413, 0.000946,
0.001414, 0.001026, 0.001333, 0.00116, 0.00121, 0.001318, 0.001093, 0.00145, 0.001022, 0.001505,
0.001035, 0.001478, 0.001124, 0.001377, 0.001279, 0.001243, 0.001433, 0.001121, 0.001548, 0.001063,
0.001571, 0.001086, 0.001525, 0.001192, 0.001406, 0.001339, 0.001265, 0.001484, 0.001147, 0.001588,
0.001096, 0.001596, 0.001128, 0.001535, 0.001232, 0.001415, 0.001372, 0.001282, 0.001491, 0.001185,
0.001563, 0.001147, 0.001557, 0.00118, 0.001493, 0.001267, 0.001374, 0.001385, 0.001264, 0.001484,
0.001184, 0.001521, 0.001169, 0.001491, 0.001219, 0.001411, 0.001309, 0.001299, 0.001397, 0.001202,
0.001465, 0.001148, 0.001472, 0.001161, 0.001407, 0.001231, 0.001303, 0.001322, 0.001199, 0.001405,
0.001118, 0.001433, 0.001094, 0.001405, 0.001129, 0.001309, 0.001218, 0.001199, 0.001306, 0.001088,
0.001367, 0.001023, 0.00138, 0.001021, 0.001325, 0.001073, 0.001228, 0.001156, 0.001091, 0.001252,
9.869999E-04, 0.001303, 0.00093, 0.001306, 0.000928, 0.001237, 0.000988, 0.00113, 0.001072, 0.001008,
0.001147, 0.000913, 0.001184, 0.000864, 0.001173, 0.000873, 0.0011, 0.000923, 0.000997, 0.000996,
0.000894, 0.001049, 0.00082, 0.001058, 0.000793, 0.001021, 0.000812, 0.000946, 0.000867, 0.000835,
0.000922, 0.000762, 0.000939, 0.000714, 0.000934, 0.000711, 0.000862, 0.000747, 0.000774, 0.000791,
0.000687, 0.000834, 0.000624, 0.000837, 0.000597, 0.00079, 0.00062, 0.00071, 0.000669, 0.000613,
0.00071, 0.000536, 0.000735, 0.000482, 0.000718, 0.000478, 0.000654, 0.000523, 0.000558, 0.000573,
0.000459, 0.000622, 0.00038, 0.000634, 0.000341, 0.00061, 0.000351, 0.000529, 0.000397, 0.000428,
0.000456, 0.000336, 0.00049, 0.000266, 0.000501, 0.000232, 0.000457, 0.00025, 0.000384, 0.00029,
0.000298, 0.00033, 0.000224, 0.000349, 0.000172, 0.000333, 0.000162, 0.000285, 0.000182, 0.000212,
0.000203, 0.000153, 0.000228, 0.000096, 0.000219, 0.000077, 0.00018, 0.000092, 0.000114, 0.000112,
0.000043, 0.000144, -0.000009, 0.000
};

    //R1.00 Mako 1 Low
    float TIR2[1024] = {
0.030729, 0.099211, 0.170272, 0.212165, 0.215975, 0.183765, 0.131405, 0.082296, 0.049045, 0.032152,
0.037136, 0.065387, 0.094813, 0.102911, 0.087885, 0.060806, 0.036672, 0.022918, 0.018985, 0.025215,
0.038884, 0.051507, 0.05456, 0.04565, 0.030983, 0.020027, 0.018293, 0.024055, 0.03076, 0.034832,
0.037905, 0.036848, 0.025001, 0.009222, 0.001669, 0.00086, 0.00105, -0.00015, -0.004685, -0.009353,
-0.008602, -0.002989, 0.001804, 0.004733, 0.011103, 0.019007, 0.020024, 0.013295, 0.003649, -0.00455,
-0.008103, -0.007612, -0.005479, -0.002514, -0.000365, -0.000896, -0.002719, -0.003779, -0.003732, -0.002679,
-0.001619, -0.002023, -0.003917, -0.006256, -0.009274, -0.01295, -0.014625, -0.015003, -0.016494, -0.016098,
-0.013708, -0.012574, -0.011496, -0.010512, -0.01076, -0.011188, -0.012483, -0.015783, -0.021209, -0.027279,
-0.031577, -0.033926, -0.034216, -0.032272, -0.029257, -0.02624, -0.024377, -0.022991, -0.02094, -0.019015,
-0.01692, -0.014521, -0.012797, -0.012063, -0.013257, -0.01668, -0.021312, -0.0267, -0.032482, -0.037471,
-0.040456, -0.04178, -0.04176, -0.039564, -0.035755, -0.032164, -0.030097, -0.029108, -0.028019, -0.027261,
-0.02633, -0.023974, -0.020886, -0.017428, -0.014206, -0.0126, -0.012742, -0.014396, -0.017108, -0.020626,
-0.02418, -0.025613, -0.024434, -0.02092, -0.015491, -0.010635, -0.00783, -0.007122, -0.008571, -0.011482,
-0.015201, -0.018312, -0.019543, -0.019509, -0.019046, -0.019385, -0.021421, -0.024371, -0.026927, -0.02759,
-0.026074, -0.023249, -0.019638, -0.015994, -0.012836, -0.010907, -0.011025, -0.012183, -0.013066, -0.013042,
-0.011975, -0.010399, -0.008609, -0.007329, -0.007151, -0.007749, -0.009242, -0.011183, -0.01241, -0.012553,
-0.01203, -0.011787, -0.01233, -0.013839, -0.016385, -0.019056, -0.020848, -0.021195, -0.020284, -0.018923,
-0.017239, -0.015785, -0.015287, -0.015229, -0.01539, -0.01574, -0.015809, -0.015544, -0.01509, -0.014664,
-0.014495, -0.014869, -0.016169, -0.018141, -0.019941, -0.020744, -0.020495, -0.019849, -0.018803, -0.016943,
-0.014593, -0.012609, -0.011635, -0.011609, -0.011913, -0.012055, -0.012035, -0.011712, -0.010552, -0.008449,
-0.005876, -0.003649, -0.002297, -0.001551, -0.001103, -0.000433, 0.000897, 0.00234, 0.003636, 0.004738,
0.005088, 0.004892, 0.004593, 0.0038, 0.002424, 0.001198, 0.000431, -0.00042, -0.001503, -0.002429,
-0.003244, -0.004233, -0.005167, -0.005388, -0.004753, -0.003708, -0.002574, -0.001581, -0.000941, -0.000667,
-0.000373, 0.000239, 0.000899, 0.001584, 0.002341, 0.003017, 0.003601, 0.003536, 0.002711, 0.00172,
0.000601, -0.000046, 0.000338, 0.001093, 0.00168, 0.001647, 0.000754, -0.000666, -0.002348, -0.003623,
-0.004004, -0.003798, -0.003349, -0.003089, -0.003414, -0.004388, -0.00572, -0.006915, -0.007735, -0.00784,
-0.007091, -0.006104, -0.00526, -0.004608, -0.004387, -0.004604, -0.004896, -0.004905, -0.004524, -0.003798,
-0.002975, -0.002318, -0.00167, -0.000958, -0.00031, 0.00039, 0.001253, 0.002351, 0.00353, 0.004415,
0.004973, 0.005247, 0.005386, 0.005675, 0.005952, 0.006224, 0.006719, 0.007257, 0.007787, 0.008209,
0.008366, 0.008489, 0.008455, 0.008194, 0.008092, 0.007872, 0.007131, 0.006126, 0.005068, 0.003879,
0.002496, 0.001124, -0.000153, -0.001604, -0.003037, -0.004051, -0.004706, -0.005117, -0.005399, -0.005617,
-0.005724, -0.00575, -0.005502, -0.004912, -0.004273, -0.003628, -0.002978, -0.002486, -0.00216, -0.001906,
-0.001541, -0.001029, -0.000615, -0.000317, 0.000056, 0.000473, 0.000895, 0.001474, 0.0022, 0.002857,
0.003302, 0.003554, 0.003708, 0.00369, 0.003384, 0.003072, 0.003171, 0.003803, 0.004665, 0.005201,
0.005158, 0.004551, 0.003459, 0.002224, 0.001363, 0.00115, 0.001244, 0.001116, 0.000703, 0.000064,
-0.000697, -0.00136, -0.001839, -0.00202, -0.001902, -0.001682, -0.001428, -0.001223, -0.001008, -0.000508,
0.000277, 0.001193, 0.002185, 0.00317, 0.004041, 0.004784, 0.005489, 0.006072, 0.00649, 0.006986,
0.007464, 0.007558, 0.007307, 0.006948, 0.006684, 0.006773, 0.007265, 0.007858, 0.008254, 0.008331,
0.007806, 0.006454, 0.004741, 0.003415, 0.002717, 0.002501, 0.00252, 0.002443, 0.002013, 0.001198,
0.000097, -0.001189, -0.002625, -0.004108, -0.005298, -0.005807, -0.005577, -0.005054, -0.004782, -0.004742,
-0.004626, -0.004464, -0.004413, -0.0045, -0.004718, -0.004981, -0.005043, -0.004614, -0.003659, -0.002358,
-0.000954, 0.000163, 0.000757, 0.000883, 0.000632, 0.00025, 0.000012, 0.000091, 0.000834, 0.002115,
0.003084, 0.003234, 0.002715, 0.001963, 0.001394, 0.001169, 0.001385, 0.001827, 0.002096, 0.002184,
0.001914, 0.001158, 0.000532, 0.000509, 0.000991, 0.001598, 0.001986, 0.002249, 0.002333, 0.002066,
0.001674, 0.00121, 0.000708, 0.000414, 0.00018, -0.000205, -0.000599, -0.000795, -0.000636, -0.000002,
0.000923, 0.001558, 0.00154, 0.000968, 0.00012, -0.000531, -0.000618, 0.000009, 0.001241, 0.002429,
0.003056, 0.00305, 0.002376, 0.001275, 0.000095, -0.00092, -0.001385, -0.001117, -0.000411, 0.000122,
0.000151, 0.000083, 0.000327, 0.000861, 0.001549, 0.002105, 0.002326, 0.002179, 0.00166, 0.001074,
0.000747, 0.000625, 0.000683, 0.000949, 0.0012, 0.001159, 0.000854, 0.000676, 0.000862, 0.001181,
0.001396, 0.001427, 0.001115, 0.0004, -0.00048, -0.001225, -0.00175, -0.001948, -0.001612, -0.001032,
-0.000773, -0.000924, -0.001361, -0.001954, -0.002357, -0.002396, -0.002242, -0.00192, -0.001341, -0.000762,
-0.000526, -0.000531, -0.000461, -0.000193, 0.000307, 0.000957, 0.001442, 0.001637, 0.001734, 0.001814,
0.001889, 0.001998, 0.00205, 0.002016, 0.002012, 0.001989, 0.001787, 0.001667, 0.001934, 0.002139,
0.001929, 0.00152, 0.000996, 0.000589, 0.00063, 0.001062, 0.001679, 0.002242, 0.002705, 0.003067,
0.003158, 0.003034, 0.002683, 0.002151, 0.001813, 0.001666, 0.001681, 0.001921, 0.002048, 0.001909,
0.001564, 0.001102, 0.000683, 0.00028, -0.000132, -0.00089, -0.002282, -0.003807, -0.005375, -0.00693,
-0.007113, -0.005, -0.001277, 0.002784, 0.006033, 0.007688, 0.007557, 0.006317, 0.004821, 0.003205,
0.001728, 0.001058, 0.001333, 0.002065, 0.002723, 0.003157, 0.003344, 0.003261, 0.003166, 0.003216,
0.003454, 0.004119, 0.00517, 0.006225, 0.007197, 0.008205, 0.008904, 0.008931, 0.008528, 0.007768,
0.006811, 0.006175, 0.005502, 0.004403, 0.003195, 0.001399, -0.001851, -0.005992, -0.00886, -0.008803,
-0.006249, -0.002086, 0.002719, 0.006461, 0.008454, 0.008855, 0.007387, 0.004477, 0.001608, 0.000276,
0.000827, 0.002345, 0.003797, 0.004133, 0.002909, 0.001119, -0.000383, -0.001347, -0.001243, 0.000027,
0.001925, 0.003712, 0.00468, 0.00488, 0.004794, 0.004451, 0.003869, 0.003475, 0.00371, 0.00449,
0.005129, 0.00517, 0.004727, 0.003883, 0.002885, 0.002282, 0.002134, 0.00199, 0.001537, 0.000807,
-0.000129, -0.001312, -0.002303, -0.002468, -0.001916, -0.001134, -0.000512, -0.000296, -0.000386, -0.000518,
-0.000343, 0.000343, 0.001217, 0.00205, 0.002822, 0.003154, 0.002874, 0.002418, 0.002063, 0.001773,
0.00174, 0.002126, 0.002608, 0.002686, 0.002216, 0.001475, 0.000692, 0.000019, -0.000327, -0.000516,
-0.00063, -0.000335, 0.000325, 0.0012, 0.002302, 0.003486, 0.004577, 0.005224, 0.005283, 0.004902,
0.004011, 0.003003, 0.002302, 0.001682, 0.001271, 0.001328, 0.001591, 0.001608, 0.001154, 0.000654,
0.000595, 0.001071, 0.001979, 0.00284, 0.003368, 0.003663, 0.003771, 0.003863, 0.004032, 0.004316,
0.004708, 0.004917, 0.004962, 0.005002, 0.00487, 0.004442, 0.003614, 0.002592, 0.001684, 0.000355,
-0.001729, -0.003484, -0.003524, -0.001852, 0.000171, 0.001425, 0.001745, 0.001452, 0.000991, 0.000469,
-0.000243, -0.000912, -0.001354, -0.00172, -0.002024, -0.002228, -0.002275, -0.002019, -0.001611, -0.00112,
-0.000328, 0.0004, 0.000532, 0.000344, 0.000248, 0.000068, -0.000322, -0.000613, -0.000651, -0.000441,
0.000004, 0.000354, 0.000604, 0.001042, 0.001196, 0.000591, -0.000176, -0.000276, 0.000132, 0.000304,
0.00019, 0.000169, 0.000442, 0.000989, 0.001456, 0.002046, 0.003152, 0.004133, 0.004535, 0.004656,
0.004834, 0.005308, 0.005693, 0.005554, 0.004967, 0.003873, 0.002445, 0.001141, 0.000394, 0.000721,
0.002066, 0.003677, 0.004761, 0.005087, 0.005009, 0.004594, 0.003938, 0.003557, 0.003639, 0.00422,
0.005142, 0.005822, 0.005926, 0.005373, 0.004299, 0.003209, 0.002475, 0.002076, 0.001884, 0.002005,
0.00233, 0.002333, 0.002054, 0.001807, 0.00153, 0.001467, 0.001888, 0.002534, 0.003009, 0.002923,
0.002228, 0.001414, 0.00083, 0.000498, 0.000495, 0.000759, 0.001081, 0.001359, 0.001495, 0.001522,
0.001494, 0.001276, 0.001102, 0.001319, 0.001861, 0.002619, 0.003335, 0.00369, 0.003817, 0.004085,
0.004633, 0.005157, 0.005457, 0.005608, 0.005337, 0.004583, 0.003858, 0.003391, 0.003176, 0.003255,
0.003484, 0.003611, 0.00354, 0.003368, 0.003181, 0.002971, 0.002783, 0.002693, 0.002688, 0.002718,
0.002771, 0.002857, 0.003063, 0.003404, 0.003637, 0.003609, 0.003409, 0.003077, 0.002664, 0.002311,
0.002125, 0.002166, 0.002401, 0.002533, 0.002223, 0.001623, 0.001107, 0.000826, 0.000841, 0.001108,
0.001531, 0.00197, 0.00218, 0.002109, 0.00172, 0.000937, 0.000095, -0.000441, -0.000633, -0.000594,
-0.000455, -0.000161, 0.000287, 0.000596, 0.00048, -0.000057, -0.000657, -9.869999E-04, -0.001054, -0.000966,
-0.000819, -0.000717, -0.000687, -0.000727, -0.000828, -0.000994, -0.001261, -0.001542, -0.001648, -0.001555,
-0.001382, -0.001179, -0.000963, -0.000658, -0.0003, -0.000078, 0.000083, 0.000305, 0.000395, 0.000256,
0.00005, -0.000057, -0.000054, 0.000053, 0.000308, 0.000572, 0.000696, 0.000673, 0.00046, 0.000134,
-0.000153, -0.00035, -0.000409, -0.00033, -0.000173, -0.000035, 0.000032, 0.000056, -0.000024, -0.000271,
-0.000628, -0.000998, -0.001277, -0.001448, -0.001535, -0.001536, -0.001505, -0.001511, -0.001591, -0.001725,
-0.001851, -0.001962, -0.00212, -0.002408, -0.002806, -0.0032, -0.003464, -0.003456, -0.003252, -0.003089,
-0.00302, -0.003036, -0.003207, -0.003532, -0.003849, -0.003998, -0.003976, -0.003885, -0.00388, -0.004084,
-0.004482, -0.004951, -0.005295, -0.005391, -0.00521, -0.004758, -0.004166, -0.003551, -0.003008, -0.002722,
-0.002684, -0.002727, -0.002755, -0.002645, -0.002386, -0.002106, -0.001856, -0.001728, -0.001744, -0.0018,
-0.001904, -0.001989, -0.001977, -0.001975, -0.001938, -0.001841, -0.001833, -0.001904, -0.002003, -0.002059,
-0.002027, -0.00207, -0.002171, -0.002172, -0.002088, -0.001955, -0.001842, -0.001828, -0.001929, -0.002126,
-0.002308, -0.0024, -0.002432, -0.002425, -0.002371, -0.002274, -0.002218, -0.002251, -0.002361, -0.00257,
-0.002783, -0.002849, -0.002767, -0.002594, -0.0024, -0.002265, -0.002229, -0.002303, -0.002468, -0.002657,
-0.002811, -0.002849, -0.002743, -0.002611, -0.002534, -0.002491, -0.002455, -0.002395, -0.002328, -0.002255,
-0.002123, -0.001986, -0.001931, 0.00
};

//R1.00 Mako 1 High
float TIR2B[1024] = {
0.038119, 0.134002, 0.250032, 0.332253, 0.346052, 0.278452, 0.147091, -0.005357, -0.138208, -0.226789,
-0.2501, -0.200345, -0.108242, -0.020542, 0.034404, 0.052056, 0.046277, 0.033029, 0.022061, 0.022012,
0.035191, 0.052833, 0.06104, 0.050931, 0.026254, 0.000255, -0.014172, -0.012856, -0.001507, 0.012913,
0.028164, 0.037575, 0.029772, 0.009434, -0.006624, -0.013254, -0.013682, -0.011305, -0.010475, -0.009652,
-0.002365, 0.012386, 0.027856, 0.040139, 0.053583, 0.066569, 0.068473, 0.055246, 0.031767, 0.006117,
-0.013176, -0.022345, -0.022252, -0.014542, -0.003156, 0.006765, 0.013871, 0.019259, 0.02345, 0.026781,
0.028496, 0.026903, 0.021746, 0.014545, 0.005933, -0.00328, -0.009113, -0.010999, -0.011586, -0.008263,
-0.000791, 0.006465, 0.013154, 0.018408, 0.020335, 0.019713, 0.016104, 0.008467, -0.003148, -0.016367,
-0.026872, -0.032529, -0.032155, -0.025372, -0.014175, -0.001432, 0.009478, 0.01749, 0.023201, 0.025861,
0.02606, 0.024619, 0.021144, 0.01567, 0.007444, -0.003749, -0.016367, -0.029005, -0.04036, -0.048298,
-0.050649, -0.047386, -0.039389, -0.026735, -0.011138, 0.003737, 0.01438, 0.019715, 0.020792, 0.017889,
0.012598, 0.007807, 0.004313, 0.002509, 0.001925, 0.000605, -0.002445, -0.007506, -0.014152, -0.021798,
-0.029051, -0.032565, -0.030578, -0.022897, -0.010407, 0.00276, 0.012631, 0.0171, 0.015068, 0.00734,
-0.004282, -0.016452, -0.025548, -0.030431, -0.031425, -0.030403, -0.029644, -0.029583, -0.029284, -0.026826,
-0.021286, -0.013469, -0.004463, 0.004074, 0.010625, 0.013575, 0.01143, 0.005259, -0.002473, -0.009468,
-0.014112, -0.01612, -0.015694, -0.014067, -0.012766, -0.012311, -0.013346, -0.015608, -0.017521, -0.017981,
-0.016958, -0.015554, -0.014856, -0.015635, -0.018348, -0.021974, -0.024805, -0.025357, -0.023208, -0.019239,
-0.014121, -0.009151, -0.005971, -0.004681, -0.005113, -0.006989, -0.009331, -0.01146, -0.013059, -0.014131,
-0.014932, -0.015947, -0.017893, -0.020747, -0.023479, -0.024687, -0.023766, -0.021312, -0.01765, -0.01267,
-0.007043, -0.002256, 0.000237, -0.00005, -0.002437, -0.005822, -0.009432, -0.012444, -0.013676, -0.012534,
-0.009462, -0.005825, -0.003042, -0.00156, -0.001344, -0.001717, -0.001772, -0.001695, -0.001693, -0.001851,
-0.002888, -0.004755, -0.006882, -0.009492, -0.01263, -0.015332, -0.016874, -0.01764, -0.017955, -0.017588,
-0.016694, -0.01579, -0.014871, -0.013229, -0.010524, -0.007213, -0.003895, -0.001166, 0.000427, 0.000678,
0.000068, -0.000728, -0.001622, -0.002419, -0.00291, -0.00318, -0.003266, -0.003915, -0.005528, -0.007536,
-0.009695, -0.011073, -0.010622, -0.008811, -0.00651, -0.004774, -0.004458, -0.005559, -0.007744, -0.009923,
-0.010943, -0.010633, -0.009278, -0.0076, -0.006495, -0.006504, -0.007547, -0.00897, -0.010152, -0.010315,
-0.008944, -0.006636, -0.004139, -0.001948, -0.000697, -0.000673, -0.001504, -0.002521, -0.00321, -0.003293,
-0.002901, -0.002401, -0.001778, -0.001027, -0.000355, 0.000291, 0.001037, 0.00201, 0.003067, 0.003698,
0.003691, 0.003022, 0.00191, 0.000859, -0.000074, -0.000767, -0.000881, -0.000547, 0.000096, 0.000803,
0.001217, 0.001455, 0.001358, 0.000817, 0.00031, -0.000322, -0.001547, -0.003162, -0.004832, -0.00649,
-0.008128, -0.009465, -0.010318, -0.010979, -0.011339, -0.010954, -0.009827, -0.00816, -0.006257, -0.00441,
-0.002751, -0.001415, -0.00021, 0.001033, 0.00206, 0.002793, 0.003233, 0.003212, 0.002726, 0.001946,
0.00122, 0.000778, 0.000443, 0.00016, 0.000119, 0.000305, 0.000645, 0.001268, 0.00216, 0.003013,
0.003523, 0.003576, 0.003249, 0.002503, 0.001256, -0.000093, -0.000844, -0.00056, 0.000556, 0.001704,
0.002197, 0.001698, 0.000141, -0.002025, -0.003899, -0.004708, -0.004515, -0.003968, -0.003457, -0.003189,
-0.003214, -0.003311, -0.003303, -0.002949, -0.002152, -0.001126, -0.000047, 0.000863, 0.001567, 0.002376,
0.003391, 0.004491, 0.005579, 0.006513, 0.007105, 0.007286, 0.007167, 0.006726, 0.005973, 0.005274,
0.004672, 0.003785, 0.002561, 0.001269, 0.000252, -0.000027, 0.000645, 0.001929, 0.003265, 0.004189,
0.004086, 0.002446, -0.000278, -0.002904, -0.004565, -0.004982, -0.004305, -0.003079, -0.001965, -0.001401,
-0.001539, -0.002345, -0.003715, -0.005392, -0.006757, -0.007046, -0.005887, -0.003731, -0.001505, 0.000331,
0.001823, 0.002862, 0.003247, 0.002946, 0.002043, 0.000778, -0.000346, -0.000709, -0.000002, 0.00166,
0.003859, 0.005839, 0.006915, 0.006832, 0.005605, 0.003645, 0.001584, 0.000025, -0.000266, 0.000848,
0.002322, 0.003065, 0.002712, 0.001559, 0.000231, -0.000754, -0.000971, -0.000486, 0.000183, 0.000784,
0.000902, 0.000185, -0.000723, -0.001008, -0.00044, 0.000653, 0.001711, 0.002573, 0.003022, 0.002737,
0.001925, 0.000759, -0.00056, -0.001568, -0.002234, -0.002767, -0.003072, -0.002921, -0.002097, -0.000433,
0.001825, 0.003787, 0.004616, 0.004077, 0.002439, 0.000492, -0.000865, -0.000975, 0.000339, 0.002302,
0.003897, 0.004515, 0.003761, 0.001799, -0.000792, -0.003345, -0.004969, -0.005029, -0.003652, -0.001723,
-0.000129, 0.001122, 0.002362, 0.003639, 0.004841, 0.005614, 0.005618, 0.004735, 0.002988, 0.000871,
-0.000926, -0.002121, -0.00256, -0.00217, -0.001289, -0.000479, -0.000016, 0.000385, 0.00106, 0.001852,
0.00245, 0.002646, 0.002169, 0.000884, -0.000904, -0.002638, -0.003924, -0.004412, -0.00371, -0.002112,
-0.000511, 0.00054, 0.000862, 0.000487, -0.000102, -0.000446, -0.000497, -0.000191, 0.000606, 0.001579,
0.002162, 0.002238, 0.002115, 0.002056, 0.002255, 0.002718, 0.003078, 0.003049, 0.002749, 0.002301,
0.001803, 0.001375, 0.000977, 0.000579, 0.000313, 0.000147, -0.000131, -0.000276, 0.000133, 0.00067,
0.000761, 0.000441, -0.00022, -0.000884, -0.000981, -0.000354, 0.000818, 0.002135, 0.003321, 0.004191,
0.004426, 0.004003, 0.002936, 0.00138, -0.000046, -0.001051, -0.001486, -0.00121, -0.000648, -0.0002,
-0.000027, -0.000152, -0.000404, -0.00074, -0.001137, -0.001967, -0.003673, -0.005747, -0.007892, -0.009835,
-0.00973, -0.00587, 0.001382, 0.010126, 0.017883, 0.022397, 0.022388, 0.018313, 0.01174, 0.003981,
-0.003515, -0.008927, -0.011224, -0.010639, -0.008134, -0.004656, -0.001051, 0.001965, 0.004222, 0.005755,
0.006669, 0.007417, 0.008231, 0.008852, 0.009183, 0.009378, 0.009087, 0.007809, 0.005731, 0.003059,
0.000179, -0.002018, -0.003613, -0.005049, -0.006128, -0.007506, -0.010514, -0.014925, -0.018009, -0.016766,
-0.010635, -0.00059, 0.011388, 0.021868, 0.028295, 0.029637, 0.025272, 0.016055, 0.004878, -0.004592,
-0.009935, -0.011007, -0.009044, -0.006221, -0.004454, -0.003737, -0.003397, -0.002956, -0.001487, 0.001511,
0.005568, 0.009555, 0.012075, 0.012601, 0.011516, 0.009124, 0.005833, 0.002602, 0.000522, 0.000005,
0.000383, 0.000781, 0.000845, 0.000361, -0.00052, -0.001077, -0.000965, -0.000568, -0.000386, -0.000603,
-0.001257, -0.002409, -0.003491, -0.003486, -0.002156, -0.000003, 0.002239, 0.003854, 0.004591, 0.004627,
0.004501, 0.004743, 0.005251, 0.005795, 0.006276, 0.006159, 0.00506, 0.003404, 0.001719, 0.000246,
-0.000591, -0.000423, 0.00044, 0.001208, 0.001326, 0.000828, -0.000044, -0.000974, -0.001508, -0.001642,
-0.001434, -0.000518, 0.001094, 0.003153, 0.005504, 0.007805, 0.009654, 0.010438, 0.0098, 0.007869,
0.004746, 0.001169, -0.001894, -0.004222, -0.005464, -0.00522, -0.003808, -0.002058, -0.000734, 0.000233,
0.001335, 0.002831, 0.004727, 0.006453, 0.007464, 0.007677, 0.007128, 0.006131, 0.005022, 0.004081,
0.003497, 0.003006, 0.002559, 0.002283, 0.001971, 0.001389, 0.000302, -0.00111, -0.0024, -0.004037,
-0.006512, -0.008619, -0.008269, -0.004762, 0.000427, 0.005218, 0.008339, 0.009441, 0.008879, 0.00706,
0.004261, 0.001177, -0.001487, -0.003496, -0.004698, -0.005017, -0.004463, -0.002999, -0.000968, 0.001279,
0.003745, 0.005852, 0.006713, 0.006377, 0.005399, 0.003886, 0.001922, 0.000073, -0.001186, -0.001635,
-0.001209, -0.000363, 0.000634, 0.001911, 0.002804, 0.002476, 0.001379, 0.000688, 0.000696, 0.000725,
0.00054, 0.000464, 0.000798, 0.001614, 0.002488, 0.003502, 0.005096, 0.006565, 0.007127, 0.006857,
0.006178, 0.005612, 0.004972, 0.003775, 0.001998, -0.000425, -0.003241, -0.005735, -0.007064, -0.006273,
-0.003172, 0.001227, 0.005352, 0.008084, 0.009241, 0.008778, 0.006946, 0.004712, 0.002882, 0.002012,
0.002225, 0.002842, 0.00314, 0.002631, 0.00121, -0.000546, -0.001931, -0.002629, -0.002616, -0.001761,
-0.000248, 0.001099, 0.00195, 0.002465, 0.002584, 0.002641, 0.003136, 0.003968, 0.004661, 0.004568,
0.00339, 0.00164, -0.000019, -0.001212, -0.001611, -0.001157, -0.000139, 0.00112, 0.002259, 0.003093,
0.003559, 0.003459, 0.003064, 0.002943, 0.00327, 0.004037, 0.004922, 0.005389, 0.005375, 0.005278,
0.005413, 0.005567, 0.005471, 0.005135, 0.004209, 0.002544, 0.000761, -0.000602, -0.00129, -0.001116,
-0.000244, 0.000878, 0.00186, 0.002572, 0.003001, 0.003119, 0.003006, 0.002831, 0.002683, 0.002583,
0.002546, 0.002601, 0.002859, 0.003363, 0.003807, 0.003888, 0.003588, 0.002942, 0.00206, 0.001207,
0.000654, 0.000602, 0.001085, 0.001711, 0.001879, 0.001511, 0.000991, 0.000646, 0.000721, 0.001282,
0.002234, 0.00333, 0.004118, 0.004335, 0.003811, 0.002424, 0.00063, -0.000877, -0.001706, -0.001785,
-0.001245, -0.000144, 0.001379, 0.002802, 0.003505, 0.00323, 0.002322, 0.00137, 0.000684, 0.000356,
0.000352, 0.000525, 0.000751, 0.000935, 0.001016, 0.000945, 0.00066, 0.00026, 0.000031, 0.000115,
0.000438, 0.000908, 0.001437, 0.002049, 0.002678, 0.003051, 0.003169, 0.003178, 0.002912, 0.002245,
0.001373, 0.000598, 0.000083, -0.000087, 0.000171, 0.000681, 0.001149, 0.001415, 0.001316, 0.000887,
0.00034, -0.000152, -0.000417, -0.00037, -0.000054, 0.000377, 0.000767, 0.001056, 0.001119, 0.000832,
0.000243, -0.000485, -0.001125, -0.001535, -0.001667, -0.001501, -0.001126, -0.000699, -0.000363, -0.00018,
-0.000114, -0.000144, -0.000318, -0.00073, -0.001363, -0.002044, -0.002533, -0.002548, -0.002082, -0.001439,
-0.000836, -0.000403, -0.000317, -0.000642, -0.001182, -0.001633, -0.001839, -0.001823, -0.001777, -0.001932,
-0.002376, -0.003, -0.003521, -0.003666, -0.003285, -0.002339, -0.001018, 0.0004, 0.001625, 0.002272,
0.0022, 0.001589, 0.000684, -0.000176, -0.000769, -0.001113, -0.001252, -0.001339, -0.001491, -0.001655,
-0.001854, -0.002016, -0.002024, -0.001972, -0.001836, -0.001594, -0.00144, -0.001432, -0.00154, -0.001664,
-0.001702, -0.001804, -0.001978, -0.00204, -0.001951, -0.001733, -0.001485, -0.001357, -0.001434, -0.00173,
-0.002101, -0.002389, -0.002546, -0.002564, -0.00243, -0.002162, -0.001892, -0.001745, -0.00177, -0.002015,
-0.002368, -0.002586, -0.002561, -0.002307, -0.001916, -0.001546, -0.001329, -0.00135, -0.001617, -0.002037,
-0.002475, -0.002751, -0.002742, -0.002547, -0.002306, -0.002072, -0.001866, -0.001678, -0.00153, -0.001428,
-0.001304, -0.001197, -0.001217, -0.00
};



    //R1.00 Mako R212 01 Low
    float TIR3[1024] = {
-0.000007, 0.000014, -0.000023, 0.000024, -0.000036, -0.000036, 0.001526, 0.081998, 0.3279, 0.516397,
0.466909, 0.384919, 0.279672, 0.131189, 0.063375, 0.016903, 0.010446, 0.043229, 0.081529, 0.115317,
0.094326, 0.050795, -0.00626, -0.039965, -0.060993, -0.071466, -0.044494, -0.009321, 0.032794, 0.042209,
0.030808, 0.002632, -0.023618, -0.029122, -0.031982, -0.026774, -0.035583, -0.040626, -0.050951, -0.058227,
-0.061209, -0.063984, -0.056738, -0.053312, -0.041496, -0.036365, -0.027652, -0.022656, -0.018308, -0.013589,
-0.014913, -0.013692, -0.020217, -0.021916, -0.028548, -0.030922, -0.034348, -0.036734, -0.036838, -0.040027,
-0.03942, -0.044601, -0.046165, -0.053037, -0.057258, -0.063257, -0.068159, -0.070749, -0.074502, -0.073586,
-0.075763, -0.073076, -0.073817, -0.071144, -0.070176, -0.067971, -0.064745, -0.062783, -0.057781, -0.056119,
-0.050974, -0.049872, -0.046265, -0.045564, -0.044132, -0.043233, -0.043573, -0.042043, -0.043221, -0.041074,
-0.042041, -0.039745, -0.039803, -0.038034, -0.037008, -0.036322, -0.034588, -0.035134, -0.033318, -0.034733,
-0.033483, -0.035119, -0.034844, -0.035961, -0.036626, -0.03671, -0.037933, -0.036907, -0.038101, -0.036316,
-0.036896, -0.034885, -0.034373, -0.032599, -0.030912, -0.029733, -0.02726, -0.026814, -0.024266, -0.024442,
-0.02253, -0.022949, -0.022078, -0.022269, -0.022451, -0.02209, -0.022988, -0.022053, -0.023182, -0.021971,
-0.022836, -0.021752, -0.021964, -0.021317, -0.020677, -0.0205, -0.019039, -0.019125, -0.017133, -0.01713,
-0.015034, -0.014605, -0.012847, -0.011808, -0.010667, -0.009052, -0.008571, -0.00663, -0.0066, -0.004705,
-0.004775, -0.003291, -0.00311, -0.002273, -0.001626, -0.001458, -0.000342, -0.000645, 0.000742, 0.000309,
0.001658, 0.001466, 0.002485, 0.002801, 0.003326, 0.004227, 0.004284, 0.005624, 0.005401, 0.006872,
0.006651, 0.007897, 0.007936, 0.00868, 0.009129, 0.009278, 0.010119, 0.009779, 0.010839, 0.010287,
0.011308, 0.010869, 0.011594, 0.011542, 0.011828, 0.01228, 0.012135, 0.013021, 0.012616, 0.013718,
0.013314, 0.014333, 0.014189, 0.014878, 0.015159, 0.015383, 0.016089, 0.015891, 0.016872, 0.016434,
0.01742, 0.016999, 0.017726, 0.017556, 0.017831, 0.018025, 0.017815, 0.01835, 0.017776, 0.018473,
0.01777, 0.018395, 0.017834, 0.01816, 0.017931, 0.017845, 0.018021, 0.01755, 0.018038, 0.017339,
0.017945, 0.017262, 0.017743, 0.017293, 0.01746, 0.017385, 0.017167, 0.017457, 0.016913, 0.01744,
0.016753, 0.017299, 0.016687, 0.017031, 0.016688, 0.016688, 0.016699, 0.016325, 0.01665, 0.016017,
0.016503, 0.015796, 0.016229, 0.015669, 0.015861, 0.015599, 0.015438, 0.015521, 0.015029, 0.015378,
0.014683, 0.015117, 0.014422, 0.014741, 0.01424, 0.014273, 0.014087, 0.013776, 0.013915, 0.013317,
0.013667, 0.012945, 0.013328, 0.012685, 0.012905, 0.012512, 0.012444, 0.012389, 0.012006, 0.012249,
0.011645, 0.01205, 0.011396, 0.011768, 0.01125, 0.011416, 0.011175, 0.011038, 0.011109, 0.01068,
0.010997, 0.010394, 0.010796, 0.010191, 0.010498, 0.010068, 0.010131, 0.009978, 0.009736, 0.009871,
0.009374, 0.009699, 0.009078, 0.009432, 0.008865, 0.00908, 0.008715, 0.008669, 0.008587, 0.008254,
0.008431, 0.007882, 0.008204, 0.007588, 0.007894, 0.007377, 0.00751, 0.007225, 0.007094, 0.007092,
0.006696, 0.006925, 0.006362, 0.006696, 0.00612, 0.006394, 0.005963, 0.006041, 0.005864, 0.005679,
0.005774, 0.005356, 0.005651, 0.005112, 0.005462, 0.004959, 0.00521, 0.004887, 0.004919, 0.004854,
0.00463, 0.004817, 0.00439, 0.004731, 0.004226, 0.004576, 0.004147, 0.004358, 0.004129, 0.004107,
0.004133, 0.003866, 0.004112, 0.003675, 0.004031, 0.003558, 0.003877, 0.003511, 0.003661, 0.003508,
0.003419, 0.003508, 0.00319, 0.003466, 0.003013, 0.003355, 0.002901, 0.003167, 0.002848, 0.002923,
0.002824, 0.002658, 0.002786, 0.002416, 0.002698, 0.002226, 0.002537, 0.002103, 0.002306, 0.002033,
0.002031, 0.001982, 0.00175, 0.001913, 0.001505, 0.001791, 0.001322, 0.001603, 0.001207, 0.001355,
0.001142, 0.001078, 0.00109, 0.000811, 0.001013, 0.000589, 0.000881, 0.000433, 0.000685, 0.000341,
0.000438, 0.000289, 0.000172, 0.000239, -0.000077, 0.000154, -0.000276, 0.00001, -0.000411, -0.000196,
-0.000491, -0.000449, -0.000543, -0.000713, -0.000604, -0.000953, -0.000707, -0.00114, -0.000871, -0.001265,
-0.001095, -0.001341, -0.001355, -0.001397, -0.001617, -0.001469, -0.001843, -0.001586, -0.002011, -0.001762,
-0.002116, -0.001987, -0.002176, -0.002236, -0.002221, -0.002472, -0.002283, -0.002662, -0.002391, -0.002787,
-0.002551, -0.002849, -0.00275, -0.00287, -0.00296, -0.002881, -0.003146, -0.002915, -0.00328, -0.002994,
-0.003346, -0.003122, -0.003356, -0.003281, -0.00333, -0.003443, -0.003305, -0.003576, -0.003309, -0.003652,
-0.003361, -0.003668, -0.003459, -0.003631, -0.003583, -0.00357, -0.003706, -0.003517, -0.003793, -0.003501,
-0.003825, -0.003533, -0.003796, -0.003609, -0.003724, -0.003707, -0.003637, -0.003795, -0.003565, -0.003844,
-0.003535, -0.003835, -0.003554, -0.003772, -0.003614, -0.003673, -0.003688, -0.003566, -0.003747, -0.003485,
-0.003762, -0.003448, -0.003723, -0.003462, -0.003635, -0.00351, -0.003518, -0.003568, -0.003406, -0.003605,
-0.003324, -0.003598, -0.003291, -0.00354, -0.003306, -0.003439, -0.003354, -0.00332, -0.003404, -0.003211,
-0.003429, -0.00314, -0.003411, -0.003119, -0.003343, -0.003143, -0.00324, -0.003195, -0.003125, -0.003243,
-0.003029, -0.003264, -0.002972, -0.003238, -0.002966, -0.003166, -0.003002, -0.003063, -0.003057, -0.002956,
-0.003106, -0.002872, -0.003119, -0.00283, -0.003086, -0.002837, -0.003009, -0.002881, -0.002906, -0.002938,
-0.002804, -0.002979, -0.00273, -0.002983, -0.0027, -0.002939, -0.002714, -0.002853, -0.00276, -0.002747,
-0.002811, -0.002647, -0.002841, -0.00258, -0.00283, -0.002555, -0.002771, -0.002573, -0.002676, -0.002615,
-0.002564, -0.002656, -0.002466, -0.002671, -0.0024, -0.002642, -0.002379, -0.002568, -0.002396, -0.002461,
-0.002432, -0.002347, -0.002461, -0.002248, -0.00246, -0.002186, -0.002415, -0.002167, -0.002328, -0.002182,
-0.002213, -0.002212, -0.002094, -0.002229, -0.001997, -0.002214, -0.001939, -0.002154, -0.001922, -0.002054,
-0.001936, -0.001931, -0.001958, -0.00181, -0.001963, -0.001715, -0.001931, -0.001659, -0.001856, -0.001644,
-0.001743, -0.001652, -0.001612, -0.001665, -0.001489, -0.001655, -0.001395, -0.001606, -0.001341, -0.001514,
-0.001323, -0.001389, -0.001326, -0.001252, -0.001327, -0.001127, -0.001302, -0.001035, -0.001237, -0.000982,
-0.001131, -0.000963, -0.000997, -0.000961, -0.000856, -0.000952, -0.000734, -0.000915, -0.000646, -0.000838,
-0.000599, -0.000724, -0.000583, -0.000588, -0.00058, -0.000451, -0.000568, -0.000338, -0.000526, -0.000262,
-0.000445, -0.000227, -0.000332, -0.000222, -0.000201, -0.000225, -0.000078, -0.000216, 0.000018, -0.000177,
0.000074, -0.000101, 0.000089, 0.000003, 0.000078, 0.000118, 0.000061, 0.000221, 0.00006, 0.000292,
0.000089, 0.000321, 0.000153, 0.000312, 0.000241, 0.000279, 0.000334, 0.000244, 0.00041, 0.000228,
0.000452, 0.000244, 0.000453, 0.000293, 0.000417, 0.000362, 0.000363, 0.000433, 0.000312, 0.000483,
0.000283, 0.000498, 0.000288, 0.000473, 0.000324, 0.000417, 0.00038, 0.000348, 0.000433, 0.000288,
0.000465, 0.000255, 0.000462, 0.000258, 0.000423, 0.000293, 0.000358, 0.000344, 0.000287, 0.000392,
0.000231, 0.000418, 0.000208, 0.00041, 0.000222, 0.000371, 0.000267, 0.000311, 0.000326, 0.000252,
0.000381, 0.000213, 0.000412, 0.00021, 0.00041, 0.000244, 0.00038, 0.000307, 0.000335, 0.000381,
0.000294, 0.000447, 0.000278, 0.000487, 0.000297, 0.000494, 0.000352, 0.000475, 0.000433, 0.000444,
0.00052, 0.000421, 0.000594, 0.000424, 0.00064, 0.000462, 0.000652, 0.000533, 0.000639, 0.000624,
0.000617, 0.000717, 0.000605, 0.000791, 0.000619, 0.000834, 0.000667, 0.000843, 0.000744, 0.000829,
0.000837, 0.000807, 0.000925, 0.000798, 0.000991, 0.000816, 0.001023, 0.000865, 0.001022, 0.00094,
0.000999, 0.001025, 0.000971, 0.001101, 0.000958, 0.001151, 0.000972, 0.001166, 0.001016, 0.001149,
0.001082, 0.001112, 0.001153, 0.001074, 0.001211, 0.001051, 0.00124, 0.001056, 0.001233, 0.00109,
0.001196, 0.001141, 0.001142, 0.001195, 0.00109, 0.001231, 0.001056, 0.001237, 0.001049, 0.001206,
0.001069, 0.001148, 0.001105, 0.001076, 0.001138, 0.001009, 0.001152, 0.000963, 0.001133, 0.000945,
0.001081, 0.000952, 0.001002, 0.000972, 0.000915, 0.000988, 0.000837, 0.00098, 0.000782, 0.000941,
0.000755, 0.000869, 0.000752, 0.000776, 0.00076, 0.000678, 0.00076, 0.000593, 0.000737, 0.000534,
0.000682, 0.000504, 0.000598, 0.000497, 0.000497, 0.000497, 0.000395, 0.000488, 0.000311, 0.000456,
0.000254, 0.000393, 0.000227, 0.000304, 0.000222, 0.000202, 0.000222, 0.000105, 0.000211, 0.000028,
0.000176, -0.000019, 0.000112, -0.000037, 0.000026, -0.000035, -0.00007, -0.000029, -0.000157, -0.000037,
-0.00022, -0.000069, -0.000253, -0.000128, -0.000257, -0.000207, -0.000243, -0.000291, -0.000228, -0.000363,
-0.000229, -0.000409, -0.000254, -0.000424, -0.000305, -0.000412, -0.000373, -0.000384, -0.000443, -0.000359,
-0.000498, -0.000351, -0.000526, -0.000369, -0.000522, -0.000411, -0.000494, -0.000467, -0.000453, -0.000522,
-0.000418, -0.000559, -0.000402, -0.000569, -0.000413, -0.000549, -0.000446, -0.000506, -0.000491, -0.000454,
-0.000532, -0.000411, -0.000554, -0.000389, -0.000547, -0.000394, -0.000512, -0.00042, -0.000457, -0.000456,
-0.000398, -0.000485, -0.00035, -0.000494, -0.000325, -0.000473, -0.000326, -0.000427, -0.000347, -0.000364,
-0.000376, -0.0003, -0.000396, -0.000249, -0.000393, -0.000224, -0.000363, -0.000224, -0.000309, -0.000243,
-0.00024, -0.000267, -0.000175, -0.00028, -0.000126, -0.00027, -0.000102, -0.000232, -0.000104, -0.000173,
-0.000123, -0.000103, -0.000145, -0.000039, -0.000153, 0.000006, -0.000138, 0.000024, -0.000096, 0.000018,
-0.000035, -0.000002, 0.000034, -0.000023, 0.000093, -0.000029, 0.000131, -0.000012, 0.000143, 0.000031,
0.000131, 0.000092, 0.000106, 0.000156, 0.000084, 0.000208, 0.000078, 0.000238, 0.000097, 0.000241,
0.000139, 0.000222, 0.000197, 0.000192, 0.000254, 0.000168, 0.000298, 0.000161, 0.000318, 0.000179,
0.000312, 0.00022, 0.000285, 0.000273, 0.00025, 0.000323, 0.000224, 0.000358, 0.000216, 0.000368,
0.000234, 0.000352, 0.000271, 0.000318, 0.000319, 0.000279, 0.000362, 0.000251, 0.000388, 0.000243,
0.000388, 0.000259, 0.000363, 0.000294, 0.000324, 0.000338, 0.000282, 0.000374, 0.000252, 0.000391,
0.000245, 0.000383, 0.000261, 0.000351, 0.000295, 0.000307, 0.000334, 0.000264, 0.000365, 0.000235,
0.000375, 0.000229, 0.00036, 0.000247, 0.000323, 0.00028, 0.000276, 0.000317, 0.000233, 0.000342,
0.000207, 0.000346, 0.000204, 0.000326, 0.000225, 0.000287, 0.000259, 0.00024, 0.000293, 0.000199,
0.000315, 0.000177, 0.000315, 0.000
};

//R1.00 Mako R212 01 High
float TIR3B[1024] = {
-0.000009, 0.000015, -0.000024, 0.000024, -0.000038, -0.000054, 0.001905, 0.103767, 0.442996, 0.777158,
0.792085, 0.623658, 0.320457, -0.088845, -0.402077, -0.605246, -0.652465, -0.540333, -0.331422, -0.086337,
0.08322, 0.163427, 0.152813, 0.106198, 0.042641, -0.017319, -0.022918, 0.009315, 0.072957, 0.108992,
0.107478, 0.064958, 0.005969, -0.032718, -0.056132, -0.055181, -0.056575, -0.048816, -0.043925, -0.036373,
-0.024431, -0.012934, 0.008701, 0.025642, 0.04871, 0.061119, 0.071091, 0.0716, 0.065746, 0.056751,
0.039081, 0.023276, 0.000408, -0.015144, -0.031245, -0.038061, -0.040154, -0.036756, -0.02749, -0.020192,
-0.009639, -0.007346, -0.004958, -0.011411, -0.018359, -0.028377, -0.037391, -0.04215, -0.045615, -0.041403,
-0.038007, -0.02844, -0.022136, -0.013619, -0.008442, -0.004387, -0.001141, -0.001047, 0.00098, -0.001284,
-0.000623, -0.004359, -0.005953, -0.010394, -0.01416, -0.017868, -0.022332, -0.02388, -0.027228, -0.026167,
-0.027302, -0.024551, -0.023663, -0.021051, -0.019281, -0.01854, -0.017271, -0.019116, -0.019142, -0.022874,
-0.024162, -0.028204, -0.030129, -0.032776, -0.034446, -0.034647, -0.03542, -0.033251, -0.032956, -0.029474,
-0.028359, -0.024954, -0.023316, -0.021019, -0.019194, -0.018555, -0.017027, -0.018093, -0.017409, -0.019766,
-0.02024, -0.023, -0.024425, -0.026496, -0.028255, -0.028836, -0.030235, -0.029208, -0.029864, -0.027877,
-0.027788, -0.025809, -0.025152, -0.023943, -0.022864, -0.022591, -0.021136, -0.021449, -0.019743, -0.020091,
-0.018407, -0.018359, -0.017105, -0.01653, -0.016042, -0.015072, -0.015428, -0.014352, -0.015297, -0.014405,
-0.015448, -0.014941, -0.015572, -0.015519, -0.01542, -0.015751, -0.01491, -0.01543, -0.014119, -0.014564,
-0.0132, -0.013308, -0.012272, -0.011867, -0.011369, -0.010434, -0.010454, -0.009162, -0.009503, -0.00817,
-0.008542, -0.007515, -0.007654, -0.007183, -0.006925, -0.007047, -0.00638, -0.006924, -0.005982, -0.006614,
-0.00562, -0.005989, -0.00519, -0.005045, -0.004597, -0.003872, -0.003803, -0.002638, -0.002817, -0.001487,
-0.001689, -0.000527, -0.000517, 0.000226, 0.000602, 0.000814, 0.001554, 0.00131, 0.002278, 0.001797,
0.002741, 0.002304, 0.002988, 0.00285, 0.00311, 0.003387, 0.003214, 0.003879, 0.003416, 0.00428,
0.003763, 0.004582, 0.004274, 0.00482, 0.004889, 0.005043, 0.005538, 0.005332, 0.006136, 0.005723,
0.006623, 0.006243, 0.006992, 0.006848, 0.007255, 0.007476, 0.00748, 0.008043, 0.007712, 0.008478,
0.008007, 0.008761, 0.008372, 0.008897, 0.008785, 0.008957, 0.009201, 0.009001, 0.009551, 0.009107,
0.009806, 0.00931, 0.009937, 0.009604, 0.009977, 0.009949, 0.009959, 0.010267, 0.009944, 0.010501,
0.009983, 0.010601, 0.010094, 0.010576, 0.010277, 0.010462, 0.010483, 0.010325, 0.010671, 0.010238,
0.010785, 0.010245, 0.010809, 0.010367, 0.010752, 0.010567, 0.010649, 0.010798, 0.01056, 0.010985,
0.010521, 0.011079, 0.010567, 0.011059, 0.01068, 0.010938, 0.01083, 0.010766, 0.010958, 0.010595,
0.011012, 0.010482, 0.010964, 0.010442, 0.010807, 0.01047, 0.010582, 0.010526, 0.010331, 0.010558,
0.010115, 0.010522, 0.009967, 0.010387, 0.009901, 0.010169, 0.009896, 0.009895, 0.009907, 0.009622,
0.009888, 0.009394, 0.009792, 0.009245, 0.009612, 0.009178, 0.009359, 0.009165, 0.009077, 0.009165,
0.008814, 0.009122, 0.008612, 0.00901, 0.008497, 0.008816, 0.008457, 0.008566, 0.008462, 0.008301,
0.008461, 0.008066, 0.008411, 0.007901, 0.008282, 0.007814, 0.008076, 0.007792, 0.007821, 0.007794,
0.00756, 0.007774, 0.007341, 0.007693, 0.007189, 0.007533, 0.007114, 0.007304, 0.007089, 0.007036,
0.007075, 0.006778, 0.007028, 0.006567, 0.006914, 0.006429, 0.006725, 0.006357, 0.006473, 0.006324,
0.006197, 0.006289, 0.005938, 0.006209, 0.005733, 0.00606, 0.005597, 0.005837, 0.00552, 0.005564,
0.005474, 0.005278, 0.005415, 0.005023, 0.005308, 0.004829, 0.005133, 0.004706, 0.004896, 0.00464,
0.004621, 0.004596, 0.004349, 0.004533, 0.004119, 0.004418, 0.003955, 0.004237, 0.00386, 0.004001,
0.003812, 0.003737, 0.003775, 0.003486, 0.003707, 0.003281, 0.003581, 0.00314, 0.003388, 0.003061,
0.003146, 0.003017, 0.002884, 0.00297, 0.002641, 0.002883, 0.002448, 0.002734, 0.002318, 0.002522,
0.002242, 0.002267, 0.002191, 0.002002, 0.002127, 0.001765, 0.00202, 0.001585, 0.001852, 0.001467,
0.001626, 0.001397, 0.001368, 0.001345, 0.001111, 0.001274, 0.000893, 0.001158, 0.000737, 0.000984,
0.000642, 0.000761, 0.000591, 0.000516, 0.00055, 0.000285, 0.000486, 0.000101, 0.000373, -0.00002,
0.000206, -0.000083, -0.000003, -0.000111, -0.000225, -0.000137, -0.000424, -0.000195, -0.000575, -0.000302,
-0.000663, -0.000463, -0.000701, -0.000659, -0.00071, -0.000859, -0.000728, -0.001033, -0.000783, -0.001154,
-0.00089, -0.001217, -0.001046, -0.001232, -0.001229, -0.001229, -0.001411, -0.001241, -0.001556, -0.001292,
-0.001648, -0.001394, -0.00168, -0.001539, -0.001673, -0.001706, -0.001655, -0.001862, -0.001657, -0.001978,
-0.001702, -0.002036, -0.001797, -0.00204, -0.001932, -0.002012, -0.002079, -0.001979, -0.002208, -0.001974,
-0.002293, -0.002015, -0.002322, -0.002105, -0.002303, -0.002228, -0.002257, -0.002357, -0.002217, -0.002463,
-0.002209, -0.002521, -0.00225, -0.002527, -0.002336, -0.002489, -0.002451, -0.002435, -0.002565, -0.00239,
-0.002647, -0.002383, -0.002683, -0.002423, -0.002668, -0.002507, -0.002618, -0.002613, -0.002555, -0.00271,
-0.002509, -0.002774, -0.002502, -0.002788, -0.002543, -0.002755, -0.002623, -0.00269, -0.002718, -0.002621,
-0.0028, -0.002575, -0.002844, -0.00257, -0.002839, -0.002612, -0.002788, -0.002686, -0.002712, -0.002771,
-0.002638, -0.002836, -0.002592, -0.002861, -0.00259, -0.002836, -0.00263, -0.002769, -0.0027, -0.002684,
-0.002771, -0.002606, -0.00282, -0.002563, -0.002825, -0.002563, -0.002782, -0.002604, -0.002704, -0.002667,
-0.002612, -0.002725, -0.002535, -0.002756, -0.002492, -0.002743, -0.002495, -0.002686, -0.002534, -0.002598,
-0.002591, -0.002504, -0.00264, -0.002427, -0.002655, -0.002389, -0.002626, -0.002393, -0.002556, -0.002432,
-0.002461, -0.002483, -0.002363, -0.002518, -0.002289, -0.002519, -0.002254, -0.002475, -0.00226, -0.002393,
-0.002297, -0.002289, -0.002338, -0.002189, -0.00236, -0.002117, -0.002345, -0.002085, -0.002286, -0.002092,
-0.002191, -0.002122, -0.002079, -0.002153, -0.001978, -0.002162, -0.001908, -0.00213, -0.001878, -0.002056,
-0.001884, -0.001949, -0.001908, -0.001833, -0.001929, -0.001732, -0.001923, -0.001665, -0.001877, -0.001637,
-0.001789, -0.001642, -0.001675, -0.001662, -0.001558, -0.001674, -0.00146, -0.001657, -0.001398, -0.001598,
-0.001375, -0.001503, -0.001382, -0.001386, -0.0014, -0.001271, -0.001406, -0.001179, -0.00138, -0.001126,
-0.001315, -0.001111, -0.001215, -0.001123, -0.001099, -0.001142, -0.000991, -0.001143, -0.000909, -0.001112,
-0.000868, -0.001043, -0.000864, -0.000944, -0.000883, -0.000834, -0.000904, -0.000736, -0.000905, -0.000669,
-0.000872, -0.000642, -0.000802, -0.00065, -0.000707, -0.000677, -0.000607, -0.000702, -0.000522, -0.000704,
-0.00047, -0.000671, -0.000458, -0.000603, -0.000478, -0.000513, -0.000512, -0.000422, -0.000541, -0.000351,
-0.000543, -0.000314, -0.000509, -0.000315, -0.000442, -0.000344, -0.000355, -0.000383, -0.000272, -0.000411,
-0.000212, -0.000409, -0.000186, -0.000371, -0.000195, -0.0003, -0.000229, -0.000216, -0.000267, -0.000137,
-0.00029, -0.000083, -0.00028, -0.000064, -0.000234, -0.000078, -0.00016, -0.000113, -0.000074, -0.000147,
0.000002, -0.000161, 0.000051, -0.000141, 0.000066, -0.000088, 0.000048, -0.000009, 0.000015, 0.000077,
-0.000015, 0.00015, -0.000021, 0.000194, 0.000006, 0.000202, 0.000066, 0.000182, 0.000148, 0.000149,
0.000233, 0.000124, 0.000301, 0.000125, 0.000338, 0.000158, 0.00034, 0.000223, 0.000315, 0.000305,
0.000282, 0.000386, 0.000261, 0.000447, 0.000266, 0.000475, 0.000305, 0.00047, 0.000372, 0.000442,
0.000454, 0.000409, 0.000529, 0.000389, 0.000581, 0.000399, 0.0006, 0.000441, 0.000587, 0.000509,
0.000554, 0.000586, 0.000519, 0.000653, 0.000502, 0.000696, 0.000514, 0.000705, 0.000558, 0.000684,
0.000624, 0.000644, 0.000695, 0.000607, 0.000752, 0.000589, 0.000782, 0.000602, 0.000779, 0.000644,
0.000748, 0.000706, 0.000702, 0.000769, 0.000662, 0.000816, 0.000643, 0.000832, 0.000654, 0.000815,
0.000692, 0.000771, 0.000747, 0.000718, 0.0008, 0.000675, 0.000834, 0.000654, 0.000836, 0.000662,
0.000805, 0.000696, 0.000751, 0.000744, 0.000692, 0.000787, 0.000645, 0.000807, 0.000621, 0.000794,
0.000628, 0.000751, 0.000657, 0.000688, 0.000697, 0.000623, 0.000728, 0.000573, 0.000736, 0.00055,
0.000713, 0.000556, 0.00066, 0.000582, 0.00059, 0.000615, 0.000521, 0.000637, 0.000471, 0.000635,
0.000449, 0.000601, 0.000455, 0.00054, 0.00048, 0.000467, 0.000509, 0.000398, 0.000524, 0.00035,
0.000512, 0.000331, 0.000471, 0.000339, 0.000406, 0.000365, 0.000332, 0.00039, 0.000266, 0.0004,
0.000223, 0.000383, 0.000208, 0.000336, 0.000221, 0.000269, 0.000247, 0.000197, 0.000272, 0.000136,
0.000278, 0.000099, 0.000257, 0.000092, 0.000208, 0.00011, 0.000141, 0.000139, 0.000073, 0.000164,
0.000019, 0.000167, -0.00001, 0.000143, -0.000009, 0.000093, 0.000015, 0.00003, 0.000048, -0.000032,
0.000073, -0.000078, 0.000076, -0.000097, 0.00005, -0.000088, 0.000002, -0.000057, -0.000059, -0.000021,
-0.000114, 0.000005, -0.00015, 0.000008, -0.000158, -0.000017, -0.00014, -0.000064, -0.000103, -0.00012,
-0.000064, -0.000168, -0.000037, -0.000194, -0.000034, -0.000191, -0.000058, -0.000164, -0.000102, -0.000121,
-0.000153, -0.00008, -0.000193, -0.000053, -0.000209, -0.00005, -0.000198, -0.000074, -0.000163, -0.000115,
-0.000115, -0.00016, -0.000071, -0.000192, -0.000044, -0.0002, -0.000043, -0.000179, -0.000066, -0.000137,
-0.000105, -0.000086, -0.000145, -0.000042, -0.00017, -0.000017, -0.00017, -0.000018, -0.000143, -0.000042,
-0.000095, -0.000078, -0.000042, -0.000113, 0.000002, -0.000132, 0.000023, -0.000126, 0.000019, -0.000094,
-0.000006, -0.000044, -0.000042, 0.00001, -0.000073, 0.000052, -0.000087, 0.00007, -0.000074, 0.000063,
-0.000038, 0.000035, 0.000014, -0.000001, 0.000065, -0.00003, 0.000102, -0.000039, 0.000116, -0.000022,
0.000105, 0.000018, 0.000075, 0.000069, 0.000039, 0.000118, 0.000012, 0.000151, 0.000007, 0.000159,
0.000028, 0.000142, 0.000068, 0.000109, 0.000118, 0.000073, 0.000164, 0.000049, 0.000191, 0.000047,
0.000194, 0.00007, 0.000172, 0.000112, 0.000136, 0.00016, 0.0001, 0.000201, 0.000079, 0.000223,
0.00008, 0.000219, 0.000104, 0.000193, 0.000146, 0.000155, 0.000193, 0.00012, 0.00023, 0.000101,
0.000246, 0.000105, 0.000236, 0.000132, 0.000206, 0.000173, 0.000166, 0.000217, 0.000131, 0.000248,
0.000115, 0.000259, 0.000122, 0.000244, 0.000151, 0.000211, 0.000193, 0.000171, 0.000234, 0.000138,
0.000261, 0.000123, 0.000265, 0.000133
};


    //R1.00 5153 LOW
    float TIR4[1024] = {
0.001573, -0.000097, -0.003495, -0.009735, -0.017993, -0.026174, -0.033382, -0.035981, -0.034231, -0.030029,
-0.016267, 0.033872, 0.144912, 0.278801, 0.369441, 0.410004, 0.4066, 0.352822, 0.265447, 0.180621,
0.12151, 0.081644, 0.063653, 0.063547, 0.064848, 0.058625, 0.043887, 0.025737, 0.002699, -0.019266,
-0.038306, -0.054923, -0.066971, -0.071725, -0.066628, -0.056923, -0.042233, -0.027393, -0.01631, -0.011228,
-0.013679, -0.021914, -0.036257, -0.052505, -0.068855, -0.081967, -0.091619, -0.096683, -0.097434, -0.095421,
-0.090298, -0.085448, -0.079798, -0.075498, -0.071757, -0.068649, -0.066056, -0.062487, -0.05936, -0.055617,
-0.053042, -0.05128, -0.05122, -0.052861, -0.055987, -0.060024, -0.064237, -0.068197, -0.070788, -0.072218,
-0.07129, -0.069198, -0.065171, -0.060802, -0.056131, -0.051387, -0.048124, -0.044776, -0.042176, -0.039775,
-0.037392, -0.034825, -0.032385, -0.029806, -0.027831, -0.026163, -0.024765, -0.024012, -0.023157, -0.022889,
-0.022278, -0.022178, -0.021962, -0.021929, -0.02194, -0.022135, -0.022921, -0.023621, -0.024908, -0.026123,
-0.02804, -0.030057, -0.032303, -0.034726, -0.037172, -0.03949, -0.041652, -0.043697, -0.045072, -0.046301,
-0.046624, -0.046757, -0.046182, -0.045115, -0.044085, -0.042542, -0.04088, -0.039066, -0.036918, -0.034499,
-0.032218, -0.029763, -0.027309, -0.024838, -0.02255, -0.020187, -0.017935, -0.015732, -0.013554, -0.011448,
-0.009191, -0.007232, -0.005223, -0.003506, -0.001875, -0.00073, 0.000215, 0.000894, 0.001276, 0.001549,
0.001628, 0.001839, 0.001538, 0.001746, 0.001787, 0.001957, 0.002458, 0.002667, 0.003293, 0.00378,
0.004376, 0.005216, 0.006083, 0.006846, 0.007841, 0.008874, 0.009764, 0.010555, 0.011217, 0.012064,
0.012621, 0.012936, 0.013559, 0.013822, 0.013827, 0.014226, 0.014421, 0.014679, 0.014629, 0.014684,
0.014883, 0.014647, 0.014405, 0.013983, 0.01348, 0.012551, 0.011726, 0.010784, 0.009605, 0.008439,
0.007088, 0.005609, 0.004, 0.002596, 0.001006, -0.000433, -0.001627, -0.002788, -0.003841, -0.004464,
-0.004899, -0.004986, -0.004979, -0.004944, -0.004107, -0.003578, -0.002457, -0.001063, 0.000172, 0.00202,
0.003811, 0.005828, 0.007807, 0.009827, 0.011957, 0.013848, 0.015734, 0.01768, 0.019235, 0.020797,
0.022331, 0.023669, 0.024937, 0.025684, 0.026465, 0.026933, 0.026959, 0.027003, 0.026841, 0.026391,
0.025993, 0.02542, 0.024791, 0.024228, 0.023265, 0.022503, 0.021702, 0.020888, 0.020136, 0.019302,
0.018587, 0.018163, 0.017789, 0.017341, 0.017278, 0.016985, 0.016842, 0.016835, 0.016781, 0.016794,
0.016733, 0.016819, 0.016828, 0.01699, 0.017065, 0.017038, 0.017187, 0.017079, 0.016624, 0.0164,
0.015993, 0.015379, 0.014899, 0.014178, 0.013569, 0.012939, 0.012238, 0.011538, 0.010741, 0.009933,
0.009156, 0.008369, 0.007791, 0.007293, 0.006712, 0.006392, 0.00622, 0.006189, 0.006242, 0.006372,
0.006418, 0.006745, 0.007066, 0.007435, 0.008125, 0.008609, 0.009379, 0.010073, 0.011006, 0.011856,
0.012568, 0.013561, 0.014408, 0.015302, 0.015849, 0.016573, 0.017246, 0.017864, 0.018597, 0.019075,
0.019498, 0.019461, 0.019207, 0.01881, 0.018127, 0.017466, 0.016765, 0.016067, 0.015658, 0.015183,
0.01487, 0.014642, 0.014043, 0.013473, 0.013085, 0.012758, 0.012292, 0.012033, 0.01169, 0.011382,
0.011296, 0.011196, 0.011179, 0.011006, 0.010965, 0.010826, 0.010515, 0.010322, 0.010208, 0.010106,
0.009945, 0.009708, 0.00966, 0.009705, 0.009533, 0.009512, 0.009498, 0.009266, 0.00907, 0.008973,
0.008748, 0.008683, 0.008455, 0.00827, 0.008318, 0.007951, 0.007865, 0.00782, 0.007466, 0.007085,
0.006975, 0.006903, 0.00664, 0.006747, 0.006783, 0.006626, 0.006702, 0.006696, 0.006655, 0.006863,
0.006765, 0.006726, 0.006872, 0.006951, 0.007131, 0.007246, 0.007309, 0.007324, 0.007453, 0.007309,
0.007306, 0.007391, 0.007165, 0.007123, 0.007034, 0.007047, 0.007022, 0.00687, 0.006748, 0.006548,
0.00634, 0.005982, 0.005747, 0.005524, 0.005078, 0.004842, 0.004673, 0.004486, 0.004167, 0.003973,
0.003855, 0.003416, 0.003118, 0.002854, 0.002675, 0.002526, 0.002447, 0.002282, 0.00208, 0.002145,
0.002062, 0.001983, 0.001974, 0.001804, 0.001629, 0.001692, 0.001599, 0.00167, 0.001691, 0.001646,
0.001741, 0.001656, 0.001818, 0.001714, 0.001611, 0.001586, 0.001564, 0.001524, 0.001474, 0.001561,
0.001446, 0.001396, 0.001352, 0.001406, 0.001307, 0.001232, 0.001205, 0.001063, 0.001017, 0.000984,
0.000935, 0.00091, 0.000883, 0.000658, 0.000603, 0.000557, 0.000473, 0.000418, 0.000311, 0.00014,
0.000045, -0.000056, -0.000197, -0.0002, -0.000373, -0.000481, -0.000652, -0.000721, -0.00078, -0.000895,
-0.001048, -0.001258, -0.001297, -0.001489, -0.00154, -0.001612, -0.001775, -0.001928, -0.002028, -0.002104,
-0.002247, -0.002288, -0.002403, -0.002554, -0.00265, -0.002757, -0.002822, -0.002857, -0.003028, -0.003112,
-0.003185, -0.003327, -0.003389, -0.003362, -0.00352, -0.003683, -0.003722, -0.003862, -0.003866, -0.003949,
-0.004032, -0.004182, -0.004229, -0.004291, -0.004379, -0.004351, -0.004498, -0.004627, -0.004723, -0.004651,
-0.004802, -0.004794, -0.004862, -0.005048, -0.00501, -0.005155, -0.005081, -0.005156, -0.005231, -0.005315,
-0.005402, -0.005428, -0.005498, -0.005434, -0.005546, -0.005639, -0.005703, -0.005735, -0.005776, -0.005744,
-0.005823, -0.005982, -0.005906, -0.005973, -0.006009, -0.005994, -0.00606, -0.006112, -0.006177, -0.006139,
-0.006124, -0.006174, -0.006197, -0.00629, -0.006323, -0.006293, -0.006311, -0.006282, -0.006316, -0.006427,
-0.006429, -0.0064, -0.006439, -0.006398, -0.006411, -0.006518, -0.006497, -0.006512, -0.00649, -0.006417,
-0.006515, -0.006561, -0.006534, -0.006538, -0.006519, -0.006461, -0.006511, -0.006611, -0.00652, -0.006517,
-0.006514, -0.006467, -0.006552, -0.006572, -0.00655, -0.006541, -0.006525, -0.006467, -0.006499, -0.006573,
-0.006497, -0.006473, -0.006478, -0.006404, -0.006433, -0.006473, -0.006445, -0.006432, -0.006397, -0.006334,
-0.006356, -0.006422, -0.006334, -0.006311, -0.006307, -0.00623, -0.006261, -0.006293, -0.006268, -0.006244,
-0.006222, -0.006119, -0.00613, -0.006193, -0.006122, -0.006106, -0.00608, -0.005933, -0.005959, -0.006045,
-0.005959, -0.005912, -0.005811, -0.005824, -0.005863, -0.005796, -0.005807, -0.005752, -0.005713, -0.005643,
-0.005597, -0.005655, -0.005598, -0.00552, -0.005504, -0.005402, -0.005415, -0.005472, -0.005355, -0.005327,
-0.005297, -0.005212, -0.005237, -0.005256, -0.005182, -0.005117, -0.005054, -0.005, -0.005002, -0.005001,
-0.004957, -0.00489, -0.004849, -0.004777, -0.004762, -0.004789, -0.004715, -0.004638, -0.004612, -0.004519,
-0.004491, -0.004526, -0.004441, -0.004429, -0.004358, -0.004221, -0.004285, -0.004287, -0.004205, -0.004195,
-0.004086, -0.004035, -0.004052, -0.004028, -0.004001, -0.003914, -0.003871, -0.003778, -0.003776, -0.00382,
-0.003713, -0.00366, -0.00363, -0.003519, -0.003533, -0.003569, -0.003438, -0.003371, -0.003353, -0.003355,
-0.003263, -0.003222, -0.003211, -0.003116, -0.003064, -0.003011, -0.003052, -0.002985, -0.002942, -0.002821,
-0.002836, -0.002883, -0.002721, -0.002755, -0.002689, -0.002685, -0.002587, -0.002478, -0.002505, -0.00253,
-0.00251, -0.002343, -0.002316, -0.002279, -0.002301, -0.002246, -0.002193, -0.002182, -0.002064, -0.001986,
-0.001972, -0.002053, -0.001935, -0.001863, -0.001876, -0.001782, -0.001751, -0.001747, -0.001728, -0.001657,
-0.001558, -0.00153, -0.001533, -0.001545, -0.001504, -0.001418, -0.001376, -0.001256, -0.001296, -0.00132,
-0.001212, -0.001177, -0.001128, -0.001071, -0.001054, -0.001063, -0.000978, -0.00098, -0.000895, -0.000831,
-0.00086, -0.000832, -0.000808, -0.000745, -0.000747, -0.000633, -0.000638, -0.000654, -0.000585, -0.000573,
-0.000548, -0.000459, -0.000406, -0.000488, -0.000412, -0.000375, -0.000318, -0.000229, -0.000241, -0.000245,
-0.000184, -0.000154, -0.000138, -0.000052, -0.000065, -0.000078, -0.000038, 0.000023, 0.000013, 0.000128,
0.000113, 0.000055, 0.000139, 0.000129, 0.00022, 0.000274, 0.000271, 0.000206, 0.000306, 0.000353,
0.000365, 0.000459, 0.000409, 0.000423, 0.000446, 0.00051, 0.000503, 0.000591, 0.000605, 0.000545,
0.000584, 0.000621, 0.000719, 0.000693, 0.000649, 0.000681, 0.000736, 0.000733, 0.000807, 0.000845,
0.000809, 0.000824, 0.000857, 0.000894, 0.000905, 0.000943, 0.000952, 0.000954, 0.000936, 0.00098,
0.001035, 0.001127, 0.001086, 0.001, 0.001073, 0.001086, 0.001139, 0.001196, 0.001202, 0.001107,
0.001175, 0.001231, 0.001201, 0.001337, 0.001303, 0.001225, 0.001272, 0.001317, 0.001334, 0.001428,
0.0014, 0.001347, 0.001397, 0.001342, 0.001403, 0.001501, 0.001475, 0.001424, 0.001476, 0.001499,
0.001447, 0.00154, 0.001563, 0.001462, 0.001482, 0.001522, 0.001529, 0.001606, 0.001558, 0.001501,
0.001582, 0.001599, 0.001602, 0.001627, 0.001626, 0.001581, 0.001558, 0.001581, 0.001591, 0.001664,
0.001678, 0.001552, 0.001611, 0.001701, 0.001637, 0.001716, 0.001722, 0.001623, 0.001616, 0.001658,
0.001697, 0.001732, 0.001713, 0.001629, 0.001695, 0.00168, 0.001681, 0.001748, 0.00172, 0.001664,
0.001595, 0.001678, 0.001724, 0.001732, 0.001696, 0.001671, 0.00167, 0.001661, 0.001716, 0.001692,
0.001687, 0.001653, 0.001661, 0.001678, 0.001658, 0.001701, 0.001697, 0.001655, 0.001645, 0.001657,
0.001672, 0.001701, 0.001676, 0.001632, 0.001658, 0.001658, 0.00165, 0.001731, 0.001668, 0.001588,
0.001582, 0.001605, 0.001641, 0.001627, 0.001664, 0.001562, 0.001546, 0.001569, 0.001587, 0.001655,
0.001569, 0.001529, 0.001529, 0.001568, 0.001602, 0.001636, 0.001572, 0.00149, 0.001542, 0.001507,
0.001554, 0.001594, 0.001529, 0.001506, 0.001444, 0.001485, 0.001524, 0.001502, 0.00152, 0.001452,
0.001472, 0.001473, 0.001462, 0.001511, 0.001464, 0.001416, 0.001387, 0.001397, 0.00142, 0.001448,
0.00139, 0.001315, 0.001354, 0.001383, 0.001361, 0.001381, 0.001347, 0.001284, 0.001314, 0.0013,
0.001333, 0.00134, 0.001315, 0.001261, 0.001231, 0.001256, 0.001252, 0.001309, 0.001236, 0.00118,
0.001199, 0.00121, 0.001271, 0.001234, 0.001101, 0.001133, 0.001201, 0.001172, 0.001163, 0.00115,
0.001161, 0.001064, 0.001057, 0.001112, 0.001143, 0.001153, 0.001098, 0.001026, 0.000999, 0.001072,
0.001013, 0.001095, 0.001046, 0.000965, 0.001018, 0.000935, 0.000975, 0.001027, 0.000993, 0.000856,
0.00091, 0.000933, 0.00092, 0.00096, 0.000872, 0.000844, 0.000804, 0.000853, 0.00084, 0.000856,
0.000818, 0.000747, 0.000767, 0.000703, 0.000726, 0.000769, 0.000738, 0.000634, 0.00063, 0.000653,
0.000638, 0.000691, 0.000641, 0.000532, 0.000567, 0.00059, 0.000577, 0.000592, 0.000541, 0.000507,
0.00051, 0.000518, 0.000493, 0.000526, 0.00048, 0.000413, 0.000453, 0.000435, 0.000433, 0.000465,
0.000439, 0.000353, 0.000369, 0.00035, 0.000377, 0.000405, 0.000344, 0.00032, 0.00031, 0.000342,
0.00029, 0.00033, 0.000312, 0.000
    };

    //R1.00 5153 high
    float TIR4B[1024] = { 
        0.001952, 0.00044, -0.00419, -0.013478, -0.026562, -0.040161, -0.051545, -0.054742, -0.048073, -0.033551,
-0.003945, 0.074545, 0.24064, 0.456451, 0.623613, 0.693654, 0.649818, 0.481064, 0.217267, -0.070214,
-0.310963, -0.4739, -0.537353, -0.502828, -0.402354, -0.272922, -0.144871, -0.035656, 0.038863, 0.078755,
0.088113, 0.072915, 0.044264, 0.015278, -0.001491, -0.005118, 0.006651, 0.027509, 0.048783, 0.062511,
0.062312, 0.047573, 0.018201, -0.018815, -0.056882, -0.087398, -0.105825, -0.10875, -0.096263, -0.072106,
-0.039492, -0.006349, 0.023988, 0.046074, 0.058703, 0.061678, 0.0563, 0.04649, 0.033486, 0.020712,
0.00812, -0.003064, -0.013385, -0.023024, -0.03203, -0.039994, -0.046148, -0.049969, -0.050171, -0.046977,
-0.039367, -0.029104, -0.01632, -0.003739, 0.007285, 0.015641, 0.018995, 0.018839, 0.015006, 0.008928,
0.00199, -0.004441, -0.009822, -0.013383, -0.015805, -0.017064, -0.017416, -0.017676, -0.017412, -0.017576,
-0.017381, -0.017762, -0.018187, -0.018908, -0.019764, -0.02085, -0.022649, -0.024425, -0.026768, -0.028939,
-0.031658, -0.034315, -0.036906, -0.039311, -0.041305, -0.042655, -0.043297, -0.043334, -0.042211, -0.040553,
-0.037707, -0.03455, -0.030805, -0.026833, -0.023475, -0.02037, -0.017942, -0.016193, -0.014818, -0.013681,
-0.013106, -0.012688, -0.012455, -0.012289, -0.012348, -0.012313, -0.0123, -0.012231, -0.012056, -0.011817,
-0.011256, -0.010875, -0.010397, -0.010211, -0.010168, -0.010732, -0.011651, -0.012909, -0.014463, -0.016,
-0.017475, -0.01842, -0.019491, -0.019617, -0.01941, -0.018693, -0.017286, -0.015997, -0.014268, -0.012746,
-0.011329, -0.009877, -0.008605, -0.007714, -0.006831, -0.00606, -0.005592, -0.005397, -0.005479, -0.005407,
-0.005597, -0.006058, -0.006113, -0.006372, -0.006844, -0.00676, -0.006643, -0.00628, -0.006132, -0.005848,
-0.005324, -0.005243, -0.005275, -0.005583, -0.006051, -0.007054, -0.007997, -0.008958, -0.010039, -0.010946,
-0.011838, -0.012685, -0.013505, -0.013907, -0.014285, -0.014345, -0.013949, -0.013345, -0.012561, -0.011279,
-0.009755, -0.007901, -0.006074, -0.0045, -0.002337, -0.000728, 0.001158, 0.003117, 0.004609, 0.006442,
0.008011, 0.00956, 0.010823, 0.011857, 0.012783, 0.01325, 0.013499, 0.013709, 0.013441, 0.013114,
0.012826, 0.01246, 0.012166, 0.01143, 0.010798, 0.009978, 0.008772, 0.007692, 0.006615, 0.005452,
0.004595, 0.003863, 0.003353, 0.003202, 0.002834, 0.002787, 0.00285, 0.002997, 0.003272, 0.003482,
0.003801, 0.004467, 0.00525, 0.005923, 0.006947, 0.007662, 0.008359, 0.009063, 0.009581, 0.010016,
0.010229, 0.01048, 0.010587, 0.010818, 0.010961, 0.010957, 0.011128, 0.011032, 0.01045, 0.010022,
0.009435, 0.008635, 0.008033, 0.007297, 0.006795, 0.006451, 0.006187, 0.006041, 0.005866, 0.005705,
0.005595, 0.005487, 0.005635, 0.005941, 0.006175, 0.006668, 0.007344, 0.008171, 0.009044, 0.009895,
0.01048, 0.01117, 0.011716, 0.012152, 0.012832, 0.013226, 0.013819, 0.014284, 0.014947, 0.015493,
0.015778, 0.016265, 0.016562, 0.016838, 0.016642, 0.016515, 0.016336, 0.016125, 0.016117, 0.015927,
0.015703, 0.014958, 0.013864, 0.012565, 0.01098, 0.009527, 0.008289, 0.007387, 0.007207, 0.007383,
0.008041, 0.00901, 0.00959, 0.009989, 0.010386, 0.010702, 0.010703, 0.010769, 0.010666, 0.010522,
0.010616, 0.01076, 0.011036, 0.011149, 0.011358, 0.011427, 0.01121, 0.011019, 0.010903, 0.010835,
0.010731, 0.010542, 0.010574, 0.010793, 0.010818, 0.010974, 0.011141, 0.01102, 0.010829, 0.010701,
0.010426, 0.01032, 0.010075, 0.009876, 0.009994, 0.009715, 0.009697, 0.00979, 0.009548, 0.009175,
0.009077, 0.00911, 0.008979, 0.009276, 0.009595, 0.009683, 0.009951, 0.010103, 0.010135, 0.010387,
0.010277, 0.010113, 0.010119, 0.010084, 0.010177, 0.010231, 0.010212, 0.010101, 0.010094, 0.009771,
0.009544, 0.009452, 0.009049, 0.008823, 0.008607, 0.00857, 0.008575, 0.00847, 0.008379, 0.008189,
0.007964, 0.007541, 0.007227, 0.006979, 0.00652, 0.006303, 0.006268, 0.006314, 0.00625, 0.006311,
0.006478, 0.006262, 0.006081, 0.005905, 0.005827, 0.005817, 0.005922, 0.005953, 0.005903, 0.006132,
0.006225, 0.006271, 0.006343, 0.006185, 0.005925, 0.005901, 0.005747, 0.005782, 0.005811, 0.005767,
0.005869, 0.005771, 0.005915, 0.005775, 0.005549, 0.005368, 0.005203, 0.005037, 0.00488, 0.004916,
0.004773, 0.004679, 0.004598, 0.004649, 0.004549, 0.004441, 0.004375, 0.004175, 0.004059, 0.003982,
0.003912, 0.003885, 0.003871, 0.003618, 0.0035, 0.003424, 0.003327, 0.003273, 0.003171, 0.002979,
0.002853, 0.00274, 0.002594, 0.00262, 0.002491, 0.002407, 0.00225, 0.002197, 0.002186, 0.002124,
0.002002, 0.001781, 0.001736, 0.001553, 0.001519, 0.001505, 0.001392, 0.001263, 0.001184, 0.001145,
0.001039, 0.00104, 0.000972, 0.000841, 0.000748, 0.000645, 0.000593, 0.000593, 0.000443, 0.000355,
0.000285, 0.000137, 0.000066, 0.000126, 0.000001, -0.000182, -0.000246, -0.000403, -0.00041, -0.00047,
-0.000536, -0.000693, -0.000754, -0.000813, -0.000896, -0.000843, -0.000971, -0.00113, -0.001274, -0.001208,
-0.001349, -0.001342, -0.001399, -0.00162, -0.001619, -0.001789, -0.001722, -0.001777, -0.001855, -0.001966,
-0.002097, -0.002163, -0.00226, -0.002194, -0.002297, -0.00242, -0.002529, -0.002597, -0.002659, -0.002624,
-0.002696, -0.002898, -0.002857, -0.002931, -0.002986, -0.002979, -0.003056, -0.003141, -0.003255, -0.003251,
-0.00324, -0.003301, -0.003354, -0.0035, -0.003603, -0.003618, -0.003657, -0.003635, -0.003675, -0.003831,
-0.003895, -0.0039, -0.003963, -0.003937, -0.003957, -0.004105, -0.004141, -0.004196, -0.004203, -0.004129,
-0.004242, -0.004347, -0.004375, -0.004418, -0.004428, -0.004378, -0.004444, -0.004608, -0.004575, -0.004596,
-0.004619, -0.004588, -0.004709, -0.004795, -0.004828, -0.004856, -0.004863, -0.00481, -0.00485, -0.00497,
-0.004938, -0.004931, -0.004957, -0.004898, -0.004947, -0.005041, -0.005075, -0.005111, -0.005113, -0.005065,
-0.005104, -0.005224, -0.005183, -0.005178, -0.005197, -0.005132, -0.005183, -0.005266, -0.0053, -0.00532,
-0.005331, -0.005232, -0.005241, -0.005342, -0.005317, -0.00533, -0.005332, -0.005182, -0.005202, -0.005344,
-0.005327, -0.005318, -0.00523, -0.005258, -0.00535, -0.005339, -0.005398, -0.005383, -0.005367, -0.005301,
-0.00525, -0.005335, -0.005322, -0.005264, -0.005266, -0.005174, -0.005205, -0.005325, -0.005265, -0.005266,
-0.005264, -0.005194, -0.005239, -0.005309, -0.005276, -0.005224, -0.005156, -0.005094, -0.005107, -0.005144,
-0.005146, -0.005109, -0.005089, -0.005029, -0.00503, -0.0051, -0.00507, -0.005009, -0.004992, -0.004902,
-0.004875, -0.004944, -0.004903, -0.004924, -0.004882, -0.004739, -0.004808, -0.004863, -0.004824, -0.004843,
-0.004745, -0.004683, -0.004709, -0.004715, -0.00472, -0.004648, -0.004603, -0.0045, -0.004495, -0.004579,
-0.004513, -0.004472, -0.004455, -0.004342, -0.004359, -0.00444, -0.004341, -0.004271, -0.004254, -0.00428,
-0.004209, -0.004173, -0.00418, -0.004096, -0.004041, -0.00399, -0.004059, -0.004035, -0.004019, -0.003897,
-0.003908, -0.003993, -0.00385, -0.003884, -0.003829, -0.003833, -0.003734, -0.003593, -0.003604, -0.003662,
-0.003693, -0.003535, -0.003487, -0.003445, -0.003487, -0.003464, -0.003428, -0.003432, -0.003311, -0.003202,
-0.003171, -0.003287, -0.003211, -0.003142, -0.003163, -0.003076, -0.00304, -0.003046, -0.003048, -0.002989,
-0.002872, -0.002821, -0.002827, -0.002871, -0.002864, -0.002787, -0.002733, -0.002581, -0.002602, -0.002653,
-0.002565, -0.002528, -0.002478, -0.002414, -0.002398, -0.00243, -0.002361, -0.002372, -0.00229, -0.002207,
-0.002233, -0.002224, -0.002215, -0.002156, -0.002157, -0.002026, -0.002008, -0.00203, -0.001968, -0.001955,
-0.001935, -0.001834, -0.001752, -0.00184, -0.001788, -0.001752, -0.001686, -0.001568, -0.001559, -0.001576,
-0.001529, -0.001505, -0.001499, -0.00141, -0.001419, -0.001451, -0.001428, -0.001363, -0.001363, -0.001224,
-0.001207, -0.001275, -0.001203, -0.001212, -0.001106, -0.001017, -0.000996, -0.001079, -0.000996, -0.000933,
-0.000909, -0.000794, -0.000835, -0.000843, -0.000839, -0.000777, -0.00078, -0.000678, -0.000638, -0.000705,
-0.000686, -0.00065, -0.000524, -0.000521, -0.000573, -0.000559, -0.000499, -0.000488, -0.000391, -0.000316,
-0.000337, -0.00033, -0.000302, -0.00026, -0.000243, -0.000198, -0.000182, -0.000181, -0.000213, -0.000176,
-0.000103, 0.000034, 0.000028, -0.00008, -0.000034, -0.000023, 0.000043, 0.000134, 0.000171, 0.000065,
0.00011, 0.000179, 0.000159, 0.000325, 0.000327, 0.000234, 0.000252, 0.000296, 0.000325, 0.00045,
0.000451, 0.000387, 0.000419, 0.000345, 0.000395, 0.000526, 0.000542, 0.000495, 0.000545, 0.000581,
0.000527, 0.000626, 0.00068, 0.000575, 0.000569, 0.000608, 0.000632, 0.000746, 0.000731, 0.000668,
0.000751, 0.000794, 0.000818, 0.00086, 0.00087, 0.000815, 0.000764, 0.00077, 0.000785, 0.000891,
0.000956, 0.000837, 0.000886, 0.00101, 0.000976, 0.001076, 0.00111, 0.000997, 0.000947, 0.000968,
0.001018, 0.001085, 0.001095, 0.001005, 0.001064, 0.001061, 0.001074, 0.00117, 0.001172, 0.001116,
0.001015, 0.001088, 0.00117, 0.001223, 0.001212, 0.001189, 0.001182, 0.00117, 0.001237, 0.001233,
0.001236, 0.001202, 0.001206, 0.001233, 0.001225, 0.00129, 0.001314, 0.00128, 0.001264, 0.001276,
0.001302, 0.001353, 0.001347, 0.001298, 0.001317, 0.001323, 0.001321, 0.001428, 0.001389, 0.001288,
0.001249, 0.001263, 0.001322, 0.001342, 0.001416, 0.001329, 0.001296, 0.001319, 0.001356, 0.001465,
0.001406, 0.001352, 0.001335, 0.001378, 0.001439, 0.00151, 0.001462, 0.001349, 0.001372, 0.001327,
0.00138, 0.001454, 0.001411, 0.001384, 0.001302, 0.001334, 0.001398, 0.001407, 0.001452, 0.001392,
0.001408, 0.001414, 0.001407, 0.001471, 0.001434, 0.001371, 0.001314, 0.001309, 0.001343, 0.001404,
0.00137, 0.001285, 0.001319, 0.001373, 0.001379, 0.001425, 0.001406, 0.001331, 0.001347, 0.001333,
0.001378, 0.001408, 0.001397, 0.001333, 0.001278, 0.001292, 0.001297, 0.001382, 0.001331, 0.001263,
0.001271, 0.001291, 0.001386, 0.001381, 0.001228, 0.001225, 0.001303, 0.001298, 0.001303, 0.001298,
0.001316, 0.001206, 0.001169, 0.001226, 0.001292, 0.001343, 0.001305, 0.00121, 0.001145, 0.00121,
0.00116, 0.001263, 0.001244, 0.001153, 0.001198, 0.001109, 0.001143, 0.001224, 0.00122, 0.001065,
0.00109, 0.001125, 0.001133, 0.001201, 0.001125, 0.001077, 0.001014, 0.001058, 0.001063, 0.001104,
0.001083, 0.001004, 0.001011, 0.000939, 0.000959, 0.001031, 0.001034, 0.000928, 0.000903, 0.000929,
0.000932, 0.001019, 0.000999, 0.000874, 0.000885, 0.000915, 0.000918, 0.000953, 0.00091, 0.000862,
0.000854, 0.000864, 0.000842, 0.000887, 0.000851, 0.000767, 0.000795, 0.000784, 0.000791, 0.000844,
0.000838, 0.000739, 0.000732, 0.000703, 0.000734, 0.000787, 0.000739, 0.000706, 0.000685, 0.00072,
0.00067, 0.000713, 0.000706, 0.000
};

//R1.00 Fender LOW
float TIR5[1024] = {
0.002328, 0.001334, -0.000712, -0.005795, -0.023703, -0.033623, -0.035255, -0.051013, -0.032332, -0.036968,
-0.033426, 0.036154, 0.138424, 0.380026, 0.375794, 0.456968, 0.468444, 0.229873, 0.298745, 0.074556,
0.166073, 0.117211, 0.166782, 0.179468, 0.099603, 0.124415, -0.03794, 0.035356, -0.109487, 0.018309,
-0.08768, 0.033761, -0.060656, 0.010612, -0.070566, -0.040347, -0.084321, -0.075967, -0.078273, -0.091578,
-0.067558, -0.105894, -0.066023, -0.121055, -0.066123, -0.126507, -0.059413, -0.120766, -0.049222, -0.11123,
-0.042139, -0.102909, -0.039465, -0.096691, -0.040526, -0.092777, -0.045229, -0.091504, -0.052535, -0.091379,
-0.059787, -0.089826, -0.064745, -0.086048, -0.067379, -0.080825, -0.068405, -0.075234, -0.068513, -0.069308,
-0.067855, -0.063362, -0.066329, -0.057254, -0.064369, -0.051697, -0.062106, -0.046534, -0.059743, -0.042158,
-0.057632, -0.038291, -0.055815, -0.035057, -0.053702, -0.032266, -0.051821, -0.030109, -0.050058, -0.028271,
-0.048317, -0.026675, -0.046552, -0.025833, -0.04517, -0.025124, -0.043863, -0.02495, -0.04274, -0.024898,
-0.041732, -0.024881, -0.040722, -0.024836, -0.039353, -0.024337, -0.037479, -0.0234, -0.035274, -0.021944,
-0.032461, -0.019975, -0.029286, -0.017937, -0.025865, -0.015664, -0.022413, -0.01329, -0.018872, -0.011171,
-0.015786, -0.009075, -0.013042, -0.007576, -0.010695, -0.006454, -0.00881, -0.005832, -0.007444, -0.005419,
-0.006423, -0.005141, -0.005515, -0.004944, -0.004613, -0.004531, -0.003418, -0.003992, -0.00219, -0.00302,
-0.00056, -0.001731, 0.001535, 0.000011, 0.003725, 0.001919, 0.006192, 0.004163, 0.008784, 0.006181,
0.011277, 0.008042, 0.01344, 0.009933, 0.015244, 0.011366, 0.016602, 0.012467, 0.017588, 0.013048,
0.018074, 0.013313, 0.018253, 0.013264, 0.018058, 0.013017, 0.017778, 0.01251, 0.017225, 0.012044,
0.016658, 0.011521, 0.01635, 0.011132, 0.015885, 0.011123, 0.01566, 0.011136, 0.015712, 0.011271,
0.016078, 0.01172, 0.016501, 0.01264, 0.017193, 0.013434, 0.018033, 0.014734, 0.018884, 0.015903,
0.020261, 0.017239, 0.021256, 0.018653, 0.022454, 0.019969, 0.023678, 0.02109, 0.024535, 0.022161,
0.02533, 0.023037, 0.026052, 0.023593, 0.026243, 0.024018, 0.026383, 0.02423, 0.025984, 0.024134,
0.025541, 0.023866, 0.024899, 0.023178, 0.024083, 0.022199, 0.023098, 0.021338, 0.021956, 0.020316,
0.02089, 0.019371, 0.019664, 0.018385, 0.018487, 0.017398, 0.017236, 0.016515, 0.016265, 0.01562,
0.015344, 0.014941, 0.014596, 0.014119, 0.013717, 0.013526, 0.012952, 0.012913, 0.012462, 0.012465,
0.011935, 0.011973, 0.011523, 0.0118, 0.011051, 0.011525, 0.010835, 0.011219, 0.010539, 0.011334,
0.010566, 0.011422, 0.010689, 0.011656, 0.010797, 0.011866, 0.011021, 0.012192, 0.011358, 0.012498,
0.01174, 0.012922, 0.012196, 0.013409, 0.012582, 0.013799, 0.012967, 0.013942, 0.013317, 0.014353,
0.013201, 0.014297, 0.013138, 0.014163, 0.013051, 0.014065, 0.012846, 0.013851, 0.012765, 0.01363,
0.012546, 0.013422, 0.012215, 0.013206, 0.012089, 0.012993, 0.011933, 0.012733, 0.011711, 0.01243,
0.01149, 0.012099, 0.011228, 0.011995, 0.011031, 0.011731, 0.0107, 0.011405, 0.010498, 0.011077,
0.01024, 0.010801, 0.01, 0.010492, 0.00984, 0.010234, 0.009458, 0.009964, 0.009186, 0.009733,
0.008795, 0.009436, 0.008576, 0.009033, 0.008352, 0.008623, 0.008122, 0.008526, 0.007802, 0.008302,
0.007586, 0.007825, 0.007418, 0.007561, 0.007062, 0.007374, 0.006772, 0.007084, 0.006525, 0.00668,
0.00631, 0.006371, 0.006143, 0.006196, 0.005943, 0.005868, 0.005552, 0.005695, 0.005336, 0.005507,
0.005123, 0.005288, 0.004998, 0.00504, 0.004789, 0.004709, 0.004747, 0.004356, 0.004558, 0.004326,
0.004351, 0.00405, 0.004128, 0.003796, 0.003847, 0.003577, 0.003648, 0.003345, 0.003267, 0.003197,
0.003049, 0.003007, 0.002842, 0.002849, 0.002722, 0.002538, 0.002589, 0.002257, 0.002408, 0.002112,
0.002235, 0.002019, 0.002056, 0.001763, 0.001893, 0.001592, 0.001672, 0.001525, 0.00155, 0.001301,
0.001368, 0.001177, 0.001321, 0.001044, 0.001066, 0.000812, 0.000958, 0.000712, 0.000777, 0.000538,
0.000605, 0.00032, 0.000404, 0.000165, 0.000225, -0.000067, 0.000009, -0.000327, -0.00028, -0.000555,
-0.000487, -0.00084, -0.00081, -0.001099, -0.001045, -0.001316, -0.001333, -0.001587, -0.001549, -0.001817,
-0.001869, -0.002048, -0.002109, -0.002336, -0.002333, -0.002533, -0.002554, -0.002757, -0.002775, -0.002972,
-0.002984, -0.003176, -0.00323, -0.0034, -0.003448, -0.00362, -0.003581, -0.003748, -0.003825, -0.003953,
-0.003923, -0.004128, -0.004173, -0.004285, -0.004293, -0.004474, -0.004468, -0.004555, -0.004586, -0.004769,
-0.004804, -0.00491, -0.004914, -0.005001, -0.005031, -0.005211, -0.005243, -0.005301, -0.005322, -0.005409,
-0.005434, -0.005471, -0.005508, -0.005631, -0.005621, -0.005684, -0.005781, -0.005858, -0.005842, -0.005868,
-0.005904, -0.005986, -0.005998, -0.006068, -0.006074, -0.006094, -0.006208, -0.006237, -0.006277, -0.006323,
-0.006282, -0.006327, -0.006404, -0.006363, -0.006398, -0.006418, -0.006476, -0.006467, -0.006517, -0.006552,
-0.006586, -0.006532, -0.006564, -0.006539, -0.006573, -0.006586, -0.006591, -0.006638, -0.006666, -0.0066,
-0.006711, -0.006684, -0.006641, -0.006625, -0.00675, -0.006632, -0.006666, -0.00663, -0.006695, -0.00663,
-0.006731, -0.006737, -0.006708, -0.006661, -0.006687, -0.006699, -0.006659, -0.006635, -0.006657, -0.006657,
-0.006603, -0.006612, -0.006651, -0.00653, -0.006509, -0.006525, -0.006562, -0.006528, -0.00651, -0.006499,
-0.006538, -0.006423, -0.006455, -0.006414, -0.006474, -0.006313, -0.006356, -0.006355, -0.006339, -0.006229,
-0.006249, -0.006238, -0.006219, -0.006152, -0.0062, -0.006168, -0.006103, -0.006065, -0.006078, -0.006025,
-0.006031, -0.005974, -0.005977, -0.005918, -0.00592, -0.00586, -0.005861, -0.005801, -0.005802, -0.005696,
-0.005669, -0.005629, -0.005628, -0.005604, -0.005576, -0.00555, -0.005481, -0.005436, -0.005441, -0.005381,
-0.005336, -0.005247, -0.005222, -0.005223, -0.005152, -0.005139, -0.005066, -0.005056, -0.005032, -0.00496,
-0.00491, -0.004912, -0.004803, -0.004727, -0.004748, -0.004682, -0.004623, -0.004565, -0.004601, -0.004552,
-0.004471, -0.004373, -0.004392, -0.004331, -0.004279, -0.004229, -0.004179, -0.004172, -0.0041, -0.004134,
-0.00409, -0.00397, -0.003941, -0.003902, -0.003858, -0.00381, -0.003759, -0.003751, -0.003633, -0.003639,
-0.003614, -0.003537, -0.003435, -0.003448, -0.003427, -0.003352, -0.003296, -0.003245, -0.003241, -0.003217,
-0.003098, -0.003064, -0.00302, -0.003061, -0.002925, -0.002884, -0.002791, -0.002758, -0.002707, -0.002648,
-0.002632, -0.002551, -0.002487, -0.002474, -0.002444, -0.002366, -0.002265, -0.002188, -0.002206, -0.002136,
-0.002119, -0.002087, -0.002007, -0.00195, -0.001945, -0.001877, -0.001824, -0.001776, -0.001773, -0.001613,
-0.001642, -0.001623, -0.001545, -0.001485, -0.001476, -0.001542, -0.001385, -0.001438, -0.001271, -0.00131,
-0.001213, -0.001231, -0.001169, -0.001115, -0.001109, -0.000992, -0.001047, -0.001007, -0.000884, -0.000896,
-0.000835, -0.000785, -0.000783, -0.000717, -0.000664, -0.000616, -0.000659, -0.000617, -0.000497, -0.000421,
-0.000492, -0.000411, -0.000401, -0.000333, -0.000328, -0.000264, -0.000261, -0.000244, -0.000178, -0.000132,
-0.000137, -0.000122, -0.000057, -0.000011, -0.000016, -0.000001, 0.000019, 0.000037, 0.000145, 0.000121,
0.000128, 0.000144, 0.000207, 0.000295, 0.000269, 0.000234, 0.000319, 0.00037, 0.000365, 0.000424,
0.000377, 0.000502, 0.000485, 0.00054, 0.000538, 0.000645, 0.000672, 0.000667, 0.000688, 0.000761,
0.000676, 0.000733, 0.000804, 0.000845, 0.00083, 0.000836, 0.000893, 0.000932, 0.000968, 0.001005,
0.001001, 0.001018, 0.00095, 0.0011, 0.00104, 0.001108, 0.001101, 0.001152, 0.001095, 0.001211,
0.001233, 0.001263, 0.001163, 0.001215, 0.00124, 0.001299, 0.001243, 0.001264, 0.001273, 0.001277,
0.001279, 0.001371, 0.001289, 0.00135, 0.001341, 0.001345, 0.001354, 0.001362, 0.00137, 0.001468,
0.001345, 0.001383, 0.001397, 0.001403, 0.001451, 0.001387, 0.001403, 0.001409, 0.001456, 0.001528,
0.001488, 0.001487, 0.001498, 0.001558, 0.001553, 0.001517, 0.001549, 0.00161, 0.001644, 0.001628,
0.001587, 0.001616, 0.001629, 0.001681, 0.001666, 0.001667, 0.001672, 0.001587, 0.001675, 0.001665,
0.00166, 0.001703, 0.001682, 0.001678, 0.001727, 0.001667, 0.001733, 0.001678, 0.001654, 0.001635,
0.001749, 0.001711, 0.001653, 0.001714, 0.001698, 0.001694, 0.001741, 0.001678, 0.001697, 0.001613,
0.001699, 0.001685, 0.001677, 0.001625, 0.001684, 0.001618, 0.001674, 0.001654, 0.001647, 0.00169,
0.001671, 0.001669, 0.001628, 0.001652, 0.001705, 0.001642, 0.001612, 0.001634, 0.001636, 0.001631,
0.001669, 0.001553, 0.001537, 0.001558, 0.001508, 0.001515, 0.00155, 0.001561, 0.001571, 0.001541,
0.001536, 0.001495, 0.001475, 0.001457, 0.001526, 0.001506, 0.001494, 0.001487, 0.001528, 0.001505,
0.001455, 0.001429, 0.001408, 0.001428, 0.001425, 0.001459, 0.001429, 0.00137, 0.001429, 0.00141,
0.001357, 0.001374, 0.001375, 0.001235, 0.001286, 0.001288, 0.001228, 0.001228, 0.001213, 0.001194,
0.001222, 0.001189, 0.00122, 0.001241, 0.001126, 0.001163, 0.001175, 0.001177, 0.001176, 0.001082,
0.001117, 0.001119, 0.00111, 0.001098, 0.001042, 0.001051, 0.001091, 0.001016, 0.001023, 0.000974,
9.860001E-04, 0.001027, 0.000954, 0.000961, 0.000958, 0.000905, 0.000964, 0.000942, 0.00084, 0.000872,
0.000873, 0.000864, 0.000852, 0.000887, 0.000906, 0.000831, 0.000845, 0.000805, 0.00078, 0.000802,
0.000846, 0.000819, 0.000761, 0.00082, 0.000801, 0.000793, 0.000745, 0.000763, 0.00072, 0.00069,
0.000705, 0.000745, 0.000714, 0.000653, 0.000709, 0.000733, 0.000705, 0.000654, 0.000674, 0.000635,
0.000609, 0.000583, 0.000644, 0.000617, 0.000555, 0.000653, 0.000564, 0.000614, 0.000594, 0.000635,
0.000527, 0.000519, 0.000502, 0.000569, 0.000545, 0.000485, 0.000494, 0.000535, 0.000461, 0.000514,
0.000402, 0.000477, 0.000415, 0.000424, 0.000329, 0.000404, 0.000336, 0.000429, 0.000381, 0.000364,
0.000313, 0.000376, 0.000315, 0.000283, 0.000258, 0.000322, 0.000252, 0.000257, 0.000249, 0.000285,
0.000256, 0.000245, 0.000242, 0.000241, 0.00015, 0.000187, 0.000192, 0.000185, 0.000129, 0.000184,
0.000205, 0.000128, 0.000138, 0.000184, 0.000119, 0.000087, 0.000109, 0.000111, 0.00006, 0.000117,
0.000139, 0.000109, 0.000101, 0.000055, 0.000076, 0.000081, 0.000035, 0.000005, -0.000023, 0.000037,
0.000055, 0.000019, -0.00004, 0.000019, 0, -0.000006, -0.000007, -0.000098, -0.000014, -0.000028,
-0.000036, -0.000087, -0.000119, -0.000056, 0.000013, -0.000128, -0.000148, -0.000124, -0.000077, -0.000148,
-0.00014, -0.000188, -0.00013, -0.000107, -0.000091, -0.000163, -0.000146, -0.000184, -0.000115, -0.000174,
-0.000205, -0.000184, -0.000139, -0.000
};

//R1.00 Fender HIGH
float TIR5B[1024] = {
0.00293, 0.002535, -0.000211, -0.007796, -0.032948, -0.05272, -0.05902, -0.075568, -0.048376, -0.035881,
-0.015858, 0.086315, 0.250318, 0.602168, 0.68315, 0.762627, 0.704943, 0.268151, 0.096277, -0.350952,
-0.419266, -0.501056, -0.39328, -0.244732, -0.186405, -0.036764, -0.122673, -0.00267, -0.12169, 0.029937,
-0.046063, 0.103123, 0.032886, 0.107673, 0.018225, 0.022412, -0.044894, -0.058886, -0.066555, -0.077033,
-0.036679, -0.057839, 0.001887, -0.038841, 0.028373, -0.024734, 0.047892, -0.010893, 0.061211, -0.002753,
0.061426, -0.006023, 0.048329, -0.017865, 0.028426, -0.031725, 0.008057, -0.043087, -0.008464, -0.048479,
-0.017523, -0.045231, -0.017837, -0.034881, -0.013146, -0.022948, -0.009064, -0.014762, -0.009269, -0.011529,
-0.013314, -0.011752, -0.018545, -0.012437, -0.022915, -0.012703, -0.025762, -0.012078, -0.027408, -0.011524,
-0.029041, -0.011459, -0.031089, -0.012209, -0.032816, -0.013129, -0.034461, -0.014378, -0.035879, -0.015485,
-0.036771, -0.016247, -0.03707, -0.0174, -0.037657, -0.018579, -0.038096, -0.020091, -0.038546, -0.021445,
-0.038747, -0.022412, -0.038497, -0.022926, -0.037445, -0.0225, -0.035442, -0.021338, -0.033059, -0.019821,
-0.03034, -0.018156, -0.027754, -0.017099, -0.02568, -0.016473, -0.024142, -0.016192, -0.02278, -0.016343,
-0.02208, -0.016614, -0.021657, -0.017419, -0.021537, -0.018381, -0.021563, -0.019483, -0.021726, -0.020317,
-0.021665, -0.020679, -0.021085, -0.020535, -0.020007, -0.019726, -0.018228, -0.018526, -0.016386, -0.016975,
-0.014227, -0.015249, -0.011785, -0.013261, -0.009557, -0.01156, -0.007511, -0.009892, -0.005653, -0.008838,
-0.004328, -0.008306, -0.003633, -0.007871, -0.003255, -0.007833, -0.003237, -0.007951, -0.003308, -0.008276,
-0.003574, -0.008557, -0.003705, -0.008677, -0.003767, -0.008579, -0.003475, -0.008354, -0.003201, -0.007867,
-0.002703, -0.00725, -0.001754, -0.006297, -0.000914, -0.004983, 0.000187, -0.003726, 0.001412, -0.002526,
0.002762, -0.001158, 0.003978, 0.000503, 0.005344, 0.001785, 0.006499, 0.003346, 0.007503, 0.004496,
0.008839, 0.005771, 0.009604, 0.006833, 0.010416, 0.007711, 0.01118, 0.00832, 0.011422, 0.008752,
0.011619, 0.009084, 0.011895, 0.009251, 0.011675, 0.009317, 0.011585, 0.009432, 0.01113, 0.009329,
0.010813, 0.009335, 0.010584, 0.009122, 0.01032, 0.00875, 0.01002, 0.008737, 0.009851, 0.008772,
0.009948, 0.009089, 0.009998, 0.009361, 0.01006, 0.009581, 0.009957, 0.009821, 0.010154, 0.010095,
0.010378, 0.01056, 0.010769, 0.010773, 0.010767, 0.010981, 0.010758, 0.011068, 0.010985, 0.011362,
0.011166, 0.011501, 0.011327, 0.01191, 0.011383, 0.012057, 0.011552, 0.012075, 0.011495, 0.012469,
0.011885, 0.012916, 0.012327, 0.013414, 0.01259, 0.013644, 0.012736, 0.013831, 0.012905, 0.013929,
0.013054, 0.014125, 0.013294, 0.014404, 0.013437, 0.014487, 0.013467, 0.014185, 0.013331, 0.014193,
0.012778, 0.013602, 0.012209, 0.013049, 0.011838, 0.012838, 0.011651, 0.012727, 0.011778, 0.012777,
0.011818, 0.012807, 0.011668, 0.012727, 0.011706, 0.012697, 0.01173, 0.012597, 0.011635, 0.01239,
0.011501, 0.012148, 0.011334, 0.012218, 0.011384, 0.012193, 0.011235, 0.011992, 0.01116, 0.011797,
0.011031, 0.011668, 0.010959, 0.011532, 0.010994, 0.01149, 0.010769, 0.011331, 0.010612, 0.011233,
0.010336, 0.011027, 0.01025, 0.010759, 0.010164, 0.010492, 0.010079, 0.010624, 0.010003, 0.010594,
0.009956, 0.010188, 0.009807, 0.009978, 0.009487, 0.009845, 0.009289, 0.009648, 0.009149, 0.009329,
0.009017, 0.009131, 0.00899, 0.00914, 0.008974, 0.008932, 0.008596, 0.008747, 0.00841, 0.008617,
0.008271, 0.008471, 0.008234, 0.0083, 0.008061, 0.007953, 0.008018, 0.007606, 0.007805, 0.007624,
0.007683, 0.007379, 0.007444, 0.007089, 0.007108, 0.006826, 0.00691, 0.006626, 0.006537, 0.006502,
0.006401, 0.006414, 0.006299, 0.006358, 0.006283, 0.006092, 0.006141, 0.005771, 0.005893, 0.005587,
0.005707, 0.005514, 0.005563, 0.00525, 0.005365, 0.005054, 0.005112, 0.00498, 0.005024, 0.004766,
0.004814, 0.004615, 0.004775, 0.004503, 0.004483, 0.004168, 0.004282, 0.004027, 0.004078, 0.003826,
0.00388, 0.003573, 0.003635, 0.003392, 0.003454, 0.003153, 0.003221, 0.002868, 0.00289, 0.002609,
0.002695, 0.002356, 0.002389, 0.002119, 0.002212, 0.001995, 0.00202, 0.001799, 0.001879, 0.00165,
0.001608, 0.001443, 0.001399, 0.001176, 0.001196, 0.001028, 0.00104, 0.000865, 0.00087, 0.000691,
0.000692, 0.000508, 0.000449, 0.000269, 0.000213, 0.000036, 0.000097, -0.000034, -0.000103, -0.000234,
-0.00019, -0.000392, -0.000467, -0.000601, -0.000617, -0.000814, -0.000828, -0.000908, -0.000933, -0.001138,
-0.001215, -0.001351, -0.001368, -0.001451, -0.001477, -0.001683, -0.001761, -0.00184, -0.001863, -0.001951,
-0.001981, -0.002017, -0.002059, -0.00222, -0.002256, -0.002355, -0.002508, -0.002653, -0.002675, -0.002704,
-0.002738, -0.002835, -0.002871, -0.002969, -0.003006, -0.003047, -0.003206, -0.003297, -0.003385, -0.003471,
-0.00344, -0.003478, -0.003576, -0.003556, -0.003602, -0.00365, -0.003756, -0.003798, -0.0039, -0.003996,
-0.004087, -0.00406, -0.004099, -0.004083, -0.004131, -0.00418, -0.004227, -0.004332, -0.00443, -0.004407,
-0.004564, -0.004593, -0.004566, -0.004548, -0.004709, -0.004625, -0.004671, -0.004659, -0.004766, -0.00475,
-0.004911, -0.005, -0.005026, -0.004995, -0.005029, -0.005063, -0.005039, -0.005021, -0.005066, -0.005108,
-0.005091, -0.005136, -0.005235, -0.005153, -0.00514, -0.005188, -0.00529, -0.005324, -0.005356, -0.005387,
-0.005473, -0.005379, -0.005413, -0.005387, -0.005481, -0.005335, -0.00538, -0.00542, -0.005457, -0.005375,
-0.005418, -0.005457, -0.005491, -0.005464, -0.005556, -0.00558, -0.005544, -0.005513, -0.005543, -0.005513,
-0.005543, -0.005513, -0.005543, -0.005512, -0.005542, -0.00551, -0.005539, -0.005507, -0.005535, -0.005445,
-0.005421, -0.005399, -0.005436, -0.005467, -0.005493, -0.005514, -0.005475, -0.005439, -0.005463, -0.005426,
-0.005392, -0.005302, -0.005279, -0.005313, -0.005284, -0.005313, -0.005279, -0.005304, -0.005324, -0.005283,
-0.005243, -0.005264, -0.005165, -0.005075, -0.005108, -0.005076, -0.005046, -0.005014, -0.0051, -0.005114,
-0.005066, -0.004964, -0.004986, -0.004944, -0.004905, -0.004866, -0.004829, -0.00485, -0.004808, -0.004882,
-0.004887, -0.004773, -0.004727, -0.004684, -0.004641, -0.0046, -0.00456, -0.004579, -0.004476, -0.004496,
-0.00451, -0.004462, -0.004357, -0.004376, -0.004388, -0.004338, -0.00429, -0.004243, -0.004255, -0.004261,
-0.004149, -0.004102, -0.004057, -0.004128, -0.004014, -0.003967, -0.003862, -0.003824, -0.003785, -0.003746,
-0.003764, -0.003718, -0.003672, -0.003684, -0.00369, -0.003634, -0.003522, -0.003419, -0.003437, -0.003391,
-0.003402, -0.003408, -0.00335, -0.003295, -0.003298, -0.003239, -0.003183, -0.003128, -0.003132, -0.002959,
-0.002974, -0.002981, -0.002925, -0.002871, -0.002875, -0.002989, -0.002856, -0.002906, -0.002716, -0.002717,
-0.002598, -0.002603, -0.002545, -0.002489, -0.002492, -0.002374, -0.002439, -0.002433, -0.002309, -0.002309,
-0.002248, -0.002189, -0.002189, -0.002128, -0.002068, -0.002011, -0.002071, -0.002062, -0.001934, -0.001817,
-0.001881, -0.001818, -0.001815, -0.001751, -0.001747, -0.001681, -0.001676, -0.001668, -0.001598, -0.001533,
-0.001528, -0.001519, -0.00145, -0.001385, -0.00138, -0.001372, -0.001361, -0.001346, -0.001215, -0.001211,
-0.001202, -0.001191, -0.001119, -0.000995, -0.000996, -0.00105, -0.000979, -0.000913, -0.000907, -0.000841,
-0.000894, -0.000764, -0.000761, -0.000697, -0.000693, -0.000572, -0.000518, -0.000523, -0.000523, -0.000461,
-0.000576, -0.000559, -0.000482, -0.00041, -0.000401, -0.00039, -0.000319, -0.000254, -0.000192, -0.000133,
-0.000135, -0.000133, -0.000242, -0.000105, -0.000154, -0.000078, -0.000066, 0.000005, -0.000043, 0.000089,
0.000152, 0.00021, 0.000094, 0.000109, 0.000125, 0.0002, 0.000154, 0.000173, 0.000193, 0.000213,
0.000233, 0.000368, 0.000317, 0.00039, 0.000399, 0.00041, 0.000422, 0.000435, 0.000449, 0.000579,
0.000466, 0.000483, 0.000501, 0.000519, 0.000594, 0.000547, 0.000565, 0.000583, 0.000658, 0.000783,
0.000781, 0.000782, 0.000785, 0.000847, 0.000847, 0.000792, 0.000803, 0.000872, 0.000935, 0.000936,
0.000881, 0.000892, 0.000903, 0.000973, 0.000979, 9.860001E-04, 0.000994, 0.000888, 0.000968, 0.000983,
0.000997, 0.001069, 0.001076, 0.001084, 0.00115, 0.001095, 0.001162, 0.001109, 0.001064, 0.001024,
0.001163, 0.001171, 0.001123, 0.001196, 0.001204, 0.001212, 0.001278, 0.001222, 0.001232, 0.001127,
0.001207, 0.001221, 0.001234, 0.001189, 0.001264, 0.001216, 0.00129, 0.001298, 0.001305, 0.00137,
0.001371, 0.001373, 0.001318, 0.001327, 0.001394, 0.001338, 0.00129, 0.001305, 0.001319, 0.001331,
0.0014, 0.001289, 0.001248, 0.00127, 0.001231, 0.001254, 0.001331, 0.001399, 0.001459, 0.001455,
0.001452, 0.001392, 0.00134, 0.001294, 0.001368, 0.001375, 0.001382, 0.001388, 0.001451, 0.001449,
0.00139, 0.001338, 0.001292, 0.001309, 0.001323, 0.001392, 0.001395, 0.001341, 0.001407, 0.001408,
0.001352, 0.001359, 0.001365, 0.001198, 0.001222, 0.001242, 0.001201, 0.001222, 0.001238, 0.001252,
0.001321, 0.001323, 0.001382, 0.001433, 0.001306, 0.001309, 0.001311, 0.001312, 0.001312, 0.001197,
0.00121, 0.00122, 0.001228, 0.001234, 0.001182, 0.001192, 0.001257, 0.001199, 0.001204, 0.00115,
0.001159, 0.001224, 0.001165, 0.001169, 0.001173, 0.001116, 0.001182, 0.001181, 0.001063, 0.001074,
0.001083, 0.001089, 0.001093, 0.001154, 0.001207, 0.001136, 0.001132, 0.00107, 0.001015, 0.001023,
0.001086, 0.001083, 0.001022, 0.001082, 0.001078, 0.001072, 0.00101, 0.001011, 0.000955, 0.000903,
0.000914, 0.00098, 0.000979, 0.00092, 0.000982, 0.001036, 0.001025, 0.000958, 0.000956, 0.000895,
0.000841, 0.000792, 0.000862, 0.000865, 0.00081, 0.000932, 0.000866, 0.000923, 0.000914, 0.000963,
0.000834, 0.000777, 0.000726, 0.000794, 0.000795, 0.000738, 0.000744, 0.000806, 0.000744, 0.000804,
0.000683, 0.000748, 0.00069, 0.000694, 0.000582, 0.000654, 0.000602, 0.000726, 0.00072, 0.000713,
0.000649, 0.000707, 0.000642, 0.000584, 0.000532, 0.000599, 0.000542, 0.000548, 0.000552, 0.000612,
0.000606, 0.0006, 0.000594, 0.000588, 0.000467, 0.000473, 0.000478, 0.000481, 0.000425, 0.000489,
0.000545, 0.000478, 0.000476, 0.00053, 0.000462, 0.000402, 0.000405, 0.000406, 0.000349, 0.000412,
0.000467, 0.000457, 0.000448, 0.000381, 0.000379, 0.000377, 0.000316, 0.000262, 0.000213, 0.000283,
0.000343, 0.000338, 0.000275, 0.000334, 0.000327, 0.000321, 0.000314, 0.000193, 0.000257, 0.000255,
0.000253, 0.000192, 0.000138, 0.000204, 0.000318, 0.000189, 0.000131, 0.000137, 0.000198, 0.000136,
0.000138, 0.000081, 0.000145, 0.0002, 0.000248, 0.000175, 0.000167, 0.000103, 0.00016, 0.000095,
0.000037, 0.000042, 0.000103, 0.000041
};

    //R1.00 1960 LOW
    float TIR6[1024] = {
0.001092, 0.000651, 0.000168, -0.001426, -0.005985, -0.012783, -0.021336, -0.031383, -0.031574, -0.023176,
-0.01438, -0.000028, 0.086054, 0.309884, 0.480397, 0.458541, 0.367891, 0.225462, 0.068529, 0.002825,
0.016023, 0.072214, 0.142697, 0.170993, 0.138628, 0.070916, 0.004249, -0.027487, -0.021912, 0.006631,
0.033259, 0.041151, 0.030196, 0.009715, -0.009446, -0.018807, -0.019522, -0.016483, -0.016228, -0.023085,
-0.036288, -0.052437, -0.067332, -0.078943, -0.08557, -0.087104, -0.084492, -0.079373, -0.073127, -0.067695,
-0.062739, -0.058256, -0.053184, -0.047927, -0.043271, -0.040784, -0.04151, -0.045209, -0.050794, -0.05645,
-0.060811, -0.06309, -0.063743, -0.063824, -0.064132, -0.064939, -0.066121, -0.067352, -0.067984, -0.068102,
-0.067533, -0.066361, -0.064946, -0.063427, -0.062111, -0.060981, -0.060591, -0.060496, -0.060921, -0.061402,
-0.061706, -0.061751, -0.06124, -0.06065, -0.059636, -0.058549, -0.057149, -0.05528, -0.053186, -0.050657,
-0.048404, -0.046255, -0.044484, -0.043122, -0.042327, -0.041944, -0.042195, -0.0429, -0.043963, -0.045439,
-0.047151, -0.048953, -0.050584, -0.051788, -0.05282, -0.053258, -0.053382, -0.052879, -0.05206, -0.051101,
-0.049625, -0.048075, -0.046416, -0.044563, -0.042798, -0.040819, -0.038612, -0.036185, -0.033474, -0.030538,
-0.027127, -0.023784, -0.020363, -0.017012, -0.013826, -0.010789, -0.008171, -0.005949, -0.003973, -0.00226,
-0.000958, -0.000071, 0.000415, 0.00085, 0.000919, 0.000832, 0.00049, 0.000186, -0.000177, -0.000528,
-0.000889, -0.001072, -0.001244, -0.001415, -0.001134, -0.000998, -0.000636, -0.000248, 0.000147, 0.000786,
0.0013, 0.001779, 0.002448, 0.003359, 0.004301, 0.005245, 0.006275, 0.00746, 0.008642, 0.009731,
0.011176, 0.012307, 0.013528, 0.014598, 0.015676, 0.016437, 0.017133, 0.01765, 0.017869, 0.018368,
0.018598, 0.018772, 0.018805, 0.018925, 0.019004, 0.019178, 0.019575, 0.020069, 0.020349, 0.020723,
0.021214, 0.021779, 0.022528, 0.023257, 0.024138, 0.024949, 0.025469, 0.026269, 0.027146, 0.02786,
0.028709, 0.029654, 0.030531, 0.031337, 0.03194, 0.032526, 0.033245, 0.03363, 0.034208, 0.03452,
0.034681, 0.034793, 0.034506, 0.034243, 0.034007, 0.03362, 0.033178, 0.032695, 0.031978, 0.031177,
0.030405, 0.029773, 0.029202, 0.028801, 0.028304, 0.027864, 0.027335, 0.026995, 0.026919, 0.026904,
0.026937, 0.027207, 0.027286, 0.027407, 0.027672, 0.028096, 0.028434, 0.028651, 0.029046, 0.029303,
0.029411, 0.02937, 0.029382, 0.029378, 0.029309, 0.029065, 0.028816, 0.028329, 0.027524, 0.026907,
0.025993, 0.025216, 0.024311, 0.023465, 0.022382, 0.021179, 0.020124, 0.018966, 0.018122, 0.017107,
0.016187, 0.015159, 0.014288, 0.013383, 0.012327, 0.01158, 0.010631, 0.009995, 0.009238, 0.008452,
0.007886, 0.007103, 0.006428, 0.005878, 0.005535, 0.005065, 0.004761, 0.004388, 0.00399, 0.00391,
0.004332, 0.004769, 0.004953, 0.005237, 0.005175, 0.004765, 0.004619, 0.004425, 0.004503, 0.004458,
0.004458, 0.004376, 0.004008, 0.003726, 0.003523, 0.003378, 0.003206, 0.003099, 0.003102, 0.002943,
0.002837, 0.002639, 0.002471, 0.002635, 0.00251, 0.002594, 0.002551, 0.00249, 0.002247, 0.002227,
0.002248, 0.002277, 0.00233, 0.002308, 0.002148, 0.002085, 0.00214, 0.002143, 0.002262, 0.002426,
0.002329, 0.00225, 0.002154, 0.002128, 0.00192, 0.001904, 0.001985, 0.001913, 0.001775, 0.001763,
0.001463, 0.00121, 0.00121, 0.001223, 0.001173, 0.001098, 0.001095, 0.000859, 0.000755, 0.000915,
0.00094, 0.001054, 0.001069, 0.001175, 0.001039, 0.000981, 0.001187, 0.001267, 0.001542, 0.001519,
0.001542, 0.001525, 0.001552, 0.001717, 0.001794, 0.00196, 0.001958, 0.002088, 0.002159, 0.002026,
0.002131, 0.002072, 0.002091, 0.002343, 0.002308, 0.002244, 0.002202, 0.002006, 0.001975, 0.002022,
0.002144, 0.002246, 0.002031, 0.001926, 0.001824, 0.001832, 0.001917, 0.001822, 0.001975, 0.001899,
0.001818, 0.001755, 0.001738, 0.001797, 0.001725, 0.001758, 0.001813, 0.001764, 0.001682, 0.001655,
0.001666, 0.001606, 0.001641, 0.001582, 0.001569, 0.001497, 0.001469, 0.001366, 0.001334, 0.0013,
0.001189, 0.001192, 0.001116, 0.000979, 0.000924, 0.000823, 0.000785, 0.000601, 0.000573, 0.000496,
0.00036, 0.000244, 0.000189, 0.000004, -0.00002, -0.000151, -0.000249, -0.000345, -0.000463, -0.000571,
-0.000683, -0.000732, -0.000883, -0.000926, -0.001068, -0.00114, -0.00129, -0.001283, -0.001455, -0.001484,
-0.001644, -0.001696, -0.001791, -0.001862, -0.001938, -0.002049, -0.002108, -0.002238, -0.002235, -0.00237,
-0.002458, -0.002503, -0.002594, -0.002653, -0.002733, -0.002801, -0.002902, -0.00289, -0.003028, -0.00308,
-0.003149, -0.00319, -0.003253, -0.003315, -0.003399, -0.003465, -0.003472, -0.003544, -0.003624, -0.003664,
-0.003726, -0.003764, -0.003816, -0.00385, -0.004013, -0.003866, -0.004055, -0.004093, -0.004114, -0.004135,
-0.004183, -0.00422, -0.004265, -0.004359, -0.004318, -0.004405, -0.004399, -0.004472, -0.004467, -0.004548,
-0.004527, -0.004597, -0.004609, -0.004649, -0.004634, -0.004727, -0.004712, -0.004736, -0.004774, -0.004785,
-0.004833, -0.004819, -0.004866, -0.004882, -0.004881, -0.004911, -0.004944, -0.004923, -0.004963, -0.004966,
-0.004971, -0.005031, -0.005006, -0.005002, -0.005013, -0.00507, -0.005029, -0.005038, -0.005064, -0.005058,
-0.005074, -0.005054, -0.005074, -0.005093, -0.005075, -0.005065, -0.005087, -0.005031, -0.005097, -0.00508,
-0.005053, -0.005086, -0.005072, -0.005042, -0.005044, -0.00508, -0.005015, -0.005051, -0.005024, -0.004997,
-0.005048, -0.004983, -0.004966, -0.004999, -0.004933, -0.004971, -0.004939, -0.004899, -0.004931, -0.004895,
-0.004852, -0.004904, -0.004808, -0.004848, -0.004817, -0.004779, -0.004812, -0.004752, -0.004727, -0.004747,
-0.004693, -0.004684, -0.004657, -0.004654, -0.004574, -0.004616, -0.004566, -0.004575, -0.004533, -0.004482,
-0.004473, -0.004434, -0.00447, -0.00437, -0.004385, -0.00437, -0.004287, -0.004347, -0.00423, -0.004233,
-0.004232, -0.004194, -0.004137, -0.004125, -0.004101, -0.00407, -0.004065, -0.003984, -0.00397, -0.003943,
-0.00395, -0.003874, -0.00383, -0.003865, -0.003791, -0.003748, -0.003726, -0.003724, -0.003654, -0.003627,
-0.003591, -0.003595, -0.003538, -0.003461, -0.003479, -0.003448, -0.003386, -0.003347, -0.003366, -0.003321,
-0.003238, -0.003236, -0.003185, -0.003188, -0.003099, -0.003107, -0.003042, -0.003038, -0.002993, -0.002919,
-0.002944, -0.002847, -0.002855, -0.002827, -0.002782, -0.002708, -0.002712, -0.002696, -0.002593, -0.002625,
-0.002529, -0.002544, -0.002489, -0.002461, -0.002387, -0.002432, -0.002355, -0.002322, -0.002238, -0.002267,
-0.002201, -0.00217, -0.002164, -0.002113, -0.002069, -0.002017, -0.00203, -0.001929, -0.001943, -0.001908,
-0.001843, -0.001833, -0.001771, -0.001778, -0.001714, -0.00167, -0.00165, -0.001642, -0.001563, -0.001563,
-0.001513, -0.001486, -0.001401, -0.00143, -0.001423, -0.00134, -0.00132, -0.001274, -0.001244, -0.001214,
-0.001228, -0.00111, -0.001156, -0.001133, -0.001021, -0.001031, -0.000975, -0.001012, -0.00092, -0.000888,
-0.00086, -0.00089, -0.000806, -0.00077, -0.000761, -0.000708, -0.000712, -0.00066, -0.000661, -0.00057,
-0.000575, -0.000587, -0.000522, -0.000479, -0.000462, -0.000437, -0.00039, -0.000432, -0.00033, -0.000322,
-0.000327, -0.000257, -0.00023, -0.000222, -0.000205, -0.000189, -0.000117, -0.000155, -0.000085, -0.000058,
-0.000033, -0.000015, -0.000047, 0.000046, 0.000078, 0.000038, 0.0001, 0.000109, 0.000147, 0.000172,
0.000174, 0.000201, 0.000246, 0.000247, 0.000268, 0.000318, 0.000273, 0.000359, 0.000357, 0.000373,
0.000404, 0.000419, 0.000423, 0.000469, 0.000447, 0.000502, 0.000528, 0.00054, 0.000515, 0.000562,
0.000635, 0.000534, 0.000647, 0.000635, 0.000656, 0.000655, 0.000686, 0.00068, 0.000727, 0.000748,
0.000736, 0.000773, 0.000752, 0.000809, 0.000806, 0.000777, 0.000845, 0.000855, 0.00082, 0.000884,
0.000891, 0.000888, 0.000931, 0.000862, 0.000963, 0.00093, 0.000963, 0.000965, 0.000964, 0.000978,
0.000994, 0.001052, 0.000944, 0.001062, 0.000981, 0.001073, 0.001044, 0.001062, 0.001045, 0.001124,
0.001059, 0.001089, 0.001083, 0.001119, 0.001152, 0.001119, 0.001081, 0.001181, 0.001123, 0.001185,
0.001124, 0.001188, 0.001135, 0.001205, 0.001155, 0.001205, 0.001168, 0.001188, 0.001195, 0.001193,
0.00122, 0.001197, 0.001226, 0.001179, 0.001252, 0.00123, 0.001224, 0.001183, 0.001276, 0.00122,
0.001232, 0.001243, 0.001248, 0.001221, 0.001246, 0.00124, 0.001246, 0.001239, 0.001227, 0.001294,
0.001224, 0.001201, 0.00124, 0.001323, 0.001187, 0.001268, 0.001244, 0.00122, 0.001249, 0.001237,
0.001257, 0.001225, 0.00124, 0.001215, 0.001231, 0.001269, 0.00123, 0.001197, 0.001233, 0.001211,
0.001254, 0.001177, 0.00123, 0.001211, 0.001234, 0.001184, 0.001194, 0.001196, 0.001195, 0.001213,
0.001178, 0.001171, 0.001216, 0.001144, 0.00117, 0.001174, 0.001167, 0.001156, 0.001161, 0.001104,
0.001162, 0.001152, 0.001165, 0.001074, 0.001142, 0.001162, 0.001055, 0.001132, 0.001092, 0.00111,
0.001114, 0.001043, 0.001079, 0.00111, 0.001036, 0.001066, 0.001054, 0.001055, 0.001041, 0.001023,
0.001056, 0.001024, 0.00099, 0.001007, 0.000989, 0.001014, 0.000976, 0.000983, 0.000991, 0.000923,
0.000971, 0.000953, 0.000926, 0.000932, 0.000996, 0.000845, 0.000936, 0.000892, 0.000899, 0.000902,
0.000869, 0.000869, 0.000873, 0.000856, 0.000845, 0.000853, 0.000823, 0.000837, 0.000803, 0.000798,
0.000817, 0.00082, 0.000789, 0.000733, 0.000775, 0.000795, 0.000724, 0.000746, 0.000754, 0.000705,
0.000768, 0.000698, 0.000702, 0.000701, 0.000678, 0.000669, 0.000692, 0.000657, 0.000682, 0.000642,
0.000595, 0.000672, 0.000614, 0.000636, 0.000569, 0.000623, 0.000584, 0.000593, 0.000566, 0.000555,
0.00054, 0.000576, 0.000537, 0.000551, 0.000521, 0.000502, 0.000498, 0.000532, 0.000473, 0.000483,
0.000511, 0.000423, 0.000483, 0.000441, 0.000441, 0.000447, 0.00044, 0.000392, 0.000385, 0.000407,
0.000416, 0.000386, 0.000347, 0.000401, 0.000325, 0.000366, 0.000324, 0.000383, 0.000307, 0.000323,
0.000341, 0.000295, 0.0003, 0.000286, 0.000281, 0.000269, 0.000295, 0.000267, 0.000266, 0.000237,
0.000227, 0.000251, 0.000225, 0.000245, 0.000207, 0.000199, 0.000205, 0.000196, 0.000191, 0.000181,
0.000154, 0.000208, 0.000106, 0.00019, 0.000148, 0.000104, 0.000153, 0.000121, 0.000118, 0.000101,
0.000137, 0.000091, 0.000069, 0.000083, 0.000122, 0.00002, 0.000111, 0.00002, 0.000095, 0.000036,
0.000041, 0.00006, 0.000028, 0.000034, 0.000016, 0.000007, 0.000042, 0.000029, -0.000001, -0.000034,
0.00006, -0.000064, -0.000022, 0.000036, -0.000061, -0.000065, -0.000027, 0.000009, -0.000094, -0.000048,
-0.000058, -0.000043, -0.000067, -0.000086, -0.000072, -0.000108, -0.000054, -0.000102, -0.000103, -0.000097,
-0.000116, -0.000095, -0.000104, -0.000
};

//R1.00 1960 High
float TIR6B[1024] = {
0.001374, 0.001221, 0.000542, -0.001844, -0.008481, -0.019057, -0.032557, -0.047724, -0.049229, -0.033666,
-0.010334, 0.022637, 0.146343, 0.465628, 0.767336, 0.799713, 0.619332, 0.263797, -0.173559, -0.49214,
-0.61452, -0.546608, -0.340235, -0.114666, 0.035044, 0.084143, 0.061469, 0.026703, 0.017401, 0.041681,
0.077133, 0.094801, 0.081556, 0.042751, -0.004663, -0.040469, -0.05567, -0.051695, -0.038466, -0.027748,
-0.025724, -0.032266, -0.042379, -0.050943, -0.052849, -0.045567, -0.029563, -0.007995, 0.014944, 0.03402,
0.047043, 0.053073, 0.053784, 0.050343, 0.04351, 0.032765, 0.017612, -0.001032, -0.020811, -0.038041,
-0.049387, -0.052757, -0.048608, -0.039506, -0.028708, -0.018991, -0.012082, -0.008538, -0.007593, -0.008612,
-0.0105, -0.012418, -0.014094, -0.01535, -0.016433, -0.017414, -0.019078, -0.021149, -0.023858, -0.026617,
-0.028884, -0.030309, -0.030399, -0.029689, -0.028054, -0.026125, -0.023963, -0.021548, -0.019257, -0.016968,
-0.015553, -0.015018, -0.015653, -0.01743, -0.020349, -0.02399, -0.028281, -0.032765, -0.03704, -0.040954,
-0.044221, -0.046622, -0.047869, -0.047704, -0.046563, -0.044278, -0.041413, -0.037953, -0.034484, -0.03152,
-0.028814, -0.026844, -0.025605, -0.024884, -0.024818, -0.024903, -0.024835, -0.024407, -0.023419, -0.021889,
-0.019564, -0.017146, -0.014757, -0.012735, -0.01134, -0.010611, -0.010824, -0.011911, -0.013518, -0.015388,
-0.017465, -0.019631, -0.021773, -0.023392, -0.02474, -0.025659, -0.026321, -0.026492, -0.026323, -0.025846,
-0.025194, -0.024247, -0.023244, -0.022311, -0.020964, -0.019826, -0.018611, -0.017521, -0.016623, -0.015633,
-0.014905, -0.014396, -0.013801, -0.012925, -0.011938, -0.010927, -0.009847, -0.00862, -0.007432, -0.006462,
-0.005213, -0.004377, -0.003619, -0.003158, -0.00281, -0.002894, -0.003142, -0.003583, -0.004311, -0.00459,
-0.00486, -0.004957, -0.004988, -0.004727, -0.004306, -0.003624, -0.002539, -0.001212, -0.000131, 0.000882,
0.001868, 0.002806, 0.003846, 0.004794, 0.005822, 0.006699, 0.007091, 0.007619, 0.008239, 0.008696,
0.009299, 0.010082, 0.010859, 0.011557, 0.011964, 0.012242, 0.012642, 0.012688, 0.012933, 0.012956,
0.012815, 0.012646, 0.012068, 0.011526, 0.011166, 0.01085, 0.01066, 0.010608, 0.010433, 0.010213,
0.010087, 0.010235, 0.010608, 0.011312, 0.011996, 0.012702, 0.013208, 0.013782, 0.014594, 0.015463,
0.01632, 0.017355, 0.018074, 0.018622, 0.019158, 0.019787, 0.020268, 0.020498, 0.020824, 0.02097,
0.020875, 0.020528, 0.020202, 0.019929, 0.019694, 0.019361, 0.019093, 0.018635, 0.017813, 0.017204,
0.01639, 0.015832, 0.015319, 0.015034, 0.014622, 0.014103, 0.013781, 0.01343, 0.013515, 0.013534,
0.013666, 0.01365, 0.013728, 0.013718, 0.01343, 0.013383, 0.013099, 0.01312, 0.013044, 0.012888,
0.01294, 0.012737, 0.012559, 0.012491, 0.012686, 0.012769, 0.012982, 0.013071, 0.013013, 0.013224,
0.014075, 0.015076, 0.015714, 0.016204, 0.016009, 0.015011, 0.013995, 0.01291, 0.012272, 0.011783,
0.011574, 0.011465, 0.011101, 0.01079, 0.010593, 0.010514, 0.010447, 0.010462, 0.010617, 0.010581,
0.01051, 0.010256, 0.009948, 0.010019, 0.009843, 0.009871, 0.009779, 0.009621, 0.009191, 0.008948,
0.008815, 0.00876, 0.008782, 0.008736, 0.008485, 0.008272, 0.008201, 0.008116, 0.008185, 0.008352,
0.008211, 0.007972, 0.007643, 0.007369, 0.006901, 0.006654, 0.006632, 0.00654, 0.006376, 0.006346,
0.00599, 0.005597, 0.005505, 0.005546, 0.005584, 0.005595, 0.005668, 0.005435, 0.005251, 0.005384,
0.005445, 0.005605, 0.005642, 0.005726, 0.005478, 0.005204, 0.005231, 0.005205, 0.005439, 0.005374,
0.005271, 0.005071, 0.004888, 0.004892, 0.004862, 0.004958, 0.00488, 0.004916, 0.004894, 0.004595,
0.004508, 0.004277, 0.004133, 0.00433, 0.004296, 0.004176, 0.004038, 0.003694, 0.003505, 0.003481,
0.003643, 0.003863, 0.003705, 0.003544, 0.003351, 0.003287, 0.003374, 0.003303, 0.003505, 0.003489,
0.003396, 0.003273, 0.003189, 0.003216, 0.003124, 0.003138, 0.003211, 0.003172, 0.003054, 0.002969,
0.002939, 0.002847, 0.002865, 0.002803, 0.002781, 0.002695, 0.002646, 0.002515, 0.002455, 0.002418,
0.002303, 0.002318, 0.002273, 0.00214, 0.002083, 0.001989, 0.001973, 0.001803, 0.001789, 0.00176,
0.001665, 0.001575, 0.001559, 0.001406, 0.00142, 0.001346, 0.001296, 0.001249, 0.001175, 0.001103,
0.001025, 0.001026, 0.000921, 0.000922, 0.000824, 0.000785, 0.000659, 0.000704, 0.000574, 0.000577,
0.000443, 0.000408, 0.000342, 0.000302, 0.00026, 0.000173, 0.000133, 0.000015, 0.000041, -0.000067,
-0.000152, -0.000185, -0.000259, -0.000302, -0.000367, -0.000424, -0.000522, -0.000493, -0.000616, -0.000675,
-0.000746, -0.000782, -0.000834, -0.000892, -0.00098, -0.00106, -0.001063, -0.001124, -0.001211, -0.00126,
-0.00133, -0.001372, -0.001427, -0.001462, -0.001656, -0.001508, -0.001685, -0.001756, -0.001793, -0.001812,
-0.001858, -0.001899, -0.001955, -0.00208, -0.002059, -0.002157, -0.002164, -0.002252, -0.002262, -0.002363,
-0.002359, -0.002444, -0.002477, -0.002537, -0.002532, -0.002646, -0.002659, -0.002697, -0.002755, -0.002787,
-0.002861, -0.002868, -0.002933, -0.002974, -0.002989, -0.003036, -0.003097, -0.003095, -0.003153, -0.003182,
-0.003207, -0.003301, -0.00331, -0.003319, -0.003344, -0.003433, -0.00342, -0.003443, -0.003494, -0.003515,
-0.003559, -0.003562, -0.003605, -0.003657, -0.003669, -0.003677, -0.003723, -0.003682, -0.003771, -0.003795,
-0.003794, -0.003856, -0.003876, -0.003864, -0.003881, -0.003949, -0.003908, -0.003965, -0.003966, -0.003956,
-0.004038, -0.004004, -0.003999, -0.00406, -0.004019, -0.004083, -0.004087, -0.004065, -0.004125, -0.004123,
-0.004096, -0.004178, -0.004106, -0.004163, -0.004164, -0.004145, -0.004208, -0.004177, -0.004165, -0.004211,
-0.004182, -0.004192, -0.004187, -0.00421, -0.004144, -0.004208, -0.004192, -0.004233, -0.004222, -0.004185,
-0.004187, -0.004164, -0.004233, -0.004158, -0.004187, -0.004203, -0.004136, -0.004221, -0.004127, -0.004135,
-0.004162, -0.004155, -0.004114, -0.004117, -0.004116, -0.004108, -0.004132, -0.004068, -0.004061, -0.004051,
-0.004088, -0.004036, -0.003997, -0.004057, -0.004012, -0.003977, -0.003967, -0.003988, -0.003938, -0.00392,
-0.003898, -0.003926, -0.003894, -0.003821, -0.00385, -0.003848, -0.003803, -0.003771, -0.003815, -0.003802,
-0.003726, -0.003726, -0.003685, -0.003707, -0.00363, -0.003647, -0.003598, -0.003611, -0.003587, -0.003518,
-0.003558, -0.003475, -0.003493, -0.003489, -0.003463, -0.003391, -0.0034, -0.003406, -0.003308, -0.003347,
-0.003261, -0.003286, -0.00325, -0.003235, -0.003164, -0.003226, -0.003173, -0.003145, -0.003052, -0.003083,
-0.003031, -0.003006, -0.003018, -0.002985, -0.002946, -0.002892, -0.002914, -0.002817, -0.002831, -0.002815,
-0.002757, -0.002752, -0.002696, -0.002714, -0.002662, -0.002617, -0.0026, -0.002608, -0.002536, -0.002538,
-0.002496, -0.002471, -0.002378, -0.002408, -0.002429, -0.002359, -0.002336, -0.002286, -0.002251, -0.002222,
-0.002251, -0.002134, -0.002178, -0.002178, -0.00206, -0.002055, -0.001995, -0.002044, -0.001964, -0.001924,
-0.001892, -0.001939, -0.001868, -0.001824, -0.001812, -0.001756, -0.001761, -0.001714, -0.00172, -0.001622,
-0.001616, -0.001642, -0.001587, -0.001534, -0.001509, -0.001483, -0.001432, -0.001486, -0.00139, -0.001369,
-0.001379, -0.001308, -0.001267, -0.001255, -0.001245, -0.001239, -0.001162, -0.001199, -0.001131, -0.00109,
-0.001054, -0.001032, -0.001078, -0.00099, -0.000937, -0.000978, -0.000922, -0.000907, -0.000864, -0.000828,
-0.000822, -0.000796, -0.000743, -0.000736, -0.000716, -0.000659, -0.000708, -0.000624, -0.000612, -0.000594,
-0.000558, -0.000536, -0.000532, -0.000482, -0.000502, -0.000445, -0.000402, -0.000378, -0.000409, -0.000368,
-0.000273, -0.000375, -0.000268, -0.000262, -0.000234, -0.000233, -0.0002, -0.000204, -0.000149, -0.00011,
-0.000116, -0.000075, -0.000097, -0.000038, -0.000029, -0.000066, 0.000004, 0.000035, 0.000002, 0.000072,
0.0001, 0.000105, 0.000159, 0.000083, 0.000184, 0.000166, 0.000206, 0.000216, 0.000216, 0.000231,
0.000253, 0.000334, 0.000223, 0.000338, 0.000265, 0.000365, 0.000357, 0.000385, 0.000371, 0.000466,
0.000411, 0.000434, 0.000428, 0.00047, 0.000525, 0.000503, 0.00045, 0.000555, 0.000513, 0.000585,
0.00053, 0.000598, 0.00055, 0.000628, 0.000589, 0.000646, 0.000617, 0.000638, 0.000653, 0.00066,
0.0007, 0.000688, 0.000725, 0.000678, 0.000758, 0.000755, 0.000753, 0.000702, 0.000803, 0.000765,
0.000778, 0.000797, 0.000813, 0.000788, 0.000817, 0.000821, 0.000835, 0.000836, 0.000828, 0.000914,
0.000857, 0.000818, 0.000854, 0.000975, 0.00085, 0.000924, 0.000913, 0.000885, 0.000917, 0.000915,
0.000948, 0.000924, 0.000942, 0.000921, 0.00094, 0.001, 0.000977, 0.000936, 0.00097, 0.000955,
0.001013, 0.000939, 0.000992, 9.869999E-04, 0.001024, 0.000979, 0.000983, 0.00099, 0.000997, 0.00103,
0.001004, 0.000995, 0.001053, 0.000984, 0.001004, 0.001019, 0.001023, 0.001019, 0.001031, 0.000969,
0.00103, 0.001041, 0.001074, 0.000976, 0.001037, 0.001081, 0.000969, 0.001039, 0.001011, 0.001037,
0.001056, 0.000981, 0.00101, 0.001061, 0.000994, 0.001021, 0.001019, 0.001028, 0.001021, 0.001003,
0.001045, 0.001023, 0.000982, 0.000996, 0.000982, 0.001019, 0.000993, 0.001004, 0.001024, 0.000952,
0.000998, 0.000993, 0.000967, 0.000975, 0.001065, 0.00091, 9.860001E-04, 0.000951, 0.000961, 0.000975,
0.000947, 0.000947, 0.000957, 0.000948, 0.000939, 0.000952, 0.000925, 0.000942, 0.000909, 0.0009,
0.000927, 0.000948, 0.000925, 0.000852, 0.000887, 0.000926, 0.00086, 0.000877, 0.000897, 0.000849,
0.000923, 0.000861, 0.000856, 0.000853, 0.000828, 0.000816, 0.000847, 0.000823, 0.000857, 0.000822,
0.00076, 0.000841, 0.000797, 0.000823, 0.000751, 0.000804, 0.000773, 0.000785, 0.000759, 0.000744,
0.000725, 0.000771, 0.000744, 0.000763, 0.000736, 0.000708, 0.000697, 0.00074, 0.000685, 0.00069,
0.00073, 0.000638, 0.000692, 0.000658, 0.000657, 0.00067, 0.000671, 0.000618, 0.000597, 0.000621,
0.000648, 0.000629, 0.000582, 0.000639, 0.000563, 0.000598, 0.000559, 0.000629, 0.000559, 0.000564,
0.000589, 0.000541, 0.000537, 0.000519, 0.000512, 0.000501, 0.000537, 0.00052, 0.000521, 0.000488,
0.000468, 0.000493, 0.000472, 0.000498, 0.00046, 0.000443, 0.000446, 0.000439, 0.000435, 0.000426,
0.000394, 0.000457, 0.000351, 0.000432, 0.000404, 0.000349, 0.000396, 0.00037, 0.000367, 0.000348,
0.000391, 0.000349, 0.000312, 0.000318, 0.000372, 0.000265, 0.000353, 0.000265, 0.00034, 0.00029,
0.000286, 0.000309, 0.000277, 0.000278, 0.000255, 0.000238, 0.00028, 0.000279, 0.000245, 0.000193,
0.000294, 0.000169, 0.000191, 0.000267, 0.000174, 0.000148, 0.000186, 0.000248, 0.000145, 0.000176,
0.000169, 0.00019, 0.000167, 0.000138, 0.000146, 0.000103, 0.000162, 0.00012, 0.000113, 0.000118,
0.000099, 0.000122, 0.00012, 0.00000
};

//R1.00 ROS C1-1 01 LOW
float TIR7[1024] = {
0.000446, 0.067887, 0.221912, 0.329336, 0.471847, 0.494231, 0.295082, 0.25531, 0.135092, 0.035916,
0.085424, 0.013407, 0.118538, 0.119489, 0.101916, 0.117498, -0.011126, 0.014671, -0.066464, -0.043484,
-0.041097, -0.068528, -0.012645, -0.070643, -0.014706, -0.057045, -0.023198, -0.017535, -0.028419, 0.001699,
-0.047222, -0.010826, -0.060728, -0.036595, -0.062294, -0.061073, -0.052483, -0.072558, -0.043106, -0.076829,
-0.045449, -0.074673, -0.054343, -0.064609, -0.060201, -0.046839, -0.056624, -0.028233, -0.047947, -0.02096,
-0.042655, -0.027988, -0.04066, -0.0398, -0.0369, -0.047732, -0.032875, -0.051649, -0.034469, -0.055126,
-0.044431, -0.060367, -0.060374, -0.066462, -0.076694, -0.072224, -0.088206, -0.076335, -0.091846, -0.078188,
-0.088324, -0.078807, -0.080679, -0.078413, -0.071631, -0.075499, -0.062018, -0.068766, -0.052766, -0.058597,
-0.044629, -0.046183, -0.037831, -0.033571, -0.031908, -0.022625, -0.026661, -0.015391, -0.022116, -0.011694,
-0.017989, -0.011247, -0.014522, -0.013533, -0.012847, -0.017564, -0.013664, -0.022258, -0.016997, -0.026333,
-0.021916, -0.029044, -0.027038, -0.030284, -0.031395, -0.030038, -0.034122, -0.028979, -0.034158, -0.027223,
-0.031733, -0.025289, -0.027371, -0.023372, -0.022454, -0.021637, -0.017755, -0.019674, -0.013898, -0.017269,
-0.010956, -0.014131, -0.008908, -0.010448, -0.007632, -0.006692, -0.006721, -0.003357, -0.005564, -0.000779,
-0.004034, 0.000865, -0.001874, 0.001953, 0.00074, 0.002602, 0.003565, 0.003255, 0.006315, 0.004486,
0.008833, 0.006396, 0.010975, 0.00933, 0.012838, 0.012577, 0.014488, 0.015833, 0.0158, 0.01857,
0.016955, 0.020478, 0.017994, 0.021202, 0.018866, 0.020945, 0.019592, 0.019858, 0.019881, 0.018291,
0.01939, 0.016429, 0.018138, 0.014621, 0.016121, 0.012952, 0.013311, 0.011141, 0.010071, 0.009192,
0.006858, 0.007025, 0.003853, 0.004825, 0.001551, 0.002655, 0.000145, 0.000812, -0.000357, -0.000391,
-0.000018, -0.000563, 0.001281, 0.000412, 0.00336, 0.002669, 0.005874, 0.006012, 0.008977, 0.010318,
0.012371, 0.014927, 0.016165, 0.019363, 0.020138, 0.023543, 0.023807, 0.027148, 0.027374, 0.029716,
0.030252, 0.031163, 0.032113, 0.031831, 0.033037, 0.031809, 0.033011, 0.031241, 0.032043, 0.030321,
0.030298, 0.028942, 0.028066, 0.027296, 0.025741, 0.025615, 0.023578, 0.023848, 0.021635, 0.021897,
0.020344, 0.020388, 0.019488, 0.019196, 0.019147, 0.018424, 0.018943, 0.018125, 0.019154, 0.018424,
0.019657, 0.018877, 0.019917, 0.019825, 0.020296, 0.020772, 0.020627, 0.021579, 0.020874, 0.021961,
0.020962, 0.021713, 0.020912, 0.021083, 0.020495, 0.020073, 0.01966, 0.01863, 0.018407, 0.016979,
0.016705, 0.015103, 0.014793, 0.013083, 0.012555, 0.011015, 0.009951, 0.008954, 0.007397, 0.006734,
0.005248, 0.004737, 0.00319, 0.002856, 0.001275, 0.000848, -0.000275, -0.000981, -0.001699, -0.002545,
-0.002876, -0.004057, -0.004268, -0.005471, -0.005711, -0.006913, -0.007163, -0.008183, -0.008513, -0.008969,
-0.009174, -0.008262, -0.007238, -0.004418, -0.002467, -0.000171, 0.000525, 0.001591, 0.001612, 0.001851,
0.001994, 0.002165, 0.003003, 0.003261, 0.00416, 0.003888, 0.004163, 0.003619, 0.003682, 0.003335,
0.003256, 0.00316, 0.002997, 0.003005, 0.002846, 0.003228, 0.003083, 0.003738, 0.003613, 0.004033,
0.003941, 0.004208, 0.004071, 0.004098, 0.004281, 0.00394, 0.004267, 0.003862, 0.004059, 0.003773,
0.003905, 0.003583, 0.00363, 0.003502, 0.003426, 0.003594, 0.003532, 0.003782, 0.003705, 0.004253,
0.004154, 0.004637, 0.004789, 0.00515, 0.005376, 0.005677, 0.006006, 0.006227, 0.006851, 0.006975,
0.007498, 0.007571, 0.007989, 0.007987, 0.008277, 0.008293, 0.008344, 0.008467, 0.008336, 0.008386,
0.008057, 0.008156, 0.007794, 0.007787, 0.007476, 0.007393, 0.007083, 0.006742, 0.006562, 0.006165,
0.005967, 0.005537, 0.005426, 0.005094, 0.004965, 0.004587, 0.004405, 0.004123, 0.004019, 0.003822,
0.003673, 0.003776, 0.003431, 0.003502, 0.003357, 0.003316, 0.003334, 0.003348, 0.003273, 0.003427,
0.003365, 0.003396, 0.003469, 0.003387, 0.003578, 0.003567, 0.00363, 0.003559, 0.003773, 0.003623,
0.003718, 0.003698, 0.003635, 0.003763, 0.003559, 0.003587, 0.003478, 0.003418, 0.003252, 0.003247,
0.003058, 0.00301, 0.002829, 0.002712, 0.002614, 0.002361, 0.002318, 0.002102, 0.00204, 0.001827,
0.001834, 0.001603, 0.001492, 0.0014, 0.001237, 0.001166, 0.001031, 0.000954, 0.00076, 0.000798,
0.000606, 0.000565, 0.000483, 0.000406, 0.000288, 0.000201, 0.000167, 0.000033, 0.000013, -0.000105,
-0.00014, -0.000272, -0.000326, -0.000445, -0.000455, -0.000577, -0.000673, -0.000708, -0.000845, -0.000876,
-0.000967, -0.001028, -0.00118, -0.001205, -0.001309, -0.001388, -0.001492, -0.001597, -0.001682, -0.001748,
-0.00186, -0.001961, -0.001958, -0.002148, -0.002191, -0.002289, -0.002369, -0.00245, -0.002497, -0.002602,
-0.002712, -0.002748, -0.002843, -0.002908, -0.00296, -0.002997, -0.003177, -0.003147, -0.003238, -0.003307,
-0.003335, -0.003438, -0.003473, -0.003503, -0.003558, -0.003634, -0.003603, -0.003727, -0.003737, -0.003813,
-0.00382, -0.003842, -0.00389, -0.003912, -0.003978, -0.003964, -0.004078, -0.00405, -0.004098, -0.004125,
-0.004188, -0.00418, -0.004206, -0.004282, -0.004252, -0.004296, -0.004313, -0.004379, -0.004375, -0.004441,
-0.004424, -0.004487, -0.004532, -0.004515, -0.004569, -0.004557, -0.004575, -0.004611, -0.00467, -0.004641,
-0.004688, -0.004678, -0.004678, -0.004737, -0.004743, -0.004735, -0.0048, -0.00479, -0.004756, -0.004833,
-0.004814, -0.004847, -0.004859, -0.004868, -0.004837, -0.004894, -0.004908, -0.004886, -0.004923, -0.004888,
-0.004892, -0.004892, -0.004915, -0.004889, -0.004948, -0.004903, -0.004911, -0.004974, -0.004913, -0.0049,
-0.004916, -0.004924, -0.004879, -0.004921, -0.004934, -0.004916, -0.004879, -0.00491, -0.004899, -0.004894,
-0.004904, -0.00485, -0.004885, -0.004868, -0.004855, -0.004851, -0.004877, -0.004846, -0.004841, -0.004824,
-0.00478, -0.004814, -0.004807, -0.004783, -0.004785, -0.004775, -0.004703, -0.004727, -0.004743, -0.004712,
-0.004701, -0.004675, -0.004622, -0.004621, -0.004626, -0.004609, -0.004572, -0.004528, -0.004519, -0.004489,
-0.004512, -0.004484, -0.004476, -0.004427, -0.004396, -0.004387, -0.004363, -0.004338, -0.004299, -0.00431,
-0.004243, -0.00426, -0.004238, -0.004189, -0.004174, -0.00417, -0.004092, -0.004089, -0.004097, -0.004024,
-0.004057, -0.003983, -0.003967, -0.003929, -0.00392, -0.003886, -0.003857, -0.003848, -0.003761, -0.003788,
-0.003776, -0.003707, -0.003674, -0.003689, -0.003605, -0.003618, -0.003588, -0.003546, -0.003541, -0.003472,
-0.003469, -0.003425, -0.003421, -0.003362, -0.003347, -0.003359, -0.003271, -0.003186, -0.003231, -0.003219,
-0.00317, -0.003114, -0.003061, -0.00308, -0.002997, -0.003004, -0.002964, -0.002951, -0.00289, -0.002843,
-0.002865, -0.002783, -0.002783, -0.002759, -0.002711, -0.002658, -0.002656, -0.00263, -0.002608, -0.002552,
-0.002509, -0.002532, -0.002467, -0.002422, -0.002426, -0.002383, -0.002305, -0.002321, -0.002311, -0.002254,
-0.00221, -0.002199, -0.002127, -0.002119, -0.002119, -0.002042, -0.002018, -0.001976, -0.001951, -0.001913,
-0.001918, -0.001839, -0.00183, -0.001801, -0.001725, -0.001759, -0.001712, -0.001676, -0.001635, -0.00161,
-0.001549, -0.001556, -0.001523, -0.001449, -0.001466, -0.001431, -0.001364, -0.001349, -0.001352, -0.001295,
-0.001284, -0.001223, -0.001186, -0.0012, -0.001165, -0.001126, -0.001126, -0.001069, -0.001007, -0.001027,
-0.000993, -0.000958, -0.000939, -0.000927, -0.000838, -0.000861, -0.000857, -0.000764, -0.000767, -0.000728,
-0.000712, -0.000681, -0.00065, -0.000631, -0.000636, -0.000582, -0.000543, -0.000559, -0.000503, -0.000477,
-0.000474, -0.00045, -0.0004, -0.000397, -0.000386, -0.000327, -0.000343, -0.000324, -0.000232, -0.000254,
-0.000246, -0.000202, -0.000171, -0.000144, -0.000141, -0.000116, -0.000121, -0.000033, -0.000056, -0.000047,
0.000035, 0, 0.000029, 0.00005, 0.000033, 0.000071, 0.000143, 0.000108, 0.000115, 0.000168,
0.000178, 0.0002, 0.000217, 0.000246, 0.000229, 0.000238, 0.000269, 0.000306, 0.000341, 0.000333,
0.000352, 0.000395, 0.000387, 0.00042, 0.000446, 0.000426, 0.000446, 0.000442, 0.000446, 0.000493,
0.000557, 0.000529, 0.000526, 0.000574, 0.000562, 0.000597, 0.000638, 0.000654, 0.000654, 0.000668,
0.000661, 0.000695, 0.000744, 0.000698, 0.000705, 0.000769, 0.000737, 0.000739, 0.000848, 0.000762,
0.000729, 0.000851, 0.000815, 0.000837, 0.000859, 0.00085, 0.00085, 0.000849, 0.000885, 0.000884,
0.000952, 0.000913, 0.000889, 0.000953, 0.000951, 0.000945, 0.000924, 0.000984, 0.000982, 0.000966,
0.000969, 0.000985, 0.001023, 0.001011, 0.001009, 0.001015, 0.00102, 0.001031, 0.001071, 0.001041,
0.001062, 0.001088, 0.001062, 0.001102, 0.001115, 0.00109, 0.001102, 0.001117, 0.001076, 0.001114,
0.001152, 0.00113, 0.001105, 0.001139, 0.001132, 0.001127, 0.001165, 0.001111, 0.001124, 0.001134,
0.00113, 0.00114, 0.001183, 0.001149, 0.001133, 0.001154, 0.001109, 0.001171, 0.001191, 0.001173,
0.001152, 0.001182, 0.001131, 0.001151, 0.00121, 0.001121, 0.00117, 0.001175, 0.001149, 0.001163,
0.001202, 0.001176, 0.001178, 0.001186, 0.001136, 0.001194, 0.001199, 0.001181, 0.001163, 0.0012,
0.001188, 0.001181, 0.001199, 0.001172, 0.001183, 0.001182, 0.001204, 0.001196, 0.001198, 0.001175,
0.001192, 0.001192, 0.00114, 0.001163, 0.001178, 0.001145, 0.001152, 0.00117, 0.001114, 0.001149,
0.001179, 0.001126, 0.001109, 0.001142, 0.001136, 0.001114, 0.001122, 0.001096, 0.001112, 0.001126,
0.0011, 0.001092, 0.001122, 0.001096, 0.001089, 0.001086, 0.001067, 0.001091, 0.001099, 0.001086,
0.001061, 0.001073, 0.001066, 0.001064, 0.001062, 0.001029, 0.000997, 0.001014, 0.000989, 0.00099,
0.001013, 0.000957, 0.00099, 0.000978, 0.00093, 0.000929, 0.000952, 0.00093, 0.000896, 0.000906,
0.000874, 0.000862, 0.000911, 0.000878, 0.000812, 0.000814, 0.000793, 0.000832, 0.000811, 0.000794,
0.000771, 0.000768, 0.000766, 0.000718, 0.000768, 0.000693, 0.000703, 0.000718, 0.000677, 0.000685,
0.000699, 0.000655, 0.000636, 0.000649, 0.000603, 0.000652, 0.000599, 0.000603, 0.00059, 0.000567,
0.000559, 0.000545, 0.000572, 0.000524, 0.000547, 0.00052, 0.000503, 0.000498, 0.000512, 0.000468,
0.000457, 0.000476, 0.000442, 0.00045, 0.000432, 0.000428, 0.00039, 0.000417, 0.000388, 0.000397,
0.000401, 0.000368, 0.000379, 0.000364, 0.000357, 0.000349, 0.000358, 0.000328, 0.000343, 0.000306,
0.000318, 0.00031, 0.000303, 0.000302, 0.000269, 0.000294, 0.00026, 0.000271, 0.000246, 0.000251,
0.000242, 0.00024, 0.000225, 0.000228, 0.000238, 0.000188, 0.00021, 0.000187, 0.000194, 0.000189,
0.000202, 0.000176, 0.000174, 0.000172, 0.000146, 0.000177, 0.000154, 0.000149, 0.000125, 0.000142,
0.000129, 0.000131, 0.000143, 0.000122
};

//R1.00 ROS C1-1 01 HIGH
float TIR7B[1024] = {
0.000553, 0.084372, 0.299481, 0.495211, 0.721445, 0.781053, 0.487574, 0.254249, -0.082734, -0.394863,
-0.475831, -0.586124, -0.426264, -0.287838, -0.154818, 0.005684, -0.028619, 0.046107, -0.012552, 0.010985,
0.019461, -0.011147, 0.048877, -0.00806, 0.046627, 0.005363, 0.036184, 0.04794, 0.037137, 0.068642,
0.011959, 0.036657, -0.026215, -0.01671, -0.049483, -0.053865, -0.039708, -0.051707, -0.007845, -0.027849,
0.015399, -0.00394, 0.020521, 0.014106, 0.017271, 0.031346, 0.01945, 0.047115, 0.024561, 0.046571,
0.01795, 0.021758, -0.001081, -0.01199, -0.016401, -0.0331, -0.019062, -0.036727, -0.016436, -0.033299,
-0.020287, -0.033917, -0.034364, -0.040032, -0.05148, -0.046181, -0.061018, -0.045692, -0.056156, -0.036353,
-0.039006, -0.023183, -0.018608, -0.012819, -0.003342, -0.007499, 0.004747, -0.00503, 0.007132, -0.002949,
0.005977, -0.000006, 0.002999, 0.002898, -0.000698, 0.004167, -0.004971, 0.001493, -0.010206, -0.004706,
-0.015353, -0.013056, -0.019605, -0.022105, -0.023605, -0.03066, -0.027987, -0.037682, -0.03276, -0.041829,
-0.036731, -0.042198, -0.038437, -0.03893, -0.037553, -0.032952, -0.034344, -0.026275, -0.029036, -0.019972,
-0.022779, -0.015421, -0.016835, -0.01323, -0.012861, -0.013622, -0.011288, -0.015391, -0.011644, -0.01717,
-0.012802, -0.01758, -0.013875, -0.016362, -0.014652, -0.014258, -0.01516, -0.012266, -0.015223, -0.011013,
-0.014981, -0.010837, -0.014196, -0.011136, -0.012792, -0.011624, -0.010946, -0.011801, -0.00892, -0.011073,
-0.006842, -0.00944, -0.005037, -0.00672, -0.003488, -0.003896, -0.002502, -0.001549, -0.002398, -0.000351,
-0.002978, -0.000392, -0.003881, -0.001676, -0.004833, -0.003587, -0.005378, -0.0056, -0.005617, -0.007293,
-0.005934, -0.008653, -0.006472, -0.009426, -0.007287, -0.009621, -0.008535, -0.009746, -0.010017, -0.009831,
-0.011202, -0.009842, -0.011858, -0.009546, -0.011399, -0.008827, -0.009711, -0.007515, -0.007001, -0.005569,
-0.003647, -0.002891, 0.000267, 0.000435, 0.004304, 0.004291, 0.007869, 0.008192, 0.010976, 0.012037,
0.013372, 0.015108, 0.015171, 0.017063, 0.016309, 0.018095, 0.016592, 0.018118, 0.016579, 0.017049,
0.015856, 0.01493, 0.014257, 0.012393, 0.012318, 0.009975, 0.010373, 0.008054, 0.008543, 0.006825,
0.006936, 0.005999, 0.005606, 0.005584, 0.004838, 0.005758, 0.004814, 0.0063, 0.005315, 0.006773,
0.006515, 0.007802, 0.008201, 0.009069, 0.01021, 0.010483, 0.011916, 0.011839, 0.013539, 0.013374,
0.015073, 0.014608, 0.015757, 0.015772, 0.016176, 0.016594, 0.016235, 0.016993, 0.015983, 0.016755,
0.015402, 0.015745, 0.014621, 0.014455, 0.013658, 0.013047, 0.012584, 0.011525, 0.011416, 0.010173,
0.010189, 0.008966, 0.009143, 0.008009, 0.0081, 0.00728, 0.006892, 0.006701, 0.005926, 0.006123,
0.005565, 0.006049, 0.005494, 0.006154, 0.005512, 0.005917, 0.005627, 0.005642, 0.005633, 0.005417,
0.005723, 0.005062, 0.005295, 0.004459, 0.004518, 0.003573, 0.003557, 0.002811, 0.002771, 0.002755,
0.003093, 0.004895, 0.007207, 0.011809, 0.015723, 0.019501, 0.020579, 0.020646, 0.018438, 0.015495,
0.012124, 0.00893, 0.007105, 0.005569, 0.005548, 0.004972, 0.005271, 0.004919, 0.005223, 0.005171,
0.005353, 0.00552, 0.00556, 0.005722, 0.005654, 0.006149, 0.006108, 0.006889, 0.006867, 0.007269,
0.007053, 0.00707, 0.006592, 0.00618, 0.005981, 0.005251, 0.005273, 0.004649, 0.004692, 0.004357,
0.004513, 0.004265, 0.004396, 0.004406, 0.004461, 0.004813, 0.00495, 0.005395, 0.005468, 0.006171,
0.006181, 0.006684, 0.006822, 0.007085, 0.007151, 0.00722, 0.007291, 0.007218, 0.007604, 0.007501,
0.00778, 0.007583, 0.007693, 0.007355, 0.00728, 0.006947, 0.006639, 0.006466, 0.006077, 0.005918,
0.005405, 0.005388, 0.004986, 0.004993, 0.004767, 0.004815, 0.004673, 0.004467, 0.004441, 0.004205,
0.004179, 0.003926, 0.004026, 0.003956, 0.004111, 0.004011, 0.004075, 0.004033, 0.004173, 0.00423,
0.004313, 0.00469, 0.004569, 0.004799, 0.004801, 0.004853, 0.004956, 0.005051, 0.005018, 0.005213,
0.005183, 0.0052, 0.005254, 0.005119, 0.005263, 0.005225, 0.00524, 0.00509, 0.005245, 0.005032,
0.005032, 0.004939, 0.004783, 0.004852, 0.004582, 0.004526, 0.004357, 0.004243, 0.004026, 0.004001,
0.003817, 0.003789, 0.003648, 0.003571, 0.003539, 0.003338, 0.003355, 0.003216, 0.003239, 0.003118,
0.003234, 0.003113, 0.003078, 0.003069, 0.002979, 0.002974, 0.002905, 0.002891, 0.00274, 0.002834,
0.002706, 0.00271, 0.002686, 0.00266, 0.002576, 0.002506, 0.002499, 0.002385, 0.002382, 0.002284,
0.002266, 0.002143, 0.002091, 0.001971, 0.001974, 0.001872, 0.001779, 0.001759, 0.001634, 0.001616,
0.001548, 0.001509, 0.00136, 0.001336, 0.001245, 0.001173, 0.001073, 0.000966, 0.000882, 0.000831,
0.000737, 0.000649, 0.000689, 0.000528, 0.000494, 0.000416, 0.000351, 0.000288, 0.000265, 0.00018,
0.00007, 0.000038, -0.000046, -0.000104, -0.000142, -0.000155, -0.00034, -0.000319, -0.000397, -0.000467,
-0.00049, -0.000594, -0.000637, -0.000664, -0.000713, -0.000798, -0.000766, -0.000895, -0.00093, -0.00103,
-0.001058, -0.001089, -0.001147, -0.001184, -0.001272, -0.001279, -0.001425, -0.001432, -0.001498, -0.001547,
-0.001639, -0.001655, -0.001692, -0.001793, -0.001785, -0.00184, -0.001874, -0.00197, -0.001999, -0.002098,
-0.00211, -0.002198, -0.00228, -0.002285, -0.002353, -0.002353, -0.002374, -0.00242, -0.002509, -0.002506,
-0.002575, -0.002586, -0.002599, -0.002684, -0.002726, -0.002743, -0.002842, -0.002868, -0.002847, -0.002943,
-0.002954, -0.00301, -0.003051, -0.003085, -0.003066, -0.003139, -0.003185, -0.003186, -0.003245, -0.003229,
-0.003244, -0.003259, -0.003308, -0.00331, -0.003407, -0.003401, -0.003435, -0.003541, -0.003515, -0.00351,
-0.003538, -0.003568, -0.003537, -0.003599, -0.00365, -0.003668, -0.00365, -0.003702, -0.00372, -0.003742,
-0.00378, -0.003743, -0.003794, -0.003804, -0.003813, -0.003831, -0.00389, -0.003889, -0.003906, -0.003907,
-0.00387, -0.003918, -0.003942, -0.003944, -0.003972, -0.003989, -0.003927, -0.00396, -0.004009, -0.00401,
-0.004026, -0.004021, -0.003975, -0.003983, -0.004012, -0.004028, -0.004017, -0.003988, -0.003996, -0.003988,
-0.004046, -0.004061, -0.004089, -0.004065, -0.004045, -0.004047, -0.00404, -0.00403, -0.004004, -0.004038,
-0.003992, -0.004031, -0.004043, -0.004018, -0.004022, -0.004043, -0.00398, -0.003984, -0.004019, -0.003966,
-0.004023, -0.003973, -0.003969, -0.003943, -0.003951, -0.003938, -0.003926, -0.00394, -0.003865, -0.003905,
-0.003927, -0.00388, -0.003854, -0.003889, -0.00382, -0.003846, -0.00384, -0.003815, -0.003827, -0.00377,
-0.003776, -0.003745, -0.003759, -0.003715, -0.003711, -0.003749, -0.003679, -0.00358, -0.003628, -0.00365,
-0.00363, -0.003588, -0.003536, -0.003568, -0.0035, -0.00352, -0.003501, -0.003511, -0.003466, -0.003421,
-0.003458, -0.00339, -0.003399, -0.003392, -0.003358, -0.003305, -0.00331, -0.003301, -0.003297, -0.00325,
-0.003207, -0.003243, -0.003194, -0.00315, -0.003162, -0.003131, -0.00305, -0.003067, -0.003079, -0.00304,
-0.002999, -0.002994, -0.002923, -0.002915, -0.002934, -0.002868, -0.002845, -0.002807, -0.002789, -0.00276,
-0.002784, -0.002718, -0.002715, -0.002697, -0.00262, -0.002662, -0.002635, -0.002607, -0.002567, -0.002542,
-0.002476, -0.002484, -0.002464, -0.002389, -0.00241, -0.002393, -0.002329, -0.002313, -0.00233, -0.002286,
-0.002281, -0.00222, -0.002175, -0.002192, -0.002171, -0.002136, -0.002144, -0.002089, -0.002013, -0.002026,
-0.002, -0.001969, -0.001954, -0.00195, -0.001856, -0.001873, -0.001883, -0.001789, -0.001783, -0.001743,
-0.001728, -0.001701, -0.001673, -0.001658, -0.001676, -0.001631, -0.001585, -0.001601, -0.001546, -0.001509,
-0.001502, -0.00148, -0.001424, -0.001414, -0.001407, -0.001344, -0.001358, -0.001349, -0.001246, -0.001253,
-0.001252, -0.001211, -0.001174, -0.001141, -0.001138, -0.001119, -0.001133, -0.00104, -0.001053, -0.00105,
-0.000959, -0.000985, -0.000963, -0.000943, -0.000967, -0.000935, -0.000843, -0.000861, -0.000856, -0.000797,
-0.000773, -0.000741, -0.000717, -0.000681, -0.000701, -0.000704, -0.000678, -0.000632, -0.000578, -0.000573,
-0.000551, -0.0005, -0.000503, -0.00047, -0.00044, -0.000465, -0.000456, -0.000469, -0.000474, -0.000421,
-0.000323, -0.000323, -0.000321, -0.000265, -0.00027, -0.000234, -0.000181, -0.000152, -0.000151, -0.000146,
-0.000166, -0.000143, -0.000086, -0.000131, -0.000138, -0.000068, -0.000088, -0.000086, 0.000051, -0.000015,
-0.000067, 0.000063, 0.000052, 0.000084, 0.000119, 0.000116, 0.000112, 0.000103, 0.00014, 0.000147,
0.000235, 0.000213, 0.00018, 0.000244, 0.000255, 0.000252, 0.000224, 0.000287, 0.000303, 0.000293,
0.000296, 0.000317, 0.000372, 0.000378, 0.000382, 0.00039, 0.000398, 0.000413, 0.000467, 0.000448,
0.000471, 0.000507, 0.000484, 0.000527, 0.000552, 0.000527, 0.000533, 0.000548, 0.000501, 0.000535,
0.000594, 0.00059, 0.000568, 0.000607, 0.000611, 0.000611, 0.000663, 0.000614, 0.000621, 0.000636,
0.00064, 0.000662, 0.000729, 0.000715, 0.000698, 0.00072, 0.00067, 0.000736, 0.000781, 0.00078,
0.00076, 0.000791, 0.000735, 0.000745, 0.00082, 0.000737, 0.000784, 0.000804, 0.000787, 0.000807,
0.000867, 0.000859, 0.000865, 0.000877, 0.000818, 0.000872, 0.000891, 0.00088, 0.000857, 0.000898,
0.000896, 0.000892, 0.000917, 0.000893, 0.000903, 0.000906, 0.000939, 0.000943, 0.000952, 0.000928,
0.000944, 0.000948, 0.000889, 0.000903, 0.000928, 0.000905, 0.000918, 0.000954, 0.000906, 0.000948,
0.001003, 0.000965, 0.00094, 0.000976, 0.000981, 0.000962, 0.000971, 0.000945, 0.000961, 0.000989,
0.000973, 0.000967, 0.001006, 0.000991, 0.000985, 0.000982, 0.00096, 9.869999E-04, 0.001009, 0.001008,
0.000983, 0.000993, 0.000988, 0.000989, 0.000991, 0.000956, 0.000911, 0.000924, 0.000904, 0.000916,
0.000962, 0.000925, 0.000971, 0.000981, 0.000937, 0.000931, 0.000961, 0.00095, 0.000914, 0.000922,
0.000892, 0.000879, 0.000946, 0.000937, 0.000868, 0.000859, 0.000836, 0.00089, 0.000894, 0.000894,
0.000879, 0.00088, 0.000886, 0.000837, 0.000893, 0.000821, 0.000825, 0.000851, 0.000818, 0.000832,
0.000861, 0.000827, 0.000804, 0.000818, 0.000772, 0.000828, 0.000786, 0.00079, 0.000782, 0.00076,
0.000751, 0.000739, 0.000778, 0.00074, 0.000769, 0.00075, 0.000733, 0.000725, 0.000746, 0.000703,
0.000683, 0.000705, 0.000676, 0.000687, 0.000675, 0.000675, 0.000636, 0.000665, 0.000644, 0.000659,
0.000674, 0.000645, 0.000655, 0.00064, 0.000629, 0.000618, 0.000627, 0.000595, 0.000607, 0.000566,
0.000574, 0.000569, 0.000565, 0.000569, 0.000534, 0.00056, 0.000529, 0.000539, 0.000513, 0.000517,
0.000512, 0.000512, 0.000498, 0.000501, 0.000518, 0.000464, 0.000479, 0.000454, 0.000459, 0.000457,
0.000477, 0.000454, 0.000449, 0.000445, 0.000411, 0.000441, 0.000422, 0.000413, 0.000381, 0.000395,
0.000383, 0.000386, 0.000406, 0.000388
};

//R1.00 ACOUSTIC LOW
float TIR8[1024] = {
0, 0.000016, -0.001605, -0.000765, 0.000027, -0.001898, 0.002916, 0.002684, -0.003704, 0.000126,
-0.008926, -0.007377, 0.011881, -0.011075, 0.000477, 0.011341, -0.035658, -0.013563, 0.020863, -0.03631,
-0.000355, 0.09673, -0.044671, -0.11774, -0.028904, 0.008908, 0.022126, -0.077208, -0.08836, 0.063019,
0.108149, -0.137548, -0.317554, 0.024202, 0.394436, 0.076154, -0.401096, -0.174991, 0.31087, 0.101231,
-0.42121, -0.108156, 0.446399, 0.296921, -0.221062, -0.424763, -0.016449, 0.346288, 0.09669, -0.307754,
0.096616, 0.542295, -0.109109, -0.44088, -0.074927, 0.088559, 0.132591, -0.018631, -0.204258, -0.044727,
0.286035, 0.032296, -0.291597, -0.138503, -0.051415, 0.173413, 0.171946, -0.071679, -0.145047, -0.051736,
0.068182, -0.088997, -0.143268, 0.019052, 0.009662, -0.00633, 0.22406, 0.167916, -0.235695, -0.342185,
-0.109279, 0.133284, 0.193861, -0.144472, -0.142417, 0.133165, -0.138123, -0.109996, 0.324739, 0.13511,
-0.553977, -0.213726, 0.278701, 0.093443, 0.04575, -0.204772, -0.072336, 0.172201, 0.103167, -0.014959,
-0.139713, -0.124076, 0.065524, 0.160837, -0.101777, -0.093561, 0.034381, 0.117725, 0.051273, -0.118153,
0.012128, 0.103212, -0.025889, -0.043891, 0.044544, -0.002162, 0.019105, 0.006821, 0.000665, 0.027833,
-0.073638, -0.142001, 0.047481, 0.180796, 0.01498, -0.093278, -0.051849, 0.108992, 0.089044, -0.141405,
-0.206991, 0.00189, 0.157347, 0.092648, -0.020762, -0.104088, -0.014627, 0.068345, 0.004281, -0.025281,
0.069976, -0.013893, 0.038197, 0.317831, -0.004705, -0.230895, 0.052577, 0.178359, 0.001892, -0.078938,
0.07478, 0.079228, 0.151423, -0.029946, -0.220138, 0.007674, 0.096434, 0.033574, -0.065182, -0.009624,
0.081195, 0.032137, -0.017045, 0.022579, 0.002014, -0.026652, 0.050979, 0.039248, 0.063619, 0.119072,
-0.048279, -0.183587, 0.094414, 0.180882, -0.00445, -0.059345, -0.02765, 0.120007, 0.132401, 0.028541,
-0.060943, -0.04479, 0.068851, 0.030902, -0.136348, -0.036963, 0.172942, 0.065194, -0.109781, -0.025843,
0.077752, -0.020423, 0.005337, -0.014717, -0.000981, 0.155578, 0.083668, -0.041248, 0.005023, 0.13573,
0.009298, -0.021396, 0.118436, 0.001983, 0.023245, 0.078918, -0.024049, -0.055419, 0.019216, 0.071054,
0.015692, -0.005484, -0.041112, 0.019685, 0.084281, 0.004646, -0.036537, -0.018423, 0.062744, 0.056939,
-0.070117, -0.087895, 0.009227, 0.034846, 0.043545, -0.005218, -0.059346, 0.006635, 0.008294, -0.039314,
0.020504, 0.008767, -0.082564, 0.054741, 0.042296, -0.058784, 0.00536, 0.012081, 0.005567, 0.027344,
0.041711, -0.059609, -0.007419, 0.043661, -0.028233, 0.028593, 0.062078, 0.033681, -0.01315, 0.02651,
0.0164, -0.042726, 0.001974, -0.002193, -0.014267, -0.00358, -0.002531, 0.061329, 0.088959, -0.047434,
-0.117087, 0.047272, 0.079389, 0.011581, 0.020652, -0.036104, -0.019589, 0.041761, 0.084326, 0.081412,
-0.024495, -0.060424, 0.059365, 0.095643, 0.010151, 0.000476, 0.011841, 0.043326, 0.037647, -0.005648,
0.023464, 0.058601, -0.003922, -0.054162, -0.031587, -0.0223, -0.009115, -0.035182, -0.041207, -0.00782,
0.008394, -0.004002, -0.038771, -0.035049, 0.014434, 0.023658, -0.004111, -0.000438, -0.022401, -0.045442,
0.016985, 0.052081, 0.003025, 0.004844, -0.016474, -0.018501, 0.035093, 0.030352, 0.012001, 0.007993,
0.008617, 0.0104, 0.072021, 0.058959, -0.01345, -0.003445, 0.041212, 0.074012, 0.045637, 0.019557,
0.029141, 0.029093, -0.031713, -0.050149, 0.022813, 0.03965, -0.007702, -0.038004, -0.010614, 0.026348,
0.001059, -0.078503, -0.043197, 0.057741, 0.005906, -0.01519, 0.035938, 0.010002, -0.038244, -0.007507,
0.020741, -0.00682, 0.036699, -0.000999, -0.017973, 0.0585, 0.022064, -0.025568, 0.002301, 0.056036,
0.001519, -0.044964, -0.021701, -0.020386, 0.000021, 0.004703, 0.009485, 0.004002, -0.008169, -0.011849,
-0.015115, -0.003128, -0.008734, -0.032289, -0.01836, 0.017998, 0.012092, -0.017268, -0.039712, -0.032111,
0.001079, -0.003001, -0.041662, -0.02911, -0.004457, -0.00505, -0.001392, -0.014631, -0.013304, 0.001495,
-0.030842, -0.060879, -0.009599, -0.000717, -0.008669, 0.01827, -0.017788, -0.046984, -0.031604, -0.005797,
0.005221, 0.012372, -0.020347, -0.014822, 0.039497, 0.006405, -0.024596, -0.016878, 0.008174, 0.01278,
-0.009856, -0.022463, -0.026894, -0.006297, 0.019255, 0.012027, -0.027779, -0.024629, 0.004317, -0.003233,
-0.012163, -0.019413, -0.026393, 0.028747, 0.033529, -0.025853, -0.013393, 0.007235, -0.000966, 0.000167,
0.011409, -0.036683, -0.030916, 0.034173, 0.012444, -0.014733, -0.041278, -0.045711, -0.019767, -0.008373,
-0.033265, -0.045866, -0.022939, -0.036459, -0.00913, 0.015019, -0.031064, -0.06889, -0.03575, 0.025831,
0.001542, -0.045861, -0.039459, -0.015774, -0.00875, -0.008127, -0.011817, -0.032219, -0.039426, -0.023647,
-0.024105, -0.012764, 0.003725, -0.04749, -0.050225, -0.009079, -0.018055, -0.02484, -0.028507, -0.048146,
-0.042338, 0.027453, 0.020278, -0.036571, -0.026005, -0.01284, -0.010745, -0.031559, -0.032905, -0.014251,
-0.010503, 0.009324, 0.00508, -0.027077, -0.017394, -0.011904, -0.016804, 0.005502, -0.005515, -0.040926,
-0.009648, 0.036622, -0.001723, -0.004953, 0.007987, -0.006849, 0.001204, 0.000977, 0.005738, 0.004342,
0.011192, 0.003369, 0.001923, 0.024521, 0.009518, -0.010791, -0.003454, 0.000963, -0.009699, 0.007309,
0.009188, -0.025888, -0.001081, 0.004814, -0.02124, 0.005315, -0.005595, -0.029872, -0.018864, -0.013664,
-0.024965, 0.003616, 0.014816, -0.014297, 0.006974, 0.00475, 0.004441, 0.011507, 0.002143, 0.011048,
0.028822, 0.024381, 0.009158, 0.031154, 0.027375, 0.017913, 0.017197, 0.016917, 0.022529, 0.021561,
0.024612, 0.018004, 0.02317, 0.02152, 0.011998, 0.015835, 0.017331, 0.010545, 0.007908, 0.018821,
0.0219, 0.005793, 0.006026, 0.029162, 0.027171, 0.019404, 0.029103, 0.023499, 0.009372, 0.022385,
0.029206, 0.017567, 0.035988, 0.018019, 0.001715, 0.037309, 0.032395, 0.018368, 0.020172, 0.0103,
0.004361, 0.031257, 0.023547, 0.006403, 0.017671, -0.003657, 0.01104, 0.028051, 0.011431, 0.00033,
0.006176, 0.010693, 0.018211, 0.016162, -0.008674, 0.009386, 0.014121, 0.001879, 0.002181, 0.003607,
-0.002846, 0.003536, 0.018501, -0.006526, -0.004088, 0.010643, 0.006564, -0.000405, 0.002575, 0.008013,
-0.00047, 0.015509, 0.01458, -0.000152, 0.00704, 0.004182, 0.007467, 0.018708, 0.012788, 0.002966,
0.014526, 0.020167, 0.003742, 0.001803, 0.005917, 0.011886, 0.010243, 0.000779, -0.000218, -0.003043,
0.002513, 0.009012, 0.001235, -0.001338, 0.001884, -0.005609, -0.006355, -0.001787, 0.004534, 0.012036,
0.003572, -0.002395, 0.007252, 0.006129, 0.00426, 0.004743, -0.004764, 0.004368, 0.020658, 0.017623,
0.008316, 0.007544, -0.000398, 0.003532, 0.010332, 0.00217, 0.005599, 0.003485, -0.005075, -0.004851,
0.011789, 0.009231, -0.00854, -0.003451, -0.000546, -0.002802, -0.004037, -0.004333, -0.002863, 0.001656,
0.000675, -0.007004, -0.005412, -0.011058, -0.014858, -0.003537, 0.000667, -0.000838, -0.002791, -0.006385,
-0.006747, -0.004496, -0.009757, -0.01477, -0.013672, -0.007225, 0.004785, 0.004075, -0.002832, -0.004379,
-0.00565, -0.000876, 0.004073, -0.004029, -0.003906, -0.005479, -0.013744, -0.005732, -0.000107, -0.009335,
-0.01837, -0.01367, -0.007844, -0.001748, -0.006773, -0.01503, -0.01284, -0.011871, -0.007713, -0.008093,
-0.006902, -0.004863, -0.004971, 0.002908, 0.005497, -0.004997, -0.000499, 0.010374, 0.004361, 0.003086,
0.012751, 0.012372, 0.013481, 0.014349, 0.00573, 0.012512, 0.011189, 0.010261, 0.019827, 0.013418,
0.005494, 0.012315, 0.011878, 0.009876, 0.011375, -0.003319, 0.005489, 0.015897, 0.005846, 0.005034,
0.010607, -0.000911, -0.002888, 0.010546, 0.005232, 0.00949, 0.006986, 0.005732, 0.013327, 0.009714,
0.005377, 0.006569, 0.003019, -0.001553, 0.008236, 0.007083, 0.008714, 0.011085, 0.00283, -0.00413,
-0.001461, -0.004309, -0.011524, 0.001866, 0.000616, -0.003211, 0.003616, 0.004969, 0.004979, 0.006196,
0.000214, -0.007467, 0.002412, -0.002072, 0.000384, 0.012405, 0.003242, 0.003047, 0.011896, 0.006319,
-0.007178, 0.000679, 0.004477, 0.007942, 0.016619, 0.010226, 0.01696, 0.023257, 0.020476, 0.013646,
0.011921, 0.019866, 0.017917, 0.01066, 0.01671, 0.021882, 0.013585, 0.011095, 0.010325, 0.011216,
0.018222, 0.011512, 0.001633, 0.015046, 0.01589, 0.006953, 0.007385, 0.006232, 0.01371, 0.014748,
0.006623, 0.009802, 0.015129, 0.002284, 0.004412, 0.011587, 0.008683, 0.009249, 0.004428, 0.005628,
0.012555, 0.016551, 0.009078, 0.007125, 0.011577, 0.017327, 0.017547, 0.010857, 0.013461, 0.015061,
0.010975, 0.005152, 0.005645, 0.010102, 0.012441, 0.008792, 0.008133, 0.009762, 0.005188, 0.001064,
-0.000304, -0.002319, -0.005875, -0.002578, 0.001615, 0.002171, -0.000359, -0.003616, -0.005199, -0.004843,
-0.006357, -0.012158, -0.004434, 0.003191, -0.002376, -0.001207, 0.001595, 0.002383, 0.004998, 0.001052,
-0.005439, 0.001668, 0.004903, -0.00014, 0.006464, 0.003757, 0.002778, 0.005982, -0.003734, -0.009285,
0.000185, -0.002414, -0.014707, -0.005636, -0.008034, -0.011571, -0.009559, -0.013391, -0.01277, -0.013614,
-0.016085, -0.021041, -0.015905, -0.014102, -0.013591, -0.012472, -0.015977, -0.016033, -0.015509, -0.013647,
-0.012928, -0.010163, -0.013858, -0.014271, -0.007814, -0.009248, -0.009437, -0.006655, -0.007013, -0.010663,
-0.010712, -0.008069, -0.006317, -0.007843, -0.011034, -0.006113, -0.004866, -0.00823, -0.006032, -0.002713,
-0.003615, -0.00554, -0.006263, -0.006223, -0.004165, -0.006533, -0.005726, -0.004708, -0.007386, -0.005852,
-0.00602, -0.008298, -0.004628, -0.002341, -0.002595, 0.000589, -0.003154, -0.00359, 0.001173, -0.000572,
-0.002566, 0.001276, 0.000483, 0.000461, 0.006038, 0.004566, 0.003835, 0.001768, 0.001944, 0.00589,
0.004566, 0.002487, 0.004115, 0.004971, 0.005484, 0.005949, 0.002271, 0.001167, 0.00021, -0.000098,
0.000945, -0.000425, -0.000553, 0.000072, -0.00171, -0.000553, -0.000488, -0.004518, -0.004345, -0.003003,
-0.002214, -0.001236, -0.003352, -0.004384, -0.002169, -0.003968, -0.002213, -0.001568, -0.006854, -0.003942,
0.000268, -0.004989, -0.002373, 0.001119, -0.005725, -0.001461, -0.001144, -0.003709, -0.000077, -0.001734,
-0.006687, -0.003684, -0.001312, -0.006289, -0.002888, -0.005497, -0.005987, -0.003707, -0.005539, -0.004072,
-0.004101, -0.005431, -0.004807, -0.003205, -0.006828, -0.003352, -0.001117, -0.002367, -0.000877, 0.000093,
-0.001157, -0.002743, 0.00158, 0.000894, -0.000077, 0.000791, 0.001425, 0.002087, 0.003447, 0.004996,
0.000965, 0.001271, 0.004356, 0.002076, 0.001428, 0.003041, 0.002117, 0.002188, 0.003412, 0.002813,
0.000553, 0.001728, 0.002743, 0.000989, 0.002305, 0.002112, 0.001675, 0.001583, 0.000387, 0.000711,
0.002531, 0.001535, -0.000936, 0.000554, 0.000371, 0.001735, 0.002816, 0.000245, 0.00036, 0.002783,
0.002464, 0.001636, 0.003731, 0.001732
};

//R1.00 ACOUSTIC HIGH
float TIR8B[1024] = {
0, 0.00002, -0.002014, -0.001552, -0.000384, -0.002204, 0.003583, 0.005137, -0.002251, -0.000042,
-0.011499, -0.0129, 0.011161, -0.009121, 0.001212, 0.017345, -0.036456, -0.024735, 0.020537, -0.034374,
-0.001664, 0.129386, -0.007159, -0.140613, -0.082817, -0.020282, 0.031122, -0.063771, -0.099422, 0.079344,
0.195334, -0.069749, -0.384403, -0.07182, 0.490255, 0.298162, -0.346353, -0.308033, 0.26623, 0.197032,
-0.442042, -0.254844, 0.476198, 0.546626, -0.051182, -0.535587, -0.242972, 0.287428, 0.189826, -0.30365,
0.040578, 0.678293, 0.068218, -0.53705, -0.332731, -0.091213, 0.094842, 0.020618, -0.205016, -0.072606,
0.380491, 0.214122, -0.236952, -0.223118, -0.147207, 0.174404, 0.299427, 0.052469, -0.131047, -0.110102,
0.027533, -0.120273, -0.216426, -0.035825, 0.023126, 0.049578, 0.358704, 0.375621, -0.143987, -0.484807,
-0.350127, -0.000068, 0.242162, -0.051759, -0.120433, 0.182054, -0.075311, -0.109848, 0.407599, 0.320779,
-0.569641, -0.453654, 0.156134, 0.157258, 0.164233, -0.148174, -0.096692, 0.199173, 0.204999, 0.065202,
-0.157928, -0.237681, -0.038952, 0.155593, -0.085085, -0.132771, 0.001601, 0.15148, 0.126678, -0.093441,
-0.014748, 0.101774, -0.024307, -0.083561, -0.000494, -0.03473, -0.008595, -0.012509, -0.01643, 0.019013,
-0.096481, -0.214867, -0.01145, 0.229476, 0.118663, -0.04526, -0.069175, 0.102375, 0.124536, -0.154523,
-0.330684, -0.134143, 0.137441, 0.180989, 0.078086, -0.068313, -0.026547, 0.071155, 0.019621, -0.03125,
0.065708, -0.0162, 0.025973, 0.38365, 0.081349, -0.301615, -0.102963, 0.095906, -0.035982, -0.152535,
0.007173, 0.067608, 0.19669, 0.011867, -0.29047, -0.1166, 0.04263, 0.044628, -0.053868, -0.009501,
0.114351, 0.093056, 0.024775, 0.039829, 0.002313, -0.053259, 0.021934, 0.029107, 0.068824, 0.151101,
-0.038452, -0.274239, -0.010925, 0.180228, 0.037371, -0.061918, -0.066755, 0.110035, 0.183978, 0.08383,
-0.0693, -0.118124, -0.003721, -0.010378, -0.204086, -0.12592, 0.171348, 0.15228, -0.051974, -0.020499,
0.097798, 0.007713, 0.014695, -0.018166, -0.013921, 0.183249, 0.15362, -0.011673, -0.023145, 0.1157,
-0.006257, -0.078069, 0.06857, -0.032549, -0.023639, 0.049477, -0.051631, -0.113646, -0.037214, 0.060824,
0.038837, 0.017025, -0.036566, 0.019833, 0.116017, 0.048266, -0.024888, -0.036323, 0.054779, 0.076841,
-0.070574, -0.139369, -0.043577, 0.026855, 0.079992, 0.047229, -0.029834, 0.021497, 0.031167, -0.024889,
0.032363, 0.031689, -0.081314, 0.05605, 0.07863, -0.033867, 0.010037, 0.02264, 0.019502, 0.045994,
0.06994, -0.053275, -0.029818, 0.033713, -0.033019, 0.024957, 0.084276, 0.070133, 0.005177, 0.025811,
0.008885, -0.073516, -0.043501, -0.038356, -0.04067, -0.016777, 0.001518, 0.096179, 0.163815, 0.011725,
-0.134133, 0.007426, 0.083546, 0.034632, 0.035787, -0.042675, -0.050774, 0.020248, 0.100415, 0.128885,
0.004022, -0.090052, 0.016017, 0.089824, 0.014699, -0.016856, -0.019077, 0.018057, 0.025537, -0.021508,
0.002795, 0.054117, -0.005401, -0.08405, -0.082002, -0.066938, -0.03241, -0.038686, -0.03358, 0.019466,
0.063238, 0.065795, 0.02039, 0.001824, 0.046891, 0.065023, 0.03208, 0.019873, -0.0211, -0.068578,
-0.006836, 0.059167, 0.0268, 0.023587, -0.009339, -0.026256, 0.032827, 0.045217, 0.029182, 0.016904,
0.008363, 0.003409, 0.076903, 0.081215, -0.009498, -0.032361, 0.006816, 0.059475, 0.044662, 0.009584,
0.006638, 0.000917, -0.077972, -0.122884, -0.040001, 0.019925, -0.001063, -0.032864, -0.005083, 0.051312,
0.04033, -0.061914, -0.053423, 0.070769, 0.048005, 0.02175, 0.072198, 0.046392, -0.028129, -0.02162,
0.008537, -0.01518, 0.036938, 0.006688, -0.020906, 0.064868, 0.043687, -0.020665, -0.010851, 0.053914,
0.005844, -0.064707, -0.060171, -0.056854, -0.020595, 0.007951, 0.033695, 0.040446, 0.027412, 0.014184,
-0.000741, 0.004221, -0.004736, -0.037781, -0.030092, 0.019526, 0.032229, 0.004078, -0.033417, -0.038931,
-0.000101, 0.010052, -0.030063, -0.024503, 0.007661, 0.019363, 0.030385, 0.016027, 0.009659, 0.020705,
-0.021275, -0.074743, -0.029109, -0.002155, 0.005116, 0.047189, 0.015495, -0.032512, -0.034374, -0.006902,
0.016994, 0.036946, 0.002631, -0.003557, 0.056919, 0.029852, -0.019388, -0.032109, -0.010264, 0.002574,
-0.018121, -0.037561, -0.048074, -0.022538, 0.022681, 0.033893, -0.009231, -0.020737, 0.00674, 0.003842,
-0.007306, -0.020865, -0.035006, 0.03032, 0.057749, -0.005604, -0.012471, 0.003054, -0.005376, -0.006329,
0.007086, -0.049233, -0.058024, 0.021347, 0.023367, -0.00258, -0.044395, -0.065863, -0.040414, -0.013532,
-0.028015, -0.041355, -0.013442, -0.019062, 0.017835, 0.060355, 0.014932, -0.051077, -0.039319, 0.03555,
0.029711, -0.028334, -0.041995, -0.022046, -0.006838, 0.002369, 0.002993, -0.022433, -0.040844, -0.028539,
-0.024602, -0.005393, 0.024207, -0.029538, -0.049812, -0.008459, -0.007013, -0.011155, -0.01652, -0.043782,
-0.04537, 0.042208, 0.062353, -0.000124, -0.012144, -0.010703, -0.013407, -0.042459, -0.053375, -0.032489,
-0.017256, 0.01856, 0.02796, -0.008825, -0.011815, -0.012347, -0.021666, 0.002486, -0.004581, -0.050559,
-0.026167, 0.038531, 0.013418, 0.004023, 0.012055, -0.00941, -0.009428, -0.013429, -0.009738, -0.010459,
-0.00131, -0.007869, -0.011146, 0.015865, 0.004869, -0.023858, -0.025667, -0.022405, -0.033032, -0.011464,
0.000722, -0.035625, -0.013247, 0.000869, -0.024661, 0.002738, -0.003285, -0.034223, -0.030316, -0.023088,
-0.032144, 0.004869, 0.032924, 0.007587, 0.02536, 0.021899, 0.015768, 0.016901, -0.000224, 0.00132,
0.020258, 0.018609, -0.002228, 0.016336, 0.013995, 0.000472, -0.006462, -0.010732, -0.005022, -0.003145,
0.003567, -0.00086, 0.005753, 0.006151, -0.005113, -0.003653, -0.001453, -0.008216, -0.012113, 0.001852,
0.011962, -0.002796, -0.005883, 0.021837, 0.028139, 0.021189, 0.029306, 0.021001, -0.0026, 0.003029,
0.010787, -0.000942, 0.019871, 0.00389, -0.020034, 0.017312, 0.023179, 0.010874, 0.009802, -0.004328,
-0.016462, 0.014078, 0.015613, -0.001744, 0.008189, -0.015877, -0.003131, 0.023149, 0.013373, -0.001139,
0.001109, 0.006801, 0.018956, 0.021207, -0.008912, 0.003832, 0.012095, 0.000719, -0.001082, 0.000417,
-0.006102, 0.001662, 0.024468, 0.00185, -0.000647, 0.015659, 0.01542, 0.007282, 0.007761, 0.013469,
0.003973, 0.020973, 0.024047, 0.006275, 0.008312, 0.002923, 0.004888, 0.019575, 0.017129, 0.004511,
0.014153, 0.022775, 0.004547, -0.004052, -0.003069, 0.004956, 0.007075, -0.002116, -0.004869, -0.008493,
-0.000965, 0.011628, 0.00812, 0.005433, 0.008518, -0.000405, -0.004168, 0.00017, 0.010101, 0.023821,
0.018071, 0.008206, 0.014837, 0.012926, 0.008702, 0.00654, -0.007339, -0.00054, 0.022057, 0.026591,
0.017309, 0.011878, -0.002764, -0.004397, 0.002633, -0.004504, -0.000408, -0.000438, -0.009313, -0.010073,
0.01217, 0.017874, -0.000504, 0.000178, 0.002425, 0.000235, -0.001486, -0.00194, 0.000427, 0.007701,
0.009534, 0.000919, -0.000098, -0.00841, -0.015689, -0.003063, 0.007641, 0.011517, 0.011464, 0.006374,
0.002791, 0.002693, -0.005239, -0.014491, -0.016026, -0.007391, 0.012631, 0.020373, 0.015415, 0.010588,
0.004363, 0.005434, 0.009661, -0.000574, -0.004456, -0.008777, -0.020486, -0.012943, -0.001805, -0.006909,
-0.017186, -0.013182, -0.003452, 0.009327, 0.008657, -0.001537, -0.003339, -0.004637, -0.000701, -0.000302,
0.001344, 0.004075, 0.00421, 0.013335, 0.018031, 0.004667, 0.00375, 0.013906, 0.007428, 0.002545,
0.011135, 0.011614, 0.012246, 0.011943, -0.000829, 0.002061, -0.00035, -0.002278, 0.009449, 0.005464,
-0.004896, 0.000257, 0.001029, -0.000304, 0.00198, -0.0152, -0.008443, 0.007404, 0.001922, 0.001369,
0.008421, -0.003888, -0.009926, 0.004716, 0.003098, 0.009626, 0.008714, 0.006767, 0.014819, 0.011564,
0.004172, 0.001796, -0.004775, -0.012821, -0.00243, 0.000727, 0.006324, 0.012652, 0.004961, -0.00627,
-0.007338, -0.011611, -0.020755, -0.004656, 0.001222, 0.00176, 0.012452, 0.017856, 0.019239, 0.01942,
0.009078, -0.006915, -0.0023, -0.008122, -0.005935, 0.011052, 0.006386, 0.006286, 0.016915, 0.01231,
-0.00695, -0.004735, -0.000818, 0.005505, 0.019599, 0.016575, 0.023918, 0.031839, 0.028084, 0.014835,
0.004236, 0.007073, 0.003394, -0.006924, -0.001943, 0.007165, 0.001802, -0.000817, -0.001563, 0.000533,
0.011504, 0.00793, -0.004319, 0.008891, 0.013875, 0.005315, 0.003456, 0.000772, 0.009436, 0.013849,
0.005809, 0.007423, 0.013898, -0.000433, -0.002038, 0.005987, 0.005607, 0.007677, 0.002955, 0.003535,
0.012616, 0.020901, 0.014416, 0.009364, 0.011601, 0.018052, 0.01952, 0.01078, 0.010039, 0.010176,
0.004387, -0.004847, -0.006855, -0.000851, 0.006174, 0.006471, 0.007848, 0.011296, 0.007033, 0.000669,
-0.003281, -0.006894, -0.011408, -0.00675, 0.002416, 0.008877, 0.010053, 0.007314, 0.004221, 0.002915,
-0.000036, -0.008407, -0.00097, 0.011556, 0.009913, 0.011821, 0.015134, 0.015859, 0.017719, 0.011184,
-0.001222, 0.001696, 0.005219, 0.000215, 0.007594, 0.006617, 0.005654, 0.009238, -0.002263, -0.012944,
-0.004655, -0.004611, -0.017695, -0.008194, -0.006557, -0.007505, -0.003003, -0.004825, -0.003159, -0.002946,
-0.005316, -0.011836, -0.006976, -0.002676, 0.000525, 0.003734, 0.000372, -0.001288, -0.002274, -0.001163,
-0.000584, 0.002608, -0.00164, -0.004092, 0.002298, 0.001739, 0.001007, 0.003368, 0.002543, -0.003356,
-0.006429, -0.005139, -0.002918, -0.00396, -0.00805, -0.003076, -0.000004, -0.002931, -0.001115, 0.003017,
0.002631, -0.00059, -0.003637, -0.005722, -0.004551, -0.007442, -0.007112, -0.005514, -0.007885, -0.006098,
-0.005393, -0.007514, -0.00316, 0.001222, 0.0025, 0.006659, 0.00224, -0.000959, 0.002263, -0.000387,
-0.004555, -0.001896, -0.002702, -0.002982, 0.003874, 0.004012, 0.003048, -0.000693, -0.002662, 0.000768,
-0.00031, -0.003281, -0.002343, -0.001121, 0.000117, 0.001306, -0.002886, -0.005618, -0.007849, -0.008625,
-0.006751, -0.006707, -0.005467, -0.003121, -0.003619, -0.001513, -0.000387, -0.004827, -0.005902, -0.004663,
-0.002975, -0.000655, -0.002134, -0.003637, -0.001393, -0.003034, -0.00118, 0.000139, -0.006077, -0.00437,
0.001191, -0.003416, -0.001159, 0.00364, -0.003567, -0.000541, 0.000037, -0.003143, 0.000261, -0.001111,
-0.007746, -0.006151, -0.002753, -0.007312, -0.003636, -0.005434, -0.005872, -0.002717, -0.00361, -0.001539,
-0.000837, -0.002251, -0.002098, -0.000399, -0.004714, -0.001849, 0.001453, 0.000889, 0.002409, 0.003448,
0.001447, -0.002062, 0.001315, 0.000683, -0.001025, -0.000911, -0.000507, 0.000237, 0.002028, 0.004336,
-0.000346, -0.001881, 0.000766, -0.001629, -0.003127, -0.001638, -0.002219, -0.001922, 0.000005, 0.000137,
-0.002414, -0.001666, -0.000203, -0.00176, -0.000351, -0.00011, -0.000419, -0.000577, -0.00214, -0.002176,
0.000178, -0.000114, -0.003007, -0.001921, -0.001782, 0.000329, 0.002639, 0.000341, -0.00023, 0.002295,
0.002439, 0.001336, 0.003312, 0.000972
};


//R1.00 412 Low
float TIR9[1024] = {
0, 0, 0.000068, 0.000383, 0.000811, 0.001185, 0.001172, 0.001074, -0.00003, -0.004228,
-0.012295, -0.021994, -0.032898, -0.039748, -0.040625, -0.036305, -0.024878, 0.018601, 0.144841, 0.310651,
0.426983, 0.469581, 0.434409, 0.348206, 0.235824, 0.139472, 0.093849, 0.104302, 0.145069, 0.168481,
0.156188, 0.102119, 0.033426, -0.032625, -0.074199, -0.092477, -0.087778, -0.073009, -0.056745, -0.049475,
-0.051669, -0.059428, -0.069393, -0.077235, -0.083159, -0.083608, -0.080331, -0.071372, -0.063639, -0.056863,
-0.057569, -0.062152, -0.073684, -0.084468, -0.095287, -0.09947, -0.099173, -0.093407, -0.087634, -0.080867,
-0.078325, -0.077229, -0.078968, -0.08057, -0.083586, -0.085385, -0.088246, -0.091355, -0.095236, -0.099111,
-0.103798, -0.107949, -0.11158, -0.114213, -0.114908, -0.113805, -0.109617, -0.10402, -0.09645, -0.088614,
-0.08099, -0.074677, -0.069297, -0.064805, -0.05998, -0.055468, -0.050286, -0.04593, -0.04167, -0.039176,
-0.037746, -0.037777, -0.038213, -0.039504, -0.040339, -0.041287, -0.041228, -0.04073, -0.039279, -0.03809,
-0.036022, -0.033292, -0.030138, -0.026814, -0.02296, -0.019598, -0.015964, -0.013076, -0.010738, -0.009115,
-0.008105, -0.007446, -0.007021, -0.007166, -0.007178, -0.007706, -0.008005, -0.008354, -0.008097, -0.007719,
-0.006765, -0.00525, -0.003484, -0.000912, 0.00151, 0.004403, 0.00712, 0.010108, 0.01319, 0.016553,
0.019847, 0.023016, 0.026222, 0.02925, 0.032205, 0.034979, 0.037592, 0.039886, 0.041536, 0.042956,
0.043702, 0.044147, 0.043892, 0.04315, 0.040977, 0.038669, 0.035965, 0.032703, 0.029277, 0.025006,
0.021106, 0.017162, 0.013334, 0.01036, 0.007565, 0.005023, 0.003196, 0.001917, 0.001171, 0.001109,
0.001079, 0.001797, 0.002588, 0.004132, 0.005202, 0.007079, 0.008344, 0.0096, 0.011094, 0.012152,
0.013362, 0.014442, 0.015151, 0.015845, 0.016566, 0.017036, 0.017478, 0.017668, 0.018385, 0.018854,
0.019373, 0.019331, 0.019646, 0.019625, 0.019649, 0.019604, 0.01985, 0.019505, 0.019629, 0.019244,
0.019248, 0.01899, 0.018427, 0.018334, 0.017953, 0.017622, 0.017491, 0.017391, 0.017286, 0.016887,
0.017064, 0.016959, 0.017126, 0.017143, 0.017348, 0.017625, 0.018133, 0.018581, 0.019195, 0.019739,
0.020224, 0.020944, 0.021218, 0.021818, 0.022511, 0.022784, 0.023182, 0.023583, 0.023812, 0.023824,
0.02371, 0.023269, 0.022716, 0.021956, 0.021351, 0.020339, 0.019139, 0.017754, 0.016754, 0.01511,
0.013555, 0.012062, 0.010401, 0.008947, 0.007929, 0.006801, 0.005857, 0.004921, 0.003761, 0.003234,
0.002465, 0.001914, 0.001641, 0.001354, 0.001188, 0.001197, 0.001371, 0.001374, 0.001226, 0.001269,
0.00154, 0.001675, 0.002046, 0.002507, 0.002738, 0.003273, 0.0041, 0.00453, 0.005026, 0.005741,
0.00635, 0.007373, 0.008358, 0.009614, 0.010919, 0.01184, 0.013272, 0.014799, 0.016342, 0.018291,
0.019924, 0.021261, 0.023231, 0.024704, 0.025738, 0.025441, 0.024314, 0.022566, 0.020362, 0.018787,
0.017817, 0.01722, 0.016661, 0.016466, 0.015818, 0.015059, 0.013858, 0.012994, 0.012433, 0.012151,
0.012236, 0.012503, 0.012727, 0.012876, 0.012848, 0.012417, 0.01231, 0.011885, 0.011924, 0.011545,
0.011457, 0.011269, 0.011465, 0.010887, 0.010416, 0.009842, 0.009256, 0.008899, 0.008369, 0.007899,
0.007471, 0.007053, 0.006848, 0.006525, 0.005836, 0.005427, 0.004609, 0.003909, 0.003695, 0.003046,
0.002783, 0.002363, 0.002045, 0.001408, 0.001516, 0.000806, 0.000745, 0.000461, 0.000574, 0.000651,
0.000914, 0.000679, 0.000996, 0.000726, 0.000767, 0.000613, 0.000634, 0.0004, 0.000616, 0.000293,
0.000247, -0.000248, -0.000537, -0.00102, -0.00118, -0.001648, -0.001673, -0.002159, -0.002365, -0.002692,
-0.002921, -0.003154, -0.003304, -0.003529, -0.00387, -0.003961, -0.003889, -0.00375, -0.00391, -0.00396,
-0.004027, -0.004038, -0.004098, -0.004193, -0.004052, -0.004204, -0.004159, -0.003957, -0.003716, -0.003748,
-0.003753, -0.003753, -0.003538, -0.003538, -0.003278, -0.003227, -0.002997, -0.002878, -0.002643, -0.002592,
-0.002404, -0.002568, -0.002389, -0.002415, -0.002285, -0.002225, -0.002126, -0.002365, -0.002333, -0.00242,
-0.002435, -0.002557, -0.002669, -0.002809, -0.002857, -0.00302, -0.003033, -0.00317, -0.003426, -0.003548,
-0.00355, -0.003688, -0.00381, -0.004016, -0.004156, -0.004229, -0.004336, -0.004383, -0.004595, -0.004751,
-0.004755, -0.004884, -0.004859, -0.005019, -0.005216, -0.005334, -0.005315, -0.005428, -0.005391, -0.00561,
-0.005681, -0.005828, -0.005813, -0.005854, -0.00584, -0.006125, -0.006089, -0.006181, -0.006123, -0.006257,
-0.006272, -0.006461, -0.006425, -0.006513, -0.006451, -0.006512, -0.006621, -0.006758, -0.006721, -0.006669,
-0.00666, -0.006861, -0.006832, -0.006837, -0.006854, -0.006871, -0.006819, -0.006998, -0.007024, -0.006994,
-0.006933, -0.00698, -0.007072, -0.007126, -0.007035, -0.007139, -0.007063, -0.007032, -0.007145, -0.007197,
-0.007097, -0.007123, -0.007065, -0.007165, -0.007142, -0.007135, -0.007133, -0.007065, -0.007025, -0.007057,
-0.007193, -0.007057, -0.006996, -0.006956, -0.007054, -0.007021, -0.007003, -0.006992, -0.006916, -0.006869,
-0.006896, -0.00689, -0.006943, -0.006693, -0.006826, -0.006732, -0.006874, -0.006734, -0.006735, -0.00657,
-0.006627, -0.006559, -0.006728, -0.006483, -0.006515, -0.006372, -0.006478, -0.006437, -0.006337, -0.006272,
-0.006284, -0.006061, -0.006325, -0.006157, -0.006138, -0.00598, -0.005948, -0.005965, -0.006012, -0.005816,
-0.005906, -0.005657, -0.005711, -0.005699, -0.005736, -0.005513, -0.005635, -0.005355, -0.005485, -0.005447,
-0.00541, -0.005311, -0.005245, -0.005118, -0.005223, -0.005106, -0.005094, -0.004997, -0.004925, -0.004928,
-0.004901, -0.004732, -0.004894, -0.00468, -0.004629, -0.004549, -0.004724, -0.004565, -0.004458, -0.004338,
-0.00431, -0.004389, -0.00433, -0.004217, -0.004142, -0.004078, -0.004082, -0.004055, -0.004021, -0.003921,
-0.003784, -0.00382, -0.003802, -0.003768, -0.003665, -0.003594, -0.003439, -0.003572, -0.003512, -0.003375,
-0.003377, -0.003217, -0.003175, -0.00325, -0.00319, -0.00301, -0.003102, -0.002849, -0.002967, -0.002854,
-0.002843, -0.002676, -0.002631, -0.002568, -0.0027, -0.002538, -0.002515, -0.002329, -0.002368, -0.002354,
-0.002323, -0.002222, -0.002221, -0.001994, -0.002052, -0.002106, -0.001973, -0.00189, -0.001959, -0.001701,
-0.001751, -0.001737, -0.001772, -0.001571, -0.001523, -0.001467, -0.001609, -0.001388, -0.001402, -0.001225,
-0.001265, -0.001179, -0.001241, -0.001112, -0.001031, -0.000964, -0.001036, -0.000981, -0.000876, -0.000809,
-0.00082, -0.000666, -0.000696, -0.000678, -0.000646, -0.000613, -0.000446, -0.000478, -0.000532, -0.000409,
-0.000405, -0.00032, -0.000261, -0.000205, -0.000284, -0.000166, -0.00016, -0.000002, -0.000103, -0.00006,
-0.000028, 0.000066, 0.000057, 0.000206, 0.000168, 0.000111, 0.000165, 0.000264, 0.000324, 0.000375,
0.000291, 0.000405, 0.000406, 0.000492, 0.000485, 0.00057, 0.000495, 0.00061, 0.00054, 0.00072,
0.000685, 0.000621, 0.000735, 0.00073, 0.00074, 0.00089, 0.000847, 0.000852, 0.000872, 0.000895,
0.001007, 0.001056, 0.00097, 0.001014, 0.001042, 0.00113, 0.001185, 0.001165, 0.001108, 0.001156,
0.001179, 0.001328, 0.00128, 0.001211, 0.001253, 0.001343, 0.001326, 0.001398, 0.001381, 0.001389,
0.001404, 0.00142, 0.001571, 0.001527, 0.001394, 0.001537, 0.001537, 0.001611, 0.00159, 0.001526,
0.001633, 0.001552, 0.001583, 0.00173, 0.001677, 0.001671, 0.001544, 0.00175, 0.001646, 0.001805,
0.001758, 0.001687, 0.001726, 0.001676, 0.00185, 0.001802, 0.001725, 0.00169, 0.001798, 0.001777,
0.001907, 0.00178, 0.001801, 0.001747, 0.001785, 0.001933, 0.001946, 0.001703, 0.00186, 0.001858,
0.001858, 0.001995, 0.001871, 0.001825, 0.001868, 0.001881, 0.001951, 0.001921, 0.001847, 0.001809,
0.001913, 0.001888, 0.002014, 0.001884, 0.001834, 0.001874, 0.001886, 0.001887, 0.001953, 0.001853,
0.001943, 0.001852, 0.001945, 0.001923, 0.001988, 0.001826, 0.001883, 0.0019, 0.0019, 0.001964,
0.001794, 0.001842, 0.001917, 0.001814, 0.001964, 0.001906, 0.001825, 0.001787, 0.001894, 0.001872,
0.001932, 0.001833, 0.001788, 0.001825, 0.00183, 0.001961, 0.001897, 0.001815, 0.001778, 0.001818,
0.001825, 0.001889, 0.001717, 0.001831, 0.001739, 0.001755, 0.001888, 0.001822, 0.001669, 0.001726,
0.001739, 0.001804, 0.001768, 0.001689, 0.001646, 0.001746, 0.001581, 0.00176, 0.001702, 0.001612,
0.001565, 0.001594, 0.001591, 0.001648, 0.001537, 0.001549, 0.001478, 0.001568, 0.001535, 0.001588,
0.001416, 0.001532, 0.001444, 0.001532, 0.001504, 0.001425, 0.001381, 0.001412, 0.00141, 0.001466,
0.001424, 0.001408, 0.001334, 0.001428, 0.001403, 0.001464, 0.001297, 0.001282, 0.001391, 0.001296,
0.001309, 0.001371, 0.001195, 0.001303, 0.001208, 0.001357, 0.001226, 0.001236, 0.001077, 0.001232,
0.001108, 0.001218, 0.001172, 0.001152, 0.001052, 0.001084, 0.001086, 0.001168, 0.00096, 0.00101,
0.000947, 0.001033, 0.001063, 0.001013, 0.000859, 0.000979, 0.000889, 0.000973, 0.000941, 0.000859,
0.000813, 0.000909, 0.000809, 0.000957, 0.000689, 0.000823, 0.000735, 0.000816, 0.000711, 0.000927,
0.000632, 0.000695, 0.000641, 0.000802, 0.000672, 0.000748, 0.00058, 0.000628, 0.000566, 0.000722,
0.000589, 0.000594, 0.000454, 0.000572, 0.000475, 0.000621, 0.000554, 0.000531, 0.000321, 0.000611,
0.000402, 0.000573, 0.000383, 0.000493, 0.000404, 0.000491, 0.000326, 0.000577, 0.000292, 0.000355,
0.000298, 0.000455, 0.000322, 0.000326, 0.000184, 0.000369, 0.000244, 0.000386, 0.000324, 0.000309,
0.000174, 0.000237, 0.000251, 0.000384, 0.000183, 0.000155, 0.000191, 0.000259, 0.000151, 0.000299,
0.000033, 0.000169, 0.000083, 0.000235, 0.000172, 0.000153, 0.000014, 0.000141, 0.000124, 0.000114,
0.000177, 0.000008, 0.000058, -0.000001, 0.00009, 0.000056, 0.000039, 0.000028, 0.000021, -0.000052,
0.000041, 0.000013, 0.000003, -0.000069, 0.000026, -0.000068, 0.00002, -0.000143, -0.000027, -0.000118,
0.000034, -0.000095, 0.000054, -0.000136, -0.000155, -0.00018, -0.000078, -0.000111, -0.000061, -0.000239,
-0.000126, -0.000218, -0.000133, -0.000096, -0.000273, -0.000225, -0.000216, -0.000219, -0.000159, -0.000263,
-0.000246, -0.000312, -0.000219, -0.000248, -0.000192, -0.000294, -0.000341, -0.000306, -0.000302, -0.00024,
-0.000277, -0.000357, -0.000331, -0.000395, -0.000302, -0.000264, -0.000374, -0.000356, -0.000352, -0.000283,
-0.000313, -0.000318, -0.000452, -0.00039, -0.000378, -0.000383, -0.000257, -0.000393, -0.000449, -0.000416,
-0.00041, -0.000345, -0.000379, -0.000458, -0.00043, -0.000424, -0.000427, -0.000363, -0.000463, -0.000511,
-0.00041, -0.000504, -0.00035, -0.000408, -0.00049, -0.00046, -0.00045, -0.000448, -0.000311, -0.000505,
-0.000457, -0.000441, -0.000438, -0.000439, -0.000374, -0.000541, -0.000491, -0.000413, -0.000377, -0.000486,
-0.000465, -0.000526, -0.000427, -0.000451
};


//R1.00 412 High
float TIR9B[1024] = {
0, 0, 0.000087, 0.000507, 0.001167, 0.001811, 0.001899, 0.001615, -0.000149, -0.006268,
-0.018461, -0.034137, -0.051326, -0.061898, -0.060392, -0.04602, -0.017158, 0.056048, 0.243726, 0.5088,
0.720434, 0.793585, 0.686171, 0.425357, 0.072665, -0.269296, -0.497489, -0.556426, -0.461603, -0.295775,
-0.136395, -0.043722, -0.019272, -0.04504, -0.07937, -0.098758, -0.085847, -0.047138, 0.003919, 0.045688,
0.06653, 0.065245, 0.04646, 0.020271, -0.006493, -0.023782, -0.028885, -0.018179, -0.000685, 0.018974,
0.028443, 0.02689, 0.009642, -0.013702, -0.039247, -0.054848, -0.058102, -0.046603, -0.027863, -0.005089,
0.011598, 0.020992, 0.020191, 0.012787, -0.000704, -0.014954, -0.029731, -0.042745, -0.053547, -0.060964,
-0.06601, -0.067815, -0.066831, -0.063213, -0.056474, -0.047237, -0.034595, -0.020918, -0.006651, 0.005471,
0.013858, 0.016698, 0.014371, 0.007825, -0.000074, -0.00824, -0.014263, -0.018911, -0.021421, -0.023982,
-0.026711, -0.030547, -0.034598, -0.039203, -0.042711, -0.045299, -0.04557, -0.043977, -0.040219, -0.036118,
-0.031147, -0.025784, -0.020623, -0.01627, -0.012486, -0.010418, -0.009267, -0.009894, -0.011998, -0.015441,
-0.019734, -0.024108, -0.027947, -0.031336, -0.033403, -0.034812, -0.034962, -0.034269, -0.032232, -0.029552,
-0.026065, -0.021968, -0.017862, -0.013355, -0.009621, -0.006235, -0.003882, -0.002082, -0.000791, 0.000433,
0.001431, 0.002205, 0.003013, 0.00371, 0.004425, 0.005069, 0.005653, 0.00598, 0.005591, 0.004882,
0.003451, 0.001747, -0.00053, -0.003098, -0.007005, -0.010855, -0.01451, -0.017992, -0.020804, -0.023711,
-0.025488, -0.026436, -0.026522, -0.024998, -0.022688, -0.01994, -0.01654, -0.012811, -0.008978, -0.005008,
-0.001759, 0.001416, 0.003884, 0.006428, 0.00782, 0.009442, 0.00998, 0.009974, 0.009946, 0.009354,
0.008895, 0.008464, 0.007797, 0.007261, 0.00701, 0.006749, 0.006648, 0.006427, 0.006961, 0.007498,
0.008183, 0.008197, 0.008422, 0.008229, 0.007982, 0.007657, 0.007746, 0.007309, 0.00741, 0.007092,
0.007235, 0.007237, 0.006882, 0.007044, 0.007027, 0.007081, 0.007423, 0.007907, 0.008429, 0.008555,
0.009221, 0.009621, 0.010242, 0.010663, 0.011192, 0.011763, 0.012572, 0.013309, 0.014155, 0.014845,
0.015313, 0.015904, 0.015888, 0.016054, 0.016352, 0.016193, 0.016091, 0.016034, 0.015847, 0.015427,
0.014855, 0.013917, 0.012851, 0.011636, 0.010772, 0.00972, 0.008612, 0.007445, 0.006937, 0.005999,
0.005222, 0.004665, 0.00407, 0.003808, 0.004239, 0.004764, 0.005516, 0.006213, 0.006434, 0.007099,
0.007383, 0.007678, 0.008157, 0.008543, 0.008936, 0.009424, 0.010027, 0.010336, 0.010244, 0.010145,
0.010241, 0.010196, 0.0104, 0.01077, 0.010884, 0.011265, 0.012029, 0.012357, 0.012565, 0.0129,
0.013061, 0.013647, 0.014266, 0.015205, 0.016231, 0.016709, 0.017541, 0.01846, 0.019347, 0.020642,
0.021558, 0.021886, 0.022705, 0.022956, 0.02248, 0.020096, 0.016169, 0.011204, 0.005846, 0.001922,
0.000124, 0.000415, 0.002141, 0.005132, 0.007902, 0.010115, 0.011018, 0.011346, 0.011368, 0.011355,
0.011652, 0.012224, 0.012811, 0.01327, 0.01339, 0.01281, 0.012358, 0.011527, 0.011249, 0.01071,
0.010598, 0.010573, 0.011142, 0.010945, 0.010646, 0.010111, 0.009482, 0.009141, 0.008734, 0.008468,
0.008353, 0.008343, 0.008653, 0.008902, 0.00864, 0.008502, 0.007811, 0.007091, 0.006973, 0.006552,
0.006608, 0.006627, 0.006784, 0.006548, 0.007085, 0.006756, 0.006955, 0.006933, 0.007318, 0.007721,
0.008315, 0.008248, 0.008577, 0.008167, 0.00788, 0.007328, 0.006942, 0.00633, 0.006291, 0.005809,
0.005625, 0.004985, 0.004516, 0.003879, 0.003666, 0.003253, 0.003407, 0.003171, 0.003197, 0.003107,
0.003089, 0.003049, 0.003076, 0.00299, 0.002692, 0.002607, 0.002769, 0.003092, 0.003072, 0.003036,
0.002903, 0.00277, 0.002553, 0.00226, 0.002244, 0.001957, 0.001862, 0.002022, 0.002321, 0.002316,
0.002215, 0.002031, 0.002062, 0.001879, 0.001969, 0.001869, 0.001947, 0.001924, 0.002014, 0.001889,
0.001864, 0.001407, 0.00126, 0.000954, 0.000852, 0.000757, 0.000754, 0.000374, 0.000235, 0.000019,
-0.000091, -0.000282, -0.000458, -0.00065, -0.000713, -0.000865, -0.000835, -0.000904, -0.001147, -0.001284,
-0.001244, -0.001304, -0.001367, -0.001548, -0.001689, -0.001737, -0.001791, -0.001763, -0.001921, -0.002071,
-0.002042, -0.002113, -0.002014, -0.002109, -0.002317, -0.002481, -0.002465, -0.002547, -0.002457, -0.002646,
-0.002737, -0.002912, -0.002905, -0.002909, -0.002836, -0.003124, -0.00313, -0.003231, -0.003157, -0.003273,
-0.003298, -0.003528, -0.00354, -0.003646, -0.003575, -0.003608, -0.003731, -0.003933, -0.003948, -0.003882,
-0.003833, -0.004055, -0.004086, -0.00412, -0.004158, -0.004198, -0.004153, -0.00438, -0.004499, -0.004524,
-0.004469, -0.004511, -0.00464, -0.004761, -0.004701, -0.004827, -0.004771, -0.004728, -0.004866, -0.004994,
-0.004939, -0.004983, -0.00494, -0.005078, -0.005118, -0.005158, -0.005198, -0.005152, -0.005114, -0.00517,
-0.005396, -0.00534, -0.005294, -0.005256, -0.005399, -0.00544, -0.00548, -0.005518, -0.005468, -0.005427,
-0.005479, -0.005526, -0.005656, -0.005428, -0.005573, -0.00553, -0.005752, -0.00569, -0.005724, -0.005555,
-0.005607, -0.005568, -0.005821, -0.005642, -0.005687, -0.005554, -0.005696, -0.005734, -0.005681, -0.005636,
-0.005681, -0.005461, -0.005784, -0.005723, -0.005754, -0.00561, -0.005567, -0.005615, -0.005742, -0.005594,
-0.00572, -0.005486, -0.005536, -0.005578, -0.005699, -0.005518, -0.005672, -0.005402, -0.005537, -0.005567,
-0.005591, -0.005524, -0.005464, -0.005322, -0.005451, -0.005391, -0.005421, -0.005358, -0.005301, -0.005332,
-0.005357, -0.005204, -0.00541, -0.005244, -0.005181, -0.005094, -0.005329, -0.005245, -0.00514, -0.004987,
-0.004934, -0.005056, -0.005073, -0.004998, -0.004929, -0.004863, -0.004888, -0.004906, -0.004918, -0.004839,
-0.004679, -0.004705, -0.004725, -0.004737, -0.004658, -0.004584, -0.004401, -0.004549, -0.004561, -0.004452,
-0.004466, -0.004302, -0.004239, -0.00435, -0.004358, -0.004187, -0.00429, -0.004033, -0.004145, -0.004067,
-0.004079, -0.003912, -0.003847, -0.003783, -0.00398, -0.00389, -0.003892, -0.003688, -0.003704, -0.003713,
-0.003716, -0.003628, -0.003631, -0.00337, -0.003394, -0.003495, -0.003406, -0.003322, -0.003417, -0.003149,
-0.003167, -0.003177, -0.003267, -0.003082, -0.003, -0.002924, -0.003108, -0.002921, -0.002923, -0.002719,
-0.002734, -0.002657, -0.002754, -0.002663, -0.002576, -0.002494, -0.002589, -0.002581, -0.002483, -0.002392,
-0.002392, -0.002215, -0.002225, -0.002228, -0.002226, -0.002218, -0.002035, -0.00204, -0.002124, -0.002023,
-0.002014, -0.001914, -0.001823, -0.001736, -0.001827, -0.001729, -0.001724, -0.001543, -0.001636, -0.001626,
-0.001614, -0.001512, -0.001503, -0.001319, -0.001323, -0.001408, -0.001392, -0.001287, -0.00119, -0.001101,
-0.001188, -0.001089, -0.001082, -9.860001E-04, -0.000982, -0.000889, -0.000973, -0.000871, -0.000948, -0.000754,
-0.000751, -0.00083, -0.000723, -0.000711, -0.000696, -0.000506, -0.000507, -0.000504, -0.000496, -0.000485,
-0.000356, -0.000267, -0.000357, -0.000345, -0.000331, -0.000228, -0.000134, -0.000133, -0.000213, -0.000194,
-0.000174, 0.000019, 0.000021, -0.000059, -0.00004, 0.000066, 0.000077, 0.000176, 0.000181, 0.00019,
0.0002, 0.000214, 0.000401, 0.000397, 0.000226, 0.000337, 0.000351, 0.000452, 0.000459, 0.000383,
0.000491, 0.000417, 0.00044, 0.000636, 0.000639, 0.000645, 0.000481, 0.000683, 0.000605, 0.000799,
0.0008, 0.000717, 0.000733, 0.000663, 0.000864, 0.00087, 0.000791, 0.000724, 0.00084, 0.000858,
0.001047, 0.000959, 0.00097, 0.000895, 0.000917, 0.001111, 0.001198, 0.000932, 0.001046, 0.00106,
0.001075, 0.001263, 0.001171, 0.001093, 0.001113, 0.001131, 0.001234, 0.001241, 0.001164, 0.001096,
0.00121, 0.001226, 0.001413, 0.001321, 0.001243, 0.001261, 0.001277, 0.001293, 0.001394, 0.001313,
0.001414, 0.001333, 0.001434, 0.001439, 0.001531, 0.001355, 0.001373, 0.00139, 0.001405, 0.001505,
0.001337, 0.001361, 0.001469, 0.001392, 0.001583, 0.001581, 0.001494, 0.001417, 0.001523, 0.001531,
0.001624, 0.001535, 0.001457, 0.001475, 0.001492, 0.001678, 0.001671, 0.001581, 0.001501, 0.001517,
0.001531, 0.001631, 0.001459, 0.001568, 0.00149, 0.001509, 0.001698, 0.001692, 0.001514, 0.00153,
0.001545, 0.001644, 0.001645, 0.00156, 0.001485, 0.001591, 0.001425, 0.001624, 0.001626, 0.001542,
0.001467, 0.001487, 0.001504, 0.001605, 0.00152, 0.001532, 0.001455, 0.001559, 0.001564, 0.001654,
0.001475, 0.001575, 0.001491, 0.001587, 0.001586, 0.0015, 0.001422, 0.001439, 0.001453, 0.001551,
0.00155, 0.001549, 0.001461, 0.001555, 0.001552, 0.001634, 0.001449, 0.001372, 0.001474, 0.001392,
0.001404, 0.0015, 0.001325, 0.001429, 0.001347, 0.001532, 0.001437, 0.001438, 0.001237, 0.001374,
0.001264, 0.001396, 0.001396, 0.001396, 0.001279, 0.001291, 0.001299, 0.001421, 0.001211, 0.001226,
0.001151, 0.001257, 0.001347, 0.00134, 0.001161, 0.001261, 0.001175, 0.00127, 0.001267, 0.001176,
0.001095, 0.001195, 0.001109, 0.001292, 0.001019, 0.001122, 0.00104, 0.001138, 0.001051, 0.001318,
0.001034, 0.001042, 0.000961, 0.001147, 0.001051, 0.001137, 0.000954, 0.000964, 0.000885, 0.001071,
0.000976, 0.000977, 0.000804, 0.000909, 0.000827, 0.001011, 0.001, 0.000989, 0.000721, 0.000998,
0.000812, 0.00099, 0.000802, 0.000894, 0.0008, 0.00089, 0.000709, 0.000978, 0.000698, 0.000708,
0.000629, 0.000816, 0.000722, 0.000723, 0.00055, 0.000741, 0.000648, 0.000824, 0.000806, 0.000789,
0.000602, 0.000607, 0.000611, 0.000785, 0.000593, 0.000509, 0.00052, 0.000613, 0.000522, 0.000699,
0.000423, 0.000523, 0.000437, 0.000618, 0.000605, 0.000592, 0.000408, 0.000503, 0.000499, 0.000495,
0.000576, 0.000388, 0.000394, 0.000311, 0.000409, 0.000408, 0.000406, 0.000403, 0.000399, 0.000308,
0.0004, 0.000393, 0.000386, 0.000292, 0.000382, 0.000287, 0.000375, 0.000193, 0.000289, 0.000201,
0.00038, 0.000278, 0.000447, 0.00025, 0.000163, 0.000084, 0.000186, 0.000187, 0.000273, 0.00009,
0.000187, 0.000097, 0.00019, 0.00027, 0.000082, 0.000088, 0.000092, 0.000094, 0.000181, 0.000083,
0.000083, -0.000003, 0.00009, 0.000086, 0.000168, 0.000066, -0.000023, -0.000018, -0.000014, 0.000074,
0.000065, -0.00003, -0.000028, -0.000114, -0.000019, 0.000064, -0.000035, -0.000038, -0.000041, 0.000041,
0.000027, 0.000014, -0.00017, -0.00016, -0.000153, -0.000148, 0.000027, -0.000077, -0.000169, -0.000165,
-0.000163, -0.000075, -0.000085, -0.000181, -0.000179, -0.000179, -0.00018, -0.000094, -0.000193, -0.000279,
-0.000185, -0.000274, -0.000096, -0.000112, -0.000212, -0.000215, -0.000218, -0.000221, -0.000052, -0.000247,
-0.000248, -0.000249, -0.00025, -0.000252, -0.000168, -0.000352, -0.000344, -0.000251, -0.000169, -0.00027,
-0.000273, -0.000362, -0.000269, -0.000274
};


//R1.00 Hand EQ'd IR for studio freq slope: Low.
float TIR10[1024] = {
0.001705, 0.000233, -0.003825, -0.011548, -0.021793, -0.031641, -0.03937, -0.041075, -0.037172, -0.030913,
-0.016207, 0.037133, 0.162247, 0.323318, 0.43631, 0.472152, 0.437996, 0.347173, 0.238554, 0.165188,
0.154484, 0.184664, 0.229779, 0.2637, 0.262977, 0.226709, 0.172993, 0.127336, 0.099499, 0.092352,
0.095635, 0.094392, 0.08159, 0.060379, 0.041381, 0.028882, 0.028388, 0.035392, 0.041821, 0.040626,
0.028005, 0.006935, -0.019374, -0.043664, -0.062715, -0.074534, -0.081878, -0.08695, -0.091738, -0.097581,
-0.102421, -0.107393, -0.110191, -0.112445, -0.114111, -0.116099, -0.118989, -0.121401, -0.124323, -0.12643,
-0.129165, -0.132274, -0.136629, -0.142281, -0.148893, -0.155679, -0.161741, -0.166701, -0.16974, -0.171437,
-0.171019, -0.169829, -0.16724, -0.164632, -0.161952, -0.159093, -0.157384, -0.155206, -0.153072, -0.15049,
-0.147354, -0.14362, -0.139776, -0.135711, -0.13217, -0.128841, -0.125468, -0.122263, -0.118458, -0.114764,
-0.110497, -0.106649, -0.102851, -0.099408, -0.096162, -0.093168, -0.090826, -0.088486, -0.086759, -0.085073,
-0.084206, -0.083695, -0.083617, -0.083904, -0.084383, -0.084882, -0.085374, -0.085956, -0.086096, -0.086278,
-0.085772, -0.085211, -0.084139, -0.082695, -0.08143, -0.079826, -0.078143, -0.076303, -0.074059, -0.071412,
-0.068813, -0.066054, -0.063284, -0.060454, -0.057722, -0.054797, -0.051805, -0.048709, -0.045515, -0.042309,
-0.038887, -0.0357, -0.032456, -0.029459, -0.026513, -0.023997, -0.021664, -0.019534, -0.017637, -0.015798,
-0.014106, -0.012265, -0.01095, -0.004973, -0.052088, -0.01884, 0.0314, 0.002329, -0.044158, -0.054539,
-0.055556, -0.070286, -0.041589, -0.009036, -0.014908, -0.037433, -0.02866, -0.013764, -0.013126, -0.005284,
0.002229, 0.012205, 0.010627, -0.006163, -0.023032, -0.013222, -0.002598, 0.00081, 0.009617, 0.022536,
0.035362, 0.034466, 0.036214, 0.039096, 0.039188, 0.037172, 0.0459, 0.067165, 0.082661, 0.074574,
0.056966, 0.051505, 0.051892, 0.055371, 0.054744, 0.064386, 0.07718, 0.073477, 0.063312, 0.062723,
0.063235, 0.054319, 0.046541, 0.047181, 0.054975, 0.055725, 0.058044, 0.065304, 0.067914, 0.063081,
0.05583, 0.056033, 0.062229, 0.068963, 0.073916, 0.082978, 0.087025, 0.084901, 0.083978, 0.083334,
0.084684, 0.089693, 0.095423, 0.09801, 0.100543, 0.099893, 0.096518, 0.094967, 0.095305, 0.095529,
0.098802, 0.104758, 0.10663, 0.103135, 0.096746, 0.092862, 0.08981, 0.088954, 0.089839, 0.084869,
0.075688, 0.070714, 0.07042, 0.068571, 0.066799, 0.068025, 0.069978, 0.06833, 0.063247, 0.056754,
0.051711, 0.049918, 0.049676, 0.051785, 0.055891, 0.059458, 0.058875, 0.052176, 0.043298, 0.040688,
0.042177, 0.041221, 0.037878, 0.031662, 0.026146, 0.02505, 0.028903, 0.032378, 0.029474, 0.024663,
0.022784, 0.019952, 0.015005, 0.011322, 0.008862, 0.00784, 0.007929, 0.008262, 0.007155, 0.005865,
0.007385, 0.007764, 0.003343, -0.000954, -0.001646, -0.000091, 0.00439, 0.011808, 0.019831, 0.021656,
0.015556, 0.007352, 0.002839, 0.005756, 0.014402, 0.022613, 0.024203, 0.021421, 0.020176, 0.021648,
0.022869, 0.020511, 0.014655, 0.011624, 0.016062, 0.02328, 0.027102, 0.027014, 0.025302, 0.020334,
0.014054, 0.011989, 0.015426, 0.020015, 0.021831, 0.022375, 0.022442, 0.019834, 0.014486, 0.011963,
0.013786, 0.016958, 0.019073, 0.019567, 0.01978, 0.018514, 0.016825, 0.017322, 0.016768, 0.014019,
0.012742, 0.013576, 0.015947, 0.016686, 0.014536, 0.012297, 0.011881, 0.012491, 0.012072, 0.012102,
0.012842, 0.012715, 0.010822, 0.009067, 0.009449, 0.011464, 0.013385, 0.013304, 0.014564, 0.019382,
0.019877, 0.009824, -0.000924, 0.000309, 0.009005, 0.01337, 0.015439, 0.017603, 0.014955, 0.009418,
0.005277, 0.005084, 0.007473, 0.01258, 0.021279, 0.025017, 0.020004, 0.014479, 0.011422, 0.008321,
0.008664, 0.014103, 0.018472, 0.016667, 0.013552, 0.012825, 0.009227, 0.004867, 0.006105, 0.010499,
0.013909, 0.013938, 0.00744, -0.00249, -0.005316, 0.001954, 0.012555, 0.020734, 0.020764, 0.010352,
-0.004955, -0.016638, -0.01863, -0.010675, 0.001721, 0.009448, 0.008106, 0.002962, 0.000855, 0.001683,
0.003329, 0.007341, 0.014581, 0.019291, 0.016903, 0.013233, 0.013757, 0.015898, 0.018167, 0.023835,
0.024415, 0.00995, -0.00682, -0.009758, -0.002212, 0.00391, 0.007346, 0.007067, -0.001452, -0.011618,
-0.017033, -0.017363, -0.011968, -0.005562, -0.001474, 0.003025, 0.005142, 0.00149, -0.006056, -0.013365,
-0.01564, -0.013144, -0.008974, -0.004744, -0.005346, -0.01453, -0.025415, -0.03027, -0.028843, -0.02433,
-0.020704, -0.020318, -0.02383, -0.02972, -0.034933, -0.037834, -0.037511, -0.033645, -0.028905, -0.025374,
-0.024307, -0.028289, -0.034523, -0.038593, -0.041083, -0.043103, -0.043958, -0.041957, -0.039605, -0.040606,
-0.041204, -0.038346, -0.036138, -0.038923, -0.044627, -0.046543, -0.04427, -0.044603, -0.048947, -0.05187,
-0.051524, -0.052082, -0.054794, -0.05667, -0.057744, -0.057792, -0.054179, -0.049158, -0.047523, -0.046056,
-0.041245, -0.038636, -0.042442, -0.050157, -0.054826, -0.051436, -0.042778, -0.037376, -0.039493, -0.043342,
-0.045633, -0.048664, -0.051499, -0.05204, -0.051668, -0.050357, -0.047623, -0.046774, -0.04552, -0.042638,
-0.044282, -0.047917, -0.048298, -0.045636, -0.038696, -0.032863, -0.033112, -0.03425, -0.035611, -0.039348,
-0.043642, -0.04487, -0.039612, -0.033164, -0.031406, -0.031255, -0.033042, -0.038477, -0.040383, -0.035638,
-0.030877, -0.028272, -0.027127, -0.030153, -0.034579, -0.034852, -0.031859, -0.030126, -0.0319, -0.034534,
-0.036063, -0.03676, -0.035934, -0.034942, -0.035939, -0.038147, -0.038507, -0.035656, -0.033043, -0.034485,
-0.03685, -0.035763, -0.034398, -0.035901, -0.037688, -0.036582, -0.034574, -0.03594, -0.03798, -0.034804,
-0.027958, -0.023815, -0.02478, -0.027914, -0.0287, -0.025304, -0.019204, -0.012095, -0.006857, -0.006941,
-0.009714, -0.01062, -0.009233, -0.005304, -0.000568, -0.000324, -0.005861, -0.012155, -0.013787, -0.010246,
-0.007821, -0.007727, -0.005799, -0.005188, -0.00769, -0.011718, -0.01526, -0.012629, -0.003602, 0.004101,
0.008079, 0.011623, 0.011164, 0.002901, -0.003546, 0.001902, 0.010837, 0.013226, 0.014553, 0.017645,
0.014753, 0.007765, 0.007135, 0.011928, 0.014584, 0.013587, 0.01419, 0.017524, 0.018429, 0.016256,
0.014302, 0.013139, 0.014214, 0.019475, 0.021054, 0.015411, 0.01156, 0.012617, 0.012921, 0.012667,
0.016724, 0.020792, 0.018438, 0.011631, 0.008019, 0.010075, 0.014623, 0.019226, 0.021153, 0.020847,
0.019166, 0.016567, 0.01579, 0.018105, 0.02096, 0.021316, 0.01999, 0.017591, 0.013236, 0.011481,
0.017037, 0.024016, 0.025522, 0.022, 0.016185, 0.012619, 0.015536, 0.020032, 0.019629, 0.01731,
0.015149, 0.013813, 0.015534, 0.018438, 0.018663, 0.015197, 0.013071, 0.015054, 0.017607, 0.01857,
0.016424, 0.013799, 0.016171, 0.019743, 0.020882, 0.02017, 0.015745, 0.012856, 0.016438, 0.02101,
0.020958, 0.017142, 0.015172, 0.016943, 0.019187, 0.01941, 0.01844, 0.016471, 0.011056, 0.008891,
0.016905, 0.025798, 0.026048, 0.022457, 0.021503, 0.018705, 0.012448, 0.010598, 0.014723, 0.017471,
0.016831, 0.016819, 0.016127, 0.012629, 0.008452, 0.007033, 0.008731, 0.011917, 0.01564, 0.015877,
0.013033, 0.012259, 0.013726, 0.014855, 0.015625, 0.01669, 0.017709, 0.018385, 0.016656, 0.014425,
0.015954, 0.018423, 0.01702, 0.013847, 0.015775, 0.021256, 0.022331, 0.019379, 0.018184, 0.019123,
0.018327, 0.016008, 0.014545, 0.014192, 0.014763, 0.015286, 0.015214, 0.015042, 0.015213, 0.015083,
0.014724, 0.015638, 0.016334, 0.0142, 0.011072, 0.012158, 0.018933, 0.026626, 0.030155, 0.028281,
0.022358, 0.016315, 0.013856, 0.014617, 0.016106, 0.016753, 0.01816, 0.020127, 0.019226, 0.014597,
0.009414, 0.008115, 0.010567, 0.013133, 0.014771, 0.016776, 0.018017, 0.017184, 0.017024, 0.019278,
0.021745, 0.021256, 0.016491, 0.011836, 0.013055, 0.017807, 0.01934, 0.016312, 0.012369, 0.008959,
0.006651, 0.008043, 0.013294, 0.017338, 0.014078, 0.005046, 0.000804, 0.006761, 0.013612, 0.013107,
0.008936, 0.006396, 0.005189, 0.002888, 0.000835, 0.003378, 0.009297, 0.01168, 0.008464, 0.003286,
-0.0007, -0.002042, -0.000018, 0.004201, 0.008317, 0.00935, 0.005739, 0.000072, -0.002679, 0.00206,
0.011055, 0.015095, 0.010331, 0.002011, -0.002961, -0.002191, 0.002613, 0.007364, 0.00847, 0.005169,
0.002527, 0.00231, 0.000484, -0.001311, 0.000936, 0.005158, 0.007031, 0.005054, 0.000579, -0.002399,
-0.002158, 0.0006, 0.003156, 0.003871, 0.004371, 0.003, 0.000337, 0.000111, 0.002559, 0.005127,
0.004841, 0.001286, -0.001664, -0.001071, 0.00107, 0.002302, 0.001694, 0.000563, -0.000689, -0.003508,
-0.006252, -0.006842, -0.006136, -0.004811, -0.003263, -0.004551, -0.008571, -0.011167, -0.011528, -0.010642,
-0.009629, -0.00998, -0.01087, -0.012158, -0.013528, -0.01353, -0.013875, -0.013979, -0.012445, -0.011814,
-0.013247, -0.015008, -0.014369, -0.012267, -0.012379, -0.013694, -0.013208, -0.012439, -0.013163, -0.014027,
-0.0148, -0.016037, -0.016134, -0.014313, -0.012702, -0.014392, -0.018977, -0.020397, -0.018031, -0.016656,
-0.01607, -0.015815, -0.017356, -0.019277, -0.020077, -0.020744, -0.01996, -0.017341, -0.015827, -0.016079,
-0.01712, -0.017754, -0.017415, -0.017366, -0.016759, -0.01355, -0.010674, -0.010702, -0.012862, -0.015979,
-0.017004, -0.015198, -0.013443, -0.012583, -0.011812, -0.011817, -0.013613, -0.016441, -0.017993, -0.017116,
-0.014318, -0.011901, -0.011656, -0.012355, -0.013739, -0.015476, -0.016058, -0.014799, -0.011971, -0.0106,
-0.012438, -0.014087, -0.013277, -0.012509, -0.013007, -0.012628, -0.012151, -0.013925, -0.01642, -0.016386,
-0.013459, -0.011051, -0.010674, -0.01127, -0.011749, -0.011299, -0.010573, -0.01055, -0.011795, -0.013034,
-0.012255, -0.010589, -0.01058, -0.011513, -0.011599, -0.011087, -0.011464, -0.013101, -0.014631, -0.014873,
-0.012346, -0.009105, -0.008956, -0.010053, -0.009655, -0.009627, -0.011158, -0.011427, -0.008892, -0.007203,
-0.009036, -0.010766, -0.010156, -0.00863, -0.007875, -0.009014, -0.010025, -0.008615, -0.006216, -0.004945,
-0.004401, -0.003649, -0.003457, -0.005239, -0.006479, -0.004329, -0.001773, -0.001207, -0.000249, 0.000578,
-0.001871, -0.004763, -0.004442, -0.002031, -0.000091, 0.000646, 0.000983, 0.000712, -0.000812, -0.00184,
-0.000797, 0.000736, 0.001995, 0.002035, 0.000844, 0.000768, 0.002121, 0.003124, 0.00298, 0.003221,
0.003949, 0.004255, 0.004314, 0.003792, 0.00302, 0.003571, 0.004676, 0.004448, 0.003797, 0.003903,
0.003839, 0.002559, 0.001879, 0.002998, 0.00353, 0.002779, 0.0028, 0.003534, 0.00366, 0.003361,
0.003114, 0.002653, 0.002139, 0.003007, 0.004788, 0.005301, 0.003708, 0.001593, 0.001011, 0.001925,
0.00306, 0.002822, 0.00178, 0.001677, 0.002391, 0.002393, 0.001125, 0.000291, 0.001173, 0.002248,
0.001522, 0.000555, 0.001196, 0.000
};


//R1.00 Hand EQ'd IR for studio freq slope: High.
float TIR10B[1024] = {
0.002115, 0.000895, -0.004466, -0.015817, -0.032021, -0.048596, -0.06126, -0.063068, -0.052191, -0.032449,
0.001242, 0.08577, 0.271126, 0.52548, 0.729915, 0.801113, 0.71084, 0.468078, 0.139414, -0.165365,
-0.35352, -0.405074, -0.341789, -0.215027, -0.090553, -0.011138, 0.016586, 0.016022, 0.0092, 0.014003,
0.029909, 0.042173, 0.038221, 0.017146, -0.008322, -0.027489, -0.027774, -0.009586, 0.01686, 0.037636,
0.040918, 0.024732, -0.007804, -0.044892, -0.076149, -0.093044, -0.094847, -0.084158, -0.065996, -0.046026,
-0.025824, -0.009106, 0.005069, 0.014686, 0.01966, 0.019411, 0.013895, 0.005736, -0.004545, -0.014149,
-0.023376, -0.031519, -0.0394, -0.047398, -0.055423, -0.062652, -0.06783, -0.070197, -0.068653, -0.063901,
-0.055619, -0.045988, -0.03543, -0.026375, -0.019783, -0.015978, -0.016536, -0.019493, -0.024405, -0.029785,
-0.034489, -0.037673, -0.039477, -0.039798, -0.039729, -0.039451, -0.038975, -0.038727, -0.037953, -0.037362,
-0.03628, -0.035781, -0.03569, -0.036372, -0.037672, -0.03953, -0.042301, -0.045149, -0.048484, -0.051566,
-0.055091, -0.058611, -0.062109, -0.065468, -0.068442, -0.070757, -0.07234, -0.073353, -0.073293, -0.072749,
-0.071096, -0.06911, -0.066556, -0.063702, -0.061391, -0.059304, -0.057723, -0.056571, -0.055449, -0.054147,
-0.053058, -0.051978, -0.051027, -0.050145, -0.049489, -0.048718, -0.047866, -0.046857, -0.045669, -0.044406,
-0.04286, -0.041553, -0.040282, -0.039403, -0.038751, -0.038739, -0.03912, -0.039801, -0.040699, -0.041493,
-0.042143, -0.042221, -0.042447, -0.036609, -0.094656, -0.071057, -0.004051, -0.016171, -0.072027, -0.103627,
-0.118914, -0.140893, -0.104958, -0.043779, -0.018811, -0.027298, -0.014958, 0.004357, 0.00659, 0.011253,
0.013561, 0.01732, 0.006657, -0.027037, -0.06758, -0.075714, -0.069065, -0.060365, -0.040626, -0.011987,
0.018292, 0.029454, 0.034195, 0.033381, 0.024429, 0.009109, 0.005187, 0.021424, 0.039035, 0.02986,
0.000532, -0.021703, -0.034406, -0.036833, -0.037157, -0.020851, 0.005706, 0.015465, 0.010889, 0.010337,
0.009639, -0.003167, -0.017923, -0.022599, -0.013873, -0.007541, 0.001745, 0.017686, 0.028946, 0.028348,
0.019098, 0.014475, 0.017929, 0.025373, 0.032945, 0.045954, 0.053744, 0.051536, 0.046316, 0.039199,
0.03395, 0.034592, 0.039208, 0.042475, 0.045989, 0.045529, 0.040289, 0.035556, 0.033324, 0.032407,
0.036634, 0.046429, 0.053293, 0.052532, 0.044913, 0.03727, 0.030336, 0.027192, 0.028614, 0.025493,
0.016833, 0.011993, 0.014383, 0.017994, 0.02301, 0.032173, 0.042937, 0.048878, 0.047786, 0.041106,
0.033194, 0.02852, 0.027327, 0.031262, 0.040195, 0.05047, 0.055773, 0.050965, 0.03891, 0.031236,
0.029894, 0.028764, 0.02624, 0.020477, 0.014984, 0.015337, 0.024148, 0.035703, 0.040342, 0.039615,
0.039107, 0.036081, 0.029201, 0.022603, 0.017822, 0.016173, 0.017826, 0.021608, 0.024657, 0.027145,
0.032338, 0.036177, 0.033034, 0.027126, 0.023988, 0.024357, 0.029987, 0.041363, 0.055325, 0.061788,
0.054777, 0.039086, 0.023822, 0.018033, 0.023974, 0.035342, 0.041833, 0.041539, 0.039833, 0.039704,
0.039173, 0.034099, 0.023432, 0.014733, 0.016416, 0.025944, 0.035101, 0.039754, 0.040133, 0.033735,
0.02258, 0.014728, 0.015121, 0.020788, 0.026053, 0.030189, 0.032921, 0.031074, 0.023654, 0.017438,
0.016832, 0.020299, 0.024679, 0.0278, 0.029982, 0.029419, 0.026902, 0.026, 0.024095, 0.019422,
0.015983, 0.015779, 0.01903, 0.021877, 0.021285, 0.019282, 0.01861, 0.019394, 0.019445, 0.019956,
0.021399, 0.021974, 0.020007, 0.017252, 0.016723, 0.018975, 0.022261, 0.023567, 0.025784, 0.032193,
0.0341, 0.021296, 0.002549, -0.003701, 0.004021, 0.013034, 0.021411, 0.02951, 0.030353, 0.024034,
0.015799, 0.011129, 0.011092, 0.016908, 0.029886, 0.039214, 0.036211, 0.027373, 0.018132, 0.008345,
0.003587, 0.008044, 0.015545, 0.017533, 0.016289, 0.016107, 0.011964, 0.005572, 0.005377, 0.011247,
0.018518, 0.022384, 0.016639, 0.00278, -0.005575, 0.000335, 0.016117, 0.033408, 0.041063, 0.031205,
0.007493, -0.017776, -0.031253, -0.026228, -0.006359, 0.015148, 0.026526, 0.028126, 0.027165, 0.026268,
0.025089, 0.02643, 0.03257, 0.037235, 0.032787, 0.023585, 0.017501, 0.014585, 0.014002, 0.019592,
0.021535, 0.004255, -0.021861, -0.033951, -0.027451, -0.013796, 0.001389, 0.012652, 0.010921, 0.000838,
-0.008275, -0.01175, -0.005772, 0.005512, 0.016402, 0.027381, 0.033976, 0.030334, 0.016965, -0.000427,
-0.012906, -0.016559, -0.012954, -0.004666, 0.000374, -0.006094, -0.019036, -0.027576, -0.026993, -0.01856,
-0.007502, 0.000733, 0.002414, -0.002502, -0.010256, -0.016969, -0.019287, -0.015057, -0.006764, 0.001884,
0.007325, 0.004482, -0.005023, -0.015047, -0.023511, -0.02997, -0.032894, -0.02975, -0.023182, -0.019058,
-0.015329, -0.008418, -0.002402, -0.003717, -0.011942, -0.018682, -0.020209, -0.022692, -0.02931, -0.034771,
-0.035441, -0.035039, -0.03613, -0.036309, -0.035319, -0.032776, -0.025381, -0.015134, -0.008739, -0.004735,
0.001071, 0.003255, -0.004503, -0.020936, -0.036695, -0.041624, -0.034743, -0.025771, -0.023821, -0.026195,
-0.029121, -0.03397, -0.039264, -0.041629, -0.041452, -0.038602, -0.032706, -0.028159, -0.023843, -0.01846,
-0.01931, -0.025276, -0.029663, -0.030142, -0.0236, -0.015622, -0.014323, -0.016561, -0.021233, -0.030142,
-0.040906, -0.047833, -0.044381, -0.034747, -0.027649, -0.023072, -0.022731, -0.02945, -0.034915, -0.032302,
-0.026777, -0.02215, -0.019465, -0.02308, -0.030983, -0.035587, -0.035032, -0.033545, -0.035149, -0.038364,
-0.040753, -0.041772, -0.040137, -0.037148, -0.035991, -0.03699, -0.036763, -0.032572, -0.027592, -0.027443,
-0.030302, -0.030531, -0.030003, -0.03258, -0.036241, -0.036623, -0.034655, -0.035641, -0.038024, -0.034485,
-0.024682, -0.016129, -0.01449, -0.018712, -0.023162, -0.023241, -0.018454, -0.010281, -0.003022, -0.002879,
-0.009041, -0.015836, -0.020365, -0.020424, -0.017023, -0.017081, -0.024741, -0.035655, -0.041799, -0.039337,
-0.034195, -0.029796, -0.023091, -0.017913, -0.017781, -0.022074, -0.028079, -0.027356, -0.016432, -0.003312,
0.006236, 0.01323, 0.012695, -0.001386, -0.018462, -0.022054, -0.015887, -0.012224, -0.00888, -0.003425,
-0.005181, -0.014565, -0.019069, -0.015589, -0.011376, -0.010414, -0.008327, -0.002986, 0.000019, -0.00173,
-0.005238, -0.008845, -0.009673, -0.004028, -0.000401, -0.005824, -0.012023, -0.012967, -0.01294, -0.012385,
-0.005816, 0.002702, 0.003886, -0.003234, -0.01004, -0.010398, -0.005232, 0.002692, 0.008656, 0.010982,
0.009558, 0.004866, 0.000853, 0.00084, 0.003251, 0.004065, 0.002746, -0.000594, -0.007049, -0.01105,
-0.004863, 0.006926, 0.014431, 0.014178, 0.007371, -0.00001, -0.000086, 0.004489, 0.005515, 0.003721,
0.00102, -0.001166, 0.000693, 0.00554, 0.008351, 0.005992, 0.00325, 0.004914, 0.00859, 0.011261,
0.009798, 0.00616, 0.007628, 0.012034, 0.014745, 0.014766, 0.00897, 0.002942, 0.004671, 0.010453,
0.012679, 0.009484, 0.006453, 0.007585, 0.010657, 0.012238, 0.011999, 0.009688, 0.002199, -0.002841,
0.005634, 0.020008, 0.026401, 0.025256, 0.023338, 0.017398, 0.005602, -0.002302, -0.000947, 0.003048,
0.005249, 0.008218, 0.010236, 0.008072, 0.003309, 0.00071, 0.00259, 0.008133, 0.015913, 0.02001,
0.018603, 0.017019, 0.017204, 0.01729, 0.017075, 0.017243, 0.017543, 0.017556, 0.014565, 0.010116,
0.009925, 0.012469, 0.011686, 0.008079, 0.009797, 0.017413, 0.021681, 0.020021, 0.017965, 0.017572,
0.015365, 0.010984, 0.007204, 0.005292, 0.005698, 0.007394, 0.009208, 0.01105, 0.013145, 0.014619,
0.015274, 0.016875, 0.018143, 0.015618, 0.010535, 0.009699, 0.017317, 0.029192, 0.037585, 0.037662,
0.028758, 0.015549, 0.004931, -0.000133, -0.000789, 0.000813, 0.005402, 0.012121, 0.015873, 0.013561,
0.007736, 0.00477, 0.007099, 0.011683, 0.016366, 0.021508, 0.025284, 0.025343, 0.024295, 0.025156,
0.026561, 0.024555, 0.016289, 0.006258, 0.003187, 0.007513, 0.011568, 0.010919, 0.007661, 0.003708,
0.000669, 0.002692, 0.01144, 0.021139, 0.022035, 0.012222, 0.003677, 0.007304, 0.016315, 0.019142,
0.01585, 0.011826, 0.008455, 0.003965, -0.000053, 0.002323, 0.011419, 0.018798, 0.018773, 0.013122,
0.005984, 0.001103, 0.001436, 0.006966, 0.014828, 0.019894, 0.017895, 0.01018, 0.003345, 0.005798,
0.016977, 0.025686, 0.023038, 0.01155, 0.000139, -0.004502, -0.001029, 0.006526, 0.012024, 0.011571,
0.009337, 0.008519, 0.005682, 0.002389, 0.004082, 0.009997, 0.014737, 0.014405, 0.008841, 0.002771,
0.000199, 0.00224, 0.00626, 0.009241, 0.011686, 0.01117, 0.007641, 0.005738, 0.007437, 0.01066,
0.011172, 0.006833, 0.001459, -0.000077, 0.001776, 0.004249, 0.005113, 0.004827, 0.003715, 0.000144,
-0.00415, -0.005944, -0.005034, -0.001964, 0.002422, 0.003693, 0.0003, -0.003387, -0.0049, -0.004095,
-0.002006, -0.000895, -0.000718, -0.001583, -0.003137, -0.003373, -0.003667, -0.003527, -0.001176, 0.000713,
-0.000112, -0.002427, -0.002575, -0.000383, -0.00004, -0.001605, -0.001713, -0.001221, -0.002299, -0.003952,
-0.00572, -0.008022, -0.008872, -0.006679, -0.003542, -0.004013, -0.009299, -0.012541, -0.010728, -0.008208,
-0.005893, -0.004059, -0.004927, -0.007475, -0.009548, -0.011428, -0.011219, -0.007825, -0.004563, -0.003426,
-0.004246, -0.005679, -0.006459, -0.00748, -0.00773, -0.004385, -0.000244, 0.00065, -0.002345, -0.008256,
-0.012773, -0.013193, -0.011737, -0.010004, -0.007917, -0.006732, -0.00827, -0.012232, -0.015627, -0.015838,
-0.012311, -0.00762, -0.005036, -0.004539, -0.006186, -0.009484, -0.012124, -0.012229, -0.009243, -0.006686,
-0.007999, -0.010439, -0.010583, -0.010158, -0.010858, -0.010657, -0.010083, -0.012118, -0.015791, -0.01689,
-0.013499, -0.008984, -0.006332, -0.005678, -0.0061, -0.006255, -0.006296, -0.007219, -0.009937, -0.013126,
-0.01382, -0.012335, -0.011749, -0.012285, -0.01216, -0.011227, -0.011164, -0.012931, -0.015218, -0.016143,
-0.013092, -0.0077, -0.005223, -0.005362, -0.00513, -0.005839, -0.008945, -0.011249, -0.009784, -0.007878,
-0.009738, -0.012506, -0.012859, -0.011272, -0.009709, -0.010296, -0.011446, -0.009995, -0.006616, -0.003948,
-0.002471, -0.001485, -0.001788, -0.005227, -0.009096, -0.008911, -0.006648, -0.005587, -0.004094, -0.002774,
-0.005728, -0.010669, -0.012442, -0.010483, -0.007342, -0.004842, -0.003026, -0.002528, -0.004395, -0.006698,
-0.006757, -0.005411, -0.003635, -0.003054, -0.004398, -0.005125, -0.00405, -0.002776, -0.002699, -0.002515,
-0.001841, -0.001594, -0.001781, -0.002868, -0.004548, -0.004728, -0.003636, -0.00355, -0.004116, -0.003987,
-0.003899, -0.005266, -0.006284, -0.00498, -0.003581, -0.003547, -0.00301, -0.00164, -0.000861, -0.000852,
-0.001175, -0.002007, -0.003095, -0.002508, -0.000186, 0.001262, -0.000109, -0.003175, -0.005096, -0.004786,
-0.003176, -0.0025, -0.002956, -0.002713, -0.001407, -0.000689, -0.001737, -0.002909, -0.002145, -0.000527,
-0.000721, -0.001733, -0.001183, -0.0008
};




/*
//R1.00 PV29 Low
float TIR10[1024] = {
0, 0.000137, 0.000355, 0.000694, 0.000963, 0.000977, 0.000781, -0.000073, -0.003823, -0.011605,
-0.020718, -0.029365, -0.034974, -0.036184, -0.034352, -0.026253, 0.010094, 0.126128, 0.304315, 0.438507,
0.4684, 0.426836, 0.38098, 0.346593, 0.308215, 0.244613, 0.158146, 0.064784, -0.013786, -0.059464,
-0.075526, -0.069169, -0.060279, -0.054176, -0.057196, -0.061077, -0.065103, -0.063582, -0.059419, -0.053467,
-0.048295, -0.044663, -0.042867, -0.04249, -0.04352, -0.046303, -0.050269, -0.057633, -0.065563, -0.076301,
-0.085525, -0.095216, -0.102195, -0.107259, -0.109072, -0.109218, -0.106305, -0.102858, -0.097372, -0.09208,
-0.085467, -0.079417, -0.072655, -0.066631, -0.05986, -0.053598, -0.047246, -0.040867, -0.034815, -0.02935,
-0.024141, -0.020086, -0.016626, -0.014097, -0.012502, -0.011802, -0.011697, -0.011932, -0.01247, -0.013052,
-0.013869, -0.014608, -0.015409, -0.01611, -0.017214, -0.017801, -0.018801, -0.018693, -0.018443, -0.017086,
-0.015532, -0.013116, -0.01026, -0.007198, -0.00451, -0.002043, -0.000649, 0.000102, -0.000093, -0.000589,
-0.00231, -0.004167, -0.006698, -0.009058, -0.011774, -0.01435, -0.016967, -0.019665, -0.022225, -0.02439,
-0.026609, -0.028639, -0.030112, -0.031419, -0.032447, -0.032766, -0.033232, -0.032995, -0.032915, -0.032324,
-0.03187, -0.031024, -0.030278, -0.029528, -0.028769, -0.028074, -0.027418, -0.026804, -0.026286, -0.025938,
-0.025711, -0.025405, -0.0254, -0.025234, -0.025351, -0.025316, -0.025269, -0.025337, -0.025167, -0.02507,
-0.024909, -0.024667, -0.024289, -0.023922, -0.023463, -0.023053, -0.022595, -0.022153, -0.021632, -0.021173,
-0.020714, -0.020458, -0.020298, -0.02016, -0.020057, -0.020169, -0.020269, -0.020791, -0.020964, -0.021209,
-0.02173, -0.022176, -0.022512, -0.02301, -0.023297, -0.023633, -0.023766, -0.02411, -0.024101, -0.024283,
-0.024058, -0.023948, -0.023559, -0.023292, -0.022938, -0.022236, -0.021632, -0.020972, -0.020386, -0.01996,
-0.019275, -0.018814, -0.018177, -0.017486, -0.016866, -0.016414, -0.015693, -0.015066, -0.014465, -0.013819,
-0.013411, -0.01281, -0.012406, -0.011754, -0.011084, -0.010386, -0.009967, -0.009409, -0.008796, -0.008428,
-0.00779, -0.007534, -0.007126, -0.006503, -0.00591, -0.005442, -0.004833, -0.003994, -0.003171, -0.002695,
-0.001911, -0.0012, -0.000289, 0.000552, 0.001603, 0.002465, 0.003621, 0.00449, 0.005416, 0.006403,
0.007179, 0.008006, 0.008812, 0.009435, 0.010146, 0.010764, 0.011578, 0.012026, 0.01247, 0.012923,
0.013218, 0.013812, 0.014227, 0.014659, 0.015014, 0.01561, 0.016116, 0.016978, 0.017572, 0.018231,
0.019313, 0.02016, 0.021271, 0.022467, 0.02372, 0.024736, 0.025927, 0.027283, 0.028865, 0.030456,
0.031743, 0.033262, 0.03471, 0.03626, 0.037859, 0.039307, 0.040631, 0.041907, 0.043335, 0.044392,
0.045718, 0.04704, 0.048219, 0.049186, 0.050159, 0.051186, 0.052123, 0.052857, 0.053593, 0.054464,
0.055196, 0.055623, 0.055905, 0.055127, 0.052911, 0.048842, 0.043747, 0.038884, 0.03408, 0.02988,
0.025936, 0.022513, 0.019744, 0.017824, 0.016681, 0.016437, 0.016229, 0.016032, 0.015886, 0.015676,
0.015293, 0.015177, 0.01524, 0.015154, 0.014952, 0.014846, 0.014625, 0.014547, 0.014317, 0.014158,
0.013921, 0.013577, 0.013574, 0.013761, 0.013995, 0.014243, 0.014468, 0.014981, 0.015523, 0.016339,
0.017064, 0.017931, 0.018541, 0.0194, 0.020146, 0.020831, 0.021547, 0.022288, 0.02265, 0.023226,
0.023799, 0.024252, 0.024349, 0.024628, 0.024756, 0.02477, 0.025046, 0.025026, 0.024904, 0.024935,
0.024891, 0.024677, 0.024594, 0.024379, 0.02409, 0.023831, 0.023836, 0.023752, 0.023523, 0.023376,
0.023276, 0.023168, 0.022705, 0.022588, 0.0222, 0.021835, 0.021529, 0.021223, 0.020998, 0.020501,
0.02005, 0.019602, 0.019007, 0.018498, 0.017781, 0.017021, 0.016468, 0.01595, 0.015416, 0.014996,
0.014538, 0.013836, 0.01344, 0.012884, 0.012533, 0.012065, 0.011596, 0.01103, 0.010761, 0.010637,
0.01026, 0.009749, 0.009389, 0.00909, 0.008545, 0.008409, 0.00797, 0.00753, 0.007211, 0.006808,
0.006457, 0.006177, 0.005762, 0.00538, 0.004964, 0.004618, 0.004347, 0.004034, 0.003578, 0.00323,
0.002927, 0.002601, 0.002363, 0.002089, 0.001722, 0.00145, 0.001267, 0.001004, 0.000806, 0.000512,
0.000217, 0.000092, -0.000082, -0.000266, -0.000492, -0.000736, -0.000952, -0.00107, -0.001272, -0.001374,
-0.0016, -0.001882, -0.002022, -0.002102, -0.002275, -0.002392, -0.002702, -0.002796, -0.003066, -0.003108,
-0.003175, -0.003414, -0.003641, -0.003741, -0.003918, -0.003948, -0.004125, -0.004317, -0.004524, -0.004613,
-0.004693, -0.004758, -0.005073, -0.005155, -0.005272, -0.005401, -0.005487, -0.005546, -0.005853, -0.005881,
-0.006055, -0.00607, -0.006182, -0.006286, -0.006563, -0.006532, -0.006716, -0.006683, -0.006808, -0.006952,
-0.007109, -0.007148, -0.007175, -0.007277, -0.007454, -0.007448, -0.007581, -0.007596, -0.007687, -0.007809,
-0.007907, -0.007953, -0.008066, -0.008075, -0.008165, -0.008242, -0.008451, -0.008418, -0.008497, -0.00853,
-0.008579, -0.008673, -0.008788, -0.008784, -0.008858, -0.00883, -0.008925, -0.009033, -0.009065, -0.009065,
-0.009182, -0.009084, -0.009283, -0.009354, -0.009334, -0.009357, -0.009346, -0.009444, -0.009503, -0.009504,
-0.009566, -0.009568, -0.009493, -0.009687, -0.009654, -0.009656, -0.00967, -0.009686, -0.009701, -0.009759,
-0.009751, -0.009803, -0.009703, -0.009703, -0.009784, -0.009864, -0.009777, -0.00979, -0.009707, -0.009796,
-0.009786, -0.009826, -0.009758, -0.009772, -0.00973, -0.009795, -0.009777, -0.00977, -0.009767, -0.009675,
-0.009711, -0.009807, -0.009716, -0.009767, -0.009656, -0.009644, -0.009668, -0.009711, -0.009636, -0.009641,
-0.009498, -0.009592, -0.009571, -0.009551, -0.009488, -0.009401, -0.009383, -0.009438, -0.009402, -0.009329,
-0.009327, -0.009222, -0.009152, -0.009224, -0.009189, -0.009159, -0.009043, -0.008969, -0.00904, -0.00905,
-0.008911, -0.008921, -0.008772, -0.008809, -0.008797, -0.008725, -0.008715, -0.0086, -0.008521, -0.00854,
-0.008563, -0.00842, -0.008422, -0.00831, -0.008321, -0.0083, -0.008181, -0.00819, -0.008079, -0.007952,
-0.00803, -0.007988, -0.0079, -0.007836, -0.007643, -0.007691, -0.00767, -0.007629, -0.007541, -0.00743,
-0.007392, -0.007385, -0.007354, -0.007272, -0.007167, -0.007041, -0.007074, -0.00705, -0.006963, -0.006895,
-0.006788, -0.006747, -0.00669, -0.006719, -0.006572, -0.006523, -0.006422, -0.006338, -0.006346, -0.006267,
-0.006242, -0.006112, -0.006018, -0.006025, -0.005993, -0.005861, -0.005859, -0.005692, -0.005667, -0.005659,
-0.005622, -0.005489, -0.005442, -0.005294, -0.005318, -0.005288, -0.005245, -0.005065, -0.005038, -0.00494,
-0.004988, -0.004843, -0.00488, -0.004691, -0.004662, -0.004519, -0.004678, -0.004454, -0.004457, -0.004292,
-0.004267, -0.004259, -0.004267, -0.004023, -0.004102, -0.003833, -0.003933, -0.003845, -0.003815, -0.003681,
-0.003631, -0.003526, -0.003573, -0.003475, -0.003404, -0.003251, -0.003231, -0.003178, -0.003205, -0.003054,
-0.003046, -0.002878, -0.002899, -0.002827, -0.002854, -0.002708, -0.002616, -0.002491, -0.002567, -0.002519,
-0.002427, -0.002359, -0.002255, -0.002171, -0.002272, -0.002113, -0.002105, -0.001986, -0.001901, -0.001868,
-0.001904, -0.001759, -0.001754, -0.001634, -0.001547, -0.001558, -0.001574, -0.001379, -0.001395, -0.001236,
-0.001304, -0.00126, -0.001218, -0.001092, -0.001055, -0.000963, -0.001022, -0.000933, -0.000915, -0.000842,
-0.000696, -0.00073, -0.000757, -0.000661, -0.000644, -0.000529, -0.000451, -0.000517, -0.000478, -0.000354,
-0.000318, -0.000226, -0.000286, -0.000198, -0.000135, -0.000081, -0.000074, -0.000094, -0.000047, 0.000027,
0.000073, 0.000157, 0.000177, 0.000171, 0.000192, 0.000311, 0.000346, 0.000301, 0.000389, 0.000402,
0.000424, 0.000537, 0.000565, 0.000605, 0.000559, 0.000554, 0.000694, 0.000771, 0.000743, 0.000755,
0.000778, 0.000802, 0.000869, 0.000959, 0.000935, 0.000901, 0.000941, 0.001012, 0.001055, 0.001047,
0.001012, 0.001092, 0.001091, 0.00119, 0.001208, 0.00124, 0.00119, 0.001181, 0.001317, 0.001346,
0.001335, 0.001347, 0.001365, 0.001382, 0.001444, 0.001484, 0.00152, 0.001466, 0.001498, 0.001565,
0.001607, 0.001552, 0.00158, 0.001598, 0.00161, 0.001664, 0.001697, 0.001773, 0.00165, 0.001649,
0.001737, 0.001825, 0.001792, 0.001701, 0.001751, 0.00186, 0.001829, 0.001872, 0.001861, 0.001868,
0.001835, 0.001912, 0.001952, 0.001892, 0.001915, 0.001972, 0.001915, 0.001936, 0.001991, 0.002022,
0.001957, 0.001978, 0.00199, 0.002087, 0.002009, 0.001981, 0.002011, 0.002021, 0.002024, 0.002115,
0.002076, 0.002025, 0.002004, 0.00208, 0.002068, 0.002065, 0.00202, 0.002085, 0.00207, 0.00202,
0.00213, 0.002141, 0.002071, 0.002091, 0.002057, 0.002175, 0.0021, 0.002069, 0.002095, 0.002056,
0.002119, 0.002099, 0.002135, 0.002021, 0.002053, 0.002105, 0.002083, 0.002029, 0.002045, 0.002047,
0.001998, 0.002057, 0.002082, 0.002054, 0.002002, 0.002021, 0.002027, 0.002073, 0.002007, 0.001976,
0.002044, 0.001982, 0.001994, 0.001992, 0.002079, 0.00199, 0.001954, 0.001932, 0.002046, 0.001963,
0.001971, 0.001925, 0.001942, 0.001941, 0.002026, 0.001982, 0.001926, 0.0019, 0.001926, 0.001931,
0.001928, 0.001831, 0.001954, 0.001825, 0.001847, 0.001845, 0.001927, 0.001787, 0.001765, 0.00174,
0.001848, 0.001712, 0.00173, 0.001681, 0.001735, 0.001661, 0.0018, 0.00169, 0.001647, 0.001622,
0.001691, 0.001673, 0.001664, 0.001522, 0.001621, 0.001606, 0.001592, 0.001579, 0.001615, 0.001496,
0.00157, 0.001508, 0.001563, 0.001451, 0.001479, 0.001433, 0.001443, 0.001389, 0.001485, 0.00139,
0.001435, 0.001319, 0.001393, 0.001421, 0.001393, 0.001294, 0.001331, 0.001246, 0.00137, 0.001329,
0.001401, 0.001126, 0.00121, 0.001224, 0.001259, 0.001132, 0.00115, 0.001051, 0.001165, 0.00107,
0.001155, 0.001105, 0.001043, 0.000966, 0.001051, 0.001034, 0.001022, 0.000921, 0.000951, 0.000904,
0.000956, 0.000926, 0.000953, 0.000876, 0.000884, 0.000837, 0.000944, 0.000859, 0.00082, 0.000794,
0.000768, 0.000783, 0.000775, 0.000805, 0.000771, 0.00071, 0.000629, 0.000755, 0.000716, 0.000606,
0.000589, 0.000652, 0.000625, 0.000607, 0.000595, 0.000676, 0.00054, 0.000522, 0.000547, 0.000503,
0.00056, 0.00049, 0.000449, 0.000461, 0.000408, 0.000459, 0.000475, 0.000394, 0.000399, 0.000351,
0.000366, 0.000454, 0.000364, 0.000279, 0.000317, 0.000367, 0.000295, 0.000346, 0.000277, 0.00024,
0.000257, 0.000253, 0.000289, 0.000215, 0.000176, 0.000238, 0.000171, 0.000179, 0.000266, 0.000129,
0.000109, 0.000132, 0.000085, 0.000187, 0.000096, 0.000097, 0.000048, 0.000107, 0.000085, 0.000121,
0.000052, 0.00002, 0.000088, 0.000072, 0.00002, -0.00001, -0.000036, -0.00002, 0.000019, 0.000033,
-0.000048, -0.000086, -0.000067, -0.000022, -0.000047, -0.000103, -0.000134, -0.000069, -0.000135, -0.000127,
-0.000085, -0.000066, -0.00014, -0.000172
};


//R1.00 PV29 High
float TIR10B[1024] = {
0, 0.000172, 0.000497, 0.001016, 0.001476, 0.001545, 0.001155, -0.000273, -0.005666, -0.017218,
-0.03195, -0.046114, -0.054457, -0.053133, -0.042787, -0.020492, 0.040634, 0.211373, 0.487715, 0.727811,
0.798257, 0.685839, 0.474528, 0.233675, -0.004147, -0.23375, -0.440912, -0.600403, -0.680665, -0.656177,
-0.53438, -0.342311, -0.132532, 0.054024, 0.183564, 0.251716, 0.261793, 0.232384, 0.178136, 0.114721,
0.052938, 0.001015, -0.036409, -0.057542, -0.063885, -0.059499, -0.048473, -0.038157, -0.029731, -0.027978,
-0.029206, -0.034494, -0.039123, -0.041865, -0.039913, -0.03427, -0.023736, -0.011818, 0.001621, 0.013039,
0.022927, 0.028897, 0.03216, 0.031699, 0.029817, 0.026289, 0.022519, 0.019273, 0.01666, 0.014482,
0.013033, 0.011132, 0.009006, 0.006158, 0.002438, -0.002124, -0.007044, -0.011727, -0.015823, -0.018851,
-0.020954, -0.021911, -0.022037, -0.021436, -0.020995, -0.020006, -0.019563, -0.018104, -0.016434, -0.013535,
-0.010344, -0.00636, -0.002118, 0.00186, 0.004594, 0.005964, 0.004936, 0.001969, -0.002891, -0.008407,
-0.014988, -0.021049, -0.02663, -0.030557, -0.033226, -0.034289, -0.034181, -0.033388, -0.032107, -0.030336,
-0.028816, -0.027569, -0.026209, -0.025089, -0.024117, -0.022694, -0.02167, -0.020187, -0.019096, -0.017766,
-0.016849, -0.015829, -0.01517, -0.014824, -0.014746, -0.014962, -0.015375, -0.015904, -0.016539, -0.017324,
-0.018185, -0.018829, -0.019631, -0.020113, -0.02072, -0.021035, -0.021152, -0.021267, -0.021036, -0.020786,
-0.020458, -0.020061, -0.019542, -0.019086, -0.018623, -0.018322, -0.018106, -0.018018, -0.017928, -0.01795,
-0.018011, -0.018336, -0.018838, -0.019381, -0.019908, -0.020593, -0.021194, -0.02218, -0.022709, -0.023067,
-0.023565, -0.023901, -0.023983, -0.024129, -0.023987, -0.023821, -0.023404, -0.023224, -0.022734, -0.022486,
-0.021875, -0.0214, -0.020697, -0.020193, -0.019745, -0.018999, -0.018387, -0.017833, -0.017499, -0.017528,
-0.017416, -0.017581, -0.017589, -0.017457, -0.017316, -0.01734, -0.017049, -0.016765, -0.016488, -0.016157,
-0.016121, -0.015944, -0.015986, -0.015761, -0.015417, -0.014966, -0.01482, -0.014599, -0.01431, -0.014306,
-0.014036, -0.014164, -0.014183, -0.013878, -0.013458, -0.01311, -0.012595, -0.011757, -0.010863, -0.010432,
-0.009833, -0.009372, -0.008742, -0.008193, -0.007424, -0.006862, -0.006021, -0.005505, -0.005043, -0.004572,
-0.004377, -0.004197, -0.00403, -0.004048, -0.003941, -0.003844, -0.003411, -0.003257, -0.003119, -0.002939,
-0.002897, -0.002462, -0.002081, -0.001636, -0.001246, -0.000564, 0.000091, 0.001183, 0.00203, 0.002836,
0.004069, 0.005048, 0.00621, 0.007422, 0.008623, 0.009419, 0.010204, 0.011098, 0.01226, 0.013492,
0.014329, 0.01528, 0.016105, 0.016993, 0.017939, 0.018707, 0.019265, 0.019694, 0.020296, 0.020535,
0.021082, 0.021788, 0.022459, 0.022926, 0.023388, 0.023957, 0.024504, 0.02486, 0.025217, 0.025802,
0.026358, 0.026598, 0.026616, 0.025286, 0.021838, 0.015586, 0.007577, 0.000041, -0.006149, -0.009759,
-0.010895, -0.009526, -0.005953, -0.000543, 0.005989, 0.013137, 0.019312, 0.0238, 0.02637, 0.02698,
0.025812, 0.023881, 0.021787, 0.019654, 0.017762, 0.016534, 0.015826, 0.015857, 0.016195, 0.016863,
0.017535, 0.017987, 0.018649, 0.019445, 0.020188, 0.020776, 0.021117, 0.021589, 0.022005, 0.022665,
0.023198, 0.023796, 0.023996, 0.024307, 0.024432, 0.024394, 0.02433, 0.0243, 0.02384, 0.023575,
0.023423, 0.023255, 0.022727, 0.022398, 0.022011, 0.021572, 0.021542, 0.021356, 0.021088, 0.021034,
0.020994, 0.020795, 0.020739, 0.02058, 0.020328, 0.020109, 0.02026, 0.020449, 0.020502, 0.020605,
0.020753, 0.020884, 0.02054, 0.020465, 0.020108, 0.019734, 0.019454, 0.019256, 0.019244, 0.018993,
0.018759, 0.018538, 0.018156, 0.017858, 0.017348, 0.01676, 0.016448, 0.016315, 0.016286, 0.01646,
0.016642, 0.016487, 0.016553, 0.016409, 0.016429, 0.016303, 0.016108, 0.015734, 0.015664, 0.01586,
0.015832, 0.015551, 0.015333, 0.015169, 0.01471, 0.014679, 0.014392, 0.014052, 0.013837, 0.01356,
0.013341, 0.013232, 0.012989, 0.012744, 0.012439, 0.012196, 0.012064, 0.011915, 0.01158, 0.01131,
0.011097, 0.010876, 0.010764, 0.010632, 0.01037, 0.010165, 0.010068, 0.009895, 0.009768, 0.009513,
0.009201, 0.009068, 0.008921, 0.008763, 0.008538, 0.008255, 0.007977, 0.007819, 0.007595, 0.007485,
0.007246, 0.006892, 0.00667, 0.006562, 0.006383, 0.006256, 0.00589, 0.005713, 0.00536, 0.005253,
0.00519, 0.004938, 0.004634, 0.004456, 0.004217, 0.004155, 0.003962, 0.003711, 0.003409, 0.003234,
0.003114, 0.003043, 0.00267, 0.002492, 0.002312, 0.002132, 0.002008, 0.001934, 0.00156, 0.00144,
0.001198, 0.001136, 0.001003, 0.000865, 0.000495, 0.000435, 0.00019, 0.000184, 0.000045, -0.000154,
-0.000406, -0.000531, -0.000603, -0.000742, -0.000998, -0.001068, -0.001262, -0.001335, -0.001474, -0.001671,
-0.00186, -0.001987, -0.002172, -0.002237, -0.002367, -0.002498, -0.002801, -0.002854, -0.002974, -0.003038,
-0.003109, -0.003243, -0.003433, -0.0035, -0.003631, -0.003644, -0.003785, -0.003979, -0.004105, -0.004172,
-0.004359, -0.004306, -0.004559, -0.004737, -0.004791, -0.004851, -0.004858, -0.004991, -0.005119, -0.005185,
-0.005312, -0.005376, -0.005329, -0.005584, -0.005647, -0.005712, -0.005779, -0.005847, -0.005916, -0.006044,
-0.006107, -0.006229, -0.006173, -0.00619, -0.006326, -0.006511, -0.00651, -0.006574, -0.006523, -0.006657,
-0.006724, -0.006847, -0.006847, -0.006911, -0.006916, -0.007041, -0.007099, -0.007156, -0.007212, -0.007152,
-0.007219, -0.007398, -0.007388, -0.007499, -0.007429, -0.00743, -0.007492, -0.007608, -0.007599, -0.007653,
-0.007531, -0.007657, -0.007713, -0.007766, -0.007758, -0.007697, -0.007702, -0.007824, -0.007876, -0.007866,
-0.007917, -0.007849, -0.007791, -0.007912, -0.007963, -0.00801, -0.007937, -0.007873, -0.00799, -0.008094,
-0.008013, -0.008058, -0.007926, -0.007981, -0.00803, -0.008015, -0.008059, -0.007983, -0.007916, -0.00797,
-0.008074, -0.007992, -0.008035, -0.007956, -0.008, -0.008038, -0.007955, -0.007995, -0.007914, -0.007783,
-0.007895, -0.007934, -0.00791, -0.007885, -0.00769, -0.007744, -0.007788, -0.007825, -0.007797, -0.007712,
-0.007691, -0.007727, -0.007756, -0.007721, -0.00763, -0.007488, -0.007532, -0.007567, -0.007537, -0.007506,
-0.007418, -0.007393, -0.007368, -0.007456, -0.007357, -0.007323, -0.007232, -0.007146, -0.007181, -0.00715,
-0.007176, -0.007079, -0.006988, -0.007018, -0.00704, -0.006939, -0.006961, -0.006803, -0.006773, -0.006798,
-0.006816, -0.006711, -0.006671, -0.006516, -0.006545, -0.006566, -0.006578, -0.006412, -0.006374, -0.006278,
-0.00636, -0.006254, -0.006326, -0.006156, -0.006114, -0.005956, -0.006156, -0.005981, -0.005994, -0.005827,
-0.005789, -0.005806, -0.005873, -0.005638, -0.005713, -0.005428, -0.005515, -0.00547, -0.005481, -0.00537,
-0.005323, -0.005218, -0.00529, -0.005233, -0.005177, -0.005006, -0.004964, -0.00492, -0.004989, -0.004871,
-0.004875, -0.004701, -0.004712, -0.004658, -0.00472, -0.004594, -0.004477, -0.004311, -0.004387, -0.004389,
-0.004328, -0.004267, -0.00415, -0.004039, -0.004165, -0.00404, -0.004037, -0.003914, -0.0038, -0.003749,
-0.003812, -0.003689, -0.003689, -0.003567, -0.003454, -0.003462, -0.003519, -0.003334, -0.003336, -0.003159,
-0.003226, -0.003221, -0.00321, -0.00308, -0.003015, -0.002895, -0.002955, -0.002886, -0.002877, -0.002805,
-0.002622, -0.002625, -0.002679, -0.002606, -0.002593, -0.002461, -0.002338, -0.002396, -0.002384, -0.002253,
-0.002188, -0.002068, -0.002128, -0.00206, -0.001994, -0.001929, -0.001924, -0.001971, -0.001949, -0.001868,
-0.001791, -0.00166, -0.001597, -0.001592, -0.001583, -0.001454, -0.001392, -0.001447, -0.001375, -0.001363,
-0.001348, -0.001214, -0.001149, -0.001085, -0.001139, -0.001181, -0.001041, -0.000911, -0.000908, -0.000899,
-0.000887, -0.000871, -0.000795, -0.000665, -0.000661, -0.00071, -0.00069, -0.000611, -0.000537, -0.000525,
-0.000567, -0.000485, -0.000466, -0.00033, -0.000263, -0.0002, -0.000254, -0.000297, -0.000159, -0.000091,
-0.000083, -0.000073, -0.000059, -0.000043, 0.000033, 0.000103, 0.000169, 0.000117, 0.000133, 0.000208,
0.000277, 0.000228, 0.000246, 0.000265, 0.000285, 0.000363, 0.000436, 0.000561, 0.000446, 0.000405,
0.000487, 0.000619, 0.000625, 0.000519, 0.000543, 0.000682, 0.000693, 0.000763, 0.00077, 0.00078,
0.000734, 0.000812, 0.000883, 0.000834, 0.000851, 0.000926, 0.00088, 0.000899, 0.000976, 0.001047,
0.000997, 0.001013, 0.001029, 0.001161, 0.001107, 0.001062, 0.001083, 0.001102, 0.001122, 0.001256,
0.001261, 0.001211, 0.001169, 0.001248, 0.001263, 0.001277, 0.001234, 0.001313, 0.001327, 0.001283,
0.001418, 0.001482, 0.001425, 0.001434, 0.001387, 0.001519, 0.001464, 0.001418, 0.001436, 0.001396,
0.001476, 0.001491, 0.001562, 0.001454, 0.001475, 0.001552, 0.001563, 0.001516, 0.001533, 0.001549,
0.001506, 0.001584, 0.001654, 0.001658, 0.001606, 0.001618, 0.00163, 0.001698, 0.001644, 0.001598,
0.001672, 0.001623, 0.001638, 0.001651, 0.001778, 0.001719, 0.001667, 0.001622, 0.001755, 0.0017,
0.001709, 0.001661, 0.001675, 0.001688, 0.001815, 0.001812, 0.001753, 0.001701, 0.001713, 0.001724,
0.001733, 0.001627, 0.001762, 0.001652, 0.00167, 0.001685, 0.001813, 0.001696, 0.00165, 0.001609,
0.001745, 0.001634, 0.001651, 0.001608, 0.001684, 0.001636, 0.001822, 0.001756, 0.001698, 0.001647,
0.001716, 0.001719, 0.001721, 0.001551, 0.001629, 0.00164, 0.001649, 0.001656, 0.001719, 0.001601,
0.00167, 0.001614, 0.00168, 0.001564, 0.001577, 0.001529, 0.001544, 0.001498, 0.001629, 0.00157,
0.001633, 0.001516, 0.001583, 0.001642, 0.001636, 0.001515, 0.001522, 0.001413, 0.001545, 0.001546,
0.00166, 0.001359, 0.001379, 0.001395, 0.001465, 0.001354, 0.001369, 0.001266, 0.001403, 0.001349,
0.001473, 0.001467, 0.001402, 0.001287, 0.001357, 0.001359, 0.00136, 0.001244, 0.001256, 0.001207,
0.001277, 0.00128, 0.001339, 0.001274, 0.001274, 0.001215, 0.001334, 0.001265, 0.001203, 0.001148,
0.001098, 0.001109, 0.001117, 0.00118, 0.001177, 0.001115, 0.001002, 0.001131, 0.001127, 0.001009,
0.00096, 0.001029, 0.001031, 0.001032, 0.001031, 0.001143, 0.001012, 0.000952, 0.000956, 0.0009,
0.000964, 0.000904, 0.000849, 0.000857, 0.000805, 0.000871, 0.000929, 0.000863, 0.000861, 0.000801,
0.000803, 0.000919, 0.000847, 0.000725, 0.000732, 0.000793, 0.00073, 0.000789, 0.000724, 0.000666,
0.00067, 0.000672, 0.00073, 0.000665, 0.000606, 0.000668, 0.000606, 0.000607, 0.000721, 0.000592,
0.000533, 0.000538, 0.000483, 0.000605, 0.000539, 0.000537, 0.000476, 0.000536, 0.00053, 0.000581,
0.00051, 0.000447, 0.000505, 0.000497, 0.000432, 0.000373, 0.00032, 0.000329, 0.000392, 0.000447,
0.000379, 0.000318, 0.000321, 0.000379, 0.000371, 0.000307, 0.000248, 0.00031, 0.000249, 0.000251,
0.000308, 0.000358, 0.000286, 0.000222
};

*/






};
