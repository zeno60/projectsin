#ifndef D3DHELPER_H
#define D3DHELPER_H

template<class T>
inline void Release(T t)
{
	if(t)
	{
		t->Release();
		t = NULL;
	}
}

template<class T>
inline void Delete(T t)
{
	if(t)
	{
		delete t;
		t = NULL;
	}
}

#endif