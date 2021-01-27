i64 add(i64 a, i64 b) {
	i64 ret;

	ret = a + b;

	return ret;
}

i32 add_twice(i32 a, f32 b) {
	i32 ret;
	i32 c;
	i32 d;

	c = a;
	d = b;

	ret = a + b + c + d;

	return ret;
}

i64 global_var;

i32 main() {
	int b = 3 + 4;
	int c = add(3,4);
}