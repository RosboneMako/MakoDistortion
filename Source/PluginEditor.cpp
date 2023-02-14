/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Windows.h"

//==============================================================================
MakoDist01AudioProcessorEditor::MakoDist01AudioProcessorEditor (MakoDist01AudioProcessor& p)
    : AudioProcessorEditor (&p), makoProcessor (p)
{
    imgBack = juce::ImageCache::getFromMemory(BinaryData::md_back_08_png, BinaryData::md_back_08_pngSize);
    imgPedals = juce::ImageCache::getFromMemory(BinaryData::md_pedallayout_01_png, BinaryData::md_pedallayout_01_pngSize);
    imgPedal = juce::ImageCache::getFromMemory(BinaryData::md_pedal_02_png, BinaryData::md_pedal_02_pngSize);
    imgPedal_PA = juce::ImageCache::getFromMemory(BinaryData::md_pedal_pa_png, BinaryData::md_pedal_pa_pngSize);
    imgPedal_EQ = juce::ImageCache::getFromMemory(BinaryData::md_pedal_eq_png, BinaryData::md_pedal_eq_pngSize);
    imgLogGraph = juce::ImageCache::getFromMemory(BinaryData::md_graph_log_01_png, BinaryData::md_graph_log_01_pngSize);

    //R1.00 GFX Indicate which pedals are in use.
    PEDAL_ON = PEDAL_ON | Ped_Mod1a;
    PEDAL_ON = PEDAL_ON | Ped_Mod2a;
    PEDAL_ON = PEDAL_ON | Ped_Cab1;
    PEDAL_ON = PEDAL_ON | Ped_Delay1;
    PEDAL_ON = PEDAL_ON | Ped_Reverb1;

    startTimerHz(10);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (819, 330);
    
    //R1.00 Create SLIDER ATTACHMENTS so our TREE STATE vars get adjusted automatically for Get/Set states.
    makoSA_Gain = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (makoProcessor.parameters, "gain", mjSlidVol);
    makoSA_Chan = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "chan", mjSlidChan);

    makoSA_Tone = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone", mjSlidTone);
    makoSA_Tone5 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone5", mjSlidTone5);
    makoSA_Tone5_Q = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone5_q", mjSlidTone5_Q);
    makoSA_OD = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "od", mjSlidOD);
    makoSA_OD = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "envfade", mjSlidEnvFade);
    makoSA_NGate = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "ngate", mjSlidNGate);
    makoSA_Tube = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tube", mjSlidTube);
    makoSA_Bite = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "bite", mjSlidBite);
    makoSA_Frenzy = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "frenzy", mjSlidFrenzy);
    makoSA_Comp1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "comp1", mjSlidComp_Gain);
    makoSA_Comp2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "comp2", mjSlidComp2_Gain);
    makoSA_Comp3 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "comp3", mjSlidComp3_Gain);
    makoSA_Break1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "break1", mjSlidBreak1);
    makoSA_Break2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "break2", mjSlidBreak2);
    makoSA_Slope1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (makoProcessor.parameters, "slope1", mjSlidSlope1);
    makoSA_Slope2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (makoProcessor.parameters, "slope2", mjSlidSlope2);
    makoSA_Asym1 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "asym1", mjSlidAsym1);
    makoSA_PowerAmp = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "poweramp", mjSlidPowerAmp);
    makoSA_Tone2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone2", mjSlidTone2);
    makoSA_Mix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mix", mjSlidMix);
    makoSA_MixHP = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mixhp", mjSlidMixHP);
    makoSA_InvMid = std::make_unique <juce::AudioProcessorValueTreeState::ButtonAttachment>(makoProcessor.parameters, "invmid", butInvMid);
    makoSA_EQPreCut = std::make_unique <juce::AudioProcessorValueTreeState::ButtonAttachment>(makoProcessor.parameters, "eqprecut", butEQPreCut);
    
    makoSA_Treble = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "treble", mjSlidTreble);
    makoSA_Mid = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mid", mjSlidMid);
    makoSA_Bass = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "bass", mjSlidBass);
    makoSA_TrebleCut = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "treblecut", mjSlidTrebleCut);
    makoSA_BassCut = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "basscut", mjSlidBassCut);
    makoSA_Tone6 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone6", mjSlidTone6);
    makoSA_Tone6_Q = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "tone6_q", mjSlidTone6_Q);
    makoSA_Vol2 = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "vol2", mjSlidVol2);
    makoSA_FinalBal = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "finalbal", mjSlidFinalBal);
    makoSA_FinalWidth = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "finalwidth", mjSlidFinalWidth);

    makoSA_ModType = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "modtype", mjSlidModType);
    makoSA_ModLoc = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "modloc", mjSlidModLoc);
    makoSA_ModRate = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "modrate", mjSlidModRate);
    makoSA_ModMix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "modmix", mjSlidModMix);
    makoSA_ModDepth = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "moddepth", mjSlidModDepth);

    makoSA_Mod2Type = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mod2type", mjSlidMod2Type);
    makoSA_Mod2Loc = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mod2loc", mjSlidMod2Loc);
    makoSA_Mod2Rate = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mod2rate", mjSlidMod2Rate);
    makoSA_Mod2Mix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mod2mix", mjSlidMod2Mix);
    makoSA_Mod2Depth = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "mod2depth", mjSlidMod2Depth);

    makoSA_DelayTime = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "delaytime", mjSlidDelayTime);
    makoSA_DelayMix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "delaymix", mjSlidDelayMix);
    makoSA_DelayOffset = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "delayoffset", mjSlidDelayOffset);
    makoSA_DelayBal = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "delaybal", mjSlidDelayBal);
    
    makoSA_ReverbMix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "reverbmix", mjSlidReverbMix);
    makoSA_ReverbTime = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "reverbtime", mjSlidReverbTime);
    makoSA_ReverbBal = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "reverbbal", mjSlidReverbBal);

    makoSA_IRL = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irl", mjSlidIRL);
    makoSA_IRR = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irr", mjSlidIRR);
    makoSA_IRLMix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irlmix", mjSlidIRLMix);
    makoSA_IRRMix = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irrmix", mjSlidIRRMix);
    makoSA_IRLSize = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irlsize", mjSlidIRLSize);
    makoSA_IRRSize = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "irrsize", mjSlidIRRSize);

    //makoSA_Chan0 = std::make_unique <juce::AudioProcessorValueTreeState::ButtonAttachment>(makoProcessor.parameters, "chan0", butChan0);
    
    //****************************************************************************************
    //R1.00 Add GUI CONTROLS
    //****************************************************************************************
    GUI_Init_Large_Slider(&mjSlidVol);
    mjSlidVol.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mjSlidVol.setRange(0.0, 1.0, .01);
    mjSlidVol.setValue(makoProcessor.makoVol); 
    mjSlidVol.addListener(this);
    addAndMakeVisible(mjSlidVol);
       
    GUI_Init_Large_Slider(&mjSlidChan);
    mjSlidChan.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mjSlidChan.setRange(0, 3, 1);
    mjSlidChan.setValue(makoProcessor.makoChan); 
    mjSlidChan.addListener(this);
    addAndMakeVisible(mjSlidChan);

    labChan.setText("Clean", juce::dontSendNotification);
    labChan.setJustificationType(juce::Justification::centred);
    labChan.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labChan.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labChan.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labChan);

    //****************************************************************************************
    //R1.00 INPUT EQ
    //****************************************************************************************
    GUI_Init_Small_Slider(&mjSlidTone);
    mjSlidTone.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTone.setRange(makoProcessor.makoTone_Min, makoProcessor.makoTone_Max, 10);
    mjSlidTone.setValue(makoProcessor.makoTone);
    mjSlidTone.addListener(this);
    addAndMakeVisible(mjSlidTone);

    //R1.00 Mid Boost
    GUI_Init_Small_Slider(&mjSlidTone5);
    mjSlidTone5.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTone5.setRange(makoProcessor.makoTone5_Min, makoProcessor.makoTone5_Max, 25);
    mjSlidTone5.setValue(makoProcessor.makoTone5);
    mjSlidTone5.addListener(this);
    addAndMakeVisible(mjSlidTone5);

    GUI_Init_Small_Slider(&mjSlidTone5_Q);
    mjSlidTone5_Q.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTone5_Q.setRange(makoProcessor.makoTone5_Q_Min, makoProcessor.makoTone5_Q_Max, .001);
    mjSlidTone5_Q.setValue(makoProcessor.makoTone5_Q);
    mjSlidTone5_Q.addListener(this);
    addAndMakeVisible(mjSlidTone5_Q);

    // ***********************************************
    //R1.00 INPUT OPTIONS
    // ***********************************************
    //R1.00 OverDrive
    GUI_Init_Small_Slider(&mjSlidOD);
    mjSlidOD.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidOD.setRange(0, 1, .01);
    mjSlidOD.setValue(makoProcessor.makoOD);
    mjSlidOD.addListener(this);
    addAndMakeVisible(mjSlidOD);

    //R1.00 Attack/Envelope.
    GUI_Init_Small_Slider(&mjSlidEnvFade);
    mjSlidEnvFade.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidEnvFade.setRange(0, 1, .01);
    mjSlidEnvFade.setValue(makoProcessor.makoEnvFade);
    mjSlidEnvFade.addListener(this);
    addAndMakeVisible(mjSlidEnvFade);
        
    //R1.00 Noise Gate
    GUI_Init_Small_Slider(&mjSlidNGate);
    mjSlidNGate.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidNGate.setRange(makoProcessor.makoparmNGate_Min, makoProcessor.makoparmNGate_Max, .01);
    mjSlidNGate.setValue(makoProcessor.makoparmNGate);
    mjSlidNGate.addListener(this);
    addAndMakeVisible(mjSlidNGate);

    //R1.00 Tube
    GUI_Init_Small_Slider(&mjSlidTube);
    mjSlidTube.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTube.setRange(0.0, 1.0, .01);
    mjSlidTube.setValue(makoProcessor.makoparmTube);
    mjSlidTube.addListener(this);
    addAndMakeVisible(mjSlidTube);

    //R1.00 Bite
    GUI_Init_Small_Slider(&mjSlidBite);
    mjSlidBite.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidBite.setRange(0.0, 1.0, .01);
    mjSlidBite.setValue(makoProcessor.makoBiteGain);
    mjSlidBite.addListener(this);
    addAndMakeVisible(mjSlidBite);

    //R1.00 Frenzy
    GUI_Init_Small_Slider(&mjSlidFrenzy);
    mjSlidFrenzy.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidFrenzy.setRange(0.0, 1.0, .01);
    mjSlidFrenzy.setValue(makoProcessor.makoFrenzy);
    mjSlidFrenzy.addListener(this);
    addAndMakeVisible(mjSlidFrenzy);

    labInputX.setText("---", juce::dontSendNotification);
    labInputX.setJustificationType(juce::Justification::centred);
    labInputX.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labInputX.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labInputX.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labInputX);

    
    //****************************************************************************************
    //R1.00 STAGE 1
    //****************************************************************************************
    GUI_Init_Small_Slider(&mjSlidComp_Gain);
    mjSlidComp_Gain.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidComp_Gain.setRange(.0f, 1.0f, .01f);
    mjSlidComp_Gain.setValue(makoProcessor.makoparmComp_Gain);
    mjSlidComp_Gain.addListener(this);
    addAndMakeVisible(mjSlidComp_Gain);

    GUI_Init_Small_Slider(&mjSlidBreak1);
    mjSlidBreak1.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidBreak1.setRange(0.0, 1.0, .01);
    mjSlidBreak1.setValue(makoProcessor.makoBreak);
    mjSlidBreak1.addListener(this);
    addAndMakeVisible(mjSlidBreak1);

    GUI_Init_Small_Slider(&mjSlidSlope1);
    mjSlidSlope1.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidSlope1.setRange(0.0, 1.0, .01);
    mjSlidSlope1.setValue(makoProcessor.makoSlope);
    mjSlidSlope1.addListener(this);
    addAndMakeVisible(mjSlidSlope1);
    
    GUI_Init_Small_Slider(&mjSlidAsym1);
    mjSlidAsym1.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidAsym1.setRange(0.0, 1.0, .01);
    mjSlidAsym1.setValue(makoProcessor.makoAsym1);
    mjSlidAsym1.addListener(this);
    addAndMakeVisible(mjSlidAsym1);

    labStage1.setText("---", juce::dontSendNotification);
    labStage1.setJustificationType(juce::Justification::centred);
    labStage1.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labStage1.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labStage1.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labStage1);
        
    //**************************************************************************************** 
    //R1.00 STAGE 2
    //****************************************************************************************
    
    GUI_Init_Small_Slider(&mjSlidComp2_Gain);
    mjSlidComp2_Gain.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidComp2_Gain.setRange(.0f, 1.0f, .01f);
    mjSlidComp2_Gain.setValue(makoProcessor.makoparmComp2_Gain);
    mjSlidComp2_Gain.addListener(this);
    addAndMakeVisible(mjSlidComp2_Gain);
    
    /*
    GUI_Init_Small_Slider(&mjSlidBreak2);
    mjSlidBreak2.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidBreak2.setRange(0.0, 1.0, .01);
    mjSlidBreak2.setValue(makoProcessor.makoBreak2);
    mjSlidBreak2.addListener(this);
    addAndMakeVisible(mjSlidBreak2);
    
    GUI_Init_Small_Slider(&mjSlidSlope2);
    mjSlidSlope2.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidSlope2.setRange(0.0, 1.0, .01);
    mjSlidSlope2.setValue(makoProcessor.makoSlope2);
    mjSlidSlope2.addListener(this);
    addAndMakeVisible(mjSlidSlope2);
    */
    
    GUI_Init_Small_Slider(&mjSlidPowerAmp);
    mjSlidPowerAmp.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidPowerAmp.setRange(0.0, 1.0, .01);
    mjSlidPowerAmp.setValue(makoProcessor.makoPowerAmp);
    mjSlidPowerAmp.addListener(this);
    addAndMakeVisible(mjSlidPowerAmp);
        
    labStage2.setText("---", juce::dontSendNotification);
    labStage2.setJustificationType(juce::Justification::centred);
    labStage2.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labStage2.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labStage2.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labStage2);

    labPreset.setText("none", juce::dontSendNotification);
    labPreset.setJustificationType(juce::Justification::centred);
    labPreset.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labPreset.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));         // Colour(0xFF8B9FA5));
    labPreset.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labPreset);

    //R1.00 LOW PASS FILTER - FIZZ
    GUI_Init_Large_Slider(&mjSlidTone2);
    mjSlidTone2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mjSlidTone2.setRange(makoProcessor.makoTone2_Min, makoProcessor.makoTone2_Max, 50);
    mjSlidTone2.setValue(makoProcessor.makoTone2);
    mjSlidTone2.addListener(this);
    addAndMakeVisible(mjSlidTone2);
    
    //**************************************************************************************** 
    //R1.00 CLEAN PATH
    //****************************************************************************************
    GUI_Init_Small_Slider(&mjSlidComp3_Gain);
    mjSlidComp3_Gain.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidComp3_Gain.setRange(.01f, 1.0f, .01f);
    mjSlidComp3_Gain.setValue(makoProcessor.makoparmComp3_Gain);
    mjSlidComp3_Gain.addListener(this);
    addAndMakeVisible(mjSlidComp3_Gain);

    GUI_Init_Small_Slider(&mjSlidMixHP);
    mjSlidMixHP.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMixHP.setRange(makoProcessor.makoMixHP_Min, makoProcessor.makoMixHP_Max, 10);
    mjSlidMixHP.setValue(makoProcessor.makoMixHP);
    mjSlidMixHP.addListener(this);
    addAndMakeVisible(mjSlidMixHP);
    
    GUI_Init_Small_Slider(&mjSlidMix);
    mjSlidMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMix.setRange(0.0, 1.0, .01);
    mjSlidMix.setValue(makoProcessor.makoMix);
    mjSlidMix.addListener(this);
    addAndMakeVisible(mjSlidMix);
    
    //**********************************************
    //R1.00 FINAL EQ
    //**********************************************
    GUI_Init_Large_Slider(&mjSlidTreble);
    mjSlidTreble.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mjSlidTreble.setRange(0.0, 1.0, .01);
    mjSlidTreble.setValue(makoProcessor.makoTreble);
    mjSlidTreble.addListener(this);
    addAndMakeVisible(mjSlidTreble);

    GUI_Init_Large_Slider(&mjSlidMid);
    mjSlidMid.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mjSlidMid.setRange(0.0, 1.0, .01);
    mjSlidMid.setValue(makoProcessor.makoMid);
    mjSlidMid.addListener(this);
    addAndMakeVisible(mjSlidMid);

    GUI_Init_Large_Slider(&mjSlidBass);
    mjSlidBass.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mjSlidBass.setRange(0.0, 1.0, .01);
    mjSlidBass.setValue(makoProcessor.makoBass);
    mjSlidBass.addListener(this);
    addAndMakeVisible(mjSlidBass);

    GUI_Init_Small_Slider(&mjSlidTrebleCut);
    mjSlidTrebleCut.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTrebleCut.setRange(500, 3000, 25);
    mjSlidTrebleCut.setValue(makoProcessor.makoTrebleCut);
    mjSlidTrebleCut.addListener(this);
    addAndMakeVisible(mjSlidTrebleCut);

    GUI_Init_Small_Slider(&mjSlidBassCut);
    mjSlidBassCut.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidBassCut.setRange(80, 400, 5);
    mjSlidBassCut.setValue(makoProcessor.makoBassCut);
    mjSlidBassCut.addListener(this);
    addAndMakeVisible(mjSlidBassCut);

    //R1.00 FINAL EQ - Mid Boost
    GUI_Init_Small_Slider(&mjSlidTone6);
    mjSlidTone6.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTone6.setRange(makoProcessor.makoTone6_Min, makoProcessor.makoTone6_Max, 25);
    mjSlidTone6.setValue(makoProcessor.makoTone6);
    mjSlidTone6.addListener(this);
    addAndMakeVisible(mjSlidTone6);

    GUI_Init_Small_Slider(&mjSlidTone6_Q);
    mjSlidTone6_Q.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidTone6_Q.setRange(makoProcessor.makoTone6_Q_Min, makoProcessor.makoTone6_Q_Max, .001);
    mjSlidTone6_Q.setValue(makoProcessor.makoTone6_Q);
    mjSlidTone6_Q.addListener(this);
    addAndMakeVisible(mjSlidTone6_Q);

    GUI_Init_Small_Slider(&mjSlidFinalBal);
    mjSlidFinalBal.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidFinalBal.setRange(0, 1, .01);
    mjSlidFinalBal.setValue(.5f);
    mjSlidFinalBal.addListener(this);
    addAndMakeVisible(mjSlidFinalBal);

    GUI_Init_Small_Slider(&mjSlidFinalWidth);
    mjSlidFinalWidth.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidFinalWidth.setRange(0, 1, .01);
    mjSlidFinalWidth.setValue(makoProcessor.makoFinalWidth);
    mjSlidFinalWidth.addListener(this);
    addAndMakeVisible(mjSlidFinalWidth);

    labFinalEQ.setText("---", juce::dontSendNotification);
    labFinalEQ.setJustificationType(juce::Justification::centred);
    labFinalEQ.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labFinalEQ.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labFinalEQ.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labFinalEQ);
    
    //R1.00 Modulation Type
    GUI_Init_Small_Slider(&mjSlidModType);
    mjSlidModType.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidModType.setRange(0, 5, 1);
    mjSlidModType.setValue(makoProcessor.makoModType);
    mjSlidModType.addListener(this);
    addAndMakeVisible(mjSlidModType);
    
    //R1.00 Modulation Location in signal chain.
    GUI_Init_Small_Slider(&mjSlidModLoc);
    mjSlidModLoc.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidModLoc.setRange(0, 5, 1);
    mjSlidModLoc.setValue(makoProcessor.makoModLoc);
    mjSlidModLoc.addListener(this);
    addAndMakeVisible(mjSlidModLoc);

    //R1.00 MODULATION Rate
    GUI_Init_Small_Slider(&mjSlidModRate);
    mjSlidModRate.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidModRate.setRange(makoProcessor.makoModRate_Min, makoProcessor.makoModRate_Max, .01);
    mjSlidModRate.setValue(makoProcessor.makoModRate);
    mjSlidModRate.addListener(this);
    addAndMakeVisible(mjSlidModRate);

    //R1.00 MODULATION Mix
    GUI_Init_Small_Slider(&mjSlidModMix);
    mjSlidModMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidModMix.setRange(makoProcessor.makoModMix_Min, makoProcessor.makoModMix_Max, .01);
    mjSlidModMix.setValue(makoProcessor.makoModMix);
    mjSlidModMix.addListener(this);
    addAndMakeVisible(mjSlidModMix);

    //R1.00 MODULATION Depth
    GUI_Init_Small_Slider(&mjSlidModDepth);
    mjSlidModDepth.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidModDepth.setRange(makoProcessor.makoModDepth_Min, makoProcessor.makoModDepth_Max, .01);
    mjSlidModDepth.setValue(makoProcessor.makoModDepth);
    mjSlidModDepth.addListener(this);
    addAndMakeVisible(mjSlidModDepth);

    //R1.00 Modulation Type
    GUI_Init_Small_Slider(&mjSlidMod2Type);
    mjSlidMod2Type.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMod2Type.setRange(0, 5, 1);
    mjSlidMod2Type.setValue(makoProcessor.makoMod2Type);
    mjSlidMod2Type.addListener(this);
    addAndMakeVisible(mjSlidMod2Type);

    //R1.00 Modulation Location in signal chain.
    GUI_Init_Small_Slider(&mjSlidMod2Loc);
    mjSlidMod2Loc.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMod2Loc.setRange(0, 5, 1);
    mjSlidMod2Loc.setValue(makoProcessor.makoMod2Loc);
    mjSlidMod2Loc.addListener(this);
    addAndMakeVisible(mjSlidMod2Loc);

    //R1.00 MODULATION Rate
    GUI_Init_Small_Slider(&mjSlidMod2Rate);
    mjSlidMod2Rate.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMod2Rate.setRange(makoProcessor.makoMod2Rate_Min, makoProcessor.makoMod2Rate_Max, .01);
    mjSlidMod2Rate.setValue(makoProcessor.makoMod2Rate);
    mjSlidMod2Rate.addListener(this);
    addAndMakeVisible(mjSlidMod2Rate);

    //R1.00 MODULATION Mix
    GUI_Init_Small_Slider(&mjSlidMod2Mix);
    mjSlidMod2Mix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMod2Mix.setRange(makoProcessor.makoMod2Mix_Min, makoProcessor.makoMod2Mix_Max, .01);
    mjSlidMod2Mix.setValue(makoProcessor.makoMod2Mix);
    mjSlidMod2Mix.addListener(this);
    addAndMakeVisible(mjSlidMod2Mix);

    //R1.00 MODULATION Depth
    GUI_Init_Small_Slider(&mjSlidMod2Depth);
    mjSlidMod2Depth.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidMod2Depth.setRange(makoProcessor.makoMod2Depth_Min, makoProcessor.makoMod2Depth_Max, .01);
    mjSlidMod2Depth.setValue(makoProcessor.makoMod2Depth);
    mjSlidMod2Depth.addListener(this);
    addAndMakeVisible(mjSlidMod2Depth);


    //R1.00 DELAY Time
    GUI_Init_Small_Slider(&mjSlidDelayTime);
    mjSlidDelayTime.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidDelayTime.setRange(makoProcessor.makoDelayTime_Min, makoProcessor.makoDelayTime_Max, .01);
    mjSlidDelayTime.setValue(makoProcessor.makoDelayTime);
    mjSlidDelayTime.addListener(this);
    addAndMakeVisible(mjSlidDelayTime);

    //R1.00 DELAY Mix
    GUI_Init_Small_Slider(&mjSlidDelayMix);
    mjSlidDelayMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidDelayMix.setRange(makoProcessor.makoDelayMix_Min, makoProcessor.makoDelayMix_Max, .01);
    mjSlidDelayMix.setValue(makoProcessor.makoDelayMix);
    mjSlidDelayMix.addListener(this);
    addAndMakeVisible(mjSlidDelayMix);

    //R1.00 DELAY Offset
    GUI_Init_Small_Slider(&mjSlidDelayOffset);
    mjSlidDelayOffset.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidDelayOffset.setRange(0, 1, .01);
    mjSlidDelayOffset.setValue(makoProcessor.makoDelayOffset);
    mjSlidDelayOffset.addListener(this);
    addAndMakeVisible(mjSlidDelayOffset);

    GUI_Init_Small_Slider(&mjSlidDelayBal);
    mjSlidDelayBal.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidDelayBal.setRange(0, 1, .01);
    mjSlidDelayBal.setValue(makoProcessor.makoDelayBal);
    mjSlidDelayBal.addListener(this);
    addAndMakeVisible(mjSlidDelayBal);
    
    //R1.00 REVERB 
    GUI_Init_Small_Slider(&mjSlidReverbMix);
    mjSlidReverbMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidReverbMix.setRange(makoProcessor.makoReverbMix_Min, makoProcessor.makoReverbMix_Max, .01);
    mjSlidReverbMix.setValue(makoProcessor.makoReverbMix);
    mjSlidReverbMix.addListener(this);
    addAndMakeVisible(mjSlidReverbMix);
    
    GUI_Init_Small_Slider(&mjSlidReverbTime);
    mjSlidReverbTime.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidReverbTime.setRange(0, 1, .01);
    mjSlidReverbTime.setValue(makoProcessor.makoReverbTime);
    mjSlidReverbTime.addListener(this);
    addAndMakeVisible(mjSlidReverbTime);

    GUI_Init_Small_Slider(&mjSlidReverbBal);
    mjSlidReverbBal.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidReverbBal.setRange(0, 1, .01);
    mjSlidReverbBal.setValue(makoProcessor.makoReverbBal);
    mjSlidReverbBal.addListener(this);
    addAndMakeVisible(mjSlidReverbBal);
        
    //R1.00 IR Cabinet selection.
    GUI_Init_Small_Slider(&mjSlidIRL);
    mjSlidIRL.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRL.setRange(0, 10, 1);
    mjSlidIRL.setValue(makoProcessor.makoIRL);
    mjSlidIRL.addListener(this);
    addAndMakeVisible(mjSlidIRL);

    //R1.00 IR Cabinet selection.
    GUI_Init_Small_Slider(&mjSlidIRR);
    mjSlidIRR.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRR.setRange(0, 10, 1);
    mjSlidIRR.setValue(makoProcessor.makoIRR);
    mjSlidIRR.addListener(this);
    addAndMakeVisible(mjSlidIRR);

    //R1.00 IR Cabinet Blender.
    GUI_Init_Small_Slider(&mjSlidIRLMix);
    mjSlidIRLMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRLMix.setRange(0.0f, 1.0f, .01f);
    mjSlidIRLMix.setValue(makoProcessor.makoIRLMix);
    mjSlidIRLMix.addListener(this);
    addAndMakeVisible(mjSlidIRLMix);
    
    GUI_Init_Small_Slider(&mjSlidIRRMix);
    mjSlidIRRMix.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRRMix.setRange(0.0f, 1.0f, .01f);
    mjSlidIRRMix.setValue(makoProcessor.makoIRRMix);
    mjSlidIRRMix.addListener(this);
    addAndMakeVisible(mjSlidIRRMix);

    //R1.00 IR Speaker size.
    GUI_Init_Small_Slider(&mjSlidIRLSize);
    mjSlidIRLSize.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRLSize.setRange(0.0f, 1.0f, .01f);
    mjSlidIRLSize.setValue(makoProcessor.makoIRLSize);
    mjSlidIRLSize.addListener(this);
    addAndMakeVisible(mjSlidIRLSize);

    GUI_Init_Small_Slider(&mjSlidIRRSize);
    mjSlidIRRSize.setSliderStyle(juce::Slider::LinearHorizontal);
    mjSlidIRRSize.setRange(0.0f, 1.0f, .01f);
    mjSlidIRRSize.setValue(makoProcessor.makoIRRSize);
    mjSlidIRRSize.addListener(this);
    addAndMakeVisible(mjSlidIRRSize);

    labMod1.setText("Chorus", juce::dontSendNotification);
    labMod1.setJustificationType(juce::Justification::centred);
    labMod1.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labMod1.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labMod1.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labMod1);

    labMod2.setText("Chorus", juce::dontSendNotification);
    labMod2.setJustificationType(juce::Justification::centred);
    labMod2.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labMod2.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labMod2.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labMod2);


    labFX1.setText("FX values", juce::dontSendNotification);
    labFX1.setJustificationType(juce::Justification::centred);
    labFX1.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labFX1.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labFX1.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labFX1);


    //R1.00 FINAL VOLUME
    GUI_Init_Large_Slider(&mjSlidVol2);
    mjSlidVol2.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    mjSlidVol2.setRange(0.0, 1.0, .01);
    mjSlidVol2.setValue(makoProcessor.makoVol2);
    mjSlidVol2.addListener(this);
    addAndMakeVisible(mjSlidVol2);
    
    //R1.00 OPTIONS.
    butInvMid.setButtonText("Invert Mids");
    butInvMid.setClickingTogglesState(true);
    butInvMid.setToggleState(false, false);
    butInvMid.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butInvMid.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butInvMid.setColour(juce::TextButton::buttonOnColourId, juce::Colour(100, 120, 130));
    butInvMid.addListener(this);
    addAndMakeVisible(butInvMid);

    butEQPreCut.setButtonText("EQ Pre/Post");
    butEQPreCut.setClickingTogglesState(true);
    butEQPreCut.setToggleState(false, false);
    butEQPreCut.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butEQPreCut.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butEQPreCut.setColour(juce::TextButton::buttonOnColourId, juce::Colour(100, 120, 130));
    butEQPreCut.addListener(this);
    addAndMakeVisible(butEQPreCut);    
    
    //R1.00 PRESETS.
    butPreset01.setButtonText("Clean");
    butPreset01.setClickingTogglesState(false);
    butPreset01.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset01.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset01.addListener(this);
    addAndMakeVisible(butPreset01);
    butPreset02.setButtonText("Gritty");
    butPreset02.setClickingTogglesState(false);
    butPreset02.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset02.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset02.addListener(this);
    addAndMakeVisible(butPreset02);
    butPreset03.setButtonText("Ether");
    butPreset03.setClickingTogglesState(false);
    butPreset03.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset03.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset03.addListener(this);
    addAndMakeVisible(butPreset03);
    butPreset04.setButtonText("VH Live");
    butPreset04.setClickingTogglesState(false);
    butPreset04.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset04.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset04.addListener(this);
    addAndMakeVisible(butPreset04);
    butPreset05.setButtonText("90's");
    butPreset05.setClickingTogglesState(false);
    butPreset05.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset05.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset05.addListener(this);
    addAndMakeVisible(butPreset05);
    butPreset06.setButtonText("Cocked");
    butPreset06.setClickingTogglesState(false);
    butPreset06.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset06.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset06.addListener(this);
    addAndMakeVisible(butPreset06);
    butPreset07.setButtonText("FuzzLord");
    butPreset07.setClickingTogglesState(false);
    butPreset07.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset07.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset07.addListener(this);
    addAndMakeVisible(butPreset07);
    butPreset08.setButtonText("Proggy");
    butPreset08.setClickingTogglesState(false);
    butPreset08.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset08.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset08.addListener(this);
    addAndMakeVisible(butPreset08);
    butPreset09.setButtonText("Strexi");
    butPreset09.setClickingTogglesState(false);
    butPreset09.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset09.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset09.addListener(this);
    addAndMakeVisible(butPreset09);
    butPreset10.setButtonText("Strex2");
    butPreset10.setClickingTogglesState(false);
    butPreset10.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset10.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset10.addListener(this);
    addAndMakeVisible(butPreset10);
    butPreset11.setButtonText("Acous 1");
    butPreset11.setClickingTogglesState(false);
    butPreset11.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset11.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset11.addListener(this);
    addAndMakeVisible(butPreset11);
    butPreset12.setButtonText("VH1");
    butPreset12.setClickingTogglesState(false);
    butPreset12.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset12.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset12.addListener(this);
    addAndMakeVisible(butPreset12);
    butPreset13.setButtonText("Metal 1");
    butPreset13.setClickingTogglesState(false);
    butPreset13.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset13.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset13.addListener(this);
    addAndMakeVisible(butPreset13);
    butPreset14.setButtonText("AirJo");
    butPreset14.setClickingTogglesState(false);
    butPreset14.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset14.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset14.addListener(this);
    addAndMakeVisible(butPreset14);
    butPreset15.setButtonText("Clean 2");
    butPreset15.setClickingTogglesState(false);
    butPreset15.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset15.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset15.addListener(this);
    addAndMakeVisible(butPreset15);
    butPreset16.setButtonText("Thicc Boi");
    butPreset16.setClickingTogglesState(false);
    butPreset16.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset16.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset16.addListener(this);
    addAndMakeVisible(butPreset16);
    butPreset17.setButtonText("Thic Syn");
    butPreset17.setClickingTogglesState(false);
    butPreset17.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset17.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset17.addListener(this);
    addAndMakeVisible(butPreset17);
    butPreset18.setButtonText("Clean 3");
    butPreset18.setClickingTogglesState(false);
    butPreset18.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset18.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset18.addListener(this);
    addAndMakeVisible(butPreset18);
    butPreset19.setButtonText("Marino");
    butPreset19.setClickingTogglesState(false);
    butPreset19.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset19.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset19.addListener(this);
    addAndMakeVisible(butPreset19);
    butPreset20.setButtonText("Punx");
    butPreset20.setClickingTogglesState(false);
    butPreset20.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butPreset20.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butPreset20.addListener(this);
    addAndMakeVisible(butPreset20);
            
    //R1.00 SETUP CLIPBOARD
    butClipASet.setButtonText("--> A");
    butClipASet.setClickingTogglesState(false);
    butClipASet.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butClipASet.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butClipASet.addListener(this);
    addAndMakeVisible(butClipASet);
    butClipAGet.setButtonText("A -->");
    butClipAGet.setClickingTogglesState(false);
    butClipAGet.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butClipAGet.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butClipAGet.addListener(this);
    addAndMakeVisible(butClipAGet);
    butClipBSet.setButtonText("--> B");
    butClipBSet.setClickingTogglesState(false);
    butClipBSet.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butClipBSet.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butClipBSet.addListener(this);
    addAndMakeVisible(butClipBSet);
    butClipBGet.setButtonText("B -->");
    butClipBGet.setClickingTogglesState(false);
    butClipBGet.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butClipBGet.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butClipBGet.addListener(this);
    addAndMakeVisible(butClipBGet);


    labHelp.setText("---", juce::dontSendNotification);
    labHelp.setJustificationType(juce::Justification::left);
    labHelp.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labHelp.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labHelp.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labHelp);
    
    labHelp2.setText("Mako Distortion - Guitar Processor", juce::dontSendNotification);
    labHelp2.setJustificationType(juce::Justification::left);
    labHelp2.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labHelp2.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labHelp2.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labHelp2);
       
    //R1.00 VU Meters.
    labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out1);
    labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out2);
    labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out3);
    labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out4);
    labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out5);
    labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_Out6);

    labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In1);
    labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In2);
    labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In3);
    labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In4);
    labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In5);
    labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    addAndMakeVisible(labVU_In6);
    
    //R1.00 Frequency Test.
    butFreqTest.setButtonText("Freq Test");
    butFreqTest.setClickingTogglesState(false);
    butFreqTest.setToggleState(false, false);
    butFreqTest.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butFreqTest.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butFreqTest.setColour(juce::TextButton::buttonOnColourId, juce::Colour(100, 120, 130));
    butFreqTest.addListener(this);
    addAndMakeVisible(butFreqTest);

    //R1.00 Update the Look and Feel (Global colors) so drop down menu is the correct color. 
    getLookAndFeel().setColour(juce::DocumentWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::DialogWindow::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonOnColourId, juce::Colour(192, 0, 0));
    getLookAndFeel().setColour(juce::TextButton::buttonColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ComboBox::backgroundColourId, juce::Colour(0, 0, 0));
    getLookAndFeel().setColour(juce::ListBox::backgroundColourId, juce::Colour(32, 32, 32));
    getLookAndFeel().setColour(juce::Label::backgroundColourId, juce::Colour(32, 32, 32));
    
    //R1.00 Tune A.
    butTune.setButtonText("Tune");
    butTune.setClickingTogglesState(false);
    butTune.setToggleState(false, false);
    butTune.setColour(juce::TextButton::textColourOffId, juce::Colour(32, 32, 32));
    butTune.setColour(juce::TextButton::buttonColourId, juce::Colour(colorButton));
    butTune.setColour(juce::TextButton::buttonOnColourId, juce::Colour(100, 120, 130));
    butTune.addListener(this);
    addAndMakeVisible(butTune);

    //R1.00 Tuning Frequency selector. 
    cbTune.setColour(juce::ComboBox::textColourId, juce::Colour(192, 192, 192));
    cbTune.setColour(juce::ComboBox::backgroundColourId, juce::Colour(32, 32, 32));
    cbTune.setColour(juce::ComboBox::arrowColourId, juce::Colour(192, 192, 192));
    cbTune.setColour(juce::ComboBox::outlineColourId, juce::Colour(192, 0, 0));
    addAndMakeVisible(cbTune);
    cbTune.addSectionHeading("Standard");
    cbTune.addItem("E", 1);
    cbTune.addItem("A", 2);
    cbTune.addItem("D", 3);
    cbTune.addItem("G", 4);
    cbTune.addItem("B", 5);
    cbTune.addItem("E", 6);
    cbTune.addSeparator();
    cbTune.addSectionHeading("Drop");
    cbTune.addItem("C", 7);
    cbTune.addItem("Eb", 8);
    cbTune.addItem("Db", 9);
    cbTune.addItem("B", 10);
    cbTune.addSectionHeading("Chord");
    cbTune.addItem("EB", 11);
    cbTune.addItem("AE", 12);
    cbTune.addItem("DA", 13);
    cbTune.onChange = [this] { cbTuneChanged(); };
    cbTune.setSelectedId(1);

    //R1.00 Label to print debugging info.
    labClip.setText("", juce::dontSendNotification);
    labClip.setJustificationType(juce::Justification::centred);
    labClip.setColour(juce::Label::backgroundColourId, juce::Colour(0x00000000));
    labClip.setColour(juce::Label::textColourId, juce::Colour(0xFFC0C0C0));
    labClip.setColour(juce::Label::outlineColourId, juce::Colour(0x00808080));
    addAndMakeVisible(labClip);

}

void MakoDist01AudioProcessorEditor::cbTuneChanged()
{
    //R1.00 Get note selection and calc sine wave angle/sample increment value. 
    makoProcessor.makoTuneNote = cbTune.getSelectedId();
    makoProcessor.makoTuneStp1[0] =  6.2831853f / ((1.0f / makoProcessor.makoTuneF1[makoProcessor.makoTuneNote]) * makoProcessor.makoSampleRate);
    makoProcessor.makoTuneStp1[1] = makoProcessor.makoTuneStp1[0];
  
}


MakoDist01AudioProcessorEditor::~MakoDist01AudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void MakoDist01AudioProcessorEditor::paint (juce::Graphics& g)
{    
    float x, y, max;

    //R1.00 Paint the MAKO background only when needed.
    if (!GFX_Pedal_Refresh)
    {
        g.drawImageAt(imgBack, -1, 0);
        Mako_Opt_Initialize();
    }

    if (makoProcessor.makoFreqTest == 3)
    {
        g.drawImageAt(imgLogGraph, 176, 169);
        g.setColour(juce::Colour(0xFF80FFFF));
        //165x85, Corners: 186,171 - 351,256   X = 1.30103 - 4.30103 (20-20k)  
         
        //R1.00 get loudest amplitude.
        max = -1000.0f;
        for (int t = 20; t <= 20000; t++)
            if (max < makoProcessor.makoFreqGfxAmpl[t]) max = makoProcessor.makoFreqGfxAmpl[t];

        //R1.00 Normalize to 0 dB. 
        max = 0.0f - max;
        for (int t = 20; t <= 20000; t++)
        {
            makoProcessor.makoFreqGfxAmpl[t] += max;
            if (makoProcessor.makoFreqGfxAmpl[t] < -50.0f) makoProcessor.makoFreqGfxAmpl[t] = -50.0f;
        }

        //R1.00 Draw the curve. Probably a much better way.
        for (int t = 20; t <= 20000; t++)
        {
            y = 171 - (makoProcessor.makoFreqGfxAmpl[t] * 1.7f);                //R1.00 Amplitude 0-50 dB.
            x = 186 + int((log10f(float(t)) - 1.30103f) * 55.0f);
            g.fillRect(int(x), int(y), 1, 1);
        }           
    }

    if (makoProcessor.makoFreqTest == 0)
    {
        g.setColour(juce::Colour(0xFF000000));
        g.fillRect(176, 169, 180, 100);
    }

    //R1.00 Draw Base PEDAL layout.
    g.drawImageAt(imgPedals, 0, 286);

    //R1.00 Swap EQ and Power Amp section.
    if (makoProcessor.makoEQPreCut == 1)
    {
        g.drawImageAt(imgPedal_EQ, 140, 286);
        g.drawImageAt(imgPedal_PA, 160, 286);
    }
    
    //R1.00 Blank out unused pedals. This should be reversed.
    if (!(PEDAL_ON & Ped_Comp3)) g.drawImageAt(imgPedal, 40, 306);
    if (!(PEDAL_ON & Ped_OD1)) g.drawImageAt(imgPedal, 40, 286);
    if (!(PEDAL_ON & Ped_Attack)) g.drawImageAt(imgPedal, 60, 286);
    if (!(PEDAL_ON & Ped_Mod1a)) g.drawImageAt(imgPedal, 80, 306);
    if (!(PEDAL_ON & Ped_Mod2a)) g.drawImageAt(imgPedal, 100, 306);
    if (!(PEDAL_ON & Ped_Mod1b)) g.drawImageAt(imgPedal, 80, 286);
    if (!(PEDAL_ON & Ped_Mod2b)) g.drawImageAt(imgPedal, 100, 286);
    if (!(PEDAL_ON & Ped_Mod1c)) g.drawImageAt(imgPedal, 180, 286);
    if (!(PEDAL_ON & Ped_Mod2c)) g.drawImageAt(imgPedal, 200, 286);
    if (!(PEDAL_ON & Ped_Cab1)) g.drawImageAt(imgPedal, 220, 286);

    //R1.00 Draw small Rectangles under Delay+Reverb. 
    g.setColour(juce::Colour(0xFF000000));
    g.fillRect(240, 306, 40, 4);
    g.setColour(juce::Colour(0xFF62767C));
    if ((PEDAL_ON & Ped_Mod1d)) g.fillRect(240, 306, 8, 4);
    if ((PEDAL_ON & Ped_Mod2d)) g.fillRect(250, 306, 8, 4);
    if ((PEDAL_ON & Ped_Mod1e)) g.fillRect(260, 306, 8, 4);
    if ((PEDAL_ON & Ped_Mod2e)) g.fillRect(270, 306, 8, 4);
   
    //R1.00 RANDOM DRAWING COMMANDS 
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    //g.fillAll(juce::Colour(0xFF202020));
    //g.fillRoundedRectangle( 10, 2, 80, 275, 8);
    //g.setColour(juce::Colour(0xFFFFFFFF));
    //g.setFont (15.0f);
    //g.drawFittedText ("Gain", 10, 5, 80, 15, juce::Justification::centred, 1);
    
    /*
    char s[100];
    sprintf(s, "Vol:%f", makoProcessor.makoVol);
    g.drawFittedText(s, 330, 135, 80, 15, juce::Justification::centred, 1);
    sprintf(s, "Brk:%f", makoProcessor.makoBreak);
    g.drawFittedText(s, 330, 145, 80, 15, juce::Justification::centred, 1);
    */
    
    /*
    g.setFont(11);
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.drawFittedText("Mako Distortion", 570, 237, 80, 15, juce::Justification::centred, 1);
    */
}

void MakoDist01AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
        
    //R1.00 GAIN
    mjSlidVol.setBounds(10, 10, 80, 120);
    mjSlidChan.setBounds(10, 161, 80, 80);
    labChan.setBounds(15, 262, 70, 12);
        
    //R1.00 INPUT
    mjSlidTone.setBounds(90, 25, 80, 20);
    mjSlidTone5.setBounds(90, 50, 80, 20);
    mjSlidTone5_Q.setBounds(90, 75, 80, 20);
    mjSlidOD.setBounds(90, 100, 80, 20);
    mjSlidEnvFade.setBounds(90, 125, 80, 20);
    //labInputFilter.setBounds(95, 117, 70, 14);

    mjSlidNGate.setBounds(90, 155, 80, 20);
    mjSlidTube.setBounds(90, 180, 80, 20);
    mjSlidBite.setBounds(90, 205, 80, 20);
    mjSlidFrenzy.setBounds(90, 230, 80, 20);
    labInputX.setBounds(95, 262, 70, 14);

    //R1.00 STAGE 1
    mjSlidComp_Gain.setBounds(170, 25, 80, 20);
    mjSlidBreak1.setBounds(170, 50, 80, 20);
    mjSlidSlope1.setBounds(170, 75, 80, 20);
    mjSlidAsym1.setBounds(170, 100, 80, 20);

    labStage1.setBounds(175, 117, 70, 14);

    //R1.00 STAGE 2
    //mjSlidComp2_Gain.setBounds(250, 25, 80, 20);
    //mjSlidBreak2.setBounds(250, 50, 80, 20);
    //mjSlidSlope2.setBounds(250, 75, 80, 20);
    mjSlidPowerAmp.setBounds(250, 25, 80, 20);
    mjSlidComp2_Gain.setBounds(250, 50, 80, 20);
    labStage2.setBounds(255, 117, 70, 14);
    
    //R1.00 FIZZ
    mjSlidTone2.setBounds(330, 10, 80, 120);
    
    //R1.00 DRY Channel controls. 
    mjSlidMix.setBounds(410, 25, 80, 20);
    mjSlidComp3_Gain.setBounds(410, 50, 80, 20);
    mjSlidMixHP.setBounds(410, 75, 80, 20);    

    //R1.00 Final EQ
    butInvMid.setBounds(415, 100, 70, 16);
    butEQPreCut.setBounds(415, 125, 70, 16);
    mjSlidTreble.setBounds(490, 20, 80, 80);
    mjSlidMid.setBounds(490, 105, 80, 80);
    mjSlidBass.setBounds(490, 191, 80, 80);
    mjSlidTrebleCut.setBounds(410, 155, 80, 20);
    mjSlidBassCut.setBounds(410, 180, 80, 20);
    mjSlidTone6.setBounds(410, 205, 80, 20);
    mjSlidTone6_Q.setBounds(410, 230, 80, 20);
    labFinalEQ.setBounds(415, 260, 70, 14);

    mjSlidFinalBal.setBounds(570, 150, 80, 20);
    mjSlidFinalWidth.setBounds(570, 175, 80, 20);
    
    //R1.00 Used for debugging. Needs to be moved. 
    //labClip.setBounds(580, 168, 60, 16);          

    butTune.setBounds(575, 205, 30, 16);
    cbTune.setBounds(607, 205, 38, 16);

    mjSlidVol2.setBounds(570, 10, 80, 120);
    
    //R1.00 EFFECTS
    mjSlidModType.setBounds(660, 15, 70, 20);
    mjSlidModLoc.setBounds(660, 35, 70, 20);
    mjSlidModRate.setBounds(660, 55, 70, 20);
    mjSlidModMix.setBounds(660, 75, 70, 20);
    mjSlidModDepth.setBounds(660, 95, 70, 20);
    mjSlidModLoc.setVisible(false);
    mjSlidModRate.setVisible(false);
    mjSlidModMix.setVisible(false);
    mjSlidModDepth.setVisible(false);

    //R1.00 EFFECTS
    mjSlidMod2Type.setBounds(740, 15, 70, 20);
    mjSlidMod2Loc.setBounds(740, 35, 70, 20);
    mjSlidMod2Rate.setBounds(740, 55, 70, 20);
    mjSlidMod2Mix.setBounds(740, 75, 70, 20);
    mjSlidMod2Depth.setBounds(740, 95, 70, 20);
    mjSlidMod2Loc.setVisible(false);
    mjSlidMod2Rate.setVisible(false);
    mjSlidMod2Mix.setVisible(false);
    mjSlidMod2Depth.setVisible(false);

    mjSlidDelayTime.setBounds(660, 125, 70, 20);
    mjSlidDelayMix.setBounds(660, 145, 70, 20);
    mjSlidDelayOffset.setBounds(660, 165, 70, 20);
    mjSlidDelayBal.setBounds(660, 185, 70, 20);

    mjSlidReverbTime.setBounds(740, 125, 70, 20);
    mjSlidReverbMix.setBounds(740, 145, 70, 20);
    mjSlidReverbBal.setBounds(740, 165, 70, 20);
    
    mjSlidIRL.setBounds(660, 225, 70, 20);
    mjSlidIRR.setBounds(660, 243, 70, 20);
    mjSlidIRLMix.setBounds(740, 197, 70, 20);
    mjSlidIRRMix.setBounds(740, 215, 70, 20);
    mjSlidIRLSize.setBounds(740, 243, 70, 20);
    mjSlidIRRSize.setBounds(740, 260, 70, 20);

    labMod1.setBounds(655, 5, 72, 12);
    labMod2.setBounds(736, 5, 72, 12);

    labFX1.setBounds(653, 262, 70, 12);

    //R1.00 PRESET buttons.
    butPreset01.setBounds(176, 171, 44, 19);
    butPreset02.setBounds(176, 191, 44, 19);
    butPreset03.setBounds(176, 211, 44, 19);
    butPreset04.setBounds(176, 231, 44, 19);
    butPreset05.setBounds(176, 251, 44, 19);
    butPreset06.setBounds(221, 171, 44, 19);
    butPreset07.setBounds(221, 191, 44, 19);
    butPreset08.setBounds(221, 211, 44, 19);
    butPreset09.setBounds(221, 231, 44, 19);
    butPreset10.setBounds(221, 251, 44, 19);
    butPreset11.setBounds(266, 171, 44, 19);
    butPreset12.setBounds(266, 191, 44, 19);
    butPreset13.setBounds(266, 211, 44, 19);
    butPreset14.setBounds(266, 231, 44, 19);
    butPreset15.setBounds(266, 251, 44, 19);
    butPreset16.setBounds(311, 171, 44, 19);
    butPreset17.setBounds(311, 191, 44, 19);
    butPreset18.setBounds(311, 211, 44, 19);
    butPreset19.setBounds(311, 231, 44, 19);
    butPreset20.setBounds(311, 251, 44, 19);
    labPreset.setBounds(210, 148, 70, 14);

    butFreqTest.setBounds(290, 146, 60, 16);

    //R1.00 Setup Clipboard.
    butClipASet.setBounds(368, 176, 35, 19);
    butClipBSet.setBounds(368, 196, 35, 19);
    butClipAGet.setBounds(368, 235, 35, 19);
    butClipBGet.setBounds(368, 255, 35, 19);
    
    //R1.00 HELP
    //butHelp.setBounds(2, 282, 726, 22);
    labHelp.setBounds(285, 284, 535, 20);
    labHelp2.setBounds(285, 304, 535, 20);
    
    //R1.00 Input VU
    labVU_In1.setBounds(20, 136, 9, 4);
    labVU_In2.setBounds(30, 136, 9, 4);
    labVU_In3.setBounds(40, 136, 9, 4);
    labVU_In4.setBounds(50, 136, 9, 4);
    labVU_In5.setBounds(60, 136, 9, 4);
    labVU_In6.setBounds(70, 136, 9, 4);

    //R1.00 Output VU
    labVU_Out1.setBounds(580, 136, 9, 4);
    labVU_Out2.setBounds(590, 136, 9, 4);
    labVU_Out3.setBounds(600, 136, 9, 4);
    labVU_Out4.setBounds(610, 136, 9, 4);
    labVU_Out5.setBounds(620, 136, 9, 4);
    labVU_Out6.setBounds(630, 136, 9, 4);
            
}

void MakoDist01AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    //R1.00 Temp string for labels. 
    std::string tHelp = "";
    
    //R1.00 Default to 0. 
    ctrlHelp = 0;

    //R1.00 We need to update settings in processor.
    makoProcessor.makoSettingsChanged += 1;

    //R1.00 Vol / Gate
    if (slider == &mjSlidVol)
    {
        ctrlHelp = 1;
        makoProcessor.makoVol = float(mjSlidVol.getValue());
    }
    if (slider == &mjSlidChan)
    {
        //R1.00 Turn off sound so we dont pop when cahnging.
        makoProcessor.makoVol2 = 0.0f;

        ctrlHelp = 29;
        makoProcessor.makoChan = int(mjSlidChan.getValue());
        if ((makoProcessor.makoChan == 1) || (makoProcessor.makoChan == 3))
        {
            if (makoProcessor.makoChan == 1)
                labChan.setText("Edge", juce::dontSendNotification);
            else
                labChan.setText("Metal", juce::dontSendNotification);
            mjSlidTone5.setVisible(false);
            mjSlidTone5_Q.setVisible(false);
        }
        else
        {
            if (makoProcessor.makoChan == 0) labChan.setText("Clean", juce::dontSendNotification);
            if (makoProcessor.makoChan == 2) labChan.setText("Rock", juce::dontSendNotification);
            mjSlidTone5.setVisible(true);
            mjSlidTone5_Q.setVisible(true);
        }
        if (makoProcessor.makoChan == 0)
        {
            labVU_In1.setVisible(true); labVU_In2.setVisible(true); labVU_In3.setVisible(true); labVU_In4.setVisible(true); labVU_In5.setVisible(true); labVU_In6.setVisible(true);
        }
        else
        {
            labVU_In1.setVisible(false); labVU_In2.setVisible(false); labVU_In3.setVisible(false); labVU_In4.setVisible(false); labVU_In5.setVisible(false); labVU_In6.setVisible(false);
        }
    }


    //R1.00 OPTIONS
    if (slider == &mjSlidNGate)
    {
        ctrlHelp = 2;
        makoProcessor.makoparmNGate = float(mjSlidNGate.getValue());
        makoProcessor.makoparmNGate_Adj = makoProcessor.makoparmNGate * makoProcessor.makoparmNGate * .01f;
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoparmNGate)), juce::dontSendNotification);
    }
    if (slider == &mjSlidBite)
    {
        ctrlHelp = 5;
        makoProcessor.makoBiteGain = mjSlidBite.getValue();
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoBiteGain)), juce::dontSendNotification);
    }
    if (slider == &mjSlidTube)
    {
        ctrlHelp =  6;
        makoProcessor.makoparmTube = mjSlidTube.getValue();
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoparmTube)), juce::dontSendNotification);
    }
    if (slider == &mjSlidFrenzy)
    {
        ctrlHelp = 30;
        makoProcessor.makoFrenzy = mjSlidFrenzy.getValue();
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoFrenzy)), juce::dontSendNotification);
    }

    //R1.00 Mud
    if (slider == &mjSlidTone)
    {
        ctrlHelp = 3;
        makoProcessor.makoTone = mjSlidTone.getValue();
        labInputX.setText(std::to_string(int(makoProcessor.makoTone)), juce::dontSendNotification);
    }
    //R1.00 Mid boost.
    if (slider == &mjSlidTone5)
    {
        ctrlHelp = 15;
        makoProcessor.makoTone5 = mjSlidTone5.getValue();
        labInputX.setText(std::to_string(int(makoProcessor.makoTone5)), juce::dontSendNotification);
    }
    if (slider == &mjSlidTone5_Q)
    {
        ctrlHelp = 17;
        makoProcessor.makoTone5_Q = mjSlidTone5_Q.getValue();
        labInputX.setText(std::to_string(makoProcessor.makoTone5_Q).substr(0, 5), juce::dontSendNotification);
    }
    if (slider == &mjSlidOD)
    {
        ctrlHelp = 40;
        makoProcessor.makoOD = mjSlidOD.getValue();
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoOD)), juce::dontSendNotification);

        //R1.00 GFX
        PEDAL_ON = PEDAL_ON & ~Ped_OD1;
        if (0 < makoProcessor.makoOD) PEDAL_ON = PEDAL_ON | Ped_OD1;
        GFX_Pedals_Draw(false);
    }
    if (slider == &mjSlidEnvFade)
    {
        ctrlHelp = 44;
        makoProcessor.makoEnvFade = mjSlidEnvFade.getValue();
        labInputX.setText(std::to_string(Num_Normalize(makoProcessor.makoEnvFade)), juce::dontSendNotification);

        //R1.00 GFX
        PEDAL_ON = PEDAL_ON & ~Ped_Attack;
        if (0 < makoProcessor.makoEnvFade) PEDAL_ON = PEDAL_ON | Ped_Attack;
        GFX_Pedals_Draw(false);
    }

    //R1.00 STAGE 1
    if (slider == &mjSlidComp_Gain)
    {
        ctrlHelp = 16;
        makoProcessor.makoparmComp_Gain = mjSlidComp_Gain.getValue();
        makoProcessor.makoComp_Gain_Max = 1.0f + makoProcessor.makoparmComp_Gain * 10.0f;
        makoProcessor.makoparmComp_Release = (1.0f / .300f) *  (1.0f / makoProcessor.makoSampleRate);      //300 mS release Time. 
        labStage1.setText(std::to_string(Num_Normalize(makoProcessor.makoparmComp_Gain)), juce::dontSendNotification);
    }
    if (slider == &mjSlidBreak1)
    {
        ctrlHelp = 4;
        makoProcessor.makoBreak = mjSlidBreak1.getValue();
        labStage1.setText(std::to_string(Num_Normalize(makoProcessor.makoBreak)), juce::dontSendNotification);
    }
    if (slider == &mjSlidSlope1)
    {
        ctrlHelp = 7;
        makoProcessor.makoSlope = mjSlidSlope1.getValue();
        labStage1.setText(std::to_string(Num_Normalize(makoProcessor.makoSlope)), juce::dontSendNotification);
    }
    if (slider == &mjSlidAsym1)
    {
        ctrlHelp = 46;
        makoProcessor.makoAsym1 = mjSlidAsym1.getValue();
        labStage1.setText(std::to_string(Num_Normalize(makoProcessor.makoAsym1)), juce::dontSendNotification);
    }

    //R1.00 STAGE 2    
    if (slider == &mjSlidPowerAmp)
    {
        ctrlHelp = 7;
        makoProcessor.makoPowerAmp = mjSlidPowerAmp.getValue();
        labStage2.setText(std::to_string(Num_Normalize(makoProcessor.makoPowerAmp)), juce::dontSendNotification);
    }
    if (slider == &mjSlidComp2_Gain)
    {
        ctrlHelp = 16;
        makoProcessor.makoparmComp2_Gain = mjSlidComp2_Gain.getValue();
        makoProcessor.makoComp2_Gain_Max = 1.0f + makoProcessor.makoparmComp2_Gain * 10.0f;
        labStage2.setText(std::to_string(Num_Normalize(makoProcessor.makoparmComp2_Gain)), juce::dontSendNotification);
    }

    //R1.00 Fizz
    if (slider == &mjSlidTone2)
    {
        ctrlHelp = 8;
        makoProcessor.makoTone2 = mjSlidTone2.getValue();
    }

    //************************************************************
    //R1.00 CLEAN path
    //************************************************************
    if (slider == &mjSlidMix)
    {
        ctrlHelp = 9;
        makoProcessor.makoMix = mjSlidMix.getValue();
        labFinalEQ.setText(std::to_string(Num_Normalize(makoProcessor.makoMix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidComp3_Gain)
    {
        ctrlHelp = 32;
        makoProcessor.makoparmComp3_Gain = mjSlidComp3_Gain.getValue();
        makoProcessor.makoComp3_Gain_Max = 1.0f + makoProcessor.makoparmComp3_Gain * 10.0f;
        labFinalEQ.setText(std::to_string(Num_Normalize(makoProcessor.makoparmComp3_Gain)), juce::dontSendNotification);

        //R1.00 GFX
        PEDAL_ON = PEDAL_ON & ~Ped_Comp3;
        if (makoProcessor.makoparmComp3_Gain < 1.0f) PEDAL_ON = PEDAL_ON | Ped_Comp3;
        GFX_Pedals_Draw(false);
    }    
    if (slider == &mjSlidMixHP)
    {
        ctrlHelp = 33;
        makoProcessor.makoMixHP = mjSlidMixHP.getValue();
        labFinalEQ.setText(std::to_string(int(makoProcessor.makoMixHP)), juce::dontSendNotification);
    }
   
    //R1.00 FINAL EQ
    if (slider == &mjSlidTreble)
    {
        ctrlHelp = 10;
        makoProcessor.makoTreble = mjSlidTreble.getValue();
        labFinalEQ.setText(std::to_string(Num_Normalize(makoProcessor.makoTreble)), juce::dontSendNotification);
    }
    if (slider == &mjSlidMid)
    {
        ctrlHelp = 37;
        makoProcessor.makoMid = mjSlidMid.getValue();
        labFinalEQ.setText(std::to_string(Num_Normalize(makoProcessor.makoMid)), juce::dontSendNotification);
    }
    if (slider == &mjSlidBass)
    {
        ctrlHelp = 11;
        makoProcessor.makoBass = mjSlidBass.getValue();
        labFinalEQ.setText(std::to_string(Num_Normalize(makoProcessor.makoBass)), juce::dontSendNotification);
    }
    if (slider == &mjSlidTrebleCut)
    {
        ctrlHelp = 12;
        makoProcessor.makoTrebleCut = mjSlidTrebleCut.getValue();
        labFinalEQ.setText(std::to_string(int(makoProcessor.makoTrebleCut)), juce::dontSendNotification);
    }
    if (slider == &mjSlidBassCut)
    {
        ctrlHelp = 13;
        makoProcessor.makoBassCut = mjSlidBassCut.getValue();
        labFinalEQ.setText(std::to_string(int(makoProcessor.makoBassCut)), juce::dontSendNotification);
    }
    //R1.00 FINAL EQ - Mid boost.
    if (slider == &mjSlidTone6)
    {
        ctrlHelp = 35;
        makoProcessor.makoTone6 = mjSlidTone6.getValue();
        labFinalEQ.setText(std::to_string(int(makoProcessor.makoTone6)), juce::dontSendNotification);
    }
    if (slider == &mjSlidTone6_Q)
    {
        ctrlHelp = 36;
        makoProcessor.makoTone6_Q = mjSlidTone6_Q.getValue();
        labFinalEQ.setText(std::to_string(makoProcessor.makoTone6_Q).substr(0, 5), juce::dontSendNotification);
    }
        
    //R1.00 Master Volume.
    if (slider == &mjSlidVol2)
    {
        ctrlHelp = 14;
        makoProcessor.makoVol2_Setting = mjSlidVol2.getValue();
    }
    if (slider == &mjSlidFinalWidth)
    {
        ctrlHelp = 47;
        makoProcessor.makoFinalWidth = mjSlidFinalWidth.getValue();
        //makoProcessor.FinalWidth_B_Ring_Max = makoProcessor.makoFinalWidth * 4000;
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoFinalWidth)), juce::dontSendNotification);
    }

    if (slider == &mjSlidFinalBal)
    {
        ctrlHelp = 42;
        if (mjSlidFinalBal.getValue() < .5f)
        {
            makoProcessor.makoFinalBal[0] = 1.0f;
            makoProcessor.makoFinalBal[1] = mjSlidFinalBal.getValue() * 2;
        }
        else
        {
            makoProcessor.makoFinalBal[0] = 1.0f - ((mjSlidFinalBal.getValue() - .5f) * 2.0f);
            makoProcessor.makoFinalBal[1] = 1.0f;
        }
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoFinalBal[1])), juce::dontSendNotification);
    }

    //R1.00 MODULATION
    if (slider == &mjSlidModType)
    {
        ctrlHelp = 23;
        makoProcessor.makoModType = mjSlidModType.getValue();
        
        if (makoProcessor.makoModType == 0)
            GUI_Mod1_Visible(false);
        else
            GUI_Mod1_Visible(true);

        switch (makoProcessor.makoModType)
        {
            case 1:  tHelp = "Chorus"; break;
            case 2:  tHelp = "Tremolo"; break;
            case 3:  tHelp = "Filter"; break;
            case 4:  tHelp = "Phaser"; break;
            case 5:  tHelp = "AutoWah"; break;
            default: tHelp = "Mod FX 1"; break;
        }
        labMod1.setText(tHelp, juce::dontSendNotification);
    }
    if (slider == &mjSlidModLoc)
    {
        ctrlHelp = 24;
        makoProcessor.makoModLoc = mjSlidModLoc.getValue();
        switch (makoProcessor.makoModLoc)
        {
            case 0: tHelp = "None"; break;
            case 1: tHelp = "Dry Path"; break;
            case 2: tHelp = "Pre A"; break;
            case 3: tHelp = "Post A"; break;
            case 4: tHelp = "Delay"; break;
            case 5: tHelp = "Reverb"; break;
        }
        labFX1.setText(tHelp, juce::dontSendNotification);
        
        //R1.00 GFX VARS - Store in bits of an integer. 
        //R1.00 Mask off bits. 
        PEDAL_ON = PEDAL_ON & ~Ped_Mod1a;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod1b;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod1c;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod1d;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod1e;
        //R1.00 OR bits on that are turned on. 
        if (makoProcessor.makoModLoc == 1) PEDAL_ON = PEDAL_ON | Ped_Mod1a;
        if (makoProcessor.makoModLoc == 2) PEDAL_ON = PEDAL_ON | Ped_Mod1b;
        if (makoProcessor.makoModLoc == 3) PEDAL_ON = PEDAL_ON | Ped_Mod1c;
        if (makoProcessor.makoModLoc == 4) PEDAL_ON = PEDAL_ON | Ped_Mod1d;
        if (makoProcessor.makoModLoc == 5) PEDAL_ON = PEDAL_ON | Ped_Mod1e;
        GFX_Pedals_Draw(false);
    }
    if (slider == &mjSlidModRate)
    {
        ctrlHelp = 18;
        makoProcessor.makoModRate = mjSlidModRate.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoModRate)), juce::dontSendNotification);
    }
    if (slider == &mjSlidModMix)
    {
        ctrlHelp = 19;
        makoProcessor.makoModMix = mjSlidModMix.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoModMix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidModDepth)
    {
        ctrlHelp = 28;
        makoProcessor.makoModDepth = mjSlidModDepth.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoModDepth)), juce::dontSendNotification);
    }


    //R1.00 MODULATION 2
    if (slider == &mjSlidMod2Type)
    {
        ctrlHelp = 23;
        makoProcessor.makoMod2Type = mjSlidMod2Type.getValue();

        if (makoProcessor.makoMod2Type == 0)
            GUI_Mod2_Visible(false);
        else
            GUI_Mod2_Visible(true);

        switch (makoProcessor.makoMod2Type)
        {
        case 1:  tHelp = "Chorus"; break;
        case 2:  tHelp = "Tremolo"; break;
        case 3:  tHelp = "Filter"; break;
        case 4:  tHelp = "Phaser"; break;
        case 5:  tHelp = "AutoWah"; break;
        default: tHelp = "Mod FX 2"; break;
        }
        labMod2.setText(tHelp, juce::dontSendNotification);
    }
    if (slider == &mjSlidMod2Loc)
    {
        ctrlHelp = 24;
        makoProcessor.makoMod2Loc = mjSlidMod2Loc.getValue();
        switch (makoProcessor.makoMod2Loc)
        {
        case 0: tHelp = "None"; break;
        case 1: tHelp = "Dry Path"; break;
        case 2: tHelp = "Pre A"; break;
        case 3: tHelp = "Post A"; break;
        case 4: tHelp = "Delay"; break;
        case 5: tHelp = "Reverb"; break;
        }
        labFX1.setText(tHelp, juce::dontSendNotification);
        
        //R1.00 GFX VARS - Store in bits of an integer. 
        //R1.00 Mask off bits. 
        PEDAL_ON = PEDAL_ON & ~Ped_Mod2a;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod2b;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod2c;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod2d;
        PEDAL_ON = PEDAL_ON & ~Ped_Mod2e;
        //R1.00 OR bits on that are turned on. 
        if (makoProcessor.makoMod2Loc == 1) PEDAL_ON = PEDAL_ON | Ped_Mod2a;
        if (makoProcessor.makoMod2Loc == 2) PEDAL_ON = PEDAL_ON | Ped_Mod2b;
        if (makoProcessor.makoMod2Loc == 3) PEDAL_ON = PEDAL_ON | Ped_Mod2c;
        if (makoProcessor.makoMod2Loc == 4) PEDAL_ON = PEDAL_ON | Ped_Mod2d;
        if (makoProcessor.makoMod2Loc == 5) PEDAL_ON = PEDAL_ON | Ped_Mod2e;
        GFX_Pedals_Draw(false);
    }
    if (slider == &mjSlidMod2Rate)
    {
        ctrlHelp = 18;
        makoProcessor.makoMod2Rate = mjSlidMod2Rate.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoMod2Rate)), juce::dontSendNotification);
    }
    if (slider == &mjSlidMod2Mix)
    {
        ctrlHelp = 19;
        makoProcessor.makoMod2Mix = mjSlidMod2Mix.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoMod2Mix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidMod2Depth)
    {
        ctrlHelp = 28;
        makoProcessor.makoMod2Depth = mjSlidMod2Depth.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoMod2Depth)), juce::dontSendNotification);
    }

    //R1.00 DELAY
    if (slider == &mjSlidDelayTime)
    {
        ctrlHelp = 20;
        makoProcessor.makoDelayTime = mjSlidDelayTime.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoDelayTime)), juce::dontSendNotification);
    }
    if (slider == &mjSlidDelayMix)
    {
        ctrlHelp = 21;
        makoProcessor.makoDelayMix = mjSlidDelayMix.getValue();
        if (makoProcessor.makoDelayMix < .5f)
        {
            makoProcessor.makoDelayDry = 1.0f;
            makoProcessor.makoDelayWet = makoProcessor.makoDelayMix * 2;
        }
        else
        {
            makoProcessor.makoDelayDry = 1.0f - ((makoProcessor.makoDelayMix - .5f) * 2.0f);
            makoProcessor.makoDelayWet = 1.0f;
        }
        makoProcessor.makoDelayRepeat = .25f + makoProcessor.makoDelayMix * .5f;
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoDelayMix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidDelayOffset)
    {
        ctrlHelp = 27;
        makoProcessor.makoDelayOffset = mjSlidDelayOffset.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoDelayOffset)), juce::dontSendNotification);
    }
    if (slider == &mjSlidDelayBal)
    {
        ctrlHelp = 41;
        makoProcessor.makoDelayBal = mjSlidDelayBal.getValue();
        if (makoProcessor.makoDelayBal < .5f)
        {
            makoProcessor.makoDelayBalLR[0] = 1.0f;
            makoProcessor.makoDelayBalLR[1] = makoProcessor.makoDelayBal * 2;
        }
        else
        {
            makoProcessor.makoDelayBalLR[0] = 1.0f - ((makoProcessor.makoDelayBal - .5f) * 2.0f);
            makoProcessor.makoDelayBalLR[1] = 1.0f;
        }
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoDelayBal)), juce::dontSendNotification);
    }
    if (slider == &mjSlidReverbMix)
    {
        ctrlHelp = 22;
        makoProcessor.makoReverbMix = mjSlidReverbMix.getValue();
        if (makoProcessor.makoReverbMix < .5f)
        {
            makoProcessor.makoReverbDry = 1.0f;
            makoProcessor.makoReverbWet = makoProcessor.makoReverbMix * 2;
        }
        else
        {
            makoProcessor.makoReverbDry = 1.0f - ((makoProcessor.makoReverbMix - .5f) * 2.0f);
            makoProcessor.makoReverbWet = 1.0f;
        }


        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoReverbMix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidReverbTime)
    {
        ctrlHelp = 39;
        makoProcessor.makoReverbTime = mjSlidReverbTime.getValue();
        makoProcessor.makoReverbTimeInv = 1 - makoProcessor.makoReverbTime;
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoReverbTime)), juce::dontSendNotification);
    }
    if (slider == &mjSlidReverbBal)
    {
        ctrlHelp = 42;
        makoProcessor.makoReverbBal = mjSlidReverbBal.getValue();
        if (makoProcessor.makoReverbBal < .5f)
        {
            makoProcessor.makoReverbBalLR[0] = 1.0f;
            makoProcessor.makoReverbBalLR[1] = makoProcessor.makoReverbBal * 2;
        }
        else
        {
            makoProcessor.makoReverbBalLR[0] = 1.0f - ((makoProcessor.makoReverbBal - .5f) * 2.0f);
            makoProcessor.makoReverbBalLR[1] = 1.0f;
        }
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoReverbBal)), juce::dontSendNotification);
    }

    //R1.00 IR CABS
    if (slider == &mjSlidIRL)
    {
        ctrlHelp = 25;
        makoProcessor.makoIRL = mjSlidIRL.getValue();
        switch (makoProcessor.makoIRL)
        {
            case 0: tHelp = "0-None"; break;
            case 1: tHelp = "1-Rock"; break;
            case 2: tHelp = "2-MakoA"; break;
            case 3: tHelp = "3-MakoB"; break;
            case 4: tHelp = "4-Metal"; break;
            case 5: tHelp = "5-Fender"; break;
            case 6: tHelp = "6-1960"; break;
            case 7: tHelp = "7-RosC1"; break;
            case 8: tHelp = "8-Aco"; break;
            case 9: tHelp = "9-412"; break;
            case 10: tHelp = "10-Deep"; break;
        }
        labFX1.setText(tHelp, juce::dontSendNotification);

        //R1.00 GFX
        PEDAL_ON = PEDAL_ON & ~Ped_Cab1;
        if ((0 < makoProcessor.makoIRL) && (0 < makoProcessor.makoIRR)) PEDAL_ON = PEDAL_ON | Ped_Cab1;
        GFX_Pedals_Draw(false);
    }
    if (slider == &mjSlidIRR)
    {
        ctrlHelp = 26;
        makoProcessor.makoIRR = mjSlidIRR.getValue();
        switch (makoProcessor.makoIRR)
        {
            case 0: tHelp = "0-None"; break;
            case 1: tHelp = "1-Rock"; break;
            case 2: tHelp = "2-MakoA"; break;
            case 3: tHelp = "3-MakoB"; break;
            case 4: tHelp = "4-Metal"; break;
            case 5: tHelp = "5-Fender"; break;
            case 6: tHelp = "6-1960"; break;
            case 7: tHelp = "7-RosC1"; break;
            case 8: tHelp = "8-Aco"; break;
            case 9: tHelp = "9-412"; break;
            case 10: tHelp = "10-Deep"; break;
        }
        labFX1.setText(tHelp, juce::dontSendNotification);

        //R1.00 GFX
        PEDAL_ON = PEDAL_ON & ~Ped_Cab1;
        if ((0 < makoProcessor.makoIRL) && (0 < makoProcessor.makoIRR)) PEDAL_ON = PEDAL_ON | Ped_Cab1;
        GFX_Pedals_Draw(false);
    }
    //R1.00 IR CAB BLENDER
    if (slider == &mjSlidIRLMix)
    {
        ctrlHelp = 43;
        makoProcessor.makoIRLMix = mjSlidIRLMix.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoIRLMix)), juce::dontSendNotification);
    }
    if (slider == &mjSlidIRRMix)
    {
        ctrlHelp = 43;
        makoProcessor.makoIRRMix = mjSlidIRRMix.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoIRRMix)), juce::dontSendNotification);
    }
    //R1.00 IR Speaker size.
    if (slider == &mjSlidIRLSize)
    {
        ctrlHelp = 45;
        makoProcessor.makoIRLSize = mjSlidIRLSize.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoIRLSize)), juce::dontSendNotification);
    }
    if (slider == &mjSlidIRRSize)
    {
        ctrlHelp = 45;
        makoProcessor.makoIRRSize = mjSlidIRRSize.getValue();
        labFX1.setText(std::to_string(Num_Normalize(makoProcessor.makoIRRSize)), juce::dontSendNotification);
    }
    
    //R1.00 If adjusting a new control, update the HELP text.
    //R1.00 Do not update the text if still on the same control. 
    if (ctrlHelp != ctrlHelpLast)
    {
        ctrlHelpLast = ctrlHelp;

        switch (ctrlHelp)
        {
            case 0:tHelp = "---"; break;
            case 1:tHelp = "GAIN: Adjust the distortion gain. Try to maximize input first without clipping."; break;
            case 2:tHelp = "NOISE GATE: Adjust the gate threshold. Adding TUBE helps reduce noise also."; break;
            case 3:tHelp = "LOW FREQ CUTOFF: Remove muddiness by reducing the low freq signal before the gain stage."; break;
            case 4:tHelp = "SLOPE: Adjust the upper clip point. 0-No clip, 1-Hard clip."; break;
            case 5:tHelp = "BITE: Adjust the amount of negative signal added after gain stages."; break;
            case 6:tHelp = "TUBE: Adjust the dynamic high pass filter after gain stage 1."; break;
            case 7:tHelp = "POWER AMP: Adjust how hard you are pushing the amp. Hyperbolic Tangent wave shaper."; break;
            case 8:tHelp = "HIGH FREQ CUTOFF: Remove distortion fizz by reducing high freq signals after the gain stage."; break;
            case 9:tHelp = "MIX: Adjust the ratio between the clean and distorted signals."; break;
            case 10:tHelp = "FINAL EQ TREBLE: Adjust the final stage Treble EQ. Default is .5."; break;
            case 11:tHelp = "FINAL EQ BASS: Adjust the final stage Bass EQ. Default is .5."; break;
            case 12:tHelp = "FINAL EQ TREBLE CUTOFF: Adjust the final stage Treble high pass frequency. Def=1500."; break;
            case 13:tHelp = "FINAL EQ BASS CUTOFF: Adjust the final stage Bass low pass frequency. Def=150."; break;
            case 14:tHelp = "VOL: Adjust the final stage Volume."; break;
            case 15:tHelp = "MID BOOST: Select the voice of the pre-gain mid filter. .1-Flatter sound,2.0-wah sound. Def=.707."; break;
            case 16:tHelp = "GAIN STAGE COMPRESSOR: Adjust the amount of compression before the stage."; break;
            case 17:tHelp = "MID BOOST Q: Adjust the mid freq filter Q. Higher = more pronounced effect."; break;
            case 18:tHelp = "MOD RATE: Adjust how fast the modulator cycles."; break;
            case 19:tHelp = "MOD MIX: Adjust the blend amount between the clean and modulated signal."; break;
            case 20:tHelp = "DELAY TIME: Adjust the amount of time between echoes."; break;
            case 21:tHelp = "DELAY MIX: Adjust the blend amount between the clean and delayed signal."; break;
            case 22:tHelp = "REVERB MIX: Adjust the blend amount between the clean and delayed signal."; break;
            case 23:tHelp = "MOD TYPE: Select the Mod effect type: 1-Chor, 2-Trem, 3-Filter, 4-Phaser, 5-AutoWah"; break;
            case 24:tHelp = "MOD LOC: Select the effect location: 1-Dry path,2-Pre Amp,3-Post Amp,4-Delay,5-Reverb"; break;
            case 25:tHelp = "LEFT IR CAB: Select a 48kHz 1024 sample Impulse Response."; break;
            case 26:tHelp = "RIGHT IR CAB: Select a 48kHz 1024 sample Impulse Response."; break;
            case 27:tHelp = "DELAY OFFSET: Adjust the blend between mono (0) and stereo (1) delay styles."; break;
            case 28:tHelp = "MOD DEPTH: Adjust how strong the mod effect is."; break;
            case 29:tHelp = "AMP MODEL: Select different distortion modes."; break;
            case 30:tHelp = "FRENZY: Creates an volume envelope to add dynamics to heavy gain sounds."; break;
            case 31:tHelp = "CLEAN PATH COMPRESSOR: Adjust the amount of compression."; break;
            case 32:tHelp = "MIX COMPRESSOR: Adjust the compression on the clean path signal."; break;
            case 33:tHelp = "MIX HIGH PASS: Remove some low frequency on the clean path signal."; break;
            case 34:tHelp = "MIX RECTIFIER: Rectify the clean path signal."; break;
            case 35:tHelp = "FINAL EQ MID: Set the mid range frequency center. Def=700."; break;
            case 36:tHelp = "FINAL EQ MID Q: Select the voice of the mid filter. .1-Flat sound,2.0-wah sound. Def=.707."; break;
            case 37:tHelp = "FINAL EQ MID: Adjust the final stage Mid EQ. Default is .5."; break;
            case 39:tHelp = "REVERB DECAY: Adjust the time it takes for the reverb to decay."; break;
            case 40:tHelp = "OVER DRIVE: Adjust the amount of OD boost to the signal."; break;
            case 41:tHelp = "DELAY BALANCE: Set the Stereo balance of the delay effect (L,R). Def = .5."; break;
            case 42:tHelp = "REVERB BALANCE: Set the Stereo balance of the reverb effect (L,R). Def = .5."; break;
            case 43:tHelp = "IR VOICE: Adjust the frequency response of the current selected IR. Def = .5."; break;
            case 44:tHelp = "ENVELOPE: Adjust to gradually fade in your signal."; break;
            case 45:tHelp = "IR SIZE: Resample the IR to make it appear as a different size speaker."; break;
            case 46:tHelp = "SYMMETRY: Limit the negative half of the waveform (Tube Follower)."; break;
            case 47:tHelp = "STEREO WIDTH (Haas): Add delay to channel 1 to add stereo effect."; break;
        }
        labHelp.setText(tHelp, juce::dontSendNotification);

        switch (ctrlHelp)
        {           
            case 1: tHelp = "Input volume is critical to distortion effects. Note: Do not overload."; break;
            
            case 18: case 19: case 23:  case 24: case 28:
                tHelp = "T - Type, L - Location, R - Rate, M - Mix, D - Depth"; break;
            case 20: case 21: case 27: case 41:
                tHelp = "D - Delay, M - Mix, O - Stereo Offset, B - Balance"; break;
            case 22: case 39: case 42:
                tHelp = "D - Decay, M - Mix, B - Balance"; break;
            case 29: tHelp = "Clean and rock use a Mid EQ filter before the gain stages."; break;
            case 30: tHelp = "Frenzy creates a volume envelope based on the incoming clean signal."; break;
            case 9: case 32: case 33:
                tHelp = "Clean path: Input --> Low Pass --> Compressor --> Mod* --> Output"; break;
            case 43: tHelp = "Blend between a bright and dark version of the Impulse response."; break;

            default:tHelp = "Ctrl-Mouse for precision.   Double Click for default."; break;
        }
        labHelp2.setText(tHelp, juce::dontSendNotification);
                
    }

}

int MakoDist01AudioProcessorEditor::Num_Normalize(float V)
{
    //R1.00 Convert float 0.0-1.0 to 0 to 100 for display on editor. 
    int tV = V * 100;
    return tV;
}

void MakoDist01AudioProcessorEditor::GUI_Mod1_Visible(bool Vstate)
{
    //R1.00 HIDE/SHOW Modulation Controls.
    mjSlidModLoc.setVisible(Vstate);
    mjSlidModRate.setVisible(Vstate);
    mjSlidModLoc.setVisible(Vstate);
    mjSlidModMix.setVisible(Vstate);
    mjSlidModDepth.setVisible(Vstate);
}

void MakoDist01AudioProcessorEditor::GUI_Mod2_Visible(bool Vstate)
{
    //R1.00 HIDE/SHOW Modulation Controls.
    mjSlidMod2Loc.setVisible(Vstate);
    mjSlidMod2Rate.setVisible(Vstate);
    mjSlidMod2Loc.setVisible(Vstate);
    mjSlidMod2Mix.setVisible(Vstate);
    mjSlidMod2Depth.setVisible(Vstate);
}


void MakoDist01AudioProcessorEditor::buttonStateChanged(juce::Button* butt)
{
    //***************************************************************
    //R1.00 OPTION BUTTONS
    //***************************************************************
    //R1.00 Do not execute on mouseover.
    auto buttstate = butt->getState();
    //if (buttstate == juce::Button::buttonOver) return;
    
    //R1.00 Deal with TRUE/FALSE (on/off) state changes. 
    auto togstate = butt->getToggleState();
    if (butt == &butEQPreCut)
    {
        labHelp.setText("EQ LOCATION: Put EQ before or after Power Amp stage.", juce::dontSendNotification);
        labHelp2.setText("Pre -> Post -> EQ   or   Pre -> EQ -> Post", juce::dontSendNotification);
        Mako_Opt_EQPreCut(togstate, false);
        GFX_Pedals_Draw(true);                       //R1.00 Swap the EQ and PA icons.
    }

    if (butt == &butInvMid)
    {
        labHelp.setText("INVERT MID: Invert the MID EQ phase.", juce::dontSendNotification);
        labHelp2.setText("Invert mids for jangley clean tones. Thick/Thin button.", juce::dontSendNotification); 
        Mako_Opt_InvMid(togstate, false);
    }
    
    if (butt == &butClipASet)
    {
        labHelp.setText("Compare: Store the current setup settings in memory A.", juce::dontSendNotification);
        labHelp2.setText("Use clipboard to A-B compare settings.", juce::dontSendNotification);
    }
    if (butt == &butClipAGet)
    {
        labHelp.setText("Compare: Load memory A into the current setup.", juce::dontSendNotification);
        labHelp2.setText("Use clipboard to A-B compare settings.", juce::dontSendNotification);
    }
    if (butt == &butClipBSet)
    {
        labHelp.setText("Compare: Store the current setup settings in memory B.", juce::dontSendNotification);
        labHelp2.setText("Use clipboard to A-B compare settings.", juce::dontSendNotification);
    }
    if (butt == &butClipBGet)
    {
        labHelp.setText("Compare: Load memory B into the current setup.", juce::dontSendNotification);
        labHelp2.setText("Use clipboard to A-B compare settings.", juce::dontSendNotification);
    }
    return;
}


void MakoDist01AudioProcessorEditor::buttonClicked (juce::Button* butt)
{   
    //R1.00 MUTE while changing settings so we dont get loud pops and echoes. 
    makoProcessor.makoVol2 = 0.0f;

    //R1.00 We need to update settings in processor.
    makoProcessor.makoSettingsChanged = 1;
    makoProcessor.makoSettingsClearbuffers = 1;
    
    //R1.00 Turn on/off the TUNER pitch wheel. 
    if (butt == &butTune)
    {
        if (makoProcessor.makoTune == 0)
            makoProcessor.makoTune = 1;
        else
            makoProcessor.makoTune = 0;
    }

    //R1.00 Initiate a frequency sweep to test filters, IRs, etc.
    if (butt == &butFreqTest)
    {
        if (makoProcessor.makoFreqTest == 0)
        {
            makoProcessor.makoFreqTest = 1;
            labPreset.setText("Testing", juce::dontSendNotification);
            butPreset01.setVisible(false); butPreset02.setVisible(false); butPreset03.setVisible(false); butPreset04.setVisible(false); butPreset05.setVisible(false);
            butPreset06.setVisible(false); butPreset07.setVisible(false); butPreset08.setVisible(false); butPreset09.setVisible(false); butPreset10.setVisible(false);
            butPreset11.setVisible(false); butPreset12.setVisible(false); butPreset13.setVisible(false); butPreset14.setVisible(false); butPreset15.setVisible(false);
            butPreset16.setVisible(false); butPreset17.setVisible(false); butPreset18.setVisible(false); butPreset19.setVisible(false); butPreset20.setVisible(false);
            repaint();
        }
        else
        {
            makoProcessor.makoFreqTest = 0;
            labPreset.setText("---", juce::dontSendNotification);
            butPreset01.setVisible(true); butPreset02.setVisible(true); butPreset03.setVisible(true); butPreset04.setVisible(true); butPreset05.setVisible(true);
            butPreset06.setVisible(true); butPreset07.setVisible(true); butPreset08.setVisible(true); butPreset09.setVisible(true); butPreset10.setVisible(true);
            butPreset11.setVisible(true); butPreset12.setVisible(true); butPreset13.setVisible(true); butPreset14.setVisible(true); butPreset15.setVisible(true);
            butPreset16.setVisible(true); butPreset17.setVisible(true); butPreset18.setVisible(true); butPreset19.setVisible(true); butPreset20.setVisible(true);
            repaint();
        }
    }

    //***************************************************************
    //R1.00 SETUP CLIPBOARD
    //R1.00 There is probably a much easier method.
    //***************************************************************
    if (butt == &butClipASet)
    {
        clpA_Gain = mjSlidVol.getValue();
        clpA_Chan = mjSlidChan.getValue();
        clpA_Input_Tone = mjSlidTone.getValue();
        clpA_Input_MidF = mjSlidTone5.getValue();
        clpA_Input_MidQ = mjSlidTone5_Q.getValue();
        
        clpA_FX_OD = mjSlidOD.getValue();
        clpA_FX_Attack = mjSlidEnvFade.getValue();
        clpA_FX_NGate = mjSlidNGate.getValue();
        clpA_FX_Tube = mjSlidTube.getValue();
        clpA_FX_Bite = mjSlidBite.getValue();
        clpA_FX_Frenzy = mjSlidFrenzy.getValue();
        
        clpA_S1_Comp = mjSlidComp_Gain.getValue();
        clpA_S1_Break = mjSlidBreak1.getValue();
        clpA_S1_Slope = mjSlidSlope1.getValue();
        clpA_S2_PowerAmp = mjSlidPowerAmp.getValue();
        clpA_S2_Comp = mjSlidComp2_Gain.getValue();
        clpA_Fizz = mjSlidTone2.getValue();
        
        clpA_Dry_Mix = mjSlidMix.getValue();
        clpA_Dry_InvMid = butInvMid.getToggleState();
        clpA_Dry_Comp = mjSlidComp3_Gain.getValue();
        clpA_Dry_HighPassF = mjSlidMixHP.getValue();

        clpA_Final_Treble = mjSlidTreble.getValue();
        clpA_Final_Mid = mjSlidMid.getValue();
        clpA_Final_Bass = mjSlidBass.getValue();
        clpA_Final_TrebleF = mjSlidTrebleCut.getValue();
        clpA_Final_MidF = mjSlidTone6.getValue();
        clpA_Final_MidQ = mjSlidTone6_Q.getValue();
        clpA_Final_BassF = mjSlidBassCut.getValue();
        clpA_Final_Vol = mjSlidVol2.getValue();
        clpA_Final_Bal = mjSlidFinalBal.getValue();
        clpA_Final_Width = mjSlidFinalWidth.getValue();
        clpA_But_EQPreCut = butEQPreCut.getToggleState();

        clpA_Mod1_Type = mjSlidModType.getValue();
        clpA_Mod1_Loc = mjSlidModLoc.getValue();
        clpA_Mod1_Rate = mjSlidModRate.getValue();
        clpA_Mod1_Mix = mjSlidModMix.getValue();
        clpA_Mod1_Depth = mjSlidModDepth.getValue();
        clpA_Mod2_Type = mjSlidMod2Type.getValue();
        clpA_Mod2_Loc = mjSlidMod2Loc.getValue();
        clpA_Mod2_Rate = mjSlidMod2Rate.getValue();
        clpA_Mod2_Mix = mjSlidMod2Mix.getValue();
        clpA_Mod2_Depth = mjSlidMod2Depth.getValue();

        clpA_Delay_Time = mjSlidDelayTime.getValue();
        clpA_Delay_Mix = mjSlidDelayMix.getValue();
        clpA_Delay_Offset = mjSlidDelayOffset.getValue();
        clpA_Delay_Bal = mjSlidDelayBal.getValue();
        clpA_Reverb_Time = mjSlidReverbTime.getValue();
        clpA_Reverb_Mix = mjSlidReverbMix.getValue();
        clpA_Reverb_Bal = mjSlidReverbBal.getValue();

        clpA_IRL = mjSlidIRL.getValue();
        clpA_IRL_Voice = mjSlidIRLMix.getValue();
        clpA_IRL_Size = mjSlidIRLSize.getValue();
        clpA_IRR = mjSlidIRR.getValue();
        clpA_IRR_Voice = mjSlidIRRMix.getValue();
        clpA_IRR_Size = mjSlidIRRSize.getValue();                
    }
    if (butt == &butClipBSet)
    {
        clpB_Gain = mjSlidVol.getValue();
        clpB_Chan = mjSlidChan.getValue();
        clpB_Input_Tone = mjSlidTone.getValue();
        clpB_Input_MidF = mjSlidTone5.getValue();
        clpB_Input_MidQ = mjSlidTone5_Q.getValue();

        clpB_FX_OD = mjSlidOD.getValue();
        clpB_FX_Attack = mjSlidEnvFade.getValue();
        clpB_FX_NGate = mjSlidNGate.getValue();
        clpB_FX_Tube = mjSlidTube.getValue();
        clpB_FX_Bite = mjSlidBite.getValue();
        clpB_FX_Frenzy = mjSlidFrenzy.getValue();

        clpB_S1_Comp = mjSlidComp_Gain.getValue();
        clpB_S1_Break = mjSlidBreak1.getValue();
        clpB_S1_Slope = mjSlidSlope1.getValue();
        clpB_S2_PowerAmp = mjSlidPowerAmp.getValue();
        clpB_S2_Comp = mjSlidComp2_Gain.getValue();
        clpB_Fizz = mjSlidTone2.getValue();

        clpB_Dry_Mix = mjSlidMix.getValue();
        clpB_Dry_InvMid = butInvMid.getToggleState();
        clpB_Dry_Comp = mjSlidComp3_Gain.getValue();
        clpB_Dry_HighPassF = mjSlidMixHP.getValue();

        clpB_Final_Treble = mjSlidTreble.getValue();
        clpB_Final_Mid = mjSlidMid.getValue();
        clpB_Final_Bass = mjSlidBass.getValue();
        clpB_Final_TrebleF = mjSlidTrebleCut.getValue();
        clpB_Final_MidF = mjSlidTone6.getValue();
        clpB_Final_MidQ = mjSlidTone6_Q.getValue();
        clpB_Final_BassF = mjSlidBassCut.getValue();
        clpB_Final_Vol = mjSlidVol2.getValue();
        clpB_Final_Bal = mjSlidFinalBal.getValue();
        clpB_Final_Width = mjSlidFinalWidth.getValue();
        clpB_But_EQPreCut = butEQPreCut.getToggleState();

        clpB_Mod1_Type = mjSlidModType.getValue();
        clpB_Mod1_Loc = mjSlidModLoc.getValue();
        clpB_Mod1_Rate = mjSlidModRate.getValue();
        clpB_Mod1_Mix = mjSlidModMix.getValue();
        clpB_Mod1_Depth = mjSlidModDepth.getValue();
        clpB_Mod2_Type = mjSlidMod2Type.getValue();
        clpB_Mod2_Loc = mjSlidMod2Loc.getValue();
        clpB_Mod2_Rate = mjSlidMod2Rate.getValue();
        clpB_Mod2_Mix = mjSlidMod2Mix.getValue();
        clpB_Mod2_Depth = mjSlidMod2Depth.getValue();

        clpB_Delay_Time = mjSlidDelayTime.getValue();
        clpB_Delay_Mix = mjSlidDelayMix.getValue();
        clpB_Delay_Offset = mjSlidDelayOffset.getValue();
        clpB_Delay_Bal = mjSlidDelayBal.getValue();
        clpB_Reverb_Time = mjSlidReverbTime.getValue();
        clpB_Reverb_Mix = mjSlidReverbMix.getValue();
        clpB_Reverb_Bal = mjSlidReverbBal.getValue();

        clpB_IRL = mjSlidIRL.getValue();
        clpB_IRL_Voice = mjSlidIRLMix.getValue();
        clpB_IRL_Size = mjSlidIRLSize.getValue();
        clpB_IRR = mjSlidIRR.getValue();
        clpB_IRR_Voice = mjSlidIRRMix.getValue();
        clpB_IRR_Size = mjSlidIRRSize.getValue();        
    }
    if (butt == &butClipAGet)
    {
        if (0.0f < clpA_Gain)
        {
            mjSlidVol.setValue(clpA_Gain);
            mjSlidChan.setValue(clpA_Chan);
            mjSlidTone.setValue(clpA_Input_Tone);
            mjSlidTone5.setValue(clpA_Input_MidF);
            mjSlidTone5_Q.setValue(clpA_Input_MidQ);

            mjSlidOD.setValue(clpA_FX_OD);
            mjSlidEnvFade.setValue(clpA_FX_Attack);
            mjSlidNGate.setValue(clpA_FX_NGate);
            mjSlidTube.setValue(clpA_FX_Tube);
            mjSlidBite.setValue(clpA_FX_Bite);
            mjSlidFrenzy.setValue(clpA_FX_Frenzy);

            mjSlidComp_Gain.setValue(clpA_S1_Comp);
            mjSlidBreak1.setValue(clpA_S1_Break);
            mjSlidSlope1.setValue(clpA_S1_Slope);
            mjSlidPowerAmp.setValue(clpA_S2_PowerAmp);
            mjSlidComp2_Gain.setValue(clpA_S2_Comp);
            mjSlidTone2.setValue(clpA_Fizz);

            mjSlidMix.setValue(clpA_Dry_Mix);
            butInvMid.setToggleState(clpA_Dry_InvMid,false);
            mjSlidComp3_Gain.setValue(clpA_Dry_Comp);
            mjSlidMixHP.setValue(clpA_Dry_HighPassF);

            mjSlidTreble.setValue(clpA_Final_Treble);
            mjSlidMid.setValue(clpA_Final_Mid);
            mjSlidBass.setValue(clpA_Final_Bass);
            mjSlidTrebleCut.setValue(clpA_Final_TrebleF);
            mjSlidTone6.setValue(clpA_Final_MidF);
            mjSlidTone6_Q.setValue(clpA_Final_MidQ);
            mjSlidBassCut.setValue(clpA_Final_BassF);
            mjSlidVol2.setValue(clpA_Final_Vol);
            mjSlidFinalBal.setValue(clpA_Final_Bal);
            mjSlidFinalWidth.setValue(clpA_Final_Width);
            butEQPreCut.setToggleState(clpA_But_EQPreCut, false);

            mjSlidModType.setValue(clpA_Mod1_Type);
            mjSlidModLoc.setValue(clpA_Mod1_Loc);
            mjSlidModRate.setValue(clpA_Mod1_Rate);
            mjSlidModMix.setValue(clpA_Mod1_Mix);
            mjSlidModDepth.setValue(clpA_Mod1_Depth);
            mjSlidMod2Type.setValue(clpA_Mod2_Type);
            mjSlidMod2Loc.setValue(clpA_Mod2_Loc);
            mjSlidMod2Rate.setValue(clpA_Mod2_Rate);
            mjSlidMod2Mix.setValue(clpA_Mod2_Mix);
            mjSlidMod2Depth.setValue(clpA_Mod2_Depth);

            mjSlidDelayTime.setValue(clpA_Delay_Time);
            mjSlidDelayMix.setValue(clpA_Delay_Mix);
            mjSlidDelayOffset.setValue(clpA_Delay_Offset);
            mjSlidDelayBal.setValue(clpA_Delay_Bal);
            mjSlidReverbTime.setValue(clpA_Reverb_Time);
            mjSlidReverbMix.setValue(clpA_Reverb_Mix);
            mjSlidReverbBal.setValue(clpA_Reverb_Bal);

            mjSlidIRL.setValue(clpA_IRL);
            mjSlidIRLMix.setValue(clpA_IRL_Voice);
            mjSlidIRLSize.setValue(clpA_IRL_Size);
            mjSlidIRR.setValue(clpA_IRR);
            mjSlidIRRMix.setValue(clpA_IRR_Voice);
            mjSlidIRRSize.setValue(clpA_IRR_Size);            
        }
    }
    if (butt == &butClipBGet)
    {
        if (0.0f < clpB_Gain)
        {
            mjSlidVol.setValue(clpB_Gain);
            mjSlidChan.setValue(clpB_Chan);
            mjSlidTone.setValue(clpB_Input_Tone);
            mjSlidTone5.setValue(clpB_Input_MidF);
            mjSlidTone5_Q.setValue(clpB_Input_MidQ);

            mjSlidOD.setValue(clpB_FX_OD);
            mjSlidEnvFade.setValue(clpB_FX_Attack);
            mjSlidNGate.setValue(clpB_FX_NGate);
            mjSlidTube.setValue(clpB_FX_Tube);
            mjSlidBite.setValue(clpB_FX_Bite);
            mjSlidFrenzy.setValue(clpB_FX_Frenzy);

            mjSlidComp_Gain.setValue(clpB_S1_Comp);
            mjSlidBreak1.setValue(clpB_S1_Break);
            mjSlidSlope1.setValue(clpB_S1_Slope);
            mjSlidPowerAmp.setValue(clpB_S2_PowerAmp);
            mjSlidComp2_Gain.setValue(clpB_S2_Comp);
            mjSlidTone2.setValue(clpB_Fizz);

            mjSlidMix.setValue(clpB_Dry_Mix);
            butInvMid.setToggleState(clpB_Dry_InvMid, false);
            mjSlidComp3_Gain.setValue(clpB_Dry_Comp);
            mjSlidMixHP.setValue(clpB_Dry_HighPassF);

            mjSlidTreble.setValue(clpB_Final_Treble);
            mjSlidMid.setValue(clpB_Final_Mid);
            mjSlidBass.setValue(clpB_Final_Bass);
            mjSlidTrebleCut.setValue(clpB_Final_TrebleF);
            mjSlidTone6.setValue(clpB_Final_MidF);
            mjSlidTone6_Q.setValue(clpB_Final_MidQ);
            mjSlidBassCut.setValue(clpB_Final_BassF);
            mjSlidVol2.setValue(clpB_Final_Vol);
            mjSlidFinalBal.setValue(clpB_Final_Bal);
            mjSlidFinalWidth.setValue(clpB_Final_Width);
            butEQPreCut.setToggleState(clpB_But_EQPreCut, false);

            mjSlidModType.setValue(clpB_Mod1_Type);
            mjSlidModLoc.setValue(clpB_Mod1_Loc);
            mjSlidModRate.setValue(clpB_Mod1_Rate);
            mjSlidModMix.setValue(clpB_Mod1_Mix);
            mjSlidModDepth.setValue(clpB_Mod1_Depth);
            mjSlidMod2Type.setValue(clpB_Mod2_Type);
            mjSlidMod2Loc.setValue(clpB_Mod2_Loc);
            mjSlidMod2Rate.setValue(clpB_Mod2_Rate);
            mjSlidMod2Mix.setValue(clpB_Mod2_Mix);
            mjSlidMod2Depth.setValue(clpB_Mod2_Depth);

            mjSlidDelayTime.setValue(clpB_Delay_Time);
            mjSlidDelayMix.setValue(clpB_Delay_Mix);
            mjSlidDelayOffset.setValue(clpB_Delay_Offset);
            mjSlidDelayBal.setValue(clpB_Delay_Bal);
            mjSlidReverbTime.setValue(clpB_Reverb_Time);
            mjSlidReverbMix.setValue(clpB_Reverb_Mix);
            mjSlidReverbBal.setValue(clpB_Reverb_Bal);

            mjSlidIRL.setValue(clpB_IRL);
            mjSlidIRLMix.setValue(clpB_IRL_Voice);
            mjSlidIRLSize.setValue(clpB_IRL_Size);
            mjSlidIRR.setValue(clpB_IRR);
            mjSlidIRRMix.setValue(clpB_IRR_Voice);
            mjSlidIRRSize.setValue(clpB_IRR_Size);            
        }
    }

    //***************************************************************
    //R1.00 PRESET BUTTONS
    //R1.00 Clear the Help button. This section must be first.  
    //***************************************************************
    if (butt == &butPreset01)  //R1.00 CLEAN
    {
        labPreset.setText("Clean", juce::dontSendNotification);
        Mako_Set_Input(0, 150, 900, .1);
        Mako_Set_Stages(.1f, .4f, .4f, 0.0f, 0.0f, 0.0f);
                
        mjSlidTone2.setValue(3000);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0f);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.35f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);
                        
        //R1.00 Dry/Clean
        mjSlidMix.setValue(1);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(400);
       
        Mako_Opt_InvMid(1, true);
        Mako_Set_FinalEQ(.6, .6, .45, 1500, 150, 1000, .707f);
        Mako_Set_Mod(1, 5, .15f, .3f, .5f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.3f, .1f, .75f, .5f);
        Mako_Set_Reverb(.6f, .2f, .5f);
        Mako_Set_IR(5, .5f, 0.5f, 6, .5f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.20f, 0.30f);
    }
    if (butt == &butPreset02) //R1.00 GRITTY 1
    {
        labPreset.setText("Gritty", juce::dontSendNotification);
        Mako_Set_Input(1, 200, 1000, .1);
        Mako_Set_Stages(.2f, .6f, .6f, 0.3f, 0.0f, 0.15f);

        mjSlidTone2.setValue(2000);  
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.8);
        mjSlidBite.setValue(0);        
        mjSlidFrenzy.setValue(.0f);
                        
        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0);     
        mjSlidComp3_Gain.setValue(.8);
        mjSlidMixHP.setValue(50);
      
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.65, .45, .5, 1500, 150, 900, .707f);
        Mako_Set_Mod(1, 4, .05f, .5f, .1f);
        Mako_Set_Mod2(0, 0, .15f, .3f, .5f);
        Mako_Set_Delay(.1f, .1f, .1f, .5f);
        Mako_Set_Reverb(.75f, .0f, .5f);
        Mako_Set_IR(6, .5f, 0.5f, 5, .5f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.5f, 0.2f);
    }
    if (butt == &butPreset03)   //R1.00 ETHER        
    {
        labPreset.setText("Ether", juce::dontSendNotification);
        Mako_Set_Input(0, 150, 600, .1f);
        Mako_Set_Stages(.25f, 1.0f, .0f, 0.0f, 0.1f, 0.0f);

        mjSlidTone2.setValue(3200);      
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.6);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.6);        
        mjSlidBite.setValue(.0);
        mjSlidFrenzy.setValue(.0);
                               
        //R1.00 Dry/Clean
        mjSlidMix.setValue(.9);         
        mjSlidComp3_Gain.setValue(.85);
        mjSlidMixHP.setValue(400);
      
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.6f, .4f, .5f, 1500, 150, 700, .31f);
        Mako_Set_Mod(3, 4, .7f, .65f, .3f);
        Mako_Set_Mod2(4, 5, .05f, .35f, .7f);
        Mako_Set_Delay(.4f, .5f, .05f, .5f);
        Mako_Set_Reverb(.7f, .6f, .5f);
        Mako_Set_IR(5, .5f, .5f, 1, .65f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.2f, 0.25f);

    }
    if (butt == &butPreset04) //R1.00 VAN HALEN Live
    {
        labPreset.setText("VH Live", juce::dontSendNotification);
        Mako_Set_Input(3, 440, 550, .1f);
        Mako_Set_Stages(.0f, .6f, .0f, 0.0f, 0.05f, 0.0f);

        mjSlidTone2.setValue(2600); 
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.20);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.6f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);
                
        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0f);
        mjSlidComp3_Gain.setValue(.6);
        mjSlidMixHP.setValue(225);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.55, .45, .45, 1500, 150, 1100, .4f);
        Mako_Set_Mod(2, 1, .17f, .35f, .2f);
        Mako_Set_Mod2(4, 4, .1f, 1.0f, .6f);
        Mako_Set_Delay(.25f, .3f, .1f, .5f);
        Mako_Set_Reverb(.6f, .2f, .5f);
        Mako_Set_IR(3, .5f, 0.5f, 4, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.3f, 0.42f);

    }
    if (butt == &butPreset05) //R1.00 90's
    {
        labPreset.setText("90's", juce::dontSendNotification);
        Mako_Set_Input(3, 500, 550, .1f);
        Mako_Set_Stages(.0f, .5f, .0f, 0.0f, 0.0f, 0.0f);

        mjSlidTone2.setValue(2900);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.10f);
        mjSlidBite.setValue(.5f);
        mjSlidFrenzy.setValue(.0f);
                
        //R1.00 Dry/Clean
        mjSlidMix.setValue(.85f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(50);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.55, .45, .45, 1500, 150, 1100, .4f);
        Mako_Set_Mod(1, 4, .3f, .5f, .5f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.3f, .15f, 1.0f, .5f);
        Mako_Set_Reverb(.6f, .1f, .5f);
        Mako_Set_IR(6, .5f, 0.5f, 5, .5f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.25f, 0.38f);

    }
    if (butt == &butPreset06)   //R1.00 COCKED WAH
    {
        labPreset.setText("Cocked", juce::dontSendNotification);
        Mako_Set_Input(2, 240, 600, 1.6f);
        Mako_Set_Stages(.08f, 1.0f, .0f, 0.0f, 0.4f, 0.0f);

        mjSlidTone2.setValue(2600);
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.5);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.4);
        mjSlidBite.setValue(.4);
        mjSlidFrenzy.setValue(.0f);
                
        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(290);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.65f, .4f, .7f, 1400, 150, 550, .71f);
        Mako_Set_Mod(5, 2, .5f, .35f, .65f);
        Mako_Set_Mod2(1, 2, .1f, .3f, .2f);
        Mako_Set_Delay(.5f, .1f, .1f, .5f);
        Mako_Set_Reverb(.75f, .1f, .5f);
        Mako_Set_IR(9, .7f, 0.4f, 4, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(1.0f, 0.31f);
    }
    if (butt == &butPreset07)   //R1.00 FUZZ LORD
    {
        labPreset.setText("Fuzz Lord", juce::dontSendNotification);
        Mako_Set_Input(2, 50, 400, .1f);
        Mako_Set_Stages(.0f, 1.0f, .0f, 0.0f, 0.0f, 0.0f);

        mjSlidTone2.setValue(3400); 
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0);
        mjSlidBite.setValue(.8);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.66);
        mjSlidComp3_Gain.setValue(.25);
        mjSlidMixHP.setValue(290);

        Mako_Opt_InvMid(1, true);
        Mako_Set_FinalEQ(.50f, .6f, .4f, 1500, 130, 550, .35f);
        Mako_Set_Mod(2, 5, .35f, .5f, .5f);
        Mako_Set_Mod2(1, 1, .05f, 1.0f, .05f);
        Mako_Set_Delay(.25f, .15f, 1.0f, .5f);
        Mako_Set_Reverb(.75f, .25f, .5f);
        Mako_Set_IR(3, .5f, 0.5f, 6, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(1.0f, 0.45f);
    }
    if (butt == &butPreset08)   //R1.00 PROGGY
    {
        labPreset.setText("Proggy", juce::dontSendNotification);
        Mako_Set_Input(2, 300, 950, .1f);
        Mako_Set_Stages(.0f, .75f, .0f, 0.0f, 0.25f, 0.0f);

        mjSlidTone2.setValue(2800);      
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.25);
        mjSlidBite.setValue(.0);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.2);
        mjSlidComp3_Gain.setValue(.3);
        mjSlidMixHP.setValue(190);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.7f, .2f, .5f, 1500, 140, 1100, .1f);
        Mako_Set_Mod(1, 1, .15f, 1.0f, .5f);
        Mako_Set_Mod2(4, 4, .15f, .3f, .5f);
        Mako_Set_Delay(.4f, .15f, 1.0f, .5f);
        Mako_Set_Reverb(.6f, .15f, .5f);
        Mako_Set_IR(5, .65f, 0.5f, 2, .5f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.25f, 0.5f);
    }
    if (butt == &butPreset09) //R1.00 STREXI 1
    {
        labPreset.setText("Strexi 1", juce::dontSendNotification);
        Mako_Set_Input(1, 250, 550, .1f);
        Mako_Set_Stages(.2f, .75f, .0f, 0.0f, 0.1f, 0.0f);

        mjSlidTone2.setValue(2900);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.2);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.5f);
        mjSlidComp3_Gain.setValue(.25);
        mjSlidMixHP.setValue(50);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.7f, .25f, .45f, 1500, 150, 700, .4f);
        Mako_Set_Mod(1, 5, .1f, .4f, .2f);
        Mako_Set_Mod2(0, 3, .2f, .5f, .45f);
        Mako_Set_Delay(.05f, .20f, .05f, .5f);
        Mako_Set_Reverb(.5f, .15f, .5f);
        Mako_Set_IR(6, .5f, 0.5f, 1, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.25f, 0.25f);

    }

    if (butt == &butPreset10) //R1.00 STREXI2
    {
        labPreset.setText("Strexi 2", juce::dontSendNotification);
        Mako_Set_Input(3, 340, 550, .1f);
        Mako_Set_Stages(.0f, .75f, .0f, 0.4f, 0.0f, 0.2f);

        mjSlidTone2.setValue(2500);       
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.75f);
        mjSlidComp3_Gain.setValue(.25);
        mjSlidMixHP.setValue(50);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.7f, .45f, .45f, 1500, 150, 700, .4f);
        Mako_Set_Mod(1, 1, .1f, .9f, .5f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.15f, .15f, 1.0f, .5f);
        Mako_Set_Reverb(.3f, .15f, .5f);
        Mako_Set_IR(4, .66f, 0.5f, 1, .66f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.05f, 0.38f);

    }
    if (butt == &butPreset11) //R1.00 Acoustic 1
    {        
        labPreset.setText("Acous 1", juce::dontSendNotification);
        Mako_Set_Input(0, 250, 50, .1f);
        Mako_Set_Stages(.10f, 1.0f, .0f, 0.0f, 0.0f, 0.0f);

        mjSlidTone2.setValue(5500); 
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.5f);
        mjSlidComp3_Gain.setValue(.1f);
        mjSlidMixHP.setValue(150);
        
        Mako_Opt_InvMid(1, true);
        Mako_Set_FinalEQ(.5f, .65f, .4f, 1500, 150, 525, .45f);
        Mako_Set_Mod(1, 4, .1f, .75f, .5f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.4f, .1f, 1.0f, .5f);
        Mako_Set_Reverb(.75f, .15f, .5f);
        Mako_Set_IR(8, .7f, 0.5f, 8, .6f, 0.5f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.25f, 0.6f);

    }
    if (butt == &butPreset12) //R1.00 VAN HALEN 1
    {        
        labPreset.setText("VH1", juce::dontSendNotification);
        Mako_Set_Input(3, 550, 550, .1f);
        Mako_Set_Stages(.0f, .5f, .0f, 0.2f, 0.25f, 0.0f);

        mjSlidTone2.setValue(2300);       
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.35);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.6f);
        mjSlidBite.setValue(.3f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(225);
        
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.55, .45, .45, 1500, 150, 1100, .4f);
        Mako_Set_Mod(4, 2, .15f, .3f, .5f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.5f, .2f, 1.0f, .5f);
        Mako_Set_Reverb(.7f, .4f, .75f);
        Mako_Set_IR(2, .7f, 0.5f, 4, .5f, 0.5f);

        mjSlidFinalBal.setValue(.3f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.35f, 0.38f);
    }
    if (butt == &butPreset13) //R1.00 METAL 1
    {
        labPreset.setText("Metal 1", juce::dontSendNotification);
        Mako_Set_Input(4, 400, 550, .1f);
        Mako_Set_Stages(.0f, .6f, .3f, 0.0f, 0.0f, 0.0f);

        mjSlidTone2.setValue(3800);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.5);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.1f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.15f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(225);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.6f, .35f, .5f, 1600, 100, 1200, 1.4f);
        Mako_Set_Mod(1, 2, .01f, .05f, .0f);
        Mako_Set_Mod2(0, 1, .15f, .3f, .5f);
        Mako_Set_Delay(.5f, .1f, 1.0f, .5f);
        Mako_Set_Reverb(.7f, .1f, .5f);
        Mako_Set_IR(4, .6f, 0.5f, 10, .8f, 0.55f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.6f, 0.32f);
    }
    if (butt == &butPreset14) //R1.00 AirJo
    {
        labPreset.setText("AirJo", juce::dontSendNotification);
        Mako_Set_Input(2, 200, 850, 1.2f);
        Mako_Set_Stages(.0f, 1.0f, .2f, 0.3f, .2f, 0.1f);

        mjSlidTone2.setValue(2800);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.65);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0f);
        mjSlidBite.setValue(.3f);        
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(290);
        
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.75f, .25f, .55f, 700, 170, 900, .91f);
        Mako_Set_Mod(1, 5, .1f, .2f, .5f);
        Mako_Set_Mod2(5, 2, .5f, .1f, .5f);
        Mako_Set_Delay(.3f, .25f, .15f, .5f);
        Mako_Set_Reverb(.75f, .1f, .5f);
        Mako_Set_IR(3, .5f, 0.5f, 7, .70f, 0.31f);
        
        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(.55f, 0.2);
    }
    if (butt == &butPreset15) //R1.00 CLEAN 2
    {
        labPreset.setText("Clean 2", juce::dontSendNotification);
        Mako_Set_Input(0, 150, 700, .707f);
        Mako_Set_Stages(.0f, 1.0f, .0f, 0.0f, 0.25f, 0.0f);

        mjSlidTone2.setValue(4000);    
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.2);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.7f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.6f);
        mjSlidComp3_Gain.setValue(.50);
        mjSlidMixHP.setValue(250);
       
        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.6f, .35f, .5f, 1400, 150, 900, .707f);
        Mako_Set_Mod(2, 4, .2f, .5f, .05f);
        Mako_Set_Mod2(5, 1, .2f, .5f, .5f);
        Mako_Set_Delay(.5f, .10f, 1.0f, .5f);
        Mako_Set_Reverb(.5f, .1f, .5f);
        Mako_Set_IR(5, .5f, 0.5f, 2, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.2f, 0.22f);

    }
    if (butt == &butPreset16)  //R1.00 THICC BOI
    {
        labPreset.setText("Thicc Boi", juce::dontSendNotification);
        Mako_Set_Input(3, 150, 900, .1);
        Mako_Set_Stages(.2f, .4f, .4f, 0.0f, 0.0f, 0.0f);

        mjSlidTone2.setValue(3000);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.35);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.65);
        mjSlidBite.setValue(.0);        
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.55f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(400);

        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.6, .4, .45, 1500, 150, 850, .707f);
        Mako_Set_Mod(1, 5, .15f, .2f, .5f);
        Mako_Set_Mod2(5, 1, .1f, .5f, .6f);
        Mako_Set_Delay(.3f, .2f, .35f, .5f);
        Mako_Set_Reverb(.75f, .1f, .5f);
        Mako_Set_IR(1, .65f, 0.5f, 10, .75f, 0.4f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.04f, 0.45f);
    }
    if (butt == &butPreset17)  //R1.00 THICSYN
    {
        labPreset.setText("Thic Syn", juce::dontSendNotification);
        Mako_Set_Input(3, 150, 900, .1);
        Mako_Set_Stages(.0f,.5f, .0f, 0.0f, 0.5f, 0.0f);

        mjSlidTone2.setValue(3000);
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0f);
        mjSlidEnvFade.setValue(.5f);
        mjSlidTube.setValue(.65);
        mjSlidBite.setValue(.0);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(1.0f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(400);

        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.5, .4, .55, 1500, 150, 600, .5f);
        Mako_Set_Mod(1, 2, .05f, .5f, .5f);
        Mako_Set_Mod2(5, 2, .4f, .8f, .3f);
        Mako_Set_Delay(.4f, .3f, 1.0f, .5f);
        Mako_Set_Reverb(.75f, .5f, .5f);
        Mako_Set_IR(4, .5f, 0.5f, 5, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.04f, 0.45f);
    }
    if (butt == &butPreset18)  //R1.00 CLEAN 3
    {
        labPreset.setText("Clean 3", juce::dontSendNotification);
        Mako_Set_Input(0, 190, 800, .707f);
        Mako_Set_Stages(.1f, 1.0f, .0f, 0.0f, 0.2f, 0.0f);

        mjSlidTone2.setValue(5500);
        Mako_Opt_EQPreCut(1, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0f);
        mjSlidBite.setValue(.0f);
        mjSlidFrenzy.setValue(.0f);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.5f);
        mjSlidComp3_Gain.setValue(.2);
        mjSlidMixHP.setValue(230);

        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(1.0f, .3f, .4f, 1500, 150, 800, .38f);
        Mako_Set_Mod(1, 2, .1f, 1.0f, .2f);
        Mako_Set_Mod2(4, 1, .35f, .75f, .5f);
        Mako_Set_Delay(.5f, .15f, .1f, .5f);
        Mako_Set_Reverb(.65f, .2f, .5f);
        Mako_Set_IR(5, .5f, 0.5f, 2, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.25f, 0.27f);
    }
    if (butt == &butPreset19)  //R1.00 MARINO
    {
        labPreset.setText("Marino", juce::dontSendNotification);
        Mako_Set_Input(2, 300, 850, .71f);
        Mako_Set_Stages(.35f, .75f, .0f, 0.0f, 0.7f, 0.0f);

        mjSlidTone2.setValue(3300);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.0);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0);
        mjSlidBite.setValue(.75);
        mjSlidFrenzy.setValue(.08);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.65);
        mjSlidComp3_Gain.setValue(.3);
        mjSlidMixHP.setValue(150);

        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(.7f, .3f, .5f, 1500, 150, 1100, .1f);
        Mako_Set_Mod(1, 1, .05f, 1.0f, .05f);
        Mako_Set_Mod2(5, 2, .1f, .2f, .7f);
        Mako_Set_Delay(.25f, .15f, 1.0f, .5f);
        Mako_Set_Reverb(.3f, .15f, .5f);
        Mako_Set_IR(6, .35f, 0.35f, 2, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(1.0f, 0.2f);
    }
    if (butt == &butPreset20)  //R1.00 PUNX
    {
        labPreset.setText("Punx", juce::dontSendNotification);
        Mako_Set_Input(1, 260, 850, .1f);
        Mako_Set_Stages(.0f, .6f, .0f, 0.0f, 0.25f, 0.0f);

        mjSlidTone2.setValue(4500);
        Mako_Opt_EQPreCut(0, true);

        //R1.00 OPTIONS
        mjSlidNGate.setValue(0.0f);
        mjSlidOD.setValue(.80);
        mjSlidEnvFade.setValue(.0f);
        mjSlidTube.setValue(.0);
        mjSlidBite.setValue(.0);
        mjSlidFrenzy.setValue(.0);

        //R1.00 Dry/Clean
        mjSlidMix.setValue(.4);
        mjSlidComp3_Gain.setValue(.4);
        mjSlidMixHP.setValue(230);

        Mako_Opt_InvMid(0, true);
        Mako_Set_FinalEQ(1.0f, .4f, .5f, 1500, 150, 1200, .38f);
        Mako_Set_Mod(1, 1, .05f, .1f, .15f);
        Mako_Set_Mod2(5, 5, .3f, .7f, .7f);
        Mako_Set_Delay(.1f, .2f, .05f, .5f);
        Mako_Set_Reverb(.05f, .1f, .5f);
        Mako_Set_IR(9, .7f, 0.42f, 5, .5f, 0.5f);

        mjSlidFinalBal.setValue(.5f);
        mjSlidFinalWidth.setValue(.0f);
        Mako_Set_Volume(0.75f, 0.15f);
    }

    ctrlHelp = 0;
    labHelp.setText("Preset", juce::dontSendNotification);
    labHelp2.setText("---", juce::dontSendNotification);

}


void MakoDist01AudioProcessorEditor::Mako_Set_Mod(int mType, int mLoc, float mRate, float mMix, float mDepth)
{
    mjSlidModType.setValue(mType);
    mjSlidModLoc.setValue(mLoc);
    mjSlidModRate.setValue(mRate);
    mjSlidModMix.setValue(mMix);
    mjSlidModDepth.setValue(mDepth);
}

void MakoDist01AudioProcessorEditor::Mako_Set_Mod2(int mType, int mLoc, float mRate, float mMix, float mDepth)
{
    mjSlidMod2Type.setValue(mType);
    mjSlidMod2Loc.setValue(mLoc);
    mjSlidMod2Rate.setValue(mRate);
    mjSlidMod2Mix.setValue(mMix);
    mjSlidMod2Depth.setValue(mDepth);
}

void MakoDist01AudioProcessorEditor::Mako_Set_Delay(float mDelay, float mMix, float mOffset, float mBalance)
{
    mjSlidDelayTime.setValue(mDelay);
    mjSlidDelayMix.setValue(mMix);
    mjSlidDelayOffset.setValue(mOffset);
    mjSlidDelayBal.setValue(.5f);
}

void MakoDist01AudioProcessorEditor::Mako_Opt_EQPreCut(bool ButtState, bool UpdateButton)
{
    if (UpdateButton) butEQPreCut.setToggleState(ButtState, false);
    if (ButtState == false) makoProcessor.makoEQPreCut = 0;
    if (ButtState == true)  makoProcessor.makoEQPreCut = 1;
}

void MakoDist01AudioProcessorEditor::Mako_FreqTest(bool ButtState, bool UpdateButton)
{
    if (UpdateButton) butFreqTest.setToggleState(ButtState, false);
    if (ButtState == false) makoProcessor.makoFreqTest = 0;
    if (ButtState == true)  makoProcessor.makoFreqTest = 1;
}


void MakoDist01AudioProcessorEditor::Mako_Opt_InvMid(bool ButtState, bool UpdateButton)
{    
    if (UpdateButton) butInvMid.setToggleState(ButtState, false);
    if (ButtState == false) makoProcessor.makoInvMid = 0;
    if (ButtState == true)  makoProcessor.makoInvMid = 1;
}

void MakoDist01AudioProcessorEditor::Mako_Opt_Initialize()
{
    //R1.00 This function reads the current button states and updates the processor variables.
    //R1.00 May not be needed anymore. Left to be safe.
    auto togstate = butEQPreCut.getToggleState();
    Mako_Opt_EQPreCut(togstate, false);
    
    togstate = butInvMid.getToggleState();
    Mako_Opt_InvMid(togstate, false);
}


void MakoDist01AudioProcessorEditor::timerCallback()
{       
    //***************************************************************************************
    //R1.00 VU METERS
    //R1.00 They are also not necesarily needed for VST since DAW will have them.
    //R1.00 Good for debugging code changes since you can flag where clipping occurs.
    //***************************************************************************************
     
    //R1.00 Show frequency under test during frequency sweep.
    //if (makoProcessor.makoFreqTest == 1) labClip.setText(std::to_string(makoProcessor.makoClip_Display), juce::dontSendNotification);
    
    if (makoProcessor.makoFreqTest == 2)
    {
        makoProcessor.makoFreqTest = 3;
        labPreset.setText("done", juce::dontSendNotification);
        repaint();
    }

    //R1.00 INPUT VU Meter - not needed for Distortion modes, we are always clipping. 
    if (makoProcessor.makoChan == 0)
    {
        //R1.00 Update our VU meter variables.
        if (1 < makoProcessor.makoClip_Display2)
            labVU_In1_On = true;
        else
            labVU_In1_On = false;
        if (20 < makoProcessor.makoClip_Display2)
            labVU_In2_On = true;
        else
            labVU_In2_On = false;
        if (40 < makoProcessor.makoClip_Display2)
            labVU_In3_On = true;
        else
            labVU_In3_On = false;
        if (60 < makoProcessor.makoClip_Display2)
            labVU_In4_On = true;
        else
            labVU_In4_On = false;
        if (80 < makoProcessor.makoClip_Display2)
            labVU_In5_On = true;
        else
            labVU_In5_On = false;
        if (95 < makoProcessor.makoClip_Display2)
            labVU_In6_On = true;
        else
            labVU_In6_On = false;
        
        //R1.00 If a VU segment has changed, update it. 
        //R1.00 This is faster than constantly updating the screen.
        if (labVU_In1_On != labVU_In1_On_Last)
        {
            labVU_In1_On_Last = labVU_In1_On;
            if (labVU_In1_On)
                labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF808080));
            else
                labVU_In1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
        if (labVU_In2_On != labVU_In2_On_Last)
        {
            labVU_In2_On_Last = labVU_In2_On;
            if (labVU_In2_On)
                labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF909090));
            else
                labVU_In2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
        if (labVU_In3_On != labVU_In3_On_Last)
        {
            labVU_In3_On_Last = labVU_In3_On;
            if (labVU_In3_On)
                labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFA0A0A0));
            else
                labVU_In3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
        if (labVU_In4_On != labVU_In4_On_Last)
        {
            labVU_In4_On_Last = labVU_In4_On;
            if (labVU_In4_On)
                labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFB0B0B0));
            else
                labVU_In4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
        if (labVU_In5_On != labVU_In5_On_Last)
        {
            labVU_In5_On_Last = labVU_In5_On;
            if (labVU_In5_On)
                labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFFF8000));
            else
                labVU_In5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
        if (labVU_In6_On != labVU_In6_On_Last)
        {
            labVU_In6_On_Last = labVU_In6_On;
            if (labVU_In6_On)
                labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFFF0000));
            else
                labVU_In6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
        }
    }

    //R1.00 OUTPUT VU METER
    //R1.00 Update our VU meter variables. 
    if (1 < makoProcessor.makoClip_Display)
        labVU_Out1_On = true;
    else
        labVU_Out1_On = false;
    if (20 < makoProcessor.makoClip_Display)
        labVU_Out2_On = true;
    else
        labVU_Out2_On = false;
    if (40 < makoProcessor.makoClip_Display)
        labVU_Out3_On = true;
    else
        labVU_Out3_On = false;
    if (60 < makoProcessor.makoClip_Display)
        labVU_Out4_On = true;
    else
        labVU_Out4_On = false;
    if (80 < makoProcessor.makoClip_Display)
        labVU_Out5_On = true;
    else
        labVU_Out5_On = false;
    if ((95 < makoProcessor.makoClip_Display) || (0 < makoProcessor.makoClipStage))
        labVU_Out6_On = true;
    else
        labVU_Out6_On = false;
    
    //R1.00 If a VU segment has changed, update it.
    //R1.00 This is faster than constantly updating the screen.
    if (labVU_Out1_On != labVU_Out1_On_Last)
    {
        labVU_Out1_On_Last = labVU_Out1_On;
        if (labVU_Out1_On)
            labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF808080));
        else
            labVU_Out1.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }
    if (labVU_Out2_On != labVU_Out2_On_Last)
    {
        labVU_Out2_On_Last = labVU_Out2_On;
        if (labVU_Out2_On)
            labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF909090));
        else
            labVU_Out2.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }
    if (labVU_Out3_On != labVU_Out3_On_Last)
    {
        labVU_Out3_On_Last = labVU_Out3_On;
        if (labVU_Out3_On)
            labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFA0A0A0));
        else
            labVU_Out3.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }
    if (labVU_Out4_On != labVU_Out4_On_Last)
    {
        labVU_Out4_On_Last = labVU_Out4_On;
        if (labVU_Out4_On)
            labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFB0B0B0));
        else
            labVU_Out4.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }
    if (labVU_Out5_On != labVU_Out5_On_Last)
    {
        labVU_Out5_On_Last = labVU_Out5_On;
        if (labVU_Out5_On)
            labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFFF8000));
        else
            labVU_Out5.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }
    if (labVU_Out6_On != labVU_Out6_On_Last)
    {
        labVU_Out6_On_Last = labVU_Out6_On;
        if ((95 < makoProcessor.makoClip_Display) || (0 < makoProcessor.makoClipStage))
        {
            switch (makoProcessor.makoClipStage)
            {
                //R1.00 No clipping. 
            case 0: labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFFB0B0B0)); break;
                //R1.00 PREAMP clipping. 
            case 1: labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(192, 192, 0)); break;
                //R1.00 Compressor clipping. 
            case 3: labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(192, 0, 192)); break;
                //R1.00 MASTER clipping. 
            default: labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(255, 0, 0)); break;
            }
        }
        else
            labVU_Out6.setColour(juce::Label::backgroundColourId, juce::Colour(0xFF202020));
    }

    //R1.00 Reset our CLIP flags. 
    makoProcessor.makoClipStage = 0;
    
}

void MakoDist01AudioProcessorEditor::Mako_Set_Volume(float Gain, float Volume)
{
    mjSlidVol.setValue(Gain);
    mjSlidVol2.setValue(Volume);
}

void MakoDist01AudioProcessorEditor::Mako_Set_Input(float Channel, int LowPassFreq, int MidFreq, float MidQ)
{    
    mjSlidChan.setValue(Channel);
    mjSlidTone.setValue(LowPassFreq);       
    mjSlidTone5.setValue(MidFreq);      
    mjSlidTone5_Q.setValue(MidQ);
}

void MakoDist01AudioProcessorEditor::Mako_Set_Stages(float Comp1, float Slope1, float Round1, float Asym, float PowerAmp, float Comp2) //float Comp2, float Slope2, float Round2)
{
    //R1.00 STAGE 1
    mjSlidComp_Gain.setValue(Comp1);
    mjSlidBreak1.setValue(Slope1);     
    mjSlidSlope1.setValue(Round1);     
    mjSlidAsym1.setValue(Asym);
    //R1.00 STAGE 2
    
    //mjSlidBreak2.setValue(Slope2);    
    //mjSlidSlope2.setValue(Round2);    
    mjSlidPowerAmp.setValue(PowerAmp);
    mjSlidComp2_Gain.setValue(Comp2);
}

void MakoDist01AudioProcessorEditor::Mako_Set_FinalEQ(float Treble, float Mid, float Bass, int HiCut, int LowCut, int MidCut, float MidQ)
{
    //R1.00 FINAL EQ
    mjSlidTreble.setValue(Treble);
    mjSlidMid.setValue(Mid);
    mjSlidBass.setValue(Bass);

    mjSlidTrebleCut.setValue(HiCut);
    mjSlidBassCut.setValue(LowCut);

    mjSlidTone6.setValue(MidCut);
    mjSlidTone6_Q.setValue(MidQ);
}

void MakoDist01AudioProcessorEditor::Mako_Set_IR(int IR1, float Voice1, float SpeakerSize1, int IR2, float Voice2, float SpeakerSize2)
{
    mjSlidIRL.setValue(IR1);
    mjSlidIRR.setValue(IR2);
    mjSlidIRLMix.setValue(Voice1);
    mjSlidIRRMix.setValue(Voice2);
    mjSlidIRLSize.setValue(SpeakerSize1);
    mjSlidIRRSize.setValue(SpeakerSize2);
}

void MakoDist01AudioProcessorEditor::Mako_Set_Reverb(float Decay, float Mix, float Balance)
{
    mjSlidReverbTime.setValue(Decay);
    mjSlidReverbMix.setValue(Mix);    
    mjSlidReverbBal.setValue(Balance);
}

void MakoDist01AudioProcessorEditor::GFX_Pedals_Draw(bool ForceUpdate)
{
    //R1.00 Only update if something has changed or asked to update.
    if ((PEDAL_ON != PEDAL_ON_LAST) || (ForceUpdate))
    {
        PEDAL_ON_LAST = PEDAL_ON;
        GFX_Pedal_Refresh = true;
        repaint();
        GFX_Pedal_Refresh = false;
    }
}

void MakoDist01AudioProcessorEditor::GUI_Init_Large_Slider(juce::Slider* slider)
{
    slider->setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFFC00000));
    slider->setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(colorButton));
}

void MakoDist01AudioProcessorEditor::GUI_Init_Small_Slider(juce::Slider* slider)
{
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    slider->setColour(juce::Slider::textBoxTextColourId, juce::Colour(0xFFA0A0A0));
    slider->setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xFF202020));
    slider->setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::textBoxHighlightColourId, juce::Colour(0xFF600000));
    slider->setColour(juce::Slider::trackColourId, juce::Colour(0xFFC00000));
    slider->setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF000000));
    slider->setColour(juce::Slider::thumbColourId, juce::Colour(colorButton));
}

