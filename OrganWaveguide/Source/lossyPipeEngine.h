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

#ifndef __dsp__
#define __dsp__

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

class dsp {

    public:

        dsp() {}
        virtual ~dsp() {}

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
        virtual dsp* clone() = 0;
    
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

class decorator_dsp : public dsp {

    protected:

        dsp* fDSP;

    public:

        decorator_dsp(dsp* dsp = nullptr):fDSP(dsp) {}
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
    
        virtual dsp* createDSPInstance() = 0;
    
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

/* link with : "" */
#include <algorithm>
#include <cmath>
#include <math.h>


#ifndef FAUSTCLASS 
#define FAUSTCLASS lossyPipeEngine
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

class lossyPipeEngine : public dsp {
	
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
	float fVec2[2048];
	FAUSTFLOAT fEntry7;
	FAUSTFLOAT fEntry8;
	float fRec3[3];
	FAUSTFLOAT fEntry9;
	FAUSTFLOAT fEntry10;
	float fRec1[8192];
	FAUSTFLOAT fEntry11;
	float fRec8[2];
	float fConst6;
	int iRec9[2];
	float fConst7;
	int iRec10[2];
    static float refIIRCoefs[65][5];
    static int nTaps[65];
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
		fConst1 = (0.0045454544f * fConst0);
		fConst2 = (0.100000001f * fConst0);
		iConst3 = int(fConst2);
		fConst4 = (0.0011363636f * fConst0);
		iConst5 = int((0.5f * float(iConst3)));
		fConst6 = (1.0f / std::max<float>(1.0f, (0.00999999978f * fConst0)));
		fConst7 = (1.0f / std::max<float>(1.0f, fConst2));
	}
	
	virtual void instanceResetUserInterface() {
		fEntry0 = FAUSTFLOAT(1.0f);
		fEntry1 = FAUSTFLOAT(1.0f);
		fEntry2 = FAUSTFLOAT(32.0f);
		fEntry3 = FAUSTFLOAT(1015.0f);
		fEntry4 = FAUSTFLOAT(0.0f);
		fEntry5 = FAUSTFLOAT(1.0f);
		fEntry6 = FAUSTFLOAT(0.40000000000000002f);
		fButton0 = FAUSTFLOAT(0.0f);
		fEntry7 = FAUSTFLOAT(0.0f);
		fEntry8 = FAUSTFLOAT(0.0f);
		fEntry9 = FAUSTFLOAT(0.0f);
		fEntry10 = FAUSTFLOAT(0.0f);
		fEntry11 = FAUSTFLOAT(0.5f);
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
		for (int l7 = 0; (l7 < 2048); l7 = (l7 + 1)) {
			fVec2[l7] = 0.0f;
		}
		for (int l8 = 0; (l8 < 3); l8 = (l8 + 1)) {
			fRec3[l8] = 0.0f;
		}
		for (int l9 = 0; (l9 < 8192); l9 = (l9 + 1)) {
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
	void setReflectionCoefs(int midiKey)
    {
        int midiIndex = midiKey - 32;
        fEntry1 = refIIRCoefs[midiIndex][0];
        fEntry9 = refIIRCoefs[midiIndex][1];
        fEntry10 = refIIRCoefs[midiIndex][2];
        fEntry7 = refIIRCoefs[midiIndex][3];
        fEntry8 = refIIRCoefs[midiIndex][4];
    }
    
    void setInitialDelay(int midiKey)
    {
        int midiIndex = midiKey - 32;
        fEntry3 = nTaps[midiIndex];
    }
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("lossyPipeFramework");
		ui_interface->addNumEntry("Feedback Gain", &fEntry6, 0.400000006f, 0.0f, 5.0f, 0.00999999978f);
		ui_interface->addNumEntry("Flow Gain", &fEntry5, 1.0f, 0.5f, 10.0f, 0.00999999978f);
		ui_interface->addNumEntry("Gain", &fEntry0, 1.0f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->addButton("Gate", &fButton0);
		ui_interface->addNumEntry("Jet Offset", &fEntry4, 0.0f, -1.0f, 1.0f, 0.00999999978f);
		ui_interface->addNumEntry("MIDI Note", &fEntry2, 32.0f, 32.0f, 96.0f, 1.0f);
		ui_interface->addNumEntry("Mix", &fEntry11, 0.5f, 0.0f, 1.0f, 0.00999999978f);
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
        setReflectionCoefs(int(fEntry2));
        setInitialDelay(int(fEntry2));
		float fSlow0 = (9.99999975e-05f * float(fEntry0));
		float fSlow1 = float(fEntry1);
		float fSlow2 = std::pow(2.0f, (0.0833333358f * float((int(float(fEntry2)) + -69))));
		float fSlow3 = (fConst1 / fSlow2);
		int iSlow4 = (int(float(fEntry3)) + 2);
		float fSlow5 = float(iSlow4);
		float fSlow6 = (fSlow3 + (-1.49999499f - fSlow5));
		float fSlow7 = (fSlow5 + std::floor(fSlow6));
		float fSlow8 = (fSlow3 - fSlow7);
		float fSlow9 = (fSlow3 + (-2.0f - fSlow7));
		float fSlow10 = (fSlow3 + (-3.0f - fSlow7));
		float fSlow11 = (fSlow3 + (-4.0f - fSlow7));
		float fSlow12 = (((0.0f - fSlow9) * (0.0f - (0.5f * fSlow10))) * (0.0f - (0.333333343f * fSlow11)));
		int iSlow13 = int(fSlow6);
		int iSlow14 = (std::min<int>(iConst3, std::max<int>(0, (iSlow13 + 1))) + 1);
		float fSlow15 = (fSlow3 + (-1.0f - fSlow7));
		float fSlow16 = (0.5f * ((fSlow15 * (0.0f - fSlow10)) * (0.0f - (0.5f * fSlow11))));
		int iSlow17 = (std::min<int>(iConst3, std::max<int>(0, (iSlow13 + 2))) + 1);
		float fSlow18 = (fSlow15 * fSlow9);
		float fSlow19 = (0.166666672f * (fSlow18 * (0.0f - fSlow11)));
		int iSlow20 = (std::min<int>(iConst3, std::max<int>(0, (iSlow13 + 3))) + 1);
		float fSlow21 = (0.0416666679f * (fSlow18 * fSlow10));
		int iSlow22 = (std::min<int>(iConst3, std::max<int>(0, (iSlow13 + 4))) + 1);
		float fSlow23 = (0.00100000005f * float(fEntry4));
		float fSlow24 = ((((0.0f - fSlow15) * (0.0f - (0.5f * fSlow9))) * (0.0f - (0.333333343f * fSlow10))) * (0.0f - (0.25f * fSlow11)));
		int iSlow25 = (std::min<int>(iConst3, std::max<int>(0, iSlow13)) + 1);
		float fSlow26 = (0.00100000005f * float(fEntry5));
		float fSlow27 = (fConst4 / fSlow2);
		float fSlow28 = (fSlow27 + -1.49999499f);
		float fSlow29 = std::floor(fSlow28);
		float fSlow30 = (fSlow27 + (-1.0f - fSlow29));
		float fSlow31 = (fSlow27 + (-2.0f - fSlow29));
		float fSlow32 = (fSlow27 + (-3.0f - fSlow29));
		float fSlow33 = (fSlow27 + (-4.0f - fSlow29));
		float fSlow34 = ((((0.0f - fSlow30) * (0.0f - (0.5f * fSlow31))) * (0.0f - (0.333333343f * fSlow32))) * (0.0f - (0.25f * fSlow33)));
		float fSlow35 = (0.00100000005f * float(fEntry6));
		int iSlow36 = int(float(fButton0));
		float fSlow37 = (4.65661287e-10f * float(iSlow36));
		int iSlow38 = int(fSlow28);
		int iSlow39 = std::min<int>(iConst5, std::max<int>(0, iSlow38));
		float fSlow40 = (fSlow27 - fSlow29);
		float fSlow41 = (((0.0f - fSlow31) * (0.0f - (0.5f * fSlow32))) * (0.0f - (0.333333343f * fSlow33)));
		int iSlow42 = std::min<int>(iConst5, std::max<int>(0, (iSlow38 + 1)));
		float fSlow43 = (0.5f * ((fSlow30 * (0.0f - fSlow32)) * (0.0f - (0.5f * fSlow33))));
		int iSlow44 = std::min<int>(iConst5, std::max<int>(0, (iSlow38 + 2)));
		float fSlow45 = (fSlow30 * fSlow31);
		float fSlow46 = (0.166666672f * (fSlow45 * (0.0f - fSlow33)));
		int iSlow47 = std::min<int>(iConst5, std::max<int>(0, (iSlow38 + 3)));
		float fSlow48 = (0.0416666679f * (fSlow45 * fSlow32));
		int iSlow49 = std::min<int>(iConst5, std::max<int>(0, (iSlow38 + 4)));
		int iSlow50 = std::min<int>(iConst3, std::max<int>(0, iSlow4));
		float fSlow51 = float(fEntry7);
		float fSlow52 = float(fEntry8);
		float fSlow53 = float(fEntry9);
		float fSlow54 = float(fEntry10);
		float fSlow55 = (0.00100000005f * float(fEntry11));
		int iSlow56 = (iSlow36 == 0);
		for (int i = 0; (i < count); i = (i + 1)) {
			fRec0[0] = (fSlow0 + (0.999000013f * fRec0[1]));
			float fTemp0 = (fSlow8 * ((((fSlow12 * fRec1[((IOTA - iSlow14) & 8191)]) + (fSlow16 * fRec1[((IOTA - iSlow17) & 8191)])) + (fSlow19 * fRec1[((IOTA - iSlow20) & 8191)])) + (fSlow21 * fRec1[((IOTA - iSlow22) & 8191)])));
			fRec4[0] = (fSlow23 + (0.999000013f * fRec4[1]));
			float fTemp1 = (fSlow24 * fRec1[((IOTA - iSlow25) & 8191)]);
			fRec5[0] = (fSlow26 + (0.999000013f * fRec5[1]));
			fRec6[0] = (fSlow35 + (0.999000013f * fRec6[1]));
			iVec0[0] = iSlow36;
			iRec7[0] = ((1103515245 * iRec7[1]) + 12345);
			float fTemp2 = ((fRec6[0] * (fTemp1 + fTemp0)) + (fSlow37 * float(iRec7[0])));
			fVec1[(IOTA & 2047)] = fTemp2;
			float fTemp3 = ((fTemp0 + (fRec4[0] + fTemp1)) - (fRec5[0] * float(tanhf(float(((fSlow34 * fVec1[((IOTA - iSlow39) & 2047)]) + (fSlow40 * ((((fSlow41 * fVec1[((IOTA - iSlow42) & 2047)]) + (fSlow43 * fVec1[((IOTA - iSlow44) & 2047)])) + (fSlow46 * fVec1[((IOTA - iSlow47) & 2047)])) + (fSlow48 * fVec1[((IOTA - iSlow49) & 2047)])))))))));
			fVec2[(IOTA & 2047)] = fTemp3;
			fRec3[0] = (fVec2[((IOTA - iSlow50) & 2047)] - ((fSlow51 * fRec3[1]) + (fSlow52 * fRec3[2])));
			fRec1[(IOTA & 8191)] = (((fSlow1 * fRec3[0]) + (fSlow53 * fRec3[1])) + (fSlow54 * fRec3[2]));
			float fRec2 = fTemp3;
			fRec8[0] = (fSlow55 + (0.999000013f * fRec8[1]));
			iRec9[0] = (iSlow36 + (iRec9[1] * (iVec0[1] >= iSlow36)));
			iRec10[0] = (iSlow56 * (iRec10[1] + 1));
			float fTemp4 = (0.5f * ((fRec0[0] * ((fRec1[((IOTA - 0) & 8191)] * fRec8[0]) + (fRec2 * (1.0f - fRec8[0])))) * std::max<float>(0.0f, (std::min<float>((fConst6 * float(iRec9[0])), 1.0f) - (fConst7 * float(iRec10[0]))))));
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
