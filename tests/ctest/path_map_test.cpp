#include "CppUTest/TestHarness.h"

#include <unistd.h>

#include "maps_private.h"
#include "maps.h"
#include "parser.h"

TEST_GROUP(path_maps_file)
{
	struct xdebug_path_maps *test_map;
	bool result;
	int   error_code;
	int   error_line;
	char *error_message;
	size_t mapping_count;
	FILE *filep;

	xdebug_str *local_path;
	size_t      local_line;
	int         mapping_type;

	char *filename;

	TEST_SETUP()
	{
		test_map = xdebug_path_maps_ctor();
		result = false;
		error_code = PATH_MAPS_OK;
		error_line = -1;
		error_message = NULL;
		filename = NULL;
		filep = NULL;
		mapping_type = -1;
		local_path = NULL;
		local_line = -1;
	}

	bool test_map_from_file(const char *data_string)
	{
		char templ[] = "/tmp/xdct.XXXXXX";

		int fp = mkstemp(templ);

		filep = fdopen(fp, "w");
		fputs(data_string, filep);
		fclose(filep);
		filep = NULL;

		filename = strdup(templ);

		return xdebug_path_maps_parse_file(test_map, filename, &error_code, &error_line, &error_message);
	}

	void check_result(size_t expected_error_code, int expected_error_line, const char *expected_error_message)
	{
		STRCMP_EQUAL(expected_error_message, error_message);
		LONGS_EQUAL(expected_error_code, error_code);
		LONGS_EQUAL(expected_error_line, error_line);
		LONGS_EQUAL(expected_error_code == PATH_MAPS_OK ? true : false, result);
	}

	void check_map(size_t expected_type, const char *expected_local_path)
	{
		CHECK(mapping_type != XDEBUG_PATH_MAP_TYPE_UNKNOWN);
		LONGS_EQUAL(expected_type, mapping_type);
		STRCMP_EQUAL(expected_local_path, XDEBUG_STR_VAL(local_path));
	}

	void test_remote_to_local(const char *remote_path, size_t remote_line)
	{
		mapping_type = remote_to_local(test_map, remote_path, remote_line, &local_path, &local_line);
	}

	void check_map_with_range(size_t expected_type, const char *expected_local_path, size_t expected_local_line)
	{
		check_map(expected_type, expected_local_path);
		LONGS_EQUAL(expected_local_line, local_line);
	}

	TEST_TEARDOWN()
	{
		if (error_message) {
			free(error_message);
		}
		if (local_path) {
			xdebug_str_free(local_path);
		}

		if (filename) {
			unlink(filename);
			free(filename);
			filename = NULL;
		}
		if (filep) {
			fclose(filep);
		}

		xdebug_path_maps_dtor(test_map);
	}
};

TEST(path_maps_file, fopen_non_existing)
{
	result = xdebug_path_maps_parse_file(test_map, "file-does-not-exist.map", &error_code, &error_line, &error_message);

	LONGS_EQUAL(false, result);
	LONGS_EQUAL(PATH_MAPS_CANT_OPEN_FILE, error_code);
	STRCMP_EQUAL("Can't open file", error_message);
};

TEST(path_maps_file, no_trailing_newline)
{
	const char *map = R""""(/var/www/ = /home/derick/projects/example.com/)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_NO_NEWLINE, 1, "Line does not end in a new line");
};

TEST(path_maps_file, only_new_line)
{
	const char *map = R""""(
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_NO_RULES, 1, "The map file did not provide any mappings");
};

TEST(path_maps_file, comment_no_rules)
{
	const char *map = R""""(# This is the first line of a comment
# This is the second line of a comment
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_NO_RULES, 2, "The map file did not provide any mappings");
};

TEST(path_maps_file, empty)
{
	const char *map = R""""()"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_NO_RULES, 0, "The map file did not provide any mappings");
};

TEST(path_maps_file, no_rules)
{
	const char *map = R""""(
remote_prefix: /var/www/
local_prefix: /home/derick/projects/example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_NO_RULES, 3, "The map file did not provide any mappings");
};

TEST(path_maps_file, full_path_map)
{
	const char *map = R""""(
/var/www/ = /home/derick/projects/example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);
};

TEST(path_maps_file, check_rules)
{
	const char *map = R""""(
/var/www/ = /home/derick/projects/example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/var/www/", 1);

	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.com/");
};

TEST(path_maps_file, check_rule_with_comment)
{
	const char *map = R""""(
# We map our remote path to our local projects directory
/var/www/ = /home/derick/projects/example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/var/www/", 1);

	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.com/");
};

TEST(path_maps_file, check_rule_with_odd_spaces)
{
	const char *map = R""""(
# We map our remote path to our local projects directory
	/var/www/	=	/home/derick/projects/example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/var/www/", 1);

	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.com/");
};

TEST(path_maps_file, check_rules_with_prefix_1)
{
	const char *map = R""""(
remote_prefix: /var
local_prefix: /home/derick/projects
/www/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/var/www/", 1);

	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.com/");
};

TEST(path_maps_file, empty_remote_prefix)
{
	const char *map = R""""(
remote_prefix:
local_prefix: /usr/local/www
/www/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_INVALID_PREFIX, 2, "Prefix is empty");
}

TEST(path_maps_file, empty_local_prefix)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix:
/www/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_INVALID_PREFIX, 3, "Prefix is empty");
}

TEST(path_maps_file, non_absolute_remote_prefix)
{
	const char *map = R""""(
remote_prefix: var
local_prefix: /usr/local/www
/www/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_INVALID_PREFIX, 2, "Prefix is not an absolute path: 'var'");
}

TEST(path_maps_file, non_absolute_local_prefix)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix:home/derick/projects
/www/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_INVALID_PREFIX, 3, "Prefix is not an absolute path: 'home/derick/projects'");
}

TEST(path_maps_file, check_multiple_rules_with_prefix_1)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.com/ = /example.com/
/servers/example.net/ = /example.net/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/servers/example.com/", 1);
	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.com/");
}

TEST(path_maps_file, check_multiple_rules_with_prefix_2)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.com/ = /example.com/
/servers/example.net/ = /example.net/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);


	test_remote_to_local("/usr/local/www/servers/example.net/", 1);
	check_map(XDEBUG_PATH_MAP_TYPE_DIRECTORY, "/home/derick/projects/example.net/");
};

TEST(path_maps_file, no_double_separator_remote_prefix)
{
	const char *map = R""""(
remote_prefix: /usr/local/www/
local_prefix: /home/derick/projects
/servers/example.com/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_DOUBLE_SEPARATOR, 4, "Remote prefix ends with separator ('/usr/local/www/') and mapping line begins with separator ('/servers/example.com/')");
};

TEST(path_maps_file, no_double_separator_local_prefix)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects/
/servers/example.com/ = /example.com/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_DOUBLE_SEPARATOR, 4, "Local prefix ends with separator ('/home/derick/projects/') and mapping line begins with separator ('/example.com/')");
};

TEST(path_maps_file, check_local_matches_remote_mapping_type_dir_file)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.com/ = /example.com/
/servers/example.net/ = /example.net
/servers/example.org/ = /example.org/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_MISMATCHED_TYPES, 5, "Remote mapping part ('/usr/local/www/servers/example.net/') type (directory) must match local mapping part ('/home/derick/projects/example.net') type (file)");
};

TEST(path_maps_file, check_local_matches_remote_mapping_type_file_dir)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.com/ = /example.com/
/servers/example.net = /example.net/
/servers/example.org/ = /example.org/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_MISMATCHED_TYPES, 5, "Remote mapping part ('/usr/local/www/servers/example.net') type (file) must match local mapping part ('/home/derick/projects/example.net/') type (directory)");
};

TEST(path_maps_file, remote_part_unknown_type)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.org? = /example.org/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_MISMATCHED_TYPES, 4, "Remote mapping part ('/usr/local/www/servers/example.org?') type (file) must match local mapping part ('/home/derick/projects/example.org/') type (directory)");
};

TEST(path_maps_file, local_part_unknown_type)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/projects
/servers/example.org/ = /example.org?
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_MISMATCHED_TYPES, 4, "Remote mapping part ('/usr/local/www/servers/example.org/') type (directory) must match local mapping part ('/home/derick/projects/example.org?') type (file)");
};

TEST(path_maps_file, check_type_file)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 1);
	check_map(XDEBUG_PATH_MAP_TYPE_FILE, "/home/derick/project/example.php");
};

TEST(path_maps_file, remote_path_wrong_start_range_number)
{
	const char *map = R""""(
/example.php:53x = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 2, "Remote element: Non-number found as range: ':53x'");
};

TEST(path_maps_file, local_path_wrong_start_range_number)
{
	const char *map = R""""(
/example.php = /example.php:72c
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 2, "Local element: Non-number found as range: ':72c'");
};

TEST(path_maps_file, remote_path_only_range_number)
{
	const char *map = R""""(
:53x = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 2, "Remote element: Element only contains a range, but no path: ':53x'");
};

TEST(path_maps_file, local_path_only_range_number)
{
	const char *map = R""""(
/example.php = :72c
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 2, "Local element: Element only contains a range, but no path: ':72c'");
};

TEST(path_maps_file, remote_path_with_directory)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/examples/:53x = /examples/
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: Ranges are not supported with directories: '/examples/:53x'");
};

TEST(path_maps_file, local_path_with_directory)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/examples/ = /examples/:72c
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: Ranges are not supported with directories: '/examples/:72c'");
};

TEST(path_maps_file, remote_single_range_number)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:1 = /example.php:42
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 1);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 42);
};

TEST(path_maps_file, remote_range_less_than_one)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:0 = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: Line number much be larger than 0: '/example.php:0'");
};

TEST(path_maps_file, local_range_less_than_one)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php = /example.php:0
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: Line number much be larger than 0: '/example.php:0'");
};

TEST(path_maps_file, remote_range_empty_start)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:-42 = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: The starting line number must be provided: '/example.php:-42'");
};

TEST(path_maps_file, local_range_empty_start)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:7 = /example.php:-8
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: The starting line number must be provided: '/example.php:-8'");
};

TEST(path_maps_file, remote_range_wrong_begin_lineno)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:6x-42 = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: Non-number found as begin range: ':6x-42'");
};

TEST(path_maps_file, local_range_wrong_begin_lineno)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php = /example.php:7y-43
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: Non-number found as begin range: ':7y-43'");
};

TEST(path_maps_file, remote_range_wrong_end_lineno)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:6-42x = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: Non-number found as end range: ':6-42x'");
};

TEST(path_maps_file, local_range_wrong_end_lineno)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php = /example.php:7-43y
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: Non-number found as end range: ':7-43y'");
};

TEST(path_maps_file, remote_range_wrong_range_order)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:42-5 = /example.php
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Remote element: End of range (42) is before start of range (5): ':42-5'");
};

TEST(path_maps_file, local_range_wrong_range_order)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php = /example.php:75-8
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "Local element: End of range (75) is before start of range (8): ':75-8'");
};

TEST(path_maps_file, range_span_mismatch)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:2-5 = /example.php:2-8
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_WRONG_RANGE, 4, "The remote range span (2-5) needs to have the same difference (3) as the local range span (2-8) difference (6)");
};

TEST(path_maps_file, range_span_single)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5 = /example.php:8
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 5);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 8);
};

TEST(path_maps_file, range_span_n_to_1)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 8);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 8);
};

TEST(path_maps_file, range_span_n_to_m)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8-20
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 14);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 17);
};

TEST(path_maps_file, multiple_ranges_one_file_1)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8-20
/example.php:18 = /example.php:21
/example.php:19-33 = /example.php:24
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 6);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 9);
};

TEST(path_maps_file, multiple_ranges_one_file_2)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8-20
/example.php:18 = /example.php:21
/example.php:19-33 = /example.php:24
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 18);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 21);
};

TEST(path_maps_file, multiple_ranges_one_file_3)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8-20
/example.php:18 = /example.php:21
/example.php:19-33 = /example.php:24
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 32);
	check_map_with_range(XDEBUG_PATH_MAP_TYPE_LINES, "/home/derick/project/example.php", 24);
};

TEST(path_maps_file, multiple_ranges_outside_ranges)
{
	const char *map = R""""(
remote_prefix: /usr/local/www
local_prefix: /home/derick/project
/example.php:5-17 = /example.php:8-20
/example.php:18 = /example.php:21
/example.php:19-33 = /example.php:24
)"""";

	result = test_map_from_file(map);
	check_result(PATH_MAPS_OK, -1, NULL);

	test_remote_to_local("/usr/local/www/example.php", 42);
	CHECK_EQUAL(XDEBUG_PATH_MAP_TYPE_UNKNOWN, mapping_type);
};
