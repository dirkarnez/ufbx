
UFBXT_TEST(parse_single_value)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
	);

	uint32_t value;
	ufbxt_assert(ufbxi_parse_value(uc, 'I', &value));
	ufbxt_assert(value == 0x04030201);
}
#endif

UFBXT_TEST(parse_multiple_values)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
		"Y\x11\x12"
	);

	uint32_t a, b;
	ufbxt_assert(ufbxi_parse_values(uc, "II", &a, &b));
	ufbxt_assert(a == 0x04030201);
	ufbxt_assert(b == 0x1211);
}
#endif

UFBXT_TEST(parse_sign_extension)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"Y\xff\xff"
		"I\xfe\xff\xff\xff"
		"L\xfd\xff\xff\xff\xff\xff\xff\xff"
	);

	int32_t y32 = 0, i32 = 0, l32 = 0;
	ufbxt_assert(ufbxi_parse_values(uc, "III", &y32, &i32, &l32));
	ufbxt_assert(y32 == -1);
	ufbxt_assert(i32 == -2);
	ufbxt_assert(l32 == -3);

	uc->focused_node.next_value_pos = 0;

	int64_t y64 = 0, i64 = 0, l64 = 0;
	ufbxt_assert(ufbxi_parse_values(uc, "LLL", &y64, &i64, &l64));
	ufbxt_assert(y64 == -1);
	ufbxt_assert(i64 == -2);
	ufbxt_assert(l64 == -3);
}
#endif

UFBXT_TEST(parse_value_eof)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context("");

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_value(uc, 'I', &value));
	ufbxt_log_error(uc);
}
#endif


UFBXT_TEST(parse_value_truncated)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03"
	);

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_value(uc, 'I', &value));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_value_bad_src_type)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"X\x01\x02\x03\x04"
	);

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_value(uc, 'I', &value));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_value_bad_dst_type)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
	);

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_value(uc, 'X', &value));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_value_bad_conversion)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"F\x01\x02\x03\x04"
	);

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_value(uc, 'I', &value));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_value_fail_position)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
	);

	uint32_t a, b;
	ufbxt_assert(!ufbxi_parse_values(uc, "II", &a, &b));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_value_basic_types)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
		"L\x11\x12\x13\x14\x15\x16\x17\x18"
		"Y\x21\x22"
		"C\x01"
		"F\x00\x00\x80\x3f"
		"D\x00\x00\x00\x00\x00\x00\xf0\x3f"
	);

	uint64_t i, l, y, c;
	float f;
	double d;
	ufbxt_assert(ufbxi_parse_values(uc, "LLLLFD", &i, &l, &y, &c, &f, &d));
	ufbxt_assert(i == 0x04030201);
	ufbxt_assert(l == UINT64_C(0x1817161514131211));
	ufbxt_assert(y == 0x2221);
	ufbxt_assert(c == 1);
	ufbxt_assert(f == 1.0f);
	ufbxt_assert(d == 1.0);
}
#endif

UFBXT_TEST(parse_int_to_float)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x08\x00\x00\x00"
		"Y\x04\x00"
	);

	float f;
	double d;
	ufbxt_assert(ufbxi_parse_values(uc, "FD", &f, &d));
	ufbxt_assert(f == 8.0f);
	ufbxt_assert(d == 4.0);
}
#endif

UFBXT_TEST(parse_string_value)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"S\x05\x00\x00\x00Hello"
		"R\x05\x00\x00\x00world"
	);

	ufbxi_string a, b;
	ufbxt_assert(ufbxi_parse_values(uc, "SS", &a, &b));
	ufbxt_assert(ufbxi_streq(a, "Hello"));
	ufbxt_assert(ufbxi_streq(b, "world"));
}
#endif

UFBXT_TEST(parse_string_long)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"S\x40\x00\x00\x00Hello"
	);

	ufbxi_string str;
	ufbxt_assert(!ufbxi_parse_value(uc, 'S', &str));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_string_huge)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"S\xff\xff\xff\xffHello"
	);

	ufbxi_string str;
	ufbxt_assert(!ufbxi_parse_value(uc, 'S', &str));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_string_bad_dst)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"S\x05\x00\x00\x00Hello"
	);

	uint32_t value;
	ufbxt_assert(!ufbxi_parse_values(uc, "I", &value));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_boolean_binary)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"C\x10"
		"C\x20"
		"I\x20\x20\x20\x20"
	);

	uint32_t a;
	char b, c;
	ufbxt_assert(ufbxi_parse_values(uc, "IBB", &a, &b, &c));
	ufbxt_assert(a == 1);
	ufbxt_assert(b == 1);
	ufbxt_assert(c == 1);
}
#endif

UFBXT_TEST(parse_ignore_value)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
	);

	uint32_t value;
	ufbxt_assert(ufbxi_parse_value(uc, '.', &value));
	ufbxt_assert(uc->focused_node.next_value_pos == 5);
}
#endif

UFBXT_TEST(parse_skip_value)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
		"I\x11\x12\x13\x14"
	);

	uint32_t value;
	ufbxt_assert(ufbxi_parse_values(uc, ".I", &value));
	ufbxt_assert(value == 0x14131211);
}
#endif

UFBXT_TEST(parse_any_values)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
		"L\x11\x12\x13\x14\x15\x16\x17\x18"
		"Y\x21\x22"
		"C\x01"
		"F\x00\x00\x80\x3f"
		"D\x00\x00\x00\x00\x00\x00\xf0\x3f"
		"S\x05\x00\x00\x00Hello"
		"i" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	ufbxi_any_value i, l, y, c, f, d, s, a;
	int data[3];
	ufbxt_assert(ufbxi_parse_values(uc, "????????", &i, &l, &y, &c, &f, &d, &s, &a));
	ufbxt_assert(i.type_code == 'I');
	ufbxt_assert(i.value_class == ufbxi_val_int);
	ufbxt_assert(i.value.i == 0x04030201);
	ufbxt_assert(l.type_code == 'L');
	ufbxt_assert(l.value_class == ufbxi_val_int);
	ufbxt_assert(l.value.i == UINT64_C(0x1817161514131211));
	ufbxt_assert(y.type_code == 'Y');
	ufbxt_assert(y.value_class == ufbxi_val_int);
	ufbxt_assert(y.value.i == 0x2221);
	ufbxt_assert(c.type_code == 'C');
	ufbxt_assert(c.value_class == ufbxi_val_int);
	ufbxt_assert(c.value.i == 1);
	ufbxt_assert(f.type_code == 'F');
	ufbxt_assert(f.value_class == ufbxi_val_float);
	ufbxt_assert(f.value.f == 1.0f);
	ufbxt_assert(d.type_code == 'D');
	ufbxt_assert(d.value_class == ufbxi_val_float);
	ufbxt_assert(d.value.f == 1.0);
	ufbxt_assert(s.type_code == 'S');
	ufbxt_assert(s.value_class == ufbxi_val_string);
	ufbxt_assert(ufbxi_streq(s.value.str, "Hello"));
	ufbxt_assert(a.type_code == 'i');
	ufbxt_assert(a.value_class == ufbxi_val_array);
	ufbxt_assert(a.value.arr.src_type == 'i');
	ufbxt_assert(a.value.arr.dst_type == 'i');
	ufbxt_assert(a.value.arr.num_elements == 3);
	ufbxt_assert(a.value.arr.encoding == ufbxi_encoding_basic);
	ufbxt_assert(a.value.arr.encoded_size == 3*4);
	ufbxt_assert(ufbxi_decode_array(uc, &a.value.arr, data));
	ufbxt_assert(data[0] == 1 && data[1] == 2 && data[2] == 3);
}
#endif

UFBXT_TEST(parse_skip_values)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x02\x03\x04"
		"L\x11\x12\x13\x14\x15\x16\x17\x18"
		"Y\x21\x22"
		"C\x01"
		"F\x00\x00\x80\x3f"
		"D\x00\x00\x00\x00\x00\x00\xf0\x3f"
		"S\x05\x00\x00\x00Hello"
		"i" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	ufbxt_assert(ufbxi_parse_values(uc, "........"));
	ufbxt_assert(uc->focused_node.next_value_pos == uc->focused_node.child_begin_pos);
}
#endif

UFBXT_TEST(parse_simple_array)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_values(uc, "i", &arr));
	ufbxt_assert(arr.src_type == 'i');
	ufbxt_assert(arr.dst_type == 'i');
	ufbxt_assert(arr.num_elements == 3);
	ufbxt_assert(arr.encoding == ufbxi_encoding_basic);
	ufbxt_assert(arr.encoded_size == 3*4);

	int data[3];
	ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
	ufbxt_assert(data[0] == 1);
	ufbxt_assert(data[1] == 2);
	ufbxt_assert(data[2] == 3);
}
#endif

UFBXT_TEST(parse_simple_array_conversion)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	int32_t value;
	ufbxt_assert(!ufbxi_parse_values(uc, "I", &value));
	ufbxt_log_error(uc);
}
#endif

#if UFBXT_IMPL
static void helper_array_conversion(ufbxi_context *uc, char dst_type, char src_type)
{
	ufbxt_hintf("dst = '%c', src = '%c'", dst_type, src_type);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, dst_type, &arr));

	ufbxt_assert(arr.encoding == ufbxi_encoding_basic);
	ufbxt_assert(arr.src_type == src_type);
	ufbxt_assert(arr.dst_type == dst_type);
	ufbxt_assert(arr.num_elements == 2);

	switch (src_type) {
	case 'i': case 'f': ufbxt_assert(arr.encoded_size == 2 * 4); break;
	case 'l': case 'd': ufbxt_assert(arr.encoded_size == 2 * 8); break;
	}

	switch (dst_type) {
	case 'i': {
		int32_t data[2] = { 0, 0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 4);
		ufbxt_assert(data[1] == -8);
	} break;
	case 'l': {
		int64_t data[2] = { 0, 0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 4);
		ufbxt_assert(data[1] == -8);
	} break;
	case 'f': {
		float data[2] = { 0.0f, 0.0f };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 4.0f);
		ufbxt_assert(data[1] == -8.0f);
	} break;
	case 'd': {
		double data[2] = { 0.0, 0.0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 4.0);
		ufbxt_assert(data[1] == -8.0);
	} break;
	}
}
#endif

UFBXT_TEST(parse_array_conversions)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x02\x00\x00\x00" "\x00\x00\x00\x00" "\x08\x00\x00\x00"
		"\x04\x00\x00\x00" "\xf8\xff\xff\xff"
		"l" "\x02\x00\x00\x00" "\x00\x00\x00\x00" "\x10\x00\x00\x00"
		"\x04\x00\x00\x00\x00\x00\x00\x00" "\xf8\xff\xff\xff\xff\xff\xff\xff"
		"f" "\x02\x00\x00\x00" "\x00\x00\x00\x00" "\x08\x00\x00\x00"
		"\x00\x00\x80\x40" "\x00\x00\x00\xc1"
		"d" "\x02\x00\x00\x00" "\x00\x00\x00\x00" "\x10\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x10\x40" "\x00\x00\x00\x00\x00\x00\x20\xc0"
	);

	uc->focused_node.next_value_pos = 0;
	helper_array_conversion(uc, 'i', 'i');
	helper_array_conversion(uc, 'i', 'l');
	uc->focused_node.next_value_pos = 0;
	helper_array_conversion(uc, 'l', 'i');
	helper_array_conversion(uc, 'l', 'l');
	uc->focused_node.next_value_pos = 0;
	helper_array_conversion(uc, 'f', 'i');
	helper_array_conversion(uc, 'f', 'l');
	helper_array_conversion(uc, 'f', 'f');
	helper_array_conversion(uc, 'f', 'd');
	uc->focused_node.next_value_pos = 0;
	helper_array_conversion(uc, 'd', 'i');
	helper_array_conversion(uc, 'd', 'l');
	helper_array_conversion(uc, 'd', 'f');
	helper_array_conversion(uc, 'd', 'd');
}
#endif

#if UFBXT_IMPL
static void helper_array_bool(ufbxi_context *uc, char dst_type, char src_type)
{
	ufbxt_hintf("dst = '%c', src = '%c'", dst_type, src_type);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, dst_type, &arr));

	ufbxt_assert(arr.encoding == ufbxi_encoding_basic);
	ufbxt_assert(arr.src_type == src_type);
	ufbxt_assert(arr.dst_type == dst_type);
	ufbxt_assert(arr.num_elements == 3);

	switch (src_type) {
	case 'i': ufbxt_assert(arr.encoded_size == 3 * 4); break;
	case 'b': ufbxt_assert(arr.encoded_size == 3 * 1); break;
	}

	switch (dst_type) {
	case 'i': {
		int32_t data[3] = { 0, 0, 0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 0 && data[1] == 1 && data[2] == 0);
	} break;
	case 'l': {
		int64_t data[3] = { 0, 0, 0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 0 && data[1] == 1 && data[2] == 0);
	} break;
	case 'b': {
		char data[3] = { 0, 0, 0 };
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 0 && data[1] == 1 && data[2] == 0);
	} break;
	}
}
#endif

UFBXT_TEST(parse_array_src_bool)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"b" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x03\x00\x00\x00"
		"\x00\x01\x00"
		"i" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x00\x00\x00\x00" "\x01\x00\x00\x00" "\x00\x00\x00\x00"
		"l" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x18\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00"
		"\x01\x00\x00\x00\x00\x00\x00\x00"
		"\x00\x00\x00\x00\x00\x00\x00\x00"
	);

	uc->focused_node.next_value_pos = 0;
	helper_array_bool(uc, 'b', 'b');
	helper_array_bool(uc, 'b', 'i');
	helper_array_bool(uc, 'b', 'l');
	uc->focused_node.next_value_pos = 0;
	helper_array_bool(uc, 'i', 'b');
	helper_array_bool(uc, 'i', 'i');
	helper_array_bool(uc, 'i', 'l');
	uc->focused_node.next_value_pos = 0;
	helper_array_bool(uc, 'l', 'b');
	helper_array_bool(uc, 'l', 'i');
	helper_array_bool(uc, 'l', 'l');
}
#endif

UFBXT_TEST(parse_array_bad_conversion)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"f" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x00\x00\x80\x3f" "\x00\x00\x80\x3f" "\x00\x00\x80\x3f"
		"b" "\x03\x00\x00\x00" "\x00\x00\x00\x00" "\x03\x00\x00\x00"
		"\x01\x01\x01"
	);

	char data[128];
	ufbxi_array arr;
	uc->focused_node.next_value_pos = 0;
	ufbxt_assert(ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	uc->focused_node.next_value_pos = 0;
	ufbxt_assert(ufbxi_parse_value(uc, 'l', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	uc->focused_node.next_value_pos = 0;
	ufbxt_assert(ufbxi_parse_value(uc, 'b', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));

	uint32_t begin = uc->focused_node.next_value_pos;
	ufbxt_assert(ufbxi_parse_value(uc, 'f', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	uc->focused_node.next_value_pos = begin;
	ufbxt_assert(ufbxi_parse_value(uc, 'd', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
}
#endif

UFBXT_TEST(parse_array_bad_size)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x08\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	int32_t data[8];
	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_array_bad_size_conversion)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x08\x00\x00\x00" "\x00\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	int64_t data[8];
	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, 'l', &arr));
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_array_bad_encoding)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x03\x00\x00\x00" "\xff\x00\x00\x00" "\x0c\x00\x00\x00"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	ufbxi_array arr;
	ufbxt_assert(!ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_array_huge)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"i" "\x03\x00\x00\x00" "\xff\x00\x00\x00" "\xff\xff\xff\xff"
		"\x01\x00\x00\x00" "\x02\x00\x00\x00" "\x03\x00\x00\x00"
	);

	ufbxi_array arr;
	ufbxt_assert(!ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_multivalue_array)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x00\x00\x00I\x02\x00\x00\x00I\x03\x00\x00\x00"
	);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_assert(arr.src_type == 'i');
	ufbxt_assert(arr.dst_type == 'i');
	ufbxt_assert(arr.num_elements == 3);
	ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
	ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

	int32_t data[3];
	ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
	ufbxt_assert(data[0] == 1);
	ufbxt_assert(data[1] == 2);
	ufbxt_assert(data[2] == 3);
}
#endif

UFBXT_TEST(parse_multivalue_array_float)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"F\x00\x00\x80\x3f" "F\x00\x00\x00\x40" "F\x00\x00\x80\x40"
	);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, 'f', &arr));
	ufbxt_assert(arr.src_type == 'f');
	ufbxt_assert(arr.dst_type == 'f');
	ufbxt_assert(arr.num_elements == 3);
	ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
	ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

	float data[3];
	ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
	ufbxt_assert(data[0] == 1.0f);
	ufbxt_assert(data[1] == 2.0f);
	ufbxt_assert(data[2] == 4.0f);
}
#endif

UFBXT_TEST(parse_multivalue_convert_ints)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"C\x01"
		"Y\x02\x00"
		"I\x03\x00\x00\x00"
		"L\x04\x00\x00\x00\x00\x00\x00\x00"
	);

	{
		ufbxi_array arr;
		ufbxt_assert(ufbxi_parse_value(uc, 'b', &arr));
		ufbxt_assert(arr.src_type == 'b');
		ufbxt_assert(arr.dst_type == 'b');
		ufbxt_assert(arr.num_elements == 4);
		ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
		ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

		char data[4];
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 1);
		ufbxt_assert(data[1] == 1);
		ufbxt_assert(data[2] == 1);
		ufbxt_assert(data[3] == 1);
	}

	uc->focused_node.next_value_pos = 0;

	{
		ufbxi_array arr;
		ufbxt_assert(ufbxi_parse_value(uc, 'i', &arr));
		ufbxt_assert(arr.src_type == 'i');
		ufbxt_assert(arr.dst_type == 'i');
		ufbxt_assert(arr.num_elements == 4);
		ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
		ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

		int32_t data[4];
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 1);
		ufbxt_assert(data[1] == 2);
		ufbxt_assert(data[2] == 3);
		ufbxt_assert(data[3] == 4);
	}

	uc->focused_node.next_value_pos = 0;

	{
		ufbxi_array arr;
		ufbxt_assert(ufbxi_parse_value(uc, 'l', &arr));
		ufbxt_assert(arr.src_type == 'l');
		ufbxt_assert(arr.dst_type == 'l');
		ufbxt_assert(arr.num_elements == 4);
		ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
		ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

		int64_t data[4];
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 1);
		ufbxt_assert(data[1] == 2);
		ufbxt_assert(data[2] == 3);
		ufbxt_assert(data[3] == 4);
	}
}
#endif

UFBXT_TEST(parse_multivalue_convert_floats)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"Y\x01\x00"
		"I\x02\x00\x00\x00"
		"L\x04\x00\x00\x00\x00\x00\x00\x00"
		"F\x00\x00\x00\x41"
		"D\x00\x00\x00\x00\x00\x00\x30\x40"
	);

	{
		ufbxi_array arr;
		ufbxt_assert(ufbxi_parse_value(uc, 'f', &arr));
		ufbxt_assert(arr.src_type == 'f');
		ufbxt_assert(arr.dst_type == 'f');
		ufbxt_assert(arr.num_elements == 5);
		ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
		ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

		float data[5];
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 1.0f);
		ufbxt_assert(data[1] == 2.0f);
		ufbxt_assert(data[2] == 4.0f);
		ufbxt_assert(data[3] == 8.0f);
		ufbxt_assert(data[4] == 16.0f);
	}

	uc->focused_node.next_value_pos = 0;

	{
		ufbxi_array arr;
		ufbxt_assert(ufbxi_parse_value(uc, 'd', &arr));
		ufbxt_assert(arr.src_type == 'd');
		ufbxt_assert(arr.dst_type == 'd');
		ufbxt_assert(arr.num_elements == 5);
		ufbxt_assert(arr.encoding == ufbxi_encoding_multivalue);
		ufbxt_assert(arr.encoded_size == uc->focused_node.child_begin_pos);

		double data[5];
		ufbxt_assert(ufbxi_decode_array(uc, &arr, data));
		ufbxt_assert(data[0] == 1.0);
		ufbxt_assert(data[1] == 2.0);
		ufbxt_assert(data[2] == 4.0);
		ufbxt_assert(data[3] == 8.0);
		ufbxt_assert(data[4] == 16.0);
	}
}
#endif

UFBXT_TEST(parse_multivalue_bad_leading_type)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"S\x05\x00\x00\x00Hello"
	);

	ufbxi_array arr;
	ufbxt_assert(!ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_multivalue_bad_following_type)
#if UFBXT_IMPL
{
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x00\x00\x00"
		"S\x05\x00\x00\x00Hello"
	);

	ufbxi_array arr;
	ufbxt_assert(!ufbxi_parse_value(uc, 'i', &arr));
	ufbxt_log_error(uc);
}
#endif

UFBXT_TEST(parse_multivalue_bad_conversion)
#if UFBXT_IMPL
{
	// NOTE: The semantics of this test may change if multivalue array
	// element types are validated at parse time.
	ufbxi_context *uc = ufbxt_memory_context_values(
		"I\x01\x00\x00\x00"
		"F\x00\x00\x00\x41"
	);

	ufbxi_array arr;
	ufbxt_assert(ufbxi_parse_value(uc, 'i', &arr));

	int32_t data[1];
	ufbxt_assert(!ufbxi_decode_array(uc, &arr, data));
	ufbxt_log_error(uc);
}
#endif
