/*****************

 Wesley Ferreira Marques - wesley.marques@gmail.com
 http://codevein.com

 This port was based in a inital code from Jesus Gollonet, him port Penners easing equations to C/C++:

 http://www.jesusgollonet.com/blog/2007/09/24/penner-easing-cpp/
 http://robertpenner.com/easing/

 I´m just make a better wrapper a litlle more OOP e put some callbacks like the original Tweener
 (http://code.google.com/p/tweener/)





 **********************/
#include "stdafx.h"
#include "CppTweener.h"

namespace tween
{
/***** LINEAR ****/
double Linear::easeNone(double t, double b , double c, double d)
{
    return c * t / d + b;
}
double Linear::easeIn(double t, double b , double c, double d)
{
    return c * t / d + b;
}
double Linear::easeOut(double t, double b , double c, double d)
{
    return c * t / d + b;
}

double Linear::easeInOut(double t, double b , double c, double d)
{
    return c * t / d + b;
}

/***** SINE ****/

double Sine::easeIn(double t, double b , double c, double d)
{
    return -c * cos(t / d * (PI / 2)) + c + b;
}
double Sine::easeOut(double t, double b , double c, double d)
{
    return c * sin(t / d * (PI / 2)) + b;
}

double Sine::easeInOut(double t, double b , double c, double d)
{
    return -c / 2 * (cos(PI * t / d) - 1) + b;
}

/**** Quint ****/

double Quint::easeIn(double t, double b , double c, double d)
{
    return c * (t /= d) * t * t * t * t + b;
}
double Quint::easeOut(double t, double b , double c, double d)
{
    return c * ((t = t / d - 1) * t * t * t * t + 1) + b;
}

double Quint::easeInOut(double t, double b , double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t * t * t + b;

    return c / 2 * ((t -= 2) * t * t * t * t + 2) + b;
}

/**** Quart ****/
double Quart::easeIn(double t, double b , double c, double d)
{
    return c * (t /= d) * t * t * t + b;
}
double Quart::easeOut(double t, double b , double c, double d)
{
    return -c * ((t = t / d - 1) * t * t * t - 1) + b;
}

double Quart::easeInOut(double t, double b , double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t * t + b;

    return -c / 2 * ((t -= 2) * t * t * t - 2) + b;
}

/**** Quad ****/
double Quad::easeIn(double t, double b , double c, double d)
{
    return c * (t /= d) * t + b;
}
double Quad::easeOut(double t, double b , double c, double d)
{
    return -c * (t /= d) * (t - 2) + b;
}

double Quad::easeInOut(double t, double b , double c, double d)
{
    if ((t /= d / 2) < 1) return ((c / 2) * (t * t)) + b;

    return -c / 2 * (((t - 2) * (--t)) - 1) + b;
    /*
     originally return -c/2 * (((t-2)*(--t)) - 1) + b;

     I've had to swap (--t)*(t-2) due to diffence in behaviour in
     pre-increment operators between java and c++, after hours
     of joy
     */
}

/**** Expo ****/

double Expo::easeIn(double t, double b , double c, double d)
{
    return (t == 0) ? b : c * pow(2, 10 * (t / d - 1)) + b;
}
double Expo::easeOut(double t, double b , double c, double d)
{
    return (t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b;
}

double Expo::easeInOut(double t, double b , double c, double d)
{
    if (t == 0) return b;

    if (t == d) return b + c;

    if ((t /= d / 2) < 1) return c / 2 * pow(2, 10 * (t - 1)) + b;

    return c / 2 * (-pow(2, -10 * --t) + 2) + b;
}


/****  Elastic ****/

double Elastic::easeIn(double t, double b , double c, double d)
{
    if (t == 0) return b;

    if ((t /= d) == 1) return b + c;

    double p = d * .3f;
    double a = c;
    double s = p / 4;
    double postFix = a * pow(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
    return -(postFix * sin((t * d - s) * (2 * PI) / p)) + b;
}

double Elastic::easeOut(double t, double b , double c, double d)
{
    if (t == 0) return b;

    if ((t /= d) == 1) return b + c;

    double p = d * .3f;
    double a = c;
    double s = p / 4;
    return (a * pow(2, -10 * t) * sin((t * d - s) * (2 * PI) / p) + c + b);
}

double Elastic::easeInOut(double t, double b , double c, double d)
{
    if (t == 0) return b;

    if ((t /= d / 2) == 2) return b + c;

    double p = d * (.3f * 1.5f);
    double a = c;
    double s = p / 4;

    if (t < 1) {
        double postFix = a * pow(2, 10 * (t -= 1)); // postIncrement is evil
        return -.5f * (postFix * sin((t * d - s) * (2 * PI) / p)) + b;
    }

    double postFix =  a * pow(2, -10 * (t -= 1)); // postIncrement is evil
    return postFix * sin((t * d - s) * (2 * PI) / p) * .5f + c + b;
}

/****  Cubic ****/
double Cubic::easeIn(double t, double b , double c, double d)
{
    return c * (t /= d) * t * t + b;
}
double Cubic::easeOut(double t, double b , double c, double d)
{
    return c * ((t = t / d - 1) * t * t + 1) + b;
}

double Cubic::easeInOut(double t, double b , double c, double d)
{
    if ((t /= d / 2) < 1) return c / 2 * t * t * t + b;

    return c / 2 * ((t -= 2) * t * t + 2) + b;
}

/*** Circ ***/

double Circ::easeIn(double t, double b , double c, double d)
{
    return -c * (sqrt(1 - (t /= d) * t) - 1) + b;
}
double Circ::easeOut(double t, double b , double c, double d)
{
    return c * sqrt(1 - (t = t / d - 1) * t) + b;
}

double Circ::easeInOut(double t, double b , double c, double d)
{
    if ((t /= d / 2) < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + b;

    return c / 2 * (sqrt(1 - t * (t -= 2)) + 1) + b;
}

/****  Bounce ****/
double Bounce::easeIn(double t, double b , double c, double d)
{
    return c - easeOut(d - t, 0, c, d) + b;
}
double Bounce::easeOut(double t, double b , double c, double d)
{
    if ((t /= d) < (1 / 2.75f)) {
        return c * (7.5625f * t * t) + b;
    } else if (t < (2 / 2.75f)) {
        double postFix = t -= (1.5f / 2.75f);
        return c * (7.5625f * (postFix) * t + .75f) + b;
    } else if (t < (2.5 / 2.75)) {
        double postFix = t -= (2.25f / 2.75f);
        return c * (7.5625f * (postFix) * t + .9375f) + b;
    } else {
        double postFix = t -= (2.625f / 2.75f);
        return c * (7.5625f * (postFix) * t + .984375f) + b;
    }
}

double Bounce::easeInOut(double t, double b , double c, double d)
{
    if (t < d / 2) return easeIn(t * 2, 0, c, d) * .5f + b;
    else return easeOut(t * 2 - d, 0, c, d) * .5f + c * .5f + b;
}

/**** Back *****/

double Back::easeIn(double t, double b , double c, double d)
{
    double s = 1.70158f;
    double postFix = t /= d;
    return c * (postFix) * t * ((s + 1) * t - s) + b;
}
double Back::easeOut(double t, double b , double c, double d)
{
    double s = 1.70158f;
    return c * ((t = t / d - 1) * t * ((s + 1) * t + s) + 1) + b;
}

double Back::easeInOut(double t, double b , double c, double d)
{
    double s = 1.70158f;

    if ((t /= d / 2) < 1) return c / 2 * (t * t * (((s *= (1.525f)) + 1) * t - s)) + b;

    double postFix = t -= 2;
    return c / 2 * ((postFix) * t * (((s *= (1.525f)) + 1) * t + s) + 2) + b;
}


//implementation Tweener Class*********************************************************

double Tweener::runEquation(int transition, int equation, double t, double b , double c, double d)
{
    double result;

    if (equation == EASE_IN) {
        result = funcs[transition]->easeIn(t, b, c, d);
    } else if (equation == EASE_OUT) {
        result = funcs[transition]->easeOut(t, b, c, d);
    } else if (equation == EASE_IN_OUT) {
        result = funcs[transition]->easeInOut(t, b, c, d);
    }

    return result;
}

void Tweener::dispatchEvent(const TweenerParam& param, short eventType)
{
    std::vector<TweenerListener*>::iterator itr;

    for (itr = listeners.begin(); itr != listeners.end(); ++itr) {
        TweenerListener *listener = *itr;

        switch (eventType) {
        case ON_START:
            listener->onStart(param);
            break;

        case ON_STEP:
            listener->onStep(param);
            break;

        case ON_COMPLETE:
            listener->onComplete(param);
            break;

        default:
            std::cout << "Event not found!!";
            break;
        }
    }
}


void Tweener::addTween(TweenerParam& param)
{
    param.elapsed_ = 0;

    if (param.delay_ > 0) {
        param.delayFinished_ = false;
    }

    if (param.total_properties_ > 0) {
        for (size_t i = 0 ; i < param.total_properties_; i++) {
            TweenerProperty& prop = param.properties[i];
            param.properties[i].initialValue = *(prop.ptrValue);
        }
    }

    tweens.push_back(param);
    total_tweens_ = tweens.size();
}

void Tweener::removeTween(TweenerParam  *param)
{
    total_tweens_ = tweens.size();
    tweensIT = tweens.begin();

    for (size_t i = 0; i <  total_tweens_; i++/*,tweensIT++*/) {
        if ((*param) == (*tweensIT)) {
            (*tweensIT).cleanProperties();
            //tweens.erase(tweensIT);
            tweens.erase(tweensIT++);
            //std::cout<<"\n-Tween Removed";
            --total_tweens_;
            break;
        } else {
            ++tweensIT;
        }
    }
}


void Tweener::removeAllTween()
{
    tweens.clear();
    total_tweens_ = tweens.size();
    lastTime_ = 0;
}

void Tweener::addListener(TweenerListener *listener)
{
    listeners.push_back(listener);
}


void Tweener::removeListener(TweenerListener *listener)
{
    std::vector<TweenerListener *>::iterator itr;

    for (itr = listeners.begin(); itr != listeners.end(); ++itr) {
        TweenerListener* p = (*itr);

        if (listener == p) {
            listeners.erase(itr);
            break;
        }
    }
}

void Tweener::setFunction(short funcEnum)
{
    if (funcEnum > -1 && funcEnum <= 11) {
        currentFunction = funcEnum;
    }
};

void Tweener::step(long currentMillis)
{
    //fix thanks for Etienne Mouchel
    if (lastTime_ == 0)
        lastTime_ = currentMillis;

    total_tweens_ = tweens.size();
    double elapsed = 0 ;
    double interval = 0;
    long dif = (currentMillis - lastTime_);
    std::list<TweenerParam>::iterator itr;

    for (itr = tweens.begin();  itr != tweens.end(); ++itr) {
        TweenerParam& param = (*itr);

        if (!param.started_) {
            dispatchEvent(param, ON_START);
            param.started_ = true;
        }

        dispatchEvent(param, ON_STEP);

        if (param.onStepCallBack_) {
            param.onStepCallBack_();
        }

        if (param.useMilliSeconds_) {
            param.elapsed_ += dif;
            elapsed = param.elapsed_ ;
        } else {
            param.elapsed_++;
            elapsed = param.elapsed_;
        }

        if (param.delayFinished_) {
            interval = param.interval_ ;

            if (elapsed < interval  && param.total_properties_ > 0) {
                //if (param.elapsed_ < param.interval_  ) {
                for (size_t i  = 0 ; i < param.total_properties_; ++i) {
                    TweenerProperty& prop = param.properties[i];

                    if (prop.ptrValue != NULL) {
                        *(prop.ptrValue) = runEquation(param.transition_, param.equation_,
                                                       elapsed, prop.initialValue, (prop.finalValue - prop.initialValue), interval);
                    }
                }

                // }
            } else {
                if (param.decreaseRepeat() < 0) {
                    //garante o valor final
                    /*for (unsigned int i =0 ; i < param.total_properties; i++ ) {
                        TweenerProperty prop = param.properties[i];
                        if (prop.ptrValue != NULL ) {
                            *(prop.ptrValue) = prop.finalValue;
                        }
                    }*/
                    dispatchEvent(param, ON_COMPLETE);

                    if (param.onCompleteCallBack_) {
                        param.onCompleteCallBack_();
                        param.onCompleteCallBack_ = 0;
                        param.onStepCallBack_ = 0;
                    }

                    removeTween(&param);
                    tweensIT = tweens.begin();

                    if (total_tweens_ == 0) { //if the tweener is empty again this way for the next param adding the dif won't be huge and it won't skip the tweening
                        lastTime_ = 0;
                        tweensIT = tweens.end();
                        break;
                    }
                }
            }
        } else  if ((!param.delayFinished_) && (param.elapsed_ > param.delay_)) {
            param.delayFinished_ = true;
            param.elapsed_ = 0;
        }
    }

    if (lastTime_ != 0)
        lastTime_ = currentMillis;
};

}
