#ifndef __CLASSFACTORY_H
#define __CLASSFACTORY_H


typedef HRESULT (PASCAL *dllgco_t)(REFCLSID clsid, REFIID riid, LPVOID * ppv);

//////////////////////////////////////////////////////////
//CSpyClassFactory
//---------------------

class CSpyClassFactory : public CComClassFactory
    {
    TCHAR m_szDLL[1024];
    CLSID m_clsid;

	BEGIN_COM_MAP(CComClassFactory)
		COM_INTERFACE_ENTRY(IClassFactory)
	END_COM_MAP()

    //We take as an argument the original pathname for this class.
    void Init(REFCLSID rclsid, LPCTSTR szPath)
        {
        ATLASSERT(lstrlen(szPath) < 1024);
        ::lstrcpy(m_szDLL, szPath);

        m_clsid = rclsid;
        }

	STDMETHOD(CreateInstance)(LPUNKNOWN pUnkOuter, REFIID riid, void** ppvObj)
        {
        HRESULT hr = REGDB_E_CLASSNOTREG;

        //load up the original DLL
        HMODULE h = ::LoadLibrary(m_szDLL);
        if(h != NULL)
            {
            dllgco_t p = (dllgco_t)GetProcAddress(h, "DllGetClassObject");
            if(p != NULL)
                {
                IClassFactory * pCF;
                hr = (*p)(m_clsid, IID_IClassFactory, (void**)&pCF);
                if(SUCCEEDED(hr))
                    {
                    //Create the raw object, and note that we always pass NULL as pUnkOuter.  We
                    //use that parameter in the wrapper.
                    IUnknown * pUnkInner;
                    hr = pCF->CreateInstance(NULL, IID_IUnknown, (void**)&pUnkInner);

                    if(SUCCEEDED(hr))
                        {
                        //Now lets see if we can concoct a name for this object...
                        LPOLESTR lpszProgId;
                        if(FAILED(::ProgIDFromCLSID(m_clsid, &lpszProgId)))
                            {
                            ::StringFromCLSID(m_clsid, &lpszProgId);
                            }

                        ATLTRACE("Creating %s object of type %S\n", (pUnkOuter? "an aggregated" : "a standalone"), lpszProgId);
                        hr = _Module.WrapObject(pUnkOuter,pUnkInner,IDENTITY_COCLASS,lpszProgId,riid, ppvObj);

                        CoTaskMemFree(lpszProgId);

                        //_Module.OnCoclassCreated(NULL);
                        }
                    pCF->Release();
                    }
                }
             if(FAILED(hr))
                {
                //if creation failed then free the lib.
                ::FreeLibrary(h);
                }
            }
        return hr;
        }
    };


#endif