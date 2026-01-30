long int add_i64(long int __c4internal_x_c4_tmp_0,long int __c4internal_y_c4_tmp_1)
{
	return ((__c4internal_x_c4_tmp_0) + (__c4internal_y_c4_tmp_1));
}
int main()
{
	long int __c4internal_character_c4_tmp_2 = (65);
	while (((__c4internal_character_c4_tmp_2) <= (120)))
	{
		((__c4internal_character_c4_tmp_2) = ((__c4internal_character_c4_tmp_2) + (1)));
	}
	long int __c4internal_tmp_c4_tmp_3 = ((__c4internal_character_c4_tmp_2) + (17));
	((__c4internal_tmp_c4_tmp_3) = ((__c4internal_tmp_c4_tmp_3) + (89)));
	int __c4internal_ret_c4_tmp_4 = ((int)(__c4internal_character_c4_tmp_2));
	long int __c4internal_val64_c4_tmp_5 = (add_i64((200),(106)));
	return (((int)(__c4internal_val64_c4_tmp_5)) - (((int)(__c4internal_tmp_c4_tmp_3)) - (__c4internal_ret_c4_tmp_4)));
}
