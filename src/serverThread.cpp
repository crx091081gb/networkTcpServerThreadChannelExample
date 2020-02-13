#include "serverThread.h"
#include "ofConstants.h"

serverThread::serverThread()
:newMsgRx(false){
    // setup the server to listen on 11999
    ofxTCPSettings settings(11999);

    // set other options
    //settings.blocking = false;
    //settings.reuse = true;
    //settings.messageDelimiter = "\n";

    TCP.setup(settings);

    // optionally set the delimiter to something else.  The delimiter in the client and the server have to be the same, default being [/TCP]
    TCP.setMessageDelimiter("\n");
    
    startThread();
}

serverThread::~serverThread() {
    toSend.close();
    received.close();
    waitForThread(true);
}

void serverThread::sendMsg(string & msg) {
    toSend.send(msg);
}

void serverThread::update() {
    //check incoming messages to send
    newMsgRx = false;
    while(received.tryReceive(msg)) {
        newMsgRx = true;
    }
    if(newMsgRx){
        cout << msg << endl;
    }
    
}

void serverThread::threadedFunction() {
    string msgTX;
    
    while(toSend.receive(msgTX)) {
        for(int i = 0; i < TCP.getLastID(); i++){
            if( !TCP.isClientConnected(i) ) continue;
            TCP.send(i, msgTX + ofToString(TCP.getClientPort(i)) );
        }
        
        for(unsigned int i = 0; i < (unsigned int)TCP.getLastID(); i++){

            if( !TCP.isClientConnected(i) )continue;
            //cout << "got this far" << endl;
            // get the ip and port of the client
            string port = ofToString( TCP.getClientPort(i) );
            string ip   = TCP.getClientIP(i);
            string info = "client "+ofToString(i)+" -connected from "+ip+" on port: "+port;

            // receive all the available messages, separated by \n
            // and keep only the last one
            string str;
            string tmp;
            do{
                str = tmp;
                tmp = TCP.receive(i);
            }while(tmp!="");

            // if there was a message set it to the corresponding client
            if(str.length() > 0){
                received.send(std::move(str));
            }
        }
    }
}
