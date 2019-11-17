#pragma once
#include "Geo4.h"

class GUIWings;

class WingKeyframesResource {
public:

	WingKeyframesResource();
	~WingKeyframesResource();

	typedef struct {
		float r;
		float g;
		float b;
	}sColor;
	typedef struct {
		sColor colors[7];
	}sWing;
	typedef struct {
		float t;
		sWing data[12];
	}sKeyframe;

	typedef struct {
		unsigned int	numFrames;
		float			tStart;
		float			tEnd;
		sKeyframe* keyframes;
	}sFileData;

	void SaveToFile(string, vector<WingsKeyframe*> &, float, float);
	void ReadFromFile(string, vector<WingsKeyframe*> &, float &tstart, float &tend);
	void InsertFromFile(string, vector<WingsKeyframe*> &, float &tstart, float &tend, float dt, bool expand);

	void mergeKeyframes(vector<WingsKeyframe*> &a, vector<WingsKeyframe*> &b, vector<WingsKeyframe*> &out);

	bool getInterpolatedKeyframe(vector<WingsKeyframe*> &, float t, WingsKeyframe& keyframe);

	static int keyframeMappingOut[7];
	static int keyframeMappingIn[7];

};
