
#pragma once
#include <iostream>

class Content
{
protected:
    unsigned int priority;
    
public:
    Content() {};
    virtual ~Content() {};
    
    virtual void show() = 0;
    unsigned int getPriority() { return this->priority; };
    
};
