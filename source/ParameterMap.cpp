// Created by David Bizon on 10/06/2025.

#include "ParameterMap.h"
#include <algorithm> // std::remove_if

inline WidgetUnit operator"" _w(unsigned long long int value) { return WidgetUnit(static_cast<int>(value)); }

const std::vector<settings::WidgetPanel>& ParameterMap::getPanels()
{
    using settings::MidiMessageType::CC;

    // "MIDI Channel = 0" means "use the global channel"
    static const std::vector<settings::WidgetPanel> panels = {
    //   Panel name,  width    Param name         Msg  Ch   CC#    min  max  inc  def
        {{"POLY-MOD",  8_w}, {{"F Env",          {CC,  0,   59 }, {0,   127, 1,   0  }},
                              {"Osc B",          {CC,  0,   60 }, {0,   120, 1,   0  }},
                              {"OSC A",          {CC,  0,   61 }, {0,   1,   1,   0  }},
                              {"PW",             {CC,  0,   62 }, {0,   1,   1,   0  }},
                              {"Filter",         {CC,  0,   63 }, {0,   1,   1,   0  }} }},
        {{"LFO",       8_w}, {{"Initial Amount", {CC,  0,   47 }, {0,   120, 1,   0  }},
                              {"Frequency",      {CC,  0,   46 }, {0,   120, 1,   0  }},
                              {"SAW",            {CC,  0,   117}, {0,   1,   1,   0  }},
                              {"TRI",            {CC,  0,   118}, {0,   1,   1,   0  }},
                              {"SQU",            {CC,  0,   119}, {0,   1,   1,   0  }} }},
        {{"WHEEL-MOD", 8_w}, {{"LFO/noise",      {CC,  0,   53 }, {0,   120, 1,   0  }},
                              {"A Freq",         {CC,  0,   54 }, {0,   1,   1,   0  }},
                              {"B Freq",         {CC,  0,   55 }, {0,   1,   1,   0  }},
                              {"A PW",           {CC,  0,   56 }, {0,   1,   1,   0  }},
                              {"B PW",           {CC,  0,   57 }, {0,   1,   1,   0  }},
                              {"Filter",         {CC,  0,   58 }, {0,   1,   1,   0  }} }},
        {{"OSC A",     8_w}, {{"Frequency",      {CC,  0,   3  }, {0,   120, 1,   48 }},
                              {"SAW",            {CC,  0,   15 }, {0,   1,   1,   1  }},
                              {"SQU",            {CC,  0,   20 }, {0,   1,   1,   0  }},
                              {"Pulse Width",    {CC,  0,   21 }, {0,   120, 1,   60 }},
                              {"Sync",           {CC,  0,   23 }, {0,   1,   1,   0  }} }},
        {{"OSC B",    12_w}, {{"Frequency",      {CC,  0,   9  }, {0,   120, 1,   48 }},
                              {"Fine Tune",      {CC,  0,   14 }, {0,   127, 1,   0 }},
                              {"SAW",            {CC,  0,   30 }, {0,   1,   1,   1  }},
                              {"TRI",            {CC,  0,   52 }, {0,   1,   1,   0  }},
                              {"SQU",            {CC,  0,   116}, {0,   1,   1,   0  }},
                              {"Pulse Width",    {CC,  0,   22 }, {0,   120, 1,   60 }},
                              {"Low Freq",       {CC,  0,   24 }, {0,   1,   1,   0  }},
                              {"Keyboard",       {CC,  0,   25 }, {0,   1,   1,   1  }} }},
        {{"MIXER",     8_w}, {{"Osc A",          {CC,  0,   27 }, {0,   120, 1,   120 }},
                              {"Osc B",          {CC,  0,   28 }, {0,   120, 1,   0 }},
                              {"Noise",          {CC,  0,   29 }, {0,   120, 1,   0 }} }},
        {{"FILTER",   10_w}, {{"Rev",            {CC,  0,   41 }, {0,   1,   1,   0, {"1/2", "3"} }},
                              {"Cutoff",         {CC,  0,   73 }, {0,   120, 1,   120 }},
                              {"Resonance",      {CC,  0,   31 }, {0,   120, 1,   0 }},
                              {"Env Amount",     {CC,  0,   89 }, {0,   120, 1,   0 }},
                              {"Keyboard",       {CC,  0,   35 }, {0,   2,   1,   0, {"off", "half", "full"} }},
                              {"Attack",         {CC,  0,   103}, {0,   120, 1,   0  }},
                              {"Decay",          {CC,  0,   105}, {0,   120, 1,   60 }},
                              {"Sustain",        {CC,  0,   107}, {0,   120, 1,   120}},
                              {"Release",        {CC,  0,   109}, {0,   120, 1,   0  }} }},
        {{"ENVELOPE",  8_w}, {{"Attack",         {CC,  0,   104}, {0,   120, 1,   0 }},
                              {"Decay",          {CC,  0,   106}, {0,   120, 1,   60 }},
                              {"Sustain",        {CC,  0,   108}, {0,   120, 1,   120}},
                              {"Release",        {CC,  0,   110}, {0,   120, 1,   0  }} }},
        {{"KEYBOARD",  8_w}, {{"VELO Filter",    {CC,  0,   90 }, {0,   1,   1,   0  }},
                              {"VELO VCA",       {CC,  0,   102}, {0,   1,   1,   0  }},
                              {"AFT Filter",     {CC,  0,   86 }, {0,   1,   1,   0  }},
                              {"AFT LFO",        {CC,  0,   87 }, {0,   1,   1,   0  }} }},
        {{"UNISON",    8_w}, {{"On/Off",         {CC,  0,   112}, {0,   1,   1,   0  }},
                              {"Voice Count",    {CC,  0,   113}, {0,   11,  1,   5, {"1", "2", "3", "4", "5", "", "", "", "", "", "chord", "2 poly"} }},
                              {"Note Priority",  {CC,  0,   71 }, {0,   3,   1,   0, {"lowest", "latest", "low+R", "lat+R"} }},
                              {"Detune",         {CC,  0,   114}, {0,   7,   1,   3  }} }},
        {{"GENERAL",   8_w}, {{"Glide rate",     {CC,  0,   26 }, {0,   120, 1,   0  }},
                              {"Vintage",        {CC,  0,   85 }, {0,   127, 1,   0  }},
                              {"PB Range",       {CC,  0,   70 }, {0,   11,  1,   2  }},
                              {"Release",        {CC,  0,   111}, {0,   1,   1,   0  }},
                              {"Master Volume",  {CC,  0,   7  }, {0,   120, 1,   120 }} }},
    };
    return panels;
}

std::string ParameterMap::generateParameterID(const std::string& panelName, const std::string& widgetName)
{
    auto out = removeSpaces(panelName) + "|" + removeSpaces(widgetName);
    return out;
}

std::string ParameterMap::removeSpaces(const std::string& input)
{
    std::string output = input;
    output.erase(remove_if(output.begin(), output.end(), isspace), output.end());
    return output;
}
