#include "natOscMocapDataReceiver.h"
#include <assert.h>
/***** nagayoshi tool (nat) *****/
namespace nat{

OscMocapDataReceiver::OscMocapDataReceiver()
{
	m_templateNum = 0;
	m_templateMarkerNum = 0;
	m_unidentifiedMarkerNum = 0;
	m_isInitialized = false;
	m_templateMarkerPos = NULL;
}

OscMocapDataReceiver::~OscMocapDataReceiver()
{
	for(int i=0; i<m_templateNum; i++){
		for(int j=0; j<m_templateMarkerNum[i]; j++){
			delete[] m_templateMarkerPos[i][j];
		}
		delete[] m_templateMarkerPos[i];
	}
	delete[] m_templateMarkerPos;
	delete[] m_templateMarkerNum;
	for(int i=0; i<MS_UNIDENTIFIED_MARKER_MAX_NUM; i++){
		delete[] m_unidentifiedMarkerPos[i];
	}
	delete[] m_unidentifiedMarkerPos;
}

void OscMocapDataReceiver::setup(int _port)
{
	m_receiver.setup(_port);
	
	m_unidentifiedMarkerPos = new float*[MS_UNIDENTIFIED_MARKER_MAX_NUM];
	for(int i=0; i<MS_UNIDENTIFIED_MARKER_MAX_NUM; i++){
		m_unidentifiedMarkerPos[i] = new float[3];
	}
}


float** OscMocapDataReceiver::getUnidentifiedMarkerData()
{
	m_unidentifiedMarkerNum = 0;
	if(m_message.getAddress() == "/Unidentified_Num/"){
			m_unidentifiedMarkerNum = m_message.getArgAsInt32(0);	//Unidentified
			m_receiver.getNextMessage(&m_message);
	}
	for(int i=0; i<m_unidentifiedMarkerNum; i++){
		if(m_message.getAddress() == "/Unidentified/" + intToString(i)){
			m_unidentifiedMarkerPos[i][0] = m_message.getArgAsFloat(0);
			m_unidentifiedMarkerPos[i][1] = m_message.getArgAsFloat(1);
			m_unidentifiedMarkerPos[i][2] = m_message.getArgAsFloat(2);
		}
	}
	return m_unidentifiedMarkerPos;
}

float** OscMocapDataReceiver::getTemplateMarkerData(int _id, std::string _tempName)
{
	for(int i=0; i<m_templateMarkerNum[_id]; i++){
		if(m_message.getAddress() == _tempName + intToString(i)){
			m_templateMarkerPos[_id][i][0] = m_message.getArgAsFloat(0);
			m_templateMarkerPos[_id][i][1] = m_message.getArgAsFloat(1);
			m_templateMarkerPos[_id][i][2] = m_message.getArgAsFloat(2);
		}
	}
	return m_templateMarkerPos[_id];
}

bool OscMocapDataReceiver::hasWaitingMessages()
{
	if(isInitialized()){
		if(m_receiver.hasWaitingMessages()){
			m_receiver.getNextMessage(&m_message);
		}
		return m_receiver.hasWaitingMessages();
	}else{
		return false;
	}
}

bool OscMocapDataReceiver::isInitialized()
{
	if(!m_isInitialized){
		while( m_receiver.hasWaitingMessages()){
			m_receiver.getNextMessage(&m_message);
			m_templateNum = m_message.getArgAsInt32(0);
			m_templateMarkerPos = new float**[m_templateNum];
			m_templateMarkerNum = new int[m_templateNum];
			for(int i=0; i<m_templateNum; i++){
				m_receiver.getNextMessage(&m_message);
				m_templateMarkerNum[i] = m_message.getArgAsInt32(0);
				m_templateMarkerPos[i] = new float*[m_templateMarkerNum[i]];
				for(int j=0; j<m_templateMarkerNum[i]; j++){
					m_templateMarkerPos[i][j] = new float[3];
				}
			}
			return m_isInitialized = true;
		}
	}else{
		return m_isInitialized;
	}
}
    
    string OscMocapDataReceiver::intToString(int number)
    {
        stringstream ss;
        ss << number;
        return ss.str();
    }

}