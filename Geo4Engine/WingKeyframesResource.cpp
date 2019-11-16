
#include "Geo4.h"


int WingKeyframesResource::keyframeMapping[] = {
		0,	//S1
		1,	//S2
		2,	//A1L
		3,	//A1R
		4,	//S3
		5,	//A2L
		6	//A2R
	};

WingKeyframesResource::WingKeyframesResource()
{
}


WingKeyframesResource::~WingKeyframesResource()
{
}

void WingKeyframesResource::SaveToFile(string name, vector<WingsKeyframe*> & keyframes, float tmin, float tmax)
{
	sFileData data;
	data.numFrames = (unsigned int)keyframes.size();
	data.keyframes = new sKeyframe[keyframes.size()];
	for (int k = 0; k < keyframes.size(); k++) {
		for (int w = 0; w < 12; w++) {
			for (int c = 0; c < 7; c++) {
				data.keyframes[k].t = keyframes[k]->t;
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMapping[c]].g = keyframes[k]->colors[w][c].x;
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMapping[c]].r = keyframes[k]->colors[w][c].y;
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMapping[c]].b = keyframes[k]->colors[w][c].z;
			}
		}
	}

	FILE *fout = 0;
	fopen_s(&fout, name.c_str(), "wb");

	if (!fout)return;

	fwrite(&data.numFrames, sizeof(unsigned int), 1, fout);
	fwrite(&tmin, sizeof(float), 1, fout);
	fwrite(&tmax, sizeof(float), 1, fout);

	for (unsigned int k = 0; k < data.numFrames; k++) {
		fwrite(&data.keyframes[k], sizeof(sKeyframe), 1, fout);
	}

	fclose(fout);
}

void WingKeyframesResource::ReadFromFile(string name, vector<WingsKeyframe*> &keyframes, float &tstart, float &tend)
{
	sFileData data;

	FILE* fin = 0;

	fopen_s(&fin, name.c_str(), "rb");

	if (!fin)return;

	fread(&data.numFrames, sizeof(unsigned int), 1, fin);
	fread(&data.tStart, sizeof(float), 1, fin);
	fread(&data.tEnd, sizeof(float), 1, fin);

	data.keyframes = new sKeyframe[data.numFrames];

	for (unsigned int k = 0; k < data.numFrames; k++) {
		fread(&data.keyframes[k], sizeof(sKeyframe), 1, fin);
	}

	fclose(fin);

	keyframes.clear();

	for (unsigned int k = 0; k < data.numFrames; k++) {
		WingsKeyframe* kf = new WingsKeyframe();
		kf->t = data.keyframes[k].t;

		for (int w = 0; w < 12; w++) {
			for (int c = 0; c < 7; c++) {
				kf->colors[w][WingKeyframesResource::keyframeMapping[c]].x = data.keyframes[k].data[w].colors[c].g;
				kf->colors[w][WingKeyframesResource::keyframeMapping[c]].y = data.keyframes[k].data[w].colors[c].r;
				kf->colors[w][WingKeyframesResource::keyframeMapping[c]].z = data.keyframes[k].data[w].colors[c].b;
			}
		}
		keyframes.push_back(kf);
	}

	tstart = data.tStart;
	tend = data.tEnd;
}

void WingKeyframesResource::InsertFromFile(string name, vector<WingsKeyframe*> &keyframes, float &tstart, float &tend, float dt, bool expand)
{
	sFileData data;

	FILE* fin = 0;

	fopen_s(&fin, name.c_str(), "rb");

	if (!fin)return;

	fread(&data.numFrames, sizeof(unsigned int), 1, fin);
	fread(&data.tStart, sizeof(float), 1, fin);
	fread(&data.tEnd, sizeof(float), 1, fin);

	data.keyframes = new sKeyframe[data.numFrames];

	for (unsigned int k = 0; k < data.numFrames; k++) {
		fread(&data.keyframes[k], sizeof(sKeyframe), 1, fin);
	}

	fclose(fin);

	float expandBy = data.tEnd - data.tStart;

	if (expand) {
		for (unsigned int a = 0; a < keyframes.size(); a++) {
			if (keyframes[a]->t > dt)keyframes[a]->t += expandBy;
		}
		tend = tend + expandBy;
	}

	for (unsigned int k = 0; k < data.numFrames; k++) {
		WingsKeyframe* kf = new WingsKeyframe();
		kf->t = data.keyframes[k].t + dt - data.tStart;

		for (int w = 0; w < 12; w++) {
			for (int c = 0; c < 7; c++) {
				kf->colors[w][c].x = data.keyframes[k].data[w].colors[c].g;
				kf->colors[w][c].y = data.keyframes[k].data[w].colors[c].r;
				kf->colors[w][c].z = data.keyframes[k].data[w].colors[c].b;
			}
		}
		keyframes.push_back(kf);
	}

}
