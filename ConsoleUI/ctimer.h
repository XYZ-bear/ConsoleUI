#pragma once
#include <queue>
#include <functional>
#include <sys/timeb.h>

class ctimer
{
	class base_timer
	{
	public:
		int interval;
		int64_t expire;
		function<void()> call_func_;
		bool operator() (base_timer* a, const base_timer *b){
			return a->expire > b->expire;     //这时是小根堆，改为小于号后为大根堆
		}
	};

	template<class T>
	class timer:public base_timer
	{
	public:
		T *ob_;
		typedef void (T::*callback)();
		callback regist_func_;
	};

public:
	ctimer() {};
	~ctimer() {
		while (!_pg.empty())
		{
			delete _pg.top();
			_pg.pop();
		}
	};
private:
	priority_queue<base_timer*, vector<base_timer*>, base_timer> _pg;
	base_timer *_btim=nullptr;
public:
	int64_t get_current_time()      //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
	{
		timeb t;
		ftime(&t);
		return t.time * 1000 + t.millitm; 
	}
	template<class T,class _Fn>
	void add_timer(T *ob,int interval, _Fn func_) {
		timer<T> *_tim = new timer<T>();
		_tim->regist_func_ = func_;
		_tim->ob_ = ob;

		_btim = _tim;
		_btim->interval = interval;
		_btim->expire = interval + get_current_time();
		_btim->call_func_ = [this](){
			if (_btim) {
				T *ob = ((timer<T>*)_btim)->ob_;
				auto fun = ((timer<T>*)_btim)->regist_func_;
				if (ob&&fun)
					(ob->*fun)();
			}
		};

		_pg.push(_btim);
	}
	template<class _Fn>
	void kill_timer(_Fn fn) {
		//for(a)
	}

	void check_timer() {
		if (_pg.size() > 0) {
			auto now = get_current_time();
			auto old = _pg.top();
			if (now >= old->expire) {
				_btim = _pg.top();
				old->call_func_();
				old->expire = now + old->interval;
				_pg.pop();
				_pg.push(old);
			}
		}
	}
	static ctimer& instance() {
		static ctimer time;
		return time;
	}
};

