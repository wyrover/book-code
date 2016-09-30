// Calc.cpp : Implementation of CCalc

#include "stdafx.h"
#include "Calc.h"


// CCalc


class CFoo
{
    int m_n;
    double m_d;
    void DoWork(int a) {}
};

class CAnimal {};
class CDog : public CAnimal {};


STDMETHODIMP CCalc::Add(DOUBLE Op1, DOUBLE Op2, DOUBLE* Result)
{
    CAutoPtr<CFoo> spFoo(new CFoo);
    CAutoPtr<CAnimal> spAnimal(new CAnimal);
    CDog* pDog = new CDog();
    spAnimal = CAutoPtr<CDog>(pDog);
    return S_OK;
}
