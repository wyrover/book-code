/*****************

 Wesley Ferreira Marques - wesley.marques@gmail.com
 http://codevein.com

 This port was based in a inital code from Jesus Gollonet, him port Penners easing equations to C/C++:

 http://www.jesusgollonet.com/blog/2007/09/24/penner-easing-cpp/
 http://robertpenner.com/easing/

 I¥m just make a better wrapper a litlle more OOP e put some callbacks like the original Tweener
 (http://code.google.com/p/tweener/)




 **********************/

#ifndef _CPP_TWEEEN_
#define _CPP_TWEEEN_

#include <math.h>
#include <list>
#include <vector>
#include <iostream>

#ifndef PI
    #define PI 3.1415926535897932384626433832795
#endif

namespace tween
{
class Easing
{
public:
    Easing() {}
    //pure virtual
    virtual double easeIn(double t, double b , double c, double d) = 0;
    virtual double easeOut(double t, double b , double c, double d) = 0;
    virtual double easeInOut(double t, double b , double c, double d) = 0;
};

class Back : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Bounce : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Circ : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Cubic : public Easing
{

public:

    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Elastic : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Expo : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Quad : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};


class Quart : public Easing
{
public:
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Quint : public Easing
{
public :
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Sine : public Easing
{
public :
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};

class Linear : public Easing
{
public :
    double easeNone(double t, double b , double c, double d);
    double easeIn(double t, double b , double c, double d);
    double easeOut(double t, double b , double c, double d);
    double easeInOut(double t, double b , double c, double d);
};


enum {
    LINEAR,
    SINE,
    QUINT,
    QUART,
    QUAD,
    EXPO,
    ELASTIC,
    CUBIC,
    CIRC,
    BOUNCE,
    BACK
};

enum {
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT
};

//callback temp
typedef void (*TweenerCallBack)();


struct TweenerProperty {
    double *ptrValue;
    double finalValue;
    double initialValue;
};

class UILIB_API TweenerParam
{
private:
public:
    int identifier;
    float interval_;
    short transition_;
    short equation_;
    float delay_;
    float elapsed_;
    size_t total_properties_;
    bool useMilliSeconds_;
    bool started_;
    bool delayFinished_;
    bool reverse_;
    int repeat_;
    int reverseCount_;
    TweenerCallBack onCompleteCallBack_;
    TweenerCallBack onStepCallBack_;
    std::vector<TweenerProperty>  properties;
    TweenerParam()
    {
        identifier = 0,
        useMilliSeconds_ = true;
        elapsed_ = 0;
        started_ = false;
        delayFinished_ = true;
        delay_ = 0;
        total_properties_ = 0;
        onCompleteCallBack_ = 0;
        onStepCallBack_ = 0;
        repeat_ = -1;
        reverse_ = false;
        reverseCount_ = -1;
    }
    ~TweenerParam()
    {
        properties.clear();
    }
    TweenerParam(int id, float milliSec, short ptransition = EXPO, short pequation = EASE_OUT, float pdelay = 0)
    {
        identifier = id;
        interval_ = milliSec;
        transition_ = ptransition;
        equation_ = pequation;
        useMilliSeconds_ = true;
        elapsed_ = 0;
        started_ = false;
        delayFinished_ = (pdelay > 0) ? false : true;
        delay_ = pdelay;
        total_properties_ = 0;
        onCompleteCallBack_ = 0;
        onStepCallBack_ = 0;
        repeat_ = -1;
        reverse_ = false;
        reverseCount_ = -1;
    }

    void setRepeatWithReverse(int _n, bool reverse = false)
    {
        repeat_ = _n - 1;

        if (reverse) {
            reverse_ = reverse;
            reverseCount_ = repeat_ + 1;
        }
    }

    int decreaseRepeat()
    {
        float bkpValue = 0;

        if (repeat_ > -1 || reverseCount_ > -1) {
            elapsed_ = 0;
            started_ = false;

            if (total_properties_ > 0) {
                for (size_t i = 0 ; i < total_properties_; i++) {
                    TweenerProperty prop = properties[i];

                    if (reverseCount_ <= repeat_ && reverse_) {
                        //destroca
                        bkpValue = properties[i].finalValue;
                        properties[i].finalValue = properties[i].initialValue;
                        properties[i].initialValue = bkpValue;
                    } else if (reverse_) {
                        //troca
                        bkpValue = properties[i].initialValue;
                        properties[i].initialValue = properties[i].finalValue;
                        properties[i].finalValue = bkpValue;
                    }

                    *(prop.ptrValue) = properties[i].initialValue;
                }
            }

            if (reverseCount_ <= repeat_) {
                repeat_--;
            } else {
                reverseCount_--;
            }
        }

        return repeat_;
    }

    void addProperty(double *valor, double valorFinal)
    {
        TweenerProperty prop = {valor, valorFinal, *valor};
        properties.push_back(prop);
        total_properties_ = properties.size();
    }

    void setUseMilliSeconds(bool use)
    {
        useMilliSeconds_ = use;
    }

    void cleanProperties()
    {
        properties.clear();
    }

    bool operator==(const TweenerParam& p)
    {
        bool equal = false;

        if ((identifier == p.identifier) && (interval_ == p.interval_) && (transition_ == p.transition_) && (equation_ == p.equation_)) {
            for (size_t i = 0 ; i < p.total_properties_; i++) {
                if (properties[i].initialValue == p.properties[i].initialValue ||
                    properties[i].finalValue == p.properties[i].finalValue) {
                    equal = true;
                    break;
                }
            }
        }

        return equal;
    }

    bool operator!=(const TweenerParam& p)
    {
        return ! this->operator ==(p);
    }
};

//listener geral
class UILIB_API TweenerListener
{
public:
    virtual void onStart(const TweenerParam& param) = 0;
    virtual void onStep(const TweenerParam& param) = 0 ;
    virtual void onComplete(const TweenerParam& param) = 0 ;
};


class UILIB_API Tweener
{
public:
    Tweener()
    {
        static Linear fLinear;
        static Sine fSine;
        static Quint fQuint;
        static Quart fQuart;
        static Quad  fQuad;
        static Expo fExpo;
        static Elastic fElastic;
        static Cubic fCubic;
        static Circ fCirc;
        static Bounce fBounce;
        static Back fBack;
        this->funcs[LINEAR] = &fLinear;
        this->funcs[SINE]  = &fSine;
        this->funcs[QUINT] = &fQuint;
        this->funcs[QUART] = &fQuart;
        this->funcs[QUAD] = &fQuad;
        this->funcs[EXPO] = &fExpo;
        this->funcs[ELASTIC] = &fElastic;
        this->funcs[CUBIC] = &fCubic;
        this->funcs[CIRC] =  &fCirc;
        this->funcs[BOUNCE] =  &fBounce;
        this->funcs[BACK] =  &fBack;
        lastTime_ = 0;
    }
    void addTween(TweenerParam& param);
    void removeTween(TweenerParam  *param);
    void removeAllTween();
    void addListener(TweenerListener *listener) ;
    void removeListener(TweenerListener *listener) ;
    void setFunction(short funcEnum);
    void step(long currentMillis) ;

protected:
    double runEquation(int transition, int equation, double t, double b , double c, double d);
    void dispatchEvent(const TweenerParam& param, short eventType);

protected :
    enum {ON_START, ON_STEP, ON_COMPLETE};
    short currentFunction ;
    Easing *funcs[11];
    long lastTime_;
    size_t total_tweens_ ;

    std::list<TweenerParam> tweens;
    std::vector<TweenerListener*> listeners;

    std::list<TweenerParam>::iterator tweensIT;
    std::list<TweenerListener*>::iterator listenerIT;
};

}

#endif
