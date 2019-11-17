
#include "Geo4.h"


int WingKeyframesResource::keyframeMappingOut[] = {
		0,	//S1
		1,	//S2
		2,	//A1L
		3,	//A1R
		4,	//S3
		5,	//A2L
		6	//A2R
	};
int WingKeyframesResource::keyframeMappingIn[] = {
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
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMappingOut[c]].g = keyframes[k]->colors[w][c].x;
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMappingOut[c]].r = keyframes[k]->colors[w][c].y;
				data.keyframes[k].data[w].colors[WingKeyframesResource::keyframeMappingOut[c]].b = keyframes[k]->colors[w][c].z;
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
				kf->colors[w][WingKeyframesResource::keyframeMappingIn[c]].x = data.keyframes[k].data[w].colors[c].g;
				kf->colors[w][WingKeyframesResource::keyframeMappingIn[c]].y = data.keyframes[k].data[w].colors[c].r;
				kf->colors[w][WingKeyframesResource::keyframeMappingIn[c]].z = data.keyframes[k].data[w].colors[c].b;
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

	vector<WingsKeyframe*> keyf;

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
		keyf.push_back(kf);
		//keyframes.push_back(kf);
	}
	
	vector<WingsKeyframe*> keyftmp;

	keyftmp.reserve(keyframes.size());
	keyftmp.insert(keyftmp.end(), keyframes.begin(), keyframes.end());

	keyframes.clear();
	mergeKeyframes(keyftmp, keyf, keyframes);
}

void WingKeyframesResource::mergeKeyframes(vector<WingsKeyframe*> &a, vector<WingsKeyframe*> &b, vector<WingsKeyframe*> &out)
{
	vector<WingsKeyframe*> all;
	all.reserve(a.size() + b.size());
	all.insert(all.end(), a.begin(), a.end());
	all.insert(all.end(), b.begin(), b.end());
	sort(all.begin(), all.end(), compare_keyframes);

	for (int i = 0; i < all.size(); i++)
	{
		WingsKeyframe kfa, kfb;

		bool ab = getInterpolatedKeyframe(a, all[i]->t, kfa);
		bool bb = getInterpolatedKeyframe(b, all[i]->t, kfb);

		WingsKeyframe* kf = new WingsKeyframe();
		kf->add(kfa);
		kf->add(kfb);
		//kf->mult(kfb);

		kf->t = all[i]->t;
		out.push_back(kf);
		/*
		cout << all[i]->t << " - "<< ab<<","<<bb<<" / "
			<< Utils::VectorToString(kfa.colors[0][3])
			<<" - "
			<< Utils::VectorToString(kfb.colors[0][6])
			<< " = "
			<< Utils::VectorToString(kf->colors[0][6])
			<< endl;
		*/	
	}

//	cout << out.size() << endl;

	sort(out.begin(), out.end(), compare_keyframes);
}

bool WingKeyframesResource::getInterpolatedKeyframe(vector<WingsKeyframe*> &keyframes, float t, WingsKeyframe& keyframe)
{
	if (keyframes.size() < 2)return false;

	for (int i = 0; i < keyframes.size() - 1; i++) {
		if (keyframes[i]->t <= t && keyframes[i + 1]->t >= t) {

			float dt = (t - keyframes[i]->t) / (keyframes[i + 1]->t - keyframes[i]->t);

			keyframe = keyframes[i]->getInterpolated(1.0f - dt, *keyframes[i + 1]);

			return true;
		}
	}
	return false;
}