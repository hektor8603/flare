/**
 *	thread_handler.h
 *
 *	@author	Masaki Fujimoto <fujimoto@php.net>
 *
 *	$Id$
 */
#ifndef	__THREAD_HANDLER_H__
#define	__THREAD_HANDLER_H__

#include "thread.h"

using namespace std;
using namespace boost;

namespace gree {
namespace flare {

/**
 *	thread handler base class
 */
class thread_handler {
protected:
	shared_thread			_thread;

public:
	thread_handler(shared_thread t): _thread(t) {};
	virtual ~thread_handler() {};

	virtual int run() = 0;

protected:
};

}	// namespace flare
}	// namespace gree

#endif	// __THREAD_HANDLER_H__
// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
