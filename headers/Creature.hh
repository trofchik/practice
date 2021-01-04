
#pragma once

class Creature { 

private: 
    int res = 0, att = 0, blockStr = 0, blockCh = 0, dodge = 0, parryCh = 0;
    int trueRes = 0;

public:
    Creature(const int power = 0);
    ~Creature();
    
    void show();
    
    // Setters
    void setTrueRes(int newTrueRes);
    void setRes(int newRes);
    void setAtt(int newAtt);
    void setBlockStr(int newBlockStr);
    void setBlockCh(int newBlockCh);
    void setDodge(int newDodge);
    
    // Getters
    int setParryCh(int newParryCh);
    int getTrueRes();
    int getRes();
    int getAtt();
    int getBlockStr();
    int getBlockCh();
    int getDodge();
    int getParryCh();
};
