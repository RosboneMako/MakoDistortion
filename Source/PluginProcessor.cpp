/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "cmath" 
#include "Windows.h"

//R1.00 DO NOT DECLARE ANY GLOBAL VARIABLES
//R1.00 This may cause major issues when multiple instances are used in DAWs.

//==============================================================================
MakoDist01AudioProcessor::MakoDist01AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    
    //R1.00 Added for VALUE TREE.
    parameters (*this, nullptr, "PARAMETERS",
        {
            std::make_unique<juce::AudioParameterFloat>(  "gain",    "Gain", .01f, 1.0f, .1f),
            std::make_unique<juce::AudioParameterInt>(    "chan",    "Chan",  0, 3, 0),
            std::make_unique<juce::AudioParameterInt>(    "tone",    "Tone",  50, 550, 80),
            std::make_unique<juce::AudioParameterInt>(   "tone5",   "Tone5", 400, 1200, 700),
            std::make_unique<juce::AudioParameterFloat>("tone5_q", "Tone5_Q", .1f, 2.0f, .707f),
            std::make_unique<juce::AudioParameterFloat>("od",    "OverDrive", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("envfade","EnvFade", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>( "ngate",   "NGate", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>(  "tube",    "Tube", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>(  "bite",    "Bite", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("frenzy",  "frenzy", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>( "comp1",  "Comp 1", .0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>( "comp2",  "Comp 2", .0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>( "comp3",  "Comp 3", .0f, 1.0f, 1.0f),   
            std::make_unique<juce::AudioParameterFloat>("break1", "Break 1", .0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("break2", "Break 2", .0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("slope1", "Slope 1", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("slope2", "Slope 2", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("asym1", "Asym 1", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterFloat>("poweramp", "Power Amp", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterInt>(   "tone2",  "Tone 2", 500, 5500, 2000),
            std::make_unique<juce::AudioParameterFloat>(   "mix",     "Mix", .0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>( "mixhp",  "Mix HP", .0f, 1.0f, .0f),
            std::make_unique<juce::AudioParameterBool>("invmid",  "Inv Mid", false),
            std::make_unique<juce::AudioParameterBool>("eqprecut",  "EQ Pre Cut", false),
            
            std::make_unique<juce::AudioParameterFloat>("treble",  "Treble", .0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>(   "mid",     "Mid", .0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>(  "bass",    "Bass", .0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterInt>("treblecut", "Treble Cut", 500, 3000, 1500),
            std::make_unique<juce::AudioParameterInt>( "basscut","Bass Cut", 80, 400, 150),
            std::make_unique<juce::AudioParameterInt>("tone6",   "Tone6", 200, 1600, 700),
            std::make_unique<juce::AudioParameterFloat>("tone6_q", "Tone6_Q", .1f, 2.0f, .707f),
            std::make_unique<juce::AudioParameterFloat>("vol2",    "Vol 2", .01f, 1.0f, .35f),
            std::make_unique<juce::AudioParameterFloat>("finalbal","Final Bal", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("finalwidth","Final Width", 0.0f, 1.0f, .0f),

            std::make_unique<juce::AudioParameterInt>("modtype",   "Mod Type", 0, 5, 0),
            std::make_unique<juce::AudioParameterInt>("modloc",   "Mod Loc", 0, 5, 0),
            std::make_unique<juce::AudioParameterFloat>("modrate",   "Mod Rate", 0.0f, 1.0f, .3f),
            std::make_unique<juce::AudioParameterFloat>("modmix",   "Mod Mix", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("moddepth",   "Mod Depth", 0.0f, 1.0f, .5f),

            std::make_unique<juce::AudioParameterInt>("mod2type",   "Mod 2 Type", 0, 5, 0),
            std::make_unique<juce::AudioParameterInt>("mod2loc",   "Mod 2 Loc", 0, 5, 0),
            std::make_unique<juce::AudioParameterFloat>("mod2rate",   "Mod 2 Rate", 0.0f, 1.0f, .3f),
            std::make_unique<juce::AudioParameterFloat>("mod2mix",   "Mod 2 Mix", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("mod2depth",   "Mod 2 Depth", 0.0f, 1.0f, .5f),

            std::make_unique<juce::AudioParameterFloat>("delaytime",   "Delay Time", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("delaymix",    "Delay Mix", 0.0f, 1.0f, .15f),
            std::make_unique<juce::AudioParameterFloat>("delayoffset", "Delay Offset", 0.0f, 1.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("delaybal",    "Delay Bal", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("delayrepeat", "Delay Repeat", 0.0f, 1.0f, .25f),

            std::make_unique<juce::AudioParameterFloat>("reverbmix",  "Reverb Mix", 0.0f, 1.0f, .15f),
            std::make_unique<juce::AudioParameterFloat>("reverbtime","Reverb Time", 0.0f, 1.0f, .75f),
            std::make_unique<juce::AudioParameterFloat>("reverbbal",  "Reverb Bal", 0.0f, 1.0f, .5f),

            std::make_unique<juce::AudioParameterInt>("irl", "IR Left", 0, 10, 1),
            std::make_unique<juce::AudioParameterInt>("irr", "IR Right", 0, 10, 4),
            std::make_unique<juce::AudioParameterFloat>("irlmix",  "IR Mix L",  0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("irrmix",  "IR Mix R",  0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("irlsize", "IR L Size", 0.0f, 1.0f, .5f),
            std::make_unique<juce::AudioParameterFloat>("irrsize", "IR R Size", 0.0f, 1.0f, .5f)

        }
    )

#endif
{
    


}

MakoDist01AudioProcessor::~MakoDist01AudioProcessor()
{
}

//==============================================================================
const juce::String MakoDist01AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MakoDist01AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MakoDist01AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MakoDist01AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MakoDist01AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MakoDist01AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MakoDist01AudioProcessor::getCurrentProgram()
{
    return 0;
}

void MakoDist01AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MakoDist01AudioProcessor::getProgramName (int index)
{
    return {};
}

void MakoDist01AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MakoDist01AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    //*******************************************************************
    // MAKO VARIABLES
    //*******************************************************************
    //R1.00 Define our Reverb Vars.
    Reverb_B_Ring1_Gain[0] = 0.5f;  
    Reverb_B_Ring1_GainInv[0] = .5f;
    Reverb_B_Ring1[0][0] = 1210;
    Reverb_B_Ring1[1][0] = 2210;
    Reverb_B_Ring1_Gain[1] = .75f;
    Reverb_B_Ring1_GainInv[0] = .25f;
    Reverb_B_Ring1[0][1] = 210;
    Reverb_B_Ring1[1][1] = 410;
    Reverb_B_Ring1_Gain[2] = .75f;
    Reverb_B_Ring1_GainInv[2] = .25f;
    Reverb_B_Ring1[0][2] = 158;
    Reverb_B_Ring1[1][2] = 358;     
    Reverb_B_Ring1_Gain[3] = .625f;
    Reverb_B_Ring1_GainInv[3] = .375f;
    Reverb_B_Ring1[0][3] = 561;
    Reverb_B_Ring1[1][3] = 761;
    Reverb_B_Ring1_Gain[4] = .625f;
    Reverb_B_Ring1_GainInv[4] = .375f;
    Reverb_B_Ring1[0][4] = 410;
    Reverb_B_Ring1[1][4] = 610;
    Reverb_B_Ring1_Gain[5] = .7f;
    Reverb_B_Ring1_GainInv[5] = .3f;
    Reverb_B_Ring1[0][5] = 908;
    Reverb_B_Ring1[1][5] = 1108;
    Reverb_B_Ring1_Gain[6] = .7f;
    Reverb_B_Ring1_GainInv[6] = .3f;
    Reverb_B_Ring1[0][6] = 1343;
    Reverb_B_Ring1[1][6] = 1543;
    Reverb_B_Ring1_Gain[7] = .3f;
    Reverb_B_Ring1_GainInv[7] = .7f;
    Reverb_B_Ring1[0][7] = 995;
    Reverb_B_Ring1[1][7] = 1195;
    Reverb_B_Ring1_Gain[8] = .3f;
    Reverb_B_Ring1_GainInv[8] = .7f;
    Reverb_B_Ring1[0][8] = 6590;
    Reverb_B_Ring1[1][8] = 6790;
    Reverb_B_Ring1_Gain[9] = .5f;
    Reverb_B_Ring1_GainInv[9] = .5f;
    Reverb_B_Ring1[0][9] = 3931;
    Reverb_B_Ring1[1][9] = 4131;
    Reverb_B_Ring1_Gain[10] = .5f;
    Reverb_B_Ring1_GainInv[10] = .5f;
    Reverb_B_Ring1[0][10] = 2664;
    Reverb_B_Ring1[1][10] = 2864;
    Reverb_B_Ring1_Gain[11] = .3f;
    Reverb_B_Ring1_GainInv[11] = .7f;
    Reverb_B_Ring1[0][11] = 4641;
    Reverb_B_Ring1[1][11] = 4841;
    Reverb_B_Ring1_Gain[12] = .3f;
    Reverb_B_Ring1_GainInv[12] = .7f;
    Reverb_B_Ring1[0][12] = 5505;
    Reverb_B_Ring1[1][12] = 5705;
    Reverb_B_Ring1_Gain[13] = .25f;
    Reverb_B_Ring1_GainInv[13] = .75f;
    Reverb_B_Ring1[0][13] = 9300;
    Reverb_B_Ring1[1][13] = 9500;
    Reverb_B_Ring1_Gain[14] = .25f;
    Reverb_B_Ring1_GainInv[14] = .75f;
    Reverb_B_Ring1[0][14] = 12300;
    Reverb_B_Ring1[1][14] = 12500;
    Reverb_B_Ring1_Gain[15] = .2f;
    Reverb_B_Ring1_GainInv[15] = .8f;
    Reverb_B_Ring1[0][15] = 14500;
    Reverb_B_Ring1[1][15] = 14700;

    Reverb_B_Ring1_Max[0] = 16300;
    Reverb_B_Ring1_Max[1] = 16300;
    
    //R1.00 Get the SAMPLE RATE.
    //char SR[100]; 
    //sprintf(SR, "RATE: %f \n", MakoDist01AudioProcessor::getSampleRate());
    //MessageBox(0, SR, "Error", MB_ICONSTOP);
    makoSampleRate = MakoDist01AudioProcessor::getSampleRate();
    if (makoSampleRate < 21000) makoSampleRate = 48000;
    if (192000 < makoSampleRate) makoSampleRate = 48000;

    //R1.00 Calculate some rough decay subtraction values for peak tracking (compress,autowah,etc). 
    Release_100mS = (1.0f / .100f) * (1.0f / makoSampleRate);
    Release_200mS = (1.0f / .200f) * (1.0f / makoSampleRate);
    Release_300mS = (1.0f / .300f) * (1.0f / makoSampleRate);
    Release_400mS = (1.0f / .400f) * (1.0f / makoSampleRate);
    Release_500mS = (1.0f / .500f) * (1.0f / makoSampleRate);

    //R1.00 REVERB MODULATE Vars.
    RevMod_B_Rate_Up = 1.0f / makoSampleRate;       //R1.00 RATE How fast we traverse thru the modulation indexes.
    RevMod_B_Rate_Down = -1.0f / makoSampleRate;
    RevMod_B_Rate = RevMod_B_Rate_Up;               //R1.00 Starting rate.
    RevMod_B_Ring1[0] = 0;         //R1.00 index into buffer.
    RevMod_B_Ring1_Max[0] = 8000;  //R1.00 Size of buffer.
    RevMod_B_Offset[0] = 1190;     //R1.00 Where our mod index starts
    RevMod_B_Offset_Min[0] = 880;  //R1.00 580 - Min distance our index can get from the buffer index.
    RevMod_B_Offset_Max[0] = 1460; //R1.00 1860 - Max distance our index can get from the buffer index.
    RevMod_B_Ring1[1] = 0;
    RevMod_B_Ring1_Max[1] = 8000;
    RevMod_B_Offset[1] = 900;       //R1.00 We will modulate from 1mS (48) onward. 
    RevMod_B_Offset_Min[1] = 880;
    RevMod_B_Offset_Max[1] = 1460;

    //R1.00 DELAY Vars.
    Delay_B_Ring1[0] = 15000;
    Delay_B_Ring1_Max[0] = 15001;
    Delay_B_Ring1[1] = 30000;
    Delay_B_Ring1_Max[1] = 30001;

    //R1.00 MODULATE Vars.
    Modulate_B_Rate_Up = 1.0f / makoSampleRate;     //R1.00 RATE How fast we traverse thru the modulation indexes.
    Modulate_B_Rate_Down = -1.0f / makoSampleRate;
    Modulate_B_Rate = Modulate_B_Rate_Up;           //R1.00 Starting rate.
    Modulate_B_Ring1[0] = 0;         //R1.00 index into buffer.
    Modulate_B_Ring1_Max[0] = 8000;  //R1.00 Size of buffer.
    Modulate_B_Offset[0] = 1600;     //R1.00 1220 - Where our mod index starts
    Modulate_B_Offset_Min[0] = 280;  //R1.00 580 - Min distance our index can get from the buffer index.
    Modulate_B_Offset_Max[0] = 1860; //R1.00 1860 - Max distance our index can get from the buffer index.
    
    Modulate_B_Ring1[1] = 0;         
    Modulate_B_Ring1_Max[1] = 8000;  
    Modulate_B_Offset[1] = 800;       //R1.00 581 - We will modulate from 1mS (48) onward. 
    Modulate_B_Offset_Min[1] = 280;   //R1.00 580
    Modulate_B_Offset_Max[1] = 1860;  //R1.00 1860 
    
    //R1.00 MODULATE 2 Vars.
    Modulate2_B_Rate_Up = 1.0f / makoSampleRate;     //R1.00 RATE How fast we traverse thru the modulation indexes.
    Modulate2_B_Rate_Down = -1.0f / makoSampleRate;
    Modulate2_B_Rate = Modulate2_B_Rate_Up;                                     //R1.00 Starting rate.
    Modulate2_B_Ring1[0] = 0;         //R1.00 index into buffer.
    Modulate2_B_Ring1_Max[0] = 8000;  //R1.00 Size of buffer.
    Modulate2_B_Offset[0] = 1600;     //R1.00 Where our mod index starts
    Modulate2_B_Offset_Min[0] = 280;  //R1.00 580 - Min distance our index can get from the buffer index.
    Modulate2_B_Offset_Max[0] = 1860; //R1.00 1860 - Max distance our index can get from the buffer index.

    Modulate2_B_Ring1[1] = 0;
    Modulate2_B_Ring1_Max[1] = 8000;
    Modulate2_B_Offset[1] = 800;       //R1.00 We will modulate from 1mS (48) onward. 
    Modulate2_B_Offset_Min[1] = 280;
    Modulate2_B_Offset_Max[1] = 1860;

    //R1.00 init IR Cabs.
    Mako_SetIR_Left();
    Mako_SetIR_Right();
    
    //R1.00 Calculate the default filter coefficients.
    F1_Input_High_coeffs(makoTone);
    F2_PostGain_Low_coeffs(makoTone2);
    F5_Input_Mid_coeffs(makoTone5_Q, makoTone5);
    F12_Final_LowCut_coeffs(60.0f);

    F10_Phaser_Init();
    F10_Phaser2_Init();

    //R1.00 FINAL EQ Filters. 
    F3_Final_Treble_coeffs(1500.0f);
    F4_Final_Bass_coeffs(150.0f);
    F6_Final_Mid_coeffs(makoTone6_Q, makoTone6);
    //F6_Final_Mid_coeffs_BW(makoTone6_Q * 100.0f, makoTone6);
    F7_Clean_High_coeffs(50.0f);
}

void MakoDist01AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MakoDist01AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MakoDist01AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    float tS = 0;
    float tS2 = 0;
    float tS_Bite = 0;
    float tSign = 1;
    float tMixSign = 1;
    float tS_Temp = 0;
    float tS_Bass = 0;
    float tS_Mid_EQ = 0;
    float tS_Treble = 0;
    float tMixSample = 0.0f;
    
    //R1.00 Update our output and input PEAK VOLUME variables.
    makoClip_Display = makoClip_Peak * 100;
    makoClip_Peak *= .99;
    makoClip_Display2 = makoClip_Peak2 * 100; 
    makoClip_Peak2 *= .99;

    //R1.00 Make a copy of sample rate. JUCE says this is the best place to read it.
    makoSampleRate = MakoDist01AudioProcessor::getSampleRate();

    //R1.00 Handle any settings changes made in Editor. 
    if (0 < makoSettingsChanged) Mako_SettingsUpdate();

    // JUCE NOTES
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //JUCE NOTES
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
        for (int samp = 0; samp < buffer.getNumSamples(); samp++)
        {
            //*********************************************************************************************
            //R1.00 MAKO INFORMATION
            // This app has two paths, Dry path and Wet path. 
            // The Wet path uses the normal controls to gain distortion and filtering.
            // The Dry path is a clean signal for Amp Chans 0,1,2. Chan 3 is reduced gain distortion.
            // Modulations can be placed in either path.
            //*********************************************************************************************

            //R1.00 Get SAMPLE
            tS = buffer.getSample(channel, samp);    //R1.00 Get the RAW untouched sample. 
            
            //R1.00 Frequency Test. 
            if (makoFreqTest == 1) tS = Mako_FreqTest(channel);
            
            //R1.00 Track our Input Signal Average (Absolute vals).
            Signal_AVG[channel] = (Signal_AVG[channel] * .995) + (abs(tS) * .005);
                        
            //R1.00 Noise gate.
            if (0 < makoparmNGate) tS = Mako_FX_NoiseGate(tS, channel);
            
            //R1.00 FRENZY - Store our max signal to restore volume envelope after gain/compression.
            if (0.0f < makoFrenzy)
            {
                //R1.00 Get new MAX vol or slowly reduce the current max value. Needs to be SampleRate calc.
                if (FrenzyVol[channel] < abs(tS))
                    FrenzyVol[channel] = abs(tS);
                else
                {
                    FrenzyVol[channel] -= Release_500mS;
                    if (FrenzyVol[channel] < 0.0f) FrenzyVol[channel] = 0.0f;
                }
            }

            // **************************************************
            // R1.00 DRY PATH
            // **************************************************
            //R1.00 Store a clean sample to work with.
            tMixSample = tS;
                        
            //R1.00 DRY PATH - High Pass Filter
            tMixSample = Mako_EQ_Dry_HighPass(tMixSample, channel);
            
            //R1.00 Do comp after gain for METAL amp (3).
            if (makoChan < 3) tMixSample = Mako_FX_Compress_Clean(tMixSample, channel);

            
            // **************************************************
            // R1.00 GAIN/WET PATH
            // **************************************************
            // R1.00 FX - OverDrive
            if (0.001f < makoOD) tS = Mako_FX_OD(tS);

            // R1.00 PRE GAIN / INPUT FILTERS
            tS = Mako_EQ_Input_Filters(tS, channel);
            
            //R1.00 Store filtered clean signal.
            tS_Bite = tS;
                        

            //*************************************************
            //R1.00 PRE GAIN EFFECTS
            //*************************************************
            //R1.00 FX - Input Compression
            tS = Mako_FX_Compress_1(tS, channel);

            //R1.00 FADE IN
            if (0.001f < makoEnvFade) tS = Mako_FX_Attack(tS, channel);
          
            //R1.00 PRE GAIN STAGE Modulation. 
            if (makoModLoc == 2) tS = Mako_Modulate(tS, channel);
            if (makoMod2Loc == 2) tS = Mako_Modulate2(tS, channel);
                        

            //*************************************************************
            //R1.00 GAIN STAGES
            //*************************************************************
            
            //R1.00 HIGH PASS FILTER #2 - BITE - Store this filtered sample for later use.
            tS_Bite = Mako_FX_Bite(tS, channel);

            //R1.00 GAIN and CLIP.
            switch (makoChan)
            {
                case 0: 
                    tS *= (makoVol * 5.0f);                     
                    if (makoClip_Peak2 < abs(tS)) makoClip_Peak2 = abs(tS);  //R1.00 Input VU Meter - Store the loudest Sample.
                    break;      //R1.00 Clean. Dry path is no gain.
                case 1: tS *= (makoVol * 50.0f); break;     //R1.00 Mix.   Dry path is no gain.
                case 2: tS *= (makoVol * 500.0f); break;    //R1.00 Rock.  Dry path is no gain.
                case 3: tS *= (makoVol * 2000.0f); break;   //R1.00 Metal. Dry path is partial gain.
            }
            
            //R1.00 DISTORTION STAGE 1
            tS = Mako_FX_Dist_Stage1(tS);

            //R1.00 BITE - Add some negative signal back. 
            if (0.001f < tS_Bite) tS = tS * .95f + (tS_Bite * makoBiteGain * -.25);

            //R1.00 TUBE IT - Low pass filter adjustment based on volume. Envelope filter.
            if (0.001f < makoparmTube) tS = Mako_FX_Tube(tS, channel);
                        
            //R1.00 FIZZ - Post gain Low Pass Filter. 
            tS = Mako_EQ_Fizz_Filter(tS, channel);
            
            //******************************************************************
            // R1.00 POWER AMP STAGE for PRE EQ Power Amp stage.
            //******************************************************************
            if ((makoEQPreCut == 1) && (0.001f < makoPowerAmp))
            {
                tS = Mako_FX_ODHT(tS);
                tS = Mako_FX_Compress_2(tS, channel);                
            }

            //R1.00 Handle different amp channels.
            if (makoChan < 3)
            {
                //R1.00 CHAN  0-2 POST GAIN STAGE Modulation. 
                if (makoModLoc == 1) tMixSample = Mako_Modulate(tMixSample, channel);
                if (makoMod2Loc == 1) tMixSample = Mako_Modulate2(tMixSample, channel);
            }
            else
            {
                //R1.0 CHAN 3 - METAL - Create a less distorted version of signal. 
                tMixSample = Mako_FX_Chan3_Dist_Dry(tMixSample);
                tMixSample = Mako_FX_Compress_Clean(tMixSample, channel);
                if (makoModLoc == 1) tMixSample = Mako_Modulate(tMixSample, channel);
                if (makoMod2Loc == 1) tMixSample = Mako_Modulate2(tMixSample, channel);
            }

            //R1.00 Get a FINAL mix of Dry + Wet.
            tS = (tS * makoMix) + (tMixSample * (1 - makoMix));
            tS = Mako_EQ_Final(tS, channel);            

            //R1.00 FRENZY
            if (0.0f < makoFrenzy)
            {

                tS = (tS * (1 - makoFrenzy)) + ((4.0f * tS * FrenzyVol[channel]) * makoFrenzy);        //tS = tS + (FrenzyVol[channel] * makoFrenzy);

            }
            tS = Mako_Clip(tS);
            
            //******************************************************************
            // R1.00 POWER AMP STAGE for POST EQ Power Amp (default). 
            //******************************************************************
            if ((makoEQPreCut == 0)  && (0.001f < makoPowerAmp))
            {
                tS = Mako_FX_ODHT(tS);
                tS = Mako_FX_Compress_2(tS, channel);                
            }

            tS = Mako_Clip(tS);
            
            //R1.00 Apply FX post amp code.
            if (makoModLoc == 3) tS = Mako_Modulate(tS, channel);
            if (makoMod2Loc == 3) tS = Mako_Modulate2(tS, channel);
            
            //R1.00 Apply Impulse Response, Time effects, and Volume.
            tS = Mako_CabSim(tS, channel);
            tS = Mako_FX_Delay(tS, channel);
            tS = Mako_FX_Reverb(tS, channel);
            tS = Mako_Volume_Final(tS, channel);

            //R1.00 Track our highest peak volume. 
            if (makoClip_Peak < abs(tS)) makoClip_Peak = abs(tS);
                        
            //R1.00 Store loudest sample for Freq Response test.
            if (makoFreqTest == 1)
                if ((channel == 0) && (makoFreqTestMax < abs(tS))) makoFreqTestMax = abs(tS);
            

            //R1.00 Save new data or do Tune/Pitch pipe.
            if ((makoTune == 1)) 
                channelData[samp] = (Mako_TuneA(channel) + tS) * .5f;
            else
            {
                //R1.00 Dont apply WIDTH to the frequency test. Only apply to RIGHT channel (1).
                if ((makoFreqTest != 1) && (channel == 1)) tS = Mako_FX_Width(tS);
                channelData[samp] = tS;
            }
        }
    }

    
}



float MakoDist01AudioProcessor::Mako_FX_NoiseGate(float tSample, int channel)
{
    //R1.00 Create a volume envelope based on Signal Average.
    makoNGate_Fac[channel] = Signal_AVG[channel] * 10000.0f * (1.1f - makoparmNGate);
    if (1.0f < makoNGate_Fac[channel]) makoNGate_Fac[channel] = 1.0f;

    return tSample * makoNGate_Fac[channel];
}

float MakoDist01AudioProcessor::Mako_FX_Attack(float tSample, int channel)
{
    //R1.00 A slow envelope attack for violin/synth effects.
    //R1.00 Detect when a note is played.
    if (!Signal_VolFadeOn[channel] && (.001f < tSample))
    {
        Signal_VolFadeOn[channel] = true;
        Signal_VolFade[channel] = 0.0f;
    }

    //R1.00 Check for Note off period.
    if (Signal_AVG[channel] < .0001f) Signal_VolFadeOn[channel] = false;

    //R1.00 Ramp up or down the effect volume based on if playing or not.
    if (.0005f < Signal_AVG[channel])
        Signal_VolFade[channel] += .000001f + (1.0f - makoEnvFade) * .0001f;  //R1.00 Fade in.
    else
        Signal_VolFade[channel] *= -.995f;                                    //R1.00 Fade out.

    //R1.00 Clip the volume near unity.
    if (.999f < Signal_VolFade[channel]) Signal_VolFade[channel] = .999f;

    return tSample * Signal_VolFade[channel];
}

//R1.00 Dynamic low pass filter.
float MakoDist01AudioProcessor::Mako_FX_Tube(float tSample, int channel)
{
    float tFAC = Signal_AVG[channel] * 1000.0f * ((1.0f - makoparmTube) * (1.0f - makoparmTube));
    if (.90f < tFAC) tFAC = .90f;

    float tS = tSample * (.1f + tFAC) + (Signal_Last[channel] * (1.0f - (.1f + tFAC)));
    Signal_Last[channel] = tS;

    return tS;
}

//R1.00 Add some high freqs back onto the compressed high gain signal. 
float MakoDist01AudioProcessor::Mako_FX_Bite(float tSample, int channel)
{
    float tS = tSample;

    if (0.0f < makoBiteGain)   
    {
        //R1.00 High Pass Filter. We add this result back on later. 
        f1b_xn[channel] = abs(tS);
        tS = f1b_coeffs.a0 * f1b_xn[channel] + f1b_coeffs.a1 * f1b_xnz1[channel] + f1b_coeffs.a2 * f1b_xnz2[channel] - f1b_coeffs.b1 * f1b_ynz1[channel] - f1b_coeffs.b2 * f1b_ynz2[channel];
        f1b_xnz2[channel] = f1b_xnz1[channel]; f1b_xnz1[channel] = f1b_xn[channel]; f1b_ynz2[channel] = f1b_ynz1[channel]; f1b_ynz1[channel] = tS;
    }
    else
        tS = 0.0f;

    return tS;
}

//R1.00 FINAL Volume.
float MakoDist01AudioProcessor::Mako_Volume_Final(float tSample, int channel)
{
    float tS = tSample;

    //R1.00 Slowly fade volume in when making PRESET changes.
    if (makoVol2_Setting < makoVol2)
        makoVol2 = makoVol2_Setting;
    else
        makoVol2 = (makoVol2 * .99995) + (makoVol2_Setting * .00005);

    //R1.00 Apply MASTER volume. 
    switch (makoChan)
    {
        case 0: tS = tS * (makoVol2 * makoVol2 * 140); break;  //R1.00 Clean.
        case 1: tS = tS * (makoVol2 * makoVol2 * 55); break;   //R1.00 Edge.
        case 2: tS = tS * (makoVol2 * makoVol2 * 12); break;   //R1.00 Rock.
        case 3: tS = tS * (makoVol2 * makoVol2 * 12); break;   //R1.00 Metal.
    }

    //R1.00 Set FINAL BALANCE
    tS *= makoFinalBal[channel];

    //R1.00 Final CLIP. Flag that we are clipping.
    if (tS < -.999f)
    {
        tS = -.999f;
        makoClipStage = 2;
    }
    if (.999f < tS)
    {
        tS = .999f;
        makoClipStage = 2;
    }

    return tS;
}

float MakoDist01AudioProcessor::Mako_FreqTest(int channel)
{
    //R1.00 Try to get the output channels in sync.
    if (channel == 0) makoFreqTestStart = 1;
    if (makoFreqTestStart = 0) return 0.0f;    

    //R1.00 We are starting the test.
    if (makoFreqTest_F[channel] < 1)
    {
        makoFreqGfxStart = 20;
        makoFreqGfxStop = 20;
        makoFreqTest_F[channel] = 19;
        makoFreqTest_A[channel] = 10.0f;
        makoFreqTest_Stp[channel] = 0.0f;
    }

    //R1.00 Update our current sine wave.
    makoFreqTest_A[channel] += makoFreqTest_Stp[channel];

    if (pi2 < makoFreqTest_A[channel])
    {
        //R1.00 Store our latest max value.
        if (channel == 0)
        {
            makoFreqGfxAmpl[makoFreqTest_F[channel]] = 20 * log10(makoFreqTestMax);
            makoFreqTestMax = 0.0;
            makoFreqGfxStop = makoFreqTest_F[channel] + 1;
        }

        //R1.00 We have cycled one whole sine wave, so start the next frequency.
        makoFreqTest_F[channel] += 1;        

        //R1.00 If we have swept 20000 kHz, end the test. 
        //R1.00 If we end on channel 0, channel 1 may not be completed.
        if (20001 < makoFreqTest_F[channel])
        {
            makoFreqTest_F[0] = 0;
            makoFreqTest_F[1] = 0;
            makoFreqTest = 2;         //R1.00 Turn off the freq test.
            makoFreqTestStart = 0;
        }

        //R1.00 Restart the sine wave at 0 radians and calc new radians/sample rate. 
        makoFreqTest_A[channel] = 0.0f;
        makoFreqTest_Stp[channel] = pi2 / ((1.0f / makoFreqTest_F[channel]) * makoSampleRate);
    }

    //R1.00 Return the current sine wave sample. Scale between 0 to .1
    return sinf(makoFreqTest_A[channel]) * .05f;
}

float MakoDist01AudioProcessor::Mako_TuneA(int channel)
{
    //R1.00 Update our current sine wave.
    makoTuneA1[channel] += makoTuneStp1[channel];  

    //R1.00 We need to loop 0 - 6.283 (pi2). If Sine Wave does not loop the freq will wander from lack of precision.
    if (pi2 < makoTuneA1[channel]) makoTuneA1[channel] = makoTuneA1[channel] - pi2;

    return (sinf(makoTuneA1[channel]) + sinf(makoTuneA1[channel] * makoTuneH1[makoTuneNote])) * .05f;
}

//R1.00 Some settings need to be calculated here instead of editor. 
void MakoDist01AudioProcessor::Mako_SettingsUpdate()
{
    //R1.00 Treble / Bass cutoff frequencies.
    if (makoTrebleCut != makoTrebleCut_Last)
    {
        makoTrebleCut_Last = makoTrebleCut;
        F3_Final_Treble_coeffs(makoTrebleCut);
    }
    if (makoBassCut != makoBassCut_Last)
    {
        makoBassCut_Last = makoBassCut;
        F4_Final_Bass_coeffs(makoBassCut);
    }

    //R1.00 See if we need to update our High, Mid and Low Pass filters.
    //R1.00 Max Check is because we were getting weird giant numbers occasionally. May not need anymore. 
    if ((abs(makoTone) < (makoTone_Max + 1)) && (makoTone != makoTone_Last))
    {
        makoTone_Last = makoTone;
        F1_Input_High_coeffs(makoTone);
    }
    if ((abs(makoTone5) < (makoTone5_Max + 1)) && (makoTone5 != makoTone5_Last))
    {
        makoTone5_Last = makoTone5;
        F5_Input_Mid_coeffs(makoTone5_Q, makoTone5);
    }
    if ((abs(makoTone5_Q) < (makoTone5_Q_Max + 1)) && (makoTone5_Q != makoTone5_Q_Last))
    {
        makoTone5_Q_Last = makoTone5_Q;
        F5_Input_Mid_coeffs(makoTone5_Q, makoTone5);
    }
    if ((abs(makoTone2) < (makoTone2_Max + 1)) && (makoTone2 != makoTone2_Last))
    {
        makoTone2_Last = makoTone2;
        F2_PostGain_Low_coeffs(makoTone2);
    }
    if ((abs(makoTone6) < (makoTone6_Max + 1)) && (makoTone6 != makoTone6_Last))
    {
        makoTone6_Last = makoTone6;
        F6_Final_Mid_coeffs(makoTone6_Q, makoTone6);
        //F6_Final_Mid_coeffs_BW(makoTone6_Q * 100.0f, makoTone6);
    }
    if ((abs(makoTone6_Q) < (makoTone6_Q_Max + 1)) && (makoTone6_Q != makoTone6_Q_Last))
    {
        makoTone6_Q_Last = makoTone6_Q;
        F6_Final_Mid_coeffs(makoTone6_Q, makoTone6);
        //F6_Final_Mid_coeffs_BW(makoTone6_Q * 100.0f, makoTone6);
    }
    if ((abs(makoMixHP) < (makoMixHP_Max + 1)) && (makoMixHP != makoMixHP_Last))
    {
        makoMixHP_Last = makoMixHP;
        F7_Clean_High_coeffs(makoMixHP);
    }

    //R1.00 MODULATION SETTINGS
    //R1.00 CHORUS depth settings.
    if ((makoModDepth != makoModDepth_Last) && (makoModType == 1))
    {
        Modulate_B_Offset[0] = 800; Modulate_B_Offset[1] = 1200;           //R1.00 Spread out offsets for Stereo effect.
        Modulate_B_Offset_Min[0] = 280 + (makoModDepth * 600);             //R1.00 Chorus buffer start position 280 to 880.
        Modulate_B_Offset_Min[1] = 280 + (makoModDepth * 600);
        Modulate_B_Offset_Max[0] = 1860 - ((1.0f - makoModDepth) * 400);   //R1.00 Chorus buffer End position 1860 to 1460.
        Modulate_B_Offset_Max[1] = 1860 - ((1.0f - makoModDepth) * 400);
    }
    if ((makoMod2Depth != makoMod2Depth_Last) && (makoMod2Type == 1))
    {
        Modulate2_B_Offset[0] = 800; Modulate2_B_Offset[1] = 1200;
        Modulate2_B_Offset_Min[0] = 280 + (makoMod2Depth * 600);
        Modulate2_B_Offset_Min[1] = 280 + (makoMod2Depth * 600);
        Modulate2_B_Offset_Max[0] = 1860 - ((1.0f - makoMod2Depth) * 400);
        Modulate2_B_Offset_Max[1] = 1860 - ((1.0f - makoMod2Depth) * 400);
    }

    //R1.00 DELAY Settings.
    if ((makoDelayTime != makoDelayTime_Last) || (makoDelayOffset != makoDelayOffset_Last))
    {
        makoDelayTime_Last = makoDelayTime;
        makoDelayOffset_Last = makoDelayOffset;
        Delay_B_Ring1[0] = 2 * makoDelayTime * (.5f + ((1 - makoDelayOffset) * .5f)) * makoSampleRate;
        Delay_B_Ring1_Max[0] = 2 * makoDelayTime * (.5f + ((1 - makoDelayOffset) * .5f)) * makoSampleRate + 1;
        Delay_B_Ring1[1] = 2 * makoDelayTime * makoSampleRate;
        Delay_B_Ring1_Max[1] = (2 * makoDelayTime * makoSampleRate) + 1;
    }

    //R1.00 Impulse response Settings
    if ((makoIRL != makoIRL_Last) || (makoIRLMix != makoIRLMix_Last) || (makoIRLSize != makoIRLSize_Last))
    {
        makoIRL_Last = makoIRL;
        makoIRLMix_Last = makoIRLMix;
        makoIRLSize_Last = makoIRLSize;
        makoTempIRL = makoIRL;             //R1.00 Setup TEMP vars so settings are not changing while we calc the IRs.
        makoTempIRLMix = makoIRLMix;
        makoTempIRLSize = makoIRLSize;
        
        Mako_SetIR_Left();
    }
    if ((makoIRR != makoIRR_Last) || (makoIRRMix != makoIRRMix_Last) || (makoIRRSize != makoIRRSize_Last))
    {
        makoIRR_Last = makoIRR;
        makoIRRMix_Last = makoIRRMix;
        makoIRRSize_Last = makoIRRSize;
        makoTempIRR = makoIRR;
        makoTempIRRMix = makoIRRMix;
        makoTempIRRSize = makoIRRSize;

        Mako_SetIR_Right();
    }

    if (makoSettingsClearbuffers == 1)
    {
        //R1.00 Clear delay/Reverb buffers so we dont get loud bangs when updating. 
        //R1.00 Still happens anyway.
        for (int t = 0; t <= 192000; t++)
        {
            Delay_B[0][t] = 0.0f;
            Delay_B[1][t] = 0.0f;
            Reverb_B[0][t] = 0.0f;
            Reverb_B[1][t] = 0.0f;
        }

        makoSettingsClearbuffers = 0;
    }

    //R1.00 RESET our settings changed flag. 
    makoSettingsChanged -= 1;
    if (makoSettingsChanged < 0) makoSettingsChanged = 0;
}


//==============================================================================
bool MakoDist01AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MakoDist01AudioProcessor::createEditor()
{
    return new MakoDist01AudioProcessorEditor (*this);
}

//==============================================================================
void MakoDist01AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
    
}

void MakoDist01AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
        
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
    
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MakoDist01AudioProcessor();
}


//R1.00 Input - MUD filter.
void MakoDist01AudioProcessor::F1_Input_High_coeffs(float fc)
{
    //F1.00 Second order butterworth High Pass.
    float c = tanf(pi * fc / makoSampleRate);
    f1a_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f1a_coeffs.a1 = -2.0f * f1a_coeffs.a0;
    f1a_coeffs.a2 = f1a_coeffs.a0;
    f1a_coeffs.b1 = 2.0f * f1a_coeffs.a0 * ((c * c) - 1.0f);
    f1a_coeffs.b2 = f1a_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));

    //R1.00 Bite High Pass. Use new set of Coeffs for readability. 
    f1b_coeffs = f1a_coeffs;
}

//R1.00 Post Gain - FIZZ filter.
void MakoDist01AudioProcessor::F2_PostGain_Low_coeffs(float fc)
{
    float c = 1.0f / (tanf(pi * fc / makoSampleRate));
    f2_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f2_coeffs.a1 = 2.0f * f2_coeffs.a0;
    f2_coeffs.a2 = f2_coeffs.a0;
    f2_coeffs.b1 = 2.0f * f2_coeffs.a0 * (1.0f - (c * c));
    f2_coeffs.b2 = f2_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));
}

//R1.00 Output - Low CUT
void MakoDist01AudioProcessor::F12_Final_LowCut_coeffs(float fc)
{
    //F1.00 Second order butterworth High Pass.
    float c = tanf(pi * fc / makoSampleRate);
    f12_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f12_coeffs.a1 = -2.0f * f12_coeffs.a0;
    f12_coeffs.a2 = f12_coeffs.a0;
    f12_coeffs.b1 = 2.0f * f12_coeffs.a0 * ((c * c) - 1.0f);
    f12_coeffs.b2 = f12_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));
}


//R1.00 Final EQ Treble filter.
void MakoDist01AudioProcessor::F3_Final_Treble_coeffs(float fc)
{
    //R1.00 Second order Butterworth high Pass.
    float c = tanf(pi * fc / makoSampleRate);
    f3_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f3_coeffs.a1 = -2.0f * f3_coeffs.a0;
    f3_coeffs.a2 = f3_coeffs.a0;
    f3_coeffs.b1 = 2.0f * f3_coeffs.a0 * ((c * c) - 1.0f);
    f3_coeffs.b2 = f3_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));
}


//R1.00 Final EQ Bass filter.
void MakoDist01AudioProcessor::F4_Final_Bass_coeffs(float fc)
{
    //R1.00 Second order Butterworth Low Pass.
    float c = 1.0f / (tanf(pi * fc / makoSampleRate));
    f4_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f4_coeffs.a1 = 2.0f * f4_coeffs.a0;
    f4_coeffs.a2 = f4_coeffs.a0;
    f4_coeffs.b1 = 2.0f * f4_coeffs.a0 * (1.0f - (c * c));
    f4_coeffs.b2 = f4_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));
}

void MakoDist01AudioProcessor::F5_Input_Mid_coeffs(float Q, int fc)
{
    //R1.00 Second order parametric/peaking boost filter with constant-Q
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = LinearGain_6dB; //pow(10.0, gain_db / 20.0);

    float a = 1.0f + (V0 * K) / Q + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = 1.0f - (V0 * K) / Q + K2;
    float d = 1.0f - K / Q + K2;
    float dd = 1.0f / (1.0f + K / Q + K2);

    f5_coeffs.a0 = a * dd;
    f5_coeffs.a1 = b * dd;
    f5_coeffs.a2 = g * dd;
    f5_coeffs.b1 = b * dd;
    f5_coeffs.b2 = d * dd;
    f5_coeffs.c0 = 1.0f;
    f5_coeffs.d0 = 0.0f;
}

void MakoDist01AudioProcessor::F6_Final_Mid_coeffs(float Q, int fc)
{
    //R1.00 Second Order Bandpass.
    //F_SIZE w = 2.0 * pi * fc / fs;
    //F_SIZE b = 0.5 * ((1.0 - tan(w / (2.0 * Q))) / (1.0 + tan(w / (2.0 * Q))));
    //F_SIZE g = (0.5 + b) * cos(w);
    //m_coeffs.a0 = 0.5 - b;
    //m_coeffs.a1 = 0.0;
    //m_coeffs.a2 = -(0.5 - b);
    //m_coeffs.b1 = -2.0 * g;
    //m_coeffs.b2 = 2.0 * b;

    //R1.00 Butterworth Second Order
    //float c = 1.0f / (tanf(pi * fc * bw / makoSampleRate));
    //float d = 2.0f * cosf(2.0f * pi * fc / makoSampleRate);
    //f6_coeffs.a0 = 1.0f / (1.0f + c);
    //f6_coeffs.a1 = 0.0f;
    //f6_coeffs.a2 = -f6_coeffs.a0;
    //f6_coeffs.b1 = -f6_coeffs.a0 * (c * d);
    //f6_coeffs.b2 = f6_coeffs.a0 * (c - 1.0f);

    //R1.00 Second order parametric/peaking boost filter with constant-Q
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = LinearGain_6dB; //pow(10.0, gain_db / 20.0);

    float a = 1.0f + (V0 * K) / Q + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = 1.0f - (V0 * K) / Q + K2;
    float d = 1.0f - K / Q + K2;

    float dd = 1.0f / (1.0f + K / Q + K2);

    f6_coeffs.a0 = a * dd;
    f6_coeffs.a1 = b * dd;
    f6_coeffs.a2 = g * dd;
    f6_coeffs.b1 = b * dd;
    f6_coeffs.b2 = d * dd;
    f6_coeffs.c0 = 1.0f;
    f6_coeffs.d0 = 0.0f;
}

void MakoDist01AudioProcessor::F6_Final_Mid_coeffs_BW(float bw, int fc)
{
    //R1.00 Butterworth Second Order
    float c = 1.0f / (tanf(pi * fc * bw / makoSampleRate));
    float d = 2.0f * cosf(2.0f * pi * fc / makoSampleRate);

    f6_coeffs.a0 = 1.0f / (1.0f + c);
    f6_coeffs.a1 = 0.0f;
    f6_coeffs.a2 = -f6_coeffs.a0;
    f6_coeffs.b1 = -f6_coeffs.a0 * (c * d);
    f6_coeffs.b2 = f6_coeffs.a0 * (c - 1.0f);
}

//R1.00 Added High Pass Filter.
void MakoDist01AudioProcessor::F7_Clean_High_coeffs(float fc)
{
    //R1.00 Second order Butterworth High Pass.
    float c = tanf(pi * fc / makoSampleRate);
    f7_coeffs.a0 = 1.0f / (1.0f + sqrt2 * c + (c * c));
    f7_coeffs.a1 = -2.0f * f7_coeffs.a0;
    f7_coeffs.a2 = f7_coeffs.a0;
    f7_coeffs.b1 = 2.0f * f7_coeffs.a0 * ((c * c) - 1.0f);
    f7_coeffs.b2 = f7_coeffs.a0 * (1.0f - sqrt2 * c + (c * c));
}


void MakoDist01AudioProcessor::F10_Phaser_Init()
{
    f10a_coeffs.a1 = 1.0f;
    f10a_coeffs.a2 = 0.0f;
    f10a_coeffs.b2 = 0.0f;

    f10b_coeffs.a1 = 1.0f;
    f10b_coeffs.a2 = 0.0f;
    f10b_coeffs.b2 = 0.0f;

    f10c_coeffs.a1 = 1.0f;
    f10c_coeffs.a2 = 0.0f;
    f10c_coeffs.b2 = 0.0f;

    f10d_coeffs.a1 = 1.0f;
    f10d_coeffs.a2 = 0.0f;
    f10d_coeffs.b2 = 0.0f;

    f10e_coeffs.a1 = 1.0f;
    f10e_coeffs.a2 = 0.0f;
    f10e_coeffs.b2 = 0.0f;

    f10f_coeffs.a1 = 1.0f;
    f10f_coeffs.a2 = 0.0f;
    f10f_coeffs.b2 = 0.0f;
}

void MakoDist01AudioProcessor::F10_Phaser2_Init()
{
    fM2_10a_coeffs.a1 = 1.0f;
    fM2_10a_coeffs.a2 = 0.0f;
    fM2_10a_coeffs.b2 = 0.0f;

    fM2_10b_coeffs.a1 = 1.0f;
    fM2_10b_coeffs.a2 = 0.0f;
    fM2_10b_coeffs.b2 = 0.0f;

    fM2_10c_coeffs.a1 = 1.0f;
    fM2_10c_coeffs.a2 = 0.0f;
    fM2_10c_coeffs.b2 = 0.0f;

    fM2_10d_coeffs.a1 = 1.0f;
    fM2_10d_coeffs.a2 = 0.0f;
    fM2_10d_coeffs.b2 = 0.0f;    
}

//R1.00 FILTERS USED FOR PHASER 1.
void MakoDist01AudioProcessor::F10a_Phaser_coeffs(float fc)
{
    //R1.00 Will Pirkle Book
    //float alphaNumerator = tanf((pi * fc) / makoSampleRate) - 1.0;
    //float alphaDenominator = tanf((pi * fc) / makoSampleRate) + 1.0;
    //float alpha = alphaNumerator / alphaDenominator;
    //f10a_coeffs.a0 = alpha;
    //f10a_coeffs.a1 = 1.0;
    //f10a_coeffs.a2 = 0.0;
    //f10a_coeffs.b1 = alpha;
    //f10a_coeffs.b2 = 0.0;
    float fac = tanf((pi * fc) / makoSampleRate);
    f10a_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10a_coeffs.b1 = f10a_coeffs.a0;

}

void MakoDist01AudioProcessor::F10b_Phaser_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    f10b_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10b_coeffs.b1 = f10b_coeffs.a0;
}

void MakoDist01AudioProcessor::F10c_Phaser_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    f10c_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10c_coeffs.b1 = f10c_coeffs.a0;
}

void MakoDist01AudioProcessor::F10d_Phaser_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    f10d_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10d_coeffs.b1 = f10d_coeffs.a0;
}

/*
void MakoDist01AudioProcessor::F10e_Phaser_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    f10e_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10e_coeffs.b1 = f10e_m_coeffs.a0;
}

void MakoDist01AudioProcessor::F10f_Phaser_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    f10f_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    f10f_coeffs.b1 = f10f_m_coeffs.a0;
}
*/

//R1.00 FILTERS USED FOR PHASER 2.
void MakoDist01AudioProcessor::F10a_Phaser2_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    fM2_10a_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    fM2_10a_coeffs.b1 = fM2_10a_coeffs.a0;
}

void MakoDist01AudioProcessor::F10b_Phaser2_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    fM2_10b_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    fM2_10b_coeffs.b1 = fM2_10b_coeffs.a0;
}

void MakoDist01AudioProcessor::F10c_Phaser2_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    fM2_10c_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    fM2_10c_coeffs.b1 = fM2_10c_coeffs.a0;
}

void MakoDist01AudioProcessor::F10d_Phaser2_coeffs(float fc)
{
    float fac = tanf((pi * fc) / makoSampleRate);
    fM2_10d_coeffs.a0 = (fac - 1.0f) / (fac + 1.0f);
    fM2_10d_coeffs.b1 = fM2_10d_coeffs.a0;
}


void MakoDist01AudioProcessor::F11_AutoWah_coeffs(float Q, float fc)
{
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = LinearGain_12dB; 
    
    float dd = 1.0f / (1.0f + K / Q + K2);

    f11_coeffs.a0 = (1.0f + (V0 * K) / Q + K2) * dd;   //a
    f11_coeffs.a1 = (2.0f * (K2 - 1.0f)) * dd;         //b  
    f11_coeffs.a2 = (1.0f - (V0 * K) / Q + K2) * dd;   //g
    f11_coeffs.b1 = f11_coeffs.a1;                     //b
    f11_coeffs.b2 = (1.0f - K / Q + K2) * dd;          //d
    f11_coeffs.c0 = 1.0f;
    f11_coeffs.d0 = 0.0f;

}

void MakoDist01AudioProcessor::F11_AutoWah2_coeffs(float Q, float fc)
{
    float K = pi2 * (fc * .5f) / makoSampleRate;
    float K2 = K * K;
    float V0 = LinearGain_12dB; 

    float a = 1.0f + (V0 * K) / Q + K2;
    float b = 2.0f * (K2 - 1.0f);
    float g = 1.0f - (V0 * K) / Q + K2;
    float d = 1.0f - K / Q + K2;

    float dd = 1.0f / (1.0f + K / Q + K2);

    fM2_f11_coeffs.a0 = (1.0f + (V0 * K) / Q + K2) * dd;   //a
    fM2_f11_coeffs.a1 = (2.0f * (K2 - 1.0f)) * dd;         //b  
    fM2_f11_coeffs.a2 = (1.0f - (V0 * K) / Q + K2) * dd;   //g
    fM2_f11_coeffs.b1 = fM2_f11_coeffs.a1;                 //b
    fM2_f11_coeffs.b2 = (1.0f - K / Q + K2) * dd;          //d
    fM2_f11_coeffs.c0 = 1.0f;
    fM2_f11_coeffs.d0 = 0.0f;
}

float MakoDist01AudioProcessor::Mako_EQ_Input_Filters(float tSample, int channel)
{   
    float tS = tSample;
    float tS_Temp;

    // R1.00 PRE GAIN / INPUT FILTERS
    if ((makoChan == 0) || (makoChan == 2))
    {
        //R1.00 HIGH PASS FILTER - remove low freqs before gain to reduce flubbyness. 
        f1a_xn[channel] = tS;
        tS = f1a_coeffs.a0 * f1a_xn[channel] + f1a_coeffs.a1 * f1a_xnz1[channel] + f1a_coeffs.a2 * f1a_xnz2[channel] - f1a_coeffs.b1 * f1a_ynz1[channel] - f1a_coeffs.b2 * f1a_ynz2[channel];
        f1a_xnz2[channel] = f1a_xnz1[channel]; f1a_xnz1[channel] = f1a_xn[channel]; f1a_ynz2[channel] = f1a_ynz1[channel]; f1a_ynz1[channel] = tS;

        //MID BOOST FILTER - Gain is fixed at 12 dB in calcs. Q and Freq are adjustable.
        f5_xn[channel] = tS;
        tS_Temp = f5_coeffs.a0 * f5_xn[channel] + f5_coeffs.a1 * f5_xnz1[channel] + f5_coeffs.a2 * f5_xnz2[channel] - f5_coeffs.b1 * f5_ynz1[channel] - f5_coeffs.b2 * f5_ynz2[channel];
        f5_xnz2[channel] = f5_xnz1[channel]; f5_xnz1[channel] = f5_xn[channel]; f5_ynz2[channel] = f5_ynz1[channel]; f5_ynz1[channel] = tS_Temp;
        tS = f5_coeffs.d0 * tS + f5_coeffs.c0 * tS_Temp;

        //R1.00 Drop some volume here, MID will give a little boost. 
        tS *= .8f;
    }
    else
    {   //R1.00 Edge and Metal channels use less filtering.
        tS_Temp = tS;
        float FAC = .8f + (1.0f - ((makoTone - 50.f) * .001f)) * .2f;  //R1.00 Very rough guess at frequency. 
        tS = FAC * f1a_xnz1[channel] + FAC * (tS - f1a_xn[channel]);
        f1a_xn[channel] = tS_Temp;
        f1a_xnz1[channel] = tS;
         
        //R1.00 HIGH PASS FILTER - remove low freqs before gain to reduce flubbyness. 
        //f1a_xn[channel] = tS;
        //tS = f1a_coeffs.a0 * f1a_xn[channel] + f1a_coeffs.a1 * f1a_xnz1[channel] + f1a_coeffs.a2 * f1a_xnz2[channel] - f1a_coeffs.b1 * f1a_ynz1[channel] - f1a_coeffs.b2 * f1a_ynz2[channel];
        //f1a_xnz2[channel] = f1a_xnz1[channel]; f1a_xnz1[channel] = f1a_xn[channel]; f1a_ynz2[channel] = f1a_ynz1[channel]; f1a_ynz1[channel] = tS;

    }

    return tS;
}

//R1.00 FIZZ - Post gain stage 1 Low Pass filter. 
float MakoDist01AudioProcessor::Mako_EQ_Fizz_Filter(float tSample, int channel)
{
    float tS;

    if (makoTone2 < 5500.0f)
    {
        f2_xn[channel] = tSample;
        tS = f2_coeffs.a0 * f2_xn[channel] + f2_coeffs.a1 * f2_xnz1[channel] + f2_coeffs.a2 * f2_xnz2[channel] - f2_coeffs.b1 * f2_ynz1[channel] - f2_coeffs.b2 * f2_ynz2[channel];
        f2_xnz2[channel] = f2_xnz1[channel]; f2_xnz1[channel] = f2_xn[channel]; f2_ynz2[channel] = f2_ynz1[channel]; f2_ynz1[channel] = tS;
    }
    else
        tS = tSample;

    return tS;
}

//R1.00 Dry Path High Pass filter. 
float MakoDist01AudioProcessor::Mako_EQ_Dry_HighPass(float tSample, int channel)
{
    float tS;

    f7_xn[channel] = tSample;
    tS = f7_coeffs.a0 * f7_xn[channel] + f7_coeffs.a1 * f7_xnz1[channel] + f7_coeffs.a2 * f7_xnz2[channel] - f7_coeffs.b1 * f7_ynz1[channel] - f7_coeffs.b2 * f7_ynz2[channel];
    f7_xnz2[channel] = f7_xnz1[channel]; f7_xnz1[channel] = f7_xn[channel]; f7_ynz2[channel] = f7_ynz1[channel]; f7_ynz1[channel] = tS;

    return tS;
}

//R1.00 Clip signal to -1 and 1.
float MakoDist01AudioProcessor::Mako_Clip(float tSample)
{
    float tS = tSample;

    if (.9999f < tS) tS = .9999f;
    if (tS < -.9999f) tS = -.9999f;

    return tS;
}

float MakoDist01AudioProcessor::Mako_FX_Dist_Stage1(float tSample)
{   
    float tS;
    float tS2;
    float tSym; 
    
    //R1.00 Work with ABSOLUTE values to simplify math and checks.
    tS = abs(tSample);
    
    //R1.00 Calculate a rounded sample and mix it with normal sample. 
    tS2 = tanhf(tS);
    tS2 = (makoSlope * tS2) + ((1 - makoSlope) * tS);

    //R1.00 Mix between hard clipped and normal/rounded. 
    if (0.75f < tS) tS = 0.75f;
    tS = (makoBreak * tS2) + ((1 - makoBreak) * tS);

    //R1.00 Create an asymmetric sample. 
    if ((0.0f < makoAsym1) && (tSample < 0.0f))
    {
        tSym = 0.25f;                                      //R1.00 Calc new Clipping point for NEG samples. 
        if (tSym < tS) tS2 = tSym;                         //R1.00 Apply to NEG sample. 
        tS = (makoAsym1 * tS2) + ((1 - makoAsym1) * tS);   //R1.00 Blend new dist curve. 
    }

    //R1.00 Clip to 1.0f. 
    if (.9999f < tS) tS = .9999f;

    //R1.00 Undo ABSOLUTE and return value. 
    if (tSample < 0.0f) tS = -tS;
    
    return tS;    
}

float MakoDist01AudioProcessor::Mako_FX_Chan3_Dist_Dry(float tSample)
{   
    float tS = tSample;
    float tS2;

    //R1.00 Do amplitude work in ABS.
    tS = abs(tS) * (makoVol * 500.0f);
    
    //R1.00 Clip.
    if (.9999f < tS) tS = .9999f;

    //R1.00 Distortion.
    //tS = tS - ((tS * tS) * makoSlope * 0.5f);  //R1.00 Roundness
    //tS2 = tS;

    //R1.00 Calculate a rounded sample and mix it with normal sample. 
    tS2 = tanhf(tS);
    tS = (makoSlope * tS2) + ((1 - makoSlope) * tS);

    //R1.00 Mix between hard clipped and normal/rounded. 
    if (0.75f < tS) tS2 = 0.75f;                                        
    tS = (makoBreak * tS) + ((1.0f - makoBreak) * tS2);
    
    //R1.00 Undo ABSOLUTE and return value. 
    if (tSample < 0.0f) tS = -tS;

    return tS;
}


//R1.00 OVERDRIVE
float MakoDist01AudioProcessor::Mako_FX_OD(float tSample)
{
    //R1.00 Mako method, blend with linear value to avoid non linear at 0. 
    float V = abs(tSample) * 10.0f;
    if (.9999f < V) V = .9999f;

    if (tSample < 0)
        return -(pow(abs(tSample), 1 - makoOD * .4f) * V) + (tSample * (1.0f - V));
    else
        return (pow(abs(tSample), 1 - makoOD * .4f) * V) + (tSample * (1.0f - V));
}

//R1.00 OVERDRIVE HYPERBOLIC TANGENT
float MakoDist01AudioProcessor::Mako_FX_ODHT(float tSample)
{
    //R1.00 Hyperbolic Arc Tangent Wave Shaper. Returns -1 to 1.
    float v = tanhf(tSample * (1.0f + (makoPowerAmp * 5.0f)));

    //R1.00 try to reduce the volume gain made by the OD.
    if (makoEQPreCut == 1) v *= (1.0f - (makoPowerAmp * .75f));

    return v;
}

//R1.00 GAIN STAGE 1 COMPRESSOR
float MakoDist01AudioProcessor::Mako_FX_Compress_1(float tSample, int channel)
{
    if (makoparmComp_Gain < .001f) return tSample;

    float tS = abs(tSample);

    //R1.00 Get loudest sample. 
    if (makoComp_Max[channel] < tS)
        makoComp_Max[channel] = tS;         // (makoComp_Max[channel] + tS) * .5f;
    else
    {
        //makoComp_Max[channel] *= .9995f;    //R1.00 Slowly release compression. 
        makoComp_Max[channel] -= makoparmComp_Release;
        if (makoComp_Max[channel] < 0.0f) makoComp_Max[channel] = 0.0f;
    }

    //R1.00 Calc new gain. 
    makoComp_Gain[channel] = .5f / makoComp_Max[channel]; //makoparmComp_Gain / makoComp_Max[channel];

    //R1.00 Limit gain value so we dont bring up the noise floor. 
    if (makoComp_Gain_Max < makoComp_Gain[channel]) makoComp_Gain[channel] = makoComp_Gain_Max;

    //R1.00 Apply new gain.
    tS *= tSample * makoComp_Gain[channel]; 

    tS = (tSample * (1.0f - makoparmComp_Gain)) + (tS * makoparmComp_Gain);

    if (1.0f < abs(tS))
        makoClipStage = 3;

    return tS;
}

//R1.00 GAIN STAGE 2 COMPRESSOR
float MakoDist01AudioProcessor::Mako_FX_Compress_2(float tSample, int channel)
{
    if (makoparmComp2_Gain < .001f) return tSample;

    float tS = abs(tSample);

    //R1.00 Get loudest sample. 
    if (makoComp2_Max[channel] < tS)
        makoComp2_Max[channel] = tS;  //R1.00 Could do some averaging here to get an attack time.  (makoComp2_Max[channel] + tS) * .5f;
    else
    {
        //makoComp2_Max[channel] *= .9995f;    //R1.00 Slowly release compression. 
        makoComp2_Max[channel] -= makoparmComp_Release;
        if (makoComp2_Max[channel] < 0.0f) makoComp2_Max[channel] = 0.0f;
    }

    //R1.00 Calc new gain. 
    makoComp2_Gain[channel] = .5f / makoComp2_Max[channel]; 

    //R1.00 Limit gain value so we dont bring up the noise floor. 
    if (makoComp2_Gain_Max < makoComp2_Gain[channel]) makoComp2_Gain[channel] = makoComp2_Gain_Max;

    //R1.00 Apply new gain.
    tS *= tSample * makoComp2_Gain[channel];

    tS = (tSample * (1.0f - makoparmComp2_Gain)) + (tS * makoparmComp2_Gain);

    if (1.0f < abs(tS))
        makoClipStage = 3;

    return tS;
}

//R1.00 CLEAN path compressor.
float MakoDist01AudioProcessor::Mako_FX_Compress_Clean(float tSample, int channel)
{
    if (makoparmComp3_Gain < .0101f) return tSample;
        
    float tS = abs(tSample);
   
    //R1.00 Get loudest sample. 
    if (makoComp3_Max[channel] < tS)
        makoComp3_Max[channel] = (makoComp_Max[channel] + tS) * .5f;
    else
    {
        //makoComp3_Max[channel] *= .9995f;    //R1.00 Slowly release compression. 
        makoComp3_Max[channel] -= makoparmComp_Release;
        if (makoComp3_Max[channel] < 0.0f) makoComp3_Max[channel] = 0.0f;
    }

    //R1.00 Calc new gain. 
    makoComp3_Gain[channel] = .5f / makoComp3_Max[channel];

    //R1.00 Limit gain value so we dont bring up the noise floor. 
    if (makoComp3_Gain_Max < makoComp3_Gain[channel]) makoComp3_Gain[channel] = makoComp3_Gain_Max;

    //R1.00 Apply new gain.
    tS *= tSample * makoComp3_Gain[channel];
        
    tS = (tSample * (1.0f - makoparmComp3_Gain)) + (tS * makoparmComp3_Gain);

    if (1.0f < abs(tS))
        makoClipStage = 3;
    
    return tS;
}

//R1.00 FINAL EQ - 3 BAND
float MakoDist01AudioProcessor::Mako_EQ_Final(float tSample, int channel)
{
    float tS_Treble, tS_Bass, tS_Mid;
    float tS = tSample * .05f;


    //R1.00 LOW CUT and removal of DC offset from Asymmetry code. 
    f12_xn[channel] = tS;
    tS = f12_coeffs.a0 * f12_xn[channel] + f12_coeffs.a1 * f12_xnz1[channel] + f12_coeffs.a2 * f12_xnz2[channel] - f12_coeffs.b1 * f12_ynz1[channel] - f12_coeffs.b2 * f12_ynz2[channel];
    f12_xnz2[channel] = f12_xnz1[channel]; f12_xnz1[channel] = f12_xn[channel]; f12_ynz2[channel] = f12_ynz1[channel]; f12_ynz1[channel] = tS;

    
    //R1.00 TREBLE
    f3_xn[channel] = tS;
    tS_Treble = f3_coeffs.a0 * f3_xn[channel] + f3_coeffs.a1 * f3_xnz1[channel] + f3_coeffs.a2 * f3_xnz2[channel] - f3_coeffs.b1 * f3_ynz1[channel] - f3_coeffs.b2 * f3_ynz2[channel];
    f3_xnz2[channel] = f3_xnz1[channel]; f3_xnz1[channel] = f3_xn[channel]; f3_ynz2[channel] = f3_ynz1[channel]; f3_ynz1[channel] = tS_Treble;

    //R1.00 BASS
    f4_xn[channel] = tS;
    tS_Bass = f4_coeffs.a0 * f4_xn[channel] + f4_coeffs.a1 * f4_xnz1[channel] + f4_coeffs.a2 * f4_xnz2[channel] - f4_coeffs.b1 * f4_ynz1[channel] - f4_coeffs.b2 * f4_ynz2[channel];
    f4_xnz2[channel] = f4_xnz1[channel]; f4_xnz1[channel] = f4_xn[channel]; f4_ynz2[channel] = f4_ynz1[channel]; f4_ynz1[channel] = tS_Bass;

    //MID - Gain is fixed at 12 dB in calcs. Q and Freq are adjustable.
    f6_xn[channel] = tS;
    tS_Mid = f6_coeffs.a0 * f6_xn[channel] + f6_coeffs.a1 * f6_xnz1[channel] + f6_coeffs.a2 * f6_xnz2[channel] - f6_coeffs.b1 * f6_ynz1[channel] - f6_coeffs.b2 * f6_ynz2[channel];
    f6_xnz2[channel] = f6_xnz1[channel]; f6_xnz1[channel] = f6_xn[channel]; f6_ynz2[channel] = f6_ynz1[channel]; f6_ynz1[channel] = tS_Mid;
    tS_Mid = f6_coeffs.d0 * tS + f6_coeffs.c0 * tS_Mid;

    //f6_xn[channel] = tS;
    //tS_Mid = f6_coeffs.a0 * f6_xn[channel] + f6_coeffs.a1 * f6_xnz1[channel] + f6_coeffs.a2 * f6_xnz2[channel] - f6_coeffs.b1 * f6_ynz1[channel] - f6_coeffs.b2 * f6_ynz2[channel];
    //f6_xnz2[channel] = f6_xnz1[channel]; f6_xnz1[channel] = f6_xn[channel]; f6_ynz2[channel] = f6_ynz1[channel]; f6_ynz1[channel] = tS_Mid;

    //R1.00 Normal calcs for MID are out of phase. 
    if (makoInvMid == 0) tS_Mid = -tS_Mid;         //R1.00 Flip phase by default. Use normal phase for jangley cleans.
            
    return (tS_Treble * makoTreble * 5.0f) + (tS_Bass * makoBass * 5.0f) + (tS_Mid * makoMid);
}

//R1.00 1024 sample IMPULSE RESPONSE calcs.
float MakoDist01AudioProcessor::Mako_CabSim(float tSample, int channel)
{
    int T1;
    float V;
    
    //R1.00 NO CAB selected. 
    if ((makoIRL == 0) && (channel == 0)) return tSample;
    if ((makoIRR == 0) && (channel == 1)) return tSample;

    T1 = IRB_Ring1[channel];
    IRB[channel][T1] = tSample;

    //Loop the Buffer + IR
    V = IRB[channel][T1];
    
    //R1.00 Apply cabs based on channel 1-Right.
    if (channel == 1)
    {
        //R1.00 Do RIGHT channel. 
        for (int t = 0; t < 1024; t++)
        {
            V += (IRB[channel][T1] * TIRR[t]);

            //R1.00 Increment index, mask off bits past 1023 to keep our index between 0-1023.
            T1 = (T1 + 1) & 0x3FF;             
        }
    }
    else
    {
        //R1.00 Apply Left cab (0) to ALL OTHER channels. Should only ever be 0.
        for (int t = 0; t < 1024; t++)
        {
            V += (IRB[channel][T1] * TIRL[t]);

            //R1.00 Increment index, mask off bits past 1023 to keep our index between 0-1023.
            T1 = (T1 + 1) & 0x3FF;            
        }
    }

    //R1.00 Decrement our ring buffer index and loop around at 0. 
    IRB_Ring1[channel]--;
    if (IRB_Ring1[channel] < 0) IRB_Ring1[channel] = 1023;
    
    //R1.00 We gain volume here so reduce it to some random level. Need good math here.
    return V * .25f;  
}

//R1.00 DIGITAL DELAY.
float MakoDist01AudioProcessor::Mako_FX_Delay(float tSample, int channel)
{
    //R1.00 Exit if not even using Delay.
    if (makoDelayMix < .001f) return tSample;

    long idx = Delay_B_Ring1[channel];
    
    if (makoModLoc == 4) Delay_B[channel][idx] = Mako_Modulate(Delay_B[channel][idx], channel);
    if (makoMod2Loc == 4) Delay_B[channel][idx] = Mako_Modulate2(Delay_B[channel][idx], channel);
    
    //R1.00 Create Final Mix sample.
    float V = (tSample * makoDelayDry) + (Delay_B[channel][idx] * makoDelayWet) * makoDelayBalLR[channel];
        
    //R1.00 Store new echo.
    if (makoDelayTime < .1f)
        Delay_B[channel][idx] = tSample * makoDelayDry;
    else
        Delay_B[channel][idx] = V * makoDelayRepeat; 
    
    //R1.00 Update echo ring buffer position. 
    Delay_B_Ring1[channel]++;
    if (Delay_B_Ring1_Max[channel] < Delay_B_Ring1[channel]) Delay_B_Ring1[channel] = 0;

    return V;
}

float MakoDist01AudioProcessor::Mako_FX_Width(float tSample)
{
    //R1.00 Exit if not even using Delay.
    if (makoFinalWidth < .001f) return tSample;

    //R1.00 Create Final Mix sample.
    float V = FinalWidth_B[FinalWidth_B_Ring];

    //R1.00 Store new echo. 
    FinalWidth_B[FinalWidth_B_Ring] = tSample;
    
    //R1.00 Update echo ring buffer position. 
    FinalWidth_B_Ring++;
    //if (FinalWidth_B_Ring_Max < FinalWidth_B_Ring) FinalWidth_B_Ring = 0;
    if ((makoFinalWidth * 4000) < FinalWidth_B_Ring) FinalWidth_B_Ring = 0;

    return V;    
}

//R1.00 MODULATION 1 EFFECTS.
float MakoDist01AudioProcessor::Mako_Modulate(float tSample, int channel)
{
    if (makoModType == 0) return tSample;

    float V = tSample;

    switch (makoModType)
    {
      case 1: 
        V = Mako_FX_Chorus(tSample, channel); break;
      case 2:
        V = Mako_FX_Tremolo(tSample, channel); break;
      case 3:
        V = Mako_FX_ModWah(tSample, channel); break;  
      case 4:
        V = Mako_FX_Phaser(tSample, channel); break;
      case 5:
        V = Mako_FX_AutoWah(tSample, channel); break;
    }

    return V;
}

//R1.00 MODULATION 2 EFFECTS.
float MakoDist01AudioProcessor::Mako_Modulate2(float tSample, int channel)
{
    if (makoMod2Type == 0) return tSample;

    float V = tSample;

    switch (makoMod2Type)
    {
      case 1:
        V = Mako_FX_Chorus2(tSample, channel); break;
      case 2:
        V = Mako_FX_Tremolo2(tSample, channel); break;
      case 3:
        V = Mako_FX_ModWah2(tSample, channel); break;
      case 4:
        V = Mako_FX_Phaser2(tSample, channel); break;
      case 5:
        V = Mako_FX_AutoWah2(tSample, channel); break;
    }

    return V;
}

float MakoDist01AudioProcessor::Mako_FX_Chorus(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .001) return tSample;

    //R1.00 Store new sample in Ring buffer. 
    Modulate_B[channel][Modulate_B_Ring1[channel]] = tSample;

    //R1.00 Calc rotating index.
    int idx = Modulate_B_Ring1[channel] - Modulate_B_Offset[channel];
    if (idx < 0) idx = idx + Modulate_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    Modulate_B_Offset[channel] *= (1.0f + (Modulate_B_Rate * makoModRate)); 
    
    //R1.00 Alter our delay direction if needed.
    if (Modulate_B_Offset[channel] < Modulate_B_Offset_Min[channel]) Modulate_B_Rate = Modulate_B_Rate_Up;
    if (Modulate_B_Offset_Max[channel] < Modulate_B_Offset[channel]) Modulate_B_Rate = Modulate_B_Rate_Down;
    
    //R1.00 Update ring buffer position. 
    Modulate_B_Ring1[channel]++;
    if (Modulate_B_Ring1_Max[channel] < Modulate_B_Ring1[channel]) Modulate_B_Ring1[channel] = 0;

    return (tSample * (1 - makoModMix)) + (Modulate_B[channel][idx] * makoModMix);
}

float MakoDist01AudioProcessor::Mako_FX_Chorus2(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoMod2Mix < .001) return tSample;

    //R1.00 Store new sample in Ring buffer. 
    Modulate2_B[channel][Modulate2_B_Ring1[channel]] = tSample;

    //R1.00 Calc rotating index.
    int idx = Modulate2_B_Ring1[channel] - Modulate2_B_Offset[channel];
    if (idx < 0) idx = idx + Modulate2_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    Modulate2_B_Offset[channel] *= (1.0f + (Modulate2_B_Rate * makoMod2Rate)); 

    //R1.00 Alter our delay direction if needed.
    if (Modulate2_B_Offset[channel] < Modulate2_B_Offset_Min[channel]) Modulate2_B_Rate = Modulate2_B_Rate_Up;
    if (Modulate2_B_Offset_Max[channel] < Modulate2_B_Offset[channel]) Modulate2_B_Rate = Modulate2_B_Rate_Down;

    //R1.00 Update ring buffer position. 
    Modulate2_B_Ring1[channel]++;
    if (Modulate2_B_Ring1_Max[channel] < Modulate2_B_Ring1[channel]) Modulate2_B_Ring1[channel] = 0;

    return (tSample * (1 - makoMod2Mix)) + (Modulate2_B[channel][idx] * makoMod2Mix);
}


float MakoDist01AudioProcessor::Mako_FX_Tremolo(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .01) return tSample;

    float TempS;

    if (channel == 0)
    {
        Modulate_Wave[0] += Modulate_Wave_Ramp[0];

        if (Modulate_Wave[0] < -.5f)
        {
            Modulate_Wave[0] = -.5f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * 20.0f;   //R1.00 10 Hz max rate.
        }
        if (.5f < Modulate_Wave[0])
        {
            Modulate_Wave[0] = .5f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * -20.0f;  //R1.00 10 Hz max rate.
        }
    }
    
    TempS = Modulate_Wave[0] * makoModDepth * 10.0f;
    if (.5f < TempS) TempS = .5f;
    if (TempS < -.5f) TempS = -.5f;

    TempS = (tSample * (1 - makoModMix)) + (tSample * (TempS + .5f) * makoModMix);

    return TempS;
}

float MakoDist01AudioProcessor::Mako_FX_Tremolo2(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoMod2Mix < .01) return tSample;

    float TempS;

    if (channel == 0)
    {
        Modulate2_Wave[0] += Modulate2_Wave_Ramp[0];

        if (Modulate2_Wave[0] < -.5f)
        {
            Modulate2_Wave[0] = -.5f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * makoMod2Rate * 20.0f;   //R1.00 10 Hz max rate.
        }
        if (.5f < Modulate2_Wave[0])
        {
            Modulate2_Wave[0] = .5f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * makoMod2Rate * -20.0f;  //R1.00 10 Hz max rate.
        }
    }

    TempS = Modulate2_Wave[0] * makoMod2Depth * 10.0f;
    if (.5f < TempS) TempS = .5f;
    if (TempS < -.5f) TempS = -.5f;

    return (tSample * (1 - makoMod2Mix)) + (tSample * (TempS + .5f) * makoMod2Mix);
}

float MakoDist01AudioProcessor::Mako_FX_Phaser(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .01) return tSample;

    float TempSa;
   
    if (channel == 0)
    {
        
        Modulate_Wave[0] += Modulate_Wave_Ramp[0];
        if (Modulate_Wave[0] < .01f)
        {
            Modulate_Wave[0] = .01f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * (makoModRate + .01f) * 5.0f;   //R1.00 10 Hz max rate.
        }
        if (.99f < Modulate_Wave[0])
        {
            Modulate_Wave[0] = .99f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * (makoModRate + .01f) * -5.0f;   //R1.00 10 Hz max rate.
        }
        
        //R1.00 Sin Wave mode, does not sound as good. 
        //Modulate_Wave[0] += Modulate_B_Rate_Up * makoModRate * 20.0f;  //Modulate_Wave_Ramp[0];
        //if (6.283185f < Modulate_Wave[0]) Modulate_Wave[0] -= 6.283185f;
        //TempSa = sinf(Modulate_Wave[0] * .5f) + .5f;

        //R1.00 All Pass Filter 1
        /*
        //R1.00 Will Pirkle values from book.
        //R1.00 Has a heavy low freq pulsing that sounds bad. Prob good if done correctly from other calcs, not being used here.
        F10a_Phaser_coeffs(16.0f + Modulate_Wave[0] * 1600.0f);
        F10b_Phaser_coeffs(33.0f + Modulate_Wave[0] * 3300.0f);
        F10c_Phaser_coeffs(48.0f + Modulate_Wave[0] * 4800.0f);
        F10d_Phaser_coeffs(98.0f + Modulate_Wave[0] * 9800.0f);
        F10e_Phaser_coeffs(160.0f + Modulate_Wave[0] * 16000.0f);
        F10f_Phaser_coeffs(260.0f + Modulate_Wave[0] * 20480.0f);
        */
        
        //R1.00 National Semicondutor values from Will Pirkle code base.
        F10a_Phaser_coeffs(32.0f + Modulate_Wave[0] * 1500.0f);
        F10b_Phaser_coeffs(68.0f + Modulate_Wave[0] * 3400.0f);
        F10c_Phaser_coeffs(96.0f + Modulate_Wave[0] * 4800.0f);
        F10d_Phaser_coeffs(212.0f + Modulate_Wave[0] * 10000.0f);
        //F10e_Phaser_coeffs(320.0f + Modulate_Wave[0] * 16000.0f);
        //F10f_Phaser_coeffs(636.0f + Modulate_Wave[0] * 20480.0f);
        
    }
    
    //R1.00 Calculate the ALL PASS FILTERs.
    f10a_xn[channel] = (Mod_Phase_Last_Sample[channel] * (1 - makoModDepth)) + (tSample *  makoModDepth);
    TempSa = f10a_coeffs.a0 * f10a_xn[channel] + f10a_coeffs.a1 * f10a_xnz1[channel] + f10a_coeffs.a2 * f10a_xnz2[channel] - f10a_coeffs.b1 * f10a_ynz1[channel] - f10a_coeffs.b2 * f10a_ynz2[channel];
    f10a_xnz2[channel] = f10a_xnz1[channel]; f10a_xnz1[channel] = f10a_xn[channel]; f10a_ynz2[channel] = f10a_ynz1[channel]; f10a_ynz1[channel] = TempSa;
    
    f10b_xn[channel] = TempSa;
    TempSa = f10b_coeffs.a0 * f10b_xn[channel] + f10b_coeffs.a1 * f10b_xnz1[channel] + f10b_coeffs.a2 * f10b_xnz2[channel] - f10b_coeffs.b1 * f10b_ynz1[channel] - f10b_coeffs.b2 * f10b_ynz2[channel];
    f10b_xnz2[channel] = f10b_xnz1[channel]; f10b_xnz1[channel] = f10b_xn[channel]; f10b_ynz2[channel] = f10b_ynz1[channel]; f10b_ynz1[channel] = TempSa;

    f10c_xn[channel] = TempSa;
    TempSa = f10c_coeffs.a0 * f10c_xn[channel] + f10c_coeffs.a1 * f10c_xnz1[channel] + f10c_coeffs.a2 * f10c_xnz2[channel] - f10c_coeffs.b1 * f10c_ynz1[channel] - f10c_coeffs.b2 * f10c_ynz2[channel];
    f10c_xnz2[channel] = f10c_xnz1[channel]; f10c_xnz1[channel] = f10c_xn[channel]; f10c_ynz2[channel] = f10c_ynz1[channel]; f10c_ynz1[channel] = TempSa;

    f10d_xn[channel] = TempSa;
    TempSa = f10d_coeffs.a0 * f10d_xn[channel] + f10d_coeffs.a1 * f10d_xnz1[channel] + f10d_coeffs.a2 * f10d_xnz2[channel] - f10d_coeffs.b1 * f10d_ynz1[channel] - f10d_coeffs.b2 * f10d_ynz2[channel];
    f10d_xnz2[channel] = f10d_xnz1[channel]; f10d_xnz1[channel] = f10d_xn[channel]; f10d_ynz2[channel] = f10d_ynz1[channel]; f10d_ynz1[channel] = TempSa;

    //f10e_xn[channel] = TempSd;
    //TempSe = f10e_coeffs.a0 * f10e_xn[channel] + f10e_coeffs.a1 * f10e_m_xnz1[channel] + f10e_coeffs.a2 * f10e_m_xnz2[channel] - f10e_coeffs.b1 * f10e_m_ynz1[channel] - f10e_coeffs.b2 * f10e_m_ynz2[channel];
    //f10e_m_xnz2[channel] = f10e_m_xnz1[channel]; f10e_m_xnz1[channel] = f10e_xn[channel]; f10e_m_ynz2[channel] = f10e_m_ynz1[channel]; f10e_m_ynz1[channel] = TempSe;

    //f10f_xn[channel] = TempSe;
    //TempSf = f10f_coeffs.a0 * f10f_xn[channel] + f10f_coeffs.a1 * f10f_m_xnz1[channel] + f10f_coeffs.a2 * f10f_m_xnz2[channel] - f10f_coeffs.b1 * f10f_m_ynz1[channel] - f10f_coeffs.b2 * f10f_m_ynz2[channel];
    //f10f_m_xnz2[channel] = f10f_m_xnz1[channel]; f10f_m_xnz1[channel] = f10f_xn[channel]; f10f_m_ynz2[channel] = f10f_m_ynz1[channel]; f10f_m_ynz1[channel] = TempSf;

    Mod_Phase_Last_Sample[channel] = TempSa;

    //TempSa = (TempSa + TempSb + TempSc + TempSd) * .25f;

    return (tSample * (1 - makoModMix)) + (TempSa * makoModMix);
}


float MakoDist01AudioProcessor::Mako_FX_Phaser2(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoMod2Mix < .01) return tSample;

    float TempSa;
    
    if (channel == 0)
    {

        Modulate2_Wave[0] += Modulate2_Wave_Ramp[0];
        if (Modulate2_Wave[0] < .01f)
        {
            Modulate2_Wave[0] = .01f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * (makoMod2Rate + .01f) * 5.0f;   //R1.00 10 Hz max rate.
        }
        if (.99f < Modulate2_Wave[0])
        {
            Modulate2_Wave[0] = .99f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * (makoMod2Rate + .01f) * -5.0f;   //R1.00 10 Hz max rate.
        }

        //R1.00 National Semicondutor values from Will Pirkle code base.
        F10a_Phaser2_coeffs(32.0f + Modulate2_Wave[0] * 1500.0f);
        F10b_Phaser2_coeffs(68.0f + Modulate2_Wave[0] * 3400.0f);
        F10c_Phaser2_coeffs(96.0f + Modulate2_Wave[0] * 4800.0f);
        F10d_Phaser2_coeffs(212.0f + Modulate2_Wave[0] * 10000.0f);
    
    }

    //R1.00 Calculate our ALL PASS FILTERs.
    fM2_10a_xn[channel] = (Mod2_Phase_Last_Sample[channel] * (1.0f - makoMod2Depth)) + (tSample * (makoMod2Depth));
    TempSa = fM2_10a_coeffs.a0 * fM2_10a_xn[channel] + fM2_10a_coeffs.a1 * fM2_10a_xnz1[channel] + fM2_10a_coeffs.a2 * fM2_10a_xnz2[channel] - fM2_10a_coeffs.b1 * fM2_10a_ynz1[channel] - fM2_10a_coeffs.b2 * fM2_10a_ynz2[channel];
    fM2_10a_xnz2[channel] = fM2_10a_xnz1[channel]; fM2_10a_xnz1[channel] = fM2_10a_xn[channel]; fM2_10a_ynz2[channel] = fM2_10a_ynz1[channel]; fM2_10a_ynz1[channel] = TempSa;

    fM2_10b_xn[channel] = TempSa;
    TempSa = fM2_10b_coeffs.a0 * fM2_10b_xn[channel] + fM2_10b_coeffs.a1 * fM2_10b_xnz1[channel] + fM2_10b_coeffs.a2 * fM2_10b_xnz2[channel] - fM2_10b_coeffs.b1 * fM2_10b_ynz1[channel] - fM2_10b_coeffs.b2 * fM2_10b_ynz2[channel];
    fM2_10b_xnz2[channel] = fM2_10b_xnz1[channel]; fM2_10b_xnz1[channel] = fM2_10b_xn[channel]; fM2_10b_ynz2[channel] = fM2_10b_ynz1[channel]; fM2_10b_ynz1[channel] = TempSa;

    fM2_10c_xn[channel] = TempSa;
    TempSa = fM2_10c_coeffs.a0 * fM2_10c_xn[channel] + fM2_10c_coeffs.a1 * fM2_10c_xnz1[channel] + fM2_10c_coeffs.a2 * fM2_10c_xnz2[channel] - fM2_10c_coeffs.b1 * fM2_10c_ynz1[channel] - fM2_10c_coeffs.b2 * fM2_10c_ynz2[channel];
    fM2_10c_xnz2[channel] = fM2_10c_xnz1[channel]; fM2_10c_xnz1[channel] = fM2_10c_xn[channel]; fM2_10c_ynz2[channel] = fM2_10c_ynz1[channel]; fM2_10c_ynz1[channel] = TempSa;

    fM2_10d_xn[channel] = TempSa;
    TempSa = fM2_10d_coeffs.a0 * fM2_10d_xn[channel] + fM2_10d_coeffs.a1 * fM2_10d_xnz1[channel] + fM2_10d_coeffs.a2 * fM2_10d_xnz2[channel] - fM2_10d_coeffs.b1 * fM2_10d_ynz1[channel] - fM2_10d_coeffs.b2 * fM2_10d_ynz2[channel];
    fM2_10d_xnz2[channel] = fM2_10d_xnz1[channel]; fM2_10d_xnz1[channel] = fM2_10d_xn[channel]; fM2_10d_ynz2[channel] = fM2_10d_ynz1[channel]; fM2_10d_ynz1[channel] = TempSa;

    Mod2_Phase_Last_Sample[channel] = TempSa;

    return (tSample * (1 - makoMod2Mix)) + (TempSa * makoMod2Mix);
}


float MakoDist01AudioProcessor::Mako_FX_AutoWah(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .01) return tSample;

    float Temp_S = abs(tSample);
    float Temp_Q, Temp_F;
    
    //R1.00 Track our peak signal. 
    if (Mod_Peak[0] < Temp_S)
    {
        Mod_Peak[0] = Temp_S;
        if (1.0f < Mod_Peak[0]) Mod_Peak[0] = 1.0f;   //R1.00 Clip here so we do not do broken math below. 
    }
    else
    {
        Mod_Peak[0] -= Release_300mS;
        if (Mod_Peak[0] < 0.0f) Mod_Peak[0] = 0.0f;
    }

    //R1.00 Mono filter action for now. 
    if (channel == 0)
    {
        //R1.00 Try not to do calcs so often.
        if (.0005f < abs(Mod_Peak[0] - Mod_Peak_Last[0]))
        {
            Mod_Peak_Last[0] = Mod_Peak[0];

            Temp_Q = .3f + (makoModRate * 10.0f) * (1.0f - Mod_Peak[0]);
            Temp_F = 200.0f + Mod_Peak[0] * (1000.0f + makoModDepth * 4500.0f);
            F11_AutoWah_coeffs(Temp_Q, Temp_F);
        }
    }

    f11_xn[channel] = tSample;
    Temp_S = f11_coeffs.a0 * f11_xn[channel] + f11_coeffs.a1 * f11_xnz1[channel] + f11_coeffs.a2 * f11_xnz2[channel] - f11_coeffs.b1 * f11_ynz1[channel] - f11_coeffs.b2 * f11_ynz2[channel];
    f11_xnz2[channel] = f11_xnz1[channel]; f11_xnz1[channel] = f11_xn[channel]; f11_ynz2[channel] = f11_ynz1[channel]; f11_ynz1[channel] = Temp_S;

    return (tSample * (1 - makoModMix)) + (Temp_S * makoModMix * LinearGain_12dB_Inv);

}

float MakoDist01AudioProcessor::Mako_FX_AutoWah2(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoMod2Mix < .01) return tSample;

    float Temp_S = abs(tSample);
    float Temp_Q, Temp_F;

    //R1.00 Track our peak signal. 
    if (Mod2_Peak[0] < Temp_S)
    {
        Mod2_Peak[0] = Temp_S;
        if (1.0f < Mod2_Peak[0]) Mod2_Peak[0] = 1.0f;
    }
    else
    {
        Mod2_Peak[0] -= Release_300mS;
        if (Mod2_Peak[0] < 0.0f) Mod2_Peak[0] = 0.0f;
    }

    //R1.00 Mono filter action for now. 
    if (channel == 0)
    {
        //R1.00 Try not to do calcs so often.
        if (.0005f < abs(Mod2_Peak[0] - Mod2_Peak_Last[0]))
        {
            Mod2_Peak_Last[0] = Mod2_Peak[0];

            Temp_Q = .3f + (makoMod2Rate * 10.0f) * (1.0f - Mod2_Peak[0]);
            Temp_F = 200.0f + Mod2_Peak[0] * (1000.0f + makoMod2Depth * 4500.0f);
            F11_AutoWah2_coeffs(Temp_Q, Temp_F);
        }
    }

    fM2_11_xn[channel] = tSample;
    Temp_S = fM2_f11_coeffs.a0 * fM2_11_xn[channel] + fM2_f11_coeffs.a1 * fM2_11_xnz1[channel] + fM2_f11_coeffs.a2 * fM2_11_xnz2[channel] - fM2_f11_coeffs.b1 * fM2_11_ynz1[channel] - fM2_f11_coeffs.b2 * fM2_11_ynz2[channel];
    fM2_11_xnz2[channel] = fM2_11_xnz1[channel]; fM2_11_xnz1[channel] = fM2_11_xn[channel]; fM2_11_ynz2[channel] = fM2_11_ynz1[channel]; fM2_11_ynz1[channel] = Temp_S;

    return (tSample * (1 - makoMod2Mix)) + (Temp_S *  makoMod2Mix * LinearGain_12dB_Inv);

}


float MakoDist01AudioProcessor::Mako_FX_ModWah(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .01) return tSample;

    float TempS;

    if (channel == 0)
    {
        Modulate_Wave[0] += Modulate_Wave_Ramp[0];

        if (Modulate_Wave[0] < .01f)
        {
            Modulate_Wave[0] = .01f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * 20.0f;   //R1.00 10 Hz max rate.
        }
        if (.99f < Modulate_Wave[0])
        {
            Modulate_Wave[0] = .99f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * -20.0f;   //R1.00 10 Hz max rate.
        }

        F11_AutoWah_coeffs(.707, 200.0f + Modulate_Wave[0] * 1600.0f);
    }


    f11_xn[channel] = tSample;
    TempS = f11_coeffs.a0 * f11_xn[channel] + f11_coeffs.a1 * f11_xnz1[channel] + f11_coeffs.a2 * f11_xnz2[channel] - f11_coeffs.b1 * f11_ynz1[channel] - f11_coeffs.b2 * f11_ynz2[channel];
    f11_xnz2[channel] = f11_xnz1[channel]; f11_xnz1[channel] = f11_xn[channel]; f11_ynz2[channel] = f11_ynz1[channel]; f11_ynz1[channel] = TempS;

    return (tSample * (1 - makoModMix)) + (TempS * makoModMix * LinearGain_12dB_Inv);

}

float MakoDist01AudioProcessor::Mako_FX_ModWah2(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoMod2Mix < .01) return tSample;

    float TempS;

    if (channel == 0)
    {
        Modulate2_Wave[0] += Modulate2_Wave_Ramp[0];

        if (Modulate2_Wave[0] < .01f)
        {
            Modulate2_Wave[0] = .01f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * makoMod2Rate * 20.0f;   //R1.00 10 Hz max rate.
        }
        if (.99f < Modulate2_Wave[0])
        {
            Modulate2_Wave[0] = .99f;
            Modulate2_Wave_Ramp[0] = Modulate2_B_Rate_Up * makoMod2Rate * -20.0f;   //R1.00 10 Hz max rate.
        }

        F11_AutoWah2_coeffs(.707, 200.0f + Modulate2_Wave[0] * 1600.0f);
    }


    fM2_11_xn[channel] = tSample;
    TempS = fM2_f11_coeffs.a0 * fM2_11_xn[channel] + fM2_f11_coeffs.a1 * fM2_11_xnz1[channel] + fM2_f11_coeffs.a2 * fM2_11_xnz2[channel] - fM2_f11_coeffs.b1 * fM2_11_ynz1[channel] - fM2_f11_coeffs.b2 * fM2_11_ynz2[channel];
    fM2_11_xnz2[channel] = fM2_11_xnz1[channel]; fM2_11_xnz1[channel] = fM2_11_xn[channel]; fM2_11_ynz2[channel] = fM2_11_ynz1[channel]; fM2_11_ynz1[channel] = TempS;

    return (tSample * (1 - makoMod2Mix)) + (TempS * makoMod2Mix * LinearGain_12dB_Inv);

}



float MakoDist01AudioProcessor::Mako_FX_ModFilter(float tSample, int channel)
{
    //R1.00 Exit if not even using Modulate.
    if (makoModMix < .01) return tSample;

    float TempS;

    if (channel == 0)
    {
        Modulate_Wave[0] += Modulate_Wave_Ramp[0];

        if (Modulate_Wave[0] < .01f)
        {
            Modulate_Wave[0] = .01f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * 20.0f;   //R1.00 10 Hz max rate.
        }
        if (.99f < Modulate_Wave[0])
        {
            Modulate_Wave[0] = .99f;
            Modulate_Wave_Ramp[0] = Modulate_B_Rate_Up * makoModRate * -20.0f;   //R1.00 10 Hz max rate.
        }

        TempS = (tSample * Modulate_Wave[0]) + (Modulate_B_Last[channel] * (1 - Modulate_Wave[0]));
    }
    else
        TempS = (tSample * (1 - Modulate_Wave[0])) + (Modulate_B_Last[channel] * Modulate_Wave[0]);

    Modulate_B_Last[channel] = TempS;

    return (tSample * (1 - makoModMix)) + (TempS * makoModMix * LinearGain_12dB_Inv);
        
}

//R1.00 Unused effect code for smoothing out reverb echoes. Chorus works much better.
float MakoDist01AudioProcessor::Mako_FX_Reverb_ModFilter(float tSample, int channel)
{    
    float TempS;

    if (channel == 0)
    {
        Reverb_Mod_Wave[0] += Reverb_Mod_Wave_Ramp[0];

        if (Reverb_Mod_Wave[0] < .01f)
        {
            Reverb_Mod_Wave[0] = .01f;
            Reverb_Mod_Wave_Ramp[0] = RevMod_B_Rate_Up * 5.0f;   //R1.00 (makoModRate * 20.0f)
        }
        if (.99f < Reverb_Mod_Wave[0])
        {
            Reverb_Mod_Wave[0] = .99f;
            Reverb_Mod_Wave_Ramp[0] = RevMod_B_Rate_Up * 5.0f;   //R1.00 10 Hz max rate.
        }

        TempS = (tSample * Reverb_Mod_Wave[0]) + (Reverb_Mod_B_Last[channel] * (1 - Reverb_Mod_Wave[0]));
    }
    else
        TempS = (tSample * (1 - Reverb_Mod_Wave[0])) + (Reverb_Mod_B_Last[channel] * Reverb_Mod_Wave[0]);

    Reverb_Mod_B_Last[channel] = TempS;

    return (tSample + TempS) * .5f; //R1.00 May need  * LinearGain_12dB_Inv to reduce filter gain.
}

float MakoDist01AudioProcessor::Mako_FX_Reverb(float tSample, int channel)
{
    //R1.00 Exit if not even using Reverb.
    if (makoReverbMix < .001f) return tSample;

    float V = 0.0f;

    //R1.00 Add echoes. 
    for (int t = 0; t < 16; t++)
    {
        //GOOD V += Reverb_B[channel][Reverb_B_Ring1[channel][t]] * Reverb_B_Ring1_Gain[t];
        V = (V * Reverb_B_Ring1_GainInv[t]) + (Reverb_B[channel][Reverb_B_Ring1[channel][t]] * Reverb_B_Ring1_Gain[t]);

        Reverb_B_Ring1[channel][t]++;
        if (Reverb_B_Ring1_Max[channel] < Reverb_B_Ring1[channel][t]) Reverb_B_Ring1[channel][t] = 0;
    }

    //R1.00 Apply MODULATIONs.
    if (makoModLoc == 5) V = Mako_Modulate(V, channel);    //R1.00 Normal adjustable Chorus
    if (makoMod2Loc == 5) V = Mako_Modulate2(V, channel);  //R1.00 Normal adjustable Chorus
    V = Mako_FX_ReverbChorus(V, channel);                     //R1.00 Reverb chorus. Fixed settings.
    //V = Mako_Reverb_ModFilter(V, channel);

    //R1.00 Clip reverb or it runs away.
    V = Mako_Clip(V);
    
    //R1.00 Store new echo. Use LOW PASS filter to remove glitches. Or not!
    Reverb_B[channel][Reverb_B_Ring1[channel][0]] = ((Reverb_B[channel][Reverb_B_Ring1[channel][0]] + Reverb_B_Last[channel]) * .5f) * makoReverbTime + ((tSample + V) * .5) * makoReverbTimeInv;
    Reverb_B_Last[channel] = Reverb_B[channel][Reverb_B_Ring1[channel][0]];

    return (tSample * makoReverbDry) + (V * (4.0f + makoReverbTime * 8.0f) * makoReverbWet) * makoReverbBalLR[channel]; //R1.00 We need to add some vol for long decay times.
}


float MakoDist01AudioProcessor::Mako_FX_ReverbChorus(float tSample, int channel)
{
    //R1.00 Store new sample in Ring buffer. 
    RevMod_B[channel][RevMod_B_Ring1[channel]] = tSample;

    //R1.00 Calc rotating index.
    int idx = RevMod_B_Ring1[channel] - RevMod_B_Offset[channel];
    if (idx < 0) idx += RevMod_B_Ring1_Max[channel];

    //R1.00 Update our rotating index.
    RevMod_B_Offset[channel] *= (1.0f + (RevMod_B_Rate * .15f));  // makoModRate = .15f

    //R1.00 Alter our delay direction if needed.
    if (RevMod_B_Offset[channel] < RevMod_B_Offset_Min[channel]) RevMod_B_Rate = RevMod_B_Rate_Up;
    if (RevMod_B_Offset_Max[channel] < RevMod_B_Offset[channel]) RevMod_B_Rate = RevMod_B_Rate_Down;

    //R1.00 Update ring buffer position. 
    RevMod_B_Ring1[channel]++;
    if (RevMod_B_Ring1_Max[channel] < RevMod_B_Ring1[channel]) RevMod_B_Ring1[channel] = 0;

    return (tSample * .6f) + (RevMod_B[channel][idx] * .4f);
}


void MakoDist01AudioProcessor::Mako_SetIR_Left()
{   
    //R1.00 This sub uses TEMP vars in case they are being changed in EDITOR.
    float Max = 0.0f;
    float MixInv = 1.0f - makoTempIRLMix;

    //R1.00 Create IR from blended Hi and Low freq versions.
    switch (makoTempIRL)
    {
      case 1: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR1[t] * MixInv) + (TIR1B[t] * makoTempIRLMix); break;
      case 2: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR2[t] * MixInv) + (TIR2B[t] * makoTempIRLMix); break;
      case 3: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR3[t] * MixInv) + (TIR3B[t] * makoTempIRLMix); break;
      case 4: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR4[t] * MixInv) + (TIR4B[t] * makoTempIRLMix); break;
      case 5: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR5[t] * MixInv) + (TIR5B[t] * makoTempIRLMix); break;
      case 6: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR6[t] * MixInv) + (TIR6B[t] * makoTempIRLMix); break;
      case 7: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR7[t] * MixInv) + (TIR7B[t] * makoTempIRLMix); break;
      case 8: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR8[t] * MixInv) + (TIR8B[t] * makoTempIRLMix); break;
      case 9: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR9[t] * MixInv) + (TIR9B[t] * makoTempIRLMix); break;
      default: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR10[t] * MixInv) + (TIR10B[t] * makoTempIRLMix); break;
    }

    //R1.00 Apply speaker size if selected. 
    if ((makoTempIRLSize < .499f) || (.501f < makoTempIRLSize))
    {
        int t = 0;
        float t2 = 0.0f;
        int t2i = 0;
        float t2diff = 0.0f;
        float t2s = (1024.0f + ((makoTempIRLSize - .5f) * 1000.0f)) / 1024.0f;

        //R1.00 Resample the IR using new specs.
        while ((t < 1024) && (t2 < 1023.0001f))
        {
            t2i = int(t2);
            t2diff = t2 - t2i;
            TIRL[t] = (TIRT[t2i] * (1.0f - t2diff)) + (TIRT[t2i + 1] * t2diff);
            if (Max < abs(TIRL[t])) Max = abs(TIRL[t]);
            t++;
            t2 += t2s;
        }

        //R1.00 Clear the unused values or the volume explodes when Maxed/Normalized.
        if (t < 1024)
        {
            while (t < 1024)
            {
                TIRL[t] = 0.0f;
                t++;
            }
        }
    }
    else
    {
        //R1.00 Default of .5 is selected.
        //R1.00 Do not apply Speaker Size/Time stretching.
        for (int t = 0; t < 1024; t++)
        {
            TIRL[t] = TIRT[t];
            if (Max < abs(TIRL[t])) Max = abs(TIRL[t]);
        }
    }

    //R1.00 Normalize.
    if ((0.0f < Max) && (Max < 1.0f))
        Max = 1.0f / Max;
    else
        Max = 1.0f;

    for (int t = 0; t < 1024; t++) TIRL[t] *= Max;
}

void MakoDist01AudioProcessor::Mako_SetIR_Right()
{
    //R1.00 This sub uses TEMP vars in case they are being changed in EDITOR.
    float Max = 0.0f;
    float MixInv = 1.0f - makoTempIRRMix;
    
    //R1.00 Create IR from blended Hi and Low freq versions.
    switch (makoTempIRR)
    {
      case 1:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR1[t] * MixInv) + (TIR1B[t] * makoTempIRRMix); break;
      case 2:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR2[t] * MixInv) + (TIR2B[t] * makoTempIRRMix); break;
      case 3:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR3[t] * MixInv) + (TIR3B[t] * makoTempIRRMix); break;
      case 4:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR4[t] * MixInv) + (TIR4B[t] * makoTempIRRMix); break;
      case 5:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR5[t] * MixInv) + (TIR5B[t] * makoTempIRRMix); break;
      case 6:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR6[t] * MixInv) + (TIR6B[t] * makoTempIRRMix); break;
      case 7:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR7[t] * MixInv) + (TIR7B[t] * makoTempIRRMix); break;
      case 8:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR8[t] * MixInv) + (TIR8B[t] * makoTempIRRMix); break;
      case 9:  for (int t = 0; t < 1024; t++) TIRT[t] = (TIR9[t] * MixInv) + (TIR9B[t] * makoTempIRRMix); break;
      default: for (int t = 0; t < 1024; t++) TIRT[t] = (TIR10[t] * MixInv) + (TIR10B[t] * makoTempIRRMix); break;
    }

    //R1.00 Apply speaker size if selected. 
    if ((makoTempIRRSize < .499f) || (.501f < makoTempIRRSize))
    {
        int t = 0;
        float t2 = 0.0f;
        int t2i = 0;
        float t2diff = 0.0f;
        float t2s = (1024.0f + ((makoTempIRRSize - .5f) * 1000.0f)) / 1024.0f;

        //R1.00 Resample the IR using new specs.
        while ((t < 1024) && (t2 < 1023.0001f))
        {
            t2i = int(t2);
            t2diff = t2 - t2i;
            TIRR[t] = (TIRT[t2i] * (1.0f - t2diff)) + (TIRT[t2i + 1] * t2diff);
            if (Max < abs(TIRR[t])) Max = abs(TIRR[t]);
            t++;
            t2 += t2s;
        }
        
        //R1.00 Clear the unused values or the volume explodes when Maxed/Normalized.
        if (t < 1024)
        {
            while (t < 1024)
            {
                TIRR[t] = 0.0f;
                t++;
            }
        }
    }
    else
    {
        //R1.00 Default of .5 is selected.
        //R1.00 Do not apply Speaker Size/Time stretching.
        for (int t = 0; t < 1024; t++)
        {
            TIRR[t] = TIRT[t];
            if (Max < abs(TIRR[t])) Max = abs(TIRR[t]);
        }
    }
   
    //R1.00 Normalize / Max.
    if ((0.0f < Max) && (Max < 1.0f))
        Max = 1.0f / Max;
    else
        Max = 1.0f;

    for (int t = 0; t < 1024; t++) TIRR[t] *= Max;
   
}

