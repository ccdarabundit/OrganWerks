/* ------------------------------------------------------------
author: "Champ Darabundit"
name: "lossyPipeFramework"
version: "0.5"
Code generated with Faust 2.28.5 (https://faust.grame.fr)
Compilation options: -lang cpp -scal -ftz 0
------------------------------------------------------------ */

#ifndef  __lossyPipeEngine_H__
#define  __lossyPipeEngine_H__

#include <cmath>
#include <cstring>

/************************** BEGIN MapUI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>

/************************** BEGIN UI.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct UIReal
{
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* zone, const char* key, const char* val) {}
};

struct UI : public UIReal<FAUSTFLOAT>
{
    UI() {}
    virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_PATHBUILDER_H
#define FAUST_PATHBUILDER_H

#include <vector>
#include <string>
#include <algorithm>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class PathBuilder
{

    protected:
    
        std::vector<std::string> fControlsLevel;
       
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        std::string buildPath(const std::string& label) 
        {
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res += fControlsLevel[i];
                res += "/";
            }
            res += label;
            std::replace(res.begin(), res.end(), ' ', '_');
            return res;
        }
    
        std::string buildLabel(std::string label)
        {
            std::replace(label.begin(), label.end(), ' ', '_');
            return label;
        }
    
        void pushLabel(const std::string& label) { fControlsLevel.push_back(label); }
        void popLabel() { fControlsLevel.pop_back(); }
    
};

#endif  // FAUST_PATHBUILDER_H
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface
 * This class creates a map of complete hierarchical path and zones for each UI items.
 ******************************************************************************/

class MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Complete path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            fPathZoneMap[buildPath(label)] = zone;
            fLabelZoneMap[label] = zone;
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
        
        // set/get
        void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                *fPathZoneMap[path] = value;
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                *fLabelZoneMap[path] = value;
            }
        }
        
        FAUSTFLOAT getParamValue(const std::string& path)
        {
            if (fPathZoneMap.find(path) != fPathZoneMap.end()) {
                return *fPathZoneMap[path];
            } else if (fLabelZoneMap.find(path) != fLabelZoneMap.end()) {
                return *fLabelZoneMap[path];
            } else {
                return FAUSTFLOAT(0);
            }
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getMap() { return fPathZoneMap; }
        
        int getParamsCount() { return int(fPathZoneMap.size()); }
        
        std::string getParamAddress(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
    
        std::string getParamAddress(FAUSTFLOAT* zone)
        {
            for (auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        FAUSTFLOAT* getParamZone(const std::string& str)
        {
            if (fPathZoneMap.find(str) != fPathZoneMap.end()) {
                return fPathZoneMap[str];
            }
            if (fLabelZoneMap.find(str) != fLabelZoneMap.end()) {
                return fLabelZoneMap[str];
            }
            return nullptr;
        }
    
        FAUSTFLOAT* getParamZone(int index)
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
};


#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

struct Meta
{
    virtual ~Meta() {};
    virtual void declare(const char* key, const char* value) = 0;
    
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h **************************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2017 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __faustdsp__
#define __faustdsp__

#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct UI;
struct Meta;

/**
 * DSP memory manager.
 */

struct dsp_memory_manager {
    
    virtual ~dsp_memory_manager() {}
    
    virtual void* allocate(size_t size) = 0;
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class faustdsp {

    public:

        faustdsp() {}
        virtual ~faustdsp() {}

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Returns the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceInit(int sample_rate) = 0;

        /**
         * Init instance constant state
         *
         * @param sample_rate - the sampling rate in Hertz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (delay lines...) */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual faustdsp* clone() = 0;
    
        /**
         * Trigger the Meta* parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;
    
        /**
         * DSP instance computation, to be called with successive in/out audio buffers.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (eiher float, double or quad)
         *
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * DSP instance computation: alternative method to be used by subclasses.
         *
         * @param date_usec - the timestamp in microsec given by audio driver.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         *
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class decorator_dsp : public faustdsp {

    protected:

        faustdsp* fDSP;

    public:

        decorator_dsp(faustdsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class.
 */

class dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() {}
    
    public:
    
        virtual std::string getName() = 0;
        virtual std::string getSHAKey() = 0;
        virtual std::string getDSPCode() = 0;
        virtual std::string getCompileOptions() = 0;
        virtual std::vector<std::string> getLibraryList() = 0;
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        virtual faustdsp* createDSPInstance() = 0;
    
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

/**
 * On Intel set FZ (Flush to Zero) and DAZ (Denormals Are Zero)
 * flags to avoid costly denormals.
 */

#ifdef __SSE__
    #include <xmmintrin.h>
    #ifdef __SSE2__
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8040)
    #else
        #define AVOIDDENORMALS _mm_setcsr(_mm_getcsr() | 0x8000)
    #endif
#else
    #define AVOIDDENORMALS
#endif

#endif
/**************************  END  dsp.h **************************/

// BEGIN-FAUSTDSP


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <math.h>

static float lossyPipeEngine_faustpower2_f(float value) {
	return (value * value);
}

#ifndef FAUSTCLASS 
#define FAUSTCLASS lossyPipeEngine
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class lossyPipeEngine : public faustdsp {
    private:
       
       FAUSTFLOAT fEntry0;
       float fRec0[2];
       FAUSTFLOAT fEntry1;
       int fSampleRate;
       float fConst0;
       float fConst1;
       FAUSTFLOAT fEntry2;
       FAUSTFLOAT fEntry3;
       float fConst2;
       int iConst3;
       FAUSTFLOAT fEntry4;
       float fRec4[2];
       FAUSTFLOAT fEntry5;
       float fRec5[2];
       float fConst4;
       FAUSTFLOAT fEntry6;
       float fRec6[2];
       FAUSTFLOAT fButton0;
       int iVec0[2];
       int iRec7[2];
       int IOTA;
       float fVec1[2048];
       int iConst5;
       float fVec2[4096];
       FAUSTFLOAT fEntry7;
       FAUSTFLOAT fEntry8;
       float fRec3[3];
       FAUSTFLOAT fEntry9;
       FAUSTFLOAT fEntry10;
       float fRec1[4096];
       FAUSTFLOAT fEntry11;
       float fRec8[2];
       FAUSTFLOAT fEntry12;
       int iRec9[2];
       float fConst6;
       int iRec10[2];
       
    public:
       
       void metadata(Meta* m) {
           m->declare("author", "Champ Darabundit");
           m->declare("delays.lib/name", "Faust Delay Library");
           m->declare("delays.lib/version", "0.1");
           m->declare("description", "Waveguide Pipe");
           m->declare("envelopes.lib/asr:author", "Yann Orlarey, Stéphane Letz");
           m->declare("envelopes.lib/author", "GRAME");
           m->declare("envelopes.lib/copyright", "GRAME");
           m->declare("envelopes.lib/license", "LGPL with exception");
           m->declare("envelopes.lib/name", "Faust Envelope Library");
           m->declare("envelopes.lib/version", "0.1");
           m->declare("filename", "lossyPipeFramework.dsp");
           m->declare("filters.lib/fir:author", "Julius O. Smith III");
           m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
           m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
           m->declare("filters.lib/iir:author", "Julius O. Smith III");
           m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
           m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
           m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
           m->declare("filters.lib/name", "Faust Filters Library");
           m->declare("maths.lib/author", "GRAME");
           m->declare("maths.lib/copyright", "GRAME");
           m->declare("maths.lib/license", "LGPL with exception");
           m->declare("maths.lib/name", "Faust Math Library");
           m->declare("maths.lib/version", "2.3");
           m->declare("misceffects.lib/name", "Misc Effects Library");
           m->declare("misceffects.lib/version", "2.0");
           m->declare("name", "lossyPipeFramework");
           m->declare("noises.lib/name", "Faust Noise Generator Library");
           m->declare("noises.lib/version", "0.0");
           m->declare("platform.lib/name", "Generic Platform Library");
           m->declare("platform.lib/version", "0.1");
           m->declare("signals.lib/name", "Faust Signal Routing Library");
           m->declare("signals.lib/version", "0.0");
           m->declare("version", "0.5");
       }

       virtual int getNumInputs() {
           return 0;
       }
       virtual int getNumOutputs() {
           return 2;
       }
       virtual int getInputRate(int channel) {
           int rate;
           switch ((channel)) {
               default: {
                   rate = -1;
                   break;
               }
           }
           return rate;
       }
       virtual int getOutputRate(int channel) {
           int rate;
           switch ((channel)) {
               case 0: {
                   rate = 1;
                   break;
               }
               case 1: {
                   rate = 1;
                   break;
               }
               default: {
                   rate = -1;
                   break;
               }
           }
           return rate;
       }
       
       static void classInit(int sample_rate) {
       }
       
       virtual void instanceConstants(int sample_rate) {
           fSampleRate = sample_rate;
           fConst0 = std::min<float>(192000.0f, std::max<float>(1.0f, float(fSampleRate)));
           fConst1 = (0.0022727272f * fConst0);
           fConst2 = (0.100000001f * fConst0);
           iConst3 = int(fConst2);
           fConst4 = (0.0011363636f * fConst0);
           iConst5 = int((0.5f * float(iConst3)));
           fConst6 = (1.0f / std::max<float>(1.0f, fConst2));
       }
       
       virtual void instanceResetUserInterface() {
           fEntry0 = FAUSTFLOAT(1.0f);
           fEntry1 = FAUSTFLOAT(1.0f);
           fEntry2 = FAUSTFLOAT(32.0f);
           fEntry3 = FAUSTFLOAT(0.0f);
           fEntry4 = FAUSTFLOAT(1.0f);
           fEntry5 = FAUSTFLOAT(0.0f);
           fEntry6 = FAUSTFLOAT(0.40000000000000002f);
           fButton0 = FAUSTFLOAT(0.0f);
           fEntry7 = FAUSTFLOAT(0.0f);
           fEntry8 = FAUSTFLOAT(0.0f);
           fEntry9 = FAUSTFLOAT(0.0f);
           fEntry10 = FAUSTFLOAT(0.0f);
           fEntry11 = FAUSTFLOAT(0.5f);
           fEntry12 = FAUSTFLOAT(0.050000000000000003f);
       }
       
       virtual void instanceClear() {
           for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
               fRec0[l0] = 0.0f;
           }
           for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
               fRec4[l1] = 0.0f;
           }
           for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
               fRec5[l2] = 0.0f;
           }
           for (int l3 = 0; (l3 < 2); l3 = (l3 + 1)) {
               fRec6[l3] = 0.0f;
           }
           for (int l4 = 0; (l4 < 2); l4 = (l4 + 1)) {
               iVec0[l4] = 0;
           }
           for (int l5 = 0; (l5 < 2); l5 = (l5 + 1)) {
               iRec7[l5] = 0;
           }
           IOTA = 0;
           for (int l6 = 0; (l6 < 2048); l6 = (l6 + 1)) {
               fVec1[l6] = 0.0f;
           }
           for (int l7 = 0; (l7 < 4096); l7 = (l7 + 1)) {
               fVec2[l7] = 0.0f;
           }
           for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
               fRec3[l8] = 0.0f;
           }
           for (int l9 = 0; (l9 < 4096); l9 = (l9 + 1)) {
               fRec1[l9] = 0.0f;
           }
           for (int l10 = 0; (l10 < 2); l10 = (l10 + 1)) {
               fRec8[l10] = 0.0f;
           }
           for (int l11 = 0; (l11 < 2); l11 = (l11 + 1)) {
               iRec9[l11] = 0;
           }
           for (int l12 = 0; (l12 < 2); l12 = (l12 + 1)) {
               iRec10[l12] = 0;
           }
       }
       
       virtual void init(int sample_rate) {
           classInit(sample_rate);
           instanceInit(sample_rate);
       }
       virtual void instanceInit(int sample_rate) {
           instanceConstants(sample_rate);
           instanceResetUserInterface();
           instanceClear();
       }
       
       virtual lossyPipeEngine* clone() {
           return new lossyPipeEngine();
       }
       
       virtual int getSampleRate() {
           return fSampleRate;
       }
       
       virtual void buildUserInterface(UI* ui_interface) {
           ui_interface->openVerticalBox("lossyPipeFramework");
           ui_interface->addNumEntry("Attack", &fEntry12, 0.0500000007f, 9.99999975e-05f, 0.5f, 9.99999975e-05f);
           ui_interface->addNumEntry("Feedback Gain", &fEntry6, 0.400000006f, 0.0f, 5.0f, 0.00999999978f);
           ui_interface->addNumEntry("Flow Gain", &fEntry4, 1.0f, 0.5f, 10.0f, 0.00999999978f);
           ui_interface->addNumEntry("Gain", &fEntry0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
           ui_interface->addButton("Gate", &fButton0);
           ui_interface->addNumEntry("Group Delay", &fEntry3, 0.0f, 0.0f, 1.0f, 1.00000001e-07f);
           ui_interface->addNumEntry("Jet Offset", &fEntry5, 0.0f, -1.0f, 1.0f, 0.00999999978f);
           ui_interface->addNumEntry("MIDI Note", &fEntry2, 32.0f, 32.0f, 96.0f, 1.0f);
           ui_interface->addNumEntry("Mix", &fEntry11, 0.5f, 0.0f, 1.0f, 0.00999999978f);
           ui_interface->addNumEntry("a1", &fEntry7, 0.0f, 0.0f, 2.0f, 9.99999975e-06f);
           ui_interface->addNumEntry("a2", &fEntry8, 0.0f, 0.0f, 2.0f, 9.99999975e-06f);
           ui_interface->addNumEntry("b0", &fEntry1, 1.0f, 0.0f, 2.0f, 9.99999975e-06f);
           ui_interface->addNumEntry("b1", &fEntry9, 0.0f, 0.0f, 2.0f, 9.99999975e-06f);
           ui_interface->addNumEntry("b2", &fEntry10, 0.0f, 0.0f, 2.0f, 9.99999975e-06f);
           ui_interface->closeBox();
       }
       
       virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
           FAUSTFLOAT* output0 = outputs[0];
           FAUSTFLOAT* output1 = outputs[1];
           float fSlow0 = (9.99999975e-05f * float(fEntry0));
           float fSlow1 = float(fEntry1);
           float fSlow2 = std::pow(2.0f, (0.0833333358f * float((int(float(fEntry2)) + -69))));
           float fSlow3 = (fConst1 / fSlow2);
           float fSlow4 = (fSlow3 + -1.49999499f);
           float fSlow5 = std::floor(fSlow4);
           float fSlow6 = (fSlow3 + (-1.0f - fSlow5));
           float fSlow7 = (fSlow3 + (-2.0f - fSlow5));
           float fSlow8 = (fSlow3 + (-3.0f - fSlow5));
           float fSlow9 = (fSlow3 + (-4.0f - fSlow5));
           float fSlow10 = ((((0.0f - fSlow6) * (0.0f - (0.5f * fSlow7))) * (0.0f - (0.333333343f * fSlow8))) * (0.0f - (0.25f * fSlow9)));
           float fSlow11 = (fConst0 * ((0.0022727272f / fSlow2) - float(fEntry3)));
           float fSlow12 = (fSlow11 + -1.49999499f);
           float fSlow13 = std::floor(fSlow12);
           float fSlow14 = (fSlow11 + (-1.0f - fSlow13));
           float fSlow15 = (fSlow11 + (-2.0f - fSlow13));
           float fSlow16 = (fSlow11 + (-3.0f - fSlow13));
           float fSlow17 = (fSlow11 + (-4.0f - fSlow13));
           float fSlow18 = ((((0.0f - fSlow14) * (0.0f - (0.5f * fSlow15))) * (0.0f - (0.333333343f * fSlow16))) * (0.0f - (0.25f * fSlow17)));
           int iSlow19 = int(fSlow12);
           int iSlow20 = (std::min<int>(iConst3, std::max<int>(0, iSlow19)) + 1);
           float fSlow21 = (fSlow11 - fSlow13);
           float fSlow22 = (((0.0f - fSlow15) * (0.0f - (0.5f * fSlow16))) * (0.0f - (0.333333343f * fSlow17)));
           int iSlow23 = (std::min<int>(iConst3, std::max<int>(0, (iSlow19 + 1))) + 1);
           float fSlow24 = (0.5f * ((fSlow14 * (0.0f - fSlow16)) * (0.0f - (0.5f * fSlow17))));
           int iSlow25 = (std::min<int>(iConst3, std::max<int>(0, (iSlow19 + 2))) + 1);
           float fSlow26 = (fSlow14 * fSlow15);
           float fSlow27 = (0.166666672f * (fSlow26 * (0.0f - fSlow17)));
           int iSlow28 = (std::min<int>(iConst3, std::max<int>(0, (iSlow19 + 3))) + 1);
           float fSlow29 = (0.0416666679f * (fSlow26 * fSlow16));
           int iSlow30 = (std::min<int>(iConst3, std::max<int>(0, (iSlow19 + 4))) + 1);
           float fSlow31 = (0.00100000005f * float(fEntry4));
           float fSlow32 = (0.00100000005f * float(fEntry5));
           float fSlow33 = (fConst4 / fSlow2);
           float fSlow34 = (fSlow33 + -1.49999499f);
           float fSlow35 = std::floor(fSlow34);
           float fSlow36 = (fSlow33 + (-1.0f - fSlow35));
           float fSlow37 = (fSlow33 + (-2.0f - fSlow35));
           float fSlow38 = (fSlow33 + (-3.0f - fSlow35));
           float fSlow39 = (fSlow33 + (-4.0f - fSlow35));
           float fSlow40 = ((((0.0f - fSlow36) * (0.0f - (0.5f * fSlow37))) * (0.0f - (0.333333343f * fSlow38))) * (0.0f - (0.25f * fSlow39)));
           float fSlow41 = (0.00100000005f * float(fEntry6));
           int iSlow42 = int(float(fButton0));
           float fSlow43 = (4.65661287e-10f * float(iSlow42));
           int iSlow44 = int(fSlow34);
           int iSlow45 = std::min<int>(iConst5, std::max<int>(0, iSlow44));
           float fSlow46 = (fSlow33 - fSlow35);
           float fSlow47 = (((0.0f - fSlow37) * (0.0f - (0.5f * fSlow38))) * (0.0f - (0.333333343f * fSlow39)));
           int iSlow48 = std::min<int>(iConst5, std::max<int>(0, (iSlow44 + 1)));
           float fSlow49 = (0.5f * ((fSlow36 * (0.0f - fSlow38)) * (0.0f - (0.5f * fSlow39))));
           int iSlow50 = std::min<int>(iConst5, std::max<int>(0, (iSlow44 + 2)));
           float fSlow51 = (fSlow36 * fSlow37);
           float fSlow52 = (0.166666672f * (fSlow51 * (0.0f - fSlow39)));
           int iSlow53 = std::min<int>(iConst5, std::max<int>(0, (iSlow44 + 3)));
           float fSlow54 = (0.0416666679f * (fSlow51 * fSlow38));
           int iSlow55 = std::min<int>(iConst5, std::max<int>(0, (iSlow44 + 4)));
           int iSlow56 = int(fSlow4);
           int iSlow57 = std::min<int>(iConst3, std::max<int>(0, iSlow56));
           float fSlow58 = (fSlow3 - fSlow5);
           float fSlow59 = (((0.0f - fSlow7) * (0.0f - (0.5f * fSlow8))) * (0.0f - (0.333333343f * fSlow9)));
           int iSlow60 = std::min<int>(iConst3, std::max<int>(0, (iSlow56 + 1)));
           float fSlow61 = (0.5f * ((fSlow6 * (0.0f - fSlow8)) * (0.0f - (0.5f * fSlow9))));
           int iSlow62 = std::min<int>(iConst3, std::max<int>(0, (iSlow56 + 2)));
           float fSlow63 = (fSlow6 * fSlow7);
           float fSlow64 = (0.166666672f * (fSlow63 * (0.0f - fSlow9)));
           int iSlow65 = std::min<int>(iConst3, std::max<int>(0, (iSlow56 + 3)));
           float fSlow66 = (0.0416666679f * (fSlow63 * fSlow8));
           int iSlow67 = std::min<int>(iConst3, std::max<int>(0, (iSlow56 + 4)));
           float fSlow68 = float(fEntry7);
           float fSlow69 = float(fEntry8);
           float fSlow70 = float(fEntry9);
           float fSlow71 = float(fEntry10);
           float fSlow72 = (0.00100000005f * float(fEntry11));
           float fSlow73 = (1.0f / std::max<float>(1.0f, (fConst0 * float(fEntry12))));
           int iSlow74 = (iSlow42 == 0);
           for (int i = 0; (i < count); i = (i + 1)) {
               fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
               float fTemp0 = ((fSlow18 * fRec1[((IOTA - iSlow20) & 4095)]) + (fSlow21 * ((((fSlow22 * fRec1[((IOTA - iSlow23) & 4095)]) + (fSlow24 * fRec1[((IOTA - iSlow25) & 4095)])) + (fSlow27 * fRec1[((IOTA - iSlow28) & 4095)])) + (fSlow29 * fRec1[((IOTA - iSlow30) & 4095)]))));
               fRec4[0] = (fSlow31 + (0.999000013f * fRec4[1]));
               fRec5[0] = (fSlow32 + (0.999000013f * fRec5[1]));
               fRec6[0] = (fSlow41 + (0.999000013f * fRec6[1]));
               iVec0[0] = iSlow42;
               iRec7[0] = ((1103515245 * iRec7[1]) + 12345);
               float fTemp1 = ((fRec6[0] * fTemp0) + (fSlow43 * float(iRec7[0])));
               fVec1[(IOTA & 2047)] = fTemp1;
               float fTemp2 = std::max<float>(-1.0f, std::min<float>(1.0f, ((fRec5[0] + ((fSlow40 * fVec1[((IOTA - iSlow45) & 2047)]) + (fSlow46 * ((((fSlow47 * fVec1[((IOTA - iSlow48) & 2047)]) + (fSlow49 * fVec1[((IOTA - iSlow50) & 2047)])) + (fSlow52 * fVec1[((IOTA - iSlow53) & 2047)])) + (fSlow54 * fVec1[((IOTA - iSlow55) & 2047)]))))) + 1.0f)));
               float fTemp3 = (fTemp0 - ((fRec4[0] * fTemp2) * (1.0f - (0.333333343f * lossyPipeEngine_faustpower2_f(fTemp2)))));
               fVec2[(IOTA & 4095)] = fTemp3;
               fRec3[0] = (((fSlow10 * fVec2[((IOTA - iSlow57) & 4095)]) + (fSlow58 * ((((fSlow59 * fVec2[((IOTA - iSlow60) & 4095)]) + (fSlow61 * fVec2[((IOTA - iSlow62) & 4095)])) + (fSlow64 * fVec2[((IOTA - iSlow65) & 4095)])) + (fSlow66 * fVec2[((IOTA - iSlow67) & 4095)])))) - ((fSlow68 * fRec3[1]) + (fSlow69 * fRec3[2])));
               fRec1[(IOTA & 4095)] = (((fSlow1 * fRec3[0]) + (fSlow70 * fRec3[1])) + (fSlow71 * fRec3[2]));
               float fRec2 = fTemp3;
               fRec8[0] = (fSlow72 + (0.999000013f * fRec8[1]));
               iRec9[0] = (iSlow42 + (iRec9[1] * (iVec0[1] >= iSlow42)));
               iRec10[0] = (iSlow74 * (iRec10[1] + 1));
               float fTemp4 = ((fRec0[0] * ((fRec1[((IOTA - 0) & 4095)] * fRec8[0]) + (fRec2 * (1.0f - fRec8[0])))) * std::max<float>(0.0f, (std::min<float>((fSlow73 * float(iRec9[0])), 1.0f) - (fConst6 * float(iRec10[0])))));
               output0[i] = FAUSTFLOAT(fTemp4);
               output1[i] = FAUSTFLOAT(fTemp4);
               fRec0[1] = fRec0[0];
               fRec4[1] = fRec4[0];
               fRec5[1] = fRec5[0];
               fRec6[1] = fRec6[0];
               iVec0[1] = iVec0[0];
               iRec7[1] = iRec7[0];
               IOTA = (IOTA + 1);
               fRec3[2] = fRec3[1];
               fRec3[1] = fRec3[0];
               fRec8[1] = fRec8[0];
               iRec9[1] = iRec9[0];
               iRec10[1] = iRec10[0];
           }
       }

};

// END-FAUSTDSP

#endif
