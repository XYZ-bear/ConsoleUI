#pragma once

#include <map>
#include <functional>

class base_event_brigde {
public:
	virtual void do_event(int id, void *data) {};
};

template<typename T>
class event_brigde :public base_event_brigde {
	typedef void (T::*callback)(void* data);
private:
	T * ob_;
	map<int, callback> event_func_;
public:
	void add_event(T *ob, int id, callback func) {
		ob_ = ob;
		event_func_[id] = func;
	}
	void do_event(int id, void *data) {
		if (event_func_[id])
			(ob_->*event_func_[id])(data);
	}
};

class cevent
{
public:
	cevent() { };
	~cevent() {};
public:
	template<class T, class _Fn>
	void add_cmd(T *con, int id, _Fn func) {
		if (!brigde_) {
			brigde_ = new event_brigde<T>();
		}
		((event_brigde<T>*)brigde_)->add_event(con, id, func);
		call_func_ = [this](int id, void *data) {
			brigde_->do_event(id, data);
		};
	}
private:
	base_event_brigde * brigde_ = nullptr;
protected:
	function<void(int id, void* data)> call_func_ = [this](int id, void *data) {};
};