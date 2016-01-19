#pragma once
#include "ofxOsc.h"
#define MS_UNIDENTIFIED_MARKER_MAX_NUM 20
/***** nagayoshi tool (nat) *****/
namespace nat{

class OscMocapDataReceiver
{
public:
	OscMocapDataReceiver(void);
	~OscMocapDataReceiver(void);
	void setup(int _port);
	float** getTemplateMarkerData(int _id, std::string _tempName);
	float** getUnidentifiedMarkerData();
	bool hasWaitingMessages();
	bool isInitialized();
	ofxOscReceiver m_receiver;
	/***** マーカーの位置保存配列 *****/
	float ***m_templateMarkerPos;			//m_templateMarkerPos[テンプレート数][マーカー数]の動的3次元配列
	float **m_unidentifiedMarkerPos;		//m_unidentifiedMarkerPos[最大マーカー数]の動的2次元配列
	int m_templateNum;	//テンプレートの数
	int *m_templateMarkerNum;	//テンプレートのマーカーの数
	int m_unidentifiedMarkerNum; //Unidentifiedマーカーの数
	bool m_isInitialized; //初期化確認
	ofxOscMessage m_message;
    string intToString(int number);
};

}