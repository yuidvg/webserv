#ifndef RESULT_HPP
# define RESULT_HPP

# include <cstring>
# include <stdexcept>

// 成功したら構造体、失敗したらエラーコードを返すresultを作成する
template <typename T, typename E>
class Result
{
	private:
		T		*_value;
		E		_error;
		bool	_is_ok;

	public:
		Result();
		~Result();
		// 成功を表す静的メソッド
		static Result<T, E>	Ok(const T &value);
		// 失敗を表す静的メソッド
		static Result<T, E>	Err(E error);

		bool	ok() const;

		T		unwrap();
		E		unwrapErr();
};

# include "Result.tpp"

#endif
