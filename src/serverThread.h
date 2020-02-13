#pragma once
#include "ofMain.h"
#include "ofxNetwork.h"

class serverThread: public ofThread {
    public:
        serverThread();
        ~serverThread();
        void update();
        void sendMsg(string & msg);
        
private:
    ofxTCPServer TCP;
    void threadedFunction();
    ofThreadChannel<string> toSend;
    ofThreadChannel<string> received;
    string msg;
    bool newMsg;
    bool newMsgRx;
};
