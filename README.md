# Mako Distortion
* Guitar Effects Processor
* Microsoft Visual Studio 2022.
* JUCE Audio Development Framework
* https://juce.com/
* Version: 1.0
* Posted: February 5, 2023

VERSION
------------------------------------------------------------------
1.00 - Initial release.
       
SUMMARY
------------------------------------------------------------------
Mako Distortion is a C++ program designed to be used as a Virtual
Studio Technology (VST) plugin to be used in Digital Audio
Workstations (DAW). The program allows a guitar signal to be
modified to sound as though it is going thru an amplifier and
various studio effects such as delay and reverb.

It was developed on the JUCE framework. JUCE handles all of the 
heavy lifting interfacing to the DAW as a VST. JUCE will also 
compile to a stand alone EXE. Program debugging is easiest in 
the standalone EXE.

The VST has only been successfully tested in Reaper and Cakewalk.

I am not a C++ programmer. So many parts of this code could be 
done efficiently and easier to read if using correct C++ style.

It is presented as a simple example to get started in JUCE VST
programming.

VST/EXE INFORMATION
------------------------------------------------------------------
1) Download the ZIP file that the holds the VST and EXE files.
2) The EXE can be located and run from anywhere.
3) The VST needs to be located where your other VST files are
located. Most DAWs will have a list of folders that it searches
thru when starting.
4) information of program operation can be found in the ReadMe.txt
file included in the ZIP file.

SOURCE CODE INFORMATION
------------------------------------------------------------------
All of the C++ project setup and the various data file resources
are setup in the JUCE application PROJUCER. After you create the
JUCE project, you can save it and startup MS C++ from inside JUCE.

The C++ project will have many files included. There are only four
files that need to be edited to create a simple VST:
- PluginEditor.h
- PluginEditor.cpp
- PluginProcessor.h
- PluginProcessor.cpp

The VST will be a multithreaded application. 

During operation the PluginProcessor is constantly running and
processing incomong audio data. When a user wants to change a
setting, the editor can be loaded and runs in a seperate thread. 

GUI ELEMENTS
------------------------------------------------------------------
JUCE can be used to create any type of graphical presentation of
data and controls. This application only uses a few basic controls
provided by JUCE. There are some simple bitmaps used to represent
the signal path as it changes. 

The program uses:
- Slider Object
- Button Object
- Label Object

PROCESSING AUDIO
------------------------------------------------------------------
JUCE handles all of the interfacing with the DAW to get access to
the incoming and outgoing audio data.

The function that is constantly called to handle data is:
```C++
void MakoDist01AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
```
This function has premade for loops that will loop thru the data
channels.

Read incoming data with:
```C++
sample = buffer.getSample(channel, samp); 
```
Then your code can process this incoming data sample that is 
scaled from -1.0f to 1.0f.

When processing is complete save your data to the outgoing audio
stream with:
```C++
channelData[samp] = sample; 
```

Example code to double your volume would be:
```C++
sample = buffer.getSample(channel, samp);  //Read sample.
sample *= 2.0f;                            //Double the volume.
channelData[samp] = sample;                //Save the new sample value.
```

JUCE has many Digital Signal Processing (DSP) abilities built in.
None of them were used in this application. All code was hand 
made for fun and experimentation with inspiration from the book:
Designing Audio Effect Plugins in C++
By Will Pirkle
       
https://www.amazon.com/Designing-Audio-Effect-Plugins-C/dp/1138591939

Code implemented that uses ideas from the book are:
- BiQuad Frequency filters.
- Phaser All Pass Filter design.
- Basic Reverb delay and gain values. 


SETUP STORAGE
------------------------------------------------------------------
Mako Distortion is designed to be used as a VST. All setup saving
and restoring is handled by the DAW thru the VST code.

To make your setup available to the DAW you need to setup up a few 
variables. This program uses these methods to interface to the DAW.


1) Create a ValueStateTree in PluginProcessor.
   
   ```C++   
   juce::AudioProcessorValueTreeState parameters;
    
   parameters (*this, nullptr, "PARAMETERS",
   {
     std::make_unique<juce::AudioParameterFloat>(  "gain",    "Gain", .01f, 1.0f, .1f),
     std::make_unique<juce::AudioParameterInt>(    "chan",    "Chan",  0, 3, 0)
   }
   ```
   
2) Connect your controls to the ValueStateTree in PluginEditor.
   ```C++   
   makoSA_Gain = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment> (makoProcessor.parameters, "gain", mjSlidVol);
   makoSA_Chan = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(makoProcessor.parameters, "chan", mjSlidChan);
   ```   
3) Create the functions required to send the ValueTree data to 
   the DAW. Tese are in PluginEditor.cpp.
   
   ```C++   
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
   ```   

VERSION HISTORY
------------------------------------------------------------------
v1.00
* Initial Release.
