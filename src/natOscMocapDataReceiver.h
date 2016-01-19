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
	/***** �}�[�J�[�̈ʒu�ۑ��z�� *****/
	float ***m_templateMarkerPos;			//m_templateMarkerPos[�e���v���[�g��][�}�[�J�[��]�̓��I3�����z��
	float **m_unidentifiedMarkerPos;		//m_unidentifiedMarkerPos[�ő�}�[�J�[��]�̓��I2�����z��
	int m_templateNum;	//�e���v���[�g�̐�
	int *m_templateMarkerNum;	//�e���v���[�g�̃}�[�J�[�̐�
	int m_unidentifiedMarkerNum; //Unidentified�}�[�J�[�̐�
	bool m_isInitialized; //�������m�F
	ofxOscMessage m_message;
    string intToString(int number);
};

}