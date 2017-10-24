//
// Add this as a file to your Introjucer project as an easy way of including all
// the source files for DspFilters
//
// The path to the DSPFilters include directory must be defined in your header search paths
// e.g. /Develop/DSPFilters/shared/DSPFilters/include
//
// This lets you include Dsp.h in your code (#include "DspFilters/Dsp.h") but also enables
// the use of relative paths to the cpp files below
//
// Including the JUCE headers fixes a problem with std::max in RootFinder.cpp and also allows
// us to disable Visual Studio warnings about unreachable code in Param.cpp
#include "../JuceLibraryCode/JuceHeader.h"

// DspFilter Library Source
#include "../source/Bessel.cpp"
#include "../source/Biquad.cpp"
#include "../source/Butterworth.cpp"
#include "../source/Cascade.cpp"
#include "../source/ChebyshevI.cpp"
#include "../source/ChebyshevII.cpp"
#include "../source/Custom.cpp"
#include "../source/Design.cpp"
#include "../source/Documentation.cpp"
#include "../source/Elliptic.cpp"
#include "../source/Filter.cpp"
#include "../source/Legendre.cpp"
#if JUCE_MSVC
#pragma warning (push, 0)&nbsp;
#pragma warning (disable: 4702)
#endif
#include "../source/Param.cpp"
#if JUCE_MSVC
#pragma warning(pop)
#endif
#include "../source/PoleFilter.cpp"
#include "../source/RBJ.cpp"
#include "../source/RootFinder.cpp"
#include "../source/State.cpp"
