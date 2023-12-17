#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <map>

# include "utils.hpp"

# define SUCCESS 0
# define FAILURE 1

class HTTPParser
{
	private:
		enum state
		{
			e_dead = 1,
			e_start_req_or_res,
			e_res_or_resp_H,
			e_start_res,
			e_res_H,
			e_res_HT,
			e_res_HTT,
			e_res_HTTP,
			e_res_http_major,
			e_res_http_dot,
			e_res_http_minor,
			e_res_http_end,
			e_res_first_status_code,
			e_res_status_code,
			e_res_status_start,
			e_res_status,
			e_res_line_almost_done,

			e_start_req,

			e_req_method,
			e_req_spaces_before_url,
			e_req_schema,
			e_req_schema_slash,
			e_req_schema_slash_slash,
			e_req_server_start,
			e_req_server,
			e_req_server_with_at,
			e_req_path,
			e_req_query_string_start,
			e_req_query_string,
			e_req_fragment_start,
			e_req_fragment,
			e_req_http_start,
			e_req_http_H,
			e_req_http_HT,
			e_req_http_HTT,
			e_req_http_HTTP,
			e_req_http_I,
			e_req_http_IC,
			e_req_http_major,
			e_req_http_dot,
			e_req_http_minor,
			e_req_http_end,
			e_req_line_almost_done,

			e_header_field_start,
			e_header_field,
			e_header_value_discard_ws,
			e_header_value_discard_ws_almost_done,
			e_header_value_discard_lws,
			e_header_value_start,
			e_header_value,
			e_header_value_lws,

			e_header_almost_done,

			e_chunk_size_start,
			e_chunk_size,
			e_chunk_parameters,
			e_chunk_size_almost_done,

			e_headers_almost_done,
			e_headers_done,

			e_chunk_data,
			e_chunk_data_almost_done,
			e_chunk_data_done,

			e_body_identity,
			e_body_identity_eof,

			e_message_done
		};
		// state								_state;
		std::string							_method;
		std::string							_version;
		std::string							_url;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		int									_error_code;

		int	parseRequestLine(std::string &data);
		int	parseHeader(std::string &data);

	public:
		HTTPParser(std::string &request);
		HTTPParser(const HTTPParser &src);
		HTTPParser	&operator=(const HTTPParser &rhs);
		~HTTPParser(void);

		std::string	getMethod(void) const;
		std::string	getTarget(void) const;
		std::string	getVersion(void) const;

		void	executeParse(std::string &data);
};

#endif
