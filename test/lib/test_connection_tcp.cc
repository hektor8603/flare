/**
 *	test_connection_tcp.cc
 *
 *	@author	Benjamin Surma <benjamin.surma@gree.net>
 */
#include <cppcutter.h>

#include "test_connection_tcp.h"

#include "common_connection_tests.h"
#include <app.h>
#include <connection_tcp.h>

#include <iostream>

using namespace gree::flare;

namespace test_connection_tcp
{
	server* server_object;

	void setup()
	{
		stats_object = new stats();
		stats_object->update_timestamp();
		server_object = NULL;
	}

	connection* new_connection_tcp(const std::string& input, int* timeout = NULL)
	{
		server_object = new server(input);
		connection_tcp* instance = new connection_tcp("localhost", server_object->get_port());
		if (timeout)
			instance->set_read_timeout(*timeout);
		cut_assert_equal_string("localhost", instance->get_host().c_str());
		cut_assert_equal_int(server_object->get_port(), instance->get_port());
		sleep(1);
		return instance;
	}

	connection* connection_tcp_factory(const std::string& input)
	{
		int one_second = 1000;
		return new_connection_tcp(input, &one_second);
	}

	COMMON_CONNECTION_TEST(connection_tcp, readsize_basic);
	COMMON_CONNECTION_TEST(connection_tcp, readsize_zero);
	COMMON_CONNECTION_TEST(connection_tcp, readsize_empty);
	COMMON_CONNECTION_TEST(connection_tcp, readline_basic);
	COMMON_CONNECTION_TEST(connection_tcp, readline_unix);
	COMMON_CONNECTION_TEST(connection_tcp, push_back_basic);

	struct connection_tcp_test : public connection_tcp
	{
		using connection_tcp::_read_buf;
		using connection_tcp::_read_buf_p;
		using connection_tcp::_read_buf_len;
	};

	void test_connection_tcp_readsize_greedy()
	{
		shared_connection c(new_connection_tcp("short"));
		c->open();
		connection_tcp& ctcp = dynamic_cast<connection_tcp&>(*c);
		cut_assert_equal_int(10*60*1000, ctcp.get_read_timeout());
		cut_assert_equal_int(0, ctcp.set_read_timeout(1000)); // 1s.
		cut_assert_equal_int(1000, ctcp.get_read_timeout());
		char* buffer = NULL;
		cut_assert_equal_int(-1, c->readsize(10, &buffer));
		cut_assert_equal_int(0, static_cast<connection_tcp_test&>(*c)._read_buf_len);
		delete[] buffer;
	}
	
	void test_connection_tcp_readline_no_newline()
	{
		shared_connection c(new_connection_tcp("1 line only!"));
		c->open();
		connection_tcp& ctcp = dynamic_cast<connection_tcp&>(*c);
		cut_assert_equal_int(10*60*1000, ctcp.get_read_timeout());
		cut_assert_equal_int(0, ctcp.set_read_timeout(1000)); // 1s.
		cut_assert_equal_int(1000, ctcp.get_read_timeout());
		char* buffer = NULL;
		cut_assert_equal_int(-1, c->readline(&buffer));
		cut_assert_equal_int(0, static_cast<connection_tcp_test&>(*c)._read_buf_len);
		delete[] buffer;
	}

	void test_connection_tcp_check_internal_buffer()
	{
		shared_connection c(new_connection_tcp("0123456789"));
		c->open();
		char* buffer = NULL;
		bool actual;
		cut_assert_equal_int(5, c->read(&buffer, 5, false, actual));
		cut_assert_equal_substring("01234", buffer, 5);
		cut_assert_equal_boolean(true, actual);
		delete[] buffer;
		buffer = NULL;
		cut_assert_equal_substring("0123456789", static_cast<connection_tcp_test&>(*c)._read_buf, 10);
		cut_assert_equal_int(5, static_cast<connection_tcp_test&>(*c)._read_buf_len);
		cut_assert_equal_substring("56789", static_cast<connection_tcp_test&>(*c)._read_buf_p, 5);
		cut_assert_equal_int(5, c->read(&buffer, 5, false, actual));
		cut_assert_equal_substring("56789", buffer, 5);
		cut_assert_equal_boolean(false, actual);
		delete[] buffer;
	}
	
	void teardown()
	{
		delete stats_object;
		delete server_object;
	}
}
// vim: foldmethod=marker tabstop=2 shiftwidth=2 noexpandtab autoindent
