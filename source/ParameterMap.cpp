// Created by David Bizon on 10/06/2025.

#include "ParameterMap.h"

inline WidgetUnit operator"" _w(unsigned long long int value) { return WidgetUnit(static_cast<int>(value)); }

const std::vector<settings::WidgetPanel>& ParameterMap::getPanels()
{
    using settings::MidiMessageType::CC;

    static const std::vector<settings::WidgetPanel> panels = {
    //   Panel name,  width    Param name         Msg  Ch   CC#    min  max  inc  def
        {{"POLY-MOD",  8_w}, {{"F Env",          {CC,  7,   59 }, {0,   127, 1,   0  }},
                              {"Osc B",          {CC,  7,   60 }, {0,   120, 1,   0  }},
                              {"OSC A",          {CC,  7,   61 }, {0,   1,   1,   0  }},
                              {"PW",             {CC,  7,   62 }, {0,   1,   1,   0  }},
                              {"Filter",         {CC,  7,   63 }, {0,   1,   1,   0  }} }},
        {{"LFO",       8_w}, {{"Initial Amount", {CC,  7,   47 }, {0,   120, 1,   0  }},
                              {"Frequency",      {CC,  7,   46 }, {0,   120, 1,   0  }},
                              {"SAW",            {CC,  7,   117}, {0,   1,   1,   0  }},
                              {"TRI",            {CC,  7,   118}, {0,   1,   1,   0  }},
                              {"SQU",            {CC,  7,   119}, {0,   1,   1,   0  }} }},
        {{"WHEEL-MOD", 8_w}, {{"LFO/noise",      {CC,  7,   53 }, {0,   120, 1,   0  }},
                              {"A Freq",         {CC,  7,   54 }, {0,   1,   1,   0  }},
                              {"B Freq",         {CC,  7,   55 }, {0,   1,   1,   0  }},
                              {"A PW",           {CC,  7,   56 }, {0,   1,   1,   0  }},
                              {"B PW",           {CC,  7,   57 }, {0,   1,   1,   0  }},
                              {"Filter",         {CC,  7,   58 }, {0,   1,   1,   0  }} }},
        {{"OSC A",     8_w}, {{"Frequency",      {CC,  7,   3  }, {0,   120, 1,   60 }},
                              {"SAW",            {CC,  7,   15 }, {0,   1,   1,   0  }},
                              {"SQU",            {CC,  7,   20 }, {0,   1,   1,   0  }},
                              {"Pulse Width",    {CC,  7,   21 }, {0,   120, 1,   60 }},
                              {"Sync",           {CC,  7,   23 }, {0,   1,   1,   0  }} }},
        {{"OSC B",    12_w}, {{"Frequency",      {CC,  7,   9  }, {0,   120, 1,   60 }},
                              {"Fine Tune",      {CC,  7,   14 }, {0,   127, 1,   64 }},
                              {"SAW",            {CC,  7,   30 }, {0,   1,   1,   0  }},
                              {"TRI",            {CC,  7,   52 }, {0,   1,   1,   0  }},
                              {"SQU",            {CC,  7,   116}, {0,   1,   1,   0  }},
                              {"Pulse Width",    {CC,  7,   22 }, {0,   120, 1,   60 }},
                              {"Low Freq",       {CC,  7,   24 }, {0,   1,   1,   0  }},
                              {"Keyboard",       {CC,  7,   25 }, {0,   1,   1,   0  }} }},
        {{"MIXER",     8_w}, {{"Osc A",          {CC,  7,   27 }, {0,   120, 1,   60 }},
                              {"Osc B",          {CC,  7,   28 }, {0,   120, 1,   60 }},
                              {"Noise",          {CC,  7,   29 }, {0,   120, 1,   60 }} }},
        {{"FILTER",   10_w}, {{"Rev",            {CC,  7,   41 }, {0,   1,   1,   0, {"1/2", "3"} }},
                              {"Cutoff",         {CC,  7,   73 }, {0,   120, 1,   60 }},
                              {"Resonance",      {CC,  7,   31 }, {0,   120, 1,   60 }},
                              {"Env Amount",     {CC,  7,   89 }, {0,   120, 1,   60 }},
                              {"Keyboard",       {CC,  7,   35 }, {0,   2,   1,   0, {"off", "half", "full"} }},
                              {"Attack",         {CC,  7,   103}, {0,   120, 1,   0  }},
                              {"Decay",          {CC,  7,   105}, {0,   120, 1,   60 }},
                              {"Sustain",        {CC,  7,   107}, {0,   120, 1,   120}},
                              {"Release",        {CC,  7,   109}, {0,   120, 1,   0  }} }},
        {{"ENVELOPE",  8_w}, {{"Attack VCA",     {CC,  7,   104}, {0,   120, 1,   60 }},
                              {"Decay VCA",      {CC,  7,   106}, {0,   120, 1,   60 }},
                              {"Sustain VCA",    {CC,  7,   108}, {0,   120, 1,   120}},
                              {"Release VCA",    {CC,  7,   110}, {0,   120, 1,   0  }} }},
        {{"KEYBOARD",  8_w}, {{"VELO Filter",    {CC,  7,   90 }, {0,   1,   1,   0  }},
                              {"VELO VCA",       {CC,  7,   102}, {0,   1,   1,   0  }},
                              {"AFT Filter",     {CC,  7,   86 }, {0,   1,   1,   0  }},
                              {"AFT LFO",        {CC,  7,   87 }, {0,   1,   1,   0  }} }},
        {{"UNISON",    8_w}, {{"On/Off",         {CC,  7,   112}, {0,   1,   1,   0  }},
                              {"Voice Count",    {CC,  7,   113}, {0,   11,  1,   5, {"1", "2", "3", "4", "5", "", "", "", "", "", "chord", "2 poly"} }},
                              {"Note Priority",  {CC,  7,   71 }, {0,   3,   1,   0, {"lowest", "latest", "low+R", "lat+R"} }},
                              {"Detune",         {CC,  7,   114}, {0,   7,   1,   3  }} }},
        {{"GENERAL",   8_w}, {{"Glide rate",     {CC,  7,   26 }, {0,   120, 1,   0  }},
                              {"Vintage",        {CC,  7,   85 }, {0,   127, 1,   0  }},
                              {"PB Range",       {CC,  7,   70 }, {0,   11,  1,   2  }},
                              {"Release",        {CC,  7,   111}, {0,   1,   1,   0  }},
                              {"Master Volume",  {CC,  7,   7  }, {0,   120, 1,   60 }} }},
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
