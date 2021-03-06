/**
 *	op_stats_index.h
 *
 *	@author	Masaki Fujimoto <fujimoto@php.net>
 *
 *	$Id$
 */
#ifndef __OP_STATS_INDEX_H__
#define __OP_STATS_INDEX_H__

#include <sstream>

#include "op_stats.h"

namespace gree {
namespace flare {

/**
 *	opcode text parser for index application
 */
class op_stats_index : public op_stats {
protected:

public:
	op_stats_index(shared_connection c);
	virtual ~op_stats_index();

protected:
	virtual int _parse_text_server_parameters();
	virtual int _run_server();
};

}	// namespace flare
}	// namespace gree

#endif // __OP_STATS_INDEX_H__
// vim: foldmethod=marker tabstop=2 shiftwidth=2 autoindent
