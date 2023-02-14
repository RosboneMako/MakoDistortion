/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MakoDist01AudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener, public juce::Button::Listener, public juce::Timer
                                        
{
public:
    MakoDist01AudioProcessorEditor (MakoDist01AudioProcessor&);
    ~MakoDist01AudioProcessorEditor() override;
    
    void timerCallback() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void buttonClicked(juce::Button* butt) override;
    void buttonStateChanged(juce::Button* butt);
    //void sliderValueChanged(juce::Slider* slider) override;
    
    juce::uint32 colorButton = 0xFFC0B4A0;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MakoDist01AudioProcessor& makoProcessor;
   
    void GUI_Init_Large_Slider(juce::Slider* slider);
    void GUI_Init_Small_Slider(juce::Slider* slider);

    int ctrlHelp = 0;
    int ctrlHelpLast = 0;
    //juce::TextButton butHelp;
    juce::Label labHelp;
    juce::Label labHelp2;
    //juce::TextButton butClip;

    juce::Image imgBack;
    juce::Image imgPedals;
    juce::Image imgPedal;
    juce::Image imgPedal_PA;
    juce::Image imgPedal_EQ;
    juce::Image imgLogGraph;

    //R1.00 BITMASK to track pedal graphics. 
    bool GFX_Pedal_Refresh = false;
    int PEDAL_ON = 0;
    int PEDAL_ON_LAST = 0;
    int Ped_Comp3 = 1;       //R1.00 Lean Comp
    int Ped_OD1 = 2;
    int Ped_Mod1a = 4;
    int Ped_Mod2a = 8;
    int Ped_Mod1b = 16;
    int Ped_Mod2b = 32;
    int Ped_Mod1c = 64;
    int Ped_Mod2c = 128;
    int Ped_Mod1d = 256;
    int Ped_Mod2d = 512;
    int Ped_Mod1e = 1024;
    int Ped_Mod2e = 2048;
    int Ped_Delay1 = 4096;
    int Ped_Reverb1 = 8192;
    int Ped_Cab1 = 16384;
    int Ped_Attack = 32768;
        
    juce::Slider mjSlidVol;
    juce::Slider mjSlidChan;

    juce::Slider mjSlidNGate;
    juce::Slider mjSlidTube;
    juce::Slider mjSlidBite;
    juce::Slider mjSlidFrenzy;
    juce::Label labInputX;

    juce::Slider mjSlidComp_Gain;   //R1.00 Pre Amp Compressor.
    juce::Slider mjSlidComp2_Gain;  //R1.00 Post Amp Compressor.
    juce::Slider mjSlidComp3_Gain;  //R1.00 Dry/Clean Compressor.
    
    juce::Slider mjSlidDrive1;
    juce::Slider mjSlidDrive2;
    juce::Slider mjSlidBreak1;
    juce::Slider mjSlidBreak2;
    juce::Slider mjSlidAsym1;
    
    juce::Slider mjSlidSlope1;
    juce::Slider mjSlidSlope2;
    juce::Slider mjSlidPowerAmp;
    juce::Slider mjSlidGate;
    juce::Slider mjSlidGate2;
    juce::Slider mjSlidTone;        //R1.00 High Pass before Gain. 
    juce::Slider mjSlidTone2;       //R1.00 Low Pass after gain.
    juce::Slider mjSlidTone5;       //R1.00 Mid Boost before gain. 
    juce::Slider mjSlidTone5_Q;     //R1.00 Mid Boost Q.
    juce::Slider mjSlidOD;          //R1.00 Mid Boost Q.
    juce::Slider mjSlidEnvFade;     //R1.00 Mid Boost Q.

    juce::Slider mjSlidVol2;

    juce::Slider mjSlidMix;
    juce::Slider mjSlidMixHP;       //R1.00 Dry path High Pass filter.
    juce::Slider mjSlidMixRect;
    juce::Slider mjSlidTreble;
    juce::Slider mjSlidMid;
    juce::Slider mjSlidBass;
    juce::Slider mjSlidBassCut;
    juce::Slider mjSlidTrebleCut;
    juce::Slider mjSlidTone6;       //R1.00 Mid Boost Final EQ. 
    juce::Slider mjSlidTone6_Q;     //R1.00 Mid Boost Q.
    juce::Label labFinalEQ;
    juce::Slider mjSlidFinalBal;
    juce::Slider mjSlidFinalWidth;

    juce::Slider mjSlidModType;
    juce::Slider mjSlidModLoc;
    juce::Slider mjSlidModMix;
    juce::Slider mjSlidModRate;
    juce::Slider mjSlidModDepth;

    juce::Slider mjSlidMod2Type;
    juce::Slider mjSlidMod2Loc;
    juce::Slider mjSlidMod2Mix;
    juce::Slider mjSlidMod2Rate;
    juce::Slider mjSlidMod2Depth;

    juce::Slider mjSlidDelayMix;
    juce::Slider mjSlidDelayTime;
    juce::Slider mjSlidDelayOffset;
    juce::Slider mjSlidDelayBal;

    juce::Slider mjSlidReverbMix;
    juce::Slider mjSlidReverbTime;
    juce::Slider mjSlidReverbBal;

    juce::Slider mjSlidIRL;
    juce::Slider mjSlidIRR;
    juce::Slider mjSlidIRLMix;
    juce::Slider mjSlidIRRMix;
    juce::Slider mjSlidIRLSize;
    juce::Slider mjSlidIRRSize;

    juce::Slider mjSlidSynthMix;

    juce::Label labVU_Out1;
    juce::Label labVU_Out2;
    juce::Label labVU_Out3;
    juce::Label labVU_Out4;
    juce::Label labVU_Out5;
    juce::Label labVU_Out6;
    bool labVU_Out1_On = false;
    bool labVU_Out2_On = false;
    bool labVU_Out3_On = false;
    bool labVU_Out4_On = false;
    bool labVU_Out5_On = false;
    bool labVU_Out6_On = false;
    bool labVU_Out1_On_Last = false;
    bool labVU_Out2_On_Last = false;
    bool labVU_Out3_On_Last = false;
    bool labVU_Out4_On_Last = false;
    bool labVU_Out5_On_Last = false;
    bool labVU_Out6_On_Last = false;

    juce::Label labVU_In1;
    juce::Label labVU_In2;
    juce::Label labVU_In3;
    juce::Label labVU_In4;
    juce::Label labVU_In5;
    juce::Label labVU_In6;
    bool labVU_In1_On = false;
    bool labVU_In2_On = false;
    bool labVU_In3_On = false;
    bool labVU_In4_On = false;
    bool labVU_In5_On = false;
    bool labVU_In6_On = false;
    bool labVU_In1_On_Last = false;
    bool labVU_In2_On_Last = false;
    bool labVU_In3_On_Last = false;
    bool labVU_In4_On_Last = false;
    bool labVU_In5_On_Last = false;
    bool labVU_In6_On_Last = false;

    juce::Label labPreset;

    juce::Label labChan;
    juce::Label labMod1;
    juce::Label labMod2;
    juce::Label labFX1;
    juce::Label labClip;
        
    juce::Label labStage1;
    juce::Label labStage2;

    juce::TextButton butFreqTest;
    juce::TextButton butTune;
    juce::ComboBox cbTune;
    void cbTuneChanged();
    
    juce::TextButton butEQPreCut;
    juce::TextButton butInvMid;
    
    juce::TextButton butPreset01;
    juce::TextButton butPreset02;
    juce::TextButton butPreset03;
    juce::TextButton butPreset04;
    juce::TextButton butPreset05;
    juce::TextButton butPreset06;
    juce::TextButton butPreset07;
    juce::TextButton butPreset08;
    juce::TextButton butPreset09;
    juce::TextButton butPreset10;
    juce::TextButton butPreset11;
    juce::TextButton butPreset12;
    juce::TextButton butPreset13;
    juce::TextButton butPreset14;
    juce::TextButton butPreset15;
    juce::TextButton butPreset16;
    juce::TextButton butPreset17;
    juce::TextButton butPreset18;
    juce::TextButton butPreset19;
    juce::TextButton butPreset20;

    //*********************************************************
    //R1.00 Setup Clipboard buttons and variables.
    //*********************************************************
    juce::TextButton butClipAGet;
    juce::TextButton butClipASet;
    juce::TextButton butClipBGet;
    juce::TextButton butClipBSet;

    float clpA_Gain = 0.0f;
    int clpA_Chan = 0;
    float clpA_Input_Tone = 150.0f;
    float clpA_Input_MidF = 700.0f;
    float clpA_Input_MidQ = 700.0f;
    float clpA_FX_OD = 0.0f;
    float clpA_FX_Attack = 0.0f;
    float clpA_FX_NGate = 0.0f;
    float clpA_FX_Tube = 0.0f;
    float clpA_FX_Bite = 0.0f;
    float clpA_FX_Frenzy = 0.0f;
    float clpA_S1_Comp = 0.0f;
    float clpA_S1_Break = 0.0f;
    float clpA_S1_Slope = 0.0f;
    float clpA_S2_PowerAmp = 0.0f;
    float clpA_S2_Comp = 0.0f;
    float clpA_Fizz = 0.0f;
    float clpA_Dry_Mix = 0.0f;
    bool clpA_Dry_InvMid = false;
    float clpA_Dry_Comp = 0.0f;
    float clpA_Dry_HighPassF = 0.0f;    
    float clpA_Final_Treble = 0.0f;
    float clpA_Final_Mid = 0.0f;
    float clpA_Final_Bass = 0.0f;
    float clpA_Final_TrebleF = 0.0f;
    float clpA_Final_MidF = 0.0f;
    float clpA_Final_MidQ = 0.0f;
    float clpA_Final_BassF = 0.0f;
    float clpA_Final_Vol = 0.0f;
    float clpA_Final_Bal = 0.0f;
    float clpA_Final_Width = 0.0f;
    float clpA_Mod1_Type = 0.0f;
    float clpA_Mod1_Loc = 0.0f;
    float clpA_Mod1_Rate = 0.0f;
    float clpA_Mod1_Mix = 0.0f;
    float clpA_Mod1_Depth = 0.0f;
    float clpA_Mod2_Type = 0.0f;
    float clpA_Mod2_Loc = 0.0f;
    float clpA_Mod2_Rate = 0.0f;
    float clpA_Mod2_Mix = 0.0f;
    float clpA_Mod2_Depth = 0.0f;
    float clpA_Delay_Time = 0.0f;
    float clpA_Delay_Mix = 0.0f;
    float clpA_Delay_Offset = 0.0f;
    float clpA_Delay_Bal = 0.0f;
    float clpA_Reverb_Time = 0.0f;
    float clpA_Reverb_Mix = 0.0f;
    float clpA_Reverb_Bal = 0.0f;
    int clpA_IRL = 0;
    float clpA_IRL_Voice = 0.5f;
    float clpA_IRL_Size = 0.5f;
    int clpA_IRR = 0;
    float clpA_IRR_Voice = 0.5f;
    float clpA_IRR_Size = 0.5f;
    bool clpA_But_EQPreCut = false;

    float clpB_Gain = 0.0f;
    int clpB_Chan = 0;
    float clpB_Input_Tone = 150.0f;
    float clpB_Input_MidF = 700.0f;
    float clpB_Input_MidQ = 700.0f;
    float clpB_FX_OD = 0.0f;
    float clpB_FX_Attack = 0.0f;
    float clpB_FX_NGate = 0.0f;
    float clpB_FX_Tube = 0.0f;
    float clpB_FX_Bite = 0.0f;
    float clpB_FX_Frenzy = 0.0f;
    float clpB_S1_Comp = 0.0f;
    float clpB_S1_Break = 0.0f;
    float clpB_S1_Slope = 0.0f;
    float clpB_S2_PowerAmp = 0.0f;
    float clpB_S2_Comp = 0.0f;
    float clpB_Fizz = 0.0f;
    float clpB_Dry_Mix = 0.0f;
    bool clpB_Dry_InvMid = false;
    float clpB_Dry_Comp = 0.0f;
    float clpB_Dry_HighPassF = 0.0f;
    float clpB_Final_Treble = 0.0f;
    float clpB_Final_Mid = 0.0f;
    float clpB_Final_Bass = 0.0f;
    float clpB_Final_TrebleF = 0.0f;
    float clpB_Final_MidF = 0.0f;
    float clpB_Final_MidQ = 0.0f;
    float clpB_Final_BassF = 0.0f;
    float clpB_Final_Vol = 0.0f;
    float clpB_Final_Bal = 0.0f;
    float clpB_Final_Width = 0.0f;
    float clpB_Mod1_Type = 0.0f;
    float clpB_Mod1_Loc = 0.0f;
    float clpB_Mod1_Rate = 0.0f;
    float clpB_Mod1_Mix = 0.0f;
    float clpB_Mod1_Depth = 0.0f;
    float clpB_Mod2_Type = 0.0f;
    float clpB_Mod2_Loc = 0.0f;
    float clpB_Mod2_Rate = 0.0f;
    float clpB_Mod2_Mix = 0.0f;
    float clpB_Mod2_Depth = 0.0f;
    float clpB_Delay_Time = 0.0f;
    float clpB_Delay_Mix = 0.0f;
    float clpB_Delay_Offset = 0.0f;
    float clpB_Delay_Bal = 0.0f;
    float clpB_Reverb_Time = 0.0f;
    float clpB_Reverb_Mix = 0.0f;
    float clpB_Reverb_Bal = 0.0f;
    int clpB_IRL = 0;
    float clpB_IRL_Voice = 0.5f;
    float clpB_IRL_Size = 0.5f;
    int clpB_IRR = 0;
    float clpB_IRR_Voice = 0.5f;
    float clpB_IRR_Size = 0.5f;
    bool clpB_But_EQPreCut = false;

    int Num_Normalize(float V);
    void GUI_Mod1_Visible(bool Vstate);
    void GUI_Mod2_Visible(bool Vstate);

    void Mako_Set_Mod(int mType, int mLoc, float mRate, float mMix, float mDepth);
    void Mako_Set_Mod2(int mType, int mLoc, float mRate, float mMix, float mDepth);
    void Mako_Set_Delay(float mDelay, float mMix, float mOffset, float mBalance);

    //void Mako_Opt_Frenzy(int Mode);
    void Mako_Opt_EQPreCut(bool Mode, bool UpdateButton);
    void Mako_Opt_InvMid(bool Mode, bool UpdateButton);
    
    void Mako_FreqTest(bool Mode, bool UpdateButton);

    void Mako_Opt_Initialize();

    void Mako_Set_Input(float Channel, int LowPassFreq, int MidFreq, float MidQ);
    void Mako_Set_Stages(float Comp1, float Slope1, float Round1, float Asym, float PowerAmp, float Comp2);   //float Comp2, float Slope2, float Round2);
    void Mako_Set_FinalEQ(float Treble, float Mid, float Bass, int HiCut, int LowCut, int MidCut, float MidQ);
    void Mako_Set_Volume(float Gain, float Volume);
    void Mako_Set_IR(int IR1, float Voice1, float SpeakerSize1, int IR2, float Voice2, float SpeakerSize2);
    void Mako_Set_Reverb(float Mix, float Decay, float Balance);

    void GFX_Pedals_Draw(bool ForceUpdate);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MakoDist01AudioProcessorEditor)


public:
    
    //R1.00 We need to make a new PUBLIC here so these variables DESTRUCT BEFORE the private objects they attach to. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Gain;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Chan;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone;     //R1.00 Input Low Cut.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone5;    //R1.00 Input Mid. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone5_Q;  //R1.00 Input Mid Q. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_OD;       //R1.00 Input Mid. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_NGate;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tube;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Bite;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Frenzy;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Break1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Break2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Slope1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Slope2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Asym1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_PowerAmp;    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Comp1;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Comp2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Comp3;     //R1.00 Clean Compressor
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone2;     //R1.00 High Cut.
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mix;       //R1.00 Clean Mix. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_MixHP;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> makoSA_InvMid;
    std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> makoSA_EQPreCut;
    
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Treble;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mid;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Bass;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_TrebleCut;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_BassCut;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone6;     //R1.00 Output Mid. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Tone6_Q;   //R1.00 Output Mid Q. 
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Vol2;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_FinalBal;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_FinalWidth;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ModType;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ModLoc;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ModRate;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ModMix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ModDepth;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mod2Type;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mod2Loc;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mod2Rate;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mod2Mix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_Mod2Depth;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_DelayTime;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_DelayMix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_DelayOffset;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_DelayBal;

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ReverbMix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ReverbTime;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_ReverbBal;        

    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRL;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRR;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRLMix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRRMix;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRLSize;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> makoSA_IRRSize;

    //std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> makoSA_Chan0;
    //std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> makoSA_Chan1;
    //std::unique_ptr <juce::AudioProcessorValueTreeState::ButtonAttachment> makoSA_Chan2;
    

};
